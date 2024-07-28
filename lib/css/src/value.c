/*
 * lib/css/src/value.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/CSS/Value_definition_syntax
 * @see https://drafts.csswg.org/css-values/#value-defs
 * @see https://developer.mozilla.org/en-US/docs/Web/API/CSS/RegisterProperty
 **/

// #define DEBUG

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <css/keywords.h>
#include <css/library.h>
#include <css/style_value.h>
#include <css/value.h>
#include "parser.h"
#include "dump.h"
#include "debug.h"

#define CSS_VALDEF_PARSER_ERROR_SIZE 256

typedef enum css_valdef_sign_t {
	CSS_VALDEF_SIGN_NONE,
	CSS_VALDEF_SIGN_JUXTAPOSITION,
	CSS_VALDEF_SIGN_DOUBLE_AMPERSAND,
	CSS_VALDEF_SIGN_DOUBLE_BAR,
	CSS_VALDEF_SIGN_SINGLE_BAR,
	CSS_VALDEF_SIGN_BRACKETS,
	CSS_VALDEF_SIGN_ANGLE_BRACKET
} css_valdef_sign_t;

struct css_value_type_record_t {
	char *name;
	css_value_parse_func_t parse_value;
};

struct css_valdef_t {
	css_valdef_sign_t sign;
	unsigned min_count;
	unsigned max_count;
	const css_valdef_t *source;
	union {
		css_keyword_value_t ident;
		/** list_t<css_valdef_t> */
		list_t children;
		const css_value_type_record_t *type;
	};
};

typedef enum css_valdef_parser_target_t {
	CSS_VALDEF_PARSER_TARGET_NONE,
	CSS_VALDEF_PARSER_TARGET_ERROR,
	CSS_VALDEF_PARSER_TARGET_KEYWORD,
	CSS_VALDEF_PARSER_TARGET_DATA_TYPE,
	CSS_VALDEF_PARSER_TARGET_CURLY_BRACES,
	CSS_VALDEF_PARSER_TARGET_BRACKETS,
	CSS_VALDEF_PARSER_TARGET_QUESTION_MARK,
	CSS_VALDEF_PARSER_TARGET_SIGN
} css_valdef_parser_target_t;

typedef struct css_valdef_parser_t {
	const char *cur;
	char *buffer;
	char terminator;
	size_t pos;
	size_t buffer_size;
	css_valdef_parser_target_t target;
	char error[CSS_VALDEF_PARSER_ERROR_SIZE];

	css_valdef_t *valdef;

	/** list_t<css_valdef_parser_target_t> */
	list_t valdef_parents;
} css_valdef_parser_t;

typedef struct css_value_matcher_t {
	const char *cur;
	const char *next;

	char *value_str;
	size_t value_str_len;

	css_style_array_value_t value;
	unsigned value_len;
} css_value_matcher_t;

static struct css_value_module_t {
	/** dict_t<string, css_valdef_t> */
	dict_t *alias;

	/** dict_t<string, css_value_type_record_t> */
	dict_t *types;
} css_value;

static bool css_valdef_has_children(css_valdef_t *valdef)
{
	switch (valdef->sign) {
	case CSS_VALDEF_SIGN_JUXTAPOSITION:
	case CSS_VALDEF_SIGN_DOUBLE_AMPERSAND:
	case CSS_VALDEF_SIGN_DOUBLE_BAR:
	case CSS_VALDEF_SIGN_SINGLE_BAR:
	case CSS_VALDEF_SIGN_BRACKETS:
		return true;
	default:
		break;
	}
	return FALSE;
}

void css_valdef_destroy(css_valdef_t *valdef)
{
	if (css_valdef_has_children(valdef)) {
		list_destroy(&valdef->children,
			     (list_item_destructor_t)(css_valdef_destroy));
	}
	free(valdef);
}

static void css_valdef_shallow_destroy(css_valdef_t *valdef)
{
	if (css_valdef_has_children(valdef)) {
		list_destroy(&valdef->children, NULL);
	}
	free(valdef);
}

static void css_value_alias_destroy_value(void *priv, void *val)
{
	css_valdef_destroy(val);
}

static void css_value_types_destroy_value(void *priv, void *val)
{
	css_value_type_record_t *t = val;

	free(t->name);
	free(t);
}

void css_init_value_definitons(void)
{
	static dict_type_t alias_dt;
	static dict_type_t types_dt;

	dict_init_string_copy_key_type(&alias_dt);
	alias_dt.val_destructor = css_value_alias_destroy_value;
	css_value.alias = dict_create(&alias_dt, NULL);

	dict_init_string_key_type(&types_dt);
	types_dt.val_destructor = css_value_types_destroy_value;
	css_value.types = dict_create(&types_dt, NULL);
}

void css_destroy_value_definitons(void)
{
	dict_destroy(css_value.alias);
	dict_destroy(css_value.types);
	css_value.types = NULL;
	css_value.alias = NULL;
}

