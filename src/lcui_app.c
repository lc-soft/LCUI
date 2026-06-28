/*
 * src/lcui_app.c
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <yutil.h>
#include <ptk.h>
#include <ptk/steptimer.h>
#include <ui.h>
#include <ui_server.h>
#include <LCUI/ui.h>
#include <LCUI/worker.h>
#include <LCUI/app.h>
#include "lcui_app.h"

typedef struct lcui_frame_request {
        list_node_t node;
        int id;
        lcui_frame_cb_t callback;
        void *data;
} lcui_frame_request_t;

static struct lcui_app {
        // Animation frame driver. Fixed-step, no catch-up: the handler
        // fires at most once per tick when enough time has accumulated,
        // flushing all registered lcui_request_frame callbacks.
        ptk_steptimer_t anim_timer;
        list_t frame_cbs;
        int next_frame_id;

        // Render throttling. Two consecutive renders are at least
        // min_render_interval milliseconds apart. 0 disables throttling.
        uint64_t last_render_time;
        uint64_t min_render_interval;

        // FPS tracking based on actually rendered frames.
        uint64_t fps_window_start;
        uint32_t frames_this_second;
        uint32_t frames_per_second;
} lcui_app;
static char *lcui_app_id = NULL;

static bool lcui_app_id_is_valid(const char *app_id)
{
        const unsigned char *p;

        if (!app_id || app_id[0] == 0) {
                return false;
        }
        for (p = (const unsigned char *)app_id; *p; ++p) {
                if (isalnum(*p) || *p == '.' || *p == '_' || *p == '-') {
                        continue;
                }
                return false;
        }
        return true;
}

bool lcui_set_app_id(const char *app_id)
{
        size_t len;
        char *new_app_id;

        if (!lcui_app_id_is_valid(app_id)) {
                return false;
        }
        len = strlen(app_id);
        new_app_id = malloc(sizeof(char) * (len + 1));
        if (!new_app_id) {
                return false;
        }
        strcpy(new_app_id, app_id);
        free(lcui_app_id);
        lcui_app_id = new_app_id;
        return true;
}

const char *lcui_get_app_id(void)
{
        return lcui_app_id ? lcui_app_id : "lcui";
}

static void lcui_dispatch_ui_mouse_event(ui_event_type_t type,
                                         ptk_event_t *app_evt)
{
        ui_event_t e = { 0 };
        float scale = ui_server_get_window_scale(app_evt->window);

        e.type = type;
        e.mouse.y = (float)round(app_evt->mouse.y / scale);
        e.mouse.x = (float)round(app_evt->mouse.x / scale);
        ui_dispatch_event(&e);
}

static void lcui_dispatch_ui_keyboard_event(ui_event_type_t type,
                                            ptk_event_t *app_evt)
{
        ui_event_t e = { 0 };

        e.type = type;
        e.key.code = app_evt->key.code;
        e.key.is_composing = app_evt->key.is_composing;
        e.key.alt_key = app_evt->key.alt_key;
        e.key.shift_key = app_evt->key.shift_key;
        e.key.ctrl_key = app_evt->key.ctrl_key;
        e.key.meta_key = app_evt->key.meta_key;
        ui_dispatch_event(&e);
}

static void lcui_dispatch_ui_touch_event(ptk_event_t *app_event)
{
        size_t i;
        ui_event_t e = { 0 };
        float scale = ui_server_get_window_scale(app_event->window);

        e.type = UI_EVENT_TOUCH;
        e.touch.n_points = app_event->touch.n_points;
        e.touch.points = malloc(sizeof(ui_touch_point_t) * e.touch.n_points);
        for (i = 0; i < e.touch.n_points; ++i) {
                switch (app_event->touch.points[i].state) {
                case PTK_EVENT_TOUCHDOWN:
                        e.touch.points[i].state = UI_EVENT_TOUCHDOWN;
                        break;
                case PTK_EVENT_TOUCHUP:
                        e.touch.points[i].state = UI_EVENT_TOUCHUP;
                        break;
                case PTK_EVENT_TOUCHMOVE:
                        e.touch.points[i].state = UI_EVENT_TOUCHMOVE;
                        break;
                default:
                        break;
                }
                e.touch.points[i].x =
                    (float)round(app_event->touch.points[i].x / scale);
                e.touch.points[i].y =
                    (float)round(app_event->touch.points[i].y / scale);
        }
        ui_dispatch_event(&e);
        ui_event_destroy(&e);
}

static void lcui_dispatch_ui_textinput_event(ptk_event_t *app_evt)
{
        ui_event_t e = { 0 };

        e.type = UI_EVENT_TEXTINPUT;
        e.text.length = app_evt->text.length;
        e.text.text = y_wcsdup(app_evt->text.text);
        ui_dispatch_event(&e);
        ui_event_destroy(&e);
}

static void lcui_dispatch_ui_wheel_event(app_wheel_event_t *wheel)
{
        ui_event_t e = { 0 };

        // TODO:
        e.type = UI_EVENT_WHEEL;
        e.wheel.delta_mode = UI_WHEEL_DELTA_PIXEL;
        e.wheel.delta_y = wheel->delta_y;
        ui_dispatch_event(&e);
}

static void lcui_dispatch_ui_event(ptk_event_t *app_event)
{
        switch (app_event->type) {
        case PTK_EVENT_KEYDOWN:
                lcui_dispatch_ui_keyboard_event(UI_EVENT_KEYDOWN, app_event);
                break;
        case PTK_EVENT_KEYUP:
                lcui_dispatch_ui_keyboard_event(UI_EVENT_KEYUP, app_event);
                break;
        case PTK_EVENT_KEYPRESS:
                lcui_dispatch_ui_keyboard_event(UI_EVENT_KEYPRESS, app_event);
                break;
        case PTK_EVENT_MOUSEDOWN:
                lcui_dispatch_ui_mouse_event(UI_EVENT_MOUSEDOWN, app_event);
                break;
        case PTK_EVENT_MOUSEUP:
                lcui_dispatch_ui_mouse_event(UI_EVENT_MOUSEUP, app_event);
                break;
        case PTK_EVENT_MOUSEMOVE:
                lcui_dispatch_ui_mouse_event(UI_EVENT_MOUSEMOVE, app_event);
                break;
        case PTK_EVENT_TOUCH:
                lcui_dispatch_ui_touch_event(app_event);
                break;
        case PTK_EVENT_WHEEL:
                lcui_dispatch_ui_wheel_event(&app_event->wheel);
                break;
        case PTK_EVENT_COMPOSITION:
                lcui_dispatch_ui_textinput_event(app_event);
                break;
        default:
                break;
        }
}

static void lcui_app_on_anim_frame(ptk_steptimer_t *timer, void *data)
{
        uint64_t timestamp = (uint64_t)y_gettime();
        list_node_t *node;

        list_for_each(node, &lcui_app.frame_cbs)
        {
                lcui_frame_request_t *req = node->data;
                req->callback(timestamp, req->data);
        }
        list_destroy(&lcui_app.frame_cbs, NULL);
}

int lcui_request_frame(lcui_frame_cb_t callback, void *data)
{
        lcui_frame_request_t *req = malloc(sizeof(lcui_frame_request_t));

        if (!req) {
                return -1;
        }
        req->id = ++lcui_app.next_frame_id;
        req->callback = callback;
        req->data = data;
        list_append(&lcui_app.frame_cbs, req);
        return req->id;
}

void lcui_cancel_frame(int request_id)
{
        list_node_t *node;

        list_for_each(node, &lcui_app.frame_cbs)
        {
                lcui_frame_request_t *req = node->data;
                if (req->id == request_id) {
                        list_unlink(&lcui_app.frame_cbs, node);
                        free(req);
                        list_node_free(node);
                        return;
                }
        }
}

static void lcui_app_render_frame(void)
{
        uint64_t now = (uint64_t)y_gettime();

        // Throttle: skip this frame if the previous render is too recent.
        if (lcui_app.min_render_interval > 0 &&
            now - lcui_app.last_render_time < lcui_app.min_render_interval) {
                return;
        }
        lcui_app.last_render_time = now;

        lcui_ui_render();
        ptk_app_present();

        // Count actually rendered frames into the FPS statistic.
        lcui_app.frames_this_second++;
        if (now - lcui_app.fps_window_start >= 1000) {
                lcui_app.frames_per_second = lcui_app.frames_this_second;
                lcui_app.frames_this_second = 0;
                lcui_app.fps_window_start = now;
        }
}

static int lcui_app_dispatch(ptk_event_t *e)
{
        if (e->type == PTK_EVENT_QUIT) {
                return 0;
        }
        lcui_dispatch_ui_event(e);
        lcui_worker_run();
        ptk_steptimer_tick(&lcui_app.anim_timer, lcui_app_on_anim_frame, NULL);
        lcui_ui_update();
        lcui_app_render_frame();
        return 0;
}

uint32_t lcui_get_fps(void)
{
        return lcui_app.frames_per_second;
}

void lcui_set_fps_cap(unsigned fps_cap)
{
        lcui_app.min_render_interval = (fps_cap > 0) ? (1000 / fps_cap) : 0;
        lcui_app.anim_timer.target_elapsed_time =
            (fps_cap > 0) ? (1000 / fps_cap) : (1000 / 60);
}

int lcui_app_process_events(ptk_process_events_option_t option)
{
        return ptk_process_native_events(option);
}

void lcui_app_init(void)
{
        uint64_t now = (uint64_t)y_gettime();

        lcui_worker_init();
        ptk_steptimer_init(&lcui_app.anim_timer);
        lcui_app.anim_timer.is_fixed_time_step = true;
        lcui_app.anim_timer.enable_catch_up = false;
        lcui_app.anim_timer.target_elapsed_time = 1000 / 60;
        list_create(&lcui_app.frame_cbs);
        lcui_app.next_frame_id = 0;
        lcui_app.last_render_time = now;
        lcui_app.min_render_interval = 0;
        lcui_app.fps_window_start = now;
        lcui_app.frames_this_second = 0;
        lcui_app.frames_per_second = 0;
        if (ptk_init(L"LCUI Application") != 0) {
                abort();
        }
        ptk_set_event_dispatcher(lcui_app_dispatch);
        lcui_ui_init();
}

void lcui_app_destroy(void)
{
        lcui_ui_destroy();
        lcui_worker_destroy();
        list_destroy_without_node(&lcui_app.frame_cbs, NULL);
        free(lcui_app_id);
        lcui_app_id = NULL;
        ptk_destroy();
}
