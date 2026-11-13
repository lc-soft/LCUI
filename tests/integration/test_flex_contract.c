/*
 * tests/integration/test_flex_contract.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Flex layout contract tests.
 *
 * Each `ctest_describe` name starts with a rule id from
 * docs/testing/flex-layout-coverage-audit.md. Assertions only use final
 * geometry (boxes and relative positions), never internal pass state.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <math.h>
#include <stdio.h>

#define TOL 1.f

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

static ui_widget_t *create_flex(const char *direction, float width,
                                float height)
{
        ui_widget_t *w = ui_create_widget(NULL);

        set_style(w, "display", "flex");
        set_style(w, "flex-direction", direction);
        if (width > 0) {
                set_px(w, "width", width);
        }
        if (height > 0) {
                set_px(w, "height", height);
        }
        return w;
}

static ui_widget_t *create_block(float width, float height)
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

static ui_widget_t *create_text(const char *content)
{
        ui_widget_t *w = ui_create_widget("text");

        ui_text_set_content(w, content);
        return w;
}

static ui_widget_t *create_text_reference(const char *content)
{
        ui_widget_t *ref = create_text(content);

        ui_widget_append(ui_root(), ref);
        return ref;
}

static float x_in(ui_widget_t *w, ui_widget_t *ancestor)
{
        float x = 0;

        ui_widget_get_offset(w, ancestor, &x, NULL);
        return x;
}

static float right_in(ui_widget_t *w, ui_widget_t *ancestor)
{
        return x_in(w, ancestor) + w->border_box.width;
}

static bool close_to(float actual, float expected, float tolerance)
{
        return fabsf(actual - expected) <= tolerance;
}

static void append_root(ui_widget_t *w)
{
        ui_widget_append(ui_root(), w);
}

/* F01: default row placement and cross-axis stretch. */
static void test_default_row(void)
{
        ui_widget_t *row = create_flex("row", 300, 100);
        ui_widget_t *a = create_block(50, 20);
        ui_widget_t *b = create_block(50, 20);
        ui_widget_t *c = create_block(50, 0);

        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F01 first item should start at the main-axis origin",
                         close_to(a->border_box.x, 0, TOL), true);
        ctest_equal_bool("F01 second item should follow the first",
                         close_to(b->border_box.x, 50, TOL), true);
        ctest_equal_bool("F01 items should share the same row",
                         close_to(a->border_box.y, b->border_box.y, TOL), true);
        ctest_equal_bool(
            "F01 auto-height item should stretch to the row height",
            close_to(c->border_box.height, 100, TOL), true);
        ctest_equal_bool("F01 explicit-height item should not stretch",
                         close_to(a->border_box.height, 20, TOL), true);

        ui_widget_destroy(row);
}

/* F02: column main axis. */
static void test_column_placement(void)
{
        ui_widget_t *column = create_flex("column", 200, 300);
        ui_widget_t *a = create_block(40, 50);
        ui_widget_t *b = create_block(40, 50);

        ui_widget_append(column, a);
        ui_widget_append(column, b);
        append_root(column);
        ui_update();

        ctest_equal_bool("F02 first item should start at the top",
                         close_to(a->border_box.y, 0, TOL), true);
        ctest_equal_bool("F02 second item should follow vertically",
                         close_to(b->border_box.y, 50, TOL), true);
        ctest_equal_bool("F02 items should share the same column",
                         close_to(a->border_box.x, b->border_box.x, TOL), true);

        ui_widget_destroy(column);
}

/* F03: justify-content values. */
static void test_justify_center(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *item = create_block(50, 20);

        set_style(row, "justify-content", "center");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F03 center should split free space evenly",
                         close_to(item->border_box.x, 125, TOL), true);

        ui_widget_destroy(row);
}

static void test_justify_flex_end(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *item = create_block(50, 20);

        set_style(row, "justify-content", "flex-end");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F03 flex-end should push the item to the end",
                         close_to(item->border_box.x, 250, TOL), true);

        ui_widget_destroy(row);
}

static void test_justify_space_between(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(30, 20);
        ui_widget_t *b = create_block(30, 20);
        ui_widget_t *c = create_block(30, 20);

        set_style(row, "justify-content", "space-between");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F03 space-between should pin the first item",
                         close_to(a->border_box.x, 0, TOL), true);
        ctest_equal_bool("F03 space-between should pin the last item",
                         close_to(c->border_box.x, 270, TOL), true);
        ctest_equal_bool("F03 space-between should center the middle item",
                         close_to(b->border_box.x, 135, TOL), true);

        ui_widget_destroy(row);
}

