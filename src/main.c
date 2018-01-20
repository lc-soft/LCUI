/* ***************************************************************************
 * main.c -- The main functions for the LCUI normal work
 *
 * Copyright (C) 2012-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2017 归属于 刘超 <lc-soft@live.cn>
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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/worker.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
#include <LCUI/input.h>
#include <LCUI/ime.h>
#include <LCUI/platform.h>
#include <LCUI/display.h>
#ifdef LCUI_EVENTS_H
#include LCUI_EVENTS_H
#endif
#ifdef LCUI_MOUSE_H
#include LCUI_MOUSE_H
#endif
#ifdef LCUI_KEYBOARD_H
#include LCUI_KEYBOARD_H
#endif
#ifdef LCUI_DISPLAY_H
#include LCUI_DISPLAY_H
#endif
#include <LCUI/font.h>

#define STATE_ACTIVE 1
#define STATE_KILLED 0

/** 一秒内的最大更新帧数 */
#define MAX_FRAMES_PER_SEC 120

/** 主循环的状态 */
enum MainLoopState {
	STATE_PAUSED,
	STATE_RUNNING,
	STATE_EXITED
};

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
	int exit_code;			/**< 退出码 */
	unsigned long int main_tid;	/**< 主线程ID */
	struct {
		LCUI_EventTrigger trigger;	/**< 系统事件容器 */
		LCUI_Mutex mutex;		/**< 互斥锁 */
	} event;
} System;

#define LCUI_WORKER_NUM	4

/** LCUI 应用程序数据 */
static struct LCUI_App {
	LCUI_BOOL	active;				/**< 是否已经初始化并处于活动状态 */
	LCUI_Mutex	loop_mutex;			/**< 互斥锁，确保一次只允许一个线程跑主循环 */
	LCUI_Cond	loop_changed;			/**< 条件变量，用于指示当前运行的主循环是否改变 */
	LCUI_MainLoop	loop;				/**< 当前运行的主循环 */
	LinkedList	loops;				/**< 主循环列表 */
	StepTimer	timer;				/**< 渲染循环计数器 */
	LCUI_AppDriver	driver;				/**< 程序事件驱动支持 */
	LCUI_BOOL	driver_ready;			/**< 事件驱动支持是否已经准备就绪 */
	LCUI_Worker	main_worker;			/**< 主工作线程 */
	LCUI_Worker	workers[LCUI_WORKER_NUM];	/**< 普通工作线程 */
	int		worker_next;			/**< 下一个工作线程编号 */
} MainApp;

/*-------------------------- system event <START> ---------------------------*/

/** 初始化事件模块 */
static void LCUI_InitEvent( void )
{
	LCUIMutex_Init( &System.event.mutex );
	System.event.trigger = EventTrigger();
}

/** 停用事件模块并进行清理 */
static void LCUI_FreeEvent( void )
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
	free( arg );
}

