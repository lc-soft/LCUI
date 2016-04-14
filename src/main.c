/* ***************************************************************************
 * main.c -- The main functions for the LCUI normal work
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * main.c -- 使LCUI能够正常工作的相关主要函数
 *
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/
//#define DEBUG
#define __IN_MAIN_SOURCE_FILE__

#include <time.h>
#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
#include <LCUI/font.h>
#include <LCUI/input.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_MOUSE_H
#include LCUI_KEYBOARD_H
#include LCUI_DISPLAY_H

#define STATE_ACTIVE 1
#define STATE_KILLED 0

/** 主循环的状态 */
enum MainLoopState {
	STATE_PAUSED,
	STATE_RUNNING,
	STATE_EXITED
};

typedef struct LCUI_MainLoopRec_ {
	int state;		/**< 主循环的状态 */
	unsigned long int tid;	/**< 当前运行该主循环的线程的ID */
} LCUI_MainLoopRec;

typedef struct SysEventHandlerRec_ {
	LCUI_SysEventFunc func;
	void *data;
	void (*destroy_data)(void*);
} SysEventHandlerRec, *SysEventHandler;

typedef struct SysEventPackRec_ {
	LCUI_SysEvent event;
	void *arg;
} SysEventPackRec, *SysEventPack;

/** LCUI 系统相关数据 */
static struct LCUI_System {
	int state;			/**< 状态 */
	int mode;			/**< LCUI的运行模式 */
	LCUI_BOOL is_inited;		/**< 标志，指示LCUI是否初始化过 */

	unsigned long int main_tid;	/**< 主线程ID */

	struct {
		LCUI_EventTrigger trigger;	/**< 系统事件容器 */
		LCUI_Mutex mutex;		/**< 互斥锁 */
	} event;

	int exit_code;			/**< 退出码 */
	void (*func_atexit)(void);	/**< 在LCUI退出时调用的函数 */
} System;

/** LCUI 应用程序数据 */
static struct LCUI_App {
	LinkedList tasks;		/**< 程序的任务队列 */
	LCUI_Mutex tasks_mutex;		/**< 任务记录互斥锁 */
	LCUI_Mutex loop_mutex;		/**< 互斥锁，确保一次只允许一个线程跑主循环 */
	LCUI_Mutex loop_changed;	/**< 互斥锁，用于指示当前运行的主循环是否改变 */
	LCUI_Cond loop_cond;		/**< 条件变量，用于决定是否继续任务循环 */
	LCUI_MainLoop loop;		/**< 当前运行的主循环 */
	LinkedList loops;		/**< 主循环列表 */
	LCUI_AppDriverRec driver;	/**< 程序事件驱动支持 */
} MainApp;

/*-------------------------- system event <START> ---------------------------*/

/** 初始化事件模块 */
static void LCUI_InitEvent(void)
{
	LCUIMutex_Init( &System.event.mutex );
	System.event.trigger = EventTrigger();
}

/** 停用事件模块并进行清理 */
static void LCUI_ExitEvent(void)
{
	LCUIMutex_Destroy( &System.event.mutex );
	EventTrigger_Destroy( System.event.trigger );
	System.event.trigger = NULL;
}

static void OnEvent( LCUI_Event e, void *arg )
{
	SysEventHandler handler = e->data;
	SysEventPack pack = arg;
	pack->event->type = e->type;
	pack->event->data = handler->data;
	handler->func( pack->event, pack->arg );
}

static void DestroySysEventHandler( void *arg )
{
	SysEventHandler handler = arg;
	if( handler->data && handler->destroy_data ) {
		handler->destroy_data( handler->data );
	}
	handler->data = NULL;
}

int LCUI_BindEvent( int id, LCUI_SysEventFunc func, void *data,
		    void (*destroy_data)(void*) )
{
	int ret;
	SysEventHandler handler;
	handler = NEW( SysEventHandlerRec, 1 );
	handler->func = func;
	handler->data = data;
	handler->destroy_data = destroy_data;
	LCUIMutex_Lock( &System.event.mutex );
	ret = EventTrigger_Bind( System.event.trigger, id, OnEvent,
				 handler, DestroySysEventHandler );
	LCUIMutex_Unlock( &System.event.mutex );
	return ret;
}

