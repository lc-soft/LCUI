/*
 * lib/ui-server/src/server.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
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

typedef struct ui_flash_rect {
        int64_t paint_time;
        pd_rect_t rect;
} ui_flash_rect_t;

typedef struct ui_dirty_layer {
        list_t rects;
        pd_rect_t rect;
        int dirty;
} ui_dirty_layer_t;

typedef enum {
        UI_CONNECTION_STATE_PENDING,
        UI_CONNECTION_STATE_INITIALIZING,
        UI_CONNECTION_STATE_INITIALIZED,
} ui_connection_state_t;

typedef struct ui_connection {
        ui_connection_state_t state;

        /** flashing rect list */
        list_t flash_rects;

        bool window_visible;
        ptk_window_t *window;
        ui_widget_t *widget;
        ui_updater_t *updater;
} ui_connection_t;

static struct ui_server {
        /** list_t<ui_connection_t> */
        list_t connections;
        bool paint_flashing_enabled;
        int num_rendering_threads;
} ui_server;

/* css_cursor_t 值序到 ptk_cursor_type_t 的映射（值序 1:1，显式表防漂移） */
static const ptk_cursor_type_t cursor_map[CSS_CURSOR_TOTAL] = {
        PTK_CURSOR_AUTO,        PTK_CURSOR_DEFAULT,
        PTK_CURSOR_NONE,        PTK_CURSOR_CONTEXT_MENU,
        PTK_CURSOR_HELP,        PTK_CURSOR_POINTER,
        PTK_CURSOR_PROGRESS,    PTK_CURSOR_WAIT,
        PTK_CURSOR_CELL,        PTK_CURSOR_CROSSHAIR,
        PTK_CURSOR_TEXT,        PTK_CURSOR_VERTICAL_TEXT,
        PTK_CURSOR_ALIAS,       PTK_CURSOR_COPY,
        PTK_CURSOR_MOVE,        PTK_CURSOR_NO_DROP,
        PTK_CURSOR_NOT_ALLOWED, PTK_CURSOR_GRAB,
        PTK_CURSOR_GRABBING,    PTK_CURSOR_ALL_SCROLL,
        PTK_CURSOR_COL_RESIZE,  PTK_CURSOR_ROW_RESIZE,
        PTK_CURSOR_N_RESIZE,    PTK_CURSOR_E_RESIZE,
        PTK_CURSOR_S_RESIZE,    PTK_CURSOR_W_RESIZE,
        PTK_CURSOR_EW_RESIZE,   PTK_CURSOR_NS_RESIZE,
        PTK_CURSOR_NE_RESIZE,   PTK_CURSOR_NW_RESIZE,
        PTK_CURSOR_SE_RESIZE,   PTK_CURSOR_SW_RESIZE,
        PTK_CURSOR_NESW_RESIZE, PTK_CURSOR_NWSE_RESIZE,
        PTK_CURSOR_ZOOM_IN,     PTK_CURSOR_ZOOM_OUT,
};

/* hover 目标悬空（部件销毁）时用于回写 AUTO 的窗口；随连接销毁置空 */
static ptk_window_t *last_synced_cursor_window;

static inline int is_rect_equals(const pd_rect_t *a, const pd_rect_t *b)
{
        return a->x == b->x && a->y == b->y && a->width == b->width &&
               a->height == b->height;
}

static ui_connection_t *ui_server_find_connection(ui_widget_t *widget,
                                                  ptk_window_t *window)
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

ui_widget_t *ui_server_get_widget(ptk_window_t *window)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(NULL, window);
        return conn ? conn->widget : NULL;
}

ptk_window_t *ui_server_get_window(ui_widget_t *widget)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(widget, NULL);
        return conn ? conn->window : NULL;
}

bool ui_server_is_widget_ready(ui_widget_t *widget)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(widget, NULL);
        return conn && conn->state != UI_CONNECTION_STATE_PENDING;
}

static void ui_connection_destroy(void *arg)
{
        ui_connection_t *conn = arg;

        if (last_synced_cursor_window == conn->window) {
                last_synced_cursor_window = NULL;
        }
        list_destroy(&conn->flash_rects, free);
        ui_updater_destroy(conn->updater);
        free(conn);
}

