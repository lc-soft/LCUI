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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
#include <LCUI/font.h>
#include <LCUI/input.h>
#include <LCUI/display.h>
#include <LCUI/ime.h>
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
	LCUI_BOOL is_inited;		/**< 标志，指示LCUI是否初始化过 */
	int state;			/**< 状态 */
	int mode;			/**< LCUI的运行模式 */
	unsigned long int main_tid;	/**< 主线程ID */
	struct {
		LCUI_EventTrigger trigger;	/**< 系统事件容器 */
		LCUI_Mutex mutex;		/**< 互斥锁 */
	} event;
} System = { 0 };

/** LCUI 应用程序数据 */
static struct LCUI_App {
	LCUI_Mutex loop_mutex;		/**< 互斥锁，确保一次只允许一个线程跑主循环 */
	LCUI_Cond loop_changed;		/**< 条件变量，用于指示当前运行的主循环是否改变 */
	LCUI_MainLoop loop;		/**< 当前运行的主循环 */
	LinkedList loops;		/**< 主循环列表 */
	LCUI_AppDriver driver;		/**< 程序事件驱动支持 */
	LCUI_BOOL driver_ready;		/**< 事件驱动支持是否已经准备就绪 */
	struct LCUI_AppTaskAgent {
		int state;		/**< 状态 */
		LinkedList tasks;	/**< 任务队列 */
		LCUI_Mutex mutex;	/**< 互斥锁 */
		LCUI_Cond cond;		/**< 条件变量 */
	} agent;
} MainApp = { 0 };

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
	if( !System.is_inited ) {
		return;
	}
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
	if( !System.is_inited ) {
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
	if( !System.is_inited ) {
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

void LCUI_DispatchEvent( void )
{
	LCUI_AppTask task;
	LinkedListNode *node;
	LCUIMutex_Lock( &MainApp.agent.mutex );
	node = LinkedList_GetNode( &MainApp.agent.tasks, 0 );
	if( node ) {
		task = node->data;
		LinkedList_Unlink( &MainApp.agent.tasks, node );
		LCUIMutex_Unlock( &MainApp.agent.mutex );
		LCUI_RunTask( task );
		LCUI_DeleteTask( task );
		free( task );
		free( node );
		return;
	} else {
		LCUIMutex_Unlock( &MainApp.agent.mutex );
	}
	if( MainApp.agent.state == STATE_RUNNING ) {
		return;
	}
	if( MainApp.driver_ready ) {
		MainApp.driver->DispatchEvent();
	}
}

LCUI_BOOL LCUI_PostTask( LCUI_AppTask task )
{
	LCUI_AppTask newtask;
	newtask = NEW( LCUI_AppTaskRec, 1 );
	*newtask = *task;
	LCUIMutex_Lock( &MainApp.agent.mutex );
	LinkedList_Append( &MainApp.agent.tasks, newtask );
	LCUICond_Signal( &MainApp.agent.cond );
	LCUIMutex_Unlock( &MainApp.agent.mutex );
	if( MainApp.driver_ready ) {
		return MainApp.driver->PostTask( newtask );
	}
	return TRUE;
}

void LCUI_DeleteTask( LCUI_AppTask task )
{
	if( task->destroy_arg[0] && task->arg[0] ) {
		task->destroy_arg[0]( task->arg[0] );
	}
	if( task->destroy_arg[1] && task->arg[1] ) {
		task->destroy_arg[1]( task->arg[1] );
	}
}

int LCUI_RunTask( LCUI_AppTask task )
{
	if( task && task->func ) {
		task->func( task->arg[0], task->arg[1] );
		return 0;
	}
	return -1;
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
	LCUI_BOOL at_same_thread = FALSE;
	if( loop->state == STATE_RUNNING ) {
		DEBUG_MSG("error: main-loop already running.");
		return -1;
	}
	loop->state = STATE_RUNNING;
	loop->tid = LCUIThread_SelfID();
	if( MainApp.loop ) {
		at_same_thread = MainApp.loop->tid == loop->tid;
	}
	DEBUG_MSG("at_same_thread: %d\n", at_same_thread);
	if( !at_same_thread ) {
		LCUIMutex_Lock( &MainApp.loop_mutex );
		LinkedList_Insert( &MainApp.loops, 0, loop );
		LCUIMutex_Unlock( &MainApp.loop_mutex );
	} else {
		LinkedList_Insert( &MainApp.loops, 0, loop );
	}
	DEBUG_MSG("loop: %p, enter\n", loop);
	MainApp.loop = loop;
	while( loop->state != STATE_EXITED ) {
		LCUI_WaitEvent();
		LCUI_DispatchEvent();
		/* 如果当前运行的主循环不是自己 */
		while( MainApp.loop != loop ) {
			loop->state = STATE_PAUSED;
			LCUICond_Wait( &MainApp.loop_changed, 
				       &MainApp.loop_mutex );
		}
	}
	loop->state = STATE_EXITED;
	LinkedList_Delete( &MainApp.loops, 0 );
	/* 获取处于列表表头的主循环 */
	loop = LinkedList_Get( &MainApp.loops, 0 );
	if( loop ) {
		/* 改变当前运行的主循环 */
		MainApp.loop = loop;
	}
	DEBUG_MSG("loop: %p, exit\n", loop);
	LCUICond_Broadcast( &MainApp.loop_changed );
	return 0;
}

/** 标记目标主循环需要退出 */
void LCUI_MainLoop_Quit( LCUI_MainLoop loop )
{
	loop->state = STATE_EXITED;
}

void LCUI_InitApp( LCUI_AppDriver app )
{
	if( MainApp.driver_ready ) {
		return;
	}
	MainApp.driver_ready = FALSE;
	MainApp.agent.state = STATE_RUNNING;
	if( !app ) {
		app = LCUI_CreateAppDriver();
		if( !app ) {
			return;
		}
	}
	MainApp.driver = app;
	MainApp.driver_ready = TRUE;
	LCUICond_Init( &MainApp.loop_changed );
	LCUIMutex_Init( &MainApp.loop_mutex );
	LinkedList_Init( &MainApp.loops );
	LCUIMutex_Init( &MainApp.agent.mutex );
	LCUICond_Init( &MainApp.agent.cond );
	LinkedList_Init( &MainApp.agent.tasks );
}

static void OnDeleteTask( void *arg )
{
	LCUI_DeleteTask( arg );
	free( arg );
}

static void LCUI_ExitApp( void )
{
	LCUI_MainLoop loop;
	LinkedListNode *node;
	for( LinkedList_Each( node, &MainApp.loops ) ) {
		loop = node->data;
		LCUI_MainLoop_Quit( loop );
		LCUIThread_Join( loop->tid, NULL );
	}
	LCUIMutex_Destroy( &MainApp.loop_mutex );
	LCUICond_Destroy( &MainApp.loop_changed );
	LCUIMutex_Destroy( &MainApp.agent.mutex );
	LCUICond_Destroy( &MainApp.agent.cond );
	LinkedList_Clear( &MainApp.loops, free );
	LinkedList_Clear( &MainApp.agent.tasks, OnDeleteTask );
	LCUI_DestroyAppDriver( MainApp.driver );
	MainApp.driver_ready = FALSE;
}

LCUI_BOOL LCUI_WaitEvent( void )
{
	if( MainApp.agent.tasks.length > 0 ) {
		return TRUE;
	}
	if( MainApp.agent.state != STATE_RUNNING ) {
		return MainApp.driver->WaitEvent();
	}
	LCUIMutex_Lock( &MainApp.agent.mutex );
	while( MainApp.agent.state == STATE_RUNNING ) {
		if( MainApp.agent.tasks.length > 0 ) {
			LCUIMutex_Unlock( &MainApp.agent.mutex );
			return TRUE;
		}
		LCUICond_Wait( &MainApp.agent.cond, &MainApp.agent.mutex );
	}
	LCUIMutex_Unlock( &MainApp.agent.mutex );
	return FALSE;
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

void LCUI_SetTaskAgent( LCUI_BOOL enabled )
{
	LCUIMutex_Lock( &MainApp.agent.mutex );
	if( enabled ) {
		MainApp.agent.state = STATE_RUNNING;
	} else {
		MainApp.agent.state = STATE_PAUSED;
	}
	LCUICond_Signal( &MainApp.agent.cond );
	LCUIMutex_Unlock( &MainApp.agent.mutex );
}

/** 退出所有主循环 */
static void LCUIApp_QuitAllMainLoop(void)
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
	Logger_Log( "           ________\n"
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
		"Copyright (C) 2012-2016 Liu Chao <lc-soft@live.cn>.\n"
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

void LCUI_InitBase( void )
{
	if( System.is_inited ) {
		return;
	}
	System.is_inited = TRUE;
	System.state = STATE_ACTIVE;
	System.main_tid = LCUIThread_SelfID();
	LCUI_ShowCopyrightText();
	/* 初始化各个模块 */
	LCUI_InitEvent();
	LCUI_InitFont();
	LCUI_InitTimer();
	LCUI_InitKeyboard();
	LCUI_InitIME();
	LCUI_InitWidget();
}

void LCUI_Init( void )
{
	LCUI_InitBase();
	LCUI_InitApp( NULL );
	LCUI_InitMouseDriver();
	LCUI_InitKeyboardDriver();
	LCUI_InitDisplay( NULL );
	LCUI_InitCursor();
}

int LCUI_Destroy( void )
{
	LCUI_SysEventRec e;
	e.type = LCUI_QUIT;
	LCUI_TriggerEvent( &e, NULL );
	System.state = STATE_KILLED;
	LCUI_ExitIME();
	LCUI_ExitKeyboard();
	LCUI_ExitEvent();
	LCUI_ExitCursor();
	LCUI_ExitWidget();
	LCUI_ExitFont();
	LCUI_ExitTimer();
	LCUI_ExitDisplay();
	LCUI_ExitApp();
	return 0;
}

void LCUI_Quit( void )
{
	System.state = STATE_KILLED;
	LCUIApp_QuitAllMainLoop();
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
