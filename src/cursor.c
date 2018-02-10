/* cursor.c -- Mouse cursor operation set.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/cursor.h>
#include <LCUI/display.h>

static struct LCUICursorModule {
	LCUI_Pos pos;		/* 当前帧的坐标 */
	LCUI_Pos new_pos;	/* 下一帧将要更新的坐标 */
	LCUI_BOOL visible;	/* 是否可见 */
	LCUI_Graph graph;	/* 游标的图形 */
} cursor;

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

static void OnMouseMoveEvent( LCUI_SysEvent e, void *arg )
{
	cursor.new_pos.x = e->motion.x;
	cursor.new_pos.y = e->motion.y;
}

/* 初始化游标数据 */
void LCUI_InitCursor( void )
{
	LCUI_Graph pic;
	Graph_Init( &pic );
	Graph_Init( &cursor.graph );
	/* 载入自带的游标的图形数据 */
	LCUICursor_LoadDefualtGraph( &pic );
	cursor.new_pos.x = LCUIDisplay_GetWidth() / 2;
	cursor.new_pos.y = LCUIDisplay_GetHeight() / 2;
	LCUI_BindEvent( LCUI_MOUSEMOVE, OnMouseMoveEvent, NULL, NULL );
	LCUICursor_SetGraph( &pic );
	LCUICursor_Show();
	Graph_Free( &pic );
}

void LCUI_FreeCursor( void )
{
	Graph_Free( &cursor.graph );
}

/* 获取鼠标游标的区域范围 */
void LCUICursor_GetRect( LCUI_Rect *rect )
{
	float scale;
	scale = LCUIMetrics_GetScale();
	rect->x = iround( cursor.pos.x / scale );
	rect->y = iround( cursor.pos.y / scale );
	rect->width = iround( cursor.graph.width / scale );
	rect->height = iround( cursor.graph.height / scale );
}

/* 刷新鼠标游标在屏幕上显示的图形 */
void LCUICursor_Refresh( void )
{
	LCUI_Rect rect;
	if( !cursor.visible ) {
		return;
	}
	LCUICursor_GetRect( &rect );
	LCUIDisplay_InvalidateArea( &rect );
}

/* 检测鼠标游标是否可见 */
LCUI_BOOL LCUICursor_IsVisible( void )
{
	return cursor.visible;
}

/* 显示鼠标游标 */
void LCUICursor_Show( void )
{
	cursor.visible = TRUE;	/* 标识游标为可见 */
	LCUICursor_Refresh();
}

/* 隐藏鼠标游标 */
void LCUICursor_Hide( void )
{
	LCUICursor_Refresh();
	cursor.visible = FALSE;
}

/* 更新鼠标指针的位置 */
void LCUICursor_Update( void )
{
	if( cursor.pos.x == cursor.new_pos.x
	 && cursor.pos.y == cursor.new_pos.y ) {
		return;
	}
	LCUICursor_Refresh();
 	cursor.pos = cursor.new_pos;
	LCUICursor_Refresh();
}

/* 设定游标的位置 */
void LCUICursor_SetPos( LCUI_Pos pos )
{
	cursor.new_pos = pos;
}

/** 设置游标的图形 */
int LCUICursor_SetGraph( LCUI_Graph *graph )
{
	if( Graph_IsValid (graph) ) {
		LCUICursor_Refresh();
		if( Graph_IsValid(&cursor.graph) ) {
			Graph_Free( &cursor.graph );
		}
		Graph_Copy( &cursor.graph, graph );
		LCUICursor_Refresh();
		return 0;
	}
	return -1;
}

/* 获取鼠标指针当前的坐标 */
void LCUICursor_GetPos( LCUI_Pos *pos )
{
	*pos = cursor.new_pos;
}

/* 检测鼠标游标是否覆盖在矩形区域上 */
LCUI_BOOL LCUICursor_IsCoverRect( LCUI_Rect rect )
{
	LCUI_Rect cursor_rect;
	LCUICursor_GetRect( &cursor_rect );
	return LCUIRect_IsCoverRect( &rect, &cursor_rect );
}

/* 将当前鼠标游标的图像叠加至目标图像指定位置 */
int LCUICursor_MixGraph( LCUI_Graph *buff, LCUI_Pos pos )
{
	return Graph_Mix( buff, &cursor.graph, pos.x, pos.y, FALSE );
}