static void test_justify_space_around(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(30, 20);
        ui_widget_t *b = create_block(30, 20);
        ui_widget_t *c = create_block(30, 20);

        set_style(row, "justify-content", "space-around");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F03 space-around should leave a half space first",
                         close_to(a->border_box.x, 35, TOL), true);
        ctest_equal_bool(
            "F03 space-around should space items evenly",
            close_to(b->border_box.x - a->border_box.x - a->border_box.width,
                     70, TOL),
            true);

        ui_widget_destroy(row);
}

static void test_justify_space_evenly(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(30, 20);
        ui_widget_t *b = create_block(30, 20);
        ui_widget_t *c = create_block(30, 20);

        set_style(row, "justify-content", "space-evenly");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F03 space-evenly should leave equal spaces",
                         close_to(a->border_box.x, 52.5f, TOL), true);
        ctest_equal_bool(
            "F03 space-evenly should keep equal gaps",
            close_to(b->border_box.x - a->border_box.x - a->border_box.width,
                     52.5f, TOL),
            true);
        ctest_equal_bool("F03 space-evenly should not overflow the row",
                         c->border_box.x + c->border_box.width <= 300.f + TOL,
                         true);

        ui_widget_destroy(row);
}

/* F04: align-items values. */
static void test_align_start(void)
{
        ui_widget_t *row = create_flex("row", 300, 200);
        ui_widget_t *item = create_block(50, 50);

        set_style(row, "align-items", "flex-start");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F04 flex-start should pin the item to the top",
                         close_to(item->border_box.y, 0, TOL), true);

        ui_widget_destroy(row);
}

static void test_align_center(void)
{
        ui_widget_t *row = create_flex("row", 300, 200);
        ui_widget_t *item = create_block(50, 50);

        set_style(row, "align-items", "center");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F04 center should split cross-axis free space",
                         close_to(item->border_box.y, 75, TOL), true);

        ui_widget_destroy(row);
}

static void test_align_flex_end(void)
{
        ui_widget_t *row = create_flex("row", 300, 200);
        ui_widget_t *item = create_block(50, 50);

        set_style(row, "align-items", "flex-end");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F04 flex-end should push the item to the bottom",
                         close_to(item->border_box.y, 150, TOL), true);

        ui_widget_destroy(row);
}

static void test_align_stretch(void)
{
        ui_widget_t *row = create_flex("row", 300, 200);
        ui_widget_t *item = create_block(50, 0);

        set_style(row, "align-items", "stretch");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F04 stretch should fill the cross axis",
                         close_to(item->border_box.height, 200, TOL), true);

        ui_widget_destroy(row);
}

/* F05: main-axis auto margin. */
static void test_auto_margin_main_axis(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(30, 20);
        ui_widget_t *b = create_block(30, 20);

        set_style(b, "margin-left", "auto");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        append_root(row);
        ui_update();

        ctest_equal_bool("F05 auto margin should push the item to the end",
                         close_to(b->border_box.x, 270, TOL), true);

        ui_widget_destroy(row);
}

/* F06: flex-grow distribution. */
static void test_flex_grow(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(50, 20);
        ui_widget_t *b = create_block(50, 20);

        set_style(a, "flex-grow", "1");
        set_style(b, "flex-grow", "2");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        append_root(row);
        ui_update();

        ctest_equal_bool("F06 grow 1 should receive one third of free space",
                         close_to(a->border_box.width, 116.667f, TOL), true);
        ctest_equal_bool("F06 grow 2 should receive two thirds of free space",
                         close_to(b->border_box.width, 183.333f, TOL), true);

        ui_widget_destroy(row);
}

/* F07: flex-shrink distribution. */
static void test_flex_shrink(void)
{
        ui_widget_t *row = create_flex("row", 100, 50);
        ui_widget_t *a = create_block(100, 20);
        ui_widget_t *b = create_block(100, 20);

        set_style(a, "flex-shrink", "1");
        set_style(b, "flex-shrink", "3");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        append_root(row);
        ui_update();

        ctest_equal_bool("F07 shrink 1 should keep three quarters",
                         close_to(a->border_box.width, 75, TOL), true);
        ctest_equal_bool("F07 shrink 3 should keep one quarter",
                         close_to(b->border_box.width, 25, TOL), true);

        ui_widget_destroy(row);
}

/* F08: flex-basis resolution. */
static void test_flex_basis_fixed(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *item = create_block(0, 20);

        set_px(item, "flex-basis", 80);
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F08 fixed basis should define the used width",
                         close_to(item->border_box.width, 80, TOL), true);

        ui_widget_destroy(row);
}

