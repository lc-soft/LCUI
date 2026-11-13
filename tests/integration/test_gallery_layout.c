/*
 * tests/integration/test_gallery_layout.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * The doc-viewer home gallery is a home-grid widget inside a scroll area.
 * Flexible columns must follow both programmatic and native window resizes,
 * and a hover style refresh inside a card must not reflow the card at the
 * page's max-content width before the column width is applied.
 *
 * home-grid lives in the doc-viewer app.  Compiling that single component
 * into this target keeps the test against the real implementation instead
 * of a copy of its layout code.
 */

#include <LCUI.h>
#include <css.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctest-custom.h>
#include "../../examples/doc-viewer/app/global.css.h"

#define HOME_GRID_COLUMNS 4
#define HOME_GAP 24.0f
#define PAGE_GUTTER 96.0f
#define PAGE_MAX_WIDTH 1560.0f

void ui_register_home_grid(void);
void ui_widget_get_sizehint(ui_widget_t *w, ui_sizehint_t *hint);

typedef struct {
        ui_widget_t *scroll;
        ui_widget_t *page;
        ui_widget_t *inner;
        ui_widget_t *grid;
        ui_widget_t *cards[HOME_GRID_COLUMNS];
        ui_widget_t *bodies[HOME_GRID_COLUMNS];
        ui_widget_t *columns[HOME_GRID_COLUMNS];
        ui_widget_t *accordion;
        ui_widget_t *accordion_triggers[2];
        ui_widget_t *accordion_contents[2];
        ui_widget_t *production_card;
        ui_widget_t *production_desc;
        ui_widget_t *production_icon_box;
        ui_widget_t *production_icon;
        ui_widget_t *empty_widget;
        ui_widget_t *empty_header;
        ui_widget_t *empty_desc;
        ui_widget_t *empty_content;
        ui_widget_t *empty_button;
        float min_empty_height;
        float max_empty_height;
        int empty_height_passes;
        float min_card_width;
        float max_card_width;
        float min_card_height;
        float max_card_height;
        int layout_passes;
} home_grid_scene_t;

static home_grid_scene_t *scene;

static ui_widget_t *create_box(const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(w, class_name);
        return w;
}

static void home_scene_collect_columns(ui_widget_t *w, void *arg)
{
        home_grid_scene_t *s = arg;
        int i;

        if (!ui_check_widget_type(w, "home-column")) {
                return;
        }
        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                if (!s->columns[i]) {
                        s->columns[i] = w;
                        return;
                }
        }
}

static void home_scene_record_card(ui_widget_t *w, ui_event_t *e, void *arg)
{
        home_grid_scene_t *s = e->data;
        ui_widget_t *card = w->parent ? w->parent : w;
        float width = card->border_box.width;
        float height = w->border_box.height;

        if (width <= 0) {
                return;
        }
        ++s->layout_passes;
        if (width < s->min_card_width) {
                s->min_card_width = width;
        }
        if (width > s->max_card_width) {
                s->max_card_width = width;
        }
        if (s->min_card_height == 0 || height < s->min_card_height) {
                s->min_card_height = height;
        }
        if (height > s->max_card_height) {
                s->max_card_height = height;
        }
}

static void home_scene_record_empty(ui_widget_t *w, ui_event_t *e, void *arg)
{
        home_grid_scene_t *s = e->data;
        float height = w->border_box.height;

        if (height <= 0) {
                return;
        }
        if (height < s->min_empty_height) {
                s->min_empty_height = height;
        }
        if (height > s->max_empty_height) {
                s->max_empty_height = height;
        }
        ++s->empty_height_passes;
}

