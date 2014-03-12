/* ***************************************************************************
 * textstyle.h -- text style processing module.
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * 版权所有 (C) 2012-2014 归属于
 * 刘超
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

typedef enum font_style { 
	FONT_STYLE_NORMAL = 0, 
	FONT_STYLE_ITALIC = 1, 
	FONT_STYLE_OBIQUE = 2 
};

typedef enum font_weight {
	FONT_WEIGHT_NORMAL	= 0,
	FONT_WEIGHT_BOLD	= 1 
};

typedef enum font_decoration {
	FONT_DECORATION_NONE		= 0,	/* 无装饰 */
	FONT_DECORATION_BLINK		= 1,	/* 闪烁 */
	FONT_DECORATION_UNDERLINE	= 2,	/* 下划线 */
	FONT_DECORATION_LINE_THROUGH	= 3,	/* 贯穿线 */
	FONT_DECORATION_OVERLINE	= 4	/* 上划线 */
};

typedef struct LCUI_TextStyle_ {
	LCUI_BOOL _family:1;
	LCUI_BOOL _style:1;
	LCUI_BOOL _weight:1;
	LCUI_BOOL _decoration:1;
	LCUI_BOOL _back_color:1;
	LCUI_BOOL _fore_color:1;
	LCUI_BOOL _pixel_size:1;
	
	int font_id;
	enum font_style	style:3;
	enum font_weight weight:3;
	enum font_decoration decoration:4;
	
	LCUI_RGB fore_color;
	LCUI_RGB back_color;
	
	int pixel_size;	
} LCUI_TextStyle;

typedef enum {
	TAG_ID_FAMILY = 0,
	TAG_ID_STYLE = 1,
	TAG_ID_WIEGHT = 2,
	TAG_ID_DECORATION = 3,
	TAG_ID_SIZE = 4,
	TAG_ID_COLOR = 5
} LCUI_StyleTagID;

typedef struct LCUI_StyleTagData_ {
	LCUI_StyleTagID tag_id;
	union {
		LCUI_RGB color;
		PixelOrPoint_t size;
	} style;
} LCUI_StyleTagData;

typedef LCUI_Queue LCUI_StyleTagStack;

/* 初始化字体样式数据 */
LCUI_API void TextStyle_Init ( LCUI_TextStyle *data );

/* 设置字体族 */
LCUI_API void TextStyle_FontFamily( LCUI_TextStyle *style, const char *fontfamily );

/* 设置字体族ID */
LCUI_API void TextStyle_FontFamilyID( LCUI_TextStyle *style, int id );

/* 设置字体大小 */
LCUI_API void TextStyle_FontSize( LCUI_TextStyle *style, int fontsize );

/* 设置字体颜色 */
LCUI_API void TextStyle_FontColor( LCUI_TextStyle *style, LCUI_RGB color );

/* 设置字体背景颜色 */
LCUI_API void TextStyle_FontBackColor( LCUI_TextStyle *style, LCUI_RGB color );

/* 设置字体样式 */
LCUI_API void TextStyle_FontStyle( LCUI_TextStyle *style, enum font_style fontstyle );

LCUI_API void TextStyle_FontWeight( LCUI_TextStyle *style, enum font_weight fontweight );

/* 设置字体下划线 */
LCUI_API void TextStyle_FontDecoration( LCUI_TextStyle *style, enum font_decoration decoration );

LCUI_API int TextStyle_Cmp( LCUI_TextStyle *a, LCUI_TextStyle *b );

/*-------------------------- StyleTag --------------------------------*/
#define MAX_TAG_NUM 2

/** 初始化样式标签栈 */
LCUI_API void StyleTagStack_Init( LCUI_StyleTagStack *tags );

/** 销毁样式标签栈 */
LCUI_API void StyleTagStack_Destroy( LCUI_StyleTagStack *tags );

/** 获取当前的文本样式 */
LCUI_API LCUI_TextStyle* StyleTagStack_GetTextStyle( LCUI_StyleTagStack *tags );

/** 在字符串中获取样式的结束标签，输出的是标签名 */
LCUI_API const wchar_t* scan_style_ending_tag( const wchar_t *wstr, 
						char *name_buff );

/** 从字符串中获取样式标签的名字及样式属性 */
LCUI_API const wchar_t* scan_style_tag(	const wchar_t *wstr, char *name_buff,
					int max_name_len, char *data_buff );

/** 处理样式标签 */
LCUI_API const wchar_t* StyleTagStack_ScanBeginTag( LCUI_StyleTagStack *tags,
						const wchar_t *str );

/** 处理样式结束标签 */
LCUI_API const wchar_t* StyleTagStack_ScanEndingTag( LCUI_StyleTagStack *tags,
							const wchar_t *str );

/*------------------------- End StyleTag -----------------------------*/

LCUI_END_HEADER

#endif