static css_valdef_t *css_valdef_create(css_valdef_sign_t sign)
{
	css_valdef_t *valdef;

	valdef = calloc(1, sizeof(css_valdef_t));
	if (!valdef) {
		return NULL;
	}
	valdef->max_count = 1;
	valdef->min_count = 1;
	valdef->sign = sign;
	return valdef;
}

static css_valdef_t *css_valdef_shallow_copy(const css_valdef_t *valdef)
{
	list_node_t *node;
	css_valdef_t *copy;

	copy = css_valdef_create(valdef->sign);
	if (!copy) {
		return NULL;
	}
	*copy = *valdef;
	if (valdef->sign != CSS_VALDEF_SIGN_NONE &&
	    valdef->sign != CSS_VALDEF_SIGN_ANGLE_BRACKET) {
		list_create(&copy->children);
		for (list_each(node, &valdef->children)) {
			list_append(&copy->children, node->data);
		}
	}
	return copy;
}

static void css_valdef_append(css_valdef_t *valdef, css_valdef_t *child)
{
	assert(valdef->sign != CSS_VALDEF_SIGN_NONE &&
	       valdef->sign != CSS_VALDEF_SIGN_ANGLE_BRACKET);
	list_append(&valdef->children, child);
}

const css_value_type_record_t *css_register_value_type(
    const char *type_name, css_value_parse_func_t parse)
{
	css_value_type_record_t *t;

	t = malloc(sizeof(css_value_type_record_t));
	if (!t) {
		return NULL;
	}
	t->name = strdup2(type_name);
	t->parse_value = parse;
	if (dict_add(css_value.types, t->name, t) != 0) {
		free(t->name);
		free(t);
		return NULL;
	}
	return t;
}

const css_value_type_record_t *css_get_value_type(const char *type_name)
{
	return dict_fetch_value(css_value.types, type_name);
}

const css_valdef_t *css_resolve_valdef_alias(const char *alias)
{
	return dict_fetch_value(css_value.alias, alias);
}

css_valdef_parser_t *css_valdef_parser_create(size_t buffer_size,
					      char terminator)
{
	css_valdef_parser_t *parser;

	parser = calloc(sizeof(css_valdef_parser_t), 1);
	parser->buffer = calloc(sizeof(char), buffer_size);
	parser->buffer_size = buffer_size;
	parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
	parser->terminator = terminator;
	list_create(&parser->valdef_parents);
	list_append(&parser->valdef_parents,
		    css_valdef_create(CSS_VALDEF_SIGN_JUXTAPOSITION));
	return parser;
}

void css_valdef_parser_destroy(css_valdef_parser_t *parser)
{
	free(parser->buffer);
	free(parser);
}

LIBCSS_INLINE void css_valdef_parser_get_char(css_valdef_parser_t *parser)
{
	parser->buffer[parser->pos++] = *(parser->cur);
	parser->buffer[parser->pos] = 0;
}

static int css_valdef_parser_error(css_valdef_parser_t *parser, const char *fmt,
				   ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(parser->error, CSS_VALDEF_PARSER_ERROR_SIZE, fmt, args);
	va_end(args);
	parser->buffer[CSS_VALDEF_PARSER_ERROR_SIZE - 1] = 0;
	parser->target = CSS_VALDEF_PARSER_TARGET_ERROR;
	return -1;
}

LIBCSS_INLINE css_valdef_t *css_valdef_parser_get_parent_valdef(
    css_valdef_parser_t *parser)
{
	assert(parser->valdef_parents.tail.prev);
	return parser->valdef_parents.tail.prev->data;
}

LIBCSS_INLINE void css_valdef_parser_reset_buffer(css_valdef_parser_t *parser)
{
	parser->buffer[0] = 0;
	parser->pos = 0;
}