static void test_flex_basis_percentage(void)
{
        ui_widget_t *row = create_flex("row", 400, 50);
        ui_widget_t *item = create_block(0, 20);

        set_style(item, "flex-basis", "25%");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F08 percentage basis should resolve against the row",
                         close_to(item->border_box.width, 100, TOL), true);

        ui_widget_destroy(row);
}

/* F09: flex-wrap. */
static void test_flex_wrap(void)
{
        ui_widget_t *row = create_flex("row", 100, 100);
        ui_widget_t *a = create_block(40, 20);
        ui_widget_t *b = create_block(40, 20);
        ui_widget_t *c = create_block(40, 20);

        set_style(row, "flex-wrap", "wrap");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F09 second item should fit on the first line",
                         close_to(b->border_box.x, 40, TOL), true);
        ctest_equal_bool("F09 third item should start a new line",
                         c->border_box.y > a->border_box.y + TOL, true);
        ctest_equal_bool("F09 wrapped item should start at the line origin",
                         close_to(c->border_box.x, 0, TOL), true);

        ui_widget_destroy(row);
}

static void test_flex_nowrap(void)
{
        ui_widget_t *row = create_flex("row", 100, 100);
        ui_widget_t *a = create_block(40, 20);
        ui_widget_t *b = create_block(40, 20);
        ui_widget_t *c = create_block(40, 20);

        set_style(row, "flex-wrap", "nowrap");
        set_style(a, "flex-shrink", "0");
        set_style(b, "flex-shrink", "0");
        set_style(c, "flex-shrink", "0");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F09 nowrap should keep all items on one line",
                         close_to(c->border_box.y, a->border_box.y, TOL), true);
        ctest_equal_bool("F09 nowrap should not reorder items",
                         close_to(c->border_box.x, 80, TOL), true);

        ui_widget_destroy(row);
}

/* F10: gap. */
static void test_gap_row(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(50, 20);
        ui_widget_t *b = create_block(50, 20);
        ui_widget_t *c = create_block(50, 20);

        set_px(row, "column-gap", 10);
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        ui_widget_append(row, c);
        append_root(row);
        ui_update();

        ctest_equal_bool("F10 column-gap should separate row items",
                         close_to(b->border_box.x, 60, TOL), true);
        ctest_equal_bool("F10 column-gap should accumulate",
                         close_to(c->border_box.x, 120, TOL), true);

        ui_widget_destroy(row);
}

static void test_gap_column(void)
{
        ui_widget_t *column = create_flex("column", 100, 300);
        ui_widget_t *a = create_block(20, 50);
        ui_widget_t *b = create_block(20, 50);

        set_px(column, "row-gap", 20);
        ui_widget_append(column, a);
        ui_widget_append(column, b);
        append_root(column);
        ui_update();

        ctest_equal_bool("F10 row-gap should separate column items",
                         close_to(b->border_box.y, 70, TOL), true);

        ui_widget_destroy(column);
}

/* F11: percentage sizes. */
static void test_percentage_width_row(void)
{
        ui_widget_t *row = create_flex("row", 200, 50);
        ui_widget_t *item = create_block(0, 20);

        set_style(item, "width", "50%");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool(
            "F11 percentage width should use the row content width",
            close_to(item->border_box.width, 100, TOL), true);

        ui_widget_destroy(row);
}

static void test_percentage_height_column(void)
{
        ui_widget_t *column = create_flex("column", 100, 200);
        ui_widget_t *item = create_block(20, 0);

        set_style(item, "height", "25%");
        ui_widget_append(column, item);
        append_root(column);
        ui_update();

        ctest_equal_bool(
            "F11 percentage height should use the column content height",
            close_to(item->border_box.height, 50, TOL), true);

        ui_widget_destroy(column);
}

/* F12: min/max constraints. */
static void test_max_width(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *item = create_block(50, 20);

        set_px(item, "max-width", 30);
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F12 max-width should clamp the used width",
                         close_to(item->border_box.width, 30, TOL), true);

        ui_widget_destroy(row);
}

static void test_min_width(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *item = create_block(50, 20);

        set_px(item, "min-width", 80);
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F12 min-width should expand the used width",
                         close_to(item->border_box.width, 80, TOL), true);

        ui_widget_destroy(row);
}

