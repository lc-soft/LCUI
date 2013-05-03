/* ***************************************************************************
 * label.h -- LCUI's Label widget
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
 * label.h -- LCUI 的文本标签部件
 *
 * 版权所有 (C) 2013 归属于
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
#ifndef __LCUI_LABEL_H__
#define __LCUI_LABEL_H__

#include LC_FONT_H

LCUI_BEGIN_HEADER

typedef struct _LCUI_Label 
{
	LCUI_BOOL auto_size;	/* 指定是否根据文本图层的尺寸来调整部件尺寸 */
	AUTOSIZE_MODE mode;	/* 自动尺寸调整的模式 */
	wchar_t *text_buff;	/* 文本块缓冲 */
	LCUI_TextLayer layer;	/* 文本图层 */
}
LCUI_Label;

/* 设定与标签关联的文本内容 */
LCUI_API int
Label_TextW( LCUI_Widget *widget, const wchar_t *unicode_text );

LCUI_API void
Label_Text( LCUI_Widget *widget, const char *utf8_text );

LCUI_API void
Label_TextA( LCUI_Widget *widget, const char *ascii_text );

/* 为Label部件内显示的文本设定文本样式 */
LCUI_API int
Label_TextStyle( LCUI_Widget *widget, LCUI_TextStyle style );

/* 获取Label部件的文本样式 */
LCUI_API LCUI_TextStyle
Label_GetTextStyle( LCUI_Widget *widget );

/* 获取label部件内的文本图层的指针 */
LCUI_API LCUI_TextLayer*
Label_GetTextLayer( LCUI_Widget *widget );

/* 刷新label部件显示的文本 */
LCUI_API void
Label_Refresh( LCUI_Widget *widget );

/* 启用或禁用Label部件的自动尺寸调整功能 */
LCUI_API void
Label_AutoSize( LCUI_Widget *widget, LCUI_BOOL flag, AUTOSIZE_MODE mode );
LCUI_END_HEADER

#ifdef __cplusplus
#include LC_LABEL_HPP
#endif

#endif
