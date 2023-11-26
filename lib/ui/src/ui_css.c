#include <stdio.h>
#include <css.h>
#include <pandagl.h>
#include <ui/base.h>
#include <ui/css.h>
#include <ui/events.h>
#include "ui_css.h"

const char *ui_default_css = css_string(

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

static void ui_on_parsed_font_face(const css_font_face_t *face)
{
	ui_event_t e;

	ui_event_init(&e, "css_font_face_load");
	pd_font_library_load_file(face->src);
	ui_post_event(&e, (css_font_face_t*)face, NULL);
}

static void ui_on_css_loaded(void)
{
	ui_event_t e;

        ui_refresh_style();
	ui_event_init(&e, "css_load");
	ui_post_event(&e, NULL, NULL);
}

int ui_load_css_file(const char *filepath)
{
	size_t n;
	FILE *fp;
	char buff[512];
	css_parser_t *parser;

	fp = fopen(filepath, "r");
	if (!fp) {
		return -1;
	}
	parser = css_parser_create(filepath);
	css_font_face_parser_on_load(parser, ui_on_parsed_font_face);
	while ((n = fread(buff, 1, 511, fp)) > 0) {
		buff[n] = 0;
		css_parser_parse(parser, buff);
	}
	css_parser_destroy(parser);
	fclose(fp);
	ui_on_css_loaded();
	return 0;
}

size_t ui_load_css_string(const char *str, const char *space)
{
	size_t len = 1;
	const char *cur;
	css_parser_t *parser;

	DEBUG_MSG("parse begin\n");
	parser = css_parser_create(space);
	css_font_face_parser_on_load(parser, ui_on_parsed_font_face);
	for (cur = str; len > 0; cur += len) {
		len = css_parser_parse(parser, cur);
	}
	css_parser_destroy(parser);
	ui_on_css_loaded();
	DEBUG_MSG("parse end\n");
	return 0;
}

void ui_init_css(void)
{
	css_init();
	ui_load_css_string(ui_default_css, __FILE__);
}

void ui_destroy_css(void)
{
	css_destroy();
}
