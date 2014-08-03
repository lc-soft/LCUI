/* ***************************************************************************
 * LCUI_Cursor.c -- control mouse global_cursor
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
//#define DEBUG
#include <LCUI_Build.h>

#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_CURSOR_H
#include LC_DISPLAY_H

static struct LCUI_Cursor {
	LCUI_Pos current_pos;	/* 当前的坐标 */
	LCUI_Pos new_pos;	/* 将要更新的坐标 */
	int visible;		/* 是否可见 */
	LCUI_Graph graph;	/* 游标的图形 */
} global_cursor;

static uchar_t cursor_img_rgba[4][12*19] = {
	{3,3,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,3,229,3,3,0,0,0,0,0,
	0,0,0,3,255,229,3,3,0,0,0,0,0,0,0,3,255,255,229,3,3,0,0,0,0,0,0,3,255,255,255,229,
	3,3,0,0,0,0,0,3,255,255,255,255,229,3,3,0,0,0,0,3,255,255,255,255,253,226,3,3,0,0,0,3,
	255,255,255,253,251,248,220,3,3,0,0,3,255,255,253,251,248,245,241,214,3,3,0,3,255,253,251,248,245,241,238,234,
	207,3,3,3,253,251,248,245,241,238,234,230,226,201,3,3,251,248,245,217,238,234,3,3,3,3,3,3,248,245,217,3,
	164,230,3,3,0,0,0,3,245,217,3,3,3,226,201,3,0,0,0,3,217,3,3,0,3,176,219,3,3,0,0,3,
	3,3,0,0,3,3,216,192,3,0,0,0,0,0,0,0,0,3,192,211,3,0,0,0,0,0,0,0,0,3,3,3,
	3,0,0},
	{6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,6,230,6,6,0,0,0,0,0,
	0,0,0,6,255,230,6,6,0,0,0,0,0,0,0,6,255,255,230,6,6,0,0,0,0,0,0,6,255,255,255,230,
	6,6,0,0,0,0,0,6,255,255,255,255,230,6,6,0,0,0,0,6,255,255,255,255,253,226,6,6,0,0,0,6,
	255,255,255,253,251,248,221,6,6,0,0,6,255,255,253,251,248,245,241,214,6,6,0,6,255,253,251,248,245,241,238,234,
	207,6,6,6,253,251,248,245,241,238,234,230,226,201,6,6,251,248,245,217,238,234,6,6,6,6,6,6,248,245,217,6,
	165,230,6,6,0,0,0,6,245,217,6,6,6,226,201,6,0,0,0,6,217,6,6,0,6,176,219,6,6,0,0,6,
	6,6,0,0,6,6,216,192,6,0,0,0,0,0,0,0,0,6,192,211,6,0,0,0,0,0,0,0,0,6,6,6,
	6,0,0},
	{26,26,0,0,0,0,0,0,0,0,0,0,26,26,26,0,0,0,0,0,0,0,0,0,26,232,26,26,0,0,0,0,0,
	0,0,0,26,255,232,26,26,0,0,0,0,0,0,0,26,255,255,232,26,26,0,0,0,0,0,0,26,255,255,255,232,
	26,26,0,0,0,0,0,26,255,255,255,255,232,26,26,0,0,0,0,26,255,255,255,255,253,228,26,26,0,0,0,26,
	255,255,255,253,251,248,223,26,26,0,0,26,255,255,253,251,248,245,241,216,26,26,0,26,255,253,251,248,245,241,238,234,
	209,26,26,26,253,251,248,245,241,238,234,230,226,203,26,26,251,248,245,219,238,234,26,26,26,26,26,26,248,245,219,26,
	171,230,26,26,0,0,0,26,245,219,26,26,26,226,203,26,0,0,0,26,219,26,26,0,26,181,219,26,26,0,0,26,
	26,26,0,0,26,26,216,194,26,0,0,0,0,0,0,0,0,26,194,211,26,0,0,0,0,0,0,0,0,26,26,26,
	26,0,0},
	{231,55,0,0,0,0,0,0,0,0,0,0,231,189,55,0,0,0,0,0,0,0,0,0,231,255,189,55,0,0,0,0,0,
	0,0,0,231,255,255,189,55,0,0,0,0,0,0,0,231,255,255,255,189,55,0,0,0,0,0,0,231,255,255,255,255,
	189,55,0,0,0,0,0,231,255,255,255,255,255,189,55,0,0,0,0,231,255,255,255,255,255,255,189,55,0,0,0,231,
	255,255,255,255,255,255,255,189,55,0,0,231,255,255,255,255,255,255,255,255,189,55,0,231,255,255,255,255,255,255,255,255,
	255,189,55,231,255,255,255,255,255,255,255,255,255,255,189,231,255,255,255,255,255,255,189,189,189,189,189,231,255,255,255,244,
	255,255,188,77,0,0,0,231,255,255,244,55,211,255,255,211,0,0,0,231,255,244,55,0,180,255,255,180,77,0,0,189,
	244,55,0,0,55,215,255,255,209,0,0,0,0,0,0,0,0,180,255,255,204,0,0,0,0,0,0,0,0,26,215,158,
	49,0,0}
};

