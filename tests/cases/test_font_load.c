#include <LCUI.h>
#include <LCUI/font.h>
#include <LCUI/ui.h>
#include <LCUI/css.h>
#include "ctest.h"
#include <stdio.h>

#define GetSegoeUIFont(S, W) fontlib_get_font_id("Segoe UI", S, W)
#define GetArialFont(S, W) fontlib_get_font_id("Arial", S, W)

void test_segoe_ui_font_load(void)
{
	font_t *font;
	int id;
	if (fontlib_load_file("C:/windows/fonts/segoeui.ttf") != 0) {
		return;
	}
	it_i("load segoeui.ttf",
	     fontlib_load_file("C:/windows/fonts/segoeui.ttf"), 0);
	it_i("load segoeuib.ttf",
	     fontlib_load_file("C:/windows/fonts/segoeuib.ttf"), 0);
	it_i("load segoeuii.ttf",
	     fontlib_load_file("C:/windows/fonts/segoeuii.ttf"), 0);
	it_i("load segoeuil.ttf",
	     fontlib_load_file("C:/windows/fonts/segoeuil.ttf"), 0);
	it_i("load segoeuisl.ttf",
	     fontlib_load_file("C:/windows/fonts/segoeuisl.ttf"), 0);
	it_i("load segoeuiz.ttf",
	     fontlib_load_file("C:/windows/fonts/segoeuiz.ttf"), 0);
	it_i("load seguibl.ttf",
	     fontlib_load_file("C:/windows/fonts/seguibl.ttf"), 0);
	it_i("load seguili.ttf",
	     fontlib_load_file("C:/windows/fonts/seguili.ttf"), 0);
	it_b("check get font (style: normal, weight: normal)",
	     id = GetSegoeUIFont(FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: italic, weight: normal)",
	     id = GetSegoeUIFont(FONT_STYLE_ITALIC, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: normal, weight: bold)",
	     id = GetSegoeUIFont(FONT_STYLE_NORMAL, FONT_WEIGHT_BOLD), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_BOLD);
	}
	it_b("check get font (style: normal, weight: light)",
	     id = GetSegoeUIFont(FONT_STYLE_NORMAL, FONT_WEIGHT_LIGHT), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_LIGHT);
	}
	it_b("check get font (style: italic, weight: extra light)",
	     id = GetSegoeUIFont(FONT_STYLE_ITALIC, FONT_WEIGHT_EXTRA_LIGHT), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_EXTRA_LIGHT);
	}
}

void test_arial_font_load(void)
{
	int id;
	font_t *font;
	if (fontlib_load_file("C:/windows/fonts/arial.ttf") != 0) {
		return;
	}
	it_i("load arialbd.ttf",
	     fontlib_load_file("C:/windows/fonts/arialbd.ttf"), 0);
	it_i("load arialblk.ttf",
	     fontlib_load_file("C:/windows/fonts/ariblk.ttf"), 0);
	it_i("load arialbi.ttf",
	     fontlib_load_file("C:/windows/fonts/arialbi.ttf"), 0);
	it_i("load ariali.ttf",
	     fontlib_load_file("C:/windows/fonts/ariali.ttf"), 0);
	it_b("check get font (style: normal, weight: normal)",
	     id = GetArialFont(FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: italic, weight: normal)",
	     id = GetArialFont(FONT_STYLE_ITALIC, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: normal, weight: bold)",
	     id = GetArialFont(FONT_STYLE_NORMAL, FONT_WEIGHT_BOLD), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_BOLD);
	}
	it_b("check get font (style: normal, weight: black)",
	     id = GetArialFont(FONT_STYLE_NORMAL, FONT_WEIGHT_BLACK), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_BLACK);
	}
	it_b("check get font (style: italic, weight: bold)",
	     id = GetSegoeUIFont(FONT_STYLE_ITALIC, FONT_WEIGHT_BOLD), TRUE);
	if (id > 0) {
		font = fontlib_get_font(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_BOLD);
	}
}

void test_font_load(void)
{
	fontlib_init();
	/* 测试是否能够从字体文件中载入字体 */
	it_i("check fontlib_load_file success",
	     fontlib_load_file("test_font_load.ttf"), 0);
#ifdef LCUI_PLATFORM_WIN32
	describe("test segoe ui font load", test_segoe_ui_font_load);
	describe("test arial font load", test_arial_font_load);
#endif
	fontlib_destroy();

	ui_init();
	/* 测试是否能够根据 CSS 文件中定义的 @font-face 规则来载入字体 */
	it_i("check LCUIFont_LoadCSSFile success",
	     ui_load_css_file("test_font_load.css"), 0);
	it_b("check fontlib_get_font_id success",
	     fontlib_get_font_id("icomoon", 0, 0) > 0, TRUE);
	ui_destroy();
}
