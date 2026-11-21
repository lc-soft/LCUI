/*
 * src/lcui_window.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI.h>
#include "lcui_app.h"
#include "lcui_icon.h"

struct lcui_window {
        list_node_t node;
        ptk_window_t *ptk;
        ui_widget_t *widget;
        char *id;
        bool show_requested;
        bool closed;
        bool state_applied;
        bool has_custom_icon;
        int last_width;
        int last_height;
        int last_x;
        int last_y;
        lcui_window_close_cb_t close_cb;
        void *close_arg;
};

static struct {
        list_t windows;
        bool ready;
        bool listener_ready;
} lcui_windows;

static bool lcui_quit_on_last_window = true;

static bool lcui_window_has_open_window(void)
{
        list_node_t *node;

        for (list_each(node, &lcui_windows.windows)) {
                lcui_window_t *w = node->data;

                if (!w->closed) {
                        return true;
                }
        }
        return false;
}

static lcui_window_t *lcui_window_find_by_ptk(ptk_window_t *ptk)
{
        list_node_t *node;

        for (list_each(node, &lcui_windows.windows)) {
                lcui_window_t *w = node->data;

                if (w->ptk == ptk) {
                        return w;
                }
        }
        return NULL;
}

static void lcui_window_on_widget_destroy(ui_widget_t *widget, ui_event_t *e,
                                          void *arg)
{
        lcui_window_t *w = e->data;

        if (w->widget == widget) {
                w->widget = NULL;
        }
}

static int lcui_window_to_px(ptk_window_t *ptk, int value)
{
        return (int)round(value * ptk_window_get_dpi(ptk) / 96.0);
}

static int lcui_window_to_dp(ptk_window_t *ptk, int value)
{
        return (int)round(value * 96.0 / ptk_window_get_dpi(ptk));
}

static void lcui_window_apply_state(lcui_window_t *w)
{
        lcui_window_state_t state;

        if (!w->id || !lcui_get_window_state(w->id, &state)) {
                return;
        }
        ptk_window_set_position(w->ptk, state.x, state.y);
        ptk_window_set_size(w->ptk, lcui_window_to_px(w->ptk, state.width),
                            lcui_window_to_px(w->ptk, state.height));
        ptk_window_set_maximized(w->ptk, state.maximized);
}

static void lcui_window_save_state(lcui_window_t *w)
{
        lcui_window_state_t state;

        if (!w->id || !w->ptk || !lcui_settings_is_loaded()) {
                return;
        }
        ptk_window_get_position(w->ptk, &state.x, &state.y);
        state.width = lcui_window_to_dp(w->ptk, ptk_window_get_width(w->ptk));
        state.height = lcui_window_to_dp(w->ptk, ptk_window_get_height(w->ptk));
        state.maximized = ptk_window_is_maximized(w->ptk);
        if (lcui_set_window_state(w->id, &state)) {
                lcui_settings_flush();
        }
}

static void lcui_window_on_ptk_close(ptk_event_t *e, void *arg)
{
        list_node_t *node;

        if (!lcui_windows.ready) {
                return;
        }
        for (list_each(node, &lcui_windows.windows)) {
                lcui_window_t *w = node->data;

                if (w->ptk != e->window) {
                        continue;
                }
                lcui_window_save_state(w);
                w->last_width =
                    lcui_window_to_dp(w->ptk, ptk_window_get_width(w->ptk));
                w->last_height =
                    lcui_window_to_dp(w->ptk, ptk_window_get_height(w->ptk));
                ptk_window_get_position(w->ptk, &w->last_x, &w->last_y);
                lcui_app_forget_window(w->ptk);
                w->closed = true;
                w->ptk = NULL;
                if (w->close_cb) {
                        w->close_cb(w, w->close_arg);
                }
                if (lcui_quit_on_last_window &&
                    !lcui_window_has_open_window()) {
                        lcui_quit();
                }
                return;
        }
}

static void lcui_window_ensure_listener(void)
{
        if (!lcui_windows.listener_ready) {
                ptk_on_event(PTK_EVENT_CLOSE, lcui_window_on_ptk_close, NULL);
                lcui_windows.listener_ready = true;
        }
}

static void lcui_window_ensure_list(void)
{
        if (!lcui_windows.ready) {
                list_create(&lcui_windows.windows);
                lcui_windows.ready = true;
        }
}

static void lcui_window_apply_default_icon(lcui_window_t *w)
{
        const ptk_icon_image_t *images;
        int count = 0;

        if (!w->ptk || w->has_custom_icon) {
                return;
        }
        images = lcui_icon_default_images(&count);
        ptk_window_set_icons(w->ptk, images, count);
}

static void lcui_window_refresh_default_icons(void)
{
        list_node_t *node;

        if (!lcui_windows.ready) {
                return;
        }
        for (list_each(node, &lcui_windows.windows)) {
                lcui_window_apply_default_icon(node->data);
        }
}

lcui_window_t *lcui_window_create(const lcui_window_options_t *opt)
{
        lcui_window_t *w;
        int width = 0;
        int height = 0;
        int x, y;

        w = calloc(1, sizeof(*w));
        if (!w) {
                return NULL;
        }
        w->ptk = ptk_window_create(opt && opt->title ? opt->title : L"", 0, 0,
                                   0, 0, NULL);
        if (!w->ptk) {
                free(w);
                return NULL;
        }
        lcui_window_ensure_list();
        lcui_window_ensure_listener();
        w->node.data = w;
        list_append_node(&lcui_windows.windows, &w->node);
        if (opt && opt->icon) {
                lcui_window_set_icon(w, opt->icon);
        }
        if (!w->has_custom_icon) {
                lcui_window_apply_default_icon(w);
        }
        if (opt) {
                width = opt->width;
                height = opt->height;
        }
        if (width > 0 && height > 0) {
                int px_w = lcui_window_to_px(w->ptk, width);
                int px_h = lcui_window_to_px(w->ptk, height);

                /* First call establishes the frame; second call corrects the
                 * client size. */
                ptk_window_set_size(w->ptk, px_w, px_h);
                ptk_window_set_size(w->ptk, px_w, px_h);
        }
        if (opt && opt->id) {
                w->id = y_strdup(opt->id);
                if (lcui_settings_is_loaded()) {
                        lcui_window_apply_state(w);
                        w->state_applied = true;
                }
        }
        if (!w->state_applied) {
                x = opt ? opt->x : LCUI_WINDOW_POS_AUTO;
                y = opt ? opt->y : LCUI_WINDOW_POS_AUTO;
                if (x == LCUI_WINDOW_POS_AUTO && y == LCUI_WINDOW_POS_AUTO) {
                        x = (ptk_screen_get_width() -
                             ptk_window_get_width(w->ptk)) /
                            2;
                        y = (ptk_screen_get_height() -
                             ptk_window_get_height(w->ptk)) /
                            2;
                }
                ptk_window_set_position(w->ptk, x, y);
        }
        if (opt && opt->maximized) {
                ptk_window_set_maximized(w->ptk, true);
        }
        if (opt && opt->fullscreen) {
                ptk_window_set_fullscreen(w->ptk, true);
        }
        return w;
}

