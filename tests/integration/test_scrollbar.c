/*
 * tests/cases/test_scrollbar.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include "../scenes/scrollbar_scene.h"

void test_scrollbar(void)
{
        float left, top;
        ui_event_t e = { 0 };
        ui_widget_t *content;

        lcui_init();
        scrollbar_scene_build();
        lcui_ui_update();

        content = ui_get_widget("license_content");
        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 300;
        e.mouse.y = 275;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = 300;
        e.mouse.y = 275;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 600;
        e.mouse.y = 275;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the left",
                         content->computed_style.left < left &&
                             top == content->computed_style.top,
                         true);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 400;
        e.mouse.y = 275;
        ui_dispatch_event(&e);
        e.type = UI_EVENT_MOUSEUP;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = 400;
        e.mouse.y = 275;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the right",
                         content->computed_style.left > left &&
                             top == content->computed_style.top,
                         true);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 555;
        e.mouse.y = 45;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = 555;
        e.mouse.y = 45;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 555;
        e.mouse.y = 200;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the top",
                         content->computed_style.left == left &&
                             top > content->computed_style.top,
                         true);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = 555;
        e.mouse.y = 100;
        ui_dispatch_event(&e);
        e.type = UI_EVENT_MOUSEUP;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = 555;
        e.mouse.y = 100;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the bottom",
                         content->computed_style.left == left &&
                             top < content->computed_style.top,
                         true);

        lcui_destroy();
}
