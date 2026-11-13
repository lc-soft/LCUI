/*
 * tests/integration/test_flex_hidden_gap.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Hidden children (display: none) must not contribute a flex gap.  A row
 * that only exposes one fixed-size item must keep that item at its own
 * width instead of shrinking it by the gap that would precede a hidden
 * sibling.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static ui_widget_t *create_box(const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(w, class_name);
        return w;
}

static void test_hidden_sibling_gap(void)
{
        ui_widget_t *row = create_box("test-hg-row");
        ui_widget_t *hidden = create_box("test-hg-hidden");
        ui_widget_t *visible = create_box("test-hg-visible");

        ui_widget_append(row, hidden);
        ui_widget_append(row, visible);
        ui_widget_append(ui_root(), row);
        ui_widget_resize(ui_root(), 800, 600);
        ui_update();
        ui_update();
        ui_update();

        ctest_equal_float("visible item keeps its own width",
                          visible->border_box.width, 28.f);
        ctest_equal_float("row width excludes the hidden sibling",
                          row->content_box.width, 28.f);

        ui_widget_destroy(row);
        ui_clear_trash();
}

static void test_visible_first(void)
{
        ui_widget_t *row = create_box("test-hg-row");
        ui_widget_t *visible = create_box("test-hg-visible");
        ui_widget_t *hidden = create_box("test-hg-hidden");

        ui_widget_append(row, visible);
        ui_widget_append(row, hidden);
        ui_widget_append(ui_root(), row);
        ui_widget_resize(ui_root(), 800, 600);
        ui_update();
        ui_update();
        ui_update();

        ctest_equal_float("visible first keeps its own width",
                          visible->border_box.width, 28.f);
        ctest_equal_float("visible first row width", row->content_box.width,
                          28.f);

        ui_widget_destroy(row);
        ui_clear_trash();
}

void test_flex_hidden_gap(void)
{
        lcui_init();
        ui_load_css_file("test_flex_hidden_gap.css");
        ui_widget_set_title(ui_root(), L"test flex hidden gap");

        ctest_describe("flex hidden gap", test_hidden_sibling_gap);
        ctest_describe("flex visible first", test_visible_first);

        lcui_destroy();
}
