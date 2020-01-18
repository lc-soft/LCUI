#include "libtest.h"
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/widget/textview.h>

static void test_dropdown(void)
{
	LCUI_Widget w;
	int width, height;

	w = LCUIWidget_GetById("test-dropdown-menu");
	it_b("$('#test-dropdown-menu')[0].box.border.height < 200",
	     w->width < 200, TRUE);
	it_b("$('#test-dropdown-menu')[0].box.border.width > 100",
	     w->width > 100, TRUE);
	it_i("$('#test-dropdown-menu')[0].box.border.height", (int)w->height,
	     142);

	w = Widget_GetChild(w, 0);
	width = (int)w->width;
	height = (int)w->height;

	w = Widget_GetNext(w);
	it_i("$('#test-dropdown-menu .dropdown-item')[1].box.border.width",
	     (int)w->width, width);
	it_i("$('#test-dropdown-menu .dropdown-item')[1].box.border.height",
	     (int)w->height, height);

	w = Widget_GetNext(w);
	it_i("$('#test-dropdown-menu .dropdown-item')[2].box.border.width",
	     (int)w->width, width);
	it_i("$('#test-dropdown-menu .dropdown-item')[2].box.border.height",
	     (int)w->height, height);

	w = Widget_GetNext(w);
	it_i("$('#test-dropdown-menu .dropdown-item')[3].box.border.width",
	     (int)w->width, width);
	it_i("$('#test-dropdown-menu .dropdown-item')[3].box.border.height",
	     (int)w->height, height);
}

