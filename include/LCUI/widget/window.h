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


#ifndef __LCUI_WINDOW_H_
#define __LCUI_WINDOW_H_ 

LCUI_BEGIN_HEADER

/*-------------- 窗口风格 ----------------*/
typedef enum WINDOW_STYLE
{
	WINDOW_STYLE_NONE,
	WINDOW_STYLE_STANDARD,
	WINDOW_STYLE_LINE,
	WINDOW_STYLE_PURE_BLUE,
	WINDOW_STYLE_PURE_GREEN,
	WINDOW_STYLE_PURE_RED,
	WINDOW_STYLE_PURE_ORANGE,
	WINDOW_STYLE_PURE_PURPLE 
}
WINDOW_STYLE;
/*--------------- END --------------------*/

/******************* 窗口标题栏 ***********************/
typedef struct _LCUI_TitleBar
{
	LCUI_Widget *icon_box;		/* 图标 */
	LCUI_Widget *label;		/* 标题栏中显示的文本 */
}
LCUI_TitleBar;
/****************************************************/

/************************ 窗口数据 **************************/
typedef struct _LCUI_Window
{
	LCUI_Widget *titlebar;		/* 标题栏 */
	LCUI_Widget *client_area;	/* 客户区 */
	LCUI_Widget *btn_close;	/* 关闭按钮 */
}
LCUI_Window;
/***********************************************************/

LCUI_API LCUI_Widget*
Window_GetTitleBar(LCUI_Widget *window);
/* 功能：获取窗口标题栏的指针 */ 

LCUI_API LCUI_Widget*
Window_GetClientArea(LCUI_Widget *window);
/* 功能：获取窗口客户区的指针 */

/* 获取窗口右上角关闭按钮 */
LCUI_API LCUI_Widget*
Window_GetCloseButton( LCUI_Widget *window );

LCUI_API void
Window_SetTitleIcon(LCUI_Widget *window, LCUI_Graph *icon);
/* 功能：自定义指定窗口的标题栏图标 */ 

LCUI_API void
Window_SetTitleText(LCUI_Widget *win_p, const char *text);
/* 功能：为窗口设置标题文字 */ 

LCUI_API void
Window_SetTitleTextW(LCUI_Widget *win_p, const wchar_t *text);
/* 功能：为窗口设置标题文字 */

LCUI_API void
Window_ClientArea_Add(LCUI_Widget *window, LCUI_Widget *widget);
/* 功能：将部件添加至窗口客户区 */ 

LCUI_API void
Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget);
/* 功能：将部件添加至窗口标题栏 */ 

/* 新建一个窗口 */
LCUI_API LCUI_Widget*
Window_New( const char *title, LCUI_Graph *icon, LCUI_Size size );

LCUI_END_HEADER

#ifdef __cplusplus
#include LC_WINDOW_HPP
#endif

#endif /* __LCUI_WINDOW_H__ */
