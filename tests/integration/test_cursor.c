/*
 * tests/integration/test_cursor.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ui_server.h>
#include <ctest-custom.h>
#include "../scenes/cursor_scene.h"

static ptk_window_t *window;

static void move_mouse(int x, int y)
{
        ui_event_t e = { 0 };

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = (float)x;
        e.mouse.y = (float)y;
        ui_dispatch_event(&e);
        lcui_ui_update();
}

static void should_default_window_cursor_to_auto(void)
{
        ctest_equal_int("should default window cursor to auto",
                        ptk_window_get_cursor(window), PTK_CURSOR_AUTO);
}

static void should_apply_explicit_cursor_on_hovered_widget(void)
{
        move_mouse(190, 30);
        ctest_equal_int("should show pointer over cursor:pointer widget",
                        ptk_window_get_cursor(window), PTK_CURSOR_POINTER);
}

static void should_inherit_cursor_from_nearest_ancestor(void)
{
        move_mouse(160, 10);
        ctest_equal_int("should show pointer over child without cursor",
                        ptk_window_get_cursor(window), PTK_CURSOR_POINTER);
}

static void should_let_child_value_truncate_ancestor(void)
{
        move_mouse(220, 10);
        ctest_equal_int("should show default over child with cursor:default",
                        ptk_window_get_cursor(window), PTK_CURSOR_DEFAULT);
}

static void should_resolve_hover_rule_cursor_in_the_same_frame(void)
{
        move_mouse(60, 70);
        ctest_equal_int("should show crosshair by :hover rule without extra "
                        "mouse move",
                        ptk_window_get_cursor(window), PTK_CURSOR_CROSSHAIR);
}

static void should_apply_none_cursor(void)
{
        move_mouse(190, 70);
        ctest_equal_int("should hide pointer over cursor:none widget",
                        ptk_window_get_cursor(window), PTK_CURSOR_NONE);
}

static void should_reset_to_auto_over_plain_area(void)
{
        move_mouse(60, 20);
        ctest_equal_int("should restore auto over widget without cursor",
                        ptk_window_get_cursor(window), PTK_CURSOR_AUTO);
}

static void should_default_button_cursor_to_pointer(void)
{
        move_mouse(60, 120);
        ctest_equal_int("should show pointer over real button",
                        ptk_window_get_cursor(window), PTK_CURSOR_POINTER);
}

static void should_default_textinput_cursor_to_text(void)
{
        move_mouse(190, 120);
        ctest_equal_int("should show text caret over real textinput",
                        ptk_window_get_cursor(window), PTK_CURSOR_TEXT);
}

static void should_clear_cursor_when_hovered_widget_destroyed(void)
{
        ui_widget_t *tile = ui_get_widget("tile-inherit");

        move_mouse(160, 10);
        ctest_equal_int("should show pointer before destroy",
                        ptk_window_get_cursor(window), PTK_CURSOR_POINTER);
        ui_widget_destroy(tile);
        lcui_ui_update();
        ctest_equal_int("should reset to auto after hovered widget destroyed",
                        ptk_window_get_cursor(window), PTK_CURSOR_AUTO);
}

void test_cursor(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *win;

        lcui_init();
        cursor_scene_build();
        win = lcui_window_create_for_widget(ui_root(), &opt);
        ctest_equal_bool("should create a window", win != NULL, true);
        lcui_ui_update();
        window = ui_server_get_window(ui_root());

        ctest_describe("initial state", should_default_window_cursor_to_auto);
        ctest_describe("explicit cursor",
                       should_apply_explicit_cursor_on_hovered_widget);
        ctest_describe("inherited cursor",
                       should_inherit_cursor_from_nearest_ancestor);
        ctest_describe("truncated cursor",
                       should_let_child_value_truncate_ancestor);
        ctest_describe("hover rule cursor",
                       should_resolve_hover_rule_cursor_in_the_same_frame);
        ctest_describe("none cursor", should_apply_none_cursor);
        ctest_describe("reset to auto", should_reset_to_auto_over_plain_area);
        ctest_describe("button default cursor",
                       should_default_button_cursor_to_pointer);
        ctest_describe("textinput default cursor",
                       should_default_textinput_cursor_to_text);
        ctest_describe("destroyed hover target",
                       should_clear_cursor_when_hovered_widget_destroyed);

        lcui_destroy();
}
