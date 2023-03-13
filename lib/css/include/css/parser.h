/*
 * css_parser.h -- CSS parser module
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIBCSS_INCLUDE_CSS_PARSER_H
#define LIBCSS_INCLUDE_CSS_PARSER_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

typedef enum css_parser_target_t {
	CSS_PARSER_TARGET_NONE,      /**< 无 */
	CSS_PARSER_TARGET_RULE_NAME, /**< 规则名称 */
	CSS_PARSER_TARGET_RULE_DATA, /**< 规则数据 */
	CSS_PARSER_TARGET_SELECTOR,  /**< 选择器 */
	CSS_PARSER_TARGET_KEY,       /**< 属性名 */
	CSS_PARSER_TARGET_VALUE,     /**< 属性值 */
	CSS_PARSER_TARGET_COMMENT,   /**< 注释 */
	CSS_PARSER_TARGET_TOTAL_NUM
} css_parser_target_t;

typedef enum css_rule_type_t {
	CSS_RULE_NONE,
	CSS_RULE_FONT_FACE, /**< @font-face */
	CSS_RULE_IMPORT,    /**< @import */
	CSS_RULE_MEDIA,     /**< @media */
	CSS_RULE_TOTAL_NUM
} css_rule_type_t;

typedef struct css_parser_t css_parser_t;
typedef struct css_style_parser_t css_style_parser_t;
typedef int (*css_parser_method_t)(css_parser_t *);
typedef int (*css_property_parser_method_t)(css_style_parser_t *, const char *);

typedef struct css_rule_parser_t {
	char name[32];
	void *data;
	css_parser_method_t begin;
	css_parser_method_t parse;
} css_rule_parser_t;

typedef struct css_style_parser_t {
	char *dirname; /**< 当前所在的目录 */
	char *space;   /**< 样式记录所属的空间 */
	char *property;

	void (*style_handler)(int, css_style_value_t *, void *);
	void *style_handler_arg;

	list_t selectors;        /**< 当前匹配到的选择器列表 */
	css_style_decl_t *style; /**< 当前缓存的样式表 */
} css_style_parser_t;

typedef struct css_comment_parser_t {
	/** 是否为单行注释 */
	libcss_bool_t is_line_comment;

	/** 保存的上一个目标，解析完注释后将还原成该目标 */
	css_parser_target_t prev_target;
} css_comment_parser_t;

/** CSS 代码解析器的环境参数（上下文数据） */
struct css_parser_t {
	int pos;         /**< 缓存中的字符串的下标位置 */
	const char *cur; /**< 用于遍历字符串的指针 */
	char *space;     /**< 样式记录所属的空间 */
	char *buffer;
	size_t buffer_size;

	css_parser_target_t target;
	css_comment_parser_t comment_parser;
	css_style_parser_t style_parser;
	css_rule_type_t rule;
	css_rule_parser_t rule_parsers[CSS_RULE_TOTAL_NUM];
};

// css parser

LIBCSS_INLINE void css_parser_get_char(css_parser_t *parser)
{
	parser->buffer[parser->pos++] = *(parser->cur);
}

LIBCSS_PUBLIC css_parser_t *css_parser_create(size_t buffer_size, const char *space);

LIBCSS_PUBLIC void css_parser_destroy(css_parser_t *parser);

LIBCSS_PUBLIC void css_parser_end_parse_rule(css_parser_t *parser);

LIBCSS_PUBLIC void css_parser_commit(css_parser_t *parser);

LIBCSS_PUBLIC size_t css_parser_parse(css_parser_t *parser, const char *str);

LIBCSS_PUBLIC int css_parser_begin_parse_comment(css_parser_t *parser);

// css style parser

LIBCSS_PUBLIC void css_style_parser_init(css_style_parser_t *parser,
				    const char *space);

LIBCSS_PUBLIC void css_style_parser_destroy(css_style_parser_t *parser);

LIBCSS_PUBLIC void css_style_parser_commit(css_style_parser_t *parser);

// css property parser

LIBCSS_PUBLIC void css_init_preset_property_parsers(void);

LIBCSS_PUBLIC void css_destroy_preset_property_parsers(void);

// css font face parser

LIBCSS_PUBLIC void css_font_face_parser_on_load(
    css_parser_t *ctx, void (*func)(const css_font_face_t *));

LIBCSS_PUBLIC int css_font_face_parser_init(css_parser_t *ctx);

LIBCSS_PUBLIC void css_font_face_parser_destroy(css_parser_t *ctx);

LIBCSS_END_DECLS

#endif
