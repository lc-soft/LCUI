/* ***************************************************************************
 * textbox.h -- Function declaration for TextBox widget 
 * 
 * Copyright (C) 2012-2014 by
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
 * textbox.h -- 一些文本框部件的操作函数的声明
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

#ifndef __LCUI_TEXTBOX_H__
#define __LCUI_TEXTBOX_H__

LCUI_BEGIN_HEADER
	
#define WIDGET_TEXT_BOX "LCUI::TextBox"
#define TextBox_New() Widget_New(WIDGET_TEXT_BOX)

LCUI_API void RegisterTextBox( void );

/** 指定文本框是否处理控制符 */
LCUI_API void TextBox_SetUsingStyleTags( LCUI_Widget *widget, LCUI_BOOL is_true );

/** 指定文本框是否启用多行文本显示 */
LCUI_API void TextBox_SetMultiline( LCUI_Widget *widget, LCUI_BOOL is_true );

/** 清空文本内容 */
LCUI_API void TextBox_ClearText( LCUI_Widget *widget );

/** 为文本框设置文本（宽字符版） */
LCUI_API int TextBox_SetTextW( LCUI_Widget *widget, const wchar_t *wstr );

/** 为文本框追加文本（宽字符版） */
LCUI_API int TextBox_AppendTextW( LCUI_Widget *widget, const wchar_t *wstr );

/** 为文本框插入文本（宽字符版） */
LCUI_API int TextBox_InsertTextW( LCUI_Widget *widget, const wchar_t *wstr );

LCUI_END_HEADER

#endif
