#include "test.h"
#include <LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/widget/textview.h>

static void test_dropdown(void)
{
	ui_widget_t* w;
	int width, height;

	w = ui_get_widget("test-dropdown-menu");
	it_b("$('#test-dropdown-menu')[0].box.border.height < 200",
	     w->width < 200, TRUE);
	it_b("$('#test-dropdown-menu')[0].box.border.width > 100",
	     w->width > 100, TRUE);
	it_i("$('#test-dropdown-menu')[0].box.border.height", (int)w->height,
	     142);

	w = ui_widget_get_child(w, 0);
	width = (int)w->width;
	height = (int)w->height;

	w = ui_widget_next(w);
	it_i("$('#test-dropdown-menu .dropdown-item')[1].box.border.width",
	     (int)w->width, width);
	it_i("$('#test-dropdown-menu .dropdown-item')[1].box.border.height",
	     (int)w->height, height);

	w = ui_widget_next(w);
	it_i("$('#test-dropdown-menu .dropdown-item')[2].box.border.width",
	     (int)w->width, width);
	it_i("$('#test-dropdown-menu .dropdown-item')[2].box.border.height",
	     (int)w->height, height);

	w = ui_widget_next(w);
	it_i("$('#test-dropdown-menu .dropdown-item')[3].box.border.width",
	     (int)w->width, width);
	it_i("$('#test-dropdown-menu .dropdown-item')[3].box.border.height",
	     (int)w->height, height);
}

