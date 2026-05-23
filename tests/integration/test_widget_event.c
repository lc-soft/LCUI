/*
 * tests/cases/test_widget_event.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <LCUI.h>
#include <ui.h>
#include <ctest-custom.h>

void test_widget_mouse_event(void)
{
        ui_widget_t *root;
        ui_widget_t *parent, *child;
        ui_event_t ev;

        lcui_init();
        root = ui_root();
        parent = ui_create_widget("button");
        child = ui_create_widget("text");

        ui_widget_resize(root, 200, 200);
        ui_widget_resize(parent, 100, 100);
        ui_widget_resize(child, 50, 50);
        ui_widget_append(parent, child);
        ui_widget_append(root, parent);
        ui_update();

        ev.type = UI_EVENT_MOUSEMOVE;
        ev.mouse.x = 150;
        ev.mouse.y = 150;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool("should mark root as hover when mouse is at (150,150)",
                         ui_widget_has_status(root, "hover"), true);
        ctest_equal_bool(
            "should not mark parent as hover when mouse is at (150,150)",
            ui_widget_has_status(parent, "hover"), false);
        ctest_equal_bool(
            "should not mark child as hover when mouse is at (150,150)",
            ui_widget_has_status(child, "hover"), false);

        ev.mouse.x = 80;
        ev.mouse.y = 80;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool("should mark root as hover when mouse is at (80,80)",
                         ui_widget_has_status(root, "hover"), true);
        ctest_equal_bool("should mark parent as hover when mouse is at (80,80)",
                         ui_widget_has_status(parent, "hover"), true);
        ctest_equal_bool(
            "should not mark child as hover when mouse is at (80,80)",
            ui_widget_has_status(child, "hover"), false);

        ev.mouse.x = 40;
        ev.mouse.y = 40;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool("should mark root as hover when mouse is at (40,40)",
                         ui_widget_has_status(root, "hover"), true);
        ctest_equal_bool("should mark parent as hover when mouse is at (40,40)",
                         ui_widget_has_status(parent, "hover"), true);
        ctest_equal_bool("should mark child as hover when mouse is at (40,40)",
                         ui_widget_has_status(child, "hover"), true);

        ev.type = UI_EVENT_MOUSEDOWN;
        ev.mouse.x = 40;
        ev.mouse.y = 40;
        ev.mouse.button = MOUSE_BUTTON_LEFT;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool("should mark root as active on mousedown",
                         ui_widget_has_status(root, "active"), true);
        ctest_equal_bool("should mark parent as active on mousedown",
                         ui_widget_has_status(parent, "active"), true);
        ctest_equal_bool("should mark child as active on mousedown",
                         ui_widget_has_status(child, "active"), true);

        ev.type = UI_EVENT_MOUSEUP;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool("should clear root active state on mouseup",
                         ui_widget_has_status(root, "active"), false);
        ctest_equal_bool("should clear parent active state on mouseup",
                         ui_widget_has_status(parent, "active"), false);
        ctest_equal_bool("should clear child active state on mouseup",
                         ui_widget_has_status(child, "active"), false);

        ev.type = UI_EVENT_MOUSEMOVE;
        ev.mouse.x = 80;
        ev.mouse.y = 80;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool(
            "should keep root as hover when mouse moves back to (80,80)",
            ui_widget_has_status(root, "hover"), true);
        ctest_equal_bool(
            "should keep parent as hover when mouse moves back to (80,80)",
            ui_widget_has_status(parent, "hover"), true);
        ctest_equal_bool(
            "should clear child hover state when mouse moves back to (80,80)",
            ui_widget_has_status(child, "hover"), false);

        ev.mouse.x = 150;
        ev.mouse.y = 150;
        ui_dispatch_event(&ev);
        ui_update();
        ctest_equal_bool(
            "should keep root as hover when mouse leaves to (150,150)",
            ui_widget_has_status(root, "hover"), true);
        ctest_equal_bool(
            "should clear parent hover state when mouse leaves to (150,150)",
            ui_widget_has_status(parent, "hover"), false);
        ctest_equal_bool(
            "should clear child hover state when mouse leaves to (150,150)",
            ui_widget_has_status(child, "hover"), false);

        lcui_destroy();
}

void test_widget_event(void)
{
        ctest_describe("widget mouse event", test_widget_mouse_event);
}
