/* ***************************************************************************
 * graph_display.c -- graphical display processing
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
 * graph_display.c -- 图形显示处理
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
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include LC_CURSOR_H

#include <unistd.h>

int Get_Screen_Width ()
/*
 * 功能：获取屏幕宽度
 * 返回值：屏幕的宽度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
{
	if ( !LCUI_Sys.init ) {
		return 0; 
	}
	return LCUI_Sys.screen.size.w; 
}

int Get_Screen_Height ()
/*
 * 功能：获取屏幕高度
 * 返回值：屏幕的高度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
{
	if ( !LCUI_Sys.init ) {
		return 0; 
	}
	return LCUI_Sys.screen.size.h; 
}

LCUI_Size Get_Screen_Size ()
/* 功能：获取屏幕尺寸 */
{
	return LCUI_Sys.screen.size; 
}

//static int debug_count = 0;
int Add_Screen_Refresh_Area (LCUI_Rect rect)
/* 功能：在整个屏幕内添加需要刷新的区域 */
{
	int ret;
	if (rect.width <= 0 || rect.height <= 0) {
		return -1; 
	} 
	DEBUG_MSG("add screen area: %d,%d,%d,%d\n", 
	rect.x, rect.y, rect.width, rect.height);
	
	rect = Get_Valid_Area(Get_Screen_Size(), rect); 
	//if( rect.width == 320 && rect.height == 240 ) {
		//++debug_count;
	//}
	//if( debug_count >= 20 ) {
	//	abort();
	//}
	/* 为队列加上“写”锁，以确保正常操作队列 */
	Queue_Using( &LCUI_Sys.invalid_area, RWLOCK_WRITE );
	ret = RectQueue_Add ( &LCUI_Sys.invalid_area, rect );
	/* 队列使用结束，解开锁 */
	Queue_End_Use( &LCUI_Sys.invalid_area );
	return ret;
}

int Get_Screen_Bits()
/* 功能：获取屏幕中的每个像素的表示所用的位数 */
{
	return LCUI_Sys.screen.bits;
}

LCUI_Pos Get_Screen_Center_Point()
/* 功能：获取屏幕中心点的坐标 */
{
	return Pos(LCUI_Sys.screen.size.w/2.0, LCUI_Sys.screen.size.h/2.0);
}

/* 获取屏幕中指定区域内实际要显示的图形 */
void Get_Screen_Real_Graph ( LCUI_Rect rect, LCUI_Graph *graph )
{
	LCUI_Pos pos;
	GraphLayer_GetGraph( LCUI_Sys.root_glayer, graph, rect );
	if ( !LCUI_Sys.cursor.visible ) { /* 如果游标可见 */
		return;
	}
	/* 如果该区域与游标的图形区域重叠 */ 
	if ( Rect_Is_Overlay( rect, Get_Cursor_Rect()) ) {
		pos.x = LCUI_Sys.cursor.pos.x - rect.x;
		pos.y = LCUI_Sys.cursor.pos.y - rect.y;
		/* 将图形合成 */ 
		Graph_Mix( graph, &LCUI_Sys.cursor.graph, pos );
	}
}

static void 
Handle_Screen_Update()
/* 功能：进行屏幕内容更新 */
{ 
	LCUI_Rect rect;
	LCUI_Graph fill_area, graph;
	
	Graph_Init(&graph);
	Graph_Init(&fill_area);
	/* 锁住队列，其它线程不能访问 */
	//_DEBUG_MSG("enter\n");
	Queue_Lock( &LCUI_Sys.invalid_area );
	while(LCUI_Active()) {
		//_DEBUG_MSG("total area: %d\n", 
		//	Queue_Get_Total( &LCUI_Sys.invalid_area ));
		/* 如果从队列中获取数据成功 */
		if ( !RectQueue_Get(&rect, 0, &LCUI_Sys.invalid_area) ) {
			break;
		}
		/* 获取内存中对应区域的图形数据 */ 
		Get_Screen_Real_Graph ( rect, &graph );
		//_DEBUG_MSG("get screen area: %d,%d,%d,%d\n", 
		//rect.x, rect.y, rect.width, rect.height);
		/* 写入至帧缓冲，让屏幕显示图形 */
		Graph_Display( &graph, Pos(rect.x, rect.y) );
		/* 移除队列中的成员 */ 
		Queue_Delete( &LCUI_Sys.invalid_area, 0 );
	}
	/* 解锁队列 */
	Queue_UnLock( &LCUI_Sys.invalid_area );
	//_DEBUG_MSG("quit\n");
	Graph_Free(&graph);
}

//#define need_autoquit
#ifdef need_autoquit
static int auto_flag = 0;
static void *autoquit()
/* 在超时后，会自动终止程序，用于调试 */
{
	LCUI_ID time = 0;
	while(time <5000000) {
		if(auto_flag == 0) {
			usleep(10000);
			time += 10000;
		} else {
			auto_flag = 0;
			time = 0;
		}
	}
	exit(-1);
}
#endif


static void 
Handle_Refresh_Area( void )
/*
 * 功能：处理已记录的刷新区域
 * 说明：此函数会将各个部件的rect队列中的处理掉，并将
 * 最终的局部刷新区域数据添加至屏幕刷新区域队列中，等
 * 待LCUI来处理。
 **/
{
	if ( LCUI_Sys.need_sync_area ) {
		/* 同步部件内记录的区域至主记录中 */ 
		Widget_SyncInvalidArea( NULL );
		LCUI_Sys.need_sync_area = FALSE; 
	} 
}

static int fps = 0, fps_count = 0;

/* 刷新FPS计数 */
static void 
refresh_fps_count( void )
{
	fps = fps_count;
	fps_count = 0;
	//printf("FPS: %d\n", fps);
}

static void *
Handle_Area_Update ()
/* 功能：进行屏幕内容更新 */
{
#ifdef need_autoquit
	thread_t t;
	LCUI_Thread_Create(&t, NULL, autoquit, NULL);
#endif
	int timer_id;
	/* 添加个定时器，每隔1秒刷新FPS计数 */
	timer_id = set_timer( 1000, refresh_fps_count, TRUE );
	//_DEBUG_MSG("enter\n");
	while(LCUI_Active()) {
		Handle_All_WidgetUpdate();/* 处理所有部件更新 */ 
		usleep(5000);/* 停顿一段时间，让程序主循环处理任务 */
		Handle_Refresh_Area(); /* 处理需要刷新的区域 */
		Handle_Screen_Update();/* 处理屏幕更新 */ 
#ifdef need_autoquit
		auto_flag = 1;
#endif
		/* 累计当前更新的帧数 */
		++fps_count;
	}
	/* 释放定时器 */
	free_timer( timer_id );
	//_DEBUG_MSG("exit\n");
	thread_exit(NULL);
}



extern int Screen_Init();
extern int Screen_Destroy();

int Enable_Graph_Display()
/* 功能：启用图形输出 */
{
	Screen_Init();
	return thread_create( &LCUI_Sys.display_thread, 
			NULL, Handle_Area_Update, NULL );
}

int Disable_Graph_Display()
/* 功能：禁用图形输出 */
{
	Screen_Destroy();
	return thread_join( LCUI_Sys.display_thread, NULL );
}
