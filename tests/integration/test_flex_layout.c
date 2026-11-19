/*
 * tests/cases/test_flex_layout.c
 *
 * Copyright (c) 2023, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <LCUI/widgets/text.h>
#include <ctest-custom.h>
#include "../scenes/flex_layout_scene.h"

#include <math.h>
#include <stdio.h>

/* Internal (non-public) constraint API used by the column-wrap tests. */
void ui_widget_set_available_width_constraint(ui_widget_t *child,
                                              float available_width,
                                              ui_widget_t *source,
                                              unsigned generation);
bool ui_widget_get_available_width(ui_widget_t *w, float *available_width);
void ui_widget_reset_height(ui_widget_t *w);
void ui_widget_update_style(ui_widget_t *w);

#define SCROLLBAR_WIDTH 10

#define LONG_TEXT                                                           \
        "The quick brown fox jumps over the lazy dog repeatedly until the " \
        "line is far too long to fit inside a narrow column without wrapping."

static void test_flex_layout_with_content_width(float width)
{
        float content_width = width - 22;

        ui_widget_t *w;
        ui_widget_t *content;
        ui_widget_t *example;
        ui_widget_t *container;
        ui_rect_t rect;

        container = ui_widget_get_child(ui_get_widget("main"), 0);
        ui_update();

        rect.y = 10;
        rect.x = 10;
        rect.width = width;
        rect.height = 104;
        w = ui_widget_get_child(container, 0);
        ctest_equal_ui_rect("$('.example')[0].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 1);
        ctest_equal_ui_rect("$('.example')[1].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 2);
        ctest_equal_ui_rect("$('.example')[2].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 3);
        ctest_equal_ui_rect("$('.example')[3].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 4);
        ctest_equal_ui_rect("$('.example')[4].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 5);
        ctest_equal_ui_rect("$('.example')[5].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 6);
        ctest_equal_ui_rect("$('.example')[6].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        rect.height = 344;
        w = ui_widget_get_child(container, 7);
        ctest_equal_ui_rect("$('.example')[7].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 204;
        w = ui_widget_get_child(container, 8);
        ctest_equal_ui_rect("$('.example')[8].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 9);
        ctest_equal_ui_rect("$('.example')[9].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 10);
        ctest_equal_ui_rect("$('.example')[10].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 11);
        ctest_equal_ui_rect("$('.example')[11].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 12);
        ctest_equal_ui_rect("$('.example')[12].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 13);
        ctest_equal_ui_rect("$('.example')[13].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 254;
        w = ui_widget_get_child(container, 14);
        ctest_equal_ui_rect("$('.example')[14].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 184;
        w = ui_widget_get_child(container, 15);
        ctest_equal_ui_rect("$('.example')[15].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 16);
        ctest_equal_ui_rect("$('.example')[16].border_box", &w->border_box,
                            &rect);

        // (1) justify-content: center;;

        example = ui_widget_get_child(container, 1);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5 + (content_width - 300) / 2.f;
        rect.y = 5;
        rect.width = 50;
        rect.height = 50;
        ctest_equal_ui_rect("$('.example:eq(1) .box')[0].border_box",
                            &w->border_box, &rect);

        // (2) justify-content: flex-end;

        example = ui_widget_get_child(container, 2);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5 + (content_width - 300);
        ctest_equal_ui_rect("$('.example:eq(2) .box')[0].border_box",
                            &w->border_box, &rect);

        // (3) justify-content: space-between;

        example = ui_widget_get_child(container, 3);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        ctest_equal_ui_rect("$('.example:eq(3) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x = 5 + 50 + (content_width - 300) / 5.f;
        ctest_equal_ui_rect("$('.example:eq(3) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_get_child(content, 5);
        rect.x = 5 + (content_width - 50);
        ctest_equal_ui_rect("$('.example:eq(3) .box')[5].border_box",
                            &w->border_box, &rect);

        // (4) justify-content: space-around;

        example = ui_widget_get_child(container, 4);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5 + (content_width - 300) / 6.f / 2.f;
        ctest_equal_ui_rect("$('.example:eq(4) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50 + (content_width - 300) / 6.f;
        ctest_equal_ui_rect("$('.example:eq(4) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_get_child(content, 5);
        rect.x = 5 + content_width - (content_width - 300) / 6.f / 2.f - 50;
        ctest_equal_ui_rect("$('.example:eq(4) .box')[5].border_box",
                            &w->border_box, &rect);

        // (5) Auto margin

        example = ui_widget_get_child(container, 5);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50 + (content_width - 200) / 2.f;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[2].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50 + (content_width - 200) / 2.f;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[3].border_box",
                            &w->border_box, &rect);

        // (7) flex-grow

        example = ui_widget_get_child(container, 7);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.width = content_width / 3.f;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[2].border_box",
                            &w->border_box, &rect);

        // line 1

        content = ui_widget_get_child(example, 2);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.width = content_width - 50 - 50;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[3].border_box",
                            &w->border_box, &rect);

        // line 2

        content = ui_widget_get_child(example, 3);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.width = (content_width - 50) / 2.f;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[6].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[7].border_box",
                            &w->border_box, &rect);

        // line 3

        content = ui_widget_get_child(example, 4);
        w = ui_widget_get_child(content, 1);
        rect.x = 5 + 50;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[10].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[11].border_box",
                            &w->border_box, &rect);

        // line 4

        content = ui_widget_get_child(example, 5);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.width = 12 + (content_width - 3 * 12) * 4.f / 7.f;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[12].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        rect.width = 12 + (content_width - 3 * 12) * 2.f / 7.f;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[13].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        rect.width = 12 + (content_width - 3 * 12) / 7.f;
        ctest_equal_ui_rect("$('.example:eq(7) .box')[14].border_box",
                            &w->border_box, &rect);

        // (10) align-items: center; height: 300px;

        example = ui_widget_get_child(container, 10);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5 + (300 / 3.f - 50) / 2.f;
        rect.width = 50;
        ctest_equal_ui_rect("$('.example:eq(10) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50;
        ctest_equal_ui_rect("$('.example:eq(10) .box')[1].border_box",
                            &w->border_box, &rect);

        // (11) align-items: end; height: 300px;

        example = ui_widget_get_child(container, 11);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5 + (300 / 3.f - 50);
        rect.width = 50;
        ctest_equal_ui_rect("$('.example:eq(11) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50;
        ctest_equal_ui_rect("$('.example:eq(11) .box')[1].border_box",
                            &w->border_box, &rect);

        // (12) align-items: stretch; height: 300px;

        example = ui_widget_get_child(container, 12);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = 50;
        rect.height = 100;
        ctest_equal_ui_rect("$('.example:eq(12) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50;
        ctest_equal_ui_rect("$('.example:eq(12) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        w = ui_widget_next(w);
        rect.x = 5 + 3 * 50;
        rect.height = 50;
        ctest_equal_ui_rect("$('.example:eq(12) .box')[3].border_box",
                            &w->border_box, &rect);

        // (13) Vertically Centered Layout

        example = ui_widget_get_child(container, 13);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5 + (content_width - 50) / 2.f;
        rect.y = 5 + (300 - 50) / 2.f;
        rect.width = 50;
        rect.height = 50;
        ctest_equal_ui_rect("$('.example:eq(13) .box')[0].border_box",
                            &w->border_box, &rect);

        // (14) flex-direction: column;

        example = ui_widget_get_child(container, 14);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = 50;
        rect.height = 50;
        ctest_equal_ui_rect("$('.example:eq(14) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_get_child(content, 4);
        rect.x = 5 + 50 + (content_width - 50 - 150 - 200 - 50 - 50) / 5.f;
        rect.width = 150;
        ctest_equal_ui_rect("$('.example:eq(14) .box')[4].border_box",
                            &w->border_box, &rect);

        w = ui_widget_get_child(content, 8);
        rect.x += 150 + (content_width - 50 - 150 - 200 - 50 - 50) / 5.f;
        rect.width = 50;
        ctest_equal_ui_rect("$('.example:eq(14) .box')[8].border_box",
                            &w->border_box, &rect);

        // (15) Holy Grail Layout

        example = ui_widget_get_child(container, 15);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = content_width;
        rect.height = 50;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__header')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_get_child(content, 2);
        rect.y = 5 + 50 + 30;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__footer')[0].border_box",
            &w->border_box, &rect);

        rect.x = 0;
        rect.y = 0;
        rect.width = 100;
        rect.height = 30;
        w = ui_widget_get_child(content, 1);
        w = ui_widget_get_child(w, 0);
        ctest_equal_ui_rect("$('.example:eq(15) .layout__left')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x = 100;
        rect.width = content_width - 100 - 100;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__center')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        rect.width = 100;
        ctest_equal_ui_rect("$('.example:eq(15) .layout__right')[0].border_box",
                            &w->border_box, &rect);

        // (16) Holy Grail Layout (height: 300px)

        example = ui_widget_get_child(container, 16);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = content_width;
        rect.height = 50;
        ctest_equal_ui_rect(
            "$('.example:eq(16) .layout__header')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_get_child(content, 2);
        rect.y = 5 + 50 + 200;
        ctest_equal_ui_rect(
            "$('.example:eq(16) .layout__footer')[0].border_box",
            &w->border_box, &rect);

        rect.x = 0;
        rect.y = 0;
        rect.width = 100;
        rect.height = 200;
        w = ui_widget_get_child(content, 1);
        w = ui_widget_get_child(w, 0);
        ctest_equal_ui_rect("$('.example:eq(16) .layout__left')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x = 100;
        rect.width = content_width - 100 - 100;
        ctest_equal_ui_rect(
            "$('.example:eq(16) .layout__center')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        rect.width = 100;
        ctest_equal_ui_rect("$('.example:eq(16) .layout__right')[0].border_box",
                            &w->border_box, &rect);
}

static void test_browser_layout(void)
{
        ui_widget_t *w;
        ui_rect_t rect;

        w = ui_get_widget("browser-tabbar");
        w = ui_widget_get_child(w, 0);
        ctest_equal_int("$('#browser-tabbar .c-frame-tab')[0].width",
                        (int)w->border_box.width, 101);
        w = ui_widget_next(w);
        ctest_equal_int("$('#browser-tabbar .c-frame-tab')[1].width",
                        (int)w->border_box.width, 101);
        w = ui_widget_next(w);
        ctest_equal_int("$('#browser-tabbar .c-frame-tab')[2].width",
                        (int)w->border_box.width, 101);
        w = ui_widget_next(w);
        ctest_equal_int("$('#browser-tabbar .c-frame-tab')[3].width",
                        (int)w->border_box.width, 101);
        w = ui_widget_next(w);
        ctest_equal_int("$('#browser-tabbar .c-frame-tab')[4].width",
                        (int)w->border_box.width, 101);

        w = ui_get_widget("browser-frame-client");
        ctest_equal_int("$('#browser-frame-client')[0].height",
                        (int)w->border_box.height, 223);
        w = ui_get_widget("browser-frame-content");
        ctest_equal_int("$('#browser-frame-content')[0].height",
                        (int)w->border_box.height, 223);
        w = ui_get_widget("browser-page-home");
        ctest_equal_int("$('#browser-page-home')[0].height",
                        (int)w->border_box.height, 223);
        w = ui_get_widget("browser-page-home-container");
        rect.width = 256;
        rect.height = 70;
        rect.x =
            (538.f - rect.width) / 2.f + w->parent->computed_style.padding_left;
        rect.y =
            (203.f - rect.height) / 2.f + w->parent->computed_style.padding_top;
        ctest_equal_ui_rect("$('#browser-page-home-container')[0].border_box",
                            &w->border_box, &rect);
}

static void test_flex_layout_1280(void)
{
        ui_widget_resize(ui_root(), 1280, 800);
        test_flex_layout_with_content_width(580);
}

static void test_flex_layout_600(void)
{
        ui_widget_resize(ui_root(), 600, 400);
        test_flex_layout_with_content_width(580 - SCROLLBAR_WIDTH);
}

static void test_flex_layout_320(void)
{
        float width = 320 - 10 - 10 - SCROLLBAR_WIDTH;
        float content_width = width - 12 - 10;

        ui_widget_t *w;
        ui_widget_t *content;
        ui_widget_t *example;
        ui_widget_t *container;
        ui_rect_t rect;

        container = ui_widget_get_child(ui_get_widget("main"), 0);
        ui_widget_resize(ui_root(), 320, 240);
        ui_update();

        rect.y = 10;
        rect.x = 10;
        rect.width = width;
        rect.height = 104;
        w = ui_widget_get_child(container, 0);
        ctest_equal_ui_rect("$('.example')[0].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 1);
        ctest_equal_ui_rect("$('.example')[1].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 2);
        ctest_equal_ui_rect("$('.example')[2].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 3);
        ctest_equal_ui_rect("$('.example')[3].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 4);
        ctest_equal_ui_rect("$('.example')[4].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 5);
        ctest_equal_ui_rect("$('.example')[5].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        w = ui_widget_get_child(container, 6);
        ctest_equal_ui_rect("$('.example')[6].border_box", &w->border_box,
                            &rect);

        rect.y += 114;
        rect.height = 344;
        w = ui_widget_get_child(container, 7);
        ctest_equal_ui_rect("$('.example')[7].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 8);
        ctest_equal_ui_rect("$('.example')[8].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 9);
        ctest_equal_ui_rect("$('.example')[9].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
#ifdef _WIN32
        rect.height = 354;
#else
        /* Ubuntu Mono wraps this title to an extra line on Linux. */
        rect.height = 376;
#endif
        w = ui_widget_get_child(container, 10);
        ctest_equal_ui_rect("$('.example')[10].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 11);
        ctest_equal_ui_rect("$('.example')[11].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
#ifdef _WIN32
        rect.height = 354;
#else
        /* Ubuntu Mono wraps this title to an extra line on Linux. */
        rect.height = 376;
#endif
        w = ui_widget_get_child(container, 12);
        ctest_equal_ui_rect("$('.example')[12].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 13);
        ctest_equal_ui_rect("$('.example')[13].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 254;
        w = ui_widget_get_child(container, 14);
        ctest_equal_ui_rect("$('.example')[14].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 184;
        w = ui_widget_get_child(container, 15);
        ctest_equal_ui_rect("$('.example')[15].border_box", &w->border_box,
                            &rect);

        rect.y += rect.height + 10;
        rect.height = 354;
        w = ui_widget_get_child(container, 16);
        ctest_equal_ui_rect("$('.example')[16].border_box", &w->border_box,
                            &rect);

        // (1) justify-content: center;;

        example = ui_widget_get_child(container, 1);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = 50 + (content_width - 300) / 6.f;
        rect.height = 50;
        ctest_equal_ui_rect("$('.example:eq(1) .box')[0].border_box",
                            &w->border_box, &rect);

        // (2) justify-content: flex-end;

        example = ui_widget_get_child(container, 2);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        ctest_equal_ui_rect("$('.example:eq(2) .box')[0].border_box",
                            &w->border_box, &rect);

        // (3) justify-content: space-between;

        example = ui_widget_get_child(container, 3);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        ctest_equal_ui_rect("$('.example:eq(3) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        ctest_equal_ui_rect("$('.example:eq(3) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_get_child(content, 5);
        rect.x = 5 + content_width - rect.width;
        ctest_equal_ui_rect("$('.example:eq(3) .box')[5].border_box",
                            &w->border_box, &rect);

        // (5) Auto margin

        example = ui_widget_get_child(container, 5);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.width = 50;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50 + (content_width - 200) / 2.f;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[1].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[2].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += 50 + (content_width - 200) / 2.f;
        ctest_equal_ui_rect("$('.example:eq(5) .box')[3].border_box",
                            &w->border_box, &rect);

        // (15) Holy Grail Layout

        example = ui_widget_get_child(container, 15);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = content_width;
        rect.height = 50;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__header')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_get_child(content, 2);
        rect.y = 5 + 50 + 30;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__footer')[0].border_box",
            &w->border_box, &rect);

        rect.x = 0;
        rect.y = 0;
        rect.width = 100;
        rect.height = 30;
        w = ui_widget_get_child(content, 1);
        w = ui_widget_get_child(w, 0);
        ctest_equal_ui_rect("$('.example:eq(15) .layout__left')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x = 100;
        rect.width = content_width - 100 - 100;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__center')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        rect.width = 100;
        ctest_equal_ui_rect("$('.example:eq(15) .layout__right')[0].border_box",
                            &w->border_box, &rect);

        // (16) Holy Grail Layout (height: 300px)

        example = ui_widget_get_child(container, 16);
        content = ui_widget_get_child(example, 1);
        w = ui_widget_get_child(content, 0);
        rect.x = 5;
        rect.y = 5;
        rect.width = content_width;
        rect.height = 50;
        ctest_equal_ui_rect(
            "$('.example:eq(15) .layout__header')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_get_child(content, 2);
        rect.y = 5 + 50 + 200;
        ctest_equal_ui_rect(
            "$('.example:eq(16) .layout__footer')[0].border_box",
            &w->border_box, &rect);

        rect.x = 0;
        rect.y = 0;
        rect.width = 100;
        rect.height = 200;
        w = ui_widget_get_child(content, 1);
        w = ui_widget_get_child(w, 0);
        ctest_equal_ui_rect("$('.example:eq(16) .layout__left')[0].border_box",
                            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x = 100;
        rect.width = content_width - 100 - 100;
        ctest_equal_ui_rect(
            "$('.example:eq(16) .layout__center')[0].border_box",
            &w->border_box, &rect);

        w = ui_widget_next(w);
        rect.x += rect.width;
        rect.width = 100;
        ctest_equal_ui_rect("$('.example:eq(16) .layout__right')[0].border_box",
                            &w->border_box, &rect);
}

static void test_align_self(void)
{
        ui_widget_t *container;
        ui_widget_t *child;
        ui_rect_t rect;

        ui_widget_resize(ui_root(), 800, 600);
        container = ui_create_widget(NULL);
        ui_widget_set_style_string(container, "display", "flex");
        ui_widget_set_style_string(container, "align-items", "center");
        ui_widget_set_style_string(container, "width", "200px");
        ui_widget_set_style_string(container, "height", "100px");
        ui_widget_append(ui_root(), container);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "20px");
        ui_widget_set_style_string(child, "height", "20px");
        ui_widget_append(container, child);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "20px");
        ui_widget_set_style_string(child, "height", "20px");
        ui_widget_set_style_string(child, "align-self", "flex-start");
        ui_widget_append(container, child);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "20px");
        ui_widget_set_style_string(child, "height", "20px");
        ui_widget_set_style_string(child, "align-self", "flex-end");
        ui_widget_append(container, child);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "20px");
        ui_widget_set_style_string(child, "height", "20px");
        ui_widget_set_style_string(child, "align-self", "center");
        ui_widget_append(container, child);
        ui_update();

        rect.width = 20;
        rect.height = 20;
        rect.x = 0;
        rect.y = 40;
        child = ui_widget_get_child(container, 0);
        ctest_equal_ui_rect("align-self auto should use parent center",
                            &child->border_box, &rect);

        rect.x = 20;
        rect.y = 0;
        child = ui_widget_get_child(container, 1);
        ctest_equal_ui_rect("align-self flex-start should override center",
                            &child->border_box, &rect);

        rect.x = 40;
        rect.y = 80;
        child = ui_widget_get_child(container, 2);
        ctest_equal_ui_rect("align-self flex-end should override center",
                            &child->border_box, &rect);

        rect.x = 60;
        rect.y = 40;
        child = ui_widget_get_child(container, 3);
        ctest_equal_ui_rect("align-self center should match parent center",
                            &child->border_box, &rect);

        child = ui_widget_get_child(container, 1);
        ui_widget_set_style_string(child, "align-self", "flex-end");
        ui_update();
        rect.x = 20;
        rect.y = 80;
        ctest_equal_ui_rect("changing align-self should reflow the item",
                            &child->border_box, &rect);

        ui_widget_destroy(container);
        ui_clear_trash();
}

static void test_flex_layout_gap(void)
{
        ui_widget_t *root;
        ui_widget_t *container;
        ui_widget_t *child;
        ui_rect_t rect;

        root = ui_root();
        ui_widget_resize(root, 800, 600);

        /* (1) row direction with column-gap shorthand */

        container = ui_create_widget(NULL);
        ui_widget_set_style_string(container, "display", "flex");
        ui_widget_set_style_string(container, "width", "300px");
        ui_widget_set_style_string(container, "height", "60px");
        ui_widget_set_style_string(container, "gap", "10px");
        ui_widget_append(root, container);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        ui_update();

        rect.y = 0;
        rect.width = 50;
        rect.height = 50;

        child = ui_widget_get_child(container, 0);
        rect.x = 0;
        ctest_equal_ui_rect("row gap: child[0] border_box", &child->border_box,
                            &rect);

        child = ui_widget_get_child(container, 1);
        rect.x = 60;
        ctest_equal_ui_rect("row gap: child[1] border_box", &child->border_box,
                            &rect);

        child = ui_widget_get_child(container, 2);
        rect.x = 120;
        ctest_equal_ui_rect("row gap: child[2] border_box", &child->border_box,
                            &rect);

        child = ui_widget_get_child(container, 3);
        rect.x = 180;
        ctest_equal_ui_rect("row gap: child[3] border_box", &child->border_box,
                            &rect);

        ui_widget_remove(container);

        /* (2) column direction with row-gap */

        container = ui_create_widget(NULL);
        ui_widget_set_style_string(container, "display", "flex");
        ui_widget_set_style_string(container, "flex-direction", "column");
        ui_widget_set_style_string(container, "width", "100px");
        ui_widget_set_style_string(container, "height", "300px");
        ui_widget_set_style_string(container, "row-gap", "20px");
        ui_widget_append(root, container);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        ui_update();

        rect.x = 0;
        rect.width = 50;
        rect.height = 50;

        child = ui_widget_get_child(container, 0);
        rect.y = 0;
        ctest_equal_ui_rect("column gap: child[0] border_box",
                            &child->border_box, &rect);

        child = ui_widget_get_child(container, 1);
        rect.y = 70;
        ctest_equal_ui_rect("column gap: child[1] border_box",
                            &child->border_box, &rect);

        child = ui_widget_get_child(container, 2);
        rect.y = 140;
        ctest_equal_ui_rect("column gap: child[2] border_box",
                            &child->border_box, &rect);

        ui_widget_remove(container);

        /* (3) row + wrap with two-value gap */

        container = ui_create_widget(NULL);
        ui_widget_set_style_string(container, "display", "flex");
        ui_widget_set_style_string(container, "flex-wrap", "wrap");
        ui_widget_set_style_string(container, "width", "200px");
        ui_widget_set_style_string(container, "height", "85px");
        ui_widget_set_style_string(container, "gap", "5px 10px");
        ui_widget_append(root, container);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "80px");
        ui_widget_set_style_string(child, "height", "40px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "80px");
        ui_widget_set_style_string(child, "height", "40px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "80px");
        ui_widget_set_style_string(child, "height", "40px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "80px");
        ui_widget_set_style_string(child, "height", "40px");
        ui_widget_append(container, child);
        ui_update();

        rect.width = 80;
        rect.height = 40;

        child = ui_widget_get_child(container, 0);
        rect.x = 0;
        rect.y = 0;
        ctest_equal_ui_rect("wrap gap: child[0] border_box", &child->border_box,
                            &rect);

        child = ui_widget_get_child(container, 1);
        rect.x = 90;
        rect.y = 0;
        ctest_equal_ui_rect("wrap gap: child[1] border_box", &child->border_box,
                            &rect);

        child = ui_widget_get_child(container, 2);
        rect.x = 0;
        rect.y = 45;
        ctest_equal_ui_rect("wrap gap: child[2] border_box", &child->border_box,
                            &rect);

        child = ui_widget_get_child(container, 3);
        rect.x = 90;
        rect.y = 45;
        ctest_equal_ui_rect("wrap gap: child[3] border_box", &child->border_box,
                            &rect);

        ui_widget_remove(container);

        /* (4) gap: normal should fall back to 0 */

        container = ui_create_widget(NULL);
        ui_widget_set_style_string(container, "display", "flex");
        ui_widget_set_style_string(container, "width", "300px");
        ui_widget_set_style_string(container, "height", "60px");
        ui_widget_set_style_string(container, "gap", "normal");
        ui_widget_append(root, container);

        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        child = ui_create_widget(NULL);
        ui_widget_set_style_string(child, "width", "50px");
        ui_widget_set_style_string(child, "height", "50px");
        ui_widget_append(container, child);
        ui_update();

        rect.width = 50;
        rect.height = 50;

        child = ui_widget_get_child(container, 0);
        rect.x = 0;
        rect.y = 0;
        ctest_equal_ui_rect("normal gap: child[0] border_box",
                            &child->border_box, &rect);

        child = ui_widget_get_child(container, 1);
        rect.x = 50;
        rect.y = 0;
        ctest_equal_ui_rect("normal gap: child[1] border_box",
                            &child->border_box, &rect);

        ui_widget_remove(container);
        ui_update();
}

static void test_choice_card(void)
{
        ui_widget_t *preview = ui_get_widget("choice-card-preview");
        ui_widget_t *card1 = ui_get_widget("choice-card-1");
        ui_widget_t *card2 = ui_get_widget("choice-card-2");
        float w1_init, w2_init;
        ui_update();

        w1_init = card1->border_box.width;
        w2_init = card2->border_box.width;

        ctest_equal_float("card1 width should be 400", card1->border_box.width,
                          400);
        ctest_equal_float("card2 width should be 400", card2->border_box.width,
                          400);

        ui_widget_request_reflow(preview);
        ui_update();
        ctest_equal_float("card1 width stable after reflow 1",
                          card1->border_box.width, w1_init);
        ctest_equal_float("card2 width stable after reflow 1",
                          card2->border_box.width, w2_init);

        ui_widget_request_reflow(preview);
        ui_update();
        ctest_equal_float("card1 width stable after reflow 2",
                          card1->border_box.width, w1_init);
        ctest_equal_float("card2 width stable after reflow 2",
                          card2->border_box.width, w2_init);

        ui_widget_request_reflow(preview);
        ui_update();
        ctest_equal_float("card1 width stable after reflow 3",
                          card1->border_box.width, w1_init);
}

static ui_widget_t *build_column_with_long_text(float box_width)
{
        ui_widget_t *root = ui_root();
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *title = ui_create_widget("text");
        ui_widget_t *desc = ui_create_widget("text");
        char wbuf[32];

        snprintf(wbuf, sizeof(wbuf), "%gpx", box_width);
        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "width", wbuf);
        ui_text_set_content(title, "Title");
        ui_widget_set_style_string(desc, "white-space", "normal");
        ui_text_set_content(desc, LONG_TEXT);
        ui_widget_append(box, title);
        ui_widget_append(box, desc);
        ui_widget_append(root, box);
        ui_update();
        return box;
}

static void test_description_wraps_within_container(void)
{
        ui_widget_t *box = build_column_with_long_text(300.f);
        ui_widget_t *ref = build_column_with_long_text(3000.f);
        ui_widget_t *desc = ui_widget_get_child(box, 1);
        ui_widget_t *ref_desc = ui_widget_get_child(ref, 1);

        ctest_equal_bool(
            "desc wraps to greater height at 300px",
            desc->content_box.height > 1.5f * ref_desc->content_box.height,
            true);
        ctest_equal_bool("desc border_box stays within 300px",
                         desc->border_box.width <= 300.f + 1.f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static void on_afterlayout_count(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        (*(size_t *)e->data)++;
}

static void test_child_reflow_count_at_most_once(void)
{
        ui_widget_t *box = build_column_with_long_text(300.f);
        ui_widget_t *desc = ui_widget_get_child(box, 1);
        float h0 = desc->content_box.height;
        size_t n = 0;

        ui_widget_on(desc, "afterlayout", on_afterlayout_count, &n);
        /* Trigger one parent layout that changes both the main (height)
         * and cross (width) size of the column child: narrowing the
         * definite column width re-wraps the text. */
        ui_widget_set_style_string(box, "width", "120px");
        ui_update();
        ctest_equal_bool("desc re-wrapped at narrower column width",
                         desc->content_box.height > h0 + 2.f, true);
        ctest_equal_bool("column child reflowed exactly once", n == 1, true);

        ui_widget_destroy(box);
}

/* 约束宽不变时复用测量缓存；约束宽变化时重新测量。 */
static void test_measure_cache_skips_repeated_reflow(void)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header;
        size_t n = 0;

        ui_widget_set_style_string(empty, "display", "flex");
        ui_widget_set_style_string(empty, "flex-direction", "column");
        ui_widget_set_style_string(empty, "align-items", "center");
        ui_widget_set_style_string(empty, "width", "400px");
        ui_widget_append(ui_root(), empty);

        ui_empty_set_description(empty, LONG_TEXT);
        header = ui_widget_get_child(empty, 0);
        ui_widget_set_style_string(header, "display", "flex");
        ui_widget_set_style_string(header, "flex-direction", "column");
        ui_widget_set_style_string(header, "align-items", "center");
        ui_update();

        ui_widget_on(header, "afterlayout", on_afterlayout_count, &n);

        /* 约束宽不变：复用缓存，不重排。 */
        ui_widget_request_reflow(empty);
        ui_update();
        ctest_equal_bool(
            "generic container measurement reused when constraint unchanged",
            n == 0, true);

        /* 约束宽变化：缓存失效，重新测量一次。 */
        n = 0;
        ui_widget_set_style_string(empty, "width", "300px");
        ui_update();
        ctest_equal_bool("generic container remeasured when constraint changed",
                         n == 1, true);

        ui_widget_destroy(empty);
}

/* 居中嵌套容器先窄后宽：变宽后应重新展开，不被历史 used size 卡住。 */
static void test_centered_container_reexpands_after_widening(void)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header;
        float h300, h600;

        ui_widget_set_style_string(empty, "display", "flex");
        ui_widget_set_style_string(empty, "flex-direction", "column");
        ui_widget_set_style_string(empty, "align-items", "center");
        ui_widget_set_style_string(empty, "width", "300px");
        ui_widget_append(ui_root(), empty);

        ui_empty_set_description(empty, LONG_TEXT);
        header = ui_widget_get_child(empty, 0);
        ui_widget_set_style_string(header, "display", "flex");
        ui_widget_set_style_string(header, "flex-direction", "column");
        ui_widget_set_style_string(header, "align-items", "center");
        ui_update();
        h300 = header->content_box.height;

        ui_widget_set_style_string(empty, "width", "600px");
        ui_update();
        h600 = header->content_box.height;

        ctest_equal_bool("container shrinks when widened", h600 < h300 - 2.f,
                         true);

        ui_widget_destroy(empty);
}

/* max-width 应参与测量：即使容器更宽，文本也按 max-width 折行。 */
static void test_max_width_limits_measurement(void)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *desc = ui_create_widget("text");
        float h;

        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "align-items", "center");
        ui_widget_set_style_string(box, "width", "800px");
        ui_widget_set_style_string(desc, "white-space", "normal");
        ui_widget_set_style_string(desc, "max-width", "300px");
        ui_text_set_content(desc, LONG_TEXT);
        ui_widget_append(ui_root(), box);
        ui_widget_append(box, desc);
        ui_update();

        h = desc->content_box.height;

        ctest_equal_bool("text width stays within max-width",
                         desc->border_box.width <= 300.f + 1.f, true);
        ctest_equal_bool("text wraps taller than a single line", h > 30.f,
                         true);

        ui_widget_destroy(box);
}

/* 文本内容变化后，测量缓存必须失效并重新测量。 */
static void test_measure_cache_invalidates_on_content_change(void)
{
        ui_widget_t *empty = ui_create_empty();
        ui_widget_t *header;
        ui_widget_t *desc;
        float h_long, h_short;

        ui_widget_set_style_string(empty, "display", "flex");
        ui_widget_set_style_string(empty, "flex-direction", "column");
        ui_widget_set_style_string(empty, "align-items", "center");
        ui_widget_set_style_string(empty, "width", "400px");
        ui_widget_append(ui_root(), empty);

        ui_empty_set_description(empty, LONG_TEXT);
        header = ui_widget_get_child(empty, 0);
        desc = ui_widget_get_child(header, 0);
        ui_widget_set_style_string(header, "display", "flex");
        ui_widget_set_style_string(header, "flex-direction", "column");
        ui_widget_set_style_string(header, "align-items", "center");
        ui_widget_set_style_string(desc, "white-space", "normal");
        ui_update();
        h_long = desc->content_box.height;

        ui_text_set_content(desc, "Short.");
        ui_update();
        h_short = desc->content_box.height;

        ctest_equal_bool("content change invalidates measurement cache",
                         h_short < h_long - 2.f, true);

        ui_widget_destroy(empty);
}

/* 零高文本仍视为已测量：不得回退到可能过期的内在高度缓存。 */
static void test_zero_height_text_ignores_stale_cache(void)
{
        ui_widget_t *container = ui_create_widget(NULL);
        ui_widget_t *text = ui_create_widget("text");
        float h;

        ui_widget_set_style_string(container, "display", "flex");
        ui_widget_set_style_string(container, "flex-direction", "column");
        ui_widget_set_style_string(container, "width", "300px");
        ui_widget_append(ui_root(), container);
        ui_widget_append(container, text);
        ui_widget_set_style_string(text, "font-size", "0px");
        ui_text_set_content(text, "");
        ui_update();
        /* 首次布局后写入一段过期的内在高度缓存，再只触发重排
         * （不触发子项自身更新），确保缓存不会被更新覆盖。 */
        text->unconstrained_content_height = 120.f;
        ui_widget_request_reflow(container);
        ui_update();
        h = text->content_box.height;

        ctest_equal_bool("empty text does not reuse stale height", h <= 1.f,
                         true);

        ui_widget_destroy(container);
}

static void test_standalone_reflow_stays_wrapped(void)
{
        ui_widget_t *box = build_column_with_long_text(300.f);
        ui_widget_t *title = ui_widget_get_child(box, 0);
        ui_widget_t *desc = ui_widget_get_child(box, 1);
        float h0 = desc->content_box.height;
        float b0 = box->content_box.height;

        ui_widget_request_reflow(title);
        ui_update();
        ctest_equal_bool("desc height unchanged after sibling reflow",
                         fabsf(desc->content_box.height - h0) <= 2.f, true);
        ctest_equal_bool("box height unchanged after sibling reflow",
                         fabsf(box->content_box.height - b0) <= 2.f, true);
        ctest_equal_bool(
            "title does not overlap desc after sibling reflow",
            title->layout_y + title->outer_box.height <= desc->layout_y + 2.f,
            true);

        ui_widget_destroy(box);
}

static void test_wrap_container_overflows_without_clamp(void)
{
        ui_widget_t *parent;
        ui_widget_t *wrap;
        ui_widget_t *child;
        ui_widget_t *children[3];
        float w_prev;
        float w_cur;
        int i;

        /* A wrap container with indefinite cross (width) must overflow
         * instead of clamping. Appending it directly under the block root
         * would stretch its auto width (fill-available) into a definite
         * value, so nest it under a column parent with
         * align-items: flex-start: the parent does not stretch the child's
         * cross axis, leaving the wrap container's width auto (indefinite). */
        parent = ui_create_widget(NULL);
        ui_widget_set_style_string(parent, "display", "flex");
        ui_widget_set_style_string(parent, "flex-direction", "column");
        ui_widget_set_style_string(parent, "align-items", "flex-start");
        ui_widget_set_style_string(parent, "width", "400px");
        ui_widget_append(ui_root(), parent);

        wrap = ui_create_widget(NULL);
        ui_widget_set_style_string(wrap, "display", "flex");
        ui_widget_set_style_string(wrap, "flex-direction", "column");
        ui_widget_set_style_string(wrap, "flex-wrap", "wrap");
        ui_widget_set_style_string(wrap, "height", "100px");
        ui_widget_append(parent, wrap);

        for (i = 0; i < 3; ++i) {
                children[i] = ui_create_widget(NULL);
                ui_widget_set_style_string(children[i], "width", "40px");
                ui_widget_set_style_string(children[i], "height", "60px");
                ui_widget_append(wrap, children[i]);
        }
        ui_update();

        /* 3 children, each 60px high, exceed the 100px container height, so
         * they wrap into 3 columns; the container width accumulates the
         * column content (3 * 40 = 120) and overflows, neither collapsing to
         * 0 nor being clamped to the parent's 400px. */
        ctest_equal_bool("container width grew to fit three columns",
                         wrap->content_box.width >= 119.f &&
                             wrap->content_box.width <= 121.f,
                         true);
        ctest_equal_bool("container width not clamped to parent width",
                         wrap->content_box.width < 200.f, true);

        /* Children keep their fixed width (not clamped to the container
         * content width, not collapsed) and land in distinct columns. */
        for (i = 0; i < 3; ++i) {
                child = ui_widget_get_child(wrap, i);
                ctest_equal_bool("child width not clamped or collapsed",
                                 child->border_box.width >= 39.f &&
                                     child->border_box.width <= 41.f,
                                 true);
                if (i > 0) {
                        ctest_equal_bool(
                            "child placed in a later column",
                            child->border_box.x >
                                ui_widget_get_child(wrap, i - 1)->border_box.x,
                            true);
                }
        }

        /* Converges: geometry is stable after a fixed number of updates
         * (no oscillation, no runaway). */
        for (i = 0; i < 3; ++i) {
                ui_update();
        }
        w_prev = wrap->content_box.width;
        ui_update();
        w_cur = wrap->content_box.width;
        ctest_equal_float("container width stable between last two updates",
                          w_cur, w_prev);
        ctest_equal_bool("container width still overflows after settle",
                         wrap->content_box.width >= 119.f, true);

        ui_widget_destroy(parent);
}

static void test_constraint_api(void)
{
        ui_widget_t *p = ui_create_widget(NULL), *c = ui_create_widget("text");
        ui_widget_t *new_parent = ui_create_widget(NULL);
        float aw;

        ui_widget_append(p, c);
        ctest_equal_bool("fresh child: no constraint",
                         ui_widget_get_available_width(c, &aw), false);
        ui_widget_set_available_width_constraint(c, 120.f, p,
                                                 p->layout_generation);
        ctest_equal_bool("recorded: child reports constraint",
                         ui_widget_get_available_width(c, &aw), true);
        ctest_equal_float("recorded: available_width round-trips", aw, 120.f);
        ui_widget_append(new_parent, c);
        ctest_equal_bool("reparented: constraint invalid",
                         ui_widget_get_available_width(c, &aw), false);
        /* a stale generation (parent has not re-laid-out to that round) is
         * not a valid constraint */
        ui_widget_append(p, c);
        ui_widget_set_available_width_constraint(c, 90.f, p,
                                                 p->layout_generation + 1);
        ctest_equal_bool("stale generation: constraint invalid",
                         ui_widget_get_available_width(c, &aw), false);

        ui_widget_destroy(p);
        ui_widget_destroy(new_parent);
}

/* Cross-axis INDEFINITE column: long text must stay single line (no
 * constraint is recorded, so the child keeps its intrinsic size). */
static void test_indefinite_width_keeps_single_line(void)
{
        ui_widget_t *outer = ui_create_widget(NULL);
        ui_widget_t *col = ui_create_widget(NULL);
        ui_widget_t *desc = ui_create_widget("text");
        ui_widget_t *wide = build_column_with_long_text(3000.f);
        ui_widget_t *wide_desc = ui_widget_get_child(wide, 1);

        /* Nest under a column parent with align-items:flex-start so `col`
         * keeps an auto (indefinite) cross width; appending it directly to
         * the block root would stretch its width to a definite value. */
        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "align-items", "flex-start");
        ui_widget_set_style_string(outer, "width", "400px");
        ui_widget_append(ui_root(), outer);

        ui_widget_set_style_string(col, "display", "flex");
        ui_widget_set_style_string(col, "flex-direction", "column");
        ui_widget_set_style_string(desc, "white-space", "normal");
        ui_text_set_content(desc, LONG_TEXT);
        ui_widget_append(col, desc);
        ui_widget_append(outer, col);
        ui_update();

        ctest_equal_bool(
            "indefinite-width column keeps long text single line",
            desc->content_box.height <= wide_desc->content_box.height + 2.f,
            true);
        ctest_equal_bool(
            "indefinite column width fits its content",
            col->content_box.width >= desc->content_box.width - 2.f, true);

        ui_widget_destroy(outer);
        ui_widget_destroy(wide);
}

/* A child with a declared width wraps at its own width, not at the
 * parent-provided constraint. */
static void test_fixed_width_child_wraps_own_width(void)
{
        ui_widget_t *root = ui_root();
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *desc = ui_create_widget("text");
        ui_widget_t *ref = build_column_with_long_text(300.f);
        ui_widget_t *ref_desc = ui_widget_get_child(ref, 1);

        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "width", "300px");
        ui_widget_set_style_string(desc, "white-space", "normal");
        ui_widget_set_style_string(desc, "width", "100px");
        ui_text_set_content(desc, LONG_TEXT);
        ui_widget_append(box, desc);
        ui_widget_append(root, box);
        ui_update();

        ctest_equal_bool(
            "fixed-width child keeps its declared width",
            desc->border_box.width >= 99.f && desc->border_box.width <= 101.f,
            true);
        ctest_equal_bool(
            "fixed-width child wraps at its own (narrower) width",
            desc->content_box.height > ref_desc->content_box.height, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

static ui_widget_t *build_fixed_height_column(ui_widget_t **item,
                                              ui_widget_t **button)
{
        ui_widget_t *outer = ui_create_widget(NULL);

        *item = ui_create_widget(NULL);
        *button = ui_create_button();
        ui_widget_set_style_string(outer, "display", "flex");
        ui_widget_set_style_string(outer, "flex-direction", "column");
        ui_widget_set_style_string(outer, "width", "300px");
        ui_widget_set_style_string(*item, "display", "flex");
        ui_widget_set_style_string(*item, "flex-direction", "column");
        ui_widget_set_style_string(*item, "width", "100%");
        ui_widget_set_style_string(*button, "height", "32px");
        ui_widget_set_style_string(*button, "flex-shrink", "1");
        ui_button_set_text(*button, "Fixed height");
        ui_widget_append(*item, *button);
        ui_widget_append(outer, *item);
        ui_widget_append(ui_root(), outer);
        return outer;
}

static void test_fixed_height_child_restores_intrinsic_size(void)
{
        ui_widget_t *item;
        ui_widget_t *button;
        ui_widget_t *outer = build_fixed_height_column(&item, &button);

        ui_widget_set_style_string(outer, "height", "20px");
        ui_update();
        ui_update();
        ctest_equal_bool(
            "fixed-height child should shrink in a definite parent",
            button->border_box.height < 32.0f, true);

        ui_widget_set_style_string(outer, "height", "auto");
        ui_widget_update_style(outer);
        ui_widget_reset_height(outer);
        ui_widget_request_reflow(item);
        ui_widget_reflow(outer);
        ctest_equal_float(
            "fixed-height child should restore its declared height",
            button->border_box.height, 32.0f);
        ctest_equal_bool(
            "auto parent should contain the restored child",
            item->content_box.height + 0.5f >= button->border_box.height, true);

        ui_widget_destroy(outer);
}

static void test_auto_margins_excluded_from_intrinsic_height(void)
{
        ui_widget_t *item;
        ui_widget_t *button;
        ui_widget_t *outer = build_fixed_height_column(&item, &button);

        ui_widget_set_style_string(outer, "height", "64px");
        ui_widget_set_style_string(item, "flex-grow", "1");
        ui_widget_set_style_string(button, "margin-top", "auto");
        ui_update();
        ui_update();
        ctest_equal_bool("auto margin should be resolved in a definite parent",
                         button->computed_style.margin_top > 1.0f, true);

        ui_widget_set_style_string(outer, "height", "auto");
        ui_widget_update_style(outer);
        ui_widget_reset_height(outer);
        ui_widget_request_reflow(item);
        ui_widget_reflow(outer);
        ctest_equal_float("auto parent should exclude the old auto margin",
                          item->content_box.height, 32.0f);
        ctest_equal_float(
            "auto margin should reset during intrinsic measurement",
            button->computed_style.margin_top, 0.0f);

        ui_widget_destroy(outer);
}

static ui_widget_t *create_measure_column(void)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_set_style_string(w, "display", "flex");
        ui_widget_set_style_string(w, "flex-direction", "column");
        return w;
}

static ui_widget_t *create_measure_text(void)
{
        ui_widget_t *w = ui_create_widget("text");

        ui_widget_set_style_string(w, "white-space", "normal");
        ui_text_set_content(w, LONG_TEXT);
        return w;
}

static void test_measurement_paths_share_resolved_width(void)
{
        ui_widget_t *fast_parent = create_measure_column();
        ui_widget_t *reflow_parent = create_measure_column();
        ui_widget_t *fast_item = create_measure_column();
        ui_widget_t *reflow_item = create_measure_column();
        ui_widget_t *wrapper = create_measure_column();
        ui_widget_t *fast_text = create_measure_text();
        ui_widget_t *reflow_text = create_measure_text();

        ui_widget_set_style_string(fast_parent, "align-items", "center");
        ui_widget_set_style_string(reflow_parent, "align-items", "center");
        ui_widget_set_style_string(fast_parent, "width", "400px");
        ui_widget_set_style_string(reflow_parent, "width", "400px");
        ui_widget_set_style_string(fast_parent, "height", "auto");
        ui_widget_set_style_string(reflow_parent, "height", "auto");
        ui_widget_set_style_string(fast_item, "max-width", "200px");
        ui_widget_set_style_string(reflow_item, "max-width", "200px");
        ui_widget_append(fast_item, fast_text);
        ui_widget_append(wrapper, reflow_text);
        ui_widget_append(reflow_item, wrapper);
        ui_widget_append(fast_parent, fast_item);
        ui_widget_append(reflow_parent, reflow_item);
        ui_widget_append(ui_root(), fast_parent);
        ui_widget_append(ui_root(), reflow_parent);
        ui_update();
        ui_update();

        ctest_equal_float("fast and reflow paths should use the same width",
                          fast_item->content_box.width,
                          reflow_item->content_box.width);
        ctest_equal_float(
            "fast and reflow paths should produce the same text height",
            fast_text->content_box.height, reflow_text->content_box.height);
        ctest_equal_float(
            "fast and reflow paths should produce the same parent height",
            fast_parent->content_box.height, reflow_parent->content_box.height);
        ctest_equal_float("fast-path cache should record the resolved width",
                          fast_item->measured_content_width,
                          fast_item->content_box.width);
        ctest_equal_float("reflow cache should record the resolved width",
                          reflow_item->measured_content_width,
                          reflow_item->content_box.width);

        ui_widget_destroy(fast_parent);
        ui_widget_destroy(reflow_parent);
}

static void test_percentage_measurement_honors_min_max_and_box_sizing(void)
{
        static const char *box_sizing[] = { "content-box", "border-box" };
        static const float expected_width[] = { 200.0f, 178.0f };
        int i;

        for (i = 0; i < 2; ++i) {
                ui_widget_t *outer = create_measure_column();
                ui_widget_t *item = create_measure_column();
                ui_widget_t *text = create_measure_text();
                ui_widget_t *reference =
                    build_column_with_long_text(expected_width[i]);
                ui_widget_t *reference_text = ui_widget_get_child(reference, 1);

                ui_widget_set_style_string(outer, "width", "400px");
                ui_widget_set_style_string(item, "width", "100%");
                ui_widget_set_style_string(item, "max-width", "200px");
                ui_widget_set_style_string(item, "box-sizing", box_sizing[i]);
                ui_widget_set_style_string(item, "padding", "10px");
                ui_widget_set_style_string(item, "border", "1px solid #000000");
                ui_widget_append(item, text);
                ui_widget_append(outer, item);
                ui_widget_append(ui_root(), outer);
                ui_update();

                ctest_equal_float(
                    "percentage item should apply min max before box sizing",
                    item->content_box.width, expected_width[i]);
                ctest_equal_float(
                    "percentage cache should use the clamped content width",
                    item->measured_content_width, expected_width[i]);
                ctest_equal_float(
                    "percentage item should wrap at its resolved width",
                    text->content_box.height,
                    reference_text->content_box.height);
                ctest_equal_bool(
                    "percentage parent should contain all wrapped lines",
                    item->content_box.height + 0.5f >= text->border_box.height,
                    true);

                ui_widget_destroy(outer);
                ui_widget_destroy(reference);
        }
}

/* A flex-grow child's main size is allocated from remaining space and must
 * not diverge across repeated updates. */
static void test_grow_child_main_size_stable(void)
{
        ui_widget_t *root = ui_root();
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *grow = ui_create_widget(NULL);
        int i;

        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "height", "200px");
        ui_widget_set_style_string(grow, "flex", "1 1 auto");
        ui_widget_append(box, grow);
        ui_widget_append(root, box);
        ui_update();

        ctest_equal_bool("grow child fills the container main size",
                         fabsf(grow->border_box.height - 200.f) <= 2.f, true);
        for (i = 0; i < 3; ++i) {
                ui_update();
        }
        ctest_equal_bool("grow child main size stable across repeated updates",
                         fabsf(grow->border_box.height - 200.f) <= 2.f, true);

        ui_widget_destroy(box);
}

/* After a parent re-layout, the child must use the refreshed constraint
 * (new generation), never a stale one. */
static void test_constraint_refreshes_after_parent_relayout(void)
{
        ui_widget_t *box = build_column_with_long_text(300.f);
        ui_widget_t *desc = ui_widget_get_child(box, 1);
        float h300 = desc->content_box.height;
        float h200;

        ui_widget_set_style_string(box, "width", "120px");
        ui_update();
        h200 = desc->content_box.height;

        ctest_equal_bool(
            "narrower constraint re-wraps taller after parent relayout",
            h200 > h300 + 2.f, true);

        ui_widget_destroy(box);
}

void test_flex_layout(void)
{
        lcui_init();
        ui_widget_set_title(ui_root(), L"test flex layout");
        flex_layout_scene_build();

        ctest_describe("browser layout", test_browser_layout);
        ctest_describe("root width 1280px", test_flex_layout_1280);
        ctest_describe("root width 600px", test_flex_layout_600);
        ctest_describe("root width 320px", test_flex_layout_320);
        ctest_describe("align self", test_align_self);
        ctest_describe("flex gap", test_flex_layout_gap);
        ctest_describe("choice card", test_choice_card);
        ctest_describe("flex column text wrap",
                       test_description_wraps_within_container);
        ctest_describe("child reflow count",
                       test_child_reflow_count_at_most_once);
        ctest_describe("measure cache",
                       test_measure_cache_skips_repeated_reflow);
        ctest_describe("centered container reexpands",
                       test_centered_container_reexpands_after_widening);
        ctest_describe("max-width limits measurement",
                       test_max_width_limits_measurement);
        ctest_describe("measure cache invalidates on content change",
                       test_measure_cache_invalidates_on_content_change);
        ctest_describe("zero height text ignores stale cache",
                       test_zero_height_text_ignores_stale_cache);
        ctest_describe("standalone reflow stays wrapped",
                       test_standalone_reflow_stays_wrapped);
        ctest_describe("constraint api", test_constraint_api);
        ctest_describe("wrap container overflows",
                       test_wrap_container_overflows_without_clamp);
        ctest_describe("indefinite width single line",
                       test_indefinite_width_keeps_single_line);
        ctest_describe("fixed width child wrap",
                       test_fixed_width_child_wraps_own_width);
        ctest_describe("fixed height intrinsic restoration",
                       test_fixed_height_child_restores_intrinsic_size);
        ctest_describe("auto margin intrinsic height",
                       test_auto_margins_excluded_from_intrinsic_height);
        ctest_describe("measurement path width consistency",
                       test_measurement_paths_share_resolved_width);
        ctest_describe(
            "percentage measurement constraints",
            test_percentage_measurement_honors_min_max_and_box_sizing);
        ctest_describe("grow child main size",
                       test_grow_child_main_size_stable);
        ctest_describe("constraint refresh",
                       test_constraint_refreshes_after_parent_relayout);

        lcui_destroy();
}
