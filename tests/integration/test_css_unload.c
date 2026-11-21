/*
 * tests/integration/test_css_unload.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <css/style_value.h>
#include <ctest-custom.h>

static void test_unload_css_string(void)
{
        ui_widget_t *w = ui_create_widget("text");
        size_t count;

        ui_load_css_string(".unload-target { color: #ff0000; }", "unload-test");
        ui_widget_add_class(w, "unload-target");
        ui_text_set_content(w, "Hello");
        ui_widget_append(ui_root(), w);
        ui_update();
        ctest_equal_bool(
            "should apply the loaded rule",
            w->computed_style.color == css_color(255, 0xff, 0x00, 0x00), true);

        count = ui_unload_css_string("unload-test");
        ui_update();
        ctest_equal_int("should remove the loaded rule", (int)count, 1);
        ctest_equal_bool(
            "should drop the removed color",
            w->computed_style.color == css_color(255, 0xff, 0x00, 0x00), false);

        ui_widget_destroy(w);
        ui_clear_trash();
}

void test_css_unload(void)
{
        lcui_init();

        ctest_describe("css unload", test_unload_css_string);

        lcui_destroy();
}
