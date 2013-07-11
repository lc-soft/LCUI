/* ***************************************************************************
 * graph_display.c -- graphical display processing
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
 * graph_display.c -- 图形显示处理
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
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include LC_CURSOR_H

#include <time.h>
#define SYNC_TIME 10


#ifdef LCUI_BUILD_IN_WIN32
#include <Windows.h>
#endif
/* 标记是否初始化 */
static LCUI_BOOL i_am_init = FALSE;
static LCUI_BOOL need_sync_area = FALSE;
/* 用于记录屏幕无效区域 */
static LCUI_RectQueue screen_invalid_area;
static LCUI_Mutex glayer_list_mutex;
static LCUI_Screen screen;

/* 获取屏幕宽度 */
LCUI_API int
LCUIScreen_GetWidth( void )
{
	return screen.size.w;
}

/* 获取屏幕高度 */
LCUI_API int
LCUIScreen_GetHeight( void )
{
	return screen.size.h;
}

/* 获取屏幕尺寸 */
LCUI_API LCUI_Size
LCUIScreen_GetSize( void )
{
	return screen.size;
}

/* 获取屏幕无效区域队列的指针 */
LCUI_API LCUI_RectQueue*
LCUIScreen_GetInvalidAreaQueue( void )
{
	if( !i_am_init ) {
		return NULL;
	}
	return &screen_invalid_area;
}

/* 设置屏幕内的指定区域为无效区域，以便刷新该区域内的图形显示 */
LCUI_API int
LCUIScreen_InvalidArea( LCUI_Rect rect )
{
	if( !i_am_init ) {
		return -1;
	}
	if (rect.width <= 0 || rect.height <= 0) {
		return -2;
	}
	rect = LCUIRect_ValidArea(LCUIScreen_GetSize(), rect);
	DEBUG_MSG("%d,%d,%d,%d\n", rect.x, rect.y, rect.width, rect.height);
	return RectQueue_AddToValid ( &screen_invalid_area, rect );
}

/* 获取屏幕每个像素点的色彩值所占的位数 */
LCUI_API int
LCUIScreen_GetBits( void )
{
	return screen.bits;
}

/* 获取屏幕显示模式 */
LCUI_API int
LCUIScreen_GetMode( void )
{
	return screen.mode;
}

/* 获取屏幕信息 */
LCUI_API void LCUIScreen_GetInfo( LCUI_Screen *info )
{
	memcpy( info, &screen, sizeof(LCUI_Screen) );
}

/* 设置屏幕信息 */
LCUI_API void LCUIScreen_SetInfo( LCUI_Screen *info )
{
	memcpy( &screen, info, sizeof(LCUI_Screen) );
}

/* 获取屏幕中心点的坐标 */
LCUI_API LCUI_Pos
LCUIScreen_GetCenter( void )
{
	return Pos(screen.size.w/2.0, screen.size.h/2.0);
}

/* 为图层树锁上互斥锁 */
LCUI_API void 
LCUIScreen_LockGraphLayerTree( void )
{
	LCUIMutex_Lock( &glayer_list_mutex );
}

/* 解除图层树互斥锁 */
LCUI_API void 
LCUIScreen_UnlockGraphLayerTree( void )
{
	LCUIMutex_Unlock( &glayer_list_mutex );
}

/* 获取屏幕中指定区域内实际要显示的图形 */
LCUI_API void
LCUIScreen_GetRealGraph( LCUI_Rect rect, LCUI_Graph *graph )
{
	LCUI_Pos pos, cursor_pos;
	/* 设置互斥锁，避免在统计图层时，图层记录被其它线程修改 */
	LCUIScreen_LockGraphLayerTree();
	GraphLayer_GetGraph( RootWidget_GetGraphLayer(), graph, rect );
	LCUIScreen_UnlockGraphLayerTree();
	/* 如果游标不可见 */
	if ( !LCUICursor_Visible() ) {
		return;
	}
	/* 如果该区域与游标的图形区域重叠 */
	if ( LCUICursor_CoverRect( rect ) ) {
		cursor_pos = LCUICursor_GetPos();
		pos.x = cursor_pos.x - rect.x;
		pos.y = cursor_pos.y - rect.y;
		/* 将鼠标游标的图形混合至当前图形里 */
		LCUICursor_MixGraph( graph, pos );
	}
}

