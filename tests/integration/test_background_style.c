/*
 * tests/integration/test_background_style.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Background image geometry.  `cover` resolves to concrete pixels using the
 * widget's used box, so the result must survive style refreshes and be
 * re-resolved when layout changes the box without a style refresh.
 */

#include <LCUI.h>
#include <math.h>
#include <ctest-custom.h>

static void settle(void)
{
        int i;

        for (i = 0; i < 40; ++i) {
                ui_update();
                ui_load_images();
        }
}

static ui_widget_t *create_background(ui_widget_t **parent_out)
{
        ui_widget_t *parent = ui_create_widget(NULL);
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(parent, "test-background-parent");
        ui_widget_add_class(w, "test-background");
        ui_widget_append(parent, w);
        ui_widget_append(ui_root(), parent);
        if (parent_out) {
                *parent_out = parent;
        }
        return w;
}

static void destroy_background(ui_widget_t *w)
{
        ui_widget_destroy(w->parent);
        ui_clear_trash();
}

static void test_cover_geometry(void)
{
        ui_widget_t *w = create_background(NULL);

        ui_widget_resize(ui_root(), 800, 600);
        settle();

        /* 30x20 image in a 200x100 box: cover scales by max(200/30, 100/20)
         * and centers the overflow vertically. */
        ctest_equal_bool(
            "cover width fills the box",
            fabsf(w->computed_style.background_width - 200.f) < 0.5f, true);
        ctest_equal_bool(
            "cover height keeps the image aspect",
            fabsf(w->computed_style.background_height - 133.33f) < 0.5f, true);
        ctest_equal_bool("cover centers vertically",
                         w->computed_style.background_position_y < -16.f, true);

        destroy_background(w);
}

static void test_hover_keeps_geometry(void)
{
        ui_widget_t *w = create_background(NULL);
        float width;
        float height;
        float x;
        float y;

        ui_widget_resize(ui_root(), 800, 600);
        settle();
        width = w->computed_style.background_width;
        height = w->computed_style.background_height;
        x = w->computed_style.background_position_x;
        y = w->computed_style.background_position_y;

        ui_widget_add_status(w, "hover");
        settle();

        ctest_equal_float("hover keeps the cover width",
                          w->computed_style.background_width, width);
        ctest_equal_float("hover keeps the cover height",
                          w->computed_style.background_height, height);
        ctest_equal_float("hover keeps the horizontal offset",
                          w->computed_style.background_position_x, x);
        ctest_equal_float("hover keeps the vertical offset",
                          w->computed_style.background_position_y, y);

        destroy_background(w);
}

static void test_layout_resize_resolves_cover(void)
{
        ui_widget_t *parent = NULL;
        ui_widget_t *w = create_background(&parent);

        ui_widget_resize(ui_root(), 800, 600);
        settle();
        ui_widget_add_class(parent, "test-background-parent-narrow");
        settle();

        /* 100x100 box: cover is max(100/30, 100/20) = 5, so the image becomes
         * 150x100 and is centered horizontally.  Only the parent style
         * changed, so this relies on layout re-resolving the background. */
        ctest_equal_bool(
            "layout resize recomputes the cover width",
            fabsf(w->computed_style.background_width - 150.f) < 0.5f, true);
        ctest_equal_bool(
            "layout resize recomputes the cover height",
            fabsf(w->computed_style.background_height - 100.f) < 0.5f, true);
        ctest_equal_bool(
            "layout resize recenters horizontally",
            fabsf(w->computed_style.background_position_x + 25.f) < 0.5f, true);
        ctest_equal_bool("layout resize clears the vertical offset",
                         fabsf(w->computed_style.background_position_y) < 0.5f,
                         true);

        destroy_background(w);
}

void test_background_style(void)
{
        lcui_init();
        ui_load_css_file("test_background.css");
        ui_widget_set_title(ui_root(), L"test background style");

        ctest_describe("background cover geometry", test_cover_geometry);
        ctest_describe("background hover stability", test_hover_keeps_geometry);
        ctest_describe("background layout resize",
                       test_layout_resize_resolves_cover);

        lcui_destroy();
}