static void home_scene_build_accordion(ui_widget_t *body)
{
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *first = ui_create_accordion_item();
        ui_widget_t *second = ui_create_accordion_item();
        ui_widget_t *first_text = ui_create_widget("text");
        ui_widget_t *second_text = ui_create_widget("text");

        scene->accordion = ui_create_accordion();
        scene->accordion_triggers[0] = ui_create_accordion_trigger();
        scene->accordion_triggers[1] = ui_create_accordion_trigger();
        scene->accordion_contents[0] = ui_create_accordion_content();
        scene->accordion_contents[1] = ui_create_accordion_content();
        ui_text_set_content(title, "Subscription & Billing");
        ui_text_set_content(description, "Common questions about your plan.");
        ui_text_set_content(first_text, "What plans are available?");
        ui_text_set_content(second_text,
                            "Billing occurs automatically at the start of each "
                            "cycle. We accept all major credit cards, PayPal "
                            "and ACH transfers. Enterprise customers can also "
                            "request invoices and review every past payment "
                            "before changing their subscription.");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(first, scene->accordion_triggers[0]);
        ui_widget_append(first, scene->accordion_contents[0]);
        ui_widget_append(scene->accordion_contents[0], first_text);
        ui_widget_append(second, scene->accordion_triggers[1]);
        ui_widget_append(second, scene->accordion_contents[1]);
        ui_widget_append(scene->accordion_contents[1], second_text);
        ui_accordion_item_set_value(first, "plans");
        ui_accordion_item_set_value(second, "billing");
        ui_accordion_item_set_open(first, true);
        ui_widget_append(scene->accordion, first);
        ui_widget_append(scene->accordion, second);
        ui_widget_append(body, header);
        ui_widget_append(body, scene->accordion);
}

static void home_scene_build_production_empty(void)
{
        ui_widget_t *body = ui_create_card_content();
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header = ui_create_empty_header();
        ui_widget_t *icon_box = ui_create_widget(NULL);
        ui_widget_t *icon = ui_create_fluent_icon("add");
        ui_widget_t *title = ui_create_empty_title();
        ui_widget_t *content = ui_create_empty_content();
        ui_widget_t *button = ui_create_button();

        scene->production_desc = ui_create_empty_description();
        scene->production_icon_box = icon_box;
        scene->production_icon = icon;
        scene->production_card = ui_create_card();
        ui_widget_add_class(scene->production_card, "home-card");
        ui_widget_set_attr(scene->production_card, "data-column", "4");
        ui_widget_add_class(body, "home-card-body home-empty");
        ui_widget_add_class(icon_box, "home-empty-icon");
        ui_text_set_content(title, "Distribute Track");
        ui_text_set_content(scene->production_desc,
                            "Upload your first master to start reaching "
                            "listeners on Spotify, Apple Music, and more.");
        ui_button_set_text(button, "Create Release");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);
        ui_widget_append(icon_box, icon);
        ui_widget_append(header, icon_box);
        ui_widget_append(header, title);
        ui_widget_append(header, scene->production_desc);
        ui_widget_append(content, button);
        ui_widget_append(empty, header);
        ui_widget_append(empty, content);
        ui_widget_append(body, empty);
        ui_widget_append(scene->production_card, body);
        ui_widget_append(scene->grid, scene->production_card);
}

