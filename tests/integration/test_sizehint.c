/*
 * tests/integration/test_sizehint.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Sizehint contract tests.
 *
 * These tests describe the intrinsic-size rules the flex layout relies on.
 * The black-box group only asserts final geometry; the capacity group is a
 * small white-box diagnostic for `ui_widget_has_sizehint()`.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <math.h>
#include <stdio.h>

/* Internal sizehint queries used by the flex measurement paths. */
bool ui_widget_has_sizehint(ui_widget_t *w);
bool ui_widget_sizehint_is_exact(ui_widget_t *w);

#define WRAP_TEXT                                                      \
        "This sentence is long enough to wrap inside a narrow column " \
        "without overflowing its content box."

static void set_style(ui_widget_t *w, const char *name, const char *value)
{
        ui_widget_set_style_string(w, name, value);
}

static void set_px(ui_widget_t *w, const char *name, float value)
{
        char buf[32];

        snprintf(buf, sizeof(buf), "%gpx", value);
        set_style(w, name, buf);
}

static ui_widget_t *create_box(float width, float height)
{
        ui_widget_t *w = ui_create_widget(NULL);

        if (width > 0) {
                set_px(w, "width", width);
        }
        if (height > 0) {
                set_px(w, "height", height);
        }
        return w;
}

static ui_widget_t *create_column(float width)
{
        ui_widget_t *w = ui_create_widget(NULL);

        set_style(w, "display", "flex");
        set_style(w, "flex-direction", "column");
        if (width > 0) {
                set_px(w, "width", width);
        }
        return w;
}

static ui_widget_t *create_row(float width)
{
        ui_widget_t *w = ui_create_widget(NULL);

        set_style(w, "display", "flex");
        set_style(w, "flex-direction", "row");
        if (width > 0) {
                set_px(w, "width", width);
        }
        return w;
}

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *w = ui_create_widget("text");

        ui_text_set_content(w, content);
        return w;
}

/* Single-line reference text: appended to the wide root so it is measured
 * without a width constraint. */
static ui_widget_t *create_text_reference(const char *content)
{
        ui_widget_t *ref = create_text(content);

        ui_widget_append(ui_root(), ref);
        return ref;
}

static bool close_to(float actual, float expected, float tolerance)
{
        return fabsf(actual - expected) <= tolerance;
}

