/* ***************************************************************************
 * textbox.h -- Function declaration for TextBox widget 
 * 
 * Copyright (C) 2013 by
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

#ifndef __LCUI_TEXTBOX_H__
#define __LCUI_TEXTBOX_H__

#include LC_FONT_H

LCUI_BEGIN_HEADER


LCUI_EXPORT(LCUI_Pos)
TextBox_ViewArea_GetPos( LCUI_Widget *widget );
/* 获取文本显示区域的位置 */

LCUI_EXPORT(int)
TextBox_ViewArea_Update( LCUI_Widget *widget );
/* 更新文本框的文本显示区域 */

/* 获取文本框部件内的label部件指针 */
LCUI_EXPORT(LCUI_Widget*)
TextBox_GetLabel( LCUI_Widget *widget );

/* 获取文本框部件内的光标 */
LCUI_EXPORT(LCUI_Widget*)
TextBox_GetCursor( LCUI_Widget *widget );

/* 获取文本框内文本 */
LCUI_EXPORT(size_t)
TextBox_GetText( LCUI_Widget *widget, wchar_t *buff, size_t max_len );

/* 获取文本框内的文本总长度 */
LCUI_EXPORT(int)
TextBox_Text_GetTotalLength( LCUI_Widget *widget );

/* 设定文本框显示的文本 */
LCUI_EXPORT(void)
TextBox_TextW( LCUI_Widget *widget, wchar_t *unicode_text );

LCUI_EXPORT(void)
TextBox_Text( LCUI_Widget *widget, char *utf8_text );

LCUI_EXPORT(void)
TextBox_TextLayer_SetOffset( LCUI_Widget *widget, LCUI_Pos offset_pos );
/* 为文本框内的文本图层设置偏移 */

/* 在光标处添加文本 */
LCUI_EXPORT(void)
TextBox_Text_AddW( LCUI_Widget *widget, wchar_t *unicode_text );

LCUI_EXPORT(void)
TextBox_Text_AddA( LCUI_Widget *widget, char *ascii_text );

LCUI_EXPORT(void)
TextBox_Text_Add( LCUI_Widget *widget, char *utf8_text );

/* 在文本末尾追加文本 */
LCUI_EXPORT(void)
TextBox_Text_AppendW( LCUI_Widget *widget, wchar_t *unicode_text );

/* 在文本末尾追加文本 */
LCUI_EXPORT(void)
TextBox_Text_Append( LCUI_Widget *widget, char *utf8_text );

LCUI_EXPORT(int)
TextBox_Text_Backspace(LCUI_Widget *widget, int n);
/* 删除光标左边处n个字符 */

LCUI_EXPORT(LCUI_Pos)
TextBox_Cursor_Update( LCUI_Widget *widget );
/* 更新文本框的光标，返回该光标的像素坐标 */

LCUI_EXPORT(LCUI_Pos)
TextBox_Cursor_Move( LCUI_Widget *widget, LCUI_Pos new_pos );
/* 移动文本框内的光标 */

LCUI_EXPORT(int)
TextBox_GetSelectedText( LCUI_Widget *widget, char *out_text );
/* 获取文本框内被选中的文本 */

LCUI_EXPORT(int)
TextBox_CopySelectedText(LCUI_Widget *widget);
/* 复制文本框内被选中的文本 */

LCUI_EXPORT(int)
TextBox_CutSelectedText(LCUI_Widget *widget);
/* 剪切文本框内被选中的文本 */

LCUI_EXPORT(void)
TextBox_Using_StyleTags(LCUI_Widget *widget, LCUI_BOOL flag);
/* 指定文本框是否处理控制符 */

LCUI_EXPORT(void)
TextBox_Multiline( LCUI_Widget *widget, LCUI_BOOL flag );
/* 指定文本框是否启用多行文本显示 */

LCUI_EXPORT(void)
TextBox_Text_SetDefaultStyle( LCUI_Widget *widget, LCUI_TextStyle style );
/* 设定文本框内的文本的缺省样式 */

LCUI_EXPORT(void)
TextBox_Text_SetMaxLength( LCUI_Widget *widget, int max );
/* 设置文本框中能够输入的最大字符数 */

LCUI_EXPORT(void)
TextBox_Text_SetPasswordChar( LCUI_Widget *widget, wchar_t ch );
/* 为文本框设置屏蔽字符 */

LCUI_EXPORT(void)
TextBox_Text_SetPlaceHolderW(	LCUI_Widget *widget, 
				LCUI_TextStyle *style,
				const wchar_t *str );

/* 为文本框设置占位符 */
LCUI_EXPORT(void)
TextBox_Text_SetPlaceHolder(	LCUI_Widget *widget, 
				LCUI_TextStyle *style,
				const char *str );


/* 
 * 功能：限制能对文本框输入的字符
 * 说明：str中存放限制范围内的字符，设置后，文本框只接受输入该字符串内的字符
 * */
LCUI_EXPORT(void)
TextBox_LimitInput( LCUI_Widget *widget, wchar_t *str );

LCUI_END_HEADER

#endif
