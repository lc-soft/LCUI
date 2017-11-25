/* ***************************************************************************
 * css_parser.h -- css parser module
 *
 * Copyright (C) 2015-2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * css_parser.h -- css 样式解析模块
 *
 * 版权所有 (C) 2015-2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#ifndef LCUI_CSS_PARSER_H
#define LCUI_CSS_PARSER_H

#include <LCUI/font/textstyle.h>

LCUI_BEGIN_HEADER

#define CASE_WHITE_SPACE \
	case ' ':\
	case '\n':\
	case '\r':\
	case '\t'

#define CSSParser_GetChar( CTX ) do {\
	(CTX)->buffer[(ctx)->pos++] = *((CTX)->cur);\
} while( 0 );

#define CSSParser_EndBuffer( CTX ) do {\
	(CTX)->buffer[(CTX)->pos] = 0;\
	(CTX)->pos = 0;\
} while( 0 );

#define CSSParser_SetStyleParser( CTX, NAME ) do {\
	(CTX)->style.parser = Dict_FetchValue( self.parsers, NAME ); \
} while( 0 );

#define CSSParser_GetRuleParser(CTX) &ctx->rule.parsers[CSS_RULE_FONT_FACE]

typedef enum LCUI_CSSParserTarget {
	CSS_TARGET_NONE,	/**< 无 */
	CSS_TARGET_RULE_NAME,	/**< 规则名称 */
	CSS_TARGET_RULE_DATA,	/**< 规则数据 */
	CSS_TARGET_SELECTOR,	/**< 选择器 */
	CSS_TARGET_KEY,		/**< 属性名 */
	CSS_TARGET_VALUE,	/**< 属性值 */
	CSS_TARGET_COMMENT,	/**< 注释 */
	CSS_TARGET_TOTAL_NUM
} LCUI_CSSParserTarget;

typedef enum LCUI_CSSRule {
	CSS_RULE_NONE,
	CSS_RULE_FONT_FACE,	/**< @font-face */
	CSS_RULE_IMPORT,	/**< @import */
	CSS_RULE_MEDIA,		/**< @media */
	CSS_RULE_TOTAL_NUM
} LCUI_CSSRule;

typedef int( *LCUI_CSSParserFunction )(struct LCUI_CSSParserContextRec_ *ctx);

typedef struct LCUI_CSSParserRec_ {
	LCUI_CSSParserFunction parse;
} LCUI_CSSParserRec, *LCUI_CSSParser;

typedef struct LCUI_CSSRuleParserRec_ {
	char name[32];
	void *data;
	LCUI_CSSParserFunction begin;
	LCUI_CSSParserFunction parse;
} LCUI_CSSRuleParserRec, *LCUI_CSSRuleParser;

typedef LCUI_CSSParserRec LCUI_CSSParsers[CSS_TARGET_TOTAL_NUM];
typedef LCUI_CSSRuleParserRec LCUI_CSSRuleParsers[CSS_RULE_TOTAL_NUM];

/** 样式的解析器 */
typedef struct LCUI_StyleParserRec_ {
	int key;
	char *name;
	int( *parse )(LCUI_StyleSheet, int, const char*);
} LCUI_StyleParserRec, *LCUI_StyleParser;

typedef struct LCUI_CSSParserStyleContext_ {
	LinkedList selectors;		/**< 当前匹配到的选择器列表 */
	LCUI_StyleSheet sheet;		/**< 当前缓存的样式表 */
	LCUI_StyleParser parser;	/**< 当前找到的样式属性解析器 */
} LCUI_CSSParserStyleContext;

typedef struct LCUI_CSSParserCommentContext_ {
	LCUI_BOOL is_line_comment;		/**< 是否为单行注释 */
	LCUI_CSSParserTarget prev_target;	/**< 保存的上一个目标，解析完注释后将还原成该目标 */
} LCUI_CSSParserCommentContext;

typedef struct LCUI_CSSParserRuleContext_ {
	int state;			/**< 规则解析器的状态 */
	LCUI_CSSRule rule;		/**< 当前规则 */
	LCUI_CSSRuleParsers parsers;	/**< 规则解析器列表 */
} LCUI_CSSParserRuleContext;

/** CSS 代码解析器的环境参数（上下文数据） */
typedef struct LCUI_CSSParserContextRec_ {
	int pos;			/**< 缓存中的字符串的下标位置 */
	const char *cur;		/**< 用于遍历字符串的指针 */
	char *space;			/**< 样式记录所属的空间 */
	char *buffer;			/**< 缓存中的字符串 */
	size_t buffer_size;		/**< 缓存区大小 */

	LCUI_CSSParserTarget target;		/**< 当前解析目标 */
	LCUI_CSSParsers parsers;		/**< 可供使用的解析器列表 */

	LCUI_CSSParserStyleContext style;
	LCUI_CSSParserCommentContext comment;
	LCUI_CSSParserRuleContext rule;
} LCUI_CSSParserContextRec, *LCUI_CSSParserContext;

LCUI_API int LCUI_GetStyleValue( const char *str );

LCUI_API const char *LCUI_GetStyleValueName( int val );

LCUI_API const char *LCUI_GetStyleName( int key );

LCUI_API int CSSValueParser_ParseUrl( const char *str, char *out_url );

/** 初始化 LCUI 的 CSS 代码解析功能 */
LCUI_API void LCUI_InitCSSParser( void );

/** 从文件中载入CSS样式数据，并导入至样式库中 */
LCUI_API int LCUI_LoadCSSFile( const char *filepath );

/** 从字符串中载入CSS样式数据，并导入至样式库中 */
LCUI_API int LCUI_LoadCSSString( const char *str, const char *space );

LCUI_API LCUI_CSSParserContext CSSParser_Begin( size_t buffer_size, const char *space );

LCUI_API void CSSParser_EndParseRuleData( LCUI_CSSParserContext ctx );

LCUI_API void CSSParser_End( LCUI_CSSParserContext ctx );

LCUI_API int CSSParser_BeginParseComment( LCUI_CSSParserContext ctx );

LCUI_API void LCUI_FreeCSSParser(void);

/** 注册新的属性和对应的属性值解析器 */
LCUI_API int LCUI_AddCSSStyleParser( LCUI_StyleParser sp );

#include <LCUI/gui/css_rule_font_face.h>

LCUI_END_HEADER

#endif
