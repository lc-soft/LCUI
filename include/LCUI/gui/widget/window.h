/* ***************************************************************************
 * window.h -- LCUI's window widget
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
 * window.h -- LCUI 的窗口部件
 *
 * 版权所有 (C) 2012-2013 归属于
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


#ifndef __LCUI_WINDOW_H_
#define __LCUI_WINDOW_H_ 

#define WIDGET_WINDOW "LCUI::Window"
#define Window_New() Widget_New(WIDGET_WINDOW)

LCUI_BEGIN_HEADER

/** 获取窗口的客户区 */
LCUI_API LCUI_Widget *Window_GetClientArea( LCUI_Widget *window );

/** 获取窗口的close按钮 */
LCUI_API LCUI_Widget *Window_GetCloseButton( LCUI_Widget *window );

/** 将部件添加至窗口客户区内 */
LCUI_API void Window_ClientArea_Add( LCUI_Widget *window, LCUI_Widget *w );

/** 设置窗口标题栏中显示的文本 */
LCUI_API int Window_SetTextW( LCUI_Widget *window, const wchar_t *text );

/** 设置窗口的图标 */
LCUI_API void Window_SetIcon( LCUI_Widget *window, LCUI_Graph *icon );

LCUI_END_HEADER

#endif
