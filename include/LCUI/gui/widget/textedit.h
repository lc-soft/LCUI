/* ***************************************************************************
 * textedit.c -- textedit widget, used to allow user edit text.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
 * textedit.c -- 文本编辑部件，用于让用户编辑文本。
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
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
#ifndef LCUI_TEXTBOX_H
#define LCUI_TEXTBOX_H

LCUI_BEGIN_HEADER

#define TextEdit_New() Widget_New("textedit")

/** 指定文本框是否处理控制符 */
LCUI_API void TextEdit_SetUsingStyleTags( LCUI_Widget widget, LCUI_BOOL is_true );

/** 指定文本框是否启用多行文本显示 */
LCUI_API void TextEdit_SetMultiline( LCUI_Widget widget, LCUI_BOOL is_true );

/** 清空文本内容 */
LCUI_API void TextEdit_ClearText( LCUI_Widget widget );

/** 获取文本内容 */
LCUI_API int TextEdit_GetTextW( LCUI_Widget widget, int start, int max_len, wchar_t *buf );

/** 获取文本长度 */
LCUI_API int TextEdit_GetTextLength( LCUI_Widget w );

/** 为文本框设置文本（宽字符版） */
LCUI_API int TextEdit_SetTextW( LCUI_Widget widget, const wchar_t *wstr );

LCUI_API int TextEdit_SetText( LCUI_Widget widget, const char *utf8_str );

/** 为文本框追加文本（宽字符版） */
LCUI_API int TextEdit_AppendTextW( LCUI_Widget widget, const wchar_t *wstr );

/** 为文本框插入文本（宽字符版） */
LCUI_API int TextEdit_InsertTextW( LCUI_Widget widget, const wchar_t *wstr );

/** 设置占位符，当文本编辑框内容为空时显示占位符 */
LCUI_API int TextEdit_SetPlaceHolderW( LCUI_Widget w, const wchar_t *wstr );

LCUI_API int TextEdit_SetPlaceHolder( LCUI_Widget w, const char *str );

LCUI_END_HEADER

#endif