static void test_min_max_height(void)
{
        ui_widget_t *column = create_flex("column", 100, 300);
        ui_widget_t *low = create_block(20, 20);
        ui_widget_t *high = create_block(20, 20);

        set_px(low, "min-height", 50);
        set_px(high, "max-height", 10);
        ui_widget_append(column, low);
        ui_widget_append(column, high);
        append_root(column);
        ui_update();

        ctest_equal_bool("F12 min-height should expand the used height",
                         close_to(low->border_box.height, 50, TOL), true);
        ctest_equal_bool("F12 max-height should clamp the used height",
                         close_to(high->border_box.height, 10, TOL), true);

        ui_widget_destroy(column);
}

/* F13: box-sizing in a flex item. */
static void test_box_sizing_content_box(void)
{
        ui_widget_t *row = create_flex("row", 200, 50);
        ui_widget_t *item = create_block(0, 20);

        set_style(item, "width", "50%");
        set_px(item, "padding-left", 10);
        set_px(item, "padding-right", 10);
        set_style(item, "box-sizing", "content-box");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F13 content-box width should exclude padding",
                         close_to(item->content_box.width, 100, TOL), true);
        ctest_equal_bool("F13 content-box border box should include padding",
                         close_to(item->border_box.width, 120, TOL), true);

        ui_widget_destroy(row);
}

static void test_box_sizing_border_box(void)
{
        ui_widget_t *row = create_flex("row", 200, 50);
        ui_widget_t *item = create_block(0, 20);

        set_style(item, "width", "50%");
        set_px(item, "padding-left", 10);
        set_px(item, "padding-right", 10);
        set_style(item, "box-sizing", "border-box");
        ui_widget_append(row, item);
        append_root(row);
        ui_update();

        ctest_equal_bool("F13 border-box border box should keep the used width",
                         close_to(item->border_box.width, 100, TOL), true);
        ctest_equal_bool("F13 border-box content width should exclude padding",
                         close_to(item->content_box.width, 80, TOL), true);

        ui_widget_destroy(row);
}

/* F14: inline-flex participates inline, flex fills the block width. */
static void test_inline_flex(void)
{
        ui_widget_t *parent = create_block(300, 100);
        ui_widget_t *inline_row = create_flex("row", 0, 0);
        ui_widget_t *block_row = create_flex("row", 0, 0);
        ui_widget_t *a = create_block(20, 20);
        ui_widget_t *b = create_block(20, 20);

        set_style(inline_row, "display", "inline-flex");
        ui_widget_append(inline_row, a);
        ui_widget_append(inline_row, b);
        ui_widget_append(parent, inline_row);
        append_root(parent);
        ui_update();

        ctest_equal_bool("F14 inline-flex should shrink to its content",
                         close_to(inline_row->border_box.width, 40, TOL), true);

        ui_widget_unlink(inline_row);
        ui_update();
        ui_widget_append(block_row, create_block(20, 20));
        ui_widget_append(block_row, create_block(20, 20));
        ui_widget_append(parent, block_row);
        ui_update();

        ctest_equal_bool("F14 flex should fill the block width",
                         close_to(block_row->border_box.width, 300, TOL), true);

        ui_widget_destroy(inline_row);
        ui_widget_destroy(parent);
}

/* F15: out-of-flow children. */
static void test_out_of_flow_children(void)
{
        ui_widget_t *row = create_flex("row", 200, 0);
        ui_widget_t *visible = create_block(50, 20);
        ui_widget_t *hidden = create_block(100, 100);
        ui_widget_t *absolute = create_block(500, 500);
        ui_widget_t *after = create_block(50, 20);

        ui_widget_hide(hidden);
        set_style(absolute, "position", "absolute");
        ui_widget_append(row, visible);
        ui_widget_append(row, hidden);
        ui_widget_append(row, absolute);
        ui_widget_append(row, after);
        append_root(row);
        ui_update();

        ctest_equal_bool("F15 hidden child should not occupy main-axis space",
                         close_to(after->border_box.x, 50, TOL), true);
        ctest_equal_bool("F15 absolute child should not occupy space",
                         close_to(after->border_box.x, 50, TOL), true);
        ctest_equal_bool("F15 out-of-flow children should not grow the row",
                         close_to(row->content_box.height, 20, TOL), true);

        ui_widget_destroy(row);
}

