/*
 * tests/integration/test_window_events.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static int hover_a;
static int hover_b;

static void on_over_a(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++hover_a;
}

static void on_over_b(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++hover_b;
}

static void test_mouse_isolated_between_windows(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *wa, *wb;
        ui_widget_t *a, *b;
        ptk_event_t pe = { 0 };

        hover_a = 0;
        hover_b = 0;
        lcui_init();
        a = ui_create_widget(NULL);
        b = ui_create_widget(NULL);
        ui_widget_resize(a, 100, 100);
        ui_widget_resize(b, 100, 100);
        ui_root_append(a);
        ui_root_append(b);
        ui_widget_on(a, "mouseover", on_over_a, NULL);
        ui_widget_on(b, "mouseover", on_over_b, NULL);
        opt.width = 100;
        opt.height = 100;
        wa = lcui_window_create_for_widget(a, &opt);
        wb = lcui_window_create_for_widget(b, &opt);
        ui_update();

        pe.type = PTK_EVENT_MOUSEMOVE;
        pe.window = lcui_window_get_handle(wb);
        pe.mouse.x = 10;
        pe.mouse.y = 10;
        ptk_process_event(&pe);

        ctest_equal_int("should hover window b", hover_b, 1);
        ctest_equal_int("should not hover window a", hover_a, 0);
        lcui_window_destroy(wa);
        lcui_window_destroy(wb);
        ui_widget_destroy(a);
        ui_widget_destroy(b);
        lcui_destroy();
}

static int key_a;
static int key_b;

static void on_key_a(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++key_a;
}

static void on_key_b(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++key_b;
}

static void test_keyboard_routes_to_focused_window(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *wa, *wb;
        ui_widget_t *a, *b;
        ptk_event_t pe = { 0 };
        int code = 'A';

        key_a = 0;
        key_b = 0;
        lcui_init();
        a = ui_create_widget(NULL);
        b = ui_create_widget(NULL);
        a->tab_index = 0;
        b->tab_index = 0;
        ui_root_append(a);
        ui_root_append(b);
        ui_widget_on(a, "keydown", on_key_a, NULL);
        ui_widget_on(b, "keydown", on_key_b, NULL);
        opt.width = 100;
        opt.height = 100;
        wa = lcui_window_create_for_widget(a, &opt);
        wb = lcui_window_create_for_widget(b, &opt);
        ui_update();
        ui_set_focus(a);
        ui_set_focus(b);

        /* 模拟窗口 b 获得系统焦点 */
        pe.type = PTK_EVENT_FOCUS;
        pe.window = lcui_window_get_handle(wb);
        ptk_process_event(&pe);

        /* 键盘事件随后进入 lcui_app_dispatch，应路由到 b 的 scope */
        pe.type = PTK_EVENT_KEYDOWN;
        pe.window = lcui_window_get_handle(wb);
        pe.key.code = code;
        ptk_process_event(&pe);

        ctest_equal_int("should route key to b", key_b, 1);
        ctest_equal_int("should not route key to a", key_a, 0);
        lcui_window_destroy(wa);
        lcui_window_destroy(wb);
        ui_widget_destroy(a);
        ui_widget_destroy(b);
        lcui_destroy();
}

static int move_hits;

static void on_content_move(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++move_hits;
}

static void test_destroyed_capturer_is_released(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *win;
        ui_widget_t *content, *child;
        ui_event_t e = { 0 };

        move_hits = 0;
        lcui_init();
        content = ui_create_widget(NULL);
        child = ui_create_widget(NULL);
        ui_widget_resize(content, 200, 100);
        ui_widget_resize(child, 50, 50);
        ui_widget_append(content, child);
        ui_root_append(content);
        ui_widget_on(content, "mousemove", on_content_move, NULL);
        win = lcui_window_create_for_widget(content, &opt);
        ui_update();
        ui_widget_set_mouse_capture(child);
        ui_widget_destroy(child);

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 10;
        e.mouse.y = 10;
        ui_dispatch_event_to(content, &e);
        ctest_equal_int("should ignore the destroyed capturer", move_hits, 1);
        lcui_window_destroy(win);
        ui_widget_destroy(content);
        lcui_destroy();
}

static void test_scopes_are_independent(void)
{
        ui_widget_t *a, *b, *a_child, *b_child;

        lcui_init();
        a = ui_create_widget(NULL);
        b = ui_create_widget(NULL);
        a_child = ui_create_widget(NULL);
        b_child = ui_create_widget(NULL);
        a_child->tab_index = 0;
        b_child->tab_index = 0;
        ui_widget_append(a, a_child);
        ui_widget_append(b, b_child);
        ui_root_append(a);
        ui_root_append(b);
        ui_set_event_scope(a);
        ui_set_event_scope(b);

        ui_set_focus(a_child);
        ui_set_focus(b_child);
        ctest_equal_bool("focus a should stay independent",
                         ui_get_focus_in(a) == a_child, true);
        ctest_equal_bool("focus b should stay independent",
                         ui_get_focus_in(b) == b_child, true);
        ui_widget_destroy(a);
        ui_widget_destroy(b);
        lcui_destroy();
}

void test_window_events(void)
{
        ctest_describe("event scope", test_scopes_are_independent);
        ctest_describe("pointer routing", test_mouse_isolated_between_windows);
        ctest_describe("keyboard routing",
                       test_keyboard_routes_to_focused_window);
        ctest_describe("capturer lifetime",
                       test_destroyed_capturer_is_released);
}