static void home_scene_build(void)
{
        ui_widget_t *content = ui_create_scrollarea_content();
        int i;
        char column[8];

        scene = calloc(1, sizeof(*scene));
        scene->scroll = ui_create_scrollarea();
        scene->page = create_box("test-home-page");
        scene->inner = create_box("test-home-inner");
        scene->grid = ui_create_widget("home-grid");

        ui_widget_add_class(scene->scroll, "test-home-scroll");
        ui_widget_append(scene->scroll, content);
        ui_widget_append(scene->scroll, ui_create_scrollbar());
        ui_widget_append(content, scene->page);
        ui_widget_append(scene->page, scene->inner);
        ui_widget_append(scene->inner, scene->grid);

        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                scene->cards[i] = ui_create_card();
                ui_widget_add_class(scene->cards[i], "test-home-card");
                scene->bodies[i] = ui_create_card_content();
                ui_widget_add_class(scene->bodies[i], "test-home-card-body");
                if (i == HOME_GRID_COLUMNS - 1) {
                        ui_widget_t *empty = ui_create_empty();
                        ui_widget_t *header = ui_create_empty_header();
                        ui_widget_t *description =
                            ui_create_empty_description();
                        ui_widget_t *empty_content = ui_create_empty_content();
                        ui_widget_t *button = ui_create_button();

                        ui_text_set_content(
                            description,
                            "Upload your first master to start reaching "
                            "listeners on Spotify, Apple Music, and more.");
                        ui_button_set_text(button, "Create Release");
                        ui_button_set_variant(button,
                                              UI_BUTTON_VARIANT_PRIMARY);
                        ui_widget_append(header, description);
                        ui_widget_append(empty_content, button);
                        ui_widget_append(empty, header);
                        ui_widget_append(empty, empty_content);
                        ui_widget_add_class(scene->bodies[i],
                                            "test-home-empty");
                        ui_widget_add_class(scene->bodies[i], "clip-safe");
                        ui_widget_append(scene->bodies[i], empty);
                        scene->empty_widget = empty;
                        scene->empty_header = header;
                        scene->empty_desc = description;
                        scene->empty_content = empty_content;
                        scene->empty_button = button;
                } else if (i == 1) {
                        home_scene_build_accordion(scene->bodies[i]);
                } else {
                        ui_widget_append(scene->bodies[i],
                                         create_box("test-home-body-block"));
                }
                ui_widget_append(scene->cards[i], scene->bodies[i]);
                snprintf(column, sizeof(column), "%d", i + 1);
                ui_widget_set_attr(scene->cards[i], "data-column", column);
                ui_widget_append(scene->grid, scene->cards[i]);
        }
        home_scene_build_production_empty();
        ui_widget_append(ui_root(), scene->scroll);
        ui_widget_on(scene->bodies[HOME_GRID_COLUMNS - 1], "afterlayout",
                     home_scene_record_card, scene);
        ui_widget_on(scene->empty_desc, "afterlayout", home_scene_record_empty,
                     scene);
}

static void home_scene_update(void)
{
        int i;

        /* The resize pass applies flex widths and queues breakpoint changes
         * from root afterlayout; the following pass applies those changes. */
        for (i = 0; i < 2; ++i) {
                ui_update();
        }
        ui_widget_each(scene->grid, home_scene_collect_columns, scene);
}

static void home_scene_resize(float width, float height)
{
        ui_widget_resize(ui_root(), width, height);
        home_scene_update();
}

static void home_scene_native_resize(float width, float height)
{
        ptk_window_t *window = ui_server_get_window(ui_root());
        float scale = ui_get_actual_scale();
        int i;

        ptk_window_set_size(window, (int)(width * scale),
                            (int)(height * scale));
        for (i = 0; i < 8; ++i) {
                ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
                ui_update();
                y_sleep(10);
        }
}

static int home_scene_visible_columns(void)
{
        int i;
        int count = 0;

        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                if (!scene->columns[i]) {
                        continue;
                }
                if (css_computed_display(&scene->columns[i]->computed_style) !=
                    CSS_DISPLAY_NONE) {
                        ++count;
                }
        }
        return count;
}

static float home_scene_expected_column_width(float root_width)
{
        float page_width =
            root_width < PAGE_MAX_WIDTH ? root_width : PAGE_MAX_WIDTH;
        float available = page_width - PAGE_GUTTER;
        int columns = root_width >= 1400.0f ? 4 : root_width >= 1024.0f ? 3 : 2;

        return (available - HOME_GAP * (columns - 1)) / columns;
}

static void home_scene_destroy(void)
{
        ui_widget_destroy(scene->scroll);
        ui_clear_trash();
        free(scene);
        scene = NULL;
}