/* F15: absolute static position follows justify-content / align-items. */
static void test_out_of_flow_static_position(void)
{
        ui_widget_t *row = create_flex("row", 200, 100);
        ui_widget_t *visible = create_block(50, 20);
        ui_widget_t *absolute = create_block(20, 10);

        set_style(row, "justify-content", "flex-end");
        set_style(row, "align-items", "center");
        set_style(absolute, "position", "absolute");
        ui_widget_append(row, visible);
        ui_widget_append(row, absolute);
        append_root(row);
        ui_update();

        ctest_equal_bool("F15 static x follows justify-content flex-end",
                         close_to(absolute->border_box.x, 180, TOL), true);
        ctest_equal_bool("F15 static y follows align-items center",
                         close_to(absolute->border_box.y, 45, TOL), true);

        set_style(row, "justify-content", "flex-start");
        set_style(row, "align-items", "flex-start");
        ui_update();
        ctest_equal_bool("F15 flex-start static x",
                         close_to(absolute->border_box.x, 0, TOL), true);
        ctest_equal_bool("F15 flex-start static y",
                         close_to(absolute->border_box.y, 0, TOL), true);

        set_px(absolute, "left", 12);
        set_px(absolute, "top", 8);
        ui_update();
        ui_update();
        ctest_equal_float("F15 left inset overrides static x",
                          absolute->border_box.x, 12.f);
        ctest_equal_float("F15 top inset overrides static y",
                          absolute->border_box.y, 8.f);

        ui_widget_destroy(row);
}

/* F16: unset flex-direction inherits the parent direction. */
static void test_direction_inheritance(void)
{
        ui_widget_t *outer = create_flex("column", 200, 300);
        ui_widget_t *inner = ui_create_widget(NULL);
        ui_widget_t *a = create_block(20, 20);
        ui_widget_t *b = create_block(20, 20);

        set_style(inner, "display", "flex");
        set_style(inner, "flex-direction", "inherit");
        ui_widget_append(inner, a);
        ui_widget_append(inner, b);
        ui_widget_append(outer, inner);
        append_root(outer);
        ui_update();

        ctest_equal_bool("F16 inherited direction should stack items",
                         b->border_box.y > a->border_box.y + TOL, true);
        ctest_equal_bool("F16 inherited direction should keep one column",
                         close_to(a->border_box.x, b->border_box.x, TOL), true);

        ui_widget_destroy(outer);
}

/* F17: repeated updates must not drift. */
static void test_repeated_updates_stable(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *a = create_block(50, 20);
        ui_widget_t *b = create_block(30, 20);
        float b_x, a_width;

        set_style(a, "flex-grow", "1");
        set_style(b, "margin-left", "auto");
        ui_widget_append(row, a);
        ui_widget_append(row, b);
        append_root(row);
        ui_update();

        b_x = b->border_box.x;
        a_width = a->border_box.width;
        ui_update();
        ui_update();

        ctest_equal_bool("F17 grow width should be stable after updates",
                         close_to(a->border_box.width, a_width, TOL), true);
        ctest_equal_bool("F17 auto margin position should be stable",
                         close_to(b->border_box.x, b_x, TOL), true);

        ui_widget_destroy(row);
}

/* F19: nested flex row/column width propagation. */
static void test_nested_flex_width(void)
{
        ui_widget_t *row = create_flex("row", 300, 80);
        ui_widget_t *side = create_block(80, 40);
        ui_widget_t *main = create_flex("column", 0, 0);
        ui_widget_t *text = create_text(WRAP_TEXT);

        set_style(side, "flex-shrink", "0");
        set_style(main, "flex-grow", "1");
        set_px(main, "flex-basis", 0);
        ui_widget_append(main, text);
        ui_widget_append(row, side);
        ui_widget_append(row, main);
        append_root(row);
        ui_update();

        ctest_equal_bool("F19 nested column should fill the remaining width",
                         close_to(main->border_box.width, 220, TOL), true);
        ctest_equal_bool("F19 nested text should stay inside its column",
                         text->border_box.width <= 220.f + TOL, true);

        ui_widget_destroy(row);
}

/* C02: navbar with brand, navigation, spacer and actions. */
static void test_navbar(void)
{
        ui_widget_t *bar = create_flex("row", 400, 40);
        ui_widget_t *brand = create_block(60, 24);
        ui_widget_t *nav = create_flex("row", 0, 24);
        ui_widget_t *spacer = create_block(0, 0);
        ui_widget_t *actions = create_block(40, 24);

        ui_widget_append(nav, create_block(30, 24));
        ui_widget_append(nav, create_block(30, 24));
        set_style(spacer, "flex-grow", "1");
        ui_widget_append(bar, brand);
        ui_widget_append(bar, nav);
        ui_widget_append(bar, spacer);
        ui_widget_append(bar, actions);
        append_root(bar);
        ui_update();

        ctest_equal_bool("C02 brand should start at the leading edge",
                         close_to(brand->border_box.x, 0, TOL), true);
        ctest_equal_bool("C02 nav should follow the brand",
                         close_to(nav->border_box.x, 60, TOL), true);
        ctest_equal_bool("C02 actions should stick to the trailing edge",
                         close_to(right_in(actions, bar), 400, TOL), true);
        ctest_equal_bool("C02 spacer should take the remaining width",
                         close_to(spacer->border_box.width, 240, TOL), true);

        ui_widget_destroy(bar);
}