void lcui_window_destroy(lcui_window_t *w)
{
        if (w->widget) {
                ui_widget_off(w->widget, "destroy",
                              lcui_window_on_widget_destroy, w);
                ui_server_detach_widget(w->widget);
                w->widget = NULL;
        }
        if (w->ptk) {
                lcui_app_forget_window(w->ptk);
                if (!w->closed) {
                        lcui_window_save_state(w);
                        ptk_window_destroy(w->ptk);
                }
                w->ptk = NULL;
        }
        if (lcui_windows.ready) {
                list_unlink(&lcui_windows.windows, &w->node);
        }
        free(w->id);
        free(w);
}

bool lcui_window_bind_widget(lcui_window_t *w, ui_widget_t *widget)
{
        ptk_window_t *old_ptk;
        lcui_window_t *old_window;

        if (!widget) {
                if (w->widget) {
                        ui_widget_off(w->widget, "destroy",
                                      lcui_window_on_widget_destroy, w);
                        ui_server_detach_widget(w->widget);
                        w->widget = NULL;
                }
                return true;
        }
        if (!w->ptk || w->closed) {
                return false;
        }
        if (w->widget == widget && ui_server_get_window(widget) == w->ptk) {
                return true;
        }
        if (w->widget) {
                ui_widget_off(w->widget, "destroy",
                              lcui_window_on_widget_destroy, w);
                ui_server_detach_widget(w->widget);
                w->widget = NULL;
        }
        old_ptk = ui_server_get_window(widget);
        if (old_ptk && old_ptk != w->ptk) {
                old_window = lcui_window_find_by_ptk(old_ptk);
                ui_server_detach_widget(widget);
                if (old_window) {
                        ui_widget_off(widget, "destroy",
                                      lcui_window_on_widget_destroy,
                                      old_window);
                        if (old_window->widget == widget) {
                                old_window->widget = NULL;
                        }
                }
        }
        ui_set_event_scope(widget);
        ui_server_connect(widget, w->ptk);
        ui_widget_on(widget, "destroy", lcui_window_on_widget_destroy, w);
        w->widget = widget;
        return true;
}

