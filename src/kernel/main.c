/* ***************************************************************************
 * main.c -- The main functions for the LCUI normal work
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
 * main.c -- 使LCUI能够正常工作的相关主要函数
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
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include LC_CURSOR_H 
#include LC_INPUT_H
#include LC_INPUT_METHOD_H
#include LC_ERROR_H
#include LC_FONT_H 
#include LC_WIDGET_H

#include <time.h>

/** LCUI程序的数据 */
struct LCUI_App {
	LCUI_Queue tasks;		/**< 程序的任务队列 */
	LCUI_Queue events;		/**< 事件队列 */
	LCUI_Queue widget_lib;		/**< 部件类型库 */
	LCUI_Mutex task_mutex;		/**< 任务互斥锁，当运行程序任务时会锁上 */
} MainApp;

LCUI_System LCUI_Sys; 

/*--------------------------- Main Loop -------------------------------*/
static LCUI_BOOL init_mainloop_queue = FALSE;
static LCUI_Queue mainloop_queue;
static LCUI_Sleeper mainloop_sleeper;	/**< 用于决定主循环是否睡眠 */

static void LCUI_MainLoopQueue_Init( void )
{
	Queue_Init( &mainloop_queue, sizeof(LCUI_MainLoop), NULL );
}

/* 查找处于运行状态的主循环 */
static LCUI_MainLoop *LCUI_MainLoopQueue_Find( void )
{
	int i, total;
	LCUI_MainLoop *loop;
	
	total = Queue_GetTotal( &mainloop_queue );
	for(i=0; i<total; ++i) {
		loop = Queue_Get( &mainloop_queue, i );
		if( loop->running ) {
			return loop;
		}
	}
	return NULL;
}

static void LCUI_MainLoopQueue_Sort( void )
{
	int i, j, total;
	LCUI_MainLoop *cur_loop, *next_loop;
	
	total = Queue_GetTotal( &mainloop_queue );
	for(i=0; i<total; ++i) {
		cur_loop = Queue_Get( &mainloop_queue, i );
		if( !cur_loop ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			next_loop = Queue_Get( &mainloop_queue, j );
			if( !next_loop ) {
				continue; 
			}
			if( next_loop->level > cur_loop->level ) {
				Queue_Swap( &mainloop_queue, j, i);
				cur_loop = next_loop;
			}
		}
	}
}

static LCUI_MainLoop* LCUI_MainLoop_GetAvailable(void)
{
	int i, total;
	LCUI_MainLoop *loop;
	
	total = Queue_GetTotal( &mainloop_queue );
	for(i=0; i<total; ++i) {
		loop = Queue_Get( &mainloop_queue, i );
		if( loop == NULL ) {
			continue;
		}
		/* 如果该loop已经退出且没有运行 */
		if( loop->quit && !loop->running ) {
			return loop;
		}
	}
	return NULL;
}

/* 新建一个主循环 */
LCUI_MainLoop* LCUI_MainLoop_New( void )
{
	LCUI_MainLoop *loop;
	
	if( !init_mainloop_queue ) {
		LCUI_MainLoopQueue_Init();
		init_mainloop_queue = TRUE;
	}
	loop = LCUI_MainLoop_GetAvailable();
	if( loop == NULL ) {
		loop = (LCUI_MainLoop*)malloc(sizeof(LCUI_MainLoop));
		if( !loop ) {
			return NULL;
		}
	}
	loop->quit = FALSE;
	loop->level = Queue_GetTotal( &mainloop_queue );
	loop->running = FALSE;
	Queue_AddPointer( &mainloop_queue, loop );
	/* 重新对主循环队列进行排序 */
	LCUI_MainLoopQueue_Sort();
	return loop;
}

/* 设定主循环等级，level值越高，处理主循环退出时，也越早处理该循环 */
int LCUI_MainLoop_Level( LCUI_MainLoop *loop, int level )
{
	if( loop == NULL ) {
		return -1;
	}
	loop->level = level;
	LCUI_MainLoopQueue_Sort();
	return 0;
}

static int LCUIApp_RunTask(void)
{ 
	LCUI_Task *task;
	
	LCUIMutex_Lock( &MainApp.task_mutex );
	Queue_Lock( &MainApp.tasks );
	task = (LCUI_Task*)Queue_Get( &MainApp.tasks, 0 );
	if( !task ) {
		Queue_Unlock( &MainApp.tasks );
		LCUIMutex_Unlock( &MainApp.task_mutex );
		return -1;
	}
	if( !task->func ) {
		Queue_Delete( &MainApp.tasks, 0 );
		Queue_Unlock( &MainApp.tasks );
		LCUIMutex_Unlock( &MainApp.task_mutex );
		return -2;
	}
	Queue_DeletePointer( &MainApp.tasks, 0 );
	Queue_Unlock( &MainApp.tasks );
	/* 调用函数指针指向的函数，并传递参数 */
	task->func( task->arg[0], task->arg[1] );
	/* 若需要在调用回调函数后销毁参数 */
	if( task->destroy_arg[0] ) {
		free( task->arg[0] );
	}
	if( task->destroy_arg[1] ) {
		free( task->arg[1] );
	}
	free( task );
	LCUIMutex_Unlock( &MainApp.task_mutex );
	return 0;
}

