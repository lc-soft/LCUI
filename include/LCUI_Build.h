/* ****************************************************************************
 * LCUI_Build.h -- macro definition defines the location of some header files
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Build.h -- 定义了一些头文件的位置的宏定义
 *
 * 版权所有 (C) 2012 归属于 
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef __cplusplus
#define LCUI_BEGIN_HEADER  extern "C"{
#define LCUI_END_HEADER  }
#else
#define LCUI_BEGIN_HEADER  /* nothing */
#define LCUI_END_HEADER
#endif

#ifndef __LCUI_BUILD_UNIX_H__
#define __LCUI_BUILD_UNIX_H__

#define LC_STYLE_LIBRARY_H	<LCUI/LCUI_StyleLibrary.h>

#define LC_KERNEL_TIMER_H	<LCUI/kernel/timer.h>
#define LC_KERNEL_DEV_H		<LCUI/kernel/device.h>
#define LC_KERNEL_MAIN_H	<LCUI/kernel/main.h>

#define LC_LCUI_H	<LCUI/LCUI.h>
#define LC_LCUI_HPP	<LCUI/LCUI.hpp>

#define LC_MISC_H	<LCUI/LCUI_Misc.h>
#define LC_FONT_H	<LCUI/LCUI_Font.h>
#define LC_GRAPH_H	<LCUI/LCUI_Graph.h>
#define LC_GRAPH_HPP	<LCUI/LCUI_Graph.hpp>
#define LC_DRAW_H	<LCUI/LCUI_Draw.h>
#define LC_DISPLAY_H	<LCUI/LCUI_Display.h>
#define LC_WIDGET_H	<LCUI/LCUI_Widget.h>
#define LC_WIDGET_HPP	<LCUI/LCUI_Widget.hpp>
#define LC_QUEUE_H	<LCUI/LCUI_Queue.h> 
#define LC_CURSOR_H	<LCUI/LCUI_Cursor.h> 
#define LC_WORK_H	<LCUI/LCUI_Work.h> 
#define LC_INPUT_H	<LCUI/LCUI_Input.h>
#define LC_MEM_H	<LCUI/LCUI_Memory.h>
#define LC_ERROR_H	<LCUI/LCUI_Error.h>
#define LC_THREAD_H	<LCUI/LCUI_Thread.h> 
#define LC_THREAD_HPP	<LCUI/LCUI_Thread.hpp> 
#define LC_RES_H	<LCUI/LCUI_Resources.h> 

#define LC_GRAPHLAYER_H		<LCUI/LCUI_GraphLayer.h>

#define LC_DRAW_BORDER_H	<LCUI/draw/border.h>

#define LC_CHARSET_H	<LCUI/font/charset.h>
#define LC_FONTBMP_H	<LCUI/font/bitmapfont.h>
#define LC_TEXTSTYLE_H	<LCUI/font/textstyle.h>
#define LC_TEXTLAYER_H	<LCUI/font/textlayer.h>

/* 一些部件的头文件路径 */
#define LC_WINDOW_H	<LCUI/widget/window.h>
#define LC_WINDOW_HPP	<LCUI/widget/window.hpp>
#define LC_LABEL_H	<LCUI/widget/label.h>
#define LC_LABEL_HPP	<LCUI/widget/label.hpp>
#define LC_BUTTON_H	<LCUI/widget/button.h>
#define LC_PICBOX_H	<LCUI/widget/picturebox.h>
#define LC_PROGBAR_H	<LCUI/widget/progressbar.h>
#define LC_MENU_H	<LCUI/widget/menu.h>
#define LC_CHECKBOX_H	<LCUI/widget/checkbox.h>
#define LC_RADIOBTN_H	<LCUI/widget/radiobutton.h>
#define LC_ACTIVEBOX_H	<LCUI/widget/activebox.h> 
#define LC_TEXTBOX_H	<LCUI/widget/textbox.h>
#define LC_SCROLLBAR_H	<LCUI/widget/scrollbar.h>
#endif

#define _DEBUG_MSG(format, ...) printf(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)

#ifdef DEBUG
#define DEBUG_MSG(format, ...) printf(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) 
#endif

#ifdef DEBUG1
#define DEBUG_MSG1(format, ...) printf(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define DEBUG_MSG1(format, ...) 
#endif

#ifdef DEBUG2
#define DEBUG_MSG2(format, ...) printf(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define DEBUG_MSG2(format, ...) 
#endif

#ifdef DEBUG3
#define DEBUG_MSG3(format, ...) printf(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define DEBUG_MSG3(format, ...) 
#endif