static void test_columns_follow_window(void)
{
        static const float widths[] = { 1600.0f, 1100.0f, 800.0f, 1500.0f };
        static const int visible[] = { 4, 3, 2, 4 };
        float expected;
        int i;
        int j;

        home_scene_resize(1600.0f, 1000.0f);
        for (i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i) {
                if (i > 0) {
                        home_scene_resize(widths[i], 1000.0f);
                }
                expected = home_scene_expected_column_width(widths[i]);
                ctest_equal_int("should keep the breakpoint column count",
                                home_scene_visible_columns(), visible[i]);
                ctest_equal_float(
                    "gallery should fit the available page width",
                    scene->grid->content_box.width,
                    expected * visible[i] + HOME_GAP * (visible[i] - 1));
                ctest_equal_float(
                    "page content should follow the window width",
                    scene->inner->content_box.width,
                    expected * visible[i] + HOME_GAP * (visible[i] - 1));
                for (j = 0; j < HOME_GRID_COLUMNS; ++j) {
                        if (!scene->columns[j]) {
                                continue;
                        }
                        if (css_computed_display(
                                &scene->columns[j]->computed_style) ==
                            CSS_DISPLAY_NONE) {
                                continue;
                        }
                        ctest_equal_float(
                            "visible column should follow the window width",
                            scene->columns[j]->border_box.width, expected);
                        ctest_equal_float(
                            "visible column should share free space with flex "
                            "grow",
                            scene->columns[j]->computed_style.flex_grow, 1.0f);
                }
        }
}

typedef struct {
        ui_widget_t *columns[HOME_GRID_COLUMNS];
        int count;
} gallery_columns_t;

static void collect_gallery_column(ui_widget_t *w, void *arg)
{
        gallery_columns_t *columns = arg;

        if (!ui_check_widget_type(w, "home-column") ||
            columns->count >= HOME_GRID_COLUMNS) {
                return;
        }
        columns->columns[columns->count++] = w;
}

static void test_initial_mount_single_pass(void)
{
        gallery_columns_t columns = { 0 };
        ui_widget_t *parent = create_box("test-home-page");
        ui_widget_t *grid = ui_create_widget("home-grid");
        float expected;
        int i;

        ui_widget_set_style_string(parent, "display", "flex");
        ui_widget_set_style_string(parent, "flex-direction", "column");
        ui_widget_set_style_string(parent, "width", "1600px");
        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                ui_widget_t *card = ui_create_card();
                char column[8];

                ui_widget_add_class(card, "test-home-card");
                snprintf(column, sizeof(column), "%d", i + 1);
                ui_widget_set_attr(card, "data-column", column);
                ui_widget_append(card, create_box("test-home-body-block"));
                ui_widget_append(grid, card);
        }
        ui_widget_append(parent, grid);
        ui_widget_append(ui_root(), parent);
        ui_update();
        ui_widget_each(grid, collect_gallery_column, &columns);

        expected = (1600.0f - HOME_GAP * 3) / 4;
        ctest_equal_int("initial mount should create all columns in one pass",
                        columns.count, HOME_GRID_COLUMNS);
        for (i = 0; i < columns.count; ++i) {
                ctest_equal_float(
                    "initial mount column should use the available width",
                    columns.columns[i]->border_box.width, expected);
        }
        ui_widget_destroy(parent);
}

static void test_remount_after_width_change(void)
{
        float expected;
        int i;

        home_scene_resize(1600.0f, 1000.0f);
        ui_widget_unlink(scene->scroll);
        ui_widget_resize(ui_root(), 1100.0f, 1000.0f);
        ui_widget_append(ui_root(), scene->scroll);
        home_scene_update();

        expected = home_scene_expected_column_width(1100.0f);
        ctest_equal_int("remounted gallery should keep the breakpoint count",
                        home_scene_visible_columns(), 3);
        ctest_equal_float("remounted gallery should use the current width",
                          scene->grid->content_box.width,
                          expected * 3 + HOME_GAP * 2);
        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                if (!scene->columns[i] ||
                    css_computed_display(&scene->columns[i]->computed_style) ==
                        CSS_DISPLAY_NONE) {
                        continue;
                }
                ctest_equal_float(
                    "remounted visible column should use the current width",
                    scene->columns[i]->border_box.width, expected);
        }
}

