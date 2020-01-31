#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/gui/builder.h>
#include "test.h"
#include "libtest.h"

static void test_btn_text_style(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-textview")->style->sheet;
	it_i("width", (int)s[key_width].val_px, 100);
	it_i("height", (int)s[key_height].val_px, 60);
	it_i("position", s[key_position].val_style, SV_ABSOLUTE);
	it_i("top", (int)s[key_top].val_px, 12);
	it_i("left", (int)s[key_left].val_px, 20);
}

static void test_btn_hover_text_style(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-textview")->style->sheet;
	it_i("background-color", s[key_background_color].val_color.value,
	     0xffff0000);
	it_i("background-size", s[key_background_size].val_style, SV_CONTAIN);
}

static void test_flex_box(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-box")->style->sheet;
	it_i("flex-grow", s[key_flex_grow].val_int, 0);
	it_i("flex-shrink", s[key_flex_shrink].val_int, 0);
	it_i("flex-basis", s[key_flex_basis].val_style, SV_AUTO);
	it_i("flex-direction", s[key_flex_direction].val_style, SV_COLUMN);
	it_i("flex-wrap", s[key_flex_wrap].val_style, SV_NOWRAP);
	it_i("justify-content", s[key_justify_content].val_style, SV_CENTER);
	it_i("align-items", s[key_align_items].val_style, SV_FLEX_END);
	it_i("align-content", s[key_align_content].val_style, SV_FLEX_END);
}

static void test_parse_flex_initial(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-initial")->style->sheet;
	it_i("<flex-grow>", s[key_flex_grow].val_int, 0);
	it_i("<flex-shrink>", s[key_flex_shrink].val_int, 1);
	it_i("<flex-basis>", s[key_flex_basis].val_style, SV_AUTO);
}
static void test_parse_flex_auto(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-auto")->style->sheet;
	it_i("<flex-grow>", s[key_flex_grow].val_int, 1);
	it_i("<flex-shrink>", s[key_flex_shrink].val_int, 1);
	it_i("<flex-basis>", s[key_flex_basis].val_style, SV_AUTO);
}

static void test_parse_flex_none(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-none")->style->sheet;
	it_i("<flex-grow>", s[key_flex_grow].val_int, 0);
	it_i("<flex-shrink>", s[key_flex_shrink].val_int, 0);
	it_i("<flex-basis>", s[key_flex_basis].val_style, SV_AUTO);
}

static void test_parse_flex_1(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-1")->style->sheet;
	it_i("<flex-grow>", s[key_flex_grow].val_int, 1);
	it_b("<flex-shrink>.isValid?", s[key_flex_shrink].is_valid, FALSE);
	it_b("<flex-basis>.isValid?", s[key_flex_basis].is_valid, FALSE);
}

static void test_parse_flex_100px(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-100px")->style->sheet;
	it_b("<flex-grow>.isValid?", s[key_flex_grow].is_valid, FALSE);
	it_b("<flex-shrink>.isValid?", s[key_flex_shrink].is_valid, FALSE);
	it_i("<flex-basis>", (int)s[key_flex_basis].val_px, 100);
}

static void test_parse_flex_1_100px(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-1-100px")->style->sheet;
	it_b("<flex-grow>.isValid?", s[key_flex_grow].is_valid, FALSE);
	it_i("<flex-shrink>", s[key_flex_shrink].val_int, 1);
	it_i("<flex-basis>", (int)s[key_flex_basis].val_px, 100);
}
static void test_parse_flex_0_0_100px(void)
{
	LCUI_Style s;

	s = LCUIWidget_GetById("test-flex-0-0-100px")->style->sheet;
	it_i("<flex-grow>", s[key_flex_grow].val_int, 0);
	it_i("<flex-shrink>", s[key_flex_shrink].val_int, 0);
	it_i("<flex-basis>", (int)s[key_flex_basis].val_px, 100);
}

void test_css_parser(void)
{
	LCUI_Widget root, box, btn;

	LCUI_Init();
	box = LCUIBuilder_LoadFile("test_css_parser.xml");
	it_b("should successfully load test_css_parser.xml", !!box, TRUE);
	if (!box) {
		LCUI_Destroy();
		return;
	}
	root = LCUIWidget_GetRoot();
	Widget_Append(root, box);
	Widget_Unwrap(box);
	LCUIWidget_Update();

	btn = LCUIWidget_GetById("test-btn");
	describe(".btn .text", test_btn_text_style);
	Widget_AddStatus(btn, "hover");
	LCUIWidget_Update();

	describe(".btn:hover .text", test_btn_text_style);
	describe("#test-flex-box", test_flex_box);
	describe("parse 'flex: auto;'", test_parse_flex_auto);
	describe("parse 'flex: none;'", test_parse_flex_none);
	describe("parse 'flex: initial;'", test_parse_flex_initial);
	describe("parse 'flex: 1;'", test_parse_flex_1);
	describe("parse 'flex: 100px;'", test_parse_flex_100px);
	describe("parse 'flex: 1 100px;'", test_parse_flex_1_100px);
	describe("parse 'flex: 0 0 100px;'", test_parse_flex_0_0_100px);
	LCUI_Destroy();
}