int ui_server_disconnect(ui_widget_t *widget, ptk_window_t *window)
{
        int count = 0;
        ui_connection_t *conn;
        list_node_t *node;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if ((widget && conn->widget == widget) ||
                    (window && conn->window == window)) {
                        ptk_window_close(conn->window);
                        count++;
                }
        }
        return count;
}

float ui_server_get_window_scale(ptk_window_t *window)
{
        ui_connection_t *conn = ui_server_find_connection(NULL, window);
        if (conn) {
                return css_metrics_actual_scale(&conn->updater->metrics);
        }
        if (window) {
                return ptk_window_get_dpi(window) / 96.f;
        }
        return 1.f;
}

static void ui_server_on_window_close(ptk_event_t *e, void *arg)
{
        ui_connection_t *conn;
        list_node_t *node;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (conn->window == e->window) {
                        ui_connection_destroy(conn);
                        list_delete_node(&ui_server.connections, node);
                        break;
                }
        }
}

static void ui_server_on_window_visibility_change(ptk_event_t *e, void *arg)
{
        ui_connection_t *conn;

        conn = ui_server_find_connection(NULL, e->window);
        if (conn && conn->state == UI_CONNECTION_STATE_INITIALIZED) {
                conn->window_visible = e->visibility_change.visible;
        }
}

static void ui_server_on_window_paint(ptk_event_t *e, void *arg)
{
        ui_rect_t rect;
        list_node_t *node;
        ui_connection_t *conn;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (conn && conn->window != e->window) {
                        continue;
                }
                ui_rect_from_pd_rect(
                    &rect, &e->paint.rect,
                    css_metrics_actual_scale(&conn->updater->metrics));
                ui_widget_mark_dirty_rect(conn->widget, &rect,
                                          UI_BOX_TYPE_GRAPH_BOX);
        }
}

static void ui_server_on_window_resize(ptk_event_t *e, void *arg)
{
        ui_connection_t *conn;
        float scale, width, height;

        conn = ui_server_find_connection(NULL, e->window);
        if (!conn || conn->state != UI_CONNECTION_STATE_INITIALIZED) {
                return;
        }
        scale = css_metrics_actual_scale(&conn->updater->metrics);
        width = e->size.width / scale;
        height = e->size.height / scale;
        ui_widget_mark_dirty_rect(conn->widget, NULL, UI_BOX_TYPE_GRAPH_BOX);
        ui_widget_resize(conn->widget, width, height);
}

static void ui_server_on_window_dpi_changed(ptk_event_t *e, void *arg)
{
        ui_refresh_style();
}

static void ui_server_on_window_minmaxinfo(ptk_event_t *e, void *arg)
{
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
        width = ptk_window_get_width(e->window);
        height = ptk_window_get_height(e->window);
        style = &widget->computed_style;
        if (IS_CSS_FIXED_LENGTH(style, min_width)) {
                e->minmaxinfo.min_width = ui_compute(style->min_width);
                if (width < e->minmaxinfo.min_width) {
                        width = e->minmaxinfo.min_width;
                }
        }
        if (IS_CSS_FIXED_LENGTH(style, max_width)) {
                e->minmaxinfo.max_width = ui_compute(style->max_width);
                if (width > e->minmaxinfo.max_width) {
                        width = e->minmaxinfo.max_width;
                }
        }
        if (IS_CSS_FIXED_LENGTH(style, min_height)) {
                e->minmaxinfo.min_height = ui_compute(style->min_height);
                if (height < e->minmaxinfo.min_height) {
                        height = e->minmaxinfo.min_height;
                }
        }
        if (IS_CSS_FIXED_LENGTH(style, max_height)) {
                e->minmaxinfo.max_height = ui_compute(style->max_height);
                if (height > e->minmaxinfo.max_height) {
                        height = e->minmaxinfo.max_height;
                }
        }
}

static void ui_server_on_destroy_widget(ui_widget_t *widget, ui_event_t *e,
                                        void *arg)
{
        /* The widget's owner keeps ownership; destroying it only unbinds the
         * connection, it must not close the platform window. */
        ui_server_detach_widget(widget);
}