/* C03: sidebar + flexible content. */
static void test_sidebar_content(void)
{
        ui_widget_t *row = create_flex("row", 400, 200);
        ui_widget_t *sidebar = create_block(120, 0);
        ui_widget_t *content = create_block(0, 0);

        set_style(sidebar, "flex-shrink", "0");
        set_style(content, "flex-grow", "1");
        ui_widget_append(row, sidebar);
        ui_widget_append(row, content);
        append_root(row);
        ui_update();

        ctest_equal_bool("C03 sidebar should keep its fixed width",
                         close_to(sidebar->border_box.width, 120, TOL), true);
        ctest_equal_bool("C03 content should fill the remaining width",
                         close_to(content->border_box.width, 280, TOL), true);

        ui_widget_destroy(row);
}

/* C05: wrapping toolbar. */
static void test_wrap_toolbar(void)
{
        ui_widget_t *bar = create_flex("row", 100, 0);
        ui_widget_t *a = create_block(40, 20);
        ui_widget_t *b = create_block(40, 20);
        ui_widget_t *c = create_block(40, 20);

        set_style(bar, "flex-wrap", "wrap");
        set_px(bar, "gap", 5);
        ui_widget_append(bar, a);
        ui_widget_append(bar, b);
        ui_widget_append(bar, c);
        append_root(bar);
        ui_update();

        ctest_equal_bool("C05 first two buttons should share a line",
                         close_to(a->border_box.y, b->border_box.y, TOL), true);
        ctest_equal_bool("C05 third button should wrap",
                         c->border_box.y > a->border_box.y + TOL, true);
        ctest_equal_bool("C05 wrapped line should use the row gap",
                         close_to(c->border_box.y, 25, TOL), true);

        ui_widget_destroy(bar);
}

/* C06: card grid. */
static void test_card_grid(void)
{
        ui_widget_t *grid = create_flex("row", 340, 200);
        ui_widget_t *cards[4];
        int i;

        set_style(grid, "flex-wrap", "wrap");
        set_px(grid, "gap", 10);
        for (i = 0; i < 4; ++i) {
                cards[i] = create_block(160, 50);
                ui_widget_append(grid, cards[i]);
        }
        append_root(grid);
        ui_update();

        ctest_equal_bool(
            "C06 first two cards should share a row",
            close_to(cards[0]->border_box.y, cards[1]->border_box.y, TOL),
            true);
        ctest_equal_bool("C06 third card should wrap to the next row",
                         cards[2]->border_box.y > cards[0]->border_box.y + TOL,
                         true);
        ctest_equal_bool("C06 wrapped card should start at the row origin",
                         close_to(cards[2]->border_box.x, 0, TOL), true);
        ctest_equal_bool("C06 card width should stay fixed",
                         close_to(cards[0]->border_box.width, 160, TOL), true);

        ui_widget_destroy(grid);
}

/* C07: media object. */
static void test_media_object(void)
{
        ui_widget_t *row = create_flex("row", 300, 100);
        ui_widget_t *media = create_block(48, 48);
        ui_widget_t *content = create_flex("column", 0, 0);
        ui_widget_t *text = create_text(WRAP_TEXT);
        ui_widget_t *ref = create_text_reference(WRAP_TEXT);

        set_style(media, "flex-shrink", "0");
        set_style(content, "flex-grow", "1");
        ui_widget_append(content, text);
        ui_widget_append(row, media);
        ui_widget_append(row, content);
        append_root(row);
        ui_update();

        ctest_equal_bool("C07 media should keep its fixed width",
                         close_to(media->border_box.width, 48, TOL), true);
        ctest_equal_bool("C07 content should fill the remaining width",
                         close_to(content->border_box.width, 252, TOL), true);
        ctest_equal_bool(
            "C07 text should wrap inside the content column",
            text->content_box.height > ref->content_box.height * 1.5f, true);

        ui_widget_destroy(row);
        ui_widget_destroy(ref);
}