ui_widget_t *lcui_window_get_widget(lcui_window_t *w)
{
        return w->widget;
}

lcui_window_t *lcui_window_create_for_widget(ui_widget_t *widget,
                                             const lcui_window_options_t *opt)
{
        lcui_window_options_t options = { 0 };
        lcui_window_t *w;

        if (opt) {
                options = *opt;
        }
        if (options.width <= 0 || options.height <= 0) {
                ui_update();
                if (options.width <= 0) {
                        options.width = (int)widget->border_box.width;
                }
                if (options.height <= 0) {
                        options.height = (int)widget->border_box.height;
                }
        }
        if (!options.title) {
                options.title = widget->title;
        }
        w = lcui_window_create(&options);
        if (w) {
                lcui_window_bind_widget(w, widget);
        }
        return w;
}

void lcui_window_show(lcui_window_t *w)
{
        if (!w->ptk) {
                return;
        }
        w->show_requested = true;
}

void lcui_window_hide(lcui_window_t *w)
{
        w->show_requested = false;
        if (w->ptk) {
                ptk_window_hide(w->ptk);
        }
}

void lcui_window_set_size(lcui_window_t *w, int cw, int ch)
{
        if (!w->ptk) {
                return;
        }
        ptk_window_set_size(w->ptk, lcui_window_to_px(w->ptk, cw),
                            lcui_window_to_px(w->ptk, ch));
        if (w->widget) {
                ui_widget_resize(w->widget, (float)cw, (float)ch);
        }
}

void lcui_window_set_position(lcui_window_t *w, int x, int y)
{
        if (!w->ptk) {
                return;
        }
        ptk_window_set_position(w->ptk, x, y);
}

void lcui_window_set_title(lcui_window_t *w, const wchar_t *title)
{
        if (!w->ptk) {
                return;
        }
        ptk_window_set_title(w->ptk, title);
}

bool lcui_window_set_icon(lcui_window_t *w, const char *file)
{
        lcui_icon_t icon;

        if (!w || !w->ptk) {
                return false;
        }
        if (!file) {
                w->has_custom_icon = false;
                lcui_window_apply_default_icon(w);
                return true;
        }
        if (!lcui_icon_create_from_file(&icon, file)) {
                return false;
        }
        ptk_window_set_icons(w->ptk, icon.image, icon.count);
        lcui_icon_destroy(&icon);
        w->has_custom_icon = true;
        return true;
}

