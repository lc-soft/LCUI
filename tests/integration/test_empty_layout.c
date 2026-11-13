/*
 * tests/integration/test_component_layout.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Empty layout scenarios built from plain widgets and the local
 * `test-*` CSS fixture. These tests intentionally avoid style-nova and
 * concrete widget types (empty/button) so theme or widget style changes
 * cannot invalidate the layout contract.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <math.h>

#define LONG_TEXT                                                        \
        "The quick brown fox jumps over the lazy dog and keeps running " \
        "across the field until the line must wrap."

static ui_widget_t *create_box(const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(w, class_name);
        return w;
}

static ui_widget_t *create_text(const char *class_name, const char *content)
{
        ui_widget_t *w = ui_create_widget("text");

        ui_widget_add_class(w, class_name);
        ui_text_set_content(w, content);
        return w;
}

static ui_widget_t *create_reference(const char *class_name,
                                     const char *content)
{
        ui_widget_t *holder = create_box("test-ref");
        ui_widget_t *text = create_text(class_name, content);

        ui_widget_append(holder, text);
        ui_widget_append(ui_root(), holder);
        return text;
}

static float center_x_in(ui_widget_t *w, ui_widget_t *ancestor)
{
        float x = 0;

        ui_widget_get_offset(w, ancestor, &x, NULL);
        return x + w->border_box.width * 0.5f;
}

static void test_empty_basic(void)
{
        ui_widget_t *preview = create_box("test-preview");
        ui_widget_t *empty = create_box("test-empty");
        ui_widget_t *header = create_box("test-empty-header");
        ui_widget_t *media = create_box("test-empty-media");
        ui_widget_t *title = create_text("test-text", "No Projects Yet");
        ui_widget_t *desc = create_text("test-text", LONG_TEXT);
        ui_widget_t *content = create_box("test-empty-content");
        ui_widget_t *actions = create_box("test-empty-actions");
        ui_widget_t *create_button = create_box("test-action-primary");
        ui_widget_t *import_button = create_box("test-action-secondary");
        ui_widget_t *learn = create_box("test-learn");
        ui_widget_t *learn_text = create_text("test-learn-text", "Learn More");
        ui_widget_t *learn_icon = create_box("test-learn-icon");
        ui_widget_t *ref_desc = create_reference("test-text", LONG_TEXT);
        ui_widget_t *ref_learn =
            create_reference("test-learn-text", "Learn More");
        float learn_width;
        float desc_height;

        ui_widget_append(learn, learn_text);
        ui_widget_append(learn, learn_icon);
        ui_widget_append(actions, create_button);
        ui_widget_append(actions, import_button);
        ui_widget_append(header, media);
        ui_widget_append(header, title);
        ui_widget_append(header, desc);
        ui_widget_append(content, actions);
        ui_widget_append(empty, header);
        ui_widget_append(empty, content);
        ui_widget_append(empty, learn);
        ui_widget_append(preview, empty);
        ui_widget_append(ui_root(), preview);
        ui_update();

        ctest_equal_bool("empty learn text should stay on one line",
                         learn_text->content_box.height <=
                             ref_learn->content_box.height + 1.f,
                         true);
        ctest_equal_bool(
            "empty learn button should fit text and icon",
            learn->content_box.width >= learn_text->content_box.width +
                                            learn_icon->border_box.width + 5.f,
            true);
        ctest_equal_bool("empty learn button should be centered",
                         fabsf(center_x_in(learn, empty) -
                               empty->border_box.width * 0.5f) <= 2.f,
                         true);
        ctest_equal_bool(
            "empty learn button should not stretch to the container width",
            learn->border_box.width < empty->content_box.width - 1.f, true);
        ctest_equal_bool(
            "empty description should wrap",
            desc->content_box.height > ref_desc->content_box.height * 1.5f,
            true);
        ctest_equal_bool(
            "empty description should stay inside the container",
            desc->border_box.width <= empty->content_box.width + 1.f, true);

        learn_width = learn->border_box.width;
        desc_height = desc->content_box.height;
        ui_update();
        ui_update();
        ctest_equal_bool("empty learn width should stay stable",
                         fabsf(learn->border_box.width - learn_width) <= 1.f,
                         true);
        ctest_equal_bool("empty description height should stay stable",
                         fabsf(desc->content_box.height - desc_height) <= 1.f,
                         true);

        ui_widget_destroy(preview);
        ui_widget_destroy(ref_desc->parent);
        ui_widget_destroy(ref_learn->parent);
}

/* The gallery uses the real empty widget inside a column. Its description
 * height must come from the wrapped line boxes, and a hover-triggered reflow
 * must not change the card height. */
static void test_empty_hover_stability(void)
{
        static const float widths[] = { 340, 320, 300, 290, 280, 270, 260,
                                        250, 240, 230, 220, 210, 200 };
        ui_widget_t *box = create_box("test-empty-hover-box");
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header = ui_create_empty_header();
        ui_widget_t *description = ui_create_empty_description();
        ui_widget_t *content = ui_create_empty_content();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *w;
        float height;
        float card_height;
        int i, j;

        ui_text_set_content(
            description,
            "Upload your first master to start reaching listeners on "
            "Spotify, Apple Music, and more.");
        ui_button_set_text(button, "Create Release");
        ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);
        ui_widget_append(header, description);
        ui_widget_append(content, button);
        ui_widget_append(empty, header);
        ui_widget_append(empty, content);
        ui_widget_append(box, empty);
        ui_widget_append(ui_root(), box);
        ui_widget_resize(ui_root(), 1280, 720);
        ui_update();
        ui_update();

        for (j = 0; j < (int)(sizeof(widths) / sizeof(widths[0])); ++j) {
                ui_widget_set_style_unit_value(box, css_prop_width, widths[j],
                                               CSS_UNIT_PX);
                for (i = 0; i < 3; ++i) {
                        ui_update();
                }
                height = description->border_box.height;
                card_height = box->border_box.height;
                ctest_equal_bool("empty description should have a height",
                                 height > 0.0f, true);

                for (w = button; w; w = w->parent) {
                        ui_widget_add_status(w, "hover");
                }
                for (i = 0; i < 3; ++i) {
                        ui_update();
                }
                ctest_equal_float(
                    "hover should keep the empty description height",
                    description->border_box.height, height);
                ctest_equal_float("hover should keep the empty card height",
                                  box->border_box.height, card_height);

                for (w = button; w; w = w->parent) {
                        ui_widget_remove_status(w, "hover");
                }
                for (i = 0; i < 3; ++i) {
                        ui_update();
                }
                ctest_equal_float(
                    "leaving hover should keep the empty description height",
                    description->border_box.height, height);
        }

        ui_widget_destroy(box);
}

void test_empty_layout(void)
{
        lcui_init();
        ui_load_css_file("test_empty_layout.css");
        ui_widget_set_title(ui_root(), L"test empty layout");

        ctest_describe("empty basic", test_empty_basic);
        ctest_describe("empty hover stability", test_empty_hover_stability);

        lcui_destroy();
}
