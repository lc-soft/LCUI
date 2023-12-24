/*
 * lib/ui-server/src/server.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <errno.h>
#include <yutil.h>
#include <pandagl.h>
#include <ui_cursor.h>
#include <ui_server.h>
#include <css/computed.h>

#ifdef LIBUI_SERVER_HAS_OPENMP
#include <omp.h>
#endif

#define TITLE_MAX_SIZE 256

typedef struct window_mutation_record_t {
        app_window_t *window;
        bool update_size;
        bool update_position;
        bool update_title;
        bool update_visible;
        int x;
        int y;
        int width;
        int height;
        wchar_t title[TITLE_MAX_SIZE];
        bool visible;
} window_mutation_record_t;

typedef struct ui_flash_rect_t {
        int64_t paint_time;
        pd_rect_t rect;
} ui_flash_rect_t;

typedef struct ui_dirty_layer_t {
        list_t rects;
        pd_rect_t rect;
        int dirty;
} ui_dirty_layer_t;

typedef struct ui_connection_t {
        unsigned id;

        bool active;

        /** widget is ready to sync data from window */
        bool ready;

        /** whether new content has been rendered */
        bool rendered;

        /** flashing rect list */
        list_t flash_rects;

        bool window_visible;
        app_window_t *window;
        ui_widget_t *widget;
} ui_connection_t;

typedef struct ui_connection_group_t {
        ui_widget_t *widget;

        /**
         * list_t<ui_connection_t>
         * */
        list_t connections;
} ui_connection_group_t;

static struct ui_server_t {
        /** list_t<ui_connection_t> */
        list_t connections;
        ui_mutation_observer_t *observer;
        bool paint_flashing_enabled;
        int rendering_threads;
} ui_server;

static inline int is_rect_equals(const pd_rect_t *a, const pd_rect_t *b)
{
        return a->x == b->x && a->y == b->y && a->width == b->width &&
               a->height == b->height;
}

// TODO: 处理单部件对多窗口的情况
static ui_connection_t *ui_server_find_connection(ui_widget_t *widget,
                                                  app_window_t *window)
{
        ui_connection_t *conn;
        list_node_t *node;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (!conn) {
                        continue;
                }
                if ((window && conn->window == window) ||
                    (widget && conn->widget == widget)) {
                        return conn;
                }
        }
        return NULL;
}

ui_widget_t *ui_server_get_widget(app_window_t *window)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(NULL, window);
        return conn && conn->active ? conn->widget : NULL;
}

app_window_t *ui_server_get_window(ui_widget_t *widget)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(widget, NULL);
        return conn && conn->active ? conn->window : NULL;
}

static void ui_connection_destroy(void *arg)
{
        ui_connection_t *conn = arg;

        list_destroy(&conn->flash_rects, free);
        free(conn);
}

int ui_server_disconnect(ui_widget_t *widget, app_window_t *window)
{
        int count = 0;
        ui_connection_t *conn;
        list_node_t *node;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if ((widget && conn->widget == widget) ||
                    (window && conn->window == window)) {
                        conn->active = false;
                        app_window_close(conn->window);
                        count++;
                }
        }
        return count;
}

static void ui_server_on_window_close(app_event_t *e, void *arg)
{
        ui_connection_t *conn;
        list_node_t *node;

        logger_debug("[ui-server] on close window %p\n", e->window);
        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (conn->window == e->window) {
                        app_window_destroy(conn->window);
                        ui_connection_destroy(conn);
                        list_delete_node(&ui_server.connections, node);
                        break;
                }
        }
        if (ui_server.connections.length < 1) {
                app_exit(0);
        }
}

static void ui_server_on_window_visibility_change(app_event_t *e, void *arg)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(NULL, e->window);
        if (conn && conn->active) {
                conn->window_visible = e->visibility_change.visible;
                if (conn->window_visible) {
                        ui_widget_show(conn->widget);
                } else {
                        ui_widget_hide(conn->widget);
                }
        }
}