static void test_native_window_resize(void)
{
        static const float widths[] = { 1600.0f, 1100.0f, 800.0f, 1500.0f };
        static const int visible[] = { 4, 3, 2, 4 };
        float expected;
        float root_width;
        int i;
        int j;

        for (i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i) {
                home_scene_native_resize(widths[i], 1000.0f);
                root_width = ui_root()->content_box.width;
                expected = home_scene_expected_column_width(root_width);
                ctest_equal_bool("native resize should update the root width",
                                 fabsf(root_width - widths[i]) < 2.0f, true);
                ctest_equal_int("native resize should update the column count",
                                home_scene_visible_columns(), visible[i]);
                ctest_equal_float(
                    "native resize should shrink the gallery content",
                    scene->grid->content_box.width,
                    expected * visible[i] + HOME_GAP * (visible[i] - 1));
                for (j = 0; j < HOME_GRID_COLUMNS; ++j) {
                        if (!scene->columns[j] ||
                            css_computed_display(
                                &scene->columns[j]->computed_style) ==
                                CSS_DISPLAY_NONE) {
                                continue;
                        }
                        ctest_equal_float(
                            "native resize should update the used column width",
                            scene->columns[j]->border_box.width, expected);
                }
        }
}

static void home_scene_click(ui_widget_t *target)
{
        ui_event_t e;

        ui_event_init(&e, "click");
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static void test_accordion_card_resize(void)
{
        float initial;
        float expanded;
        float restored;
        float closed;

        home_scene_resize(1600.0f, 1000.0f);
        initial = scene->cards[1]->border_box.height;
        home_scene_click(scene->accordion_triggers[1]);
        home_scene_update();
        expanded = scene->cards[1]->border_box.height;
        ctest_equal_bool("accordion card should grow for longer content",
                         expanded > initial + 1.0f, true);

        home_scene_click(scene->accordion_triggers[0]);
        home_scene_update();
        restored = scene->cards[1]->border_box.height;
        ctest_equal_float("accordion card should shrink when content changes",
                          restored, initial);

        home_scene_click(scene->accordion_triggers[0]);
        home_scene_update();
        closed = scene->cards[1]->border_box.height;
        ctest_equal_bool("accordion card should shrink when content closes",
                         closed < restored - 1.0f, true);
}

static void test_empty_description_hover(void)
{
        ui_widget_t *w;
        float x;
        float width;
        float height;

        home_scene_resize(1600.0f, 1000.0f);
        x = scene->empty_desc->border_box.x;
        width = scene->empty_desc->border_box.width;
        height = scene->empty_desc->border_box.height;
        for (w = scene->empty_desc; w; w = w->parent) {
                ui_widget_add_status(w, "hover");
        }
        home_scene_update();
        ctest_equal_float("description hover should keep its x",
                          scene->empty_desc->border_box.x, x);
        ctest_equal_float("description hover should keep its width",
                          scene->empty_desc->border_box.width, width);
        ctest_equal_float("description hover should keep its height",
                          scene->empty_desc->border_box.height, height);

        for (w = scene->empty_desc; w; w = w->parent) {
                ui_widget_remove_status(w, "hover");
        }
        home_scene_update();
        ctest_equal_float("leaving description hover should restore its x",
                          scene->empty_desc->border_box.x, x);
        ctest_equal_float("leaving description hover should restore its width",
                          scene->empty_desc->border_box.width, width);
        ctest_equal_float("leaving description hover should restore its height",
                          scene->empty_desc->border_box.height, height);
}

static int render_description_ink(ui_widget_t *container,
                                  ui_widget_t *description, double *centroid_x)
{
        pd_canvas_t canvas;
        pd_context_t paint;
        pd_rect_t rect;
        pd_color_t color;
        float scale = ui_get_actual_scale();
        float x = 0;
        float y = 0;
        int left;
        int top;
        int right;
        int bottom;
        int px;
        int py;
        int count = 0;
        double sum_x = 0;

        ui_widget_get_offset(description, container, &x, &y);
        left = (int)(x * scale);
        top = (int)(y * scale);
        right = (int)((x + description->border_box.width) * scale);
        bottom = (int)((y + description->border_box.height) * scale);
        rect.x = 0;
        rect.y = 0;
        rect.width = (int)(container->border_box.width * scale);
        rect.height = (int)(container->border_box.height * scale);
        pd_canvas_init(&canvas);
        canvas.color_type = PD_COLOR_TYPE_ARGB;
        if (pd_canvas_create(&canvas, (unsigned)rect.width,
                             (unsigned)rect.height) != 0) {
                return 0;
        }
        pd_canvas_fill(&canvas, pd_rgb(255, 255, 255));
        paint.with_alpha = true;
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, &canvas, &rect);
        ui_widget_render(container, &paint);
        for (py = top; py < bottom; ++py) {
                for (px = left; px < right; ++px) {
                        color = pd_canvas_get_pixel(&canvas, (unsigned)px,
                                                    (unsigned)py);
                        if (color.r < 220 || color.g < 220 || color.b < 220) {
                                sum_x += px;
                                count++;
                        }
                }
        }
        pd_canvas_destroy(&paint.canvas);
        pd_canvas_destroy(&canvas);
        *centroid_x = count > 0 ? sum_x / count : -1.0;
        return count;
}