static void ui_server_refresh_window(ui_connection_t *conn)
{
        float scale = css_metrics_actual_scale(&conn->updater->metrics);

        ui_widget_mark_dirty_rect(conn->widget, NULL, UI_BOX_TYPE_GRAPH_BOX);
        ui_widget_resize(conn->widget,
                         ptk_window_get_width(conn->window) / scale,
                         ptk_window_get_height(conn->window) / scale);
}

void ui_server_connect(ui_widget_t *widget, ptk_window_t *window)
{
        ui_connection_t *conn;

        conn = malloc(sizeof(ui_connection_t));
        conn->window = window;
        conn->widget = widget;
        conn->updater = ui_updater_create();
        conn->updater->metrics.dpi = 1.f * ptk_window_get_dpi(window);
        conn->state = UI_CONNECTION_STATE_PENDING;
        conn->window_visible = ui_widget_is_visible(widget);
        list_create(&conn->flash_rects);
        list_append(&ui_server.connections, conn);
        ui_widget_on(widget, "destroy", ui_server_on_destroy_widget, NULL);
        // 在同步前处理所有消息，包括窗口位置、大小变更消息，避免在初次更新时组件的位置和尺寸被窗口覆盖
        ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
}

void ui_server_detach_widget(ui_widget_t *widget)
{
        ui_connection_t *conn;
        list_node_t *node;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (conn->widget == widget) {
                        ui_widget_off(widget, "destroy",
                                      ui_server_on_destroy_widget, NULL);
                        ui_connection_destroy(conn);
                        list_delete_node(&ui_server.connections, node);
                        break;
                }
        }
}

static void get_rendering_layer_size(int *width, int *height)
{
        *width = (int)ptk_screen_get_width();
        *height =
            (int)ptk_screen_get_height() / ui_server.num_rendering_threads + 1;
        if (*height < 200) {
                *height = 200;
        }
}