// 在遇到符号时提交当前已解析的值定义
static int css_valdef_parser_commit(css_valdef_parser_t *parser,
				    css_valdef_sign_t sign)
{
	css_valdef_t *parent_valdef;
	css_valdef_t *deleted_valdef;
	css_valdef_t *child;

	// TODO: 处理 && 和 || 混写情况
	// TODO: 处理非数组值的情况
	if (!parser->valdef) {
		return 0;
	}
#ifdef DEBUG
	{
		char str[256];
		parent_valdef =
		    list_get_first_node(&parser->valdef_parents)->data;
		css_valdef_to_string(parser->valdef, str, 255);
		DEBUG_MSG("css_valdef_parser_commit(): enter %d, valdef: %s\n",
			  sign, str);
	}
#endif
#ifdef DEBUG
	{
		char str[256];
		parent_valdef =
		    list_get_first_node(&parser->valdef_parents)->data;
		css_valdef_to_string(parent_valdef, str, 255);
		DEBUG_MSG("css_valdef_parser_commit(): before: %s\n", str);
	}
#endif
	parent_valdef = css_valdef_parser_get_parent_valdef(parser);
	if (parent_valdef->sign == sign) {
		css_valdef_append(parent_valdef, parser->valdef);
		parser->valdef = NULL;
#ifdef DEBUG
		{
			char str[256];
			parent_valdef =
			    list_get_first_node(&parser->valdef_parents)->data;
			css_valdef_to_string(parent_valdef, str, 255);
			DEBUG_MSG("css_valdef_parser_commit(): after: %s\n",
				  str);
		}
#endif
		return 0;
	}

	// Before:
	// left && right |
	//               ^
	//               |
	//      current sign
	// After:
	// [left && right] |
	if (sign == CSS_VALDEF_SIGN_SINGLE_BAR) {
		css_valdef_append(parent_valdef, parser->valdef);
		parser->valdef = NULL;
		while (parent_valdef && parent_valdef->sign != sign) {
			list_delete_last(&parser->valdef_parents);
			deleted_valdef = parent_valdef;
			parent_valdef =
			    css_valdef_parser_get_parent_valdef(parser);
		}
		// Before:
		// none | top left |
		//                 ^
		//                 |
		//           current sign
		// After:
		// none | [top left] |
		if (parent_valdef && parent_valdef->sign == sign) {
			return 0;
		}
		// Before:
		// [top | center | bottom] && top left |
		//                                     ^
		//                                     |
		//                               current sign
		// After:
		// [[top | center | bottom] && [top left]] |
		child = css_valdef_create(sign);
		css_valdef_append(child, deleted_valdef);
		list_append(&parser->valdef_parents, child);
#ifdef DEBUG
		{
			char str[256];
			parent_valdef =
			    list_get_first_node(&parser->valdef_parents)->data;
			css_valdef_to_string(parent_valdef, str, 255);
			DEBUG_MSG("css_valdef_parser_commit(): after2: %s\n",
				  str);
		}
#endif

		return 0;
	}
	// Before:
	// left | right &&
	//               ^
	//               |
	//         current sign
	// After:
	// left | [right && ]
	child = css_valdef_create(sign);
	css_valdef_append(child, parser->valdef);
	css_valdef_append(parent_valdef, child);
	list_append(&parser->valdef_parents, child);
	parser->valdef = NULL;
#ifdef DEBUG
	{
		char str[256];
		parent_valdef =
		    list_get_first_node(&parser->valdef_parents)->data;
		css_valdef_to_string(parent_valdef, str, 255);
		DEBUG_MSG("css_valdef_parser_commit(): after3: %s\n", str);
	}
#endif

	return 0;
}

static css_valdef_t *css_valdef_parser_parse(css_valdef_parser_t *parser,
					     const char *definition_str);

static int css_valdef_parser_parse_brackets(css_valdef_parser_t *parser)
{
	css_valdef_t *brackets_inner;
	css_valdef_parser_t *subparser;

#ifdef DEBUG
	DEBUG_MSG("%s\n", "css_valdef_parser_parse_brackets(): enter");
#endif

	subparser = css_valdef_parser_create(parser->buffer_size, ']');
	brackets_inner = css_valdef_parser_parse(subparser, parser->cur);
	if (!brackets_inner) {
		css_valdef_parser_error(parser, subparser->error);
		css_valdef_parser_destroy(subparser);
		return -1;
	}
	parser->cur = subparser->cur;
	parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
	parser->valdef = css_valdef_create(CSS_VALDEF_SIGN_BRACKETS);
	css_valdef_append(parser->valdef, brackets_inner);

#ifdef DEBUG
	char str[256];
	css_valdef_to_string(parser->valdef, str, 255);
	DEBUG_MSG("css_valdef_parser_parse_brackets(): %s\n", str);
#endif

	css_valdef_parser_destroy(subparser);
	return 0;
}

static int css_valdef_parser_parse_keyword_end(css_valdef_parser_t *parser)
{
	// Example:
	// top left | right
	//         ^
	//         |
	//        cur
	parser->valdef = css_valdef_create(CSS_VALDEF_SIGN_NONE);
	parser->valdef->ident = css_get_keyword_key(parser->buffer);
	if (parser->valdef->ident == -1) {
		parser->valdef->ident = css_register_keyword(parser->buffer);
	}
	parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
	return 0;
}

static int css_valdef_parser_parse_keyword(css_valdef_parser_t *parser)
{
	switch (*parser->cur) {
	CASE_WHITE_SPACE:
	case '|':
	case '&':
	case '[':
	case ']':
	case '<':
	case '{':
		break;
	case '>':
		return css_valdef_parser_error(parser, "syntax error");
	default:
		css_valdef_parser_get_char(parser);
		return 0;
	}
	return css_valdef_parser_parse_keyword_end(parser);
}