static void test_production_description_hover(void)
{
        ui_widget_t *description = scene->production_desc;
        ui_widget_t *w;
        ui_sizehint_t hint = { 0 };
        float x;
        float width;
        double ink_x;
        double hint_ink_x;
        double hover_ink_x;
        double restored_ink_x;
        int ink_pixels;
        int hint_ink_pixels;
        int hover_ink_pixels;
        int restored_ink_pixels;

        ui_load_css_string_global_chunks();
        home_scene_resize(1600.0f, 1000.0f);
        ui_update();
        ui_update();

        x = description->border_box.x;
        width = description->border_box.width;
        ink_pixels =
            render_description_ink(scene->production_card, description, &ink_x);
        ui_widget_get_sizehint(description, &hint);
        hint_ink_pixels = render_description_ink(scene->production_card,
                                                 description, &hint_ink_x);
        ctest_equal_bool("sizehint should still render description text",
                         hint_ink_pixels > 0, true);
        ctest_equal_float(
            "sizehint should not move already rendered description text",
            (float)(hint_ink_x - ink_x), 0.0f);

        for (w = description; w; w = w->parent) {
                ui_widget_add_status(w, "hover");
        }
        ui_update();
        ctest_equal_float("production description hover should keep its x",
                          description->border_box.x, x);
        ctest_equal_float("production description hover should keep its width",
                          description->border_box.width, width);
        hover_ink_pixels = render_description_ink(scene->production_card,
                                                  description, &hover_ink_x);
        ctest_equal_bool("production description hover should render text",
                         hover_ink_pixels > 0, true);
        ctest_equal_bool("baseline description should render text",
                         ink_pixels > 0, true);
        ctest_equal_float(
            "production description hover should not move rendered text",
            (float)(hover_ink_x - ink_x), 0.0f);

        for (w = description; w; w = w->parent) {
                ui_widget_remove_status(w, "hover");
        }
        home_scene_update();
        ctest_equal_float("production description hover should restore its x",
                          description->border_box.x, x);
        ctest_equal_float(
            "production description hover should restore its width",
            description->border_box.width, width);
        restored_ink_pixels = render_description_ink(
            scene->production_card, description, &restored_ink_x);
        ctest_equal_bool("restored description should render text",
                         restored_ink_pixels > 0, true);
        ctest_equal_float(
            "leaving description hover should restore rendered text",
            (float)(restored_ink_x - ink_x), 0.0f);
}

static void test_production_empty_icon(void)
{
        ui_widget_t *box = scene->production_icon_box;
        ui_widget_t *icon = scene->production_icon;
        float x = 0;
        float y = 0;
        float icon_center;
        float box_center;

        ui_load_css_string_global_chunks();
        home_scene_resize(1600.0f, 1000.0f);
        ui_update();
        ui_update();
        ui_widget_get_offset(icon, box, &x, &y);
        icon_center = y + icon->border_box.height * 0.5f;
        box_center = box->border_box.height * 0.5f;

        ctest_equal_float("empty icon should keep a 16px border box",
                          icon->border_box.height, 16.0f);
        ctest_equal_float("empty icon should be vertically centered",
                          icon_center, box_center);
}

