/*
 * tests/cases/test_flex_layout.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

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
	ctest_equal_ui_rect("$('.example')[0].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 1);
	ctest_equal_ui_rect("$('.example')[1].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 2);
	ctest_equal_ui_rect("$('.example')[2].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 3);
	ctest_equal_ui_rect("$('.example')[3].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 4);
	ctest_equal_ui_rect("$('.example')[4].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 5);
	ctest_equal_ui_rect("$('.example')[5].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 6);
	ctest_equal_ui_rect("$('.example')[6].border_box", &w->border_box, &rect);

	rect.y += 114;
	rect.height = 344;
	w = ui_widget_get_child(container, 7);
	ctest_equal_ui_rect("$('.example')[7].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 204;
	w = ui_widget_get_child(container, 8);
	ctest_equal_ui_rect("$('.example')[8].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 9);
	ctest_equal_ui_rect("$('.example')[9].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 10);
	ctest_equal_ui_rect("$('.example')[10].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 11);
	ctest_equal_ui_rect("$('.example')[11].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 12);
	ctest_equal_ui_rect("$('.example')[12].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 13);
	ctest_equal_ui_rect("$('.example')[13].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 14);
	ctest_equal_ui_rect("$('.example')[14].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 185;
	w = ui_widget_get_child(container, 15);
	ctest_equal_ui_rect("$('.example')[15].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 16);
	ctest_equal_ui_rect("$('.example')[16].border_box", &w->border_box, &rect);

	// (1) justify-content: center;;

	example = ui_widget_get_child(container, 1);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 300) / 2.f;
	rect.y = 5;
	rect.width = 50;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(1) .box')[0].border_box", &w->border_box,
		 &rect);

	// (2) justify-content: flex-end;

	example = ui_widget_get_child(container, 2);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 300);
	ctest_equal_ui_rect("$('.example:eq(2) .box')[0].border_box", &w->border_box,
		 &rect);

	// (3) justify-content: space-between;

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	ctest_equal_ui_rect("$('.example:eq(3) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x = 5 + 50 + (content_width - 300) / 5.f;
	ctest_equal_ui_rect("$('.example:eq(3) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = 5 + (content_width - 50);
	ctest_equal_ui_rect("$('.example:eq(3) .box')[5].border_box", &w->border_box,
		 &rect);

	// (4) justify-content: space-around;

	example = ui_widget_get_child(container, 4);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 300) / 6.f / 2.f;
	ctest_equal_ui_rect("$('.example:eq(4) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 300) / 6.f;
	ctest_equal_ui_rect("$('.example:eq(4) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = 5 + content_width - (content_width - 300) / 6.f / 2.f - 50;
	ctest_equal_ui_rect("$('.example:eq(4) .box')[5].border_box", &w->border_box,
		 &rect);

	// (5) Auto margin

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[2].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[3].border_box", &w->border_box,
		 &rect);

	// (7) flex-grow

	example = ui_widget_get_child(container, 7);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = content_width / 3.f;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[2].border_box", &w->border_box,
		 &rect);

	// line 1

	content = ui_widget_get_child(example, 2);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = content_width - 50 - 50;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[3].border_box", &w->border_box,
		 &rect);

	// line 2

	content = ui_widget_get_child(example, 3);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = (content_width - 50) / 2.f;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[6].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[7].border_box", &w->border_box,
		 &rect);

	// line 3

	content = ui_widget_get_child(example, 4);
	w = ui_widget_get_child(content, 1);
	rect.x = 5 + 50;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[10].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[11].border_box", &w->border_box,
		 &rect);

	// line 4

	content = ui_widget_get_child(example, 5);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = 12 + (content_width - 3 * 12) * 4.f / 7.f;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[12].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 12 + (content_width - 3 * 12) * 2.f / 7.f;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[13].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 12 + (content_width - 3 * 12) / 7.f;
	ctest_equal_ui_rect("$('.example:eq(7) .box')[14].border_box", &w->border_box,
		 &rect);

	// (10) align-items: center; height: 300px;

	example = ui_widget_get_child(container, 10);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5 + (300 / 3.f - 50) / 2.f;
	rect.width = 50;
	ctest_equal_ui_rect("$('.example:eq(10) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	ctest_equal_ui_rect("$('.example:eq(10) .box')[1].border_box", &w->border_box,
		 &rect);

	// (11) align-items: end; height: 300px;

	example = ui_widget_get_child(container, 11);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5 + (300 / 3.f - 50);
	rect.width = 50;
	ctest_equal_ui_rect("$('.example:eq(11) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	ctest_equal_ui_rect("$('.example:eq(11) .box')[1].border_box", &w->border_box,
		 &rect);

	// (12) align-items: stretch; height: 300px;

	example = ui_widget_get_child(container, 12);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = 50;
	rect.height = 100;
	ctest_equal_ui_rect("$('.example:eq(12) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	ctest_equal_ui_rect("$('.example:eq(12) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	w = ui_widget_next(w);
	rect.x = 5 + 3 * 50;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(12) .box')[3].border_box", &w->border_box,
		 &rect);

	// (13) Vertically Centered Layout

	example = ui_widget_get_child(container, 13);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 50) / 2.f;
	rect.y = 5 + (300 - 50) / 2.f;
	rect.width = 50;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(13) .box')[0].border_box", &w->border_box,
		 &rect);

	// (14) flex-direction: column;

	example = ui_widget_get_child(container, 14);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = 50;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(14) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_get_child(content, 4);
	rect.x = 5 + 50 + (content_width - 50 - 150 - 200 - 50 - 50) / 5.f;
	rect.width = 150;
	ctest_equal_ui_rect("$('.example:eq(14) .box')[4].border_box", &w->border_box,
		 &rect);

	w = ui_widget_get_child(content, 8);
	rect.x += 150 + (content_width - 50 - 150 - 200 - 50 - 50) / 5.f;
	rect.width = 50;
	ctest_equal_ui_rect("$('.example:eq(14) .box')[8].border_box", &w->border_box,
		 &rect);

	// (15) Holy Grail Layout

	example = ui_widget_get_child(container, 15);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = content_width;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(15) .layout__header')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 31;
	ctest_equal_ui_rect("$('.example:eq(15) .layout__footer')[0].border_box",
		 &w->border_box, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 31;
	w = ui_widget_get_child(content, 1);
	w = ui_widget_get_child(w, 0);
	ctest_equal_ui_rect("$('.example:eq(15) .layout__left')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_next(w);
	rect.x = 100;
	rect.width = content_width - 100 - 100;
	ctest_equal_ui_rect("$('.example:eq(15) .layout__center')[0].border_box",
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
	ctest_equal_ui_rect("$('.example:eq(16) .layout__header')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 200;
	ctest_equal_ui_rect("$('.example:eq(16) .layout__footer')[0].border_box",
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
	ctest_equal_ui_rect("$('.example:eq(16) .layout__center')[0].border_box",
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
	ctest_equal_int("$('#browser-frame-client')[0].height", (int)w->border_box.height,
	     223);
	w = ui_get_widget("browser-frame-content");
	ctest_equal_int("$('#browser-frame-content')[0].height", (int)w->border_box.height,
	     223);
	w = ui_get_widget("browser-page-home");
	ctest_equal_int("$('#browser-page-home')[0].height", (int)w->border_box.height,
	     223);
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
	test_flex_layout_with_content_width(580 - 14);
}

static void test_flex_layout_320(void)
{
	float width = 320 - 10 - 10 - 14;
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
	ctest_equal_ui_rect("$('.example')[0].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 1);
	ctest_equal_ui_rect("$('.example')[1].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 2);
	ctest_equal_ui_rect("$('.example')[2].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 3);
	ctest_equal_ui_rect("$('.example')[3].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 4);
	ctest_equal_ui_rect("$('.example')[4].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 5);
	ctest_equal_ui_rect("$('.example')[5].border_box", &w->border_box, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 6);
	ctest_equal_ui_rect("$('.example')[6].border_box", &w->border_box, &rect);

	rect.y += 114;
	rect.height = 344;
	w = ui_widget_get_child(container, 7);
	ctest_equal_ui_rect("$('.example')[7].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 8);
	ctest_equal_ui_rect("$('.example')[8].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 9);
	ctest_equal_ui_rect("$('.example')[9].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 10);
	ctest_equal_ui_rect("$('.example')[10].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 11);
	ctest_equal_ui_rect("$('.example')[11].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 12);
	ctest_equal_ui_rect("$('.example')[12].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 13);
	ctest_equal_ui_rect("$('.example')[13].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 14);
	ctest_equal_ui_rect("$('.example')[14].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 185;
	w = ui_widget_get_child(container, 15);
	ctest_equal_ui_rect("$('.example')[15].border_box", &w->border_box, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 16);
	ctest_equal_ui_rect("$('.example')[16].border_box", &w->border_box, &rect);

	// (1) justify-content: center;;

	example = ui_widget_get_child(container, 1);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = 50 + (content_width - 300) / 6.f;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(1) .box')[0].border_box", &w->border_box,
		 &rect);

	// (2) justify-content: flex-end;

	example = ui_widget_get_child(container, 2);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	ctest_equal_ui_rect("$('.example:eq(2) .box')[0].border_box", &w->border_box,
		 &rect);

	// (3) justify-content: space-between;

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	ctest_equal_ui_rect("$('.example:eq(3) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	ctest_equal_ui_rect("$('.example:eq(3) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = 5 + content_width - rect.width;
	ctest_equal_ui_rect("$('.example:eq(3) .box')[5].border_box", &w->border_box,
		 &rect);

	// (5) Auto margin

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = 50;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[0].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[1].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[2].border_box", &w->border_box,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	ctest_equal_ui_rect("$('.example:eq(5) .box')[3].border_box", &w->border_box,
		 &rect);

	// (15) Holy Grail Layout

	example = ui_widget_get_child(container, 15);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = content_width;
	rect.height = 50;
	ctest_equal_ui_rect("$('.example:eq(15) .layout__header')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 31;
	ctest_equal_ui_rect("$('.example:eq(15) .layout__footer')[0].border_box",
		 &w->border_box, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 31;
	w = ui_widget_get_child(content, 1);
	w = ui_widget_get_child(w, 0);
	ctest_equal_ui_rect("$('.example:eq(15) .layout__left')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_next(w);
	rect.x = 100;
	rect.width = content_width - 100 - 100;
	ctest_equal_ui_rect("$('.example:eq(15) .layout__center')[0].border_box",
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
	ctest_equal_ui_rect("$('.example:eq(15) .layout__header')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 200;
	ctest_equal_ui_rect("$('.example:eq(16) .layout__footer')[0].border_box",
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
	ctest_equal_ui_rect("$('.example:eq(16) .layout__center')[0].border_box",
		 &w->border_box, &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 100;
	ctest_equal_ui_rect("$('.example:eq(16) .layout__right')[0].border_box",
		 &w->border_box, &rect);
}

void test_flex_layout(void)
{
	ui_widget_t *root;
	ui_widget_t *wrapper;

	lcui_init();
	wrapper = ui_load_xml_file("test_flex_layout.xml");
	root = ui_root();
	ui_widget_set_title(root, L"test flex layout");
	ui_widget_append(root, wrapper);
	ui_widget_unwrap(wrapper);
	ui_update();

	ctest_describe("browser layout", test_browser_layout);
	ctest_describe("root width 1280px", test_flex_layout_1280);
	ctest_describe("root width 600px", test_flex_layout_600);
	ctest_describe("root width 320px", test_flex_layout_320);

#ifndef PREVIEW_MODE
	lcui_quit();
	lcui_main();
#endif
}