static void ui_server_on_window_paint(app_event_t *e, void *arg)
{
        ui_rect_t rect;
        list_node_t *node;
        ui_connection_t *conn;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (conn && conn->window != e->window) {
                        continue;
                }
                ui_convert_rect(&e->paint.rect, &rect, ui_metrics.scale);
                ui_widget_mark_dirty_rect(conn->widget, &rect,
                                          UI_BOX_TYPE_GRAPH_BOX);
        }
}

static void ui_server_on_window_resize(app_event_t *e, void *arg)
{
        ui_connection_t *conn;
        pd_rect_t rect;
        float scale = ui_metrics.scale;
        float width = e->size.width / scale;
        float height = e->size.height / scale;

        conn = ui_server_find_connection(NULL, e->window);
        if (!conn || !conn->active) {
                return;
        }
        if (!conn->ready) {
                ui_compute_rect(&rect, &conn->widget->canvas_box);
                if (rect.width > 0 && rect.height > 0 &&
                    rect.width == e->size.width &&
                    rect.height == e->size.height) {
                        logger_debug("[ui-server][connection %u] window ready, "
                                     "rect: %d,%d,%d,%d\n",
                                     conn->id, rect.x, rect.y, rect.width,
                                     rect.height);
                        conn->ready = true;
                } else {
                        return;
                }
        }
        ui_widget_mark_dirty_rect(conn->widget, NULL, UI_BOX_TYPE_GRAPH_BOX);
        ui_widget_resize(conn->widget, width, height);
        logger_debug("[ui-server][connection %u] on_window_resize, widget: "
                     "(%p, %s), size "
                     "(%g, %g)\n",
                     conn->id, conn->widget, conn->widget->type, width, height);
}

static void ui_server_on_window_minmaxinfo(app_event_t *e, void *arg)
{
        bool resizable = FALSE;
        int width, height;
        ui_widget_t *widget;
        css_computed_style_t *style;

        if (!e->window) {
                return;
        }
        widget = ui_server_get_widget(e->window);
        if (!widget) {
                return;
        }
        width = app_window_get_width(e->window);
        height = app_window_get_height(e->window);
        style = &widget->computed_style;
        if (IS_CSS_FIXED_LENGTH(style, min_width)) {
                e->minmaxinfo.min_width = ui_compute(style->min_width);
                if (width < e->minmaxinfo.min_width) {
                        width = e->minmaxinfo.min_width;
                        resizable = TRUE;
                }
        }
        if (IS_CSS_FIXED_LENGTH(style, max_width)) {
                e->minmaxinfo.max_width = ui_compute(style->max_width);
                if (width > e->minmaxinfo.max_width) {
                        width = e->minmaxinfo.max_width;
                        resizable = TRUE;
                }
        }
        if (IS_CSS_FIXED_LENGTH(style, min_height)) {
                e->minmaxinfo.min_height = ui_compute(style->min_height);
                if (height < e->minmaxinfo.min_height) {
                        height = e->minmaxinfo.min_height;
                        resizable = TRUE;
                }
        }
        if (IS_CSS_FIXED_LENGTH(style, max_height)) {
                e->minmaxinfo.max_height = ui_compute(style->max_height);
                if (height > e->minmaxinfo.max_height) {
                        height = e->minmaxinfo.max_height;
                        resizable = TRUE;
                }
        }
        if (resizable) {
                app_window_set_size(e->window, width, height);
        }
}

static void ui_server_on_destroy_widget(ui_widget_t *widget, ui_event_t *e,
                                        void *arg)
{
        ui_server_disconnect(widget, NULL);
}

static void ui_server_refresh_window(ui_connection_t *conn)
{
        pd_rect_t rect;

        logger_debug("[ui-server][connection %u] refresh window: %p\n",
                     conn->id, conn->window);
        ui_widget_mark_dirty_rect(conn->widget, NULL, UI_BOX_TYPE_GRAPH_BOX);
        ui_compute_rect(&rect, &conn->widget->canvas_box);
        app_window_set_title(conn->window, conn->widget->title);
        app_window_set_size(conn->window, rect.width, rect.height);
        if (conn->widget->computed_style.type_bits.top == CSS_TOP_AUTO) {
                rect.y = (app_get_screen_height() - rect.height) / 2;
        }
        if (conn->widget->computed_style.type_bits.left == CSS_LEFT_AUTO) {
                rect.x = (app_get_screen_width() - rect.width) / 2;
        }
        app_window_set_position(conn->window, rect.x, rect.y);
        conn->window_visible = ui_widget_is_visible(conn->widget);
        if (conn->window_visible) {
                app_window_show(conn->window);
        } else {
                app_window_hide(conn->window);
        }
        app_window_activate(conn->window);
}