static void ui_server_dump_rects(ui_connection_t *conn, list_t *out_rects)
{
        int i;
        int max_dirty;
        int layer_width;
        int layer_height;

        pd_rect_t rect;
        pd_rect_t *sub_rect;
        ui_dirty_layer_t *layer;
        ui_dirty_layer_t *layers;
        list_node_t *node;
        list_t rects;

        list_create(&rects);
        ui_widget_get_dirty_rects(conn->widget, &rects);
        get_rendering_layer_size(&layer_width, &layer_height);
        max_dirty = (int)(0.8 * layer_width * layer_height);
        layers =
            malloc(sizeof(ui_dirty_layer_t) * ui_server.num_rendering_threads);
        for (i = 0; i < ui_server.num_rendering_threads; ++i) {
                layer = &layers[i];
                layer->dirty = 0;
                layer->rect.y = i * layer_height;
                layer->rect.x = 0;
                layer->rect.width = layer_width;
                layer->rect.height = layer_height;
                list_create(&layer->rects);
        }
        sub_rect = malloc(sizeof(pd_rect_t));
        for (list_each(node, &rects)) {
                rect = *(pd_rect_t *)node->data;
                for (i = 0; i < ui_server.num_rendering_threads; ++i) {
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
        for (i = 0; i < ui_server.num_rendering_threads; ++i) {
                layer = &layers[i];
                if (layer->dirty >= max_dirty) {
                        sub_rect = malloc(sizeof(pd_rect_t));
                        *sub_rect = layer->rect;
                        list_append(out_rects, sub_rect);
                        list_destroy(&layer->rects, free);
                } else {
                        list_concat(out_rects, &layer->rects);
                }
        }
        list_destroy(&rects, free);
        free(layers);
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

        paint = ptk_window_begin_paint(conn->window, &flash_rect->rect);
        if (!paint) {
                return 0;
        }
        period = (y_gettime() - flash_rect->paint_time);
        count = ui_widget_render(conn->widget, paint);
        ui_cursor_paint(conn->window, paint);
        if (period >= duration) {
                flash_rect->paint_time = 0;
                ptk_window_end_paint(conn->window, paint);
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
        pd_canvas_mix(&paint->canvas, &mask, 0, 0, true);
        pd_canvas_destroy(&mask);
        ptk_window_end_paint(conn->window, paint);
        return count;
}

static size_t ui_server_update_flash_rects(ui_connection_t *conn)
{
        size_t count = 0;
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
        }
        return count;
}

static void ui_server_add_flash_rect(ui_connection_t *conn, pd_rect_t *rect)
{
        list_node_t *node;
        ui_flash_rect_t *flash_rect;

        for (list_each(node, &conn->flash_rects)) {
                flash_rect = node->data;
                if (is_rect_equals(&flash_rect->rect, rect)) {
                        flash_rect->paint_time = y_gettime();
                        return;
                }
        }

        flash_rect = malloc(sizeof(ui_flash_rect_t));
        flash_rect->rect = *rect;
        flash_rect->paint_time = y_gettime();
        list_append(&conn->flash_rects, flash_rect);
}

static size_t ui_server_render_rect(ui_connection_t *conn, pd_rect_t *rect)
{
        size_t count;
        pd_context_t *paint;

        if (!conn->widget || !conn->window) {
                return 0;
        }
        paint = ptk_window_begin_paint(conn->window, rect);
        if (!paint) {
                return 0;
        }
        DEBUG_MSG("[thread %d/%d] rect: (%d,%d,%d,%d)\n", omp_get_thread_num(),
                  omp_get_num_threads(), paint->rect.x, paint->rect.y,
                  paint->rect.width, paint->rect.height);
        count = ui_widget_render(conn->widget, paint);
        if (ui_server.paint_flashing_enabled) {
                ui_server_add_flash_rect(conn, &paint->rect);
        }
        ptk_window_end_paint(conn->window, paint);
        return count;
}

static size_t ui_server_render_window(ui_connection_t *conn)
{
        int i = 0;
        int dirty = 0;
        int layer_width;
        int layer_height;
        size_t count = 0;
        pd_rect_t **rect_array;
        list_t rects;
        list_node_t *node;

        list_create(&rects);
        get_rendering_layer_size(&layer_width, &layer_height);
        ui_server_dump_rects(conn, &rects);
        if (rects.length < 1) {
                return 0;
        }
        rect_array = (pd_rect_t **)malloc(sizeof(pd_rect_t *) * rects.length);
        for (list_each(node, &rects)) {
                rect_array[i] = node->data;
                dirty += rect_array[i]->width * rect_array[i]->height;
                i++;
        }
        // Use OPENMP if the render area is larger than two render layers
        if (dirty >= layer_width * layer_height * 2) {
#ifdef ENABLE_OPENMP
#pragma omp parallel for default(none) shared(ui_server, rects, rect_array) \
    firstprivate(conn) reduction(+ : count)
#endif
                for (i = 0; i < (int)rects.length; ++i) {
                        count += ui_server_render_rect(conn, rect_array[i]);
                }
        } else {
                for (i = 0; i < (int)rects.length; ++i) {
                        count += ui_server_render_rect(conn, rect_array[i]);
                }
        }
        free(rect_array);
        pd_rects_clear(&rects);
        count += ui_server_update_flash_rects(conn);
        return count;
}

size_t ui_server_render(void)
{
        size_t count = 0;
        list_node_t *node;
        ui_connection_t *conn;
        float dpi = 1.f * ui_metrics.dpi;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                if (!conn->window_visible) {
                        continue;
                }
                ui_metrics.dpi = 1.f * ptk_window_get_dpi(conn->window);
                count += ui_server_render_window(conn);
                count += ui_server_update_flash_rects(conn);
        }
        ui_metrics.dpi = dpi;
        return count;
}

/* 将系统光标与 hover 链同步：从 hover 目标沿祖先向上，
 * 第一个非 auto 的值生效（模拟 CSS cursor 继承语义）；
 * 全链 auto 或无目标时回写 AUTO，清掉上一次设置的自定义光标 */
static void ui_update_cursor(void)
{
        list_node_t *node;
        ui_connection_t *conn;
        ui_widget_t *w;
        ui_widget_t *owner = NULL;
        css_cursor_t cursor = CSS_CURSOR_AUTO;
        ptk_window_t *window = NULL;

        for (list_each(node, &ui_server.connections)) {
                conn = node->data;
                owner = ui_get_mouseover_in(conn->widget);
                if (owner) {
                        break;
                }
        }

        /* 单遍上行：取第一个非 auto 的 cursor 值（继承模拟），
         * 并沿父链回溯到已连接的根部件以解析窗口（hover 目标可以是任意后代） */
        for (w = owner; w; w = w->parent) {
                if (cursor == CSS_CURSOR_AUTO) {
                        cursor = w->computed_style.type_bits.cursor;
                }
                if (!window) {
                        window = ui_server_get_window(w);
                }
        }
        if (!window) {
                window = last_synced_cursor_window;
                cursor = CSS_CURSOR_AUTO;
        }
        if (window) {
                ptk_window_set_cursor(window, cursor_map[cursor]);
                last_synced_cursor_window = window;
        }
}

void ui_server_update(void)
{
        ui_connection_t *conn;
        list_node_t *node;
        float dpi = 1.f * ui_metrics.dpi;

        if (ui_server.connections.length > 0) {
                for (list_each(node, &ui_server.connections)) {
                        conn = node->data;
                        ui_metrics.dpi = 1.f * ptk_window_get_dpi(conn->window);
                        ui_updater_update(conn->updater, conn->widget);
                        if (conn->state == UI_CONNECTION_STATE_PENDING) {
                                ptk_event_t e = { 0 };

                                conn->state = UI_CONNECTION_STATE_INITIALIZING;
                                ui_server_refresh_window(conn);
                                conn->state = UI_CONNECTION_STATE_INITIALIZED;
                                e.type = PTK_EVENT_CREATE;
                                e.window = conn->window;
                                ptk_process_event(&e);
                        }
                }
        } else {
                ui_update();
        }
        ui_metrics.dpi = dpi;
        ui_update_cursor();
}

void ui_server_init(void)
{
        ui_cursor_init();
        ui_server.num_rendering_threads = 4;
        ptk_on_event(PTK_EVENT_VISIBILITY_CHANGE,
                     ui_server_on_window_visibility_change, NULL);
        ptk_on_event(PTK_EVENT_MINMAXINFO, ui_server_on_window_minmaxinfo,
                     NULL);
        ptk_on_event(PTK_EVENT_SIZE, ui_server_on_window_resize, NULL);
        ptk_on_event(PTK_EVENT_CLOSE, ui_server_on_window_close, NULL);
        ptk_on_event(PTK_EVENT_PAINT, ui_server_on_window_paint, NULL);
        ptk_on_event(PTK_EVENT_DPICHANGED, ui_server_on_window_dpi_changed,
                     NULL);
        list_create(&ui_server.connections);
        switch (ptk_get_app_id()) {
        case PTK_APP_ID_LINUX_X11:
        case PTK_APP_ID_UWP:
        case PTK_APP_ID_WIN32:
                ui_cursor_hide();
                break;
        default:
                break;
        }
}

void ui_server_set_threads(int threads)
{
        if (threads < 1) {
                return;
        }
        ui_server.num_rendering_threads = threads;
}

void ui_server_set_paint_flashing_enabled(bool enabled)
{
        ui_server.paint_flashing_enabled = enabled;
}

void ui_server_destroy(void)
{
        ui_cursor_destroy();
        ptk_off_event(PTK_EVENT_MINMAXINFO, ui_server_on_window_minmaxinfo);
        ptk_off_event(PTK_EVENT_SIZE, ui_server_on_window_resize);
        ptk_off_event(PTK_EVENT_CLOSE, ui_server_on_window_close);
        ptk_off_event(PTK_EVENT_VISIBILITY_CHANGE,
                      ui_server_on_window_visibility_change);
        ptk_off_event(PTK_EVENT_DPICHANGED, ui_server_on_window_dpi_changed);
        list_destroy(&ui_server.connections, ui_connection_destroy);
}