#ifdef LCUI_BUILD_IN_WIN32
static void Win32_Clinet_InvalidArea( LCUI_Rect rect )
{
	HWND hWnd;
	RECT win32_rect;

	hWnd = Win32_GetSelfHWND();
	win32_rect.left = rect.x;
	win32_rect.top = rect.y;
	win32_rect.right = rect.x + rect.width;
	win32_rect.bottom = rect.y + rect.height;
	InvalidateRect( hWnd, &win32_rect, FALSE );
}
#endif

/* 更新无效区域内的图像 */
static int
LCUIScreen_UpdateInvalidArea(void)
{
	int ret;
	LCUI_Rect rect;
	LCUI_Graph graph;

	//_DEBUG_MSG("enter\n");
	Graph_Init( &graph );
	ret = 0;
	/* 切换可用队列为当前使用的队列 */
	RectQueue_Switch( &screen_invalid_area );
	while( i_am_init ) {
		/* 如果从队列中获取数据成功 */
		if ( !RectQueue_GetFromCurrent(&screen_invalid_area, &rect) ) {
			break;
		}
		ret = 1;
		/* 获取内存中对应区域的图形数据 */
		LCUIScreen_GetRealGraph ( rect, &graph );
		/* 写入至帧缓冲，让屏幕显示图形 */
		LCUIScreen_PutGraph( &graph, Pos(rect.x, rect.y) );
	}
	//_DEBUG_MSG("quit\n");
	Graph_Free( &graph );
	return ret;
}

/* 标记需要同步无效区域 */
LCUI_API void LCUIScreen_MarkSync(void)
{
	need_sync_area = TRUE;
}

/*
 * 功能：处理已记录的无效区域
 * 说明：此函数会将各个部件的rect队列中的处理掉，并将最终的无效区域添加至屏幕无效区域
 * 队列中，等待LCUI来处理。
 **/
static int
LCUIScreen_SyncInvalidArea( void )
{
	int ret = 0;
	if ( need_sync_area ) {
		/* 同步部件内记录的区域至主记录中 */
		Widget_SyncInvalidArea();
		need_sync_area = FALSE;
		ret = 1;
	}
	return ret;
}

/* 更新屏幕内的图形显示 */
static void LCUIScreen_Update( void* unused )
{
	int retval;
	clock_t lost_time;
	LCUI_Rect screen_area;

	/* 先标记刷新整个屏幕区域 */
	screen_area.x = screen_area.y = 0;
	screen_area.width = screen.size.w;
	screen_area.height = screen.size.h;
	LCUIScreen_InvalidArea( screen_area );

	while(LCUI_Sys.state == ACTIVE) {
		lost_time = clock();
		LCUICursor_UpdatePos();
		WidgetMsg_Proc(NULL); /* 处理所有消息 */
		retval = LCUIScreen_SyncInvalidArea();
		retval += LCUIScreen_UpdateInvalidArea();
		if( retval > 0 ) {
			LCUIScreen_SyncFrameBuffer();
		}
		lost_time = clock() - lost_time;
		if( lost_time < SYNC_TIME ) {
			LCUI_MSleep(SYNC_TIME-lost_time);
		}
	}
	LCUIThread_Exit(NULL);
}

/* 初始化图形输出模块 */
LCUI_API int
LCUIModule_Video_Init( int w, int h, int mode )
{
	if( i_am_init ) {
		return -1;
	}
	LCUIMutex_Init( &glayer_list_mutex );
	LCUIScreen_Init( w, h, mode );
	i_am_init = TRUE;
	RectQueue_Init( &screen_invalid_area );
	return _LCUIThread_Create( &LCUI_Sys.display_thread,
			LCUIScreen_Update, NULL );
}

/* 停用图形输出模块 */
LCUI_API int
LCUIModule_Video_End( void )
{
	if( !i_am_init ) {
		return -1;
	}
	LCUIScreen_Destroy();
	i_am_init = FALSE;
	RectQueue_Destroy( &screen_invalid_area );
	return _LCUIThread_Join( LCUI_Sys.display_thread, NULL );
}
