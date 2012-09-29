/* ***************************************************************************
 * LCUI_Cursor.c -- control mouse cursor
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
 * LCUI_Cursor.c -- 控制鼠标光标
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
//#define DEBUG
#include <LCUI_Build.h>

#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_CURSOR_H
#include LC_DISPLAY_H
#include LC_MISC_H
#include LC_MEM_H

void Refresh_Cursor ()
/* 功能：刷新鼠标游标在屏幕上显示的图形 */
{
	Add_Screen_Refresh_Area ( Get_Cursor_Rect() );
}

void Show_Cursor ()
/* 功能：显示鼠标游标 */
{
	LCUI_Sys.cursor.visible = IS_TRUE;	/* 标识游标为可见 */
	Refresh_Cursor ();			/* 刷新游标的显示 */
}



void Hide_Cursor ()
/* 功能：隐藏鼠标游标 */
{
	LCUI_Sys.cursor.visible = IS_FALSE;
	Refresh_Cursor ();
}


LCUI_Rect Get_Cursor_Rect()
/* 功能：获取鼠标游标的区域范围 */
{
	LCUI_Rect rect;
	rect.x = LCUI_Sys.cursor.pos.x;
	rect.y = LCUI_Sys.cursor.pos.y;
	rect.width = LCUI_Sys.cursor.graph.width;
	rect.height = LCUI_Sys.cursor.graph.height;
	return rect;
}

void Set_Cursor_Pos (LCUI_Pos pos)
/* 功能：设定游标的位置 */
{
	LCUI_Rect old;
	old = Get_Cursor_Rect();
 	LCUI_Sys.cursor.pos = pos; 
 	DEBUG_MSG("cursor new pos: %d, %d\n", pos.x, pos.y);
	Refresh_Cursor ();		/* 刷新游标的显示 */ 
	Add_Screen_Refresh_Area ( old ); /* 刷新游标原来的区域中的图形 */
}

int Set_Cursors_Graph (LCUI_Graph * graph)
/* 
 * 功能：设定游标的图形
 * 返回值：设定成功返回0，失败则返回-1
*/
{
	if (Graph_Valid (graph)) {
		Graph_Copy (&LCUI_Sys.cursor.graph, graph);
		Refresh_Cursor();
		return 0;
	}
	return -1;
}

LCUI_Pos Get_Cursor_Pos ()
/* 功能：获取鼠标指针的位置 */
{
	return LCUI_Sys.cursor.pos; 
}
