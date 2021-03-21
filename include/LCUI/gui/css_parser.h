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

#ifndef LCUI_CSS_PARSER_H
#define LCUI_CSS_PARSER_H

#include <LCUI/font/fontlibrary.h>

LCUI_BEGIN_HEADER

#define CASE_WHITE_SPACE \
	case ' ':        \
	case '\n':       \
	case '\r':       \
	case '\t'

#define CSSParser_GetChar(CTX)                               \
	do {                                                 \
		(CTX)->buffer[(ctx)->pos++] = *((CTX)->cur); \
	} while (0);

typedef enum LCUI_CSSParserTarget {
	CSS_TARGET_NONE,      /**< 无 */
	CSS_TARGET_RULE_NAME, /**< 规则名称 */
	CSS_TARGET_RULE_DATA, /**< 规则数据 */
	CSS_TARGET_SELECTOR,  /**< 选择器 */
	CSS_TARGET_KEY,       /**< 属性名 */
	CSS_TARGET_VALUE,     /**< 属性值 */
	CSS_TARGET_COMMENT,   /**< 注释 */
	CSS_TARGET_TOTAL_NUM
} LCUI_CSSParserTarget;

typedef enum LCUI_CSSRule {
	CSS_RULE_NONE,
	CSS_RULE_FONT_FACE, /**< @font-face */
	CSS_RULE_IMPORT,    /**< @import */
	CSS_RULE_MEDIA,     /**< @media */
	CSS_RULE_TOTAL_NUM
} LCUI_CSSRule;

struct LCUI_CSSParserRec_;
struct LCUI_CSSRuleParserRec_;
struct LCUI_CSSParserContextRec_;
struct LCUI_CSSParserStyleContextRec_;
struct LCUI_CSSPropertyParserRec_;

typedef struct LCUI_CSSRuleParserRec_ LCUI_CSSRuleParserRec;
typedef struct LCUI_CSSRuleParserRec_ *LCUI_CSSRuleParser;
typedef struct LCUI_CSSParserContextRec_ LCUI_CSSParserContextRec;
typedef struct LCUI_CSSParserContextRec_ *LCUI_CSSParserContext;
typedef struct LCUI_CSSParserStyleContextRec_ LCUI_CSSParserStyleContextRec;
typedef struct LCUI_CSSParserStyleContextRec_ *LCUI_CSSParserStyleContext;
typedef struct LCUI_CSSParserRec_ *LCUI_CSSParser;
typedef struct LCUI_CSSParserRec_ LCUI_CSSParserRec;
typedef struct LCUI_CSSPropertyParserRec_ *LCUI_CSSPropertyParser;
typedef struct LCUI_CSSPropertyParserRec_ LCUI_CSSPropertyParserRec;
typedef struct LCUI_CSSParserCommentContextRec_ LCUI_CSSParserCommentContextRec;
typedef struct LCUI_CSSParserCommentContextRec_ *LCUI_CSSParserCommentContext;
typedef struct LCUI_CSSParserRuleContextRec_ LCUI_CSSParserRuleContextRec;
typedef struct LCUI_CSSParserRuleContextRec_ *LCUI_CSSParserRuleContext;
typedef int (*LCUI_CSSParserFunction)(LCUI_CSSParserContext ctx);

struct LCUI_CSSParserRec_ {
	LCUI_CSSParserFunction parse;
};

struct LCUI_CSSRuleParserRec_ {
	char name[32];
	void *data;
	LCUI_CSSParserFunction begin;
	LCUI_CSSParserFunction parse;
};

typedef LCUI_CSSParserRec LCUI_CSSParsers[CSS_TARGET_TOTAL_NUM];
typedef LCUI_CSSRuleParserRec LCUI_CSSRuleParsers[CSS_RULE_TOTAL_NUM];

/** 样式属性的解析器 */
struct LCUI_CSSPropertyParserRec_ {
	int key; /**< 标识，在解析数据时可以使用它访问样式表中的自定义属性 */
	char *name; /**< 名称，对应 CSS 样式属性名称 */
	int (*parse)(LCUI_CSSParserStyleContext, const char *);
};

struct LCUI_CSSParserStyleContextRec_ {
	char *dirname;     /**< 当前所在的目录 */
	const char *space; /**< 样式记录所属的空间 */

	void (*style_handler)(int, LCUI_Style, void *);
	void *style_handler_arg;

	LinkedList selectors;          /**< 当前匹配到的选择器列表 */
	LCUI_StyleSheet sheet;         /**< 当前缓存的样式表 */
	LCUI_CSSPropertyParser parser; /**< 当前找到的样式属性解析器 */
};

struct LCUI_CSSParserCommentContextRec_ {
	LCUI_BOOL is_line_comment; /**< 是否为单行注释 */
	LCUI_CSSParserTarget
	    prev_target; /**< 保存的上一个目标，解析完注释后将还原成该目标 */
};

struct LCUI_CSSParserRuleContextRec_ {
	int state;                   /**< 规则解析器的状态 */
	LCUI_CSSRule rule;           /**< 当前规则 */
	LCUI_CSSRuleParsers parsers; /**< 规则解析器列表 */
};

/** CSS 代码解析器的环境参数（上下文数据） */
struct LCUI_CSSParserContextRec_ {
	int pos;            /**< 缓存中的字符串的下标位置 */
	const char *cur;    /**< 用于遍历字符串的指针 */
	char *space;        /**< 样式记录所属的空间 */
	char *buffer;       /**< 缓存中的字符串 */
	size_t buffer_size; /**< 缓存区大小 */

	LCUI_CSSParserTarget target; /**< 当前解析目标 */
	LCUI_CSSParsers parsers;     /**< 可供使用的解析器列表 */

	LCUI_CSSParserRuleContextRec rule;
	LCUI_CSSParserStyleContextRec style;
	LCUI_CSSParserCommentContextRec comment;
};

LCUI_API int LCUI_GetStyleValue(const char *str);

LCUI_API const char *LCUI_GetStyleValueName(int val);

LCUI_API const char *LCUI_GetStyleName(int key);

/** 初始化 LCUI 的 CSS 代码解析功能 */
LCUI_API void LCUI_InitCSSParser(void);

LCUI_API void CSSStyleParser_SetCSSProperty(LCUI_CSSParserStyleContext ctx,
					    int key, LCUI_Style s);

LCUI_API LCUI_CSSPropertyParser LCUI_GetCSSPropertyParser(const char *name);

/** 从文件中载入CSS样式数据，并导入至样式库中 */
LCUI_API int LCUI_LoadCSSFile(const char *filepath);

/** 从字符串中载入CSS样式数据，并导入至样式库中 */
LCUI_API size_t LCUI_LoadCSSString(const char *str, const char *space);

LCUI_API LCUI_CSSParserContext CSSParser_Begin(size_t buffer_size,
					       const char *space);

LCUI_API void CSSParser_EndParseRuleData(LCUI_CSSParserContext ctx);

LCUI_API void CSSParser_EndBuffer(LCUI_CSSParserContext ctx);

LCUI_API void CSSParser_End(LCUI_CSSParserContext ctx);

LCUI_API int CSSParser_BeginParseComment(LCUI_CSSParserContext ctx);

LCUI_API void LCUI_FreeCSSParser(void);

/** 注册新的属性和对应的属性值解析器 */
LCUI_API int LCUI_AddCSSPropertyParser(LCUI_CSSPropertyParser sp);

#include <LCUI/gui/css_rule_font_face.h>

LCUI_END_HEADER

#endif