bool lcui_window_set_icon_from_data(lcui_window_t *w, const void *data,
                                    size_t size)
{
        lcui_icon_t icon;

        if (!w || !w->ptk) {
                return false;
        }
        if (!data || size == 0) {
                w->has_custom_icon = false;
                lcui_window_apply_default_icon(w);
                return true;
        }
        if (!lcui_icon_create_from_data(&icon, data, size)) {
                return false;
        }
        ptk_window_set_icons(w->ptk, icon.image, icon.count);
        lcui_icon_destroy(&icon);
        w->has_custom_icon = true;
        return true;
}

bool lcui_set_default_window_icon(const char *file)
{
        if (!file) {
                lcui_icon_reset_default();
        } else if (!lcui_icon_set_default_from_file(file)) {
                return false;
        }
        lcui_window_refresh_default_icons();
        return true;
}

bool lcui_set_default_window_icon_from_data(const void *data, size_t size)
{
        if (!data || size == 0) {
                lcui_icon_reset_default();
        } else if (!lcui_icon_set_default_from_data(data, size)) {
                return false;
        }
        lcui_window_refresh_default_icons();
        return true;
}

void lcui_set_default_window_icon_enabled(bool enable)
{
        lcui_icon_set_default_enabled(enable);
        lcui_window_refresh_default_icons();
}

void lcui_window_set_maximized(lcui_window_t *w, bool maximized)
{
        if (!w->ptk) {
                return;
        }
        ptk_window_set_maximized(w->ptk, maximized);
}

void lcui_window_set_fullscreen(lcui_window_t *w, bool fullscreen)
{
        if (!w->ptk) {
                return;
        }
        ptk_window_set_fullscreen(w->ptk, fullscreen);
}

void lcui_window_close(lcui_window_t *w)
{
        if (w->ptk && !w->closed) {
                ptk_window_close(w->ptk);
        }
}

void lcui_window_on_close(lcui_window_t *w, lcui_window_close_cb_t cb,
                          void *arg)
{
        w->close_cb = cb;
        w->close_arg = arg;
        lcui_window_ensure_listener();
}

void lcui_window_get_size(lcui_window_t *w, int *cw, int *ch)
{
        if (!w->ptk) {
                *cw = w->last_width;
                *ch = w->last_height;
                return;
        }
        *cw = lcui_window_to_dp(w->ptk, ptk_window_get_width(w->ptk));
        *ch = lcui_window_to_dp(w->ptk, ptk_window_get_height(w->ptk));
}

void lcui_window_get_position(lcui_window_t *w, int *x, int *y)
{
        if (!w->ptk) {
                *x = w->last_x;
                *y = w->last_y;
                return;
        }
        ptk_window_get_position(w->ptk, x, y);
}

ptk_window_t *lcui_window_get_handle(lcui_window_t *w)
{
        return w->ptk;
}

void lcui_window_flush_show(void)
{
        list_node_t *node;

        if (!lcui_windows.ready) {
                return;
        }
        for (list_each(node, &lcui_windows.windows)) {
                lcui_window_t *w = node->data;

                if (w->show_requested && w->ptk &&
                    (!w->widget || ui_server_is_widget_ready(w->widget)) &&
                    !ptk_window_is_visible(w->ptk)) {
                        ptk_window_show(w->ptk);
                }
        }
}

void lcui_window_apply_pending_states(void)
{
        list_node_t *node;

        if (!lcui_windows.ready || !lcui_settings_is_loaded()) {
                return;
        }
        for (list_each(node, &lcui_windows.windows)) {
                lcui_window_t *w = node->data;

                if (w->id && !w->state_applied && w->ptk) {
                        lcui_window_apply_state(w);
                        w->state_applied = true;
                }
        }
}

void lcui_set_quit_on_last_window(bool enable)
{
        lcui_quit_on_last_window = enable;
}

void lcui_window_destroy_all(void)
{
        list_node_t *node;

        if (!lcui_windows.ready) {
                return;
        }
        while ((node = list_get_first_node(&lcui_windows.windows))) {
                lcui_window_destroy(node->data);
        }
        list_destroy(&lcui_windows.windows, NULL);
        lcui_windows.ready = false;
        lcui_windows.listener_ready = false;
}
