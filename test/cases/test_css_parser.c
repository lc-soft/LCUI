#include <stdio.h>
#include <LCUI.h>
#include <LCUI/ui.h>
#include <LCUI/gui/builder.h>
#include "ctest.h"

static void test_btn_text_style(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-textview")->style->sheet;
	it_i("width", (int)s[css_key_width].val_px, 100);
	it_i("height", (int)s[css_key_height].val_px, 60);
	it_i("position", s[css_key_position].val_style, CSS_KEYWORD_ABSOLUTE);
	it_i("top", (int)s[css_key_top].val_px, 12);
	it_i("left", (int)s[css_key_left].val_px, 20);
}

static void test_btn_hover_text_style(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-textview")->style->sheet;
	it_i("background-color", s[css_key_background_color].val_color.value,
	     0xffff0000);
	it_i("background-size", s[css_key_background_size].val_style, CSS_KEYWORD_CONTAIN);
}

static void test_flex_box(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-box")->style->sheet;
	it_i("flex-grow", s[css_key_flex_grow].val_int, 0);
	it_i("flex-shrink", s[css_key_flex_shrink].val_int, 0);
	it_i("flex-basis", s[css_key_flex_basis].val_style, CSS_KEYWORD_AUTO);
	it_i("flex-direction", s[css_key_flex_direction].val_style, CSS_KEYWORD_COLUMN);
	it_i("flex-wrap", s[css_key_flex_wrap].val_style, CSS_KEYWORD_NOWRAP);
	it_i("justify-content", s[css_key_justify_content].val_style, CSS_KEYWORD_CENTER);
	it_i("align-items", s[css_key_align_items].val_style, CSS_KEYWORD_FLEX_END);
	it_i("align-content", s[css_key_align_content].val_style, CSS_KEYWORD_FLEX_END);
}

static void test_parse_flex_initial(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-initial")->style->sheet;
	it_i("<flex-grow>", s[css_key_flex_grow].val_int, 0);
	it_i("<flex-shrink>", s[css_key_flex_shrink].val_int, 1);
	it_i("<flex-basis>", s[css_key_flex_basis].val_style, CSS_KEYWORD_AUTO);
}
static void test_parse_flex_auto(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-auto")->style->sheet;
	it_i("<flex-grow>", s[css_key_flex_grow].val_int, 1);
	it_i("<flex-shrink>", s[css_key_flex_shrink].val_int, 1);
	it_i("<flex-basis>", s[css_key_flex_basis].val_style, CSS_KEYWORD_AUTO);
}

static void test_parse_flex_none(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-none")->style->sheet;
	it_i("<flex-grow>", s[css_key_flex_grow].val_int, 0);
	it_i("<flex-shrink>", s[css_key_flex_shrink].val_int, 0);
	it_i("<flex-basis>", s[css_key_flex_basis].val_style, CSS_KEYWORD_AUTO);
}

static void test_parse_flex_1(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-1")->style->sheet;
	it_i("<flex-grow>", s[css_key_flex_grow].val_int, 1);
	it_b("<flex-shrink>.isValid?", s[css_key_flex_shrink].is_valid, FALSE);
	it_b("<flex-basis>.isValid?", s[css_key_flex_basis].is_valid, FALSE);
}

static void test_parse_flex_100px(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-100px")->style->sheet;
	it_b("<flex-grow>.isValid?", s[css_key_flex_grow].is_valid, FALSE);
	it_b("<flex-shrink>.isValid?", s[css_key_flex_shrink].is_valid, FALSE);
	it_i("<flex-basis>", (int)s[css_key_flex_basis].val_px, 100);
}

static void test_parse_flex_1_100px(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-1-100px")->style->sheet;
	it_b("<flex-grow>.isValid?", s[css_key_flex_grow].is_valid, FALSE);
	it_i("<flex-shrink>", s[css_key_flex_shrink].val_int, 1);
	it_i("<flex-basis>", (int)s[css_key_flex_basis].val_px, 100);
}
static void test_parse_flex_0_0_100px(void)
{
	css_unit_value_t *s;

	s = ui_get_widget("test-flex-0-0-100px")->style->sheet;
	it_i("<flex-grow>", s[css_key_flex_grow].val_int, 0);
	it_i("<flex-shrink>", s[css_key_flex_shrink].val_int, 0);
	it_i("<flex-basis>", (int)s[css_key_flex_basis].val_px, 100);
}

void test_css_parser(void)
{
	ui_widget_t *root, *box, *btn;

	lcui_init();
	box = LCUIBuilder_LoadFile("test_css_parser.xml");
	it_b("should successfully load test_css_parser.xml", !!box, TRUE);
	if (!box) {
		lcui_destroy();
		return;
	}
	root = ui_root();
	ui_widget_append(root, box);
	ui_widget_unwrap(box);
	ui_update();

	btn = ui_get_widget("test-btn");
	describe(".btn .text", test_btn_text_style);
	ui_widget_add_status(btn, "hover");
	ui_update();

	describe(".btn:hover .text", test_btn_hover_text_style);
	describe("#test-flex-box", test_flex_box);
	describe("parse 'flex: auto;'", test_parse_flex_auto);
	describe("parse 'flex: none;'", test_parse_flex_none);
	describe("parse 'flex: initial;'", test_parse_flex_initial);
	describe("parse 'flex: 1;'", test_parse_flex_1);
	describe("parse 'flex: 100px;'", test_parse_flex_100px);
	describe("parse 'flex: 1 100px;'", test_parse_flex_1_100px);
	describe("parse 'flex: 0 0 100px;'", test_parse_flex_0_0_100px);
	lcui_destroy();
}
