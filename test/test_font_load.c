#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>
#include "test.h"
#include "libtest.h"

#define GetSegoeUIFont(S, W) LCUIFont_GetId("Segoe UI", S, W)
#define GetArialFont(S, W) LCUIFont_GetId("Arial", S, W)

void test_segoe_ui_font_load(void)
{
	LCUI_Font font;
	int id;
	if (LCUIFont_LoadFile("C:/windows/fonts/segoeui.ttf") != 0) {
		return;
	}
	it_i("load segoeui.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/segoeui.ttf"), 0);
	it_i("load segoeuib.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/segoeuib.ttf"), 0);
	it_i("load segoeuii.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/segoeuii.ttf"), 0);
	it_i("load segoeuil.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/segoeuil.ttf"), 0);
	it_i("load segoeuisl.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/segoeuisl.ttf"), 0);
	it_i("load segoeuiz.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/segoeuiz.ttf"), 0);
	it_i("load seguibl.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/seguibl.ttf"), 0);
	it_i("load seguili.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/seguili.ttf"), 0);
	it_b("check get font (style: normal, weight: normal)",
	     id = GetSegoeUIFont(FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: italic, weight: normal)",
	     id = GetSegoeUIFont(FONT_STYLE_ITALIC, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: normal, weight: bold)",
	     id = GetSegoeUIFont(FONT_STYLE_NORMAL, FONT_WEIGHT_BOLD), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_BOLD);
	}
	it_b("check get font (style: normal, weight: light)",
	     id = GetSegoeUIFont(FONT_STYLE_NORMAL, FONT_WEIGHT_LIGHT), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_LIGHT);
	}
	it_b("check get font (style: italic, weight: light)",
	     id = GetSegoeUIFont(FONT_STYLE_ITALIC, FONT_WEIGHT_LIGHT), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_LIGHT);
	}
}

void test_arial_font_load(void)
{
	int id;
	LCUI_Font font;
	if (LCUIFont_LoadFile("C:/windows/fonts/arial.ttf") != 0) {
		return;
	}
	it_i("load arialbd.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/arialbd.ttf"), 0);
	it_i("load arialblk.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/ariblk.ttf"), 0);
	it_i("load arialbi.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/arialbi.ttf"), 0);
	it_i("load ariali.ttf",
	     LCUIFont_LoadFile("C:/windows/fonts/ariali.ttf"), 0);
	it_b("check get font (style: normal, weight: normal)",
	     id = GetArialFont(FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: italic, weight: normal)",
	     id = GetArialFont(FONT_STYLE_ITALIC, FONT_WEIGHT_NORMAL), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_NORMAL);
	}
	it_b("check get font (style: normal, weight: bold)",
	     id = GetArialFont(FONT_STYLE_NORMAL, FONT_WEIGHT_BOLD), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_BOLD);
	}
	it_b("check get font (style: normal, weight: black)",
	     id = GetArialFont(FONT_STYLE_NORMAL, FONT_WEIGHT_BLACK), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_NORMAL);
		it_i("check font weight", font->weight, FONT_WEIGHT_BLACK);
	}
	it_b("check get font (style: italic, weight: bold)",
	     id = GetSegoeUIFont(FONT_STYLE_ITALIC, FONT_WEIGHT_BOLD), TRUE);
	if (id > 0) {
		font = LCUIFont_GetById(id);
		it_i("check font style", font->style, FONT_STYLE_ITALIC);
		it_i("check font weight", font->weight, FONT_WEIGHT_BOLD);
	}
}

void test_font_load(void)
{
	LCUI_InitFontLibrary();
	/* 测试是否能够从字体文件中载入字体 */
	it_i("check LCUIFont_LoadFile success",
	     LCUIFont_LoadFile("test_font_load.ttf"), 0);
#ifdef LCUI_BUILD_IN_WIN32
	describe("test segoe ui font load", test_segoe_ui_font_load);
	describe("test arial font load", test_arial_font_load);
#endif
	LCUI_FreeFontLibrary();

	LCUI_InitFontLibrary();
	LCUI_InitCSSLibrary();
	LCUI_InitCSSParser();
	/* 测试是否能够根据 CSS 文件中定义的 @font-face 规则来载入字体 */
	it_i("check LCUIFont_LoadCSSFile success",
	     LCUI_LoadCSSFile("test_font_load.css"), 0);
	it_b("check LCUIFont_GetId success",
	     LCUIFont_GetId("icomoon", 0, 0) > 0, TRUE);
	LCUI_FreeCSSParser();
	LCUI_FreeCSSLibrary();
	LCUI_FreeFontLibrary();
}
