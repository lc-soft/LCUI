/*
 * src/lcui_ui.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <yutil.h>
#include <pandagl.h>
#include <ui.h>
#include <thread.h>
#include <ui_server.h>
#include <LCUI/ui.h>
#include <LCUI/worker.h>
#include "lcui_app.h"

typedef struct {
        bool active;
        thread_t thread;
        thread_mutex_t mutex;
        thread_cond_t cond;
} lcui_ui_image_loader_t;

static struct lcui_ui {
        lcui_ui_image_loader_t image_loader;
} lcui_ui;

size_t lcui_ui_render(void)
{
        size_t count = ui_server_render();

        lcui_window_flush_show();
        return count;
}

void lcui_ui_update(void)
{
        ui_server_update();
        thread_mutex_lock(&lcui_ui.image_loader.mutex);
        ui_clear_images();
        thread_mutex_unlock(&lcui_ui.image_loader.mutex);
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

        ui_init();
        ui_server_init();
        ui_set_image_loader_callback(lcui_ui_on_load_image);

        /* Start the loader only after ui_init() has created the image cache,
         * list and mutex; the thread calls ui_load_images() immediately. */
        lcui_ui.image_loader.active = true;
        if (thread_create(&lcui_ui.image_loader.thread,
                          lcui_ui_image_loader_thread, NULL) != 0) {
                logger_error(
                    "[lcui-ui] image loader thread creation failed!\n");
        }
}

void lcui_ui_destroy(void)
{
        lcui_ui.image_loader.active = false;
        lcui_ui_image_loader_refresh();
        thread_join(lcui_ui.image_loader.thread, NULL);
        thread_mutex_destroy(&lcui_ui.image_loader.mutex);
        thread_cond_destroy(&lcui_ui.image_loader.cond);
        ui_server_destroy();
        ui_destroy();
}