static void test_capability(void)
{
        ui_widget_t *sandbox = ui_create_widget(NULL);
        ui_widget_t *text = create_text("Text");
        ui_widget_t *button = ui_create_button();
        ui_widget_t *own_text_button = ui_create_button();
        ui_widget_t *label = ui_create_label();
        ui_widget_t *canvas = ui_create_widget("canvas");
        ui_widget_t *textinput = ui_create_widget("textinput");
        ui_widget_t *with_flow_child = ui_create_label();
        ui_widget_t *with_hidden_child = ui_create_label();
        ui_widget_t *with_absolute_child = ui_create_label();
        ui_widget_t *plain = ui_create_widget(NULL);
        ui_widget_t *hidden = create_text("Hidden");
        ui_widget_t *absolute = create_text("Absolute");

        ui_widget_append(button, create_text("Button"));
        ui_widget_append(label, create_text("Label"));
        ui_button_set_text(own_text_button, "Own text");

        ui_widget_hide(hidden);
        set_style(absolute, "position", "absolute");
        ui_widget_append(with_flow_child, create_text("Flow"));
        ui_widget_append(with_hidden_child, hidden);
        ui_widget_append(with_absolute_child, absolute);

        ui_widget_append(sandbox, text);
        ui_widget_append(sandbox, button);
        ui_widget_append(sandbox, own_text_button);
        ui_widget_append(sandbox, label);
        ui_widget_append(sandbox, with_flow_child);
        ui_widget_append(sandbox, with_hidden_child);
        ui_widget_append(sandbox, with_absolute_child);
        ui_widget_append(sandbox, plain);
        ui_widget_append(ui_root(), sandbox);
        ui_update();

        ctest_equal_bool("H01 text leaf should report a sizehint",
                         ui_widget_has_sizehint(text), true);
        ctest_equal_bool("H01 text leaf sizehint should be exact",
                         ui_widget_sizehint_is_exact(text), true);
        ctest_equal_bool("H02 button should keep the inherited sizehint",
                         ui_widget_has_sizehint(button), true);
        ctest_equal_bool("H02 button sizehint should not cover child text",
                         ui_widget_sizehint_is_exact(button), false);
        ctest_equal_bool("H03 button with own text should report a sizehint",
                         ui_widget_has_sizehint(own_text_button), true);
        ctest_equal_bool("H03 button with own text sizehint should be exact",
                         ui_widget_sizehint_is_exact(own_text_button), true);
        ctest_equal_bool("H02 label should keep the inherited sizehint",
                         ui_widget_has_sizehint(label), true);
        ctest_equal_bool("H02 label sizehint should not cover child text",
                         ui_widget_sizehint_is_exact(label), false);
        ctest_equal_bool("H04 canvas should report a sizehint",
                         ui_widget_has_sizehint(canvas), true);
        ctest_equal_bool("H04 canvas sizehint should be exact",
                         ui_widget_sizehint_is_exact(canvas), true);
        ctest_equal_bool("H04 textinput should report a sizehint",
                         ui_widget_has_sizehint(textinput), true);
        ctest_equal_bool("H04 textinput sizehint should be exact",
                         ui_widget_sizehint_is_exact(textinput), true);
        ctest_equal_bool("H02 container should keep the inherited sizehint",
                         ui_widget_has_sizehint(with_flow_child), true);
        ctest_equal_bool("H02 container sizehint should not cover children",
                         ui_widget_sizehint_is_exact(with_flow_child), false);
        ctest_equal_bool("H12 hidden child should keep sizehint exact",
                         ui_widget_sizehint_is_exact(with_hidden_child), true);
        ctest_equal_bool("H12 absolute child should keep sizehint exact",
                         ui_widget_sizehint_is_exact(with_absolute_child),
                         true);
        ctest_equal_bool("H01 plain widget should not report a sizehint",
                         ui_widget_has_sizehint(plain), false);
        ctest_equal_bool("H01 plain widget sizehint should not be exact",
                         ui_widget_sizehint_is_exact(plain), false);

        ui_widget_destroy(sandbox);
        ui_widget_destroy(canvas);
        ui_widget_destroy(textinput);
}

