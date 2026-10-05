/*
 * tests/integration/test_scrollbar.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include "../scenes/scrollbar_scene.h"

/* 在滚动条部件子树里找到 scrollbar-thumb 子部件 */
static ui_widget_t *find_scrollbar_thumb(ui_widget_t *w)
{
        ui_widget_t *child, *found;
        size_t i;

        if (!w) {
                return NULL;
        }
        if (ui_check_widget_type(w, "scrollbar-thumb")) {
                return w;
        }
        for (i = 0; (child = ui_widget_get_child(w, i)) != NULL; ++i) {
                found = find_scrollbar_thumb(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

/* 累加各级 border_box 偏移，得到部件在窗口坐标系里的中心 */
static void widget_abs_center(ui_widget_t *w, float *x, float *y)
{
        ui_widget_t *p;

        if (!w) {
                *x = 0;
                *y = 0;
                return;
        }
        *x = w->border_box.x + w->border_box.width / 2.f;
        *y = w->border_box.y + w->border_box.height / 2.f;
        for (p = w->parent; p; p = p->parent) {
                *x += p->border_box.x;
                *y += p->border_box.y;
        }
}

void test_scrollbar(void)
{
        float left, top;
        float hx, hy, vx, vy;
        ui_event_t e = { 0 };
        ui_widget_t *content;
        ui_widget_t *container;

        lcui_init();
        scrollbar_scene_build();
        lcui_ui_update();

        content = ui_get_widget("license_content");
        container = content->parent;
        /* 动态取横/纵滚动条 thumb 的真实位置，避免依赖写死的像素坐标 */
        widget_abs_center(
            find_scrollbar_thumb(ui_widget_get_child(container, 2)), &hx, &hy);
        widget_abs_center(
            find_scrollbar_thumb(ui_widget_get_child(container, 1)), &vx, &vy);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = hx;
        e.mouse.y = hy;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = hx;
        e.mouse.y = hy;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = hx + 200;
        e.mouse.y = hy;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the left",
                         content->computed_style.left < left &&
                             top == content->computed_style.top,
                         true);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = hx;
        e.mouse.y = hy;
        ui_dispatch_event(&e);
        e.type = UI_EVENT_MOUSEUP;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = hx;
        e.mouse.y = hy;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the right",
                         content->computed_style.left > left &&
                             top == content->computed_style.top,
                         true);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = vx;
        e.mouse.y = vy;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = vx;
        e.mouse.y = vy;
        ui_dispatch_event(&e);
        lcui_ui_update();

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = vx;
        e.mouse.y = vy + 155;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the top",
                         content->computed_style.left == left &&
                             top > content->computed_style.top,
                         true);

        left = content->computed_style.left;
        top = content->computed_style.top;

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = vx;
        e.mouse.y = vy + 55;
        ui_dispatch_event(&e);
        e.type = UI_EVENT_MOUSEUP;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = vx;
        e.mouse.y = vy + 55;
        ui_dispatch_event(&e);
        lcui_ui_update();

        ctest_equal_bool("should scroll content to the bottom",
                         content->computed_style.left == left &&
                             top < content->computed_style.top,
                         true);

        lcui_destroy();
}