static void ui_server_on_widget_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_connection_t *conn;

        // 考虑到连接可能会在组件 ready 事件之前被销毁，
        // 所以从连接列表中查找以确认连接是否存在
        conn = ui_server_find_connection(w, NULL);
        if (conn) {
                ui_server_refresh_window(conn);
        }
}

void ui_server_connect(ui_widget_t *widget, app_window_t *window)
{
        static unsigned id = 1;
        ui_connection_t *conn;
        ui_mutation_observer_init_t options = { 0 };

        conn = malloc(sizeof(ui_connection_t));
        conn->id = id++;
        conn->active = true;
        conn->ready = false;
        conn->window = window;
        conn->widget = widget;
        conn->rendered = false;
        conn->window_visible = false;
        options.properties = TRUE;
        list_create(&conn->flash_rects);
        ui_widget_on(widget, "destroy", ui_server_on_destroy_widget, NULL);
        ui_mutation_observer_observe(ui_server.observer, widget, options);
        list_append(&ui_server.connections, conn);
        if (widget->state < UI_WIDGET_STATE_NORMAL) {
                ui_widget_on(widget, "ready", ui_server_on_widget_ready, NULL);
        } else {
                ui_server_refresh_window(conn);
        }
        logger_debug("[ui-server][connection %u] connected: widget(%p, %s) <=> "
                     "window(%p)\n",
                     conn->id, widget, widget->type, window);
}

static void get_rendering_layer_size(int *width, int *height)
{
        float scale = ui_metrics.scale;

        *width = (int)(app_get_screen_width() * scale);
        *height = (int)(app_get_screen_height() * scale);
        *height = y_max(200, *height / ui_server.rendering_threads + 1);
}

static int ui_server_dump_rects(ui_widget_t *w, pd_rect_t **out_rects)
{
        int i = 0;
        int max_dirty;
        int layer_width;
        int layer_height;

        pd_rect_t rect;
        pd_rect_t *sub_rect;
        ui_dirty_layer_t *layer;
        ui_dirty_layer_t *layers;
        list_node_t *node;
        list_t widget_rects;
        list_t rects;

        list_create(&widget_rects);
        list_create(&rects);
        ui_widget_get_dirty_rects(w, &widget_rects);
        get_rendering_layer_size(&layer_width, &layer_height);
        max_dirty = (int)(0.8 * layer_width * layer_height);
        layers = malloc(sizeof(ui_dirty_layer_t) * ui_server.rendering_threads);
        for (i = 0; i < ui_server.rendering_threads; ++i) {
                layer = &layers[i];
                layer->dirty = 0;
                layer->rect.y = i * layer_height;
                layer->rect.x = 0;
                layer->rect.width = layer_width;
                layer->rect.height = layer_height;
                list_create(&layer->rects);
        }
        sub_rect = malloc(sizeof(pd_rect_t));
        for (list_each(node, &widget_rects)) {
                rect = *(pd_rect_t *)node->data;
                for (i = 0; i < ui_server.rendering_threads; ++i) {
                        layer = &layers[i];
                        if (layer->dirty >= max_dirty) {
                                continue;
                        }
                        if (!pd_rect_overlap(&layer->rect, &rect, sub_rect)) {
                                continue;
                        }
                        list_append(&layer->rects, sub_rect);
                        rect.y += sub_rect->height;
                        rect.height -= sub_rect->height;
                        layer->dirty += sub_rect->width * sub_rect->height;
                        sub_rect = malloc(sizeof(pd_rect_t));
                        if (rect.height < 1) {
                                break;
                        }
                }
        }
        free(sub_rect);
        for (i = 0; i < ui_server.rendering_threads; ++i) {
                layer = &layers[i];
                if (layer->dirty >= max_dirty) {
                        sub_rect = malloc(sizeof(pd_rect_t));
                        *sub_rect = layer->rect;
                        list_append(&rects, sub_rect);
                        list_destroy(&layer->rects, free);
                } else {
                        list_concat(&rects, &layer->rects);
                }
        }
        i = 0;
        *out_rects = (pd_rect_t *)malloc(sizeof(pd_rect_t) * rects.length);
        for (list_each(node, &rects)) {
                (*out_rects)[i] = *((pd_rect_t *)node->data);
                i++;
        }
        pd_rects_clear(&widget_rects);
        pd_rects_clear(&rects);
        free(layers);
        return i;
}

