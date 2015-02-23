/* ***************************************************************************
 * graph_display.c -- graphical display processing.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * graph_display.c -- 图形显示处理。
 *
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/display.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>
#include <LCUI/widget_build.h>
#include <LCUI/surface.h>

#include <time.h>

#ifdef LCUI_BUILD_IN_WIN32
#include <Windows.h>
#endif

#define MS_PER_FRAME (1000/MAX_FRAMES_PER_SEC)	/**< 每帧画面的最少耗时(ms) */

/** surface 记录 */
typedef struct SurfaceRecord {
	LCUI_Surface	surface;	/**< surface */
	LCUI_Widget	widget;		/**< surface 所映射的 widget */
} SurfaceRecord;

/** 图形显示功能的上下文数据 */
static struct DisplayContext {
	LCUI_BOOL	is_working;	/**< 标志，指示当前模块是否处于工作状态 */
	FrameCtrlCtx	fc_ctx;		/**< 上下文句柄，用于画面更新时的帧数控制 */
	LCUI_Thread	thread;		/**< 线程，负责画面更新工作 */
	LinkedList	surface_list;	/**< surface 列表 */
} display = { FALSE, NULL, 0 };

/** 获取当前的屏幕内容每秒更新的帧数 */
int LCUIDisplay_GetFPS(void)
{
	return FrameControl_GetFPS( display.fc_ctx );
}

/** 更新屏幕内的图形显示 */
static void LCUIDisplay_Update( void *unused )
{
	int i, n, j, m;
	LCUI_Rect *p_rect;
	SurfaceRecord *p_sr;
	LCUI_DirtyRectList rlist;
	LCUI_PaintContext paint;

	DirtyRectList_Init( &rlist );
	/* 初始化帧数控制 */
	FrameControl_Init( &display.fc_ctx );
	FrameControl_SetMaxFPS( &display.fc_ctx, 1000/MAX_FRAMES_PER_SEC );
	while( LCUI_IsActive() && display.is_working ) {
		LCUICursor_UpdatePos();		/* 更新鼠标位置 */
		LCUIWidget_Task_Step();		/* 处理所有部件任务 */
		LCUIWidget_Event_Step();	/* 派发所有事件 */
		n = LinkedList_GetTotal( &display.surface_list );
		/* 遍历当前的 surface 记录列表 */
		for( i=0; i<n; ++i ) {
			p_sr = (SurfaceRecord*)
			LinkedList_Get( &display.surface_list );
			if( !p_sr->widget || !p_sr->surface ) {
				continue;
			}
			/* 收集无效区域记录 */
			Widget_ProcInvalidArea( p_sr->widget, &rlist );
			m = LinkedList_GetTotal( &rlist );
			LinkedList_Goto( &rlist, 0 );
			/* 在 surface 上逐个重绘无效区域 */
			for( j=0; j<m; ++j ) {
				p_rect = (LCUI_Rect*)LinkedList_Get( &rlist );
				paint = Surface_BeginPaint( p_sr->surface, p_rect );
				Widget_Render( p_sr->widget, paint );
				Surface_EndPaint( p_sr->surface, paint );
				Surface_Present( p_sr->surface );
				LinkedList_Delete( &rlist );
			}
		}
		/* 让本帧停留一段时间 */
		FrameControl_Remain( &display.fc_ctx );
	}
	LCUIThread_Exit(NULL);
}

/** 初始化图形输出模块 */
int LCUIModule_Video_Init( int w, int h, int mode )
{
	if( display.is_working ) {
		return -1;
	}
	display.is_working = TRUE;
	LinkedList_Init( &display.surface_list, sizeof(SurfaceRecord) );
	return LCUIThread_Create( &display.thread, LCUIDisplay_Update, NULL );
}

/** 停用图形输出模块 */
int LCUIModule_Video_End( void )
{
	if( !display.is_working ) {
		return -1;
	}
	display.is_working = FALSE;
	return LCUIThread_Join( display.thread, NULL );
}
