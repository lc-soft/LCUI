/* ***************************************************************************
 * LCUI_ScrollBar.c -- LCUI's ScrollBar widget
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
 * LCUI_Window.c -- LCUI 的滚动条部件
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

#include <LCUI_Build.h>
#include LC_LCUI_H

typedef struct _LCUI_ScrollBar LCUI_ScrollBar;
struct _LCUI_ScrollBar
{
	/* 这两个控制滑块的位置 */
	int max_num;
	int current_num;
	/* 这两个控制滑块的宽度 */
	int max_size;
	int current_size;
	LCUI_Widget *widget;	/* 滑块 */
};


/**************************** 私有函数 **********************************/
void ScrollBar_Init( LCUI_Widget *widget )
{
	
}

void ScrollBar_Drag(  )
{
	
}

/* 滑块的图形更新 */
void ScrollBar_Update(  )
{
	
}
/* 更新容纳滑块的容器的图形 */
void ScrollBar_Container_Update(  )
{
	
}
/**********************************************************************/


/**************************** 公共函数 **********************************/
void ScrollBar_Set_MaxNum( LCUI_Widget *widget, int max_num )
{
	
}

void ScrollBar_Set_MaxSize( LCUI_Widget *widget, int max_size )
{
	
}

void ScrollBar_Set_CurrentNum( LCUI_Widget *widget, int current_num )
{
	
}

void ScrollBar_Set_CurrentSize( LCUI_Widget *widget, int current_size )
{
	
}
/*  */
void ScrollBar_Set_Direction( LCUI_Widget *widget, int direction )
{
	
}
/**********************************************************************/