static size_t ui_server_render_flash_rect(ui_connection_t *conn,
                                          ui_flash_rect_t *flash_rect)
{
        size_t count;
        int64_t period;
        float duration = 1000;

        pd_pos_t pos;
        pd_color_t color;
        pd_canvas_t mask;
        pd_context_t *paint;

        paint = app_window_begin_paint(conn->window, &flash_rect->rect);
        if (!paint) {
                return 0;
        }
        period = get_time_delta(flash_rect->paint_time);
        count = ui_widget_render(conn->widget, paint);
        ui_cursor_paint(conn->window, paint);
        if (period >= duration) {
                flash_rect->paint_time = 0;
                app_window_end_paint(conn->window, paint);
                return count;
        }
        pd_canvas_init(&mask);
        mask.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&mask, flash_rect->rect.width,
                         flash_rect->rect.height);
        pd_canvas_fill(&mask, pd_argb(125, 124, 179, 5));
        mask.opacity = 0.6f * (duration - (float)period) / duration;
        pos.x = pos.y = 0;
        color = pd_rgb(124, 179, 5);
        pd_canvas_draw_hline(&mask, color, 1, pos, mask.width - 1);
        pd_canvas_draw_vline(&mask, color, 1, pos, mask.height - 1);
        pos.x = mask.width - 1;
        pd_canvas_draw_vline(&mask, color, 1, pos, mask.height - 1);
        pos.x = 0;
        pos.y = mask.height - 1;
        pd_canvas_draw_hline(&mask, color, 1, pos, mask.width - 1);
        pd_canvas_mix(&paint->canvas, &mask, 0, 0, TRUE);
        pd_canvas_destroy(&mask);
        app_window_end_paint(conn->window, paint);
        return count;
}

static void ui_server_update_flash_rects(ui_connection_t *conn)
{
        ui_flash_rect_t *flash_rect;
        list_node_t *node, *prev;

        for (list_each(node, &conn->flash_rects)) {
                flash_rect = node->data;
                if (flash_rect->paint_time == 0) {
                        prev = node->prev;
                        free(node->data);
                        list_delete_node(&conn->flash_rects, node);
                        node = prev;
                        continue;
                }
                ui_server_render_flash_rect(conn, flash_rect);
                conn->rendered = TRUE;
        }
}

static void ui_server_add_flash_rect(ui_connection_t *conn, pd_rect_t *rect)
{
        list_node_t *node;
        ui_flash_rect_t *flash_rect;

        for (list_each(node, &conn->flash_rects)) {
                flash_rect = node->data;
                if (is_rect_equals(&flash_rect->rect, rect)) {
                        flash_rect->paint_time = get_time_ms();
                        return;
                }
        }

        flash_rect = malloc(sizeof(ui_flash_rect_t));
        flash_rect->rect = *rect;
        flash_rect->paint_time = get_time_ms();
        list_append(&conn->flash_rects, flash_rect);
}

static void ui_server_render_rect(ui_connection_t *conn, pd_rect_t *rect)
{
        pd_context_t *paint;

        paint = app_window_begin_paint(conn->window, rect);
        if (!paint) {
                return;
        }
        // logger_debug("[ui-server][connection %u] render rect: (%d,%d,%d,%d)\n",
        //              conn->id, paint->rect.x, paint->rect.y, paint->rect.width,
        //              paint->rect.height);
        ui_widget_render(conn->widget, paint);
        if (ui_server.paint_flashing_enabled) {
                ui_server_add_flash_rect(conn, &paint->rect);
        }
        app_window_end_paint(conn->window, paint);
}