/* C08: form row. */
static void test_form_row(void)
{
        ui_widget_t *row = create_flex("row", 300, 40);
        ui_widget_t *label = create_block(60, 20);
        ui_widget_t *input = create_block(0, 20);
        ui_widget_t *button = create_block(50, 20);

        set_style(input, "flex-grow", "1");
        ui_widget_append(row, label);
        ui_widget_append(row, input);
        ui_widget_append(row, button);
        append_root(row);
        ui_update();

        ctest_equal_bool("C08 input should fill the remaining width",
                         close_to(input->border_box.width, 190, TOL), true);
        ctest_equal_bool("C08 button should keep its width",
                         close_to(button->border_box.width, 50, TOL), true);

        ui_widget_destroy(row);
}

/* C10: list row with a trailing action. */
static void test_trailing_action_row(void)
{
        ui_widget_t *row = create_flex("row", 300, 40);
        ui_widget_t *text = create_block(0, 20);
        ui_widget_t *action = create_block(30, 20);

        set_style(text, "flex-grow", "1");
        ui_widget_append(row, text);
        ui_widget_append(row, action);
        append_root(row);
        ui_update();

        ctest_equal_bool("C10 text should fill the leading space",
                         close_to(text->border_box.width, 270, TOL), true);
        ctest_equal_bool("C10 action should stick to the trailing edge",
                         close_to(right_in(action, row), 300, TOL), true);

        ui_widget_destroy(row);
}

/* F05: cross-axis auto margins. */
static void test_auto_margin_cross_axis(void)
{
        ui_widget_t *row = create_flex("row", 300, 200);
        ui_widget_t *top_auto = create_block(30, 30);
        ui_widget_t *both_auto = create_block(30, 30);

        set_style(top_auto, "margin-top", "auto");
        set_style(both_auto, "margin-top", "auto");
        set_style(both_auto, "margin-bottom", "auto");
        ui_widget_append(row, top_auto);
        ui_widget_append(row, both_auto);
        append_root(row);
        ui_update();

        ctest_equal_bool("F05 top auto margin should push the item down",
                         close_to(top_auto->border_box.y, 170, TOL), true);
        ctest_equal_bool("F05 two auto margins should center the item",
                         close_to(both_auto->border_box.y, 85, TOL), true);

        ui_widget_destroy(row);
}

/* F08: flex-basis: content. */
static void test_flex_basis_content(void)
{
        ui_widget_t *row = create_flex("row", 300, 50);
        ui_widget_t *text = create_text("Content basis");
        ui_widget_t *auto_row = create_flex("row", 300, 50);
        ui_widget_t *auto_text = create_text("Content basis");

        set_style(text, "flex-basis", "content");
        ui_widget_append(row, text);
        ui_widget_append(auto_row, auto_text);
        append_root(row);
        append_root(auto_row);
        ui_update();

        ctest_equal_bool(
            "F08 content basis should match the auto basis width",
            close_to(text->border_box.width, auto_text->border_box.width, TOL),
            true);
        ctest_equal_bool("F08 content basis should not collapse",
                         text->border_box.width > 1.f, true);

        ui_widget_destroy(row);
        ui_widget_destroy(auto_row);
}

/* F12: percentage min/max constraints. */
static void test_percentage_min_max_width(void)
{
        ui_widget_t *row = create_flex("row", 200, 50);
        ui_widget_t *bounded = create_block(50, 20);
        ui_widget_t *expanded = create_block(50, 20);

        set_style(bounded, "max-width", "20%");
        set_style(expanded, "min-width", "40%");
        ui_widget_append(row, bounded);
        ui_widget_append(row, expanded);
        append_root(row);
        ui_update();

        ctest_equal_bool("F12 percentage max-width should clamp width",
                         close_to(bounded->border_box.width, 40, TOL), true);
        ctest_equal_bool("F12 percentage min-width should expand width",
                         close_to(expanded->border_box.width, 80, TOL), true);

        ui_widget_destroy(row);
}

/* C01: centered hero. */
static void test_hero_layout(void)
{
        ui_widget_t *hero = create_flex("column", 400, 300);
        ui_widget_t *title = create_text("Hero title");
        ui_widget_t *desc = create_text(WRAP_TEXT);
        ui_widget_t *actions = create_flex("row", 0, 0);
        ui_widget_t *ref_desc = create_text_reference(WRAP_TEXT);

        set_style(hero, "align-items", "center");
        set_style(hero, "justify-content", "center");
        ui_widget_append(actions, create_block(80, 30));
        ui_widget_append(actions, create_block(80, 30));
        ui_widget_append(hero, title);
        ui_widget_append(hero, desc);
        ui_widget_append(hero, actions);
        append_root(hero);
        ui_update();

        ctest_equal_bool(
            "C01 title should be centered",
            close_to(title->border_box.x + title->border_box.width * 0.5f, 200,
                     2),
            true);
        ctest_equal_bool(
            "C01 actions should be centered",
            close_to(actions->border_box.x + actions->border_box.width * 0.5f,
                     200, 2),
            true);
        ctest_equal_bool(
            "C01 description should wrap",
            desc->content_box.height > ref_desc->content_box.height * 1.5f,
            true);

        ui_widget_destroy(hero);
        ui_widget_destroy(ref_desc);
}