int LCUI_UnbindEvent( int handler_id )
{
	int ret;
	LCUIMutex_Lock( &System.event.mutex );
	ret = EventTrigger_Unbind2( System.event.trigger, handler_id );
	LCUIMutex_Unlock( &System.event.mutex );
	return ret;
}

int LCUI_TriggerEvent( LCUI_SysEvent e, void *arg )
{
	int ret;
	SysEventPackRec pack;
	pack.arg = arg;
	pack.event = e;
	LCUIMutex_Lock( &System.event.mutex );
	ret = EventTrigger_Trigger( System.event.trigger, e->type, &pack );
	LCUIMutex_Unlock( &System.event.mutex );
	return ret;
}

/*--------------------------- system event <END> ----------------------------*/

/** 销毁程序任务 */
static void DestroyTask( void *arg )
{
	LCUI_AppTask task = arg;
	if( task->destroy_arg[0] && task->arg[0] ) {
		task->destroy_arg[0]( task->arg[0] );
	}
	if( task->destroy_arg[1] && task->arg[1] ) {
		task->destroy_arg[1]( task->arg[1] );
	}
}

/** 添加任务 */
int LCUI_AddTask( LCUI_AppTask task )
{
	LCUI_AppTask buff;
	LinkedListNode *node;
	buff = NEW( LCUI_AppTaskRec, 1 );
	*buff = *task;
	if( LCUI_IsOnMainLoop() ) {
		node = LinkedList_Append( &MainApp.tasks, buff );
	} else {
		LCUIMutex_Lock( &MainApp.tasks_mutex );
		node = LinkedList_Append( &MainApp.tasks, buff );
		LCUIMutex_Unlock( &MainApp.tasks_mutex );
		LCUICond_Broadcast( &MainApp.loop_cond );
	}
	LCUI_BreakEventWaiting();
	return node ? 0:-1;
}

/* 新建一个主循环 */
LCUI_MainLoop LCUI_MainLoop_New( void )
{
	LCUI_MainLoop loop;
	loop = NEW( LCUI_MainLoopRec, 1 );
	loop->state = STATE_PAUSED;
	loop->tid = 0;
	return loop;
}

/** 运行目标主循环 */
int LCUI_MainLoop_Run( LCUI_MainLoop loop )
{
	LinkedListNode *node;
	if( loop->state == STATE_RUNNING ) {
		_DEBUG_MSG("error: main-loop already running.");
		return -1;
	}
	DEBUG_MSG("loop: %p, enter\n", loop);
	loop->state = STATE_RUNNING;
	/* 将主循环记录插入至列表表头 */
	LinkedList_Insert( &MainApp.loops, 0, loop );
	MainApp.loop = loop;
	LCUIMutex_Lock( &MainApp.loop_changed );
	/* 广播，让其它线程交出主循环运行权 */
	LCUICond_Broadcast( &MainApp.loop_cond );
	/* 获取运行权 */
	LCUIMutex_Lock( &MainApp.loop_mutex );
	LCUIMutex_Unlock( &MainApp.loop_changed );
	loop->tid = LCUIThread_SelfID();
	while( loop->state != STATE_EXITED ) {
		LCUI_AppTask task;
		DEBUG_MSG("loop: %p, sleeping...\n", loop);
		LCUI_WaitEvent();
		DEBUG_MSG("loop: %p, wakeup, lost_time: %ld\n", loop, ct);
		LCUI_PumbEvents();
		LinkedList_ForEach( node, &MainApp.tasks ) {
			task = node->data;
			if( !task || !task->func ) {
				return -1;
			}
			DEBUG_MSG("task: %p, start\n", task);
			task->func( task->arg[0], task->arg[1] );
			DEBUG_MSG("task: %p, end\n", task);
		}
		LinkedList_Clear( &MainApp.tasks, DestroyTask );
		LCUIMutex_Unlock( &MainApp.tasks_mutex );
		if( MainApp.loop == loop ) {
			continue;
		}
		/* 如果当前运行的主循环不是自己 */
		loop->state = STATE_PAUSED;
		DEBUG_MSG("loop: %p, release control.\n", loop);
		LCUIMutex_Unlock( &MainApp.loop_mutex );
		/* 等待其它线程获得主循环运行权 */
		LCUIMutex_Lock( &MainApp.loop_changed );
		LCUIMutex_Unlock( &MainApp.loop_changed );
		DEBUG_MSG("loop: %p, waiting...\n", loop);
		/* 等待其它线程释放主循环运行权 */
		LCUIMutex_Lock( &MainApp.loop_mutex );
	}
	loop->state = STATE_EXITED;
	LinkedList_Delete( &MainApp.loops, 0 );
	/* 获取处于列表表头的主循环 */
	loop = LinkedList_Get( &MainApp.loops, 0 );
	if( loop ) {
		/* 改变当前运行的主循环 */
		MainApp.loop = loop;
		LCUICond_Broadcast( &MainApp.loop_cond );
	}
	/* 释放运行权 */
	LCUIMutex_Unlock( &MainApp.loop_mutex );
	DEBUG_MSG("loop: %p, exit\n", loop);
	return 0;
}