static void ui_server_render_window(ui_connection_t *conn, pd_rect_t *rects,
                                    int rects_len, bool openmp)
{
        int i;

        if (openmp) {
#ifdef ENABLE_OPENMP
#pragma omp parallel for default(none) shared(ui_server, rects, rects_len) \
    firstprivate(conn)
#endif
                for (i = 0; i < rects_len; ++i) {
                        ui_server_render_rect(conn, rects + i);
                }
        } else {
                for (i = 0; i < rects_len; ++i) {
                        ui_server_render_rect(conn, rects + i);
                }
        }
        conn->rendered = true;
}

void ui_server_render(void)
{
        int i;
        int rects_len;
        int dirty = 0;
        int layer_width;
        int layer_height;
        list_t groups;
        pd_rect_t *rects;
        list_node_t *conn_node;
        list_node_t *group_node;
        ui_connection_group_t *group;
        ui_connection_t *conn;

        list_create(&groups);
        get_rendering_layer_size(&layer_width, &layer_height);
        for (list_each(conn_node, &ui_server.connections)) {
                conn = conn_node->data;
                if (!conn->ready || !conn->window_visible) {
                        continue;
                }
                group = NULL;
                for (list_each(group_node, &groups)) {
                        group = group_node->data;
                        if (group->widget == conn->widget) {
                                break;
                        }
                        group = NULL;
                }
                if (!group) {
                        group = malloc(sizeof(ui_connection_group_t));
                        group->widget = conn->widget;
                        list_create(&group->connections);
                        list_append(&groups, group);
                }
                list_append(&group->connections, conn);
        }
        for (list_each(group_node, &groups)) {
                group = group_node->data;
                rects_len = ui_server_dump_rects(group->widget, &rects);
                if (rects_len < 1) {
                        continue;
                }
                for (i = 0; i < rects_len; ++i) {
                        dirty += rects[i].width * rects[i].height;
                }
                for (list_each(conn_node, &group->connections)) {
                        // Use OPENMP if the render area is larger than two
                        // render layers
                        ui_server_render_window(
                            conn_node->data, rects, rects_len,
                            dirty >= layer_width * layer_height * 2);
                        ui_server_update_flash_rects(conn_node->data);
                }
                list_destroy(&group->connections, NULL);
                free(rects);
        }
        list_destroy(&groups, free);
}

static int window_mutation_list_add(list_t *list,
                                    ui_mutation_record_t *mutation)
{
        list_node_t *node;
        app_window_t *wnd;
        ui_widget_t *widget = mutation->target;
        window_mutation_record_t *wnd_mutation = NULL;

        wnd = ui_server_get_window(widget);
        if (!wnd) {
                return -1;
        }
        for (list_each(node, list)) {
                wnd_mutation = node->data;
                if (wnd_mutation->window == wnd) {
                        break;
                }
                wnd_mutation = NULL;
        }
        if (!wnd_mutation) {
                wnd_mutation = calloc(1, sizeof(window_mutation_record_t));
                if (!wnd_mutation) {
                        return -ENOMEM;
                }
                wnd_mutation->window = wnd;
                list_append(list, wnd_mutation);
        }
        wnd_mutation->update_title = wnd_mutation->update_title ||
                                     strcmp(mutation->property_name, "title");
        wnd_mutation->update_visible =
            wnd_mutation->update_visible ||
            strcmp(mutation->property_name, "visible");
        wnd_mutation->update_position =
            wnd_mutation->update_position ||
            strcmp(mutation->property_name, "x") == 0 ||
            strcmp(mutation->property_name, "y") == 0;
        wnd_mutation->update_size =
            wnd_mutation->update_size ||
            strcmp(mutation->property_name, "width") == 0 ||
            strcmp(mutation->property_name, "height") == 0;
        wcsncpy(wnd_mutation->title, widget->title ? widget->title : L"\0",
                TITLE_MAX_SIZE);
        wnd_mutation->visible = ui_widget_is_visible(widget);
        wnd_mutation->x = ui_compute(widget->border_box.x);
        wnd_mutation->y = ui_compute(widget->border_box.y);
        wnd_mutation->width = ui_compute(widget->border_box.width);
        wnd_mutation->height = ui_compute(widget->border_box.height);
        return 0;
}

