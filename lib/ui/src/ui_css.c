#include <stdio.h>
#include <LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/css_fontstyle.h>
#include "../include/ui.h"
#include "private.h"

const char *ui_default_css = CodeToString(

* {
	width: auto;
	height:	auto;
	background-color: transparent;
	border: 0px solid transparent;
	display: block;
	position: static;
	padding: 0;
	margin: 0;
}

root {
	box-sizing: border-box;
}

);

/** 载入CSS代码块，用于实现CSS代码的分块载入 */
static size_t ui_load_css_block(LCUI_CSSParserContext ctx, const char *str)
{
	size_t size = 0;

	ctx->cur = str;
	while (*ctx->cur && size < ctx->buffer_size) {
		ctx->parsers[ctx->target].parse(ctx);
		++ctx->cur;
		++size;
	}
	return size;
}

static void ui_on_parsed_font_face(LCUI_CSSFontFace face)
{
	ui_event_t e;
	ui_event_init(&e, "font_face_load");
	LCUIFont_LoadFile(face->src);
	ui_post_event(&e, face, NULL);
}

int ui_load_css_file(const char *filepath)
{
	size_t n;
	FILE *fp;
	char buff[512];
	LCUI_CSSParserContext ctx;

	fp = fopen(filepath, "r");
	if (!fp) {
		return -1;
	}
	ctx = CSSParser_Begin(512, filepath);
	CSSParser_OnFontFaceRule(ctx, ui_on_parsed_font_face);
	n = fread(buff, 1, 511, fp);
	while (n > 0) {
		buff[n] = 0;
		ui_load_css_block(ctx, buff);
		n = fread(buff, 1, 511, fp);
	}
	CSSParser_End(ctx);
	fclose(fp);
	return 0;
}

size_t ui_load_css_string(const char *str, const char *space)
{
	size_t len = 1;
	const char *cur;
	LCUI_CSSParserContext ctx;

	DEBUG_MSG("parse begin\n");
	ctx = CSSParser_Begin(512, space);
	CSSParser_OnFontFaceRule(ctx, ui_on_parsed_font_face);
	for (cur = str; len > 0; cur += len) {
		len = ui_load_css_block(ctx, cur);
	}
	CSSParser_End(ctx);
	DEBUG_MSG("parse end\n");
	return 0;
}

void ui_init_css(void)
{
	LCUI_InitCSSLibrary();
	LCUI_InitCSSParser();
	LCUI_InitCSSFontStyle();
	ui_load_css_string(ui_default_css, __FILE__);
}

void ui_destroy_css(void)
{
	LCUI_FreeCSSFontStyle();
	LCUI_FreeCSSParser();
	LCUI_FreeCSSLibrary();
}