/** 标记目标主循环需要退出 */
void LCUI_MainLoop_Quit( LCUI_MainLoop loop )
{
	loop->state = STATE_EXITED;
	LCUICond_Broadcast( &MainApp.loop_cond );
}

/** 初始化程序数据结构体 */
static void LCUIApp_Init(void)
{
	LCUICond_Init( &MainApp.loop_cond );
	LCUIMutex_Init( &MainApp.loop_changed );
	LCUIMutex_Init( &MainApp.loop_mutex );
	LinkedList_Init( &MainApp.loops );
	LCUIMutex_Init( &MainApp.tasks_mutex );
	LinkedList_Init( &MainApp.tasks );
	LCUI_InitApp( &MainApp.driver );
}

LCUI_BOOL LCUI_WaitEvent( void )
{
	return MainApp.driver.WaitEvent();
}

int LCUI_BindSysEvent( int event_id, LCUI_EventFunc func,
		       void *data, void( *destroy_data )(void*) )
{
	return MainApp.driver.BindSysEvent( event_id, func, data, destroy_data );
}

int LCUI_UnbindSysEvent( int event_id, LCUI_EventFunc func )
{
	return MainApp.driver.UnbindSysEvent( event_id, func );
}

void LCUI_PumbEvents( void )
{
	MainApp.driver.PumbEvents();
}

void *LCUI_GetAppData( void )
{
	return MainApp.driver.GetData();
}

void LCUI_BreakEventWaiting( void )
{
	MainApp.driver.BreakEventWaiting();
}

/** 退出所有主循环 */
static void LCUIApp_QuitAllMainLoop(void)
{
	LCUI_MainLoop loop;
	LinkedListNode *node;
	LinkedList_ForEach( node, &MainApp.loops ) {
		loop = node->data;
		if( loop ) {
			loop->state = STATE_EXITED;
		}
	}
	LCUICond_Broadcast( &MainApp.loop_cond );
}

/* 销毁程序占用的资源 */
static void LCUIApp_Destroy(void)
{
	LCUI_MainLoop loop;
	LinkedListNode *node;
	LinkedList_ForEach( node, &MainApp.loops ) {
		loop = node->data;
		LCUI_MainLoop_Quit( loop );
		LCUIThread_Join( loop->tid, NULL );
	}
	/* 开始清理 */
	LCUICond_Destroy( &MainApp.loop_cond );
	LCUIMutex_Destroy( &MainApp.loop_mutex );
	LCUIMutex_Destroy( &MainApp.loop_changed );
	LCUIMutex_Destroy( &MainApp.tasks_mutex );
	LinkedList_Clear( &MainApp.loops, free );
}

