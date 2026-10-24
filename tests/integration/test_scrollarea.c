/*
 * tests/integration/test_scrollarea.c: -- ScrollArea widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>

static ui_widget_t *create_spacer(void)
{
        ui_widget_t *spacer = ui_create_widget(NULL);

        ui_widget_set_style_string(spacer, "height", "300px");
        return spacer;
}

static void test_nested_wheel(void)
{
        ui_widget_t *outer = ui_create_scrollarea();
        ui_widget_t *outer_content = ui_create_scrollarea_content();
        ui_widget_t *inner = ui_create_scrollarea();
        ui_widget_t *inner_content = ui_create_scrollarea_content();
        ui_event_t event = { 0 };

        ui_widget_set_style_string(outer, "height", "100px");
        ui_widget_set_style_string(inner, "height", "40px");
        ui_widget_append(outer_content, inner);
        ui_widget_append(inner_content, create_spacer());
        ui_widget_append(inner, inner_content);
        ui_widget_append(inner, ui_create_widget("scrollbar"));
        ui_widget_append(outer_content, create_spacer());
        ui_widget_append(outer, outer_content);
        ui_widget_append(outer, ui_create_widget("scrollbar"));
        ui_widget_append(ui_root(), outer);
        ui_update();

        ui_event_init(&event, "mousewheel");
        event.wheel.delta_y = -120;
        ui_widget_emit_event(inner, event, NULL);
        ui_update();
        ctest_equal_bool("inner should scroll first",
                         ui_scrollarea_get_scroll_top(inner) > 0.0f, true);
        ctest_equal_bool("outer should stay at the top",
                         ui_scrollarea_get_scroll_top(outer) == 0.0f, true);

        ui_scrollarea_set_scroll_top(inner, 1000.0f);
        ui_update();
        ui_widget_emit_event(inner, event, NULL);
        ui_update();
        ctest_equal_bool("outer should scroll at the inner bottom edge",
                         ui_scrollarea_get_scroll_top(outer) > 0.0f, true);

        ui_widget_destroy(outer);
}

void test_scrollarea(void)
{
        lcui_init();
        ctest_describe("scrollarea nested wheel", test_nested_wheel);
        lcui_destroy();
}