static void ui_server_on_widget_mutation(ui_mutation_list_t *mutation_list,
                                         ui_mutation_observer_t *observer,
                                         void *arg)
{
        list_node_t *node;
        list_t wnd_mutation_list;
        window_mutation_record_t *wnd_mutation;
        ui_mutation_record_t *mutation;

        list_create(&wnd_mutation_list);
        for (list_each(node, mutation_list)) {
                mutation = node->data;
                if (mutation->type != UI_MUTATION_RECORD_TYPE_PROPERTIES ||
                    !mutation->property_name) {
                        continue;
                }
                logger_debug(
                    "[ui-server][observer] on_widget_mutation, type: %d\n",
                    mutation->type);
                window_mutation_list_add(&wnd_mutation_list, mutation);
        }
        for (list_each(node, &wnd_mutation_list)) {
                wnd_mutation = node->data;
                if (wnd_mutation->update_position) {
                        app_window_set_position(wnd_mutation->window,
                                                wnd_mutation->x,
                                                wnd_mutation->y);
                        logger_debug("[ui-server][observer] update position, "
                                     "window: %p, "
                                     "position: (%d, %d)\n",
                                     wnd_mutation->window, wnd_mutation->x,
                                     wnd_mutation->y);
                }
                if (wnd_mutation->update_size) {
                        app_window_set_size(wnd_mutation->window,
                                            wnd_mutation->width,
                                            wnd_mutation->height);
                        logger_debug(
                            "[ui-server][observer] update size, window: %p, "
                            "size: (%d, %d)\n",
                            wnd_mutation->window, wnd_mutation->width,
                            wnd_mutation->height);
                }
                if (wnd_mutation->update_title) {
                        app_window_set_title(wnd_mutation->window,
                                             wnd_mutation->title);
                        logger_debug(
                            "[ui-server][observer] update title, window: %p, "
                            "title: %ls\n",
                            wnd_mutation->window, wnd_mutation->title);
                }
                if (wnd_mutation->update_visible) {
                        logger_debug(
                            "[ui-server][observer] update visible, window: %p, "
                            "visible %d\n",
                            wnd_mutation->window, wnd_mutation->visible);
                        if (wnd_mutation->visible) {
                                app_window_show(wnd_mutation->window);
                        } else {
                                app_window_hide(wnd_mutation->window);
                        }
                }
        }
        list_destroy(&wnd_mutation_list, free);
}

void ui_server_init(void)
{
        ui_cursor_init();
        ui_server.observer =
            ui_mutation_observer_create(ui_server_on_widget_mutation, NULL);
        app_on_event(APP_EVENT_VISIBILITY_CHANGE,
                     ui_server_on_window_visibility_change, NULL);
        app_on_event(APP_EVENT_MINMAXINFO, ui_server_on_window_minmaxinfo,
                     NULL);
        app_on_event(APP_EVENT_SIZE, ui_server_on_window_resize, NULL);
        app_on_event(APP_EVENT_CLOSE, ui_server_on_window_close, NULL);
        app_on_event(APP_EVENT_PAINT, ui_server_on_window_paint, NULL);
        list_create(&ui_server.connections);
        switch (app_get_id()) {
        case APP_ID_LINUX_X11:
        case APP_ID_UWP:
        case APP_ID_WIN32:
                ui_cursor_hide();
                break;
        default:
                break;
        }
}

void ui_server_set_threads(int threads)
{
        ui_server.rendering_threads = threads;
}

void ui_server_set_paint_flashing_enabled(bool enabled)
{
        ui_server.paint_flashing_enabled = enabled;
}

void ui_server_destroy(void)
{
        ui_cursor_destroy();
        app_off_event(APP_EVENT_MINMAXINFO, ui_server_on_window_minmaxinfo);
        app_off_event(APP_EVENT_SIZE, ui_server_on_window_resize);
        app_off_event(APP_EVENT_CLOSE, ui_server_on_window_close);
        app_off_event(APP_EVENT_VISIBILITY_CHANGE,
                      ui_server_on_window_visibility_change);
        list_destroy(&ui_server.connections, ui_connection_destroy);
        ui_mutation_observer_destroy(ui_server.observer);
        ui_server.observer = NULL;
}
