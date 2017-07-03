/* ***************************************************************************
 * css_parser.h -- css parser module
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
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

LCUI_BEGIN_HEADER

/** 样式的解析器 */
typedef struct LCUI_StyleParserRec_ {
	int key;
	char *name;
	int (*parse)(LCUI_StyleSheet, int, const char*);
} LCUI_StyleParserRec, *LCUI_StyleParser;

LCUI_API int LCUI_GetStyleValue( const char *str );

LCUI_API const char *LCUI_GetStyleValueName( int val );

LCUI_API const char *LCUI_GetStyleName( int key );

/** 初始化 LCUI 的 CSS 代码解析功能 */
LCUI_API void LCUI_InitCSSParser( void );

/** 从文件中载入CSS样式数据，并导入至样式库中 */
LCUI_API int LCUI_LoadCSSFile( const char *filepath );

/** 从字符串中载入CSS样式数据，并导入至样式库中 */
LCUI_API int LCUI_LoadCSSString( const char *str, const char *space );

LCUI_API void LCUI_FreeCSSParser(void);

/** 注册新的属性和对应的属性值解析器 */
LCUI_API int LCUI_AddCSSParser( LCUI_StyleParser sp );

LCUI_END_HEADER

#endif
