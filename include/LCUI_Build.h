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

/* 一些部件的头文件路径 */
#define LC_WINDOW_H	<LCUI/LCUI_Window.h>
#define LC_WINDOW_HPP	<LCUI/LCUI_Window.hpp>
#define LC_LABEL_H	<LCUI/LCUI_Label.h>
#define LC_LABEL_HPP	<LCUI/LCUI_Label.hpp>
#define LC_BUTTON_H	<LCUI/LCUI_Button.h>
#define LC_PICBOX_H	<LCUI/LCUI_PictureBox.h>
#define LC_PROGBAR_H	<LCUI/LCUI_ProgressBar.h>
#define LC_MENU_H	<LCUI/LCUI_Menu.h>
#define LC_CHECKBOX_H	<LCUI/LCUI_CheckBox.h>
#define LC_RADIOBTN_H	<LCUI/LCUI_RadioButton.h>
#define LC_ACTIVEBOX_H	<LCUI/LCUI_ActiveBox.h> 
#define LC_TEXTBOX_H	<LCUI/LCUI_TextBox.h>
#endif



#ifdef DEBUG
#define DEBUG_MSG(format, ...) printf(__FILE__" %s(): %d:"format, __FUNCTION__, __LINE__,##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) 
#endif