static int css_valdef_parser_parse_data_type_end(css_valdef_parser_t *parser)
{
	parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
	parser->valdef = css_valdef_create(CSS_VALDEF_SIGN_ANGLE_BRACKET);
	parser->valdef->source = NULL;
	parser->valdef->type = css_get_value_type(parser->buffer);
	if (parser->valdef->type) {
		return 0;
	}
	parser->valdef->source = css_resolve_valdef_alias(parser->buffer);
	if (parser->valdef->source) {
		return 0;
	}
	return css_valdef_parser_error(parser, "unknown data type: `%s`\n",
				       parser->buffer);
}

static int css_valdef_parser_parse_data_type(css_valdef_parser_t *parser)
{
	switch (*parser->cur) {
	case '<':
	case '&':
	case '?':
	case '|':
	case '{':
	case '}':
	case '[':
	case ']':
	CASE_WHITE_SPACE:
		return css_valdef_parser_error(parser, "syntax error");
	case '>':
		break;
	default:
		css_valdef_parser_get_char(parser);
		return 0;
	}
	return css_valdef_parser_parse_data_type_end(parser);
}

static int css_valdef_parser_parse_sign_end(css_valdef_parser_t *parser)
{
	if (parser->pos == 0) {
		css_valdef_parser_commit(parser, CSS_VALDEF_SIGN_JUXTAPOSITION);
	} else if (parser->pos == 1 && parser->buffer[0] == '|') {
		css_valdef_parser_commit(parser, CSS_VALDEF_SIGN_SINGLE_BAR);
	} else if (parser->pos == 2 && parser->buffer[0] == '|') {
		css_valdef_parser_commit(parser, CSS_VALDEF_SIGN_DOUBLE_BAR);
	} else if (parser->pos == 2 && parser->buffer[0] == '&') {
		css_valdef_parser_commit(parser,
					 CSS_VALDEF_SIGN_DOUBLE_AMPERSAND);
	} else {
		return css_valdef_parser_error(parser, "unknown sign: `%s`\n",
					       parser->buffer);
	}
	// Example:
	// auto | none
	//        ^
	//        |
	//       cur
	if (parser->target == CSS_VALDEF_PARSER_TARGET_KEYWORD) {
		parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
		parser->cur--;
	}
	return 0;
}

static int css_valdef_parser_parse_sign(css_valdef_parser_t *parser)
{
	switch (*parser->cur) {
	case '|':
	case '&':
		css_valdef_parser_get_char(parser);
		switch (parser->pos) {
		case 1:
			break;
		case 2:
			if (parser->buffer[parser->pos] ==
			    parser->buffer[parser->pos - 1]) {
				break;
			}
		default:
			break;
		}
		return 0;
	case '[':
		parser->cur--;
	case ']':
		parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
		break;
	CASE_WHITE_SPACE:
		// Example:
		// none | left
		//       ^
		//       |
		//      cur
		return 0;
	case '{':
	case '}':
		return css_valdef_parser_error(parser, "syntax error");
	default:
		parser->target = CSS_VALDEF_PARSER_TARGET_KEYWORD;
		break;
	}
	return css_valdef_parser_parse_sign_end(parser);
}

static int css_valdef_parser_parse_curly_braces(css_valdef_parser_t *parser)
{
	switch (*parser->cur) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ',':
	CASE_WHITE_SPACE:
		css_valdef_parser_get_char(parser);
		return 0;
	case '{':
		return 0;
	case '}':
		break;
	default:
		return -1;
	}
	if (sscanf(parser->buffer, "%u,%u", &parser->valdef->min_count,
		   &parser->valdef->max_count) == 2) {
		parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
		return 0;
	}
	if (sscanf(parser->buffer, "%u", &parser->valdef->min_count) == 1) {
		parser->valdef->max_count = parser->valdef->min_count;
		parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
		return 0;
	}
	return css_valdef_parser_error(parser, "syntax error: %s\n",
				       parser->buffer);
}

static int css_valdef_parser_parse_question_mark(css_valdef_parser_t *parser)
{
	if (!parser->valdef || parser->valdef->min_count == 0) {
		return css_valdef_parser_error(parser, "syntax error");
	}
	parser->valdef->min_count = 0;
	parser->target = CSS_VALDEF_PARSER_TARGET_NONE;
	return 0;
}

static int css_valdef_parser_resolve_target(css_valdef_parser_t *parser)
{
	DEBUG_MSG("%c\n", *parser->cur);
	switch (*parser->cur) {
	case '|':
	case '&':
	CASE_WHITE_SPACE:
		css_valdef_parser_reset_buffer(parser);
		parser->target = CSS_VALDEF_PARSER_TARGET_SIGN;
		parser->cur--;
		break;
	case '<':
		css_valdef_parser_reset_buffer(parser);
		parser->target = CSS_VALDEF_PARSER_TARGET_DATA_TYPE;
		break;
	case '?':
		parser->target = CSS_VALDEF_PARSER_TARGET_QUESTION_MARK;
		parser->cur--;
		break;
	case '[':
		parser->target = CSS_VALDEF_PARSER_TARGET_BRACKETS;
		break;
	case '{':
		css_valdef_parser_reset_buffer(parser);
		parser->target = CSS_VALDEF_PARSER_TARGET_CURLY_BRACES;
		break;
	case ']':
	case '}':
	case '>':
		return css_valdef_parser_error(parser, "syntax error");
	default:
		css_valdef_parser_reset_buffer(parser);
		parser->target = CSS_VALDEF_PARSER_TARGET_KEYWORD;
		parser->cur--;
		break;
	}
	return 0;
}