static int LCUICursor_LoadDefualtGraph(LCUI_Graph *buff )
{
	if( Graph_IsValid(buff) ) {
		Graph_Free( buff );
	}
	Graph_Init( buff );
	buff->color_type = COLOR_TYPE_ARGB;
	if( Graph_Create(buff,12,19) != 0 ) {
		return -1;
	}
	Graph_SetRedBits( buff, cursor_img_rgba[0], 12*19 );
	Graph_SetGreenBits( buff, cursor_img_rgba[1], 12*19 );
	Graph_SetBlueBits( buff, cursor_img_rgba[2], 12*19 );
	Graph_SetAlphaBits( buff, cursor_img_rgba[3], 12*19 );
	return 0;
}

/* 初始化游标数据 */
void LCUIModule_Cursor_Init( void )
{
	LCUI_Graph pic;
	Graph_Init( &pic );
	Graph_Init( &global_cursor.graph );
	/* 载入自带的游标的图形数据 */ 
	LCUICursor_LoadDefualtGraph( &pic );
	LCUICursor_SetGraph( &pic );
}

void LCUIModule_Cursor_End( void )
{
	Graph_Free( &global_cursor.graph );
}

/* 获取鼠标游标的区域范围 */
LCUI_API void LCUICursor_GetRect( LCUI_Rect *rect )
{
	rect->x = global_cursor.current_pos.x;
	rect->y = global_cursor.current_pos.y;
	rect->width = global_cursor.graph.w;
	rect->height = global_cursor.graph.h;
}

/* 刷新鼠标游标在屏幕上显示的图形 */
LCUI_API void LCUICursor_Refresh( void )
{
	LCUI_Rect rect;
	if( !global_cursor.visible ) {
		return;
	}
	LCUICursor_GetRect( &rect );
	LCUIScreen_InvalidateArea( &rect );
}

/* 检测鼠标游标是否可见 */
LCUI_API LCUI_BOOL LCUICursor_IsVisible( void )
{
	return global_cursor.visible;
}

/* 显示鼠标游标 */
LCUI_API void LCUICursor_Show( void )
{
	global_cursor.visible = TRUE;	/* 标识游标为可见 */
	LCUICursor_Refresh();

}

/* 隐藏鼠标游标 */
LCUI_API void LCUICursor_Hide( void )
{
	LCUI_Rect rect;
	global_cursor.visible = FALSE;
	LCUICursor_GetRect( &rect );
	LCUIScreen_InvalidateArea( &rect );
}

/* 更新鼠标指针的位置 */
LCUI_API void LCUICursor_UpdatePos( void )
{
	if( global_cursor.current_pos.x == global_cursor.new_pos.x
	 && global_cursor.current_pos.y == global_cursor.new_pos.y ) {
		return;
	}
	LCUICursor_Refresh();
 	global_cursor.current_pos = global_cursor.new_pos;
	LCUICursor_Refresh();
}

/* 设定游标的位置 */
LCUI_API void LCUICursor_SetPos( LCUI_Pos pos )
{
	global_cursor.new_pos = pos;
	DEBUG_MSG("new pos: %d,%d\n", pos.x, pos.y);
}

/** 设置游标的图形 */
LCUI_API int LCUICursor_SetGraph( LCUI_Graph *graph )
{
	if( Graph_IsValid (graph) ) {
		LCUICursor_Refresh();
		if( Graph_IsValid(&global_cursor.graph) ) {
			Graph_Free( &global_cursor.graph );
		}
		Graph_Copy( &global_cursor.graph, graph );
		LCUICursor_Refresh();
		return 0;
	}
	return -1;
}

/* 获取鼠标指针当前的坐标 */
LCUI_API void LCUICursor_GetPos( LCUI_Pos *pos )
{
	*pos = global_cursor.current_pos;
}

/* 获取鼠标指针将要更新的坐标 */
LCUI_API void LCUICursor_GetNewPos( LCUI_Pos *pos )
{
	*pos = global_cursor.new_pos;
}

/* 检测鼠标游标是否覆盖在矩形区域上 */
LCUI_BOOL LCUICursor_IsCoverRect( LCUI_Rect rect )
{
	LCUI_Rect cursor_rect;
	LCUICursor_GetRect( &cursor_rect );
	return LCUIRect_IsCoverRect( rect, cursor_rect );
}

/* 将当前鼠标游标的图像叠加至目标图像指定位置 */
int LCUICursor_MixGraph( LCUI_Graph *buff, LCUI_Pos pos )
{
	return Graph_Mix( buff, &global_cursor.graph, pos );
}
