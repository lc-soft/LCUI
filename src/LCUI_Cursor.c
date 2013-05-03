/* ***************************************************************************
 * LCUI_Cursor.c -- control mouse cursor
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
 * LCUI_Cursor.c -- 控制鼠标光标
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
//#define DEBUG
#include <LCUI_Build.h>

#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_CURSOR_H
#include LC_DISPLAY_H
#include LC_MISC_H
#include LC_MEM_H

/* 刷新鼠标游标在屏幕上显示的图形 */
LCUI_API void
LCUICursor_Refresh( void )
{
	LCUIScreen_InvalidArea ( LCUICursor_GetRect() );
}

/* 显示鼠标游标 */
LCUI_API void
LCUICursor_Show( void )
{
	LCUI_Sys.cursor.visible = TRUE;	/* 标识游标为可见 */
	LCUICursor_Refresh();
}

/* 隐藏鼠标游标 */
LCUI_API void
LCUICursor_Hide( void )
{
	LCUI_Sys.cursor.visible = FALSE;
	LCUICursor_Refresh();
}

/* 获取鼠标游标的区域范围 */
LCUI_API LCUI_Rect
LCUICursor_GetRect( void )
{
	LCUI_Rect rect;
	rect.x = LCUI_Sys.cursor.current_pos.x;
	rect.y = LCUI_Sys.cursor.current_pos.y;
	rect.width = LCUI_Sys.cursor.graph.width;
	rect.height = LCUI_Sys.cursor.graph.height;
	return rect;
}

/* 更新鼠标指针的位置 */
LCUI_API void
LCUICursor_UpdatePos( void )
{
	LCUI_Rect old;
	if( LCUI_Sys.cursor.current_pos.x == LCUI_Sys.cursor.new_pos.x
	 && LCUI_Sys.cursor.current_pos.y == LCUI_Sys.cursor.new_pos.y ) {
		return;
	}
	old = LCUICursor_GetRect();
 	LCUI_Sys.cursor.current_pos = LCUI_Sys.cursor.new_pos;
 	/* 刷新游标的显示 */
	LCUICursor_Refresh();
	/* 刷新游标原来的区域中的图形 */
	LCUIScreen_InvalidArea ( old );
}

/* 设定游标的位置 */
LCUI_API void
LCUICursor_SetPos( LCUI_Pos pos )
{
	LCUI_Sys.cursor.new_pos = pos;
	DEBUG_MSG("new pos: %d,%d\n", pos.x, pos.y);
}

/*
 * 功能：设定游标的图形
 * 返回值：设定成功返回0，失败则返回-1
*/
LCUI_API int
LCUICursor_SetGraph( LCUI_Graph *graph )
{
	if (Graph_IsValid (graph)) {
		Graph_Copy (&LCUI_Sys.cursor.graph, graph);
		LCUICursor_Refresh();
		return 0;
	}
	return -1;
}

/* 获取鼠标指针当前的坐标 */
LCUI_API LCUI_Pos
LCUICursor_GetPos( void )
{
	return LCUI_Sys.cursor.current_pos;
}

/* 获取鼠标指针将要更新的坐标 */
LCUI_API LCUI_Pos
LCUICursor_GetNewPos( void )
{
	return LCUI_Sys.cursor.new_pos;
}
