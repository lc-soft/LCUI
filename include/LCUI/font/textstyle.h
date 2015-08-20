/* ***************************************************************************
 * textstyle.h -- text style processing module.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * textstyle.h -- 文本样式处理模块
 *
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_TEXTSTYLE_H__
#define __LCUI_TEXTSTYLE_H__

LCUI_BEGIN_HEADER

/** 下面三种字体样式虽然有定义，但还未添加能够实现该字体样式效果的功能 */

enum FontStyle {
	FONT_STYLE_NORMAL = 0,
	FONT_STYLE_ITALIC = 1,
	FONT_STYLE_OBIQUE = 2
};

enum FontWeight {
	FONT_WEIGHT_NORMAL	= 0,
	FONT_WEIGHT_BOLD	= 1
};

enum FontDecoration {
	FONT_DECORATION_NONE		= 0,	/* 无装饰 */
	FONT_DECORATION_BLINK		= 1,	/* 闪烁 */
	FONT_DECORATION_UNDERLINE	= 2,	/* 下划线 */
	FONT_DECORATION_LINE_THROUGH	= 3,	/* 贯穿线 */
	FONT_DECORATION_OVERLINE	= 4	/* 上划线 */
};

typedef struct LCUI_TextStyle {
	LCUI_BOOL has_family:1;
	LCUI_BOOL has_style:1;
	LCUI_BOOL has_weight:1;
	LCUI_BOOL has_decoration:1;
	LCUI_BOOL has_back_color:1;
	LCUI_BOOL has_fore_color:1;
	LCUI_BOOL has_pixel_size:1;

	int font_id;
	int style:3;
	int weight:3;
	int decoration:4;

	LCUI_Color fore_color;
	LCUI_Color back_color;

	int pixel_size;
} LCUI_TextStyle;

/* 初始化字体样式数据 */
LCUI_API void TextStyle_Init ( LCUI_TextStyle *data );

/*-------------------------- StyleTag --------------------------------*/
#define MAX_TAG_NUM 2

/** 初始化样式标签栈 */
LCUI_API void StyleTags_Init( LinkedList *tags );

/** 销毁样式标签栈 */
LCUI_API void StyleTags_Destroy( LinkedList *tags );

/** 获取当前的文本样式 */
LCUI_API LCUI_TextStyle* StyleTags_GetTextStyle( LinkedList *tags );

/** 在字符串中获取样式的结束标签，输出的是标签名 */
LCUI_API const wchar_t* scan_style_ending_tag( const wchar_t *wstr,
						char *name_buff );

/** 从字符串中获取样式标签的名字及样式属性 */
LCUI_API const wchar_t* scan_style_tag(	const wchar_t *wstr, char *name_buff,
					int max_name_len, char *data_buff );

/** 处理样式标签 */
LCUI_API const wchar_t* StyleTags_ScanBeginTag( LinkedList *tags,
						const wchar_t *str );

/** 处理样式结束标签 */
LCUI_API const wchar_t* StyleTags_ScanEndingTag( LinkedList *tags,
							const wchar_t *str );

/*------------------------- End StyleTag -----------------------------*/

LCUI_END_HEADER

#endif
