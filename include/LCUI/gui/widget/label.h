/* ***************************************************************************
 * label.h -- LCUI's Label widget
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
 * label.h -- LCUI 的文本标签部件
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
#ifndef __LCUI_LABEL_H__
#define __LCUI_LABEL_H__

#include LC_FONT_H

LCUI_BEGIN_HEADER

/** 设定与标签关联的文本内容 */
LCUI_API int Label_SetTextW( LCUI_Widget *widget, const wchar_t *text );

LCUI_API int Label_SetText( LCUI_Widget *widget, const char *utf8_text );

LCUI_API int Label_SetTextA( LCUI_Widget *widget, const char *ansi_text );

/** 设置Label部件显示的文本是否自动换行 */
LCUI_API void Label_SetAutoWrap( LCUI_Widget *widget, LCUI_BOOL flag );

/** 设置文本对齐方式 */
LCUI_API void Label_SetTextAlign( LCUI_Widget *widget, TextAlignType align );

/** 为Label部件内显示的文本设定文本样式 */
LCUI_API int Label_SetTextStyle( LCUI_Widget *widget, LCUI_TextStyle style );

/** 获取Label部件的全局文本样式 */
LCUI_API void Label_GetTextStyle( LCUI_Widget *widget, LCUI_TextStyle *style );

/** 刷新label部件显示的文本 */
LCUI_API void Label_Refresh( LCUI_Widget *widget );

/** 启用或禁用Label部件的自动尺寸调整功能 */
LCUI_API void Label_SetAutoSize( LCUI_Widget *widget, LCUI_BOOL flag,
							AUTOSIZE_MODE mode );
/** 注册label部件类型 */
LCUI_API void Register_Label(void);

LCUI_END_HEADER

#endif