static void test_auto_size(void)
{
	ui_widget_t* w;
	ui_widget_t* example;
	LCUI_RectF rect;

	// Update #test-text-auto-height content

	w = ui_get_widget("test-text-auto-height");
	example = w->parent->parent;
	TextView_SetTextW(w, L"long long long long long long text");
	ui_update();
	rect.x = 5;
	rect.y = 5;
	rect.width = 200;
	rect.height = 50;
	it_rectf("$('#test-text-auto-height')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	it_i("$('#test-text-auto-size')[0].box.border.y", (int)w->y, 5 + 50);

	w = ui_widget_next(w);
	it_i("$('#test-text-block-auto-height')[0].box.border.y", (int)w->y,
	     5 + 50 + 31);

	it_i("$('.example')[2].box.border.height", (int)example->height, 166);

	// Update #test-text-auto-size content

	w = ui_get_widget("test-text-auto-size");
	TextView_SetTextW(
	    w, L"long long long long long long long long long long long long "
	       L"long long long long long long long long long long long long "
	       L"long long text");
	ui_update();
	it_i("$('#test-text-auto-size')[0].box.border.x", (int)w->box.border.x,
	     5);
	it_i("$('#test-text-auto-size')[0].box.border.y", (int)w->box.border.y,
	     5 + 50);
	it_b("$('#test-text-auto-size')[0].box.border.width <= 758",
	     w->box.border.width < 758.f, TRUE);
	it_i("$('#test-text-auto-size')[0].box.border.height",
	     (int)w->box.border.height, 50);

	w = ui_widget_next(w);
	it_i("$('#test-text-block-auto-height')[0].box.border.y", (int)w->y,
	     5 + 50 + 50);

	it_i("$('.example')[2].box.border.height", (int)example->height, 185);

	// Update test-text-block-auto-height content

	w = ui_get_widget("test-text-block-auto-height");
	TextView_SetTextW(
	    w, L"long long long long long long long long long long long long "
	       L"long long long long long long long long long long long long "
	       L"long long text");
	ui_update();
	rect.x = 5;
	rect.y = 5 + 50 + 50;
	rect.width = 758;
	rect.height = 50;
	it_rectf("$('#test-text-block-auto-height')[0].box.border",
		 &w->box.border, &rect);
}

static void test_inline_block_nesting(void)
{
	ui_widget_t* w;
	LCUI_RectF rect;

	w = ui_get_widget("example-inline-block-nesting__block-1");
	rect.x = 5;
	rect.y = 5;
	rect.width = ui_widget_get_child(w, 0)->width * 2;
	rect.height = 31 * 3;
	it_rectf("$('#example-inline-block-nesting__block-1')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_get_child(w, 1);
	rect.width /= 2.f;
	rect.height = 31;
	rect.x = rect.width;
	rect.y = 0;
	it_rectf(
	    "$('#example-inline-block-nesting__block-1 .box')[1].box.border",
	    &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.width = rect.width * 2;
	rect.x = 0;
	rect.y = 31;
	it_rectf(
	    "$('#example-inline-block-nesting__block-1 .box')[2].box.border",
	    &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.y = 31 * 2;
	it_rectf(
	    "$('#example-inline-block-nesting__block-1 .box')[3].box.border",
	    &w->box.border, &rect);

	w = ui_get_widget("example-inline-block-nesting__block-2");
	rect.width = ui_widget_get_child(w, 0)->width * 2;
	rect.height = 31 * 3;
	rect.x = w->parent->padding.left;
	rect.x += w->parent->box.content.width - rect.width;
	rect.y = 5;
	it_rectf("$('#example-inline-block-nesting__block-2')[0].box.border",
		 &w->box.border, &rect);
}

static void test_absolutely_positioned_progress_bar(void)
{
	ui_widget_t* w;

	w = ui_get_widget("example-progress-bar__bar");
	it_i("It should have the correct width", (int)w->width, (int)(w->parent->box.content.width * 0.5));
}

static void test_block_layout_1280(void)
{
	ui_widget_t* w;
	ui_widget_t* content;
	ui_widget_t* example;
	ui_widget_t* container;
	LCUI_RectF rect;

	container = ui_widget_get_child(ui_get_widget("main"), 0);
	LCUIDisplay_SetSize(1280, 800);
	ui_update();

	rect.x = 10;
	rect.y = 10;
	rect.width = 780;
	rect.height = 404;
	w = ui_widget_get_child(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 304;
	w = ui_widget_get_child(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 147;
	w = ui_widget_get_child(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 550;
	w = ui_widget_get_child(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 6);
	it_rectf("$('.example')[6].box.border", &w->box.border, &rect);

	w = ui_get_widget("example-inline-block-nesting");
	rect.y += rect.height + 10;
	rect.height = 157;
	it_rectf("$('#example-inline-block-nesting')[5].box.border",
		 &w->box.border, &rect);

	example = ui_widget_get_child(container, 0);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 13);
	rect.width = 150;
	rect.height = 50;
	rect.x = content->padding.left;
	rect.y = 150 + content->padding.top;
	it_rectf("$('.example:eq(0) .box')[13].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x = content->box.content.width - 150.f;
	rect.x += content->padding.left;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box.ml-auto')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.x = (content->box.content.width - 150.f) / 2.f;
	rect.x += content->padding.left;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box.ml-auto.mr-auto')[0].box.border",
		 &w->box.border, &rect);

	example = ui_widget_get_child(container, 1);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = content->padding.left;
	rect.y = content->padding.top;
	rect.width = content->box.content.width * 0.5f;
	rect.height = 50;
	it_rectf("$('.example:eq(1) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	it_rectf("$('.example:eq(1) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = content->box.content.width;
	it_rectf("$('.example:eq(1) .box')[2].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = content->box.content.width * 0.5f;
	it_rectf("$('.example:eq(1) .box')[3].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[4].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.x = content->padding.left;
	rect.width = content->box.content.width * 0.25f;
	it_rectf("$('.example:eq(1) .box')[5].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[6].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[7].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[8].box.border", &w->box.border,
		 &rect);

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(ui_widget_get_child(example, 1), 0);
	w = ui_widget_get_child(content, 0);
	rect.x = content->padding.left + w->margin.left;
	rect.y = content->padding.top + w->margin.top;
	rect.width = content->box.content.width;
	rect.width -= w->margin.left + w->margin.right;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 5);
	rect.x = content->padding.left + 264.f;
	rect.y = content->padding.top + rect.height + 32;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[5].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width - 8;
	rect.y += 8;
	rect.width = 150;
	it_rectf("$('.example:eq(3) .box')[6].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width + w->margin.left + 8;
	rect.y -= 8;
	rect.width = 100;
	it_rectf("$('.example:eq(3 .box')[7].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x = content->padding.left - 16;
	rect.y = content->padding.top + (16 + 50 + 16) + (8 + 50 + 8) - 16;
	rect.width = content->box.content.width + 32;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[8].box.border", &w->box.border,
		 &rect);

	content = ui_widget_get_child(ui_widget_get_child(example, 1), 1);
	w = ui_widget_get_child(content, 0);
	rect.x = content->padding.left;
	rect.y = content->padding.top;
	rect.width = content->box.content.width;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[9].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += w->margin.left;
	rect.y += 50;
	rect.width -= w->margin.left;
	it_rectf("$('.example:eq(3) .box')[10].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = content->box.content.width + 16;
	it_rectf("$('.example:eq(3) .box.ml--1.mr--1')[0].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = 200;
	it_rectf("$('.example:eq(3) .box.ml--1.mr--1')[1].box.border",
		 &w->box.border, &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = content->box.content.width;
	it_rectf("$('.example:eq(3) .box.ml--1.mr--1')[2].box.border",
		 &w->box.border, &rect);

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);

	w = ui_get_widget("box-absolute-top-left");
	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 100;
	it_rectf("$('#box-absolute-top-left')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-top-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-top-right')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-center");
	rect.x = (content->box.padding.width - rect.width) * 0.5f;
	rect.y = (content->box.padding.height - rect.height) * 0.5f;
	it_rectf("$('#box-absolute-center')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-bottom-left");
	rect.x = 0;
	rect.y = content->box.padding.height - rect.height;
	it_rectf("$('#box-absolute-bottom-left')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-bottom-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-bottom-right')[0].box.border",
		 &w->box.border, &rect);

	// (2) Auto size

	w = ui_get_widget("test-text-auto-height");
	rect.x = 5;
	rect.y = 5;
	rect.width = 200;
	rect.height = 31;
	it_rectf("$('#test-text-auto-height')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("test-text-auto-size");
	it_b("$('#test-text-auto-size')[0].width < 140px", w->width < 170,
	     TRUE);
	it_i("$('#test-text-auto-size')[0].height", (int)w->height, 31);

	w = ui_get_widget("test-text-block-auto-height");
	it_i("$('#test-text-block-auto-height')[0].width", (int)w->width, 758);
	it_i("$('#test-text-block-auto-height')[0].height", (int)w->height, 31);

	describe("test auto size", test_auto_size);
	describe("test dropdown", test_dropdown);
	describe("test inline block nesting", test_inline_block_nesting);
	describe("test absolutely positioned progress bar",
		 test_absolutely_positioned_progress_bar);
}

static void test_block_layout_600(void)
{
	ui_widget_t* w;
	ui_widget_t* content;
	ui_widget_t* example;
	ui_widget_t* container;
	LCUI_RectF rect;

	container = ui_widget_get_child(ui_get_widget("main"), 0);
	LCUIDisplay_SetSize(600, 400);
	ui_update();

	rect.x = 10;
	rect.y = 10;
	rect.width = 600 - 20 - 14;
	rect.height = 504;
	w = ui_widget_get_child(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 304;
	w = ui_widget_get_child(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 204;
	w = ui_widget_get_child(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 600;
	w = ui_widget_get_child(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 354;
	w = ui_widget_get_child(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	example = ui_widget_get_child(container, 0);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 11);
	rect.width = 50;
	rect.height = 50;
	rect.x = content->padding.left;
	rect.y = 100 + content->padding.top;
	it_rectf("$('.example:eq(0) .box')[11].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 14);
	rect.width = 150;
	rect.x = content->padding.left;
	rect.x += content->box.content.width - rect.width;
	rect.y = 250 + content->padding.top;
	it_rectf("$('.example:eq(0) .box')[14].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x = content->padding.left;
	rect.x += (content->box.content.width - rect.width) / 2.f;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box')[15].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 26);
	rect.x = content->padding.left;
	rect.y = content->padding.top + 400;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(0) .box')[26].box.border", &w->box.border,
		 &rect);

	example = ui_widget_get_child(container, 1);
	content = ui_widget_get_child(example, 1);
	w = ui_widget_get_child(content, 0);
	rect.x = content->padding.left;
	rect.y = content->padding.top;
	rect.width = content->box.content.width * 0.5f;
	rect.height = 50;
	it_rectf("$('.example:eq(1) .box')[0].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	it_rectf("$('.example:eq(1) .box')[1].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = content->box.content.width;
	it_rectf("$('.example:eq(1) .box')[2].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.width = content->box.content.width * 0.5f;
	it_rectf("$('.example:eq(1) .box')[3].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[4].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.y += 50;
	rect.x = content->padding.left;
	rect.width = content->box.content.width * 0.25f;
	it_rectf("$('.example:eq(1) .box')[5].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[6].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[7].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x += rect.width;
	it_rectf("$('.example:eq(1) .box')[8].box.border", &w->box.border,
		 &rect);

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(ui_widget_get_child(example, 1), 0);
	w = ui_widget_get_child(content, 7);

	rect.x = content->padding.left - 16;
	rect.y = content->padding.top + (16 + 50 + 16) + (8 + 50 + 8);
	rect.width = 100;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[7].box.border", &w->box.border,
		 &rect);

	content = ui_widget_get_child(ui_widget_get_child(example, 1), 1);
	w = ui_widget_get_child(content, 4);
	rect.x = content->padding.left - 8;
	rect.y = content->padding.top + 200;
	rect.width = content->box.content.width;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[13].box.border", &w->box.border,
		 &rect);

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);

	w = ui_get_widget("box-absolute-top-left");
	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 100;
	it_rectf("$('#box-absolute-top-left')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-top-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-top-right')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-center");
	rect.x = (content->box.padding.width - rect.width) * 0.5f;
	rect.y = (content->box.padding.height - rect.height) * 0.5f;
	it_rectf("$('#box-absolute-center')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-bottom-left");
	rect.x = 0;
	rect.y = content->box.padding.height - rect.height;
	it_rectf("$('#box-absolute-bottom-left')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-bottom-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-bottom-right')[0].box.border",
		 &w->box.border, &rect);
}

static void test_block_layout_320(void)
{
	ui_widget_t* w;
	ui_widget_t* content;
	ui_widget_t* example;
	ui_widget_t* container;
	LCUI_RectF rect;

	container = ui_widget_get_child(ui_get_widget("main"), 0);
	LCUIDisplay_SetSize(320, 240);
	ui_update();

	rect.x = 10;
	rect.y = 10;
	rect.width = 320 - 20 - 14;
	rect.height = 604;
	w = ui_widget_get_child(container, 0);
	it_rectf("$('.example')[0].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 304;
	w = ui_widget_get_child(container, 1);
	it_rectf("$('.example')[1].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 280;
	w = ui_widget_get_child(container, 2);
	it_rectf("$('.example')[2].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 732;
	w = ui_widget_get_child(container, 3);
	it_rectf("$('.example')[3].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 504;
	w = ui_widget_get_child(container, 4);
	it_rectf("$('.example')[4].box.border", &w->box.border, &rect);

	rect.y += rect.height + 10;
	rect.height = 254;
	w = ui_widget_get_child(container, 5);
	it_rectf("$('.example')[5].box.border", &w->box.border, &rect);

	example = ui_widget_get_child(container, 0);
	content = ui_widget_get_child(example, 1);

	w = ui_widget_get_child(content, 11);
	rect.x = content->padding.left;
	rect.y = content->padding.top + 150;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(0) .box')[11].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 14);
	rect.width = 150;
	rect.x = content->padding.left;
	rect.x += content->box.content.width - rect.width;
	rect.y = content->padding.top + 300;
	it_rectf("$('.example:eq(0) .box')[14].box.border", &w->box.border,
		 &rect);

	w = ui_widget_next(w);
	rect.x = content->padding.left;
	rect.x += (content->box.content.width - rect.width) / 2.f;
	rect.y += 50;
	it_rectf("$('.example:eq(0) .box')[15].box.border", &w->box.border,
		 &rect);

	w = ui_widget_get_child(content, 26);
	rect.x = content->padding.left;
	rect.y = content->padding.top + 500;
	rect.width = 50;
	rect.height = 50;
	it_rectf("$('.example:eq(0) .box')[26].box.border", &w->box.border,
		 &rect);

	example = ui_widget_get_child(container, 3);
	content = ui_widget_get_child(ui_widget_get_child(example, 1), 0);
	w = ui_widget_get_child(content, 7);
	rect.x = content->padding.left - 16;
	rect.y = content->padding.top + (16 + 50 + 16) + 3 * (8 + 50 + 8);
	rect.width = 100;
	rect.height = 50;
	it_rectf("$('.example:eq(3) .box')[7].box.border", &w->box.border,
		 &rect);

	example = ui_widget_get_child(container, 5);
	content = ui_widget_get_child(example, 1);

	w = ui_get_widget("box-absolute-top-left");
	rect.x = 0;
	rect.y = 0;
	rect.width = 100;
	rect.height = 100;
	it_rectf("$('#box-absolute-top-left')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-top-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-top-right')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-center");
	rect.x = (content->box.padding.width - rect.width) * 0.5f;
	rect.y = (content->box.padding.height - rect.height) * 0.5f;
	it_rectf("$('#box-absolute-center')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-bottom-left");
	rect.x = 0;
	rect.y = content->box.padding.height - rect.height;
	it_rectf("$('#box-absolute-bottom-left')[0].box.border", &w->box.border,
		 &rect);

	w = ui_get_widget("box-absolute-bottom-right");
	rect.x = content->box.padding.width - rect.width;
	it_rectf("$('#box-absolute-bottom-right')[0].box.border",
		 &w->box.border, &rect);
}

void test_block_layout(void)
{
	ui_widget_t* root;
	ui_widget_t* wrapper;

	LCUI_Init();
	wrapper = LCUIBuilder_LoadFile("test_block_layout.xml");
	root = ui_root();
	ui_widget_append(root, wrapper);
	ui_widget_unwrap(wrapper);
	ui_update();

	describe("root width 1280px", test_block_layout_1280);
	describe("root width 600px", test_block_layout_600);
	describe("root width 320px", test_block_layout_320);

#ifndef PREVIEW_MODE
	LCUI_Destroy();
#endif
}
