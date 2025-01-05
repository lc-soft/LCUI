/*
 * src/lcui_app.c
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <ptk.h>
#include <ui.h>
#include <ui_server.h>
#include <LCUI/ui.h>
#include <LCUI/worker.h>
#include <LCUI/app.h>

static struct lcui_app {
        ptk_steptimer_t timer;
} lcui_app;

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
        e.text.text = wcsdup2(app_evt->text.text);
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

static void lcui_app_on_tick(ptk_steptimer_t *timer, void *data)
{
        lcui_ui_render();
        ptk_app_present();
}

static int lcui_app_dispatch(ptk_event_t *e)
{
        if (e->type == PTK_EVENT_QUIT) {
                return 0;
        }
        lcui_dispatch_ui_event(e);
        lcui_ui_update();
        lcui_worker_run();
        ptk_steptimer_tick(&lcui_app.timer, lcui_app_on_tick, NULL);
        return 0;
}

uint32_t lcui_app_get_fps(void)
{
        return lcui_app.timer.frames_per_second;
}

void lcui_app_set_frame_rate_cap(unsigned rate_cap)
{
        if (rate_cap > 0) {
                lcui_app.timer.target_elapsed_time = 1000 / rate_cap;
                lcui_app.timer.is_fixed_time_step = true;
        } else {
                lcui_app.timer.is_fixed_time_step = false;
        }
}

int lcui_app_process_events(ptk_process_events_option_t option)
{
        return ptk_process_native_events(option);
}

void lcui_app_init(void)
{
        lcui_worker_init();
        ptk_steptimer_init(&lcui_app.timer);
        lcui_app.timer.target_elapsed_time = 0;
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
        ptk_destroy();
}
