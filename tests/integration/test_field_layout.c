/*
 * tests/integration/test_field_layout.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Field layout scenarios built from plain widgets and the local
 * `test-*` CSS fixture. These tests intentionally avoid style-nova and
 * concrete widget types (field/radio/checkbox) so theme or widget style
 * changes cannot invalidate the layout contract.
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

static float right_in(ui_widget_t *w, ui_widget_t *ancestor)
{
        float x = 0;

        ui_widget_get_offset(w, ancestor, &x, NULL);
        return x + w->border_box.width;
}

static float content_width_for_card(ui_widget_t *card, ui_widget_t *radio)
{
        return card->content_box.width - radio->border_box.width - 8.f;
}

static void test_choice_card(void)
{
        ui_widget_t *preview = create_box("test-preview");
        ui_widget_t *demo = create_box("test-field-demo");
        ui_widget_t *group = create_box("test-radio-group");
        ui_widget_t *card = create_box("test-card");
        ui_widget_t *row = create_box("test-field-row");
        ui_widget_t *content = create_box("test-field-content");
        ui_widget_t *title = create_text("test-field-title", "Kubernetes");
        ui_widget_t *desc = create_text("test-field-description",
                                        "Runs on a Kubernetes cluster.");
        ui_widget_t *radio = create_box("test-radio");
        ui_widget_t *ref_desc = create_reference(
            "test-field-description", "Runs on a Kubernetes cluster.");
        float expected_content;
        float gap;

        ui_widget_append(content, title);
        ui_widget_append(content, desc);
        ui_widget_append(row, content);
        ui_widget_append(row, radio);
        ui_widget_append(card, row);
        ui_widget_append(group, card);
        ui_widget_append(demo, group);
        ui_widget_append(preview, demo);
        ui_widget_append(ui_root(), preview);
        ui_update();

        expected_content = content_width_for_card(card, radio);
        ctest_equal_bool("choice card should be 400px wide",
                         fabsf(card->border_box.width - 400.f) <= 1.f, true);
        ctest_equal_bool("choice card content should fill most of the card",
                         content->border_box.width >= expected_content - 1.f,
                         true);
        ctest_equal_bool(
            "choice card content should not exceed the available width",
            content->border_box.width <= expected_content + 1.f, true);
        ctest_equal_bool(
            "choice card description should stay on one line",
            desc->content_box.height <= ref_desc->content_box.height + 1.f,
            true);

        gap = right_in(card, preview) - right_in(radio, preview);
        ctest_equal_bool("choice card radio should be right aligned",
                         gap <= 16.f && gap >= 8.f, true);

        ui_update();
        ctest_equal_bool(
            "choice card radio alignment should stay stable",
            fabsf((right_in(card, preview) - right_in(radio, preview)) - gap) <=
                1.f,
            true);

        ui_widget_destroy(preview);
        ui_widget_destroy(ref_desc->parent);
}

static void test_checkbox_row(void)
{
        ui_widget_t *preview = create_box("test-preview");
        ui_widget_t *demo = create_box("test-field-demo");
        ui_widget_t *row = create_box("test-field-row");
        ui_widget_t *checkbox = create_box("test-checkbox");
        ui_widget_t *content = create_box("test-field-content");
        ui_widget_t *title =
            create_text("test-field-title", "Sync Desktop & Documents");
        ui_widget_t *desc = create_text("test-field-description", LONG_TEXT);
        ui_widget_t *ref_desc =
            create_reference("test-field-description", LONG_TEXT);
        float height;

        ui_widget_append(content, title);
        ui_widget_append(content, desc);
        ui_widget_append(row, checkbox);
        ui_widget_append(row, content);
        ui_widget_append(demo, row);
        ui_widget_append(preview, demo);
        ui_widget_append(ui_root(), preview);
        ui_update();

        ctest_equal_bool(
            "checkbox description should wrap",
            desc->content_box.height > ref_desc->content_box.height * 1.5f,
            true);
        ctest_equal_bool(
            "checkbox description should stay inside the row content width",
            desc->border_box.width <= row->content_box.width + 1.f, true);

        height = desc->content_box.height;
        ui_widget_add_status(desc, "hover");
        ui_update();
        ctest_equal_bool("checkbox description should stay wrapped on hover",
                         fabsf(desc->content_box.height - height) <= 2.f, true);
        ui_widget_remove_status(desc, "hover");

        ui_widget_unlink(desc);
        ui_widget_append(content, desc);
        ui_update();
        ctest_equal_bool("checkbox description should stay wrapped on remount",
                         fabsf(desc->content_box.height - height) <= 2.f, true);

        ui_widget_request_reflow(content);
        ui_update();
        ctest_equal_bool(
            "checkbox description should stay wrapped after content reflow",
            fabsf(desc->content_box.height - height) <= 2.f, true);

        ui_widget_destroy(preview);
        ui_widget_destroy(ref_desc->parent);
}

void test_field_layout(void)
{
        lcui_init();
        ui_load_css_file("test_field_layout.css");
        ui_widget_set_title(ui_root(), L"test field layout");

        ctest_describe("field choice card", test_choice_card);
        ctest_describe("field checkbox row", test_checkbox_row);

        lcui_destroy();
}
