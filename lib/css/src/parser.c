/*
 * lib/css/src/parser.c: -- css parser module
 *
 * Copyright (c) 2022-2024, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include "debug.h"

css_parser_t *css_parser_create(const char *space)
{
        css_parser_t *parser;

        parser = calloc(sizeof(css_parser_t), 1);
        if (space) {
                parser->space = strdup2(space);
        } else {
                parser->space = NULL;
        }
        parser->buffer = calloc(sizeof(char), CSS_PARSER_BUFFER_SIZE);
        parser->buffer_size = CSS_PARSER_BUFFER_SIZE;
        parser->target = CSS_PARSER_TARGET_NONE;
        parser->comment_parser.prev_target = CSS_PARSER_TARGET_NONE;
        memset(&parser->rule, 0, sizeof(parser->rule));
        css_style_parser_init(&parser->style_parser, space);
        css_font_face_parser_init(parser);
        return parser;
}

void css_parser_destroy(css_parser_t *parser)
{
        css_style_parser_destroy(&parser->style_parser);
        css_font_face_parser_destroy(parser);
        if (parser->space) {
                free(parser->space);
        }
        free(parser->buffer);
        free(parser);
}

static int css_parser_parse_comment(css_parser_t *parser)
{
        if (parser->comment_parser.is_line_comment) {
                if (*parser->cur == '\n') {
                        parser->target = parser->comment_parser.prev_target;
                }
                return 0;
        }
        if (*parser->cur == '/' && *(parser->cur - 1) == '*') {
                parser->target = parser->comment_parser.prev_target;
        }
        return 0;
}

int css_parser_begin_parse_comment(css_parser_t *parser)
{
        switch (*(parser->cur + 1)) {
        case '/':
                parser->comment_parser.is_line_comment = true;
                break;
        case '*':
                parser->comment_parser.is_line_comment = false;
                break;
        default:
                css_parser_get_char(parser);
                return -1;
        }
        if (parser->comment_parser.prev_target != CSS_PARSER_TARGET_COMMENT) {
                parser->comment_parser.prev_target = parser->target;
                parser->target = CSS_PARSER_TARGET_COMMENT;
        }
        return 0;
}

static int css_parser_parse_selector(css_parser_t *parser)
{
        css_selector_t *s;

        switch (*parser->cur) {
        case '/':
                return css_parser_begin_parse_comment(parser);
        case '{':
                parser->target = CSS_PARSER_TARGET_KEY;
                parser->style_parser.style = css_style_decl_create();
        case ',':
                css_parser_commit(parser);
                s = css_selector_create(parser->buffer);
                DEBUG_MSG("[css-parser] selector=%s, valid=%s\n", parser->buffer, s == NULL ? "false" : "true");
                if (!s) {
                        return -1;
                }
                list_append(&parser->style_parser.selectors, s);
                break;
        default:
                css_parser_get_char(parser);
                break;
        }
        return 0;
}

static int css_parser_parse_rule_name(css_parser_t *parser)
{
        css_rule_type_t rule;
        css_rule_parser_t *rule_parser;

        switch (*parser->cur) {
        CASE_WHITE_SPACE:
                if (parser->pos > 0) {
                        break;
                }
                return -1;
        default:
                css_parser_get_char(parser);
                return 0;
        }
        css_parser_commit(parser);
        for (rule = CSS_RULE_NONE; rule < CSS_RULE_TOTAL_NUM; ++rule) {
                rule_parser = &parser->rule_parsers[rule];
                if (strlen(rule_parser->name) < 1) {
                        continue;
                }
                if (strcmp(rule_parser->name, parser->buffer) == 0) {
                        rule_parser->begin(parser);
                        parser->target = CSS_PARSER_TARGET_RULE_DATA;
                        parser->rule = rule;
                        return 0;
                }
        }
        return -1;
}

static int css_parser_parse_rule_data(css_parser_t *parser)
{
        css_rule_parser_t *rule_parser;
        rule_parser = &parser->rule_parsers[parser->rule];
        if (rule_parser->parse) {
                return rule_parser->parse(parser);
        }
        return -1;
}

static int css_parser_parse_style_property_name(css_parser_t *parser)
{
        switch (*parser->cur) {
        CASE_WHITE_SPACE:
        case ';':
                return -1;
        case ':':
                parser->target = CSS_PARSER_TARGET_VALUE;
                parser->style_parser.property =
                    malloc(sizeof(char) * (parser->pos + 1));
                css_parser_commit(parser);
                strcpy(parser->style_parser.property, parser->buffer);
                break;
        case '}':
                parser->target = CSS_PARSER_TARGET_NONE;
                css_style_parser_commit(&parser->style_parser);
                break;
        default:
                css_parser_get_char(parser);
                break;
        }
        return 0;
}

static int css_parser_parse_style_property_value(css_parser_t *parser)
{
        css_propdef_t *propdef;
        css_style_value_t value;

        switch (*parser->cur) {
        case '/':
                return css_parser_begin_parse_comment(parser);
        case '}':
        case ';':
                break;
        CASE_WHITE_SPACE:
                if (parser->pos == 0) {
                        return 0;
                }
        default:
                css_parser_get_char(parser);
                return 0;
        }
        if (*parser->cur == ';') {
                parser->target = CSS_PARSER_TARGET_KEY;
        }
        css_parser_commit(parser);
        propdef = css_get_propdef_by_name(parser->style_parser.property);
        if (!propdef) {
                logger_error(
                    "[css-parser] [property: %s] value type not defined\n",
                    parser->style_parser.property);
                free(parser->style_parser.property);
                parser->style_parser.property = NULL;
                return -1;
        }
        if (propdef->key >= 0) {
                if (css_parse_value(propdef->valdef, parser->buffer, &value) <=
                    0) {
                        logger_error(
                            "[css-parser] [property: %s] invalid value: %s\n",
                            propdef->name, parser->buffer);
                } else {
                        css_style_decl_add(parser->style_parser.style,
                                           propdef->key, &value);
                }
        } else {
                if (propdef->parse(propdef, parser->buffer,
                                   parser->style_parser.style) <= 0) {
                        logger_error("[css-parser] [shorthand property: %s] "
                                     "invalid value: %s\n",
                                     propdef->name, parser->buffer);
                }
        }
        free(parser->style_parser.property);
        parser->style_parser.property = NULL;
        DEBUG_MSG("parse style value: %s\n", parser->buffer);
        if (*parser->cur == '}') {
                parser->target = CSS_PARSER_TARGET_NONE;
                css_style_parser_commit(&parser->style_parser);
        }
        return 0;
}

static int css_parser_parse_target(css_parser_t *parser)
{
        switch (*parser->cur) {
        case '/':
                return css_parser_begin_parse_comment(parser);
        CASE_WHITE_SPACE:
        case ',':
        case '{':
        case '\\':
        case '"':
        case '}':
                return -1;
        default:
                break;
        }
        parser->pos = 0;
        if (*parser->cur == '@') {
                parser->target = CSS_PARSER_TARGET_RULE_NAME;
        } else {
                css_parser_get_char(parser);
                parser->target = CSS_PARSER_TARGET_SELECTOR;
        }
        return 0;
}

void css_parser_end_parse_rule(css_parser_t *parser)
{
        parser->rule = CSS_RULE_NONE;
        parser->target = CSS_PARSER_TARGET_NONE;
}

void css_parser_commit(css_parser_t *parser)
{
        int i, start;

        parser->buffer[parser->pos] = 0;
        for (i = 0, start = -1; i < parser->pos && start == -1; ++i) {
                switch (parser->buffer[parser->pos]) {
                CASE_WHITE_SPACE:
                        break;
                default:
                        start = i;
                        break;
                }
        }
        /* trim left */
        for (i = 0; i < start; ++i) {
                parser->buffer[i] = parser->buffer[start + i];
        }
        parser->pos -= start;
        parser->buffer[parser->pos] = 0;
        /* trim right */
        for (i = parser->pos; i > 0; --i) {
                switch (parser->buffer[i]) {
                CASE_WHITE_SPACE:
                case  0:
                        parser->buffer[i] = 0;
                        break;
                default:
                        i = 0;
                        break;
                }
        }
        parser->pos = 0;
}

size_t css_parser_parse(css_parser_t *parser, const char *str)
{
        size_t size = 0;

        parser->cur = str;
        while (*parser->cur && size < parser->buffer_size) {
                switch (parser->target) {
                case CSS_PARSER_TARGET_NONE:
                        css_parser_parse_target(parser);
                        break;
                case CSS_PARSER_TARGET_RULE_NAME:
                        css_parser_parse_rule_name(parser);
                        break;
                case CSS_PARSER_TARGET_RULE_DATA:
                        css_parser_parse_rule_data(parser);
                        break;
                case CSS_PARSER_TARGET_SELECTOR:
                        css_parser_parse_selector(parser);
                        break;
                case CSS_PARSER_TARGET_KEY:
                        css_parser_parse_style_property_name(parser);
                        break;
                case CSS_PARSER_TARGET_VALUE:
                        css_parser_parse_style_property_value(parser);
                        break;
                case CSS_PARSER_TARGET_COMMENT:
                        css_parser_parse_comment(parser);
                        break;
                default:
                        break;
                }
                ++parser->cur;
                ++size;
        }
        return size;
}
