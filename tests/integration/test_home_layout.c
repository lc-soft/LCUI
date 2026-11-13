/*
 * tests/integration/test_home_layout.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * The doc-viewer home page structure: scroll area -> page -> centered
 * max-width container -> four-column card gallery with a text input inside
 * a card.  These tests pin down that a focus-driven style refresh must not
 * collapse the percentage-sized containers between layout passes, and that
 * the columns can be sized from the container width without oscillating.
 */

#include <LCUI.h>
#include <css.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <ctest-custom.h>

#define HOME_COLUMNS 4
#define HOME_GAP 24.0f

typedef struct {
        ui_widget_t *scroll;
        ui_widget_t *page;
        ui_widget_t *inner;
        ui_widget_t *grid;
        ui_widget_t *columns[HOME_COLUMNS];
        ui_widget_t *input;
        float min_inner_width;
        float max_inner_width;
        float min_column_width;
        float max_column_width;
        int layout_passes;
} home_scene_t;

static home_scene_t *home_scene;

static ui_widget_t *create_box(const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(w, class_name);
        return w;
}

static void home_scene_record(ui_widget_t *w, ui_event_t *e, void *emit_arg)
{
        home_scene_t *scene = home_scene;
        float width = w->content_box.width;

        if (!scene || width <= 0) {
                return;
        }
        scene->layout_passes++;
        if (width < scene->min_inner_width) {
                scene->min_inner_width = width;
        }
        if (width > scene->max_inner_width) {
                scene->max_inner_width = width;
        }
}

/* Mirrors `home_grid_update_visibility()`: size each column from the
 * container width in pixels, like the app does. */
static void home_scene_sync_columns(ui_widget_t *w, ui_event_t *e, void *arg)
{
        home_scene_t *scene = home_scene;
        float available;
        float width;
        int i;

        if (!scene) {
                return;
        }
        available = scene->inner->content_box.width;
        if (available <= 100.0f) {
                return;
        }
        width = (available - HOME_GAP * (HOME_COLUMNS - 1)) / HOME_COLUMNS;
        if (width < scene->min_column_width) {
                scene->min_column_width = width;
        }
        if (width > scene->max_column_width) {
                scene->max_column_width = width;
        }
        for (i = 0; i < HOME_COLUMNS; ++i) {
                ui_widget_set_style_unit_value(
                    scene->columns[i], css_prop_width, width, CSS_UNIT_PX);
        }
}

static void home_scene_build(void)
{
        ui_widget_t *content = ui_create_scrollarea_content();
        ui_widget_t *column;
        ui_widget_t *card;
        ui_widget_t *filler;
        ui_widget_t *input;
        int i;

        home_scene = calloc(1, sizeof(*home_scene));
        home_scene->scroll = ui_create_scrollarea();
        home_scene->page = create_box("test-home-page");
        home_scene->inner = create_box("test-home-inner");
        home_scene->grid = create_box("test-home-grid");

        ui_widget_add_class(home_scene->scroll, "test-home-scroll");
        ui_widget_append(home_scene->scroll, content);
        ui_widget_append(home_scene->scroll, ui_create_scrollbar());
        ui_widget_append(content, home_scene->page);
        ui_widget_append(home_scene->page, home_scene->inner);
        ui_widget_append(home_scene->inner, home_scene->grid);

        for (i = 0; i < HOME_COLUMNS; ++i) {
                column = create_box("test-home-column");
                home_scene->columns[i] = column;
                ui_widget_append(home_scene->grid, column);
                card = create_box("test-home-card");
                ui_widget_append(column, card);
                if (i == 0) {
                        ui_widget_t *group = ui_create_input_group();

                        input = ui_create_widget("textinput");
                        ui_widget_add_class(input, "test-home-input");
                        ui_widget_append(group, input);
                        ui_widget_append(card, group);
                        home_scene->input = input;
                } else {
                        filler = create_box("test-home-filler");
                        ui_widget_append(card, filler);
                }
        }
        ui_widget_append(ui_root(), home_scene->scroll);

        ui_widget_on(home_scene->grid, "afterlayout", home_scene_record,
                     home_scene);
        ui_widget_on(home_scene->grid, "afterlayout", home_scene_sync_columns,
                     home_scene);
}

static void home_scene_resize(float width, float height)
{
        ui_widget_resize(ui_root(), width, height);
        ui_update();
        ui_update();
        ui_update();
}

static void home_scene_destroy(void)
{
        ui_widget_destroy(home_scene->scroll);
        ui_clear_trash();
        free(home_scene);
        home_scene = NULL;
}

static void test_structure(void)
{
        home_scene_resize(1400, 900);

        ctest_equal_bool("inner keeps the page width",
                         home_scene->inner->content_box.width > 1000.0f, true);
        ctest_equal_bool("page fills the scroll area",
                         home_scene->page->content_box.width > 1000.0f, true);
        ctest_equal_bool("grid fills the inner container",
                         fabsf(home_scene->grid->content_box.width -
                               home_scene->inner->content_box.width) < 1.0f,
                         true);
}

static void test_columns_converge(void)
{
        float spread;

        home_scene->min_column_width = FLT_MAX;
        home_scene->max_column_width = 0.0f;
        home_scene_resize(1200, 800);
        ui_update();
        ui_update();
        spread = home_scene->max_column_width - home_scene->min_column_width;
        ctest_equal_bool("column width settles after resizing", spread < 1.0f,
                         true);
        ctest_equal_bool("column stays inside the grid",
                         home_scene->columns[0]->border_box.width <
                             home_scene->grid->content_box.width,
                         true);
}

static void test_focus_keeps_ancestor_widths(void)
{
        float inner_width;
        float page_width;

        home_scene_resize(1400, 900);
        inner_width = home_scene->inner->content_box.width;
        page_width = home_scene->page->content_box.width;
        home_scene->min_inner_width = FLT_MAX;
        home_scene->max_inner_width = 0.0f;
        home_scene->layout_passes = 0;

        ui_set_focus(home_scene->input);
        ui_widget_add_class(home_scene->page, "test-home-page-active");
        ui_scrollarea_set_scroll_top(home_scene->scroll, 120);
        ui_update();
        ui_update();
        ui_update();

        ctest_equal_bool(
            "focus keeps the inner width",
            fabsf(home_scene->inner->content_box.width - inner_width) < 0.5f,
            true);
        ctest_equal_bool(
            "focus keeps the page width",
            fabsf(home_scene->page->content_box.width - page_width) < 0.5f,
            true);
        ctest_equal_bool("layout passes observe the stable inner width",
                         home_scene->layout_passes > 0 &&
                             home_scene->min_inner_width > inner_width * 0.9f,
                         true);
}

void test_home_layout(void)
{
        lcui_init();
        ui_load_css_file("test_home_layout.css");
        ui_widget_set_title(ui_root(), L"test home layout");
        home_scene_build();

        ctest_describe("home page structure", test_structure);
        ctest_describe("home columns converge", test_columns_converge);
        ctest_describe("home focus stability",
                       test_focus_keeps_ancestor_widths);

        home_scene_destroy();
        lcui_destroy();
}