/** 运行目标主循环 */
int LCUI_MainLoop_Run( LCUI_MainLoop *loop )
{
	DEBUG_MSG("loop: %p, enter\n", loop);
	loop->running = TRUE;
	while( !loop->quit && LCUI_Sys.state == ACTIVE ) {
		if( Queue_GetTotal(&MainApp.tasks) <= 0 ) {
			LCUISleeper_StartSleep( &mainloop_sleeper, 1000 );
			continue;
		}
		LCUIApp_RunTask();
	}
	loop->running = FALSE;
	DEBUG_MSG("loop: %p, exit\n", loop);
	return 0;
}

/** 标记目标主循环需要退出 */
int LCUI_MainLoop_Quit( LCUI_MainLoop *loop )
{
	if( loop == NULL ) {
		loop = LCUI_MainLoopQueue_Find();
		DEBUG_MSG("quit loop: %p\n", loop);
		if( loop == NULL ) {
			return -1;
		}
	}
	loop->quit = TRUE;
	return 0;
}

/** 退出所有主循环 */
static void LCUI_QuitAllMainLoops(void)
{
	int i, total;
	LCUI_MainLoop *loop;

	Queue_Lock( &mainloop_queue );
	total = Queue_GetTotal( &mainloop_queue );
	for(i=0; i<total; ++i) {
		loop = (LCUI_MainLoop*)Queue_Get( &mainloop_queue, i );
		if( loop == NULL ) {
			continue;
		}
		loop->quit = TRUE;
	}
	Queue_Unlock( &mainloop_queue );
}

/** 销毁主循环队列 */
static void LCUI_DestroyMainLoopQueue(void)
{
	Queue_Destroy( &mainloop_queue );
}
/*----------------------- End MainLoop -------------------------------*/

/** 初始化程序数据结构体 */
static void LCUIApp_Init(void)
{
	AppTasks_Init( &MainApp.tasks );
	WidgetLib_Init( &MainApp.widget_lib );
	LCUIMutex_Init( &MainApp.task_mutex );
	LCUISleeper_Create( &mainloop_sleeper );
}

/* 销毁程序占用的资源 */
static void LCUIApp_Destroy(void)
{
	LCUI_DestroyMainLoopQueue();
	LCUIMutex_Unlock( &MainApp.task_mutex );
}

/*********************** App Management End ***************************/


/** 打印LCUI的信息 */
static void LCUI_ShowCopyrightText(void)
{
	printf(
	"============| LCUI v%s |============\n"
	"Copyright (C) 2012-2014 Liu Chao.\n"
	"Licensed under GPLv2.\n"
	"Report bugs to <lc-soft@live.cn>.\n"
	"Project Homepage: www.lcui.org.\n"
	"========================================\n", LCUI_VERSION );
}

/** 检测LCUI是否活动 */
LCUI_BOOL LCUI_Active(void)
{
	if(LCUI_Sys.state == ACTIVE) {
		return TRUE;
	}
	return FALSE;
}

/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */
int LCUI_Init( int w, int h, int mode )
{
	/* 如果LCUI没有初始化过 */
	if( LCUI_Sys.is_inited ) {
		return -1;
	}
	LCUI_Sys.is_inited = TRUE;
	LCUI_Sys.func_atexit = NULL;
	LCUI_Sys.exit_code = 0;
	LCUI_Sys.state = ACTIVE;
	LCUI_ShowCopyrightText();
	LCUIApp_Init();
	/* 初始化各个模块 */
	LCUIModule_Event_Init();
	LCUIModule_IME_Init();
	LCUIModule_Font_Init();
	LCUIModule_Timer_Init();
	LCUIModule_Device_Init();
	LCUIModule_Keyboard_Init();
	LCUIModule_Mouse_Init();
	LCUIModule_TouchScreen_Init();
	LCUIModule_Cursor_Init();
	LCUIModule_Widget_Init();
	LCUIModule_Video_Init(w, h, mode);
	/* 让鼠标游标居中显示 */
	LCUICursor_SetPos( LCUIScreen_GetCenter() );  
	LCUICursor_Show();
	/* 注册默认部件类型 */
	Register_DefaultWidgetType();
	return 0;
}

/** 释放LCUI占用的资源 */
static int LCUI_Destroy( void )
{
	LCUI_Sys.state = KILLED;
	if( LCUI_Sys.func_atexit ) {
		LCUI_Sys.func_atexit();
	}
	LCUIModule_IME_End();
	LCUIModule_Event_End();
	LCUIModule_Cursor_End();
	LCUIModule_Widget_End();
	LCUIModule_Font_End();
	LCUIModule_Timer_End();
	LCUIModule_Keyboard_End();
	//LCUIModule_Mouse_End();
	//LCUIModule_TouchScreen_End();
	LCUIModule_Device_End();
	LCUIModule_Video_End();
	LCUIApp_Destroy();
	return LCUI_Sys.exit_code;
}

void LCUI_Quit(void)
{
	LCUI_Sys.state = KILLED;
	LCUI_QuitAllMainLoops();
}

void LCUI_Exit( int exit_code )
{
	LCUI_Sys.exit_code = exit_code;
	LCUI_Quit();
}

/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
int LCUI_Main( void )
{
	LCUI_MainLoop *loop;
	loop = LCUI_MainLoop_New();
	LCUI_MainLoop_Run( loop );
	return LCUI_Destroy();
}

/** 注册一个函数，以在LCUI程序退出时调用 */
void LCUI_AtQuit( void (*callback_func)(void) )
{
	LCUI_Sys.func_atexit = callback_func;
}

/* 获取LCUI的版本 */
int LCUI_GetSelfVersion( char *out )
{
	return sprintf(out, "%s", LCUI_VERSION);
}
