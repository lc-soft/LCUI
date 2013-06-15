/* ****************************************************************************
 * LCUI_Build.h -- macro definition defines the location of some header files
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
 * LCUI_Build.h -- 定义了一些头文件的位置的宏定义
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

#ifndef __LCUI_BUILD_H__
#define __LCUI_BUILD_H__

#define _CRT_SECURE_NO_WARNINGS

#if defined(__GNUC__)
#  define LCUI_API 
#elif (defined(_MSC_VER) && _MSC_VER < 800) ||\
    (defined(__BORLANDC__) && __BORLANDC__ < 0x500)
  /* older Borland and MSC
   * compilers used '__export' and required this to be after
   * the type.
   */
#  define LCUI_API __export
#else /* newer compiler */
#  ifdef LCUI_EXPORTS
#    define LCUI_API __declspec(dllexport)
#  else
#    define LCUI_API 
#  endif
#endif /* compiler */

/* 自动模式，LCUI初始化时会自动选择一个模式 */
#define LCUI_INIT_MODE_AUTO		0

/* 全屏模式，使用整个屏幕进行图形显示 */
#define LCUI_INIT_MODE_FULLSCREEN	1

/* 窗口模式，LCUI的图形将输出至系统创建的窗口内 */
#define LCUI_INIT_MODE_WINDOW		2

/* LCUI模式，结合操作系统现有GUI系统，实现图形界面 */
#define LCUI_INIT_MODE_LCUI		3

#define LC_STYLE_LIBRARY_H	<LCUI/LCUI_StyleLibrary.h>
#define LC_INPUT_METHOD_H	<LCUI/LCUI_InputMethod.h>

#define LC_KERNEL_TIMER_H	<LCUI/kernel/timer.h>
#define LC_KERNEL_DEV_H		<LCUI/kernel/device.h>
#define LC_KERNEL_MAIN_H	<LCUI/kernel/main.h>
#define LC_KERNEL_EVENT_H	<LCUI/kernel/event.h>
#define LC_KERNEL_TASK_H	<LCUI/kernel/task.h>

#define LC_LCUI_H	<LCUI/LCUI.h>
#define LC_LCUI_HPP	<LCUI/LCUI.hpp>
#define LC_CONFIG_H	<LCUI/LCUI_Config.h>

#define LC_MISC_STRING_H	<LCUI/misc/string.h>
#define LC_MISC_RECT_H		<LCUI/misc/rect.h>
#define LC_MISC_DELAY_H		<LCUI/misc/delay.h>
#define LC_MISC_DIRENT_H	<LCUI/misc/dirent.h>
#define LC_MISC_H		<LCUI/LCUI_Misc.h>

#define LC_WIDGET_BASE_H	<LCUI/gui/widget_base.h>
#define LC_WIDGET_MSG_H		<LCUI/gui/widget_msg.h>
#define LC_WIDGET_EVENT_H	<LCUI/gui/widget_event.h>
#define LC_WIDGET_LIBRARY_H	<LCUI/gui/widget_library.h>
#define LC_WIDGET_STYLE_H	<LCUI/gui/widget_style.h>
#define LC_WIDGET_H		<LCUI/LCUI_Widget.h>
#define LC_WIDGET_HPP		<LCUI/LCUI_Widget.hpp>

#define LC_FONT_H	<LCUI/LCUI_Font.h>
#define LC_FONTLIB_H	<LCUI/font/fontlibrary.h>
#define LC_GRAPH_H	<LCUI/LCUI_Graph.h>
#define LC_GRAPH_HPP	<LCUI/LCUI_Graph.hpp>
#define LC_DRAW_H	<LCUI/LCUI_Draw.h>
#define LC_DISPLAY_H	<LCUI/LCUI_Display.h>
#define LC_QUEUE_H	<LCUI/LCUI_Queue.h>
#define LC_CURSOR_H	<LCUI/LCUI_Cursor.h>
#define LC_INPUT_H	<LCUI/LCUI_Input.h>
#define LC_ERROR_H	<LCUI/LCUI_Error.h>
#define LC_THREAD_H	<LCUI/LCUI_Thread.h>
#define LC_THREAD_HPP	<LCUI/LCUI_Thread.hpp>
#define LC_RES_H	<LCUI/LCUI_Resources.h>

#define LC_GRAPHLAYER_H		<LCUI/LCUI_GraphLayer.h>

#define LC_DRAW_BORDER_H	<LCUI/draw/border.h>
#define LC_DRAW_LINE_H		<LCUI/draw/line.h>

#define LC_CHARSET_H	<LCUI/font/charset.h>
#define LC_FONTBMP_H	<LCUI/font/bitmapfont.h>
#define LC_TEXTSTYLE_H	<LCUI/font/textstyle.h>
#define LC_TEXTLAYER_H	<LCUI/font/textlayer.h>

/* 一些GUI部件的头文件路径 */
#define LC_WINDOW_H	<LCUI/gui/widget/window.h>
#define LC_WINDOW_HPP	<LCUI/gui/widget/window.hpp>
#define LC_LABEL_H	<LCUI/gui/widget/label.h>
#define LC_LABEL_HPP	<LCUI/gui/widget/label.hpp>
#define LC_BUTTON_H	<LCUI/gui/widget/button.h>
#define LC_PICBOX_H	<LCUI/gui/widget/picturebox.h>
#define LC_PROGBAR_H	<LCUI/gui/widget/progressbar.h>
#define LC_MENU_H	<LCUI/gui/widget/menu.h>
#define LC_CHECKBOX_H	<LCUI/gui/widget/checkbox.h>
#define LC_RADIOBTN_H	<LCUI/gui/widget/radiobutton.h>
#define LC_ACTIVEBOX_H	<LCUI/gui/widget/activebox.h>
#define LC_TEXTBOX_H	<LCUI/gui/widget/textbox.h>
#define LC_SCROLLBAR_H	<LCUI/gui/widget/scrollbar.h>

#define LC_GUI_MSGBOX_H	<LCUI/gui/messagebox.h>


#ifdef DEBUG
#define DEBUG_MSG _DEBUG_MSG
#else
#define DEBUG_MSG(format, ...)
#endif

#ifdef DEBUG1
#define DEBUG_MSG1 _DEBUG_MSG
#else
#define DEBUG_MSG1(format, ...)
#endif

#ifdef DEBUG2
#define DEBUG_MSG2 _DEBUG_MSG
#else
#define DEBUG_MSG2(format, ...)
#endif

#define _DEBUG_MSG(format, ...) printf(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)

#if defined(WIN32) || defined(_WIN32)
#define LCUI_BUILD_IN_WIN32
#define LCUI_THREAD_WIN32
#define LCUI_VIDEO_DRIVER_WIN32
#define LCUI_FONT_ENGINE_FREETYPE
#define USE_LIBPNG
#define USE_LIBJPEG
#undef USE_TSLIB
#undef LCUI_THREAD_PTHREAD
#undef LCUI_VIDEO_DRIVER_FRAMEBUFFER
#else
#include LC_CONFIG_H
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define LCUI_BUILD_IN_LINUX
#define LCUI_KEYBOARD_DRIVER_LINUX
#define LCUI_MOUSE_DRIVER_LINUX
#endif

#ifdef __cplusplus
#define LCUI_BEGIN_HEADER  extern "C"{
#define LCUI_END_HEADER  }
#else
#define LCUI_BEGIN_HEADER  /* nothing */
#define LCUI_END_HEADER
#endif

#endif
