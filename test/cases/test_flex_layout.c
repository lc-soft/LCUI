#include "test.h"
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/ui.h>
#include <LCUI/gui/builder.h>

static void test_flex_layout_with_content_width(float width)
{
	float content_width = width - 22;

	ui_widget_t* w;
	ui_widget_t* content;
	ui_widget_t* example;
	ui_widget_t* container;
	LCUI_RectF rect;

	container = ui_widget_get_child(ui_get_widget("main"), 0);
	ui_update();

	rect.y = 10;
	rect.x = 10;
	rect.width = width;
	rect.height = 104;
	w = ui_widget_get_child(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 6);
	it_rectf("$('.example')[6].box.border", &w->box.border, &rect);

	rect.y += 114;
	rect.height = 344;
	w = ui_widget_get_child(container, 7);
	it_rectf("$('.example')[7].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 204;
	w = ui_widget_get_child(container, 8);
	it_rectf("$('.example')[8].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 9);
	it_rectf("$('.example')[9].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 10);
	it_rectf("$('.example')[10].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 11);
	it_rectf("$('.example')[11].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 12);
	it_rectf("$('.example')[12].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 13);
	it_rectf("$('.example')[13].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 14);
	it_rectf("$('.example')[14].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 185;
	w = ui_widget_get_child(container, 15);
	it_rectf("$('.example')[15].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 16);
	it_rectf("$('.example')[16].box.border", &w->box.border, &rect);

	// (1) justify-content: center;;

	example = ui_widget_get_child(container, 1);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 300) / 2.f;
	rect.y = 5;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(1) .box')[0].box.border", &w->box.border,
		 &rect);

	// (2) justify-content: flex-end;

	example = ui_widget_get_child(container, 2);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 300);
	it_rectf("$('.example:eq(2) .box')[0].box.border", &w->box.border,
		 &rect);

	// (3) justify-content: space-between;

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	it_rectf("$('.example:eq(3) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x = 5 + 50 + (content_width - 300) / 5.f;
	it_rectf("$('.example:eq(3) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = 5 + (content_width - 50);
	it_rectf("$('.example:eq(3) .box')[5].box.border", &w->box.border,
		 &rect);

	// (4) justify-content: space-around;

	example = ui_widget_get_child(container, 4);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 300) / 6.f / 2.f;
	it_rectf("$('.example:eq(4) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 300) / 6.f;
	it_rectf("$('.example:eq(4) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = 5 + content_width - (content_width - 300) / 6.f / 2.f - 50;
	it_rectf("$('.example:eq(4) .box')[5].box.border", &w->box.border,
		 &rect);

	// (5) Auto margin

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	it_rectf("$('.example:eq(5) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	it_rectf("$('.example:eq(5) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	it_rectf("$('.example:eq(5) .box')[2].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	it_rectf("$('.example:eq(5) .box')[3].box.border", &w->box.border,
		 &rect);

	// (7) flex-grow

	example = ui_widget_get_child(container, 7);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = content_width / 3.f;
	it_rectf("$('.example:eq(7) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(7) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(7) .box')[2].box.border", &w->box.border,
		 &rect);

	// line 1

	content = ui_widget_get_child(example, 2);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = content_width - 50 - 50;
	it_rectf("$('.example:eq(7) .box')[3].box.border", &w->box.border,
		 &rect);

	// line 2

	content = ui_widget_get_child(example, 3);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = (content_width - 50) / 2.f;
	it_rectf("$('.example:eq(7) .box')[6].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(7) .box')[7].box.border", &w->box.border,
		 &rect);

	// line 3

	content = ui_widget_get_child(example, 4);
	w = ui_widget_get_child(content, 1);
	rect.x = 5 + 50;
	it_rectf("$('.example:eq(7) .box')[10].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(7) .box')[11].box.border", &w->box.border,
		 &rect);

	// line 4

	content = ui_widget_get_child(example, 5);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = 50 + (content_width - 3 * 50) * 4.f / 7.f;
	it_rectf("$('.example:eq(7) .box')[12].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 50 + (content_width - 3 * 50) * 2.f / 7.f;
	it_rectf("$('.example:eq(7) .box')[13].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 50 + (content_width - 3 * 50) / 7.f;
	it_rectf("$('.example:eq(7) .box')[14].box.border", &w->box.border,
		 &rect);

	// (10) align-items: center; height: 300px;

	example = ui_widget_get_child(container, 10);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5 + (300 / 3.f - 50) / 2.f;
	rect.width = 50;
	it_rectf("$('.example:eq(10) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	it_rectf("$('.example:eq(10) .box')[1].box.border", &w->box.border,
		 &rect);

	// (11) align-items: end; height: 300px;

	example = ui_widget_get_child(container, 11);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5 + (300 / 3.f - 50);
	rect.width = 50;
	it_rectf("$('.example:eq(11) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	it_rectf("$('.example:eq(11) .box')[1].box.border", &w->box.border,
		 &rect);

	// (12) align-items: stretch; height: 300px;

	example = ui_widget_get_child(container, 12);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = 50;
	rect.height = 100;
	it_rectf("$('.example:eq(12) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	it_rectf("$('.example:eq(12) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	w = ui_widget_next(w);
	rect.x = 5 + 3 * 50;
	rect.height = 50;
	it_rectf("$('.example:eq(12) .box')[3].box.border", &w->box.border,
		 &rect);

	// (13) Vertically Centered Layout

	example = ui_widget_get_child(container, 13);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5 + (content_width - 50) / 2.f;
	rect.y = 5 + (300 - 50) / 2.f;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(13) .box')[0].box.border", &w->box.border,
		 &rect);

	// (14) flex-direction: column;

	example = ui_widget_get_child(container, 14);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(14) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 4);
	rect.x = 5 + 50 + (content_width - 50 - 150 - 200 - 50 - 50) / 5.f;
	rect.width = 150;
	it_rectf("$('.example:eq(14) .box')[4].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 8);
	rect.x += 150 + (content_width - 50 - 150 - 200 - 50 - 50) / 5.f;
	rect.width = 50;
	it_rectf("$('.example:eq(14) .box')[8].box.border", &w->box.border,
		 &rect);

	// (15) Holy Grail Layout

	example = ui_widget_get_child(container, 15);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = content_width;
	rect.height = 50;
	it_rectf("$('.example:eq(15) .layout__header')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 31;
	it_rectf("$('.example:eq(15) .layout__footer')[0].box.border",
		 &w->box.border, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 31;
	w = ui_widget_get_child(content, 1);
	w = ui_widget_get_child(w, 0);
	it_rectf("$('.example:eq(15) .layout__left')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x = 100;
	rect.width = content_width - 100 - 100;
	it_rectf("$('.example:eq(15) .layout__center')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 100;
	it_rectf("$('.example:eq(15) .layout__right')[0].box.border",
		 &w->box.border, &rect);

	// (16) Holy Grail Layout (height: 300px)

	example = ui_widget_get_child(container, 16);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = content_width;
	rect.height = 50;
	it_rectf("$('.example:eq(16) .layout__header')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 200;
	it_rectf("$('.example:eq(16) .layout__footer')[0].box.border",
		 &w->box.border, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 200;
	w = ui_widget_get_child(content, 1);
	w = ui_widget_get_child(w, 0);
	it_rectf("$('.example:eq(16) .layout__left')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x = 100;
	rect.width = content_width - 100 - 100;
	it_rectf("$('.example:eq(16) .layout__center')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 100;
	it_rectf("$('.example:eq(16) .layout__right')[0].box.border",
		 &w->box.border, &rect);
}

static void test_browser_layout(void)
{
	ui_widget_t* w;
	LCUI_RectF rect;

	w = ui_get_widget("browser-tabbar");
	w = ui_widget_get_child(w, 0);
	it_i("$('#browser-tabbar .c-frame-tab')[0].width", (int)w->width, 101);
	w = ui_widget_next(w);
	it_i("$('#browser-tabbar .c-frame-tab')[1].width", (int)w->width, 101);
	w = ui_widget_next(w);
	it_i("$('#browser-tabbar .c-frame-tab')[2].width", (int)w->width, 101);
	w = ui_widget_next(w);
	it_i("$('#browser-tabbar .c-frame-tab')[3].width", (int)w->width, 101);
	w = ui_widget_next(w);
	it_i("$('#browser-tabbar .c-frame-tab')[4].width", (int)w->width, 101);

	w = ui_get_widget("browser-frame-client");
	it_i("$('#browser-frame-client')[0].height", (int)w->height, 223);
	w = ui_get_widget("browser-frame-content");
	it_i("$('#browser-frame-content')[0].height", (int)w->height, 223);
	w = ui_get_widget("browser-page-home");
	it_i("$('#browser-page-home')[0].height", (int)w->height, 223);
	w = ui_get_widget("browser-page-home-container");
	rect.width = 256;
	rect.height = 70;
	rect.x = (538.f - rect.width) / 2.f + w->parent->padding.left;
	rect.y = (203.f - rect.height) / 2.f + w->parent->padding.top;
	it_rectf("$('#browser-page-home-container')[0].box.border",
		 &w->box.border, &rect);
}

static void test_flex_layout_1280(void)
{
	LCUIDisplay_SetSize(1280, 800);
	test_flex_layout_with_content_width(580);
}

static void test_flex_layout_600(void)
{
	LCUIDisplay_SetSize(600, 400);
	test_flex_layout_with_content_width(580 - 14);
}

static void test_flex_layout_320(void)
{
	float width = 320 - 10 - 10 - 14;
	float content_width = width - 12 - 10;

	ui_widget_t* w;
	ui_widget_t* content;
	ui_widget_t* example;
	ui_widget_t* container;
	LCUI_RectF rect;

	container = ui_widget_get_child(ui_get_widget("main"), 0);
	LCUIDisplay_SetSize(320, 240);
	ui_update();

	rect.y = 10;
	rect.x = 10;
	rect.width = width;
	rect.height = 104;
	w = ui_widget_get_child(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	rect.y += 114;
	w = ui_widget_get_child(container, 6);
	it_rectf("$('.example')[6].box.border", &w->box.border, &rect);

	rect.y += 114;
	rect.height = 344;
	w = ui_widget_get_child(container, 7);
	it_rectf("$('.example')[7].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 8);
	it_rectf("$('.example')[8].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 9);
	it_rectf("$('.example')[9].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 10);
	it_rectf("$('.example')[10].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 11);
	it_rectf("$('.example')[11].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 12);
	it_rectf("$('.example')[12].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 13);
	it_rectf("$('.example')[13].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 14);
	it_rectf("$('.example')[14].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 185;
	w = ui_widget_get_child(container, 15);
	it_rectf("$('.example')[15].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 16);
	it_rectf("$('.example')[16].box.border", &w->box.border, &rect);

	// (1) justify-content: center;;

	example = ui_widget_get_child(container, 1);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = 50 + (content_width - 300) / 6.f;
	rect.height = 50;
	it_rectf("$('.example:eq(1) .box')[0].box.border", &w->box.border,
		 &rect);

	// (2) justify-content: flex-end;

	example = ui_widget_get_child(container, 2);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	it_rectf("$('.example:eq(2) .box')[0].box.border", &w->box.border,
		 &rect);

	// (3) justify-content: space-between;

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	it_rectf("$('.example:eq(3) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(3) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = 5 + content_width - rect.width;
	it_rectf("$('.example:eq(3) .box')[5].box.border", &w->box.border,
		 &rect);

	// (5) Auto margin

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.width = 50;
	it_rectf("$('.example:eq(5) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	it_rectf("$('.example:eq(5) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50;
	it_rectf("$('.example:eq(5) .box')[2].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += 50 + (content_width - 200) / 2.f;
	it_rectf("$('.example:eq(5) .box')[3].box.border", &w->box.border,
		 &rect);

	// (15) Holy Grail Layout

	example = ui_widget_get_child(container, 15);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = content_width;
	rect.height = 50;
	it_rectf("$('.example:eq(15) .layout__header')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 31;
	it_rectf("$('.example:eq(15) .layout__footer')[0].box.border",
		 &w->box.border, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 31;
	w = ui_widget_get_child(content, 1);
	w = ui_widget_get_child(w, 0);
	it_rectf("$('.example:eq(15) .layout__left')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x = 100;
	rect.width = content_width - 100 - 100;
	it_rectf("$('.example:eq(15) .layout__center')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 100;
	it_rectf("$('.example:eq(15) .layout__right')[0].box.border",
		 &w->box.border, &rect);

	// (16) Holy Grail Layout (height: 300px)

	example = ui_widget_get_child(container, 16);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = 5;
	rect.y = 5;
	rect.width = content_width;
	rect.height = 50;
	it_rectf("$('.example:eq(15) .layout__header')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_get_child(content, 2);
	rect.y = 5 + 50 + 200;
	it_rectf("$('.example:eq(16) .layout__footer')[0].box.border",
		 &w->box.border, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 200;
	w = ui_widget_get_child(content, 1);
	w = ui_widget_get_child(w, 0);
	it_rectf("$('.example:eq(16) .layout__left')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x = 100;
	rect.width = content_width - 100 - 100;
	it_rectf("$('.example:eq(16) .layout__center')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	rect.width = 100;
	it_rectf("$('.example:eq(16) .layout__right')[0].box.border",
		 &w->box.border, &rect);
}

void test_flex_layout(void)
{
	ui_widget_t* root;
	ui_widget_t* wrapper;

	LCUI_Init();
	wrapper = LCUIBuilder_LoadFile("test_flex_layout.xml");
	root = ui_root();
	ui_widget_set_title(root, L"test flex layout");
	ui_widget_append(root, wrapper);
	ui_widget_unwrap(wrapper);
	ui_update();

	describe("browser layout", test_browser_layout);
	describe("root width 1280px", test_flex_layout_1280);
	describe("root width 600px", test_flex_layout_600);
	describe("root width 320px", test_flex_layout_320);

#ifndef PREVIEW_MODE
	LCUI_Destroy();
#endif
}