static void test_home_item_actions_center(void)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *body = ui_create_card_content();
        ui_widget_t *item = ui_create_item();
        ui_widget_t *media = ui_create_item_media();
        ui_widget_t *media_icon = ui_create_fluent_icon("settings");
        ui_widget_t *content = ui_create_item_content();
        ui_widget_t *title = ui_create_item_title();
        ui_widget_t *description = ui_create_item_description();
        ui_widget_t *actions = ui_create_item_actions();
        ui_widget_t *icon = ui_create_fluent_icon("chevron-right");
        float x = 0;
        float y = 0;
        float item_center;
        float icon_center;

        ui_widget_add_class(card, "home-card");
        ui_widget_add_class(body, "home-card-body home-shortcuts");
        ui_widget_set_attr(media, "variant", "icon");
        ui_text_set_content(title, "Change transfer limit");
        ui_text_set_content(description,
                            "Adjust how much you can send from your balance.");
        ui_widget_append(media, media_icon);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(actions, icon);
        ui_widget_append(item, media);
        ui_widget_append(item, content);
        ui_widget_append(item, actions);
        ui_widget_append(body, item);
        ui_widget_append(card, body);
        ui_widget_append(ui_root(), card);
        ui_load_css_string_global_chunks();
        ui_update();
        ui_update();

        item_center = item->border_box.height * 0.5f;
        ctest_equal_float("home item media icon should be 16px wide",
                          media_icon->border_box.width, 16.0f);
        ctest_equal_float("home item media icon should be 16px high",
                          media_icon->border_box.height, 16.0f);
        ctest_equal_float("home item media should align with the title line",
                          media->computed_style.padding_top, 1.5f);
        ui_widget_get_offset(icon, item, &x, &y);
        icon_center = y + icon->border_box.height * 0.5f;

        ctest_equal_int("home item should center its children",
                        item->computed_style.type_bits.align_items,
                        CSS_ALIGN_ITEMS_CENTER);
        ctest_equal_int("home item actions should keep auto align-self",
                        actions->computed_style.type_bits.align_self,
                        CSS_ALIGN_SELF_AUTO);
        ctest_equal_float("home item action icon should be vertically centered",
                          icon_center, item_center);

        ui_widget_destroy(card);
        ui_clear_trash();
}

static void test_gallery_empty_hover(void)
{
        static const float widths[] = { 1600, 1500, 1400, 1300, 1280,
                                        1200, 1100, 1000, 900,  800 };
        ui_widget_t *w;
        float height;
        float description_x;
        float description_width;
        int i, j;

        if (!scene->empty_desc || !scene->empty_button) {
                return;
        }
        for (j = 0; j < (int)(sizeof(widths) / sizeof(widths[0])); ++j) {
                home_scene_resize(widths[j], 1000.0f);
                height = scene->empty_desc->border_box.height;
                description_x = scene->empty_desc->border_box.x;
                description_width = scene->empty_desc->border_box.width;
                ctest_equal_bool("empty description should wrap",
                                 height > 30.0f, true);
                ctest_equal_float("clip-safe body should keep top padding",
                                  scene->bodies[3]->computed_style.padding_top,
                                  20.0f);
                ctest_equal_float(
                    "clip-safe body should keep right padding",
                    scene->bodies[3]->computed_style.padding_right, 20.0f);
                ctest_equal_float(
                    "clip-safe body should keep bottom padding",
                    scene->bodies[3]->computed_style.padding_bottom, 20.0f);
                ctest_equal_float("clip-safe body should keep left padding",
                                  scene->bodies[3]->computed_style.padding_left,
                                  20.0f);
                ctest_equal_bool("empty card should contain its padded body",
                                 scene->cards[3]->border_box.height + 0.5f >=
                                     scene->bodies[3]->border_box.height,
                                 true);
                ctest_equal_bool("empty body should contain the empty widget",
                                 scene->bodies[3]->content_box.height + 0.5f >=
                                     scene->empty_widget->border_box.height,
                                 true);
                ctest_equal_bool(
                    "empty widget should contain the description",
                    scene->empty_widget->content_box.height + 0.5f >= height,
                    true);
                ctest_equal_bool(
                    "empty content should contain its fixed-height button",
                    scene->empty_content->content_box.height + 0.5f >=
                        scene->empty_button->border_box.height,
                    true);
                ctest_equal_bool(
                    "empty widget should fit its stacked children",
                    scene->empty_widget->border_box.height + 0.5f >=
                        height + scene->empty_button->border_box.height +
                            16.0f + 48.0f,
                    true);
                scene->min_empty_height = FLT_MAX;
                scene->max_empty_height = 0;
                scene->empty_height_passes = 0;

                for (w = scene->empty_button; w; w = w->parent) {
                        ui_widget_add_status(w, "hover");
                }
                home_scene_update();
                ctest_equal_bool("hover should reflow the empty card",
                                 scene->empty_height_passes > 0, true);
                ctest_equal_float("hover should not change the empty height",
                                  scene->max_empty_height,
                                  scene->min_empty_height);
                ctest_equal_float(
                    "hover should keep the empty description height",
                    scene->empty_desc->border_box.height, height);
                ctest_equal_float("hover should keep the empty description x",
                                  scene->empty_desc->border_box.x,
                                  description_x);
                ctest_equal_float(
                    "hover should keep the empty description width",
                    scene->empty_desc->border_box.width, description_width);

                for (w = scene->empty_button; w; w = w->parent) {
                        ui_widget_remove_status(w, "hover");
                }
                for (i = 0; i < 3; ++i) {
                        ui_update();
                }
                ctest_equal_float(
                    "leaving hover should keep the empty description height",
                    scene->empty_desc->border_box.height, height);
                ctest_equal_float(
                    "leaving hover should restore the empty description x",
                    scene->empty_desc->border_box.x, description_x);
                ctest_equal_float(
                    "leaving hover should restore the empty description width",
                    scene->empty_desc->border_box.width, description_width);
        }
}