static void test_auto_size(void)
{
	LCUI_Widget w;
	LCUI_Widget example;
	LCUI_RectF rect;

	// Update #test-text-auto-height content

	w = LCUIWidget_GetById("test-text-auto-height");
	example = w->parent->parent;
	TextView_SetTextW(w, L"long long long long long long text");
	LCUIWidget_Update();
	rect.x = 5;
	rect.y = 5;
	rect.width = 200;
	rect.height = 50;
	it_rectf("$('#test-text-auto-height')[0].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	it_i("$('#test-text-auto-size')[0].box.border.y", (int)w->y, 5 + 50);

	w = Widget_GetNext(w);
	it_i("$('#test-text-block-auto-height')[0].box.border.y", (int)w->y,
	     5 + 50 + 31);

	it_i("$('.example')[2].box.border.height", (int)example->height, 166);

	// Update #test-text-auto-size content

	w = LCUIWidget_GetById("test-text-auto-size");
	TextView_SetTextW(
	    w, L"long long long long long long long long long long long long "
	       L"long long long long long long long long long long long long "
	       L"long long text");
	LCUIWidget_Update();
	it_i("$('#test-text-auto-size')[0].box.border.x", (int)w->box.border.x,
	     5);
	it_i("$('#test-text-auto-size')[0].box.border.y", (int)w->box.border.y,
	     5 + 50);
	it_b("$('#test-text-auto-size')[0].box.border.width <= 758",
	     w->box.border.width < 758.f, TRUE);
	it_i("$('#test-text-auto-size')[0].box.border.height",
	     (int)w->box.border.height, 50);

	w = Widget_GetNext(w);
	it_i("$('#test-text-block-auto-height')[0].box.border.y", (int)w->y,
	     5 + 50 + 50);

	it_i("$('.example')[2].box.border.height", (int)example->height, 185);

	// Update test-text-block-auto-height content

	w = LCUIWidget_GetById("test-text-block-auto-height");
	TextView_SetTextW(
	    w, L"long long long long long long long long long long long long "
	       L"long long long long long long long long long long long long "
	       L"long long text");
	LCUIWidget_Update();
	rect.x = 5;
	rect.y = 5 + 50 + 50;
	rect.width = 758;
	rect.height = 50;
	it_rectf("$('#test-text-block-auto-height')[0].box.border",
		 &w->box.border, &rect);
}

static void test_block_layout_1280(void)
{
	LCUI_Widget w;
	LCUI_Widget content;
	LCUI_Widget example;
	LCUI_Widget container;
	LCUI_RectF rect;

	container = Widget_GetChild(LCUIWidget_GetById("main"), 0);
	LCUIDisplay_SetSize(1280, 800);
	LCUIWidget_Update();

	rect.x = 10;
	rect.y = 10;
	rect.width = 780;
	rect.height = 404;
	w = Widget_GetChild(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.width = 780;
	rect.height = 304;
	w = Widget_GetChild(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.width = 780;
	rect.height = 147;
	w = Widget_GetChild(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.width = 780;
	rect.height = 550;
	w = Widget_GetChild(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.width = 780;
	rect.height = 354;
	w = Widget_GetChild(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.width = 780;
	rect.height = 254;
	w = Widget_GetChild(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	example = Widget_GetChild(container, 0);
	content = Widget_GetChild(example, 1);
	w = Widget_GetChild(content, 13);
	rect.width = 150;
	rect.height = 50;
	rect.x = content->padding.left;
	rect.y = 150 + content->padding.top;
	it_rectf("$('.example:eq(0) .box')[13].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x = content->box.content.width - 150.f;
	rect.x += content->padding.left;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box.ml-auto')[0].box.border",
		 &w->box.border, &rect);

	w = Widget_GetNext(w);
	rect.x = (content->box.content.width - 150.f) / 2.f;
	rect.x += content->padding.left;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box.ml-auto.mr-auto')[0].box.border",
		 &w->box.border, &rect);

	example = Widget_GetChild(container, 1);
	content = Widget_GetChild(example, 1);
	w = Widget_GetChild(content, 0);
	rect.x = content->padding.left;
	rect.y = content->padding.top;
	rect.width = content->box.content.width * 0.5f;
	rect.height = 50;
	it_rectf("$('.example:eq(1) .box')[0].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	it_rectf("$('.example:eq(1) .box')[1].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = content->box.content.width;
	it_rectf("$('.example:eq(1) .box')[2].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = content->box.content.width * 0.5f;
	it_rectf("$('.example:eq(1) .box')[3].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[4].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.x = content->padding.left;
	rect.width = content->box.content.width * 0.25f;
	it_rectf("$('.example:eq(1) .box')[5].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[6].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[7].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[8].box.border", &w->box.border,
		 &rect);

	example = Widget_GetChild(container, 3);
	content = Widget_GetChild(Widget_GetChild(example, 1), 0);
	w = Widget_GetChild(content, 0);
	rect.x = content->padding.left + w->margin.left;
	rect.y = content->padding.top + w->margin.top;
	rect.width = content->box.content.width;
	rect.width -= w->margin.left + w->margin.right;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[0].box.border", &w->box.border,
		 &rect);

	w = Widget_GetChild(content, 5);
	rect.x = content->padding.left + 264.f;
	rect.y = content->padding.top + rect.height + 32;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[5].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width - 8;
	rect.y += 8;
	rect.width = 150;
	it_rectf("$('.example:eq(3) .box')[6].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width + w->margin.left + 8;
	rect.y -= 8;
	rect.width = 100;
	it_rectf("$('.example:eq(3 .box')[7].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x = content->padding.left - 16;
	rect.y = content->padding.top + (16 + 50 + 16) + (8 + 50 + 8) - 16;
	rect.width = content->box.content.width + 32;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[8].box.border", &w->box.border,
		 &rect);

	content = Widget_GetChild(Widget_GetChild(example, 1), 1);
	w = Widget_GetChild(content, 0);
	rect.x = content->padding.left;
	rect.y = content->padding.top;
	rect.width = content->box.content.width;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[9].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += w->margin.left;
	rect.y += 50;
	rect.width -= w->margin.left;
	it_rectf("$('.example:eq(3) .box')[10].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = content->box.content.width + 16;
	it_rectf("$('.example:eq(3) .box.ml--1.mr--1')[0].box.border",
		 &w->box.border, &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = 200;
	it_rectf("$('.example:eq(3) .box.ml--1.mr--1')[1].box.border",
		 &w->box.border, &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = content->box.content.width;
	it_rectf("$('.example:eq(3) .box.ml--1.mr--1')[2].box.border",
		 &w->box.border, &rect);

	example = Widget_GetChild(container, 5);
	content = Widget_GetChild(example, 1);

	w = LCUIWidget_GetById("box-absolute-top-left");
	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 100;
	it_rectf("$('#box-absolute-top-left')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-top-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-top-right')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-center");
	rect.x = (content->box.padding.width - rect.width) * 0.5f;
	rect.y = (content->box.padding.height - rect.height) * 0.5f;
	it_rectf("$('#box-absolute-center')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-bottom-left");
	rect.x = 0;
	rect.y = content->box.padding.height - rect.height;
	it_rectf("$('#box-absolute-bottom-left')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-bottom-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-bottom-right')[0].box.border",
		 &w->box.border, &rect);

	// (2) Auto size

	w = LCUIWidget_GetById("test-text-auto-height");
	rect.x = 5;
	rect.y = 5;
	rect.width = 200;
	rect.height = 31;
	it_rectf("$('#test-text-auto-height')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("test-text-auto-size");
	it_b("$('#test-text-auto-size')[0].width < 140px", w->width < 170,
	     TRUE);
	it_i("$('#test-text-auto-size')[0].height", (int)w->height, 31);

	w = LCUIWidget_GetById("test-text-block-auto-height");
	it_i("$('#test-text-block-auto-height')[0].width", (int)w->width, 758);
	it_i("$('#test-text-block-auto-height')[0].height", (int)w->height, 31);

	describe("test auto size", test_auto_size);
	describe("test dropdown", test_dropdown);
}

static void test_block_layout_600(void)
{
	LCUI_Widget w;
	LCUI_Widget content;
	LCUI_Widget example;
	LCUI_Widget container;
	LCUI_RectF rect;

	container = Widget_GetChild(LCUIWidget_GetById("main"), 0);
	LCUIDisplay_SetSize(600, 400);
	LCUIWidget_Update();

	rect.x = 10;
	rect.y = 10;
	rect.width = 600 - 20 - 14;
	rect.height = 504;
	w = Widget_GetChild(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 304;
	w = Widget_GetChild(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 204;
	w = Widget_GetChild(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 600;
	w = Widget_GetChild(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = Widget_GetChild(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = Widget_GetChild(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	example = Widget_GetChild(container, 0);
	content = Widget_GetChild(example, 1);
	w = Widget_GetChild(content, 11);
	rect.width = 50;
	rect.height = 50;
	rect.x = content->padding.left;
	rect.y = 100 + content->padding.top;
	it_rectf("$('.example:eq(0) .box')[11].box.border", &w->box.border,
		 &rect);

	w = Widget_GetChild(content, 14);
	rect.width = 150;
	rect.x = content->padding.left;
	rect.x += content->box.content.width - rect.width;
	rect.y = 250 + content->padding.top;
	it_rectf("$('.example:eq(0) .box')[14].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x = content->padding.left;
	rect.x += (content->box.content.width - rect.width) / 2.f;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box')[15].box.border", &w->box.border,
		 &rect);

	w = Widget_GetChild(content, 26);
	rect.x = content->padding.left;
	rect.y = content->padding.top + 400;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(0) .box')[26].box.border", &w->box.border,
		 &rect);

	example = Widget_GetChild(container, 1);
	content = Widget_GetChild(example, 1);
	w = Widget_GetChild(content, 0);
	rect.x = content->padding.left;
	rect.y = content->padding.top;
	rect.width = content->box.content.width * 0.5f;
	rect.height = 50;
	it_rectf("$('.example:eq(1) .box')[0].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	it_rectf("$('.example:eq(1) .box')[1].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = content->box.content.width;
	it_rectf("$('.example:eq(1) .box')[2].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.width = content->box.content.width * 0.5f;
	it_rectf("$('.example:eq(1) .box')[3].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[4].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.y += 50;
	rect.x = content->padding.left;
	rect.width = content->box.content.width * 0.25f;
	it_rectf("$('.example:eq(1) .box')[5].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[6].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[7].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[8].box.border", &w->box.border,
		 &rect);

	example = Widget_GetChild(container, 3);
	content = Widget_GetChild(Widget_GetChild(example, 1), 0);
	w = Widget_GetChild(content, 7);

	rect.x = content->padding.left - 16;
	rect.y = content->padding.top + (16 + 50 + 16) + (8 + 50 + 8);
	rect.width = 100;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[7].box.border", &w->box.border,
		 &rect);

	content = Widget_GetChild(Widget_GetChild(example, 1), 1);
	w = Widget_GetChild(content, 4);
	rect.x = content->padding.left - 8;
	rect.y = content->padding.top + 200;
	rect.width = content->box.content.width;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[13].box.border", &w->box.border,
		 &rect);

	example = Widget_GetChild(container, 5);
	content = Widget_GetChild(example, 1);

	w = LCUIWidget_GetById("box-absolute-top-left");
	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 100;
	it_rectf("$('#box-absolute-top-left')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-top-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-top-right')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-center");
	rect.x = (content->box.padding.width - rect.width) * 0.5f;
	rect.y = (content->box.padding.height - rect.height) * 0.5f;
	it_rectf("$('#box-absolute-center')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-bottom-left");
	rect.x = 0;
	rect.y = content->box.padding.height - rect.height;
	it_rectf("$('#box-absolute-bottom-left')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-bottom-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-bottom-right')[0].box.border",
		 &w->box.border, &rect);
}

static void test_block_layout_320(void)
{
	LCUI_Widget w;
	LCUI_Widget content;
	LCUI_Widget example;
	LCUI_Widget container;
	LCUI_RectF rect;

	container = Widget_GetChild(LCUIWidget_GetById("main"), 0);
	LCUIDisplay_SetSize(320, 240);
	LCUIWidget_Update();

	rect.x = 10;
	rect.y = 10;
	rect.width = 320 - 20 - 14;
	rect.height = 604;
	w = Widget_GetChild(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 304;
	w = Widget_GetChild(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 280;
	w = Widget_GetChild(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 732;
	w = Widget_GetChild(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 504;
	w = Widget_GetChild(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = Widget_GetChild(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	example = Widget_GetChild(container, 0);
	content = Widget_GetChild(example, 1);

	w = Widget_GetChild(content, 11);
	rect.x = content->padding.left;
	rect.y = content->padding.top + 150;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(0) .box')[11].box.border", &w->box.border,
		 &rect);

	w = Widget_GetChild(content, 14);
	rect.width = 150;
	rect.x = content->padding.left;
	rect.x += content->box.content.width - rect.width;
	rect.y = content->padding.top + 300;
	it_rectf("$('.example:eq(0) .box')[14].box.border", &w->box.border,
		 &rect);

	w = Widget_GetNext(w);
	rect.x = content->padding.left;
	rect.x += (content->box.content.width - rect.width) / 2.f;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box')[15].box.border", &w->box.border,
		 &rect);

	w = Widget_GetChild(content, 26);
	rect.x = content->padding.left;
	rect.y = content->padding.top + 500;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(0) .box')[26].box.border", &w->box.border,
		 &rect);

	example = Widget_GetChild(container, 3);
	content = Widget_GetChild(Widget_GetChild(example, 1), 0);
	w = Widget_GetChild(content, 7);
	rect.x = content->padding.left - 16;
	rect.y = content->padding.top + (16 + 50 + 16) + 3 * (8 + 50 + 8);
	rect.width = 100;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[7].box.border", &w->box.border,
		 &rect);

	example = Widget_GetChild(container, 5);
	content = Widget_GetChild(example, 1);

	w = LCUIWidget_GetById("box-absolute-top-left");
	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 100;
	it_rectf("$('#box-absolute-top-left')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-top-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-top-right')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-center");
	rect.x = (content->box.padding.width - rect.width) * 0.5f;
	rect.y = (content->box.padding.height - rect.height) * 0.5f;
	it_rectf("$('#box-absolute-center')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-bottom-left");
	rect.x = 0;
	rect.y = content->box.padding.height - rect.height;
	it_rectf("$('#box-absolute-bottom-left')[0].box.border", &w->box.border,
		 &rect);

	w = LCUIWidget_GetById("box-absolute-bottom-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-bottom-right')[0].box.border",
		 &w->box.border, &rect);
}

void test_block_layout(void)
{
	LCUI_Widget root;
	LCUI_Widget wrapper;

	LCUI_Init();
	wrapper = LCUIBuilder_LoadFile("test_block_layout.xml");
	root = LCUIWidget_GetRoot();
	Widget_Append(root, wrapper);
	Widget_Unwrap(wrapper);
	LCUIWidget_Update();

	describe("root width 1280px", test_block_layout_1280);
	describe("root width 600px", test_block_layout_600);
	describe("root width 320px", test_block_layout_320);

#ifndef PREVIEW_MODE
	LCUI_Destroy();
#endif
}

#ifdef PREVIEW_MODE

int main(int argc, char **argv)
{
	Logger_SetLevel(LOGGER_LEVEL_INFO);
	test_block_layout();
	return LCUI_Main();
}

#endif
