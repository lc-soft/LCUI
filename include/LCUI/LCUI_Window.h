/* ***************************************************************************
 * LCUI_Window.h -- LCUI's window widget
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
 * LCUI_Window.h -- LCUI 的窗口部件
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


#ifndef __LCUI_WINDOW_H_
#define __LCUI_WINDOW_H_ 
/* 先使用typedef为结构体创建同义字，之后再定义结构体 */
typedef struct _LCUI_TitleBar		LCUI_TitleBar;
typedef struct _LCUI_Window			LCUI_Window;

/******************* 窗口标题栏 ***********************/
struct _LCUI_TitleBar
{
	LCUI_Widget *icon_box;		/* 图标 */
	LCUI_Widget *label;		/* 标题栏中显示的文本 */
}; /* 存储窗口的标题栏数据的结构体 */
/****************************************************/


/************************ 窗口数据 **************************/
struct _LCUI_Window
{
	LCUI_Widget *titlebar;		/* 标题栏 */
	LCUI_Widget *client_area;	/* 客户区 */
	LCUI_Widget *btn_close;	/* 关闭按钮 */
	int hide_style;			/* 窗口隐藏时使用的特效 */
	int show_style;			/* 窗口显示时使用的特效 */
	int resize_style;		/* 窗口改变尺寸时使用的特效 */
	int count;			/* 显示次数计数 */
	
	LCUI_Align init_align;		/* 窗口初始显示位置 */
};
/***********************************************************/

LCUI_Widget *Get_Window_TitleBar(LCUI_Widget *window);
/* 功能：获取窗口标题栏的指针 */ 

LCUI_Widget *Get_Window_Client_Area(LCUI_Widget *window);
/* 功能：获取窗口客户区的指针 */ 

void Set_Window_Title_Icon(LCUI_Widget *window, LCUI_Graph *icon);
/* 功能：自定义指定窗口的标题栏图标 */ 

LCUI_Size Get_Window_Client_Size(LCUI_Widget *win_p);
/* 功能：获取窗口的客户区的尺寸 */ 

void Window_Widget_Auto_Size(LCUI_Widget *win_p);
/* 功能：在窗口尺寸改变时自动改变标题栏和客户区的尺寸 */ 

LCUI_Widget *Get_Parent_Window(LCUI_Widget *widget);
/* 功能：获取指定部件所在的窗口 */ 

void Set_Window_Title_Text(LCUI_Widget *win_p, char *text);
/* 功能：为窗口设置标题文字 */ 

void Window_Client_Area_Add(LCUI_Widget *window, LCUI_Widget *widget);
/* 功能：将部件添加至窗口客户区 */ 

void Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget);
/* 功能：将部件添加至窗口标题栏 */ 
#endif /* __LCUI_WINDOW_H__ */