/* C09: equal-width tabs. */
static void test_tabs_equal_width(void)
{
        ui_widget_t *row = create_flex("row", 300, 40);
        ui_widget_t *tabs[3];
        int i;

        for (i = 0; i < 3; ++i) {
                tabs[i] = create_block(0, 30);
                set_style(tabs[i], "flex-grow", "1");
                set_px(tabs[i], "flex-basis", 0);
                ui_widget_append(row, tabs[i]);
        }
        append_root(row);
        ui_update();

        for (i = 0; i < 3; ++i) {
                ctest_equal_bool("C09 tab should take one third of the row",
                                 close_to(tabs[i]->border_box.width, 100, TOL),
                                 true);
                ctest_equal_bool(
                    "C09 tabs should follow each other",
                    close_to(tabs[i]->border_box.x, i * 100.f, TOL), true);
        }

        ui_widget_destroy(row);
}

void test_flex_contract(void)
{
        lcui_init();
        ui_widget_set_title(ui_root(), L"test flex contract");

        ctest_describe("F01 default row", test_default_row);
        ctest_describe("F02 column placement", test_column_placement);
        ctest_describe("F03 justify center", test_justify_center);
        ctest_describe("F03 justify flex-end", test_justify_flex_end);
        ctest_describe("F03 justify space-between", test_justify_space_between);
        ctest_describe("F03 justify space-around", test_justify_space_around);
        ctest_describe("F03 justify space-evenly", test_justify_space_evenly);
        ctest_describe("F04 align flex-start", test_align_start);
        ctest_describe("F04 align center", test_align_center);
        ctest_describe("F04 align flex-end", test_align_flex_end);
        ctest_describe("F04 align stretch", test_align_stretch);
        ctest_describe("F05 auto margin", test_auto_margin_main_axis);
        ctest_describe("F05 cross axis auto margin",
                       test_auto_margin_cross_axis);
        ctest_describe("F06 flex grow", test_flex_grow);
        ctest_describe("F07 flex shrink", test_flex_shrink);
        ctest_describe("F08 flex basis fixed", test_flex_basis_fixed);
        ctest_describe("F08 flex basis percentage", test_flex_basis_percentage);
        ctest_describe("F08 flex basis content", test_flex_basis_content);
        ctest_describe("F09 wrap", test_flex_wrap);
        ctest_describe("F09 nowrap", test_flex_nowrap);
        ctest_describe("F10 row gap", test_gap_row);
        ctest_describe("F10 column gap", test_gap_column);
        ctest_describe("F11 percentage width row", test_percentage_width_row);
        ctest_describe("F11 percentage height column",
                       test_percentage_height_column);
        ctest_describe("F12 max-width", test_max_width);
        ctest_describe("F12 min-width", test_min_width);
        ctest_describe("F12 min/max height", test_min_max_height);
        ctest_describe("F12 percentage min/max width",
                       test_percentage_min_max_width);
        ctest_describe("F13 box-sizing content-box",
                       test_box_sizing_content_box);
        ctest_describe("F13 box-sizing border-box", test_box_sizing_border_box);
        ctest_describe("F14 inline-flex", test_inline_flex);
        ctest_describe("F15 out-of-flow children", test_out_of_flow_children);
        ctest_describe("F15 absolute static position",
                       test_out_of_flow_static_position);
        ctest_describe("F16 direction inheritance", test_direction_inheritance);
        ctest_describe("F17 repeated updates stable",
                       test_repeated_updates_stable);
        ctest_describe("F19 nested flex width", test_nested_flex_width);
        ctest_describe("C02 navbar", test_navbar);
        ctest_describe("C03 sidebar content", test_sidebar_content);
        ctest_describe("C05 wrap toolbar", test_wrap_toolbar);
        ctest_describe("C06 card grid", test_card_grid);
        ctest_describe("C07 media object", test_media_object);
        ctest_describe("C08 form row", test_form_row);
        ctest_describe("C10 trailing action row", test_trailing_action_row);
        ctest_describe("C01 hero layout", test_hero_layout);
        ctest_describe("C09 equal width tabs", test_tabs_equal_width);

        lcui_destroy();
}