/** 打印LCUI的信息 */
static void LCUI_ShowCopyrightText(void)
{
	printf( "           ________\n"
		" _        |______  |\n"
		"| |   __    __   | |\n"
		"| |  |  |  |__|  | |\n"
		"| |  |  |   __   | |\n"
		"| |  |  |__|  |  | |\n"
		"| |  \\________/  | |\n"
		"| |_____    _____| |\n"
		"\\_______|  |_______/\n\n"
		"LCUI (LC's UI) version "LCUI_VERSION"\n"
#ifdef _MSC_VER
		"Build tool: "
#if (_MSC_VER > 1800)
		"MS VC++ (higher version)"
#elif (_MSC_VER == 1800)
		"MS VC++ 12.0"
#elif (_MSC_VER == 1700)
		"MS VC++ 11.0"
#elif (_MSC_VER == 1600)
		"MS VC++ 10.0"
#elif (_MSC_VER == 1500)
		"MS VC++ 9.0"
#elif (_MSC_VER == 1400)
		"MS VC++ 8.0"
#elif (_MSC_VER == 1310)
		"MS VC++ 7.1"
#elif (_MSC_VER == 1300)
		"MS VC++ 7.0"
#elif (_MSC_VER == 1200)
		"MS VC++ 6.0"
#else
		"MS VC++"
#endif
		"\n"
#endif
		"Build at "__DATE__" - "__TIME__"\n"
		"Copyright (C) 2012-2015 Liu Chao <lc-soft@live.cn>.\n"
		"This is free software, licensed under GPLv2. \n"
		"See source distribution for detailed copyright notices.\n"
		"To learn more, visit http://www.lcui.org.\n\n"
	);
}

/** 检测LCUI是否活动 */
LCUI_BOOL LCUI_IsActive(void)
{
	if( System.state == STATE_ACTIVE ) {
		return TRUE;
	}
	return FALSE;
}

/** 检测当前是否在主线程上 */
LCUI_BOOL LCUI_IsOnMainLoop(void)
{
	if( !MainApp.loop ) {
		return FALSE;
	}
	return (MainApp.loop->tid == LCUIThread_SelfID());
}

/*
 * 功能：用于对LCUI进行初始化操作
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */
int LCUI_Init(void)
{
	if( System.is_inited ) {
		return -1;
	}
	System.is_inited = TRUE;
	System.func_atexit = NULL;
	System.exit_code = 0;
	System.state = STATE_ACTIVE;
	System.main_tid = LCUIThread_SelfID();
	LCUI_ShowCopyrightText();
	LCUIApp_Init();
	/* 初始化各个模块 */
	LCUI_InitEvent();
	LCUI_InitFont();
	LCUI_InitTimer();
	LCUI_InitMouseDriver();
	LCUI_InitKeyboardDriver();
	LCUI_InitCursor();
	LCUI_InitWidget();
	LCUI_InitDisplay();
	{
		LCUI_Pos pos;
		pos.x = LCUIDisplay_GetWidth()/2;
		pos.y = LCUIDisplay_GetHeight()/2;
		/* 让鼠标游标居中显示 */
		LCUICursor_SetPos( pos );
		LCUICursor_Show();
	}
	return 0;
}

/** 释放LCUI占用的资源 */
static int LCUI_Destroy( void )
{
	LCUI_SysEventRec e;
	e.type = LCUI_QUIT;
	LCUI_TriggerEvent( &e, NULL );
	System.state = STATE_KILLED;
	LCUI_ExitEvent();
	LCUI_ExitCursor();
	LCUI_ExitWidget();
	LCUI_ExitFont();
	LCUI_ExitTimer();
	LCUI_ExitDisplay();
	LCUIApp_Destroy();
	return System.exit_code;
}

void LCUI_Quit(void)
{
	System.state = STATE_KILLED;
	LCUIApp_QuitAllMainLoop();
}

void LCUI_Exit( int exit_code )
{
	System.exit_code = exit_code;
	LCUI_Quit();
}

/*
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
int LCUI_Main( void )
{
	LCUI_MainLoop loop;
	loop = LCUI_MainLoop_New();
	LCUI_MainLoop_Run( loop );
	return LCUI_Destroy();
}

/* 获取LCUI的版本 */
int LCUI_GetSelfVersion( char *out )
{
	return sprintf(out, "%s", LCUI_VERSION);
}