static void test_hover_keeps_column_width(void)
{
        float column_width;
        ui_widget_t *body = scene->bodies[HOME_GRID_COLUMNS - 1];

        home_scene_resize(1600.0f, 1000.0f);
        if (!scene->columns[0]) {
                return;
        }
        column_width = scene->columns[0]->border_box.width;
        scene->min_card_width = FLT_MAX;
        scene->max_card_width = 0;
        scene->min_card_height = 0;
        scene->max_card_height = 0;
        scene->layout_passes = 0;

        ui_widget_add_status(body, "hover");
        home_scene_update();

        ctest_equal_bool("hover should reflow the card body",
                         scene->layout_passes > 0, true);
        ctest_equal_bool(
            "hover reflow should not stretch the card to the page width",
            scene->max_card_width < column_width + 0.5f, true);
        ctest_equal_bool("hover reflow should not collapse the card",
                         scene->min_card_width > column_width - 0.5f, true);
        ctest_equal_float("hover should not change the card height",
                          scene->max_card_height - scene->min_card_height,
                          0.0f);
        ui_widget_remove_status(body, "hover");
        home_scene_update();
}

void test_gallery_layout(void)
{
        lcui_init();
        ptk_window_hide(ui_server_get_window(ui_root()));
        ui_register_home_grid();
        ui_load_css_file("test_gallery_layout.css");
        ui_widget_set_title(ui_root(), L"test gallery layout");
        home_scene_build();

        ctest_describe("gallery columns", test_columns_follow_window);
        ctest_describe("gallery initial mount", test_initial_mount_single_pass);
        ctest_describe("gallery remount", test_remount_after_width_change);
        ctest_describe("gallery native window resize",
                       test_native_window_resize);
        ctest_describe("gallery accordion resize", test_accordion_card_resize);
        ctest_describe("gallery description hover",
                       test_empty_description_hover);
        ctest_describe("gallery hover", test_hover_keeps_column_width);
        ctest_describe("gallery empty hover", test_gallery_empty_hover);
        ctest_describe("production description hover",
                       test_production_description_hover);
        ctest_describe("production empty icon", test_production_empty_icon);
        ctest_describe("production item actions",
                       test_home_item_actions_center);

        home_scene_destroy();
        lcui_destroy();
}