static void test_own_hint_merges_with_child_extent(void)
{
        ui_widget_t *box = create_column(300);
        ui_widget_t *label = ui_create_label();
        ui_widget_t *child = create_text("Child content");
        ui_widget_t *ref = create_text_reference("Child content");

        set_style(box, "align-items", "center");
        ui_widget_append(label, child);
        ui_widget_append(box, label);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool(
            "H01 inherited hint should not hide child extent",
            label->content_box.width >= child->content_box.width - 1.f, true);
        ctest_equal_bool("H01 label should take fit-content width",
                         label->border_box.width < 300.f - 1.f, true);
        ctest_equal_bool(
            "H01 child text should stay on one line",
            child->content_box.height <= ref->content_box.height + 1.f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static void test_text_single_line(void)
{
        ui_widget_t *box = create_column(300);
        ui_widget_t *text = create_text("Short label");
        ui_widget_t *ref = create_text_reference("Short label");

        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool(
            "H05 text should fit on one line",
            text->content_box.height <= ref->content_box.height + 1.f, true);
        ctest_equal_bool("H05 text should have a positive width",
                         text->content_box.width > 0.f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static void test_text_wraps_at_available_width(void)
{
        ui_widget_t *box = create_column(100);
        ui_widget_t *text = create_text(WRAP_TEXT);
        ui_widget_t *ref = create_text_reference(WRAP_TEXT);

        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool(
            "H06 text should wrap at a narrow width",
            text->content_box.height > ref->content_box.height * 1.5f, true);
        ctest_equal_bool("H06 text should stay within the column width",
                         text->border_box.width <= 100.f + 1.f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static void test_max_width_limits_sizehint(void)
{
        ui_widget_t *box = create_column(600);
        ui_widget_t *text = create_text(WRAP_TEXT);
        ui_widget_t *ref = create_text_reference(WRAP_TEXT);

        set_px(text, "max-width", 120);
        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool("H07 max-width should limit measured width",
                         text->border_box.width <= 120.f + 1.f, true);
        ctest_equal_bool(
            "H07 max-width should cause wrapping",
            text->content_box.height > ref->content_box.height * 1.5f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static void test_fixed_width_wins_over_constraint(void)
{
        ui_widget_t *box = create_column(300);
        ui_widget_t *text = create_text(WRAP_TEXT);
        ui_widget_t *ref = create_text_reference(WRAP_TEXT);

        set_px(text, "width", 120);
        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool("H08 fixed width should be used as the content width",
                         close_to(text->content_box.width, 120.f, 1.f), true);
        ctest_equal_bool(
            "H08 fixed width should wrap the text",
            text->content_box.height > ref->content_box.height * 1.5f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static void test_empty_text_zero_size(void)
{
        ui_widget_t *box = create_column(200);
        ui_widget_t *text = create_text("");

        set_style(box, "align-items", "flex-start");
        set_style(text, "font-size", "0px");
        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool("H09 empty text should have zero width",
                         text->content_box.width <= 1.f, true);
        ctest_equal_bool("H09 empty text should have zero height",
                         text->content_box.height <= 1.f, true);

        ui_widget_destroy(box);
}

static void test_nested_container_height(void)
{
        ui_widget_t *outer = create_column(240);
        ui_widget_t *inner = create_column(0);
        ui_widget_t *first = create_text("Alpha");
        ui_widget_t *second = create_text("Beta");
        ui_widget_t *ref_first = create_text_reference("Alpha");
        float inner_height;

        set_style(outer, "align-items", "flex-start");
        ui_widget_append(inner, first);
        ui_widget_append(inner, second);
        ui_widget_append(outer, inner);
        ui_widget_append(ui_root(), outer);
        ui_update();

        inner_height = inner->content_box.height;
        ctest_equal_bool("H15 nested container should use its content width",
                         inner->border_box.width < 240.f, true);
        ctest_equal_bool(
            "H15 nested container height should cover both children",
            inner_height >=
                first->content_box.height + second->content_box.height - 1.f,
            true);
        ctest_equal_bool("H15 outer container should include nested height",
                         outer->content_box.height >= inner_height - 1.f, true);
        ctest_equal_bool(
            "H15 nested text should stay on one line",
            first->content_box.height <= ref_first->content_box.height + 1.f,
            true);

        ui_widget_destroy(outer);
        ui_widget_destroy(ref_first);
}

static void test_container_without_sizehint_falls_back(void)
{
        ui_widget_t *outer = create_column(240);
        ui_widget_t *inner = create_row(0);
        ui_widget_t *left = create_box(30, 20);
        ui_widget_t *right = create_box(30, 20);

        set_style(outer, "align-items", "flex-start");
        ui_widget_append(inner, left);
        ui_widget_append(inner, right);
        ui_widget_append(outer, inner);
        ui_widget_append(ui_root(), outer);
        ui_update();

        ctest_equal_bool("H11 fallback should measure nested row width",
                         close_to(inner->border_box.width, 60.f, 1.f), true);
        ctest_equal_bool("H11 fallback should measure nested row height",
                         close_to(inner->border_box.height, 20.f, 1.f), true);
        ctest_equal_bool(
            "H11 outer height should include nested row",
            outer->content_box.height >= inner->content_box.height - 1.f, true);

        ui_widget_destroy(outer);
}

static void test_out_of_flow_children_ignored(void)
{
        ui_widget_t *box = create_column(200);
        ui_widget_t *text = create_text("Visible");
        ui_widget_t *absolute = create_box(500, 500);
        ui_widget_t *hidden = create_box(100, 100);
        float text_height;

        set_style(absolute, "position", "absolute");
        ui_widget_hide(hidden);
        ui_widget_append(box, text);
        ui_widget_append(box, absolute);
        ui_widget_append(box, hidden);
        ui_widget_append(ui_root(), box);
        ui_update();

        text_height = text->content_box.height;
        ctest_equal_bool("H12 absolute child should not grow the container",
                         box->content_box.height <= text_height + 2.f, true);
        ctest_equal_bool("H12 hidden child should not grow the container",
                         box->content_box.height <= text_height + 2.f, true);

        ui_widget_destroy(box);
}

static void test_content_change_invalidates_measurement(void)
{
        ui_widget_t *box = create_column(100);
        ui_widget_t *text = create_text(WRAP_TEXT);
        float wrapped_height, short_height, rewrapped_height;

        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();
        wrapped_height = text->content_box.height;

        ui_text_set_content(text, "Short.");
        ui_update();
        short_height = text->content_box.height;
        ctest_equal_bool("H13 shorter content should reduce height",
                         short_height < wrapped_height - 2.f, true);

        ui_text_set_content(text, WRAP_TEXT);
        ui_update();
        rewrapped_height = text->content_box.height;
        ctest_equal_bool("H13 longer content should restore wrapped height",
                         rewrapped_height > short_height + 2.f, true);

        ui_widget_destroy(box);
}

static void test_widening_reexpands_text(void)
{
        ui_widget_t *box = create_column(120);
        ui_widget_t *text = create_text(WRAP_TEXT);
        float narrow_height, wide_height;

        set_style(box, "align-items", "center");
        ui_widget_append(box, text);
        ui_widget_append(ui_root(), box);
        ui_update();
        narrow_height = text->content_box.height;

        set_px(box, "width", 600);
        ui_update();
        wide_height = text->content_box.height;

        ctest_equal_bool("H14 widening the container should reduce text height",
                         wide_height < narrow_height - 2.f, true);
        ctest_equal_bool("H14 text should use the newly available width",
                         text->content_box.width > 120.f, true);

        ui_widget_destroy(box);
}

static void test_negative_available_width_clamped(void)
{
        ui_widget_t *box = create_column(20);
        ui_widget_t *child = ui_create_widget(NULL);

        set_px(child, "padding-left", 30);
        set_px(child, "padding-right", 30);
        ui_widget_append(box, child);
        ui_widget_append(ui_root(), box);
        ui_update();

        ctest_equal_bool("H16 child content width should be clamped to zero",
                         child->content_box.width >= 0.f, true);
        ctest_equal_bool("H16 child border box should not be negative",
                         child->border_box.width >= 0.f, true);

        ui_widget_destroy(box);
}

void test_sizehint(void)
{
        lcui_init();
        ui_widget_set_title(ui_root(), L"test sizehint");

        ctest_describe("sizehint capability", test_capability);
        ctest_describe("own hint merges with child extent",
                       test_own_hint_merges_with_child_extent);
        ctest_describe("text single line hint", test_text_single_line);
        ctest_describe("text wraps at available width",
                       test_text_wraps_at_available_width);
        ctest_describe("max-width limits sizehint",
                       test_max_width_limits_sizehint);
        ctest_describe("fixed width wins over constraint",
                       test_fixed_width_wins_over_constraint);
        ctest_describe("empty text zero size", test_empty_text_zero_size);
        ctest_describe("nested container height", test_nested_container_height);
        ctest_describe("container without sizehint falls back",
                       test_container_without_sizehint_falls_back);
        ctest_describe("out-of-flow children ignored",
                       test_out_of_flow_children_ignored);
        ctest_describe("content change invalidates measurement",
                       test_content_change_invalidates_measurement);
        ctest_describe("widening reexpands text", test_widening_reexpands_text);
        ctest_describe("negative available width clamped",
                       test_negative_available_width_clamped);

        lcui_destroy();
}
