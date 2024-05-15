/*
 * tests/cases/test_widget_event.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
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
	ctest_equal_bool("mousemove(150, 150): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), true);
	ctest_equal_bool("mousemove(150, 150): parent.hasStatus('hover') == false",
	     ui_widget_has_status(parent, "hover"), false);
	ctest_equal_bool("mousemove(150, 150): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), false);

	ev.mouse.x = 80;
	ev.mouse.y = 80;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(80, 80): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), true);
	ctest_equal_bool("mousemove(80, 80): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), true);
	ctest_equal_bool("mousemove(80, 80): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), false);

	ev.mouse.x = 40;
	ev.mouse.y = 40;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(40, 40): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), true);
	ctest_equal_bool("mousemove(40, 40): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), true);
	ctest_equal_bool("mousemove(40, 40): child.hasStatus('hover') == true",
	     ui_widget_has_status(child, "hover"), true);

	ev.type = UI_EVENT_MOUSEDOWN;
	ev.mouse.x = 40;
	ev.mouse.y = 40;
	ev.mouse.button = MOUSE_BUTTON_LEFT;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousedown(40, 40): root.hasStatus('active') == true",
	     ui_widget_has_status(root, "active"), true);
	ctest_equal_bool("mousedown(40, 40): parent.hasStatus('active') == true",
	     ui_widget_has_status(parent, "active"), true);
	ctest_equal_bool("mousedown(40, 40): child.hasStatus('active') == true",
	     ui_widget_has_status(child, "active"), true);

	ev.type = UI_EVENT_MOUSEUP;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mouseup(40, 40): root.hasStatus('active') == false",
	     ui_widget_has_status(root, "active"), false);
	ctest_equal_bool("mouseup(40, 40): parent.hasStatus('active') == false",
	     ui_widget_has_status(parent, "active"), false);
	ctest_equal_bool("mouseup(40, 40): child.hasStatus('active') == false",
	     ui_widget_has_status(child, "active"), false);

	ev.type = UI_EVENT_MOUSEMOVE;
	ev.mouse.x = 80;
	ev.mouse.y = 80;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(80, 80): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), true);
	ctest_equal_bool("mousemove(80, 80): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), true);
	ctest_equal_bool("mousemove(80, 80): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), false);

	ev.mouse.x = 150;
	ev.mouse.y = 150;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(150, 150): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), true);
	ctest_equal_bool("mousemove(150, 150): parent.hasStatus('hover') == false",
	     ui_widget_has_status(parent, "hover"), false);
	ctest_equal_bool("mousemove(150, 150): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), false);

	lcui_quit();
	lcui_main();
}

void test_widget_event(void)
{
	ctest_describe("test widget mouse event", test_widget_mouse_event);
}