static size_t css_valdef_parser_parse_next(css_valdef_parser_t *parser)
{
	const char *start = parser->cur;

	// printf("parse: %s\n", parser->cur);
	while (*parser->cur && *parser->cur != parser->terminator) {
		if (parser->cur >= start + parser->buffer_size) {
			++parser->cur;
			break;
		}
		switch (parser->target) {
		case CSS_VALDEF_PARSER_TARGET_NONE:
			css_valdef_parser_resolve_target(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_SIGN:
			css_valdef_parser_parse_sign(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_BRACKETS:
			css_valdef_parser_parse_brackets(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_DATA_TYPE:
			css_valdef_parser_parse_data_type(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_KEYWORD:
			css_valdef_parser_parse_keyword(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_CURLY_BRACES:
			css_valdef_parser_parse_curly_braces(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_QUESTION_MARK:
			css_valdef_parser_parse_question_mark(parser);
			break;
		case CSS_VALDEF_PARSER_TARGET_ERROR:
			return 0;
		default:
			break;
		}
		++parser->cur;
	}
	return parser->cur - start;
}

static int css_valdef_parser_finish(css_valdef_parser_t *parser)
{
	css_valdef_t *parent_valdef;

	switch (parser->target) {
	case CSS_VALDEF_PARSER_TARGET_KEYWORD:
		css_valdef_parser_parse_keyword_end(parser);
		break;
	case CSS_VALDEF_PARSER_TARGET_SIGN:
		css_valdef_parser_parse_sign_end(parser);
		break;
	case CSS_VALDEF_PARSER_TARGET_ERROR:
		return -1;
	default:
		break;
	}
	parent_valdef = css_valdef_parser_get_parent_valdef(parser);
	return css_valdef_parser_commit(parser, parent_valdef->sign);
}

static css_valdef_t *css_valdef_parser_get_result(css_valdef_parser_t *parser)
{
	css_valdef_t *valdef = NULL;

	assert(parser->valdef_parents.length > 0);
	valdef = list_get_first_node(&parser->valdef_parents)->data;
	list_destroy(&parser->valdef_parents, NULL);
	return valdef;
}

static css_valdef_t *css_valdef_parser_parse(css_valdef_parser_t *parser,
					     const char *definition_str)
{
	parser->cur = definition_str;
	while (css_valdef_parser_parse_next(parser) > 0) {
		if (parser->target == CSS_VALDEF_PARSER_TARGET_ERROR) {
			return NULL;
		}
	}
	css_valdef_parser_finish(parser);
	return css_valdef_parser_get_result(parser);
}

css_valdef_t *css_compile_valdef(const char *definition_str)
{
	css_valdef_t *valdef;
	css_valdef_parser_t *parser;

	parser = css_valdef_parser_create(512, 0);
	valdef = css_valdef_parser_parse(parser, definition_str);
	css_valdef_parser_destroy(parser);
	return valdef;
}

static int css_value_matcher_resolve_next_value(css_value_matcher_t *matcher)
{
	int quotes = 0;
	int brackets = 0;
	const char *p = matcher->next;

#ifdef DEBUG
	DEBUG_MSG("resolve_next_value(matcher<0x%p>): %s\n", matcher,
		  matcher->cur);
#endif
	while (*p) {
		switch (*p) {
		CASE_WHITE_SPACE:
			p++;
			break;
		default:
			goto resolve_value_str_tail;
		}
	}

resolve_value_str_tail:

	matcher->cur = p;
	while (*p) {
		switch (*p) {
		CASE_WHITE_SPACE:
			if (brackets < 1 && quotes < 1) {
				goto copy_value_str;
			}
			break;
		case '(':
			if (quotes < 1) {
				brackets++;
			}
			break;
		case ')':
			if (quotes < 1) {
				brackets--;
			}
			break;
		case '"':
			if (quotes > 0) {
				quotes--;
			} else {
				quotes++;
			}
			break;
		default:
			break;
		}
		p++;
	}
copy_value_str:
	if (matcher->value_str) {
		free(matcher->value_str);
		matcher->value_str = NULL;
	}
	matcher->value_str_len = p - matcher->cur;
	matcher->value_str =
	    malloc(sizeof(char) * (matcher->value_str_len + 1));
	strncpy(matcher->value_str, matcher->cur, matcher->value_str_len);
	matcher->value_str[matcher->value_str_len] = 0;
	if (matcher->value_str_len < 1) {
		return -1;
	}
	return 0;
}

static void css_value_matcher_push_value(css_value_matcher_t *matcher,
					 css_style_value_t *value)
{
	unsigned i = matcher->value_len++;
	css_array_value_set_length(&matcher->value, matcher->value_len);
	matcher->value[i] = *value;
	matcher->next = matcher->cur + matcher->value_str_len;
}

css_value_matcher_t *css_value_matcher_create(const char *str)
{
	css_value_matcher_t *matcher;

	matcher = calloc(1, sizeof(css_value_matcher_t));
	if (!matcher) {
		return NULL;
	}
	matcher->cur = str;
	matcher->next = str;
	matcher->value = NULL;
	css_value_matcher_resolve_next_value(matcher);
	return matcher;
}

static void css_value_matcher_destroy(css_value_matcher_t *matcher)
{
	matcher->cur = NULL;
	matcher->value_len = 0;
	css_array_value_destroy(matcher->value);
	free(matcher->value_str);
	matcher->value_str = NULL;
	matcher->value_str_len = 0;
	free(matcher);
}

static int css_value_matcher_match(css_value_matcher_t *matcher,
				   const css_valdef_t *valdef);

static int css_value_matcher_submatch(css_value_matcher_t *matcher,
				      const css_valdef_t *valdef);

static int css_value_matcher_match_data_type(css_value_matcher_t *matcher,
					     const css_valdef_t *valdef)
{
	css_style_value_t value = { CSS_NO_VALUE };

	if (valdef->source) {
		return css_value_matcher_submatch(matcher, valdef->source) == 0
			   ? 0
			   : -1;
	}
	if (valdef->type &&
	    valdef->type->parse_value(&value, matcher->value_str)) {
		css_value_matcher_push_value(matcher, &value);
		return 0;
	}
	return -1;
}

/**
 * @see
 * https://developer.mozilla.org/en-US/docs/Web/CSS/Value_definition_syntax#double_bar
 */
static int css_value_matcher_match_double_bar(css_value_matcher_t *matcher,
					      const css_valdef_t *valdef)
{
	int ret;
	unsigned i = 0;
	list_node_t *node;
	css_valdef_t *rest_valdef;
	char str[256];

	for (list_each(node, &valdef->children)) {
		css_valdef_to_string(node->data, str, 255);
		DEBUG_MSG("[%u/%zu] matcher->value_str: %s\n", i + 1,
			  valdef->children.length, matcher->value_str);
		if (css_value_matcher_match(matcher, node->data) != 0) {
			DEBUG_MSG("[%u/%zu] not matched valdef: %s\n", i + 1,
				  valdef->children.length, str);
			i++;
			continue;
		}
		if (valdef->children.length < 2) {
			break;
		}
		DEBUG_MSG("[%u/%zu] matched valdef: %s\n", i + 1,
			  valdef->children.length, str);
		rest_valdef = css_valdef_shallow_copy(valdef);
		// <border-width> || <border-style> || <border-color>
		//                          ^
		//                   matched valdef
		//
		// The sub matcher will use the remaining value definitions:
		// <border-width> || <border-color>
		list_delete(&rest_valdef->children, i);
		css_value_matcher_resolve_next_value(matcher);
		ret = css_value_matcher_submatch(matcher, rest_valdef);
		css_valdef_shallow_destroy(rest_valdef);
		if (ret == -1) {
			return -1;
		}
		break;
	}
	return 0;
}

/**
 * @see
 * https://developer.mozilla.org/en-US/docs/Web/CSS/Value_definition_syntax#double_ampersand
 */
static int css_value_matcher_match_double_ampersand(
    css_value_matcher_t *matcher, const css_valdef_t *valdef)
{
	int ret = -1;
	unsigned i = 0;
	list_node_t *node;
	css_valdef_t *rest_valdef;
	char str[256];

	DEBUG_MSG("\n%s\n",
		  "css_value_matcher_match_double_ampersand() enter\n");
	for (list_each(node, &valdef->children)) {
		css_valdef_to_string(node->data, str, 255);
		DEBUG_MSG("[%u/%zu] matcher->value_str: %s\n", i + 1,
			  valdef->children.length, matcher->value_str);
		if (css_value_matcher_match(matcher, node->data) != 0) {
			i++;
			DEBUG_MSG("[%u/%zu] not matched valdef: %s\n", i + 1,
				  valdef->children.length, str);
			continue;
		}
		DEBUG_MSG("[%u/%zu] matched valdef: %s\n", i + 1,
			  valdef->children.length, str);
		if (valdef->children.length < 2) {
			ret = 0;
			break;
		}
		DEBUG_MSG("match rest valdef %u\n",
			  valdef->children.length - 1);
		rest_valdef = css_valdef_shallow_copy(valdef);
		list_delete(&rest_valdef->children, i);
		css_value_matcher_resolve_next_value(matcher);
		ret = css_value_matcher_submatch(matcher, rest_valdef);
		css_valdef_shallow_destroy(rest_valdef);
		break;
	}
	DEBUG_MSG("%s\n\n", "css_value_matcher_match_double_ampersand() exit");
	return ret;
}

static int css_value_matcher_match_single_bar(css_value_matcher_t *matcher,
					      const css_valdef_t *valdef)
{
	size_t i = 0;
	list_node_t *node;
	css_value_matcher_t **submatchers;
	css_value_matcher_t *submatcher = NULL;

	submatchers =
	    malloc(valdef->children.length * sizeof(css_value_matcher_t *));
	if (!submatchers) {
		return -1;
	}
	DEBUG_MSG("\n%s\n", "css_value_matcher_match_single_bar() enter");
	for (list_each(node, &valdef->children)) {
#ifdef DEBUG
		char str[256];

		css_valdef_to_string(node->data, str, 255);
		DEBUG_MSG("[%u/%zu] %s\n", i + 1, valdef->children.length, str);
#endif
		submatchers[i] = css_value_matcher_create(matcher->cur);
		if (css_value_matcher_match(submatchers[i], node->data) == 0) {
			DEBUG_MSG("[%u/%zu] %s matched\n", i + 1,
				  valdef->children.length, str);
			if (!submatcher || submatchers[i]->value_len >=
					       submatcher->value_len) {
				submatcher = submatchers[i];
			}
		} else {
			DEBUG_MSG("[%u/%zu] %s not matched\n", i + 1,
				  valdef->children.length, str);
		}
		++i;
	}
	if (submatcher) {
		matcher->value_len =
		    css_array_value_concat(&matcher->value, &submatcher->value);
		matcher->value_str_len = submatcher->next - matcher->cur;
		matcher->next = submatcher->next;
		if (matcher->value_str) {
			free(matcher->value_str);
			matcher->value_str = NULL;
		}
	}
	for (i = 0; i < valdef->children.length; ++i) {
		css_value_matcher_destroy(submatchers[i]);
	}
	free(submatchers);
	DEBUG_MSG("%s\n\n", "css_value_matcher_match_single_bar() exit");
	return submatcher ? 0 : -1;
}

static int css_value_matcher_submatch(css_value_matcher_t *matcher,
				      const css_valdef_t *valdef)
{
	css_value_matcher_t *submatcher;

	submatcher = css_value_matcher_create(matcher->cur);
	DEBUG_MSG("submatch(matcher<0x%p>): 0x%p, cur: %s\n", matcher,
		  submatcher, submatcher->cur);
	if (css_value_matcher_match(submatcher, valdef) == 0) {
		// 可选值定义允许没有匹配的值，我们需要重置 next
		// 指针到当前值的开头，以让匹 配器继续使用当前值匹配下个值定义
		if (css_array_value_get_length(submatcher->value) == 0) {
			matcher->next = matcher->cur;
			css_value_matcher_destroy(submatcher);
			return 0;
		}
		css_array_value_concat(&matcher->value, &submatcher->value);
		matcher->value_len = css_array_value_get_length(matcher->value);
		matcher->value_str_len = submatcher->next - matcher->cur;
		matcher->next = submatcher->next;
		if (matcher->value_str) {
			free(matcher->value_str);
			matcher->value_str = NULL;
		}
		css_value_matcher_destroy(submatcher);
		return 0;
	}
	css_value_matcher_destroy(submatcher);
	return -1;
}

static int css_value_matcher_match_once(css_value_matcher_t *matcher,
					const css_valdef_t *valdef)
{
	size_t i = 0;
	list_node_t *node;
	css_style_value_t value = { CSS_NO_VALUE };

#ifdef DEBUG
	char str[256];

	css_valdef_to_string(valdef, str, 256);
	str[255] = 0;
	DEBUG_MSG("match(matcher<0x%p>, \"%s\")\n", matcher, str);
#endif
	switch (valdef->sign) {
	case CSS_VALDEF_SIGN_NONE:
		value.keyword_value = css_get_keyword_key(matcher->value_str);
		if (valdef->ident != value.keyword_value) {
			return -1;
		}
		value.type = CSS_KEYWORD_VALUE;
		css_value_matcher_push_value(matcher, &value);
		break;
	case CSS_VALDEF_SIGN_ANGLE_BRACKET:
		return css_value_matcher_match_data_type(matcher, valdef);
	case CSS_VALDEF_SIGN_JUXTAPOSITION:
	case CSS_VALDEF_SIGN_BRACKETS:
		for (list_each(node, &valdef->children)) {
			if (i > 0) {
				css_value_matcher_resolve_next_value(matcher);
			}
			if (css_value_matcher_submatch(matcher, node->data) !=
			    0) {
				return -1;
			}
			i++;
		}
		break;
	case CSS_VALDEF_SIGN_SINGLE_BAR:
		return css_value_matcher_match_single_bar(matcher, valdef);
	case CSS_VALDEF_SIGN_DOUBLE_BAR:
		return css_value_matcher_match_double_bar(matcher, valdef);
	case CSS_VALDEF_SIGN_DOUBLE_AMPERSAND:
		return css_value_matcher_match_double_ampersand(matcher,
								valdef);
	default:
		return -1;
	}
	return 0;
}

static int css_value_matcher_match(css_value_matcher_t *matcher,
				   const css_valdef_t *valdef)
{
	unsigned i;

	for (i = 0; i < valdef->max_count; ++i) {
		if (i > 0) {
			css_value_matcher_resolve_next_value(matcher);
		}
		if (css_value_matcher_match_once(matcher, valdef) != 0) {
			break;
		}
	}
	return i >= valdef->min_count ? 0 : -1;
}

int css_parse_value(const css_valdef_t *valdef, const char *str,
		    css_style_value_t *val)
{
	int ret;
	css_value_matcher_t *matcher;

	matcher = css_value_matcher_create(str);
	if (!matcher) {
		return -1;
	}
	ret = css_value_matcher_match(matcher, valdef);
	if (ret == 0) {
		val->array_value = matcher->value;
		val->type = CSS_ARRAY_VALUE;
		matcher->value = NULL;
		ret = (int)matcher->value_str_len;
	}
	css_value_matcher_destroy(matcher);
	return ret;
}

int css_register_valdef_alias(const char *alias, const char *definitons)
{
	css_valdef_t *valdef;

	if (css_get_keyword_key(alias) >= 0) {
		return -1;
	}
	valdef = css_compile_valdef(definitons);
	if (valdef) {
		return dict_add(css_value.alias, (void *)alias, valdef);
	}
	return -3;
}

static void css_dump_valdef(const css_valdef_t *valdef, css_dump_context_t *ctx)
{
	size_t len = 0, i = 0;
	list_node_t *node;
	const char *name;

	switch (valdef->sign) {
	case CSS_VALDEF_SIGN_NONE:
		name = css_get_keyword_name(valdef->ident);
		if (!name) {
			name = "unknown";
		}
		DUMP(name);
		break;
	case CSS_VALDEF_SIGN_ANGLE_BRACKET:
		DUMPF("<%s>",
		      valdef->type ? valdef->type->name : "unknown-type");
		break;
	case CSS_VALDEF_SIGN_JUXTAPOSITION:
		DUMP("(");
		len++;
		for (list_each(node, &valdef->children)) {
			css_dump_valdef(node->data, ctx);
			if (i + 1 < valdef->children.length) {
				DUMP(" ");
			}
			i++;
		}
		DUMP(")");
		len++;
		break;
	case CSS_VALDEF_SIGN_SINGLE_BAR:
		for (list_each(node, &valdef->children)) {
			css_dump_valdef(node->data, ctx);
			if (i + 1 < valdef->children.length) {
				DUMP(" | ");
			}
			i++;
		}
		break;
	case CSS_VALDEF_SIGN_DOUBLE_BAR:
		for (list_each(node, &valdef->children)) {
			css_dump_valdef(node->data, ctx);
			if (i + 1 < valdef->children.length) {
				DUMP(" || ");
			}
			i++;
		}
		break;
	case CSS_VALDEF_SIGN_DOUBLE_AMPERSAND:
		for (list_each(node, &valdef->children)) {
			css_dump_valdef(node->data, ctx);
			if (i + 1 < valdef->children.length) {
				DUMP(" && ");
			}
			i++;
		}
		break;
	case CSS_VALDEF_SIGN_BRACKETS:
		DUMP("[");
		for (list_each(node, &valdef->children)) {
			css_dump_valdef(node->data, ctx);
			if (i + 1 < valdef->children.length) {
				DUMP(" ");
			}
			i++;
		}
		DUMP("]");
		break;
	default:
		DUMP("unknown syntax");
		break;
	}
	if (valdef->min_count == 0 && valdef->max_count == 1) {
		DUMP("?");
	} else if (valdef->max_count > 1) {
		DUMPF("{%u,%u}", valdef->min_count, valdef->max_count);
	}
}

size_t css_print_valdef(const css_valdef_t *s)
{
	css_dump_context_t ctx = {
		.data = NULL, .len = 0, .max_len = 0, .func = css_dump_to_stdout
	};

	css_dump_valdef(s, &ctx);
	return ctx.len;
}

size_t css_valdef_to_string(const css_valdef_t *s, char *str, size_t max_len)
{
	css_dump_context_t ctx = { .data = str,
				   .len = 0,
				   .max_len = max_len,
				   .func = css_dump_to_buffer };

	css_dump_valdef(s, &ctx);
	return ctx.len;
}
