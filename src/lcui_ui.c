/*
 * src/lcui_ui.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <yutil.h>
#include <pandagl.h>
#include <ui.h>
#include <thread.h>
#include <ui_server.h>
#include <LCUI/ui.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

typedef struct {
        bool active;
        thread_t thread;
        thread_mutex_t mutex;
        thread_cond_t cond;
} lcui_ui_image_loader_t;

static struct lcui_ui {
        bool quit_after_all_windows_closed;
        lcui_ui_image_loader_t image_loader;
        lcui_display_t mode;
        ui_mutation_observer_t *observer;

        /** list_t<ptk_window_t> */
        list_t windows;
} lcui_ui;

size_t lcui_ui_render(void)
{
        return ui_server_render();
}

void lcui_ui_update(void)
{
        ui_server_update();
        thread_mutex_lock(&lcui_ui.image_loader.mutex);
        ui_clear_images();
        thread_mutex_unlock(&lcui_ui.image_loader.mutex);
}

static void lcui_ui_process_mutation(ui_mutation_record_t *mutation)
{
        list_node_t *node;

        if (mutation->type != UI_MUTATION_RECORD_TYPE_CHILD_LIST ||
            lcui_ui.mode != LCUI_DISPLAY_SEAMLESS) {
                return;
        }
        for (list_each(node, &mutation->removed_widgets)) {
                ui_server_disconnect(node->data, NULL);
        }
        for (list_each(node, &mutation->added_widgets)) {
                ui_server_connect(node->data, NULL);
        }
}

static void lcui_ui_on_mutation(ui_mutation_list_t *mutation_list,
                                ui_mutation_observer_t *observer, void *arg)
{
        list_node_t *node;

        for (list_each(node, mutation_list)) {
                lcui_ui_process_mutation(node->data);
        }
}

static void lcui_ui_on_window_destroy(ptk_event_t *e, void *arg)
{
        list_node_t *node;

        for (list_each(node, &lcui_ui.windows)) {
                if (node->data == e->window) {
                        list_delete_node(&lcui_ui.windows, node);
                        break;
                }
        }
        if (lcui_ui.quit_after_all_windows_closed &&
            lcui_ui.windows.length == 0) {
                ptk_app_exit(0);
        }
}

static void lcui_ui_close_window(void *arg)
{
        ptk_window_close(arg);
}

void lcui_ui_set_display(lcui_display_t mode)
{
        ptk_window_t *wnd;
        list_node_t *node;
        ui_mutation_observer_init_t options = { 0 };

        if (mode == LCUI_DISPLAY_DEFAULT) {
                mode = LCUI_DISPLAY_WINDOWED;
        }
        if (lcui_ui.mode == mode) {
                return;
        }
        if (lcui_ui.mode == LCUI_DISPLAY_FULLSCREEN ||
            lcui_ui.mode == LCUI_DISPLAY_WINDOWED) {
                wnd = ui_server_get_window(ui_root());
                if (mode == LCUI_DISPLAY_FULLSCREEN) {
                        ptk_window_set_fullscreen(wnd, true);
                        lcui_ui.mode = mode;
                        return;
                } else if (mode == LCUI_DISPLAY_WINDOWED) {
                        ptk_window_set_fullscreen(wnd, false);
                        lcui_ui.mode = mode;
                        return;
                }
        }
        if (lcui_ui.observer) {
                ui_mutation_observer_disconnect(lcui_ui.observer);
                ui_mutation_observer_destroy(lcui_ui.observer);
                lcui_ui.observer = NULL;
        }
        lcui_ui.quit_after_all_windows_closed = false;
        list_destroy(&lcui_ui.windows, lcui_ui_close_window);
        switch (mode) {
        case LCUI_DISPLAY_FULLSCREEN:
                wnd = ptk_window_create(NULL, 0, 0, 0, 0, NULL);
                list_append(&lcui_ui.windows, wnd);
                ui_server_connect(ui_root(), wnd);
                ptk_window_set_fullscreen(wnd, true);
                ui_metrics.dpi = 1.f * ptk_window_get_dpi(wnd);
                break;
        case LCUI_DISPLAY_SEAMLESS:
                options.child_list = true;
                lcui_ui.observer =
                    ui_mutation_observer_create(lcui_ui_on_mutation, NULL);
                ui_mutation_observer_observe(lcui_ui.observer, ui_root(),
                                             options);
                for (list_each(node, &ui_root()->children)) {
                        wnd = ptk_window_create(NULL, 0, 0, 0, 0, NULL);
                        ui_server_connect(node->data, wnd);
                }
                break;
        case LCUI_DISPLAY_WINDOWED:
        default:
                wnd = ptk_window_create(NULL, 0, 0, 0, 0, NULL);
                list_append(&lcui_ui.windows, wnd);
                ui_server_connect(ui_root(), wnd);
                ui_metrics.dpi = 1.f * ptk_window_get_dpi(wnd);
                break;
        }
        lcui_ui.mode = mode;
        lcui_ui.quit_after_all_windows_closed = true;
}

static void lcui_ui_image_loader_refresh(void)
{
        thread_mutex_lock(&lcui_ui.image_loader.mutex);
        thread_cond_signal(&lcui_ui.image_loader.cond);
        thread_mutex_unlock(&lcui_ui.image_loader.mutex);
}

static void lcui_ui_on_load_image(ui_image_t *unused)
{
        lcui_ui_image_loader_refresh();
}

static void lcui_ui_image_loader_thread(void *unused)
{
        while (lcui_ui.image_loader.active) {
                ui_load_images();
                thread_mutex_lock(&lcui_ui.image_loader.mutex);
                thread_cond_timedwait(&lcui_ui.image_loader.cond,
                                      &lcui_ui.image_loader.mutex, 1000);
                thread_mutex_unlock(&lcui_ui.image_loader.mutex);
        }
        thread_exit(NULL);
}

void lcui_ui_init(void)
{
        thread_cond_init(&lcui_ui.image_loader.cond);
        thread_mutex_init(&lcui_ui.image_loader.mutex);
        lcui_ui.image_loader.active = true;
        if (thread_create(&lcui_ui.image_loader.thread,
                          lcui_ui_image_loader_thread, NULL) != 0) {
                logger_error(
                    "[lcui-ui] image loader thread creation failed!\n");
        }
        ui_set_image_loader_callback(lcui_ui_on_load_image);

        ui_init();
        ui_server_init();
        ui_widget_resize(ui_root(), DEFAULT_WINDOW_WIDTH,
                         DEFAULT_WINDOW_HEIGHT);
        list_create(&lcui_ui.windows);
        lcui_ui_set_display(LCUI_DISPLAY_DEFAULT);
        ptk_on_event(PTK_EVENT_CLOSE, lcui_ui_on_window_destroy, NULL);
}

void lcui_ui_destroy(void)
{
        ptk_off_event(PTK_EVENT_CLOSE, lcui_ui_on_window_destroy);
        list_destroy(&lcui_ui.windows, lcui_ui_close_window);
        if (lcui_ui.observer) {
                ui_mutation_observer_disconnect(lcui_ui.observer);
                ui_mutation_observer_destroy(lcui_ui.observer);
                lcui_ui.observer = NULL;
        }
        lcui_ui.image_loader.active = false;
        lcui_ui.quit_after_all_windows_closed = true;
        lcui_ui_image_loader_refresh();
        thread_join(lcui_ui.image_loader.thread, NULL);
        thread_mutex_destroy(&lcui_ui.image_loader.mutex);
        thread_cond_destroy(&lcui_ui.image_loader.cond);
        ui_server_destroy();
        ui_destroy();
}
