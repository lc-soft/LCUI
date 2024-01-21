/*
 * lib/css/src/font_face_parser.c: -- CSS @font-face rule parser module
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <css.h>
#include "parser.h"

enum FontFaceParserState { STATE_HEAD, STATE_KEY, STATE_KEY_END, STATE_VALUE };

enum FontFaceKey {
	KEY_NONE,
	KEY_FONT_FAMILY,
	KEY_FONT_STYLE,
	KEY_FONT_WEIGHT,
	KEY_SRC
};

typedef struct css_font_face_parser_t {
	int key;
	int state;
	css_font_face_t *face;
	void (*callback)(const css_font_face_t *);
} css_font_face_parser_t;

LIBCSS_INLINE css_font_face_parser_t *get_css_font_face_parser(css_parser_t *parser)
{
	return parser->rule_parsers[CSS_RULE_FONT_FACE].data;
}

static int css_parser_begin_parse_font_face(css_parser_t *parser)
{
	get_css_font_face_parser(parser)->state = STATE_HEAD;
	return 0;
}

static int get_font_face_key(const char *name)
{
	if (strcmp(name, "font-family") == 0) {
		return KEY_FONT_FAMILY;
	} else if (strcmp(name, "font-style") == 0) {
		return KEY_FONT_STYLE;
	} else if (strcmp(name, "font-weight") == 0) {
		return KEY_FONT_WEIGHT;
	} else if (strcmp(name, "src") == 0) {
		return KEY_SRC;
	}
	return KEY_NONE;
}

static void css_font_face_parser_end(css_parser_t *parser)
{
	css_font_face_parser_t *data = get_css_font_face_parser(parser);

	if (data->face->font_family) {
		free(data->face->font_family);
		data->face->font_family = NULL;
	}
	if (data->face->src) {
		free(data->face->src);
		data->face->src = NULL;
	}
	data->key = KEY_NONE;
}

static int css_font_face_parser_parse_head(css_parser_t *parser)
{
	switch (*parser->cur) {
	CASE_WHITE_SPACE:
		return 0;
	case '/':
		return css_parser_begin_parse_comment(parser);
	case '{':
		break;
	default:
		return -1;
	}
	get_css_font_face_parser(parser)->state = STATE_KEY;
	css_parser_commit(parser);
	return 0;
}

static int css_font_face_parser_parse_tail(css_parser_t *parser)
{
	css_font_face_parser_t *data = get_css_font_face_parser(parser);

	if (data->callback) {
		data->callback(data->face);
	}
	css_font_face_parser_end(parser);
	css_parser_end_parse_rule(parser);
	if (parser->pos > 0) {
		return -1;
	}
	return 0;
}

static int css_font_face_parser_parse_key_end(css_parser_t *parser)
{
	css_font_face_parser_t *data;

	switch (*parser->cur) {
	CASE_WHITE_SPACE:
		return 0;
	case ':':
		break;
	case '}':
		return css_font_face_parser_parse_tail(parser);
	default:
		css_font_face_parser_end(parser);
		return -1;
	}
	css_parser_commit(parser);
	data = get_css_font_face_parser(parser);
	data->key = get_font_face_key(parser->buffer);
	data->state = STATE_VALUE;
	return 0;
}

static int FontFaceParser_ParseKey(css_parser_t *parser)
{
	switch (*parser->cur) {
	CASE_WHITE_SPACE:
		if (parser->pos > 0) {
			get_css_font_face_parser(parser)->state = STATE_KEY_END;
			return 0;
		}
		break;
	case '}':
		return css_font_face_parser_parse_tail(parser);
	case ':':
		css_font_face_parser_parse_key_end(parser);
		break;
	default:
		css_parser_get_char(parser);
		break;
	}
	return 0;
}

static int css_font_face_parser_parse_font_weight(css_font_face_t *face,
						  const char *str)
{
	int weight;
	if (css_parse_font_weight(str, &weight)) {
		face->font_weight = weight;
		return 0;
	}
	return -1;
}

static int css_font_face_parser_parse_font_style(css_font_face_t *face,
						 const char *str)
{
	int style;
	if (css_parse_font_style(str, &style)) {
		face->font_style = style;
		return 0;
	}
	return -1;
}

static int css_font_face_parser_parse_src(css_font_face_t *face,
					  const char *str, const char *dirname)
{
	css_style_value_t style;

	if (face->src) {
		free(face->src);
	}
	// TODO: dirname + str = absolutePath
	if (css_parse_url_value(&style, str)) {
		face->src = style.string_value;
		return 0;
	}
	free(face->src);
	face->src = NULL;
	return -1;
}

static int css_font_face_parser_parse_value(css_parser_t *parser)
{
	css_font_face_parser_t *data;
	switch (*parser->cur) {
	case '}':
	case ';':
		break;
	default:
		css_parser_get_char(parser);
		return 0;
	}
	css_parser_commit(parser);
	data = get_css_font_face_parser(parser);
	switch (data->key) {
	case KEY_FONT_FAMILY:
		if (data->face->font_family) {
			free(data->face->font_family);
		}
		data->face->font_family = strdup2(parser->buffer);
		if (!data->face->font_family) {
			return -ENOMEM;
		}
		strtrim(data->face->font_family, parser->buffer, " \"");
		break;
	case KEY_FONT_STYLE:
		css_font_face_parser_parse_font_style(data->face,
						      parser->buffer);
		break;
	case KEY_FONT_WEIGHT:
		css_font_face_parser_parse_font_weight(data->face,
						       parser->buffer);
		break;
	case KEY_SRC:
		css_font_face_parser_parse_src(data->face, parser->buffer,
					       parser->style_parser.dirname);
		break;
	default:
		break;
	}
	data->key = KEY_NONE;
	if (*parser->cur != '}') {
		get_css_font_face_parser(parser)->state = STATE_KEY;
		return 0;
	}
	return css_font_face_parser_parse_tail(parser);
}

int css_parser_parse_font_face(css_parser_t *parser)
{
	switch (get_css_font_face_parser(parser)->state) {
	case STATE_HEAD:
		return css_font_face_parser_parse_head(parser);
	case STATE_KEY:
		return FontFaceParser_ParseKey(parser);
	case STATE_KEY_END:
		return css_font_face_parser_parse_key_end(parser);
	case STATE_VALUE:
		return css_font_face_parser_parse_value(parser);
	default:
		break;
	}
	css_font_face_parser_end(parser);
	return -1;
}

void css_font_face_parser_on_load(css_parser_t *parser,
				  void (*callback)(const css_font_face_t *))
{
	get_css_font_face_parser(parser)->callback = callback;
}

int css_font_face_parser_init(css_parser_t *parser)
{
	css_font_face_parser_t *data;
	css_rule_parser_t *rule_parser;

	data = calloc(sizeof(css_font_face_parser_t), 1);
	if (!data) {
		return -ENOMEM;
	}
	data->face = calloc(sizeof(css_font_face_t), 1);
	if (!data->face) {
		free(data);
		return -ENOMEM;
	}
	data->face->src = NULL;
	data->face->font_family = NULL;
	data->face->font_style = CSS_FONT_STYLE_NORMAL;
	data->face->font_weight = CSS_FONT_WEIGHT_NORMAL;
	rule_parser = &parser->rule_parsers[CSS_RULE_FONT_FACE];
	rule_parser->data = data;
	rule_parser->parse = css_parser_parse_font_face;
	rule_parser->begin = css_parser_begin_parse_font_face;
	strcpy(rule_parser->name, "font-face");
	return 0;
}

void css_font_face_parser_destroy(css_parser_t *parser)
{
	css_font_face_parser_t *data;

	css_font_face_parser_end(parser);
	data = get_css_font_face_parser(parser);
	parser->rule_parsers[CSS_RULE_FONT_FACE].data = NULL;
	free(data->face);
	free(data);
}