int LCUI_BindEvent( int id, LCUI_SysEventFunc func, void *data,
		    void (*destroy_data)(void*) )
{
	int ret;
	SysEventHandler handler;
	if( System.state != STATE_ACTIVE ) {
		return -1;
	}
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
	if( System.state != STATE_ACTIVE ) {
		return -1;
	}
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

int LCUI_CreateTouchEvent( LCUI_SysEvent e,
			   LCUI_TouchPoint points, int n_points )
{
	e->type = LCUI_TOUCH;
	e->touch.n_points = n_points;
	e->touch.points = NEW( LCUI_TouchPointRec, n_points );
	if( !e->touch.points ) {
		return -ENOMEM;
	}
	for( n_points -= 1; n_points >= 0; --n_points ) {
		e->touch.points[n_points] = points[n_points];
	}
	return 0;
}

void LCUI_DestroyEvent( LCUI_SysEvent e )
{
	switch( e->type ) {
	case LCUI_TOUCH:
		if( e->touch.points ) {
			free( e->touch.points );
		}
		e->touch.points = NULL;
		e->touch.n_points = 0;
		break;
	case LCUI_TEXTINPUT:
		if( e->text.text ) {
			free( e->text.text );
		}
		e->text.text = NULL;
		e->text.length = 0;
		break;
	}
	e->type = LCUI_NONE;
}

/*--------------------------- system event <END> ----------------------------*/

void LCUI_ProcessEvents( void )
{
	if( MainApp.driver_ready ) {
		MainApp.driver->ProcessEvents();
	}
	while( LCUIWorker_RunTask( MainApp.main_worker ) ) {
		LCUIDisplay_Update();
	}
}

LCUI_BOOL LCUI_PostTask( LCUI_Task task )
{
	if( !MainApp.main_worker ) {
		return FALSE;
	}
	LCUIWorker_PostTask( MainApp.main_worker, task );
	return TRUE;
}

void LCUI_PostAsyncTaskTo( LCUI_Task task, int worker_id )
{
	int id = 0;
	if( !MainApp.active ) {
		LCUITask_Run( task );
		LCUITask_Destroy( task );
		return;
	}
	if( id >= LCUI_WORKER_NUM ) {
		id = 0;
	}
	LCUIWorker_PostTask( MainApp.workers[id], task );
}

int LCUI_PostAsyncTask( LCUI_Task task )
{
	int id;
	if( MainApp.worker_next >= LCUI_WORKER_NUM ) {
		MainApp.worker_next = 0;
	}
	id = MainApp.worker_next;
	LCUI_PostAsyncTaskTo( task, id );
	MainApp.worker_next += 1;
	return id;
}

/* 新建一个主循环 */
LCUI_MainLoop LCUIMainLoop_New( void )
{
	LCUI_MainLoop loop;
	loop = NEW( LCUI_MainLoopRec, 1 );
	loop->state = STATE_PAUSED;
	loop->tid = 0;
	return loop;
}

/** 运行目标主循环 */
int LCUIMainLoop_Run( LCUI_MainLoop loop )
{
	LCUI_BOOL at_same_thread = FALSE;
	if( loop->state == STATE_RUNNING ) {
		DEBUG_MSG( "error: main-loop already running.\n" );
		return -1;
	}
	loop->state = STATE_RUNNING;
	loop->tid = LCUIThread_SelfID();
	if( MainApp.loop ) {
		at_same_thread = MainApp.loop->tid == loop->tid;
	}
	DEBUG_MSG( "at_same_thread: %d\n", at_same_thread );
	if( !at_same_thread ) {
		LCUIMutex_Lock( &MainApp.loop_mutex );
		LinkedList_Insert( &MainApp.loops, 0, loop );
		LCUIMutex_Unlock( &MainApp.loop_mutex );
	} else {
		LinkedList_Insert( &MainApp.loops, 0, loop );
	}
	DEBUG_MSG( "loop: %p, enter\n", loop );
	MainApp.loop = loop;
	while( loop->state != STATE_EXITED ) {
		LCUI_ProcessEvents();
		LCUIDisplay_Update();
		LCUIDisplay_Render();
		LCUIDisplay_Present();
		StepTimer_Remain( MainApp.timer );
		/* 如果当前运行的主循环不是自己 */
		while( MainApp.loop != loop ) {
			loop->state = STATE_PAUSED;
			LCUICond_Wait( &MainApp.loop_changed,
				       &MainApp.loop_mutex );
		}
	}
	loop->state = STATE_EXITED;
	DEBUG_MSG( "loop: %p, exit\n", loop );
	LCUIMainLoop_Destroy( loop );
	LinkedList_Delete( &MainApp.loops, 0 );
	/* 获取处于列表表头的主循环 */
	loop = LinkedList_Get( &MainApp.loops, 0 );
	/* 改变当前运行的主循环 */
	MainApp.loop = loop;
	LCUICond_Broadcast( &MainApp.loop_changed );
	return 0;
}

void LCUIMainLoop_Quit( LCUI_MainLoop loop )
{
	loop->state = STATE_EXITED;
}

void LCUIMainLoop_Destroy( LCUI_MainLoop loop )
{
	free( loop );
}

int LCUI_GetFrameCount( void )
{
	return StepTimer_GetFrameCount( MainApp.timer );
}

void LCUI_InitApp( LCUI_AppDriver app )
{
	int i;
	if( MainApp.driver_ready ) {
		return;
	}
	MainApp.driver_ready = FALSE;
	MainApp.timer = StepTimer_Create();
	LCUICond_Init( &MainApp.loop_changed );
	LCUIMutex_Init( &MainApp.loop_mutex );
	LinkedList_Init( &MainApp.loops );
	MainApp.main_worker = LCUIWorker_New();
	for( i = 0; i < LCUI_WORKER_NUM; ++i ) {
		MainApp.workers[i] = LCUIWorker_New();
		LCUIWorker_RunAsync( MainApp.workers[i] );
	}
	StepTimer_SetFrameLimit( MainApp.timer, MAX_FRAMES_PER_SEC );
	if( !app ) {
		app = LCUI_CreateAppDriver();
		if( !app ) {
			return;
		}
	}
	MainApp.active = TRUE;
	MainApp.driver = app;
	MainApp.driver_ready = TRUE;
}

static void OnDeleteMainLoop( void *arg )
{
	LCUIMainLoop_Destroy( arg );
}

static void LCUI_FreeApp( void )
{
	int i;
	LCUI_MainLoop loop;
	LinkedListNode *node;
	MainApp.active = FALSE;
	for( LinkedList_Each( node, &MainApp.loops ) ) {
		loop = node->data;
		LCUIMainLoop_Quit( loop );
		LCUIThread_Join( loop->tid, NULL );
	}
	StepTimer_Destroy( MainApp.timer );
	LCUIMutex_Destroy( &MainApp.loop_mutex );
	LCUICond_Destroy( &MainApp.loop_changed );
	LinkedList_Clear( &MainApp.loops, OnDeleteMainLoop );
	if( MainApp.driver_ready ) {
		LCUI_DestroyAppDriver( MainApp.driver );
	}
	MainApp.driver_ready = FALSE;
	for( i = 0; i < LCUI_WORKER_NUM; ++i ) {
		LCUIWorker_Destroy( MainApp.workers[i] );
		MainApp.workers[i] = NULL;
	}
	LCUIWorker_Destroy( MainApp.main_worker );
	MainApp.main_worker = NULL;
}

int LCUI_BindSysEvent( int event_id, LCUI_EventFunc func,
		       void *data, void( *destroy_data )(void*) )
{
	if( MainApp.driver_ready ) {
		return MainApp.driver->BindSysEvent( event_id, func, 
						    data, destroy_data );
	}
	return -1;
}

int LCUI_UnbindSysEvent( int event_id, LCUI_EventFunc func )
{
	if( MainApp.driver_ready ) {
		return MainApp.driver->UnbindSysEvent( event_id, func );
	}
	return -1;
}

void *LCUI_GetAppData( void )
{
	if( MainApp.driver_ready ) {
		return MainApp.driver->GetData();
	}
	return NULL;
}

/** 退出所有主循环 */
static void LCUIApp_QuitAllMainLoop( void )
{
	LCUI_MainLoop loop;
	LinkedListNode *node;
	for( LinkedList_Each( node, &MainApp.loops ) ) {
		loop = node->data;
		if( loop ) {
			loop->state = STATE_EXITED;
		}
	}
}

/** 打印LCUI的信息 */
static void LCUI_ShowCopyrightText(void)
{
	Logger_Log(
		"LCUI (LC's UI) version "LCUI_VERSION"\n"
#ifdef _MSC_VER
		"Build tool: "
#if (_MSC_VER > 1900)
		"MS VC++ (higher version)"
#elif (_MSC_VER == 1900)
		"MS VC++ 14.0 (VisualStudio 2015)"
#elif (_MSC_VER == 1800)
		"MS VC++ 12.0 (VisualStudio 2013)"
#elif (_MSC_VER == 1700)
		"MS VC++ 11.0 (VisualStudio 2012)"
#elif (_MSC_VER == 1600)
		"MS VC++ 10.0 (VisualStudio 2010)"
#else
		"MS VC++ (older version)"
#endif
		"\n"
#endif
		"Build at "__DATE__" - "__TIME__"\n"
		"Copyright (C) 2012-2017 Liu Chao <root@lc-soft.io>.\n"
		"This is free software, licensed under GPLv2. \n"
		"See source distribution for detailed copyright notices.\n"
		"To learn more, visit http://www.lcui.org.\n\n"
	);
}

LCUI_BOOL LCUI_IsActive( void )
{
	if( System.state == STATE_ACTIVE ) {
		return TRUE;
	}
	return FALSE;
}

LCUI_BOOL LCUI_IsOnMainLoop( void )
{
	if( !MainApp.loop ) {
		return FALSE;
	}
	return (MainApp.loop->tid == LCUIThread_SelfID());
}

void LCUI_InitBase( void )
{
	if( System.state == STATE_ACTIVE ) {
		return;
	}
	System.exit_code = 0;
	System.state = STATE_ACTIVE;
	System.main_tid = LCUIThread_SelfID();
	LCUI_ShowCopyrightText();
	/* 初始化各个模块 */
	LCUI_InitEvent();
	LCUI_InitFontLibrary();
	LCUI_InitTimer();
	LCUI_InitKeyboard();
	LCUI_InitWidget();
	LCUI_InitMetrics();
}

void LCUI_Init( void )
{
	LCUI_InitBase();
	LCUI_InitApp( NULL );
	LCUI_InitMouseDriver();
	LCUI_InitKeyboardDriver();
	LCUI_InitDisplay( NULL );
	LCUI_InitCursor();
	LCUI_InitIME();
}

int LCUI_Destroy( void )
{
	LCUI_SysEventRec e;
	e.type = LCUI_QUIT;
	LCUI_TriggerEvent( &e, NULL );
	System.state = STATE_KILLED;
	LCUI_FreeDisplay();
	LCUI_FreeApp();
	LCUI_FreeIME();
	LCUI_FreeKeyboard();
	LCUI_FreeCursor();
	LCUI_FreeWidget();
	LCUI_FreeFontLibrary();
	LCUI_FreeTimer();
	LCUI_FreeEvent();
	LCUI_FreeMetrics();
	return System.exit_code;
}

void LCUI_Quit( void )
{
	System.state = STATE_KILLED;
	LCUIApp_QuitAllMainLoop();
}

void LCUI_Exit( int code )
{
	System.exit_code = code;
	LCUI_Quit();
}

int LCUI_Main( void )
{
	LCUI_MainLoop loop;
	loop = LCUIMainLoop_New();
	LCUIMainLoop_Run( loop );
	return LCUI_Destroy();
}

int LCUI_GetSelfVersion( char *out )
{
	return sprintf( out, "%s", LCUI_VERSION );
}
