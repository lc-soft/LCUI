/* ***************************************************************************
 * main.c -- The main functions for the LCUI normal work
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/linkedlist.h>
//#include LC_DISPLAY_H
#include LC_THREAD_H
//#include LC_CURSOR_H 
#include LC_INPUT_H
//#include LC_INPUT_METHOD_H
//#include LC_FONT_H 
//#include LC_WIDGET_H

#include <time.h>

typedef struct FuncDataRec_ {
	void (*func)(LCUI_SystemEvent*,void*);
	void *arg;
} FuncData;

/** LCUI 系统相关数据 */
static struct LCUI_System {
	int state;			/**< 状态 */ 
	int mode;			/**< LCUI的运行模式 */
	LCUI_BOOL is_inited;		/**< 标志，指示LCUI是否初始化过 */
	
	unsigned long int main_tid;	/**< 主线程ID */
	
	struct {
		LCUI_BOOL is_running;	/**< 是否处于运行状态 */
		LCUI_EventBox box;	/**< 系统事件容器 */
		LCUI_Cond cond;		/**< 条件变量 */
		LCUI_Thread tid;	/**< 线程ID */
	} event;

	int exit_code;			/**< 退出码 */
	void (*func_atexit)(void);	/**< 在LCUI退出时调用的函数 */
} System;
/***********************************************************************/

/** LCUI 应用程序数据 */
static struct LCUI_App {
	LinkedList task_list;		/**< 程序的任务队列 */
	LCUI_Mutex task_run_mutex;	/**< 任务互斥锁，当运行程序任务时会锁上 */
	LCUI_Mutex task_list_mutex;	/**< 任务记录互斥锁 */
	LCUI_Mutex loop_mutex;		/**< 互斥锁，确保一次只允许一个线程跑主循环 */
	LCUI_Mutex loop_changed;	/**< 互斥锁，用于指示当前运行的主循环是否改变 */
	LCUI_Cond loop_cond;		/**< 条件变量，用于决定是否继续任务循环 */
	LCUI_MainLoop *loop;		/**< 当前运行的主循环 */
} MainApp;

/*-------------------------- system event <START> ---------------------------*/

/** 系统事件处理线程 */
static void SystemEventThread(void *arg)
{
	LCUI_Event event;
	System.event.is_running = TRUE;
	while( System.event.is_running ) {
		LCUICond_Wait( &System.event.cond );
		if( LCUIEventBox_GetEvent( &System.event.box, &event ) != 0 ) {
			continue;
		}
		LCUIEventBox_Send( System.event.box, event.name, event.data );
		/* 删除这个事件的记录 */
		LCUIEventBox_DeleteEvent( System.event.box );
	}
}

/** 初始化事件模块 */
static void LCUIModule_Event_Init(void)
{
	LCUICond_Init( &System.event.cond );
	System.event.box = LCUIEventBox_Create();
	LCUIThread_Create( &System.event.tid, SystemEventThread, NULL );
}

/** 停用事件模块并进行清理 */
static void LCUIModule_Event_Exit(void)
{
	System.event.is_running = FALSE;
	LCUICond_Broadcast( &System.event.cond );
	LCUIThread_Join( System.event.tid, NULL );
	LCUICond_Destroy( &System.event.cond );
	LCUIEventBox_Destroy( System.event.box );
	System.event.box = NULL;
}

static void OnEvent( LCUI_Event *event, void *arg )
{
	FuncData *data = (FuncData*)arg;
	LCUI_SystemEvent *sys_event = (LCUI_SystemEvent*)event->data;
	sys_event->type_name = event->name;
	data->func( sys_event, data->arg );
}

/** 绑定事件 */
int LCUI_BindEvent( const char *event_name,
		    void(*func)(LCUI_SystemEvent*,void*),
		    void *func_arg, void (*arg_destroy)(void*) )
{
	FuncData *data;
	data = (FuncData*)malloc(sizeof(FuncData));
	data->func = func;
	data->arg = func_arg;
	return LCUIEventBox_Bind( System.event.box, event_name, 
					OnEvent, data, arg_destroy );
}

/** 解除事件绑定 */
int LCUI_UnbindEvent( int event_handler_id )
{
	return LCUIEventBox_Unbind( System.event.box, event_handler_id );
}

/** 投递事件 */
int LCUI_PostEvent( const char *name, LCUI_SystemEvent *event )
{
	int ret;
	LCUI_SystemEvent *sys_event;

	sys_event = (LCUI_SystemEvent*)malloc(sizeof(LCUI_SystemEvent));
	*sys_event = *event;
	ret = LCUIEventBox_Post( System.event.box, name, sys_event, free );
	if( ret == 0 ) {
		LCUICond_Broadcast( &System.event.cond );
	}
	return ret;
}

/*--------------------------- system event <END> ----------------------------*/

/*---------------------------- Main Loop <START> ----------------------------*/

static LinkedList loop_list;

enum MainLoopState {
	STATE_PAUSED,
	STATE_RUNNING,
	STATE_EXITED
};

typedef struct LCUI_MainLoopRec_ {
	int state;	/**< 主循环的状态 */
} LCUI_MainLoopRec;

/* 新建一个主循环 */
LCUI_MainLoop LCUI_MainLoop_New( void )
{
	LCUI_MainLoopRec *loop;
	loop = (LCUI_MainLoopRec*)malloc(sizeof(LCUI_MainLoopRec));
	loop->state = STATE_PAUSED;
	return (LCUI_MainLoop)loop;
}

static int LCUI_RunTask(void)
{ 
	LCUI_Task *task, task_bak;
	LCUIMutex_Lock( &MainApp.task_run_mutex );
	LCUIMutex_Lock( &MainApp.task_list_mutex );
	task = (LCUI_Task*)LinkedList_Get( &MainApp.task_list );
	if( !task ) {
		LCUIMutex_Unlock( &MainApp.task_list_mutex );
		LCUIMutex_Unlock( &MainApp.task_run_mutex );
		return -1;
	}
	if( !task->func ) {
		LinkedList_Delete( &MainApp.task_list );
		LCUIMutex_Unlock( &MainApp.task_list_mutex );
		LCUIMutex_Unlock( &MainApp.task_run_mutex );
		return -2;
	}
	/* 备份该任务数据 */
	task_bak = *task;
	/* 重置数据 */
	task->func = NULL;
	task->arg[0] = NULL;
	task->arg[1] = NULL;
	task->destroy_arg[0] = FALSE;
	task->destroy_arg[1] = FALSE;
	/* 删除之 */
	LinkedList_Delete( &MainApp.task_list );
	/* 解锁，现在的任务数据已经与任务列表独立开了 */
	LCUIMutex_Unlock( &MainApp.task_list_mutex );
	/* 调用函数指针指向的函数，并传递参数 */
	task_bak.func( task_bak.arg[0], task_bak.arg[1] );
	/* 若需要在调用回调函数后销毁参数 */
	if( task_bak.destroy_arg[0] ) {
		free( task_bak.arg[0] );
	}
	if( task_bak.destroy_arg[1] ) {
		free( task_bak.arg[1] );
	}
	LCUIMutex_Unlock( &MainApp.task_run_mutex );
	return 0;
}

/** 运行目标主循环 */
int LCUI_MainLoop_Run( LCUI_MainLoop *loop )
{
	LCUI_MainLoopRec *loop_rec = (LCUI_MainLoopRec*)loop;
	if( loop_rec->state == STATE_RUNNING ) {
		_DEBUG_MSG("error: main-loop already running.");
		return -1;
	}
	DEBUG_MSG("loop: %p, enter\n", loop);
	loop_rec->state = STATE_RUNNING;
	/* 将主循环记录插入至列表表头 */
	LinkedList_Goto( &loop_list, 0 );
	LinkedList_Insert( &loop_list, loop_rec );
	MainApp.loop = loop;
	LCUIMutex_Lock( &MainApp.loop_changed );
	/* 广播，让其它线程交出主循环运行权 */
	LCUICond_Broadcast( &MainApp.loop_cond );
	/* 获取运行权 */
	LCUIMutex_Lock( &MainApp.loop_mutex );
	LCUIMutex_Unlock( &MainApp.loop_changed );
	while( loop_rec->state != STATE_EXITED ) {
		if( LinkedList_GetTotal(&MainApp.task_list) <= 0 ) {
			LCUICond_TimedWait( &MainApp.loop_cond, 1000 );
			/** 如果当前运行的主循环不是自己 */
			if( MainApp.loop != loop ) {
				loop_rec->state = STATE_PAUSED;
				LCUIMutex_Unlock( &MainApp.loop_mutex );
				/* 等待其它线程获得主循环运行权 */
				LCUIMutex_Lock( &MainApp.loop_changed );
				LCUIMutex_Unlock( &MainApp.loop_changed );
				/* 等待其它线程释放主循环运行权 */
				LCUIMutex_Lock( &MainApp.loop_mutex );
			}
			continue;
		}
		LCUI_RunTask();
	}
	loop_rec->state = STATE_EXITED;
	LinkedList_Goto( &loop_list, 0 );
	LinkedList_Delete( &loop_list );
	/* 获取处于列表表头的主循环 */
	loop_rec = (LCUI_MainLoopRec*)LinkedList_Get( &loop_list );
	if( loop_rec ) {
		/* 改变当前运行的主循环 */
		MainApp.loop = (LCUI_MainLoop*)loop_rec;
		LCUICond_Broadcast( &MainApp.loop_cond );
	}
	/* 释放运行权 */
	LCUIMutex_Unlock( &MainApp.loop_mutex );
	DEBUG_MSG("loop: %p, exit\n", loop);
	return 0;
}

/** 标记目标主循环需要退出 */
int LCUI_MainLoop_Quit( LCUI_MainLoop *loop )
{

}

/*--------------------------- Main Loop <END> ---------------------------*/

/** 销毁程序任务 */
static void DestroyTask( void *arg )
{
	LCUI_Task *task;
	task = (LCUI_Task *)arg;
	if( task->destroy_arg[0] && task->arg[0] ) {
		free( task->arg[0] );
		task->arg[0] = NULL;
	}
	if( task->destroy_arg[1] && task->arg[1] ) {
		free( task->arg[1] );
		task->arg[1] = NULL;
	}
}

static LCUI_BOOL TaskIsEqual( LCUI_Task* t1, LCUI_Task* t2, int mode )
{
	if( t1->func != t2->func ) {
		return FALSE;
	}
	/* 如果要求是第1个参数不能重复 */
	if( HaveOption(mode, AND_ARG_F) ) {
		/* 如果要求是第2个参数也不能重复 */
		if( HaveOption(mode, AND_ARG_S) ) {
			/* 如果函数以及参数1和2都一样 */ 
			if(t1->arg[0] == t2->arg[0] 
			&& t1->arg[1] == t2->arg[1]) {
				return TRUE;
			}
			return FALSE;
		}
		/* 否则，只是要求函数以及第1个参数不能全部重复 */
		if( t1->arg[0] == t2->arg[0] ) { 
			return TRUE;
		}
		return FALSE;
	}
	/* 如果只是要求是第2个参数不能重复 */
	if( HaveOption(mode, AND_ARG_S) ) {
		if( t1->arg[1] == t2->arg[1] ) { 
			return TRUE;
		}
	}
	return FALSE;
}

static int Tasks_CustomAdd( LCUI_Queue *tasks, int mode, LCUI_Task *task )
{
	int n, i;
	LCUI_Task *exist_task = NULL;
	
	n = Queue_GetTotal(tasks);
	/* 如果模式是“添加新的”模式 */
	if( mode == ADD_MODE_ADD_NEW ) {
		Queue_Add( tasks, task );
		return 0;
	}
	for (i=0; i < n; ++i) { 
		exist_task = (LCUI_Task*)Queue_Get( tasks, i );
		/* 如果已存在的任务与当前任务匹配 */
		if( exist_task && TaskIsEqual( exist_task, task, mode ) ) {
			break;
		}
	}
	/* 如果没有已存在的任务与当前任务匹配 */
	if( i >= n || !exist_task ) {
		Queue_Add( tasks, task );
		return 0;
	}
	/* 如果任务不能重复 */
	if( HaveOption(mode, ADD_MODE_NOT_REPEAT) ) {
		n = -1;
	}
	/* 如果需要覆盖任务 */
	if( HaveOption(mode, ADD_MODE_REPLACE) )  {
		DestroyTask( exist_task );		/* 先销毁已存在的任务 */
		Queue_Replace( tasks, i, task );	/* 然后再覆盖 */
		n = 0;
	}
	return n;
}

/*
 * 功能：使用自定义方式添加程序任务
 * 用法示例：
 * 在函数的各参数与队列中的函数及各参数不重复时，添加它
 * LCUI_CustomAddTask(ADD_MODE_NOT_REPEAT | AND_ARG_F | AND_ARG_S, task);
 * 只要函数和参数1不重复则添加
 * LCUI_CustomAddTask(ADD_MODE_NOT_REPEAT | AND_ARG_F, task);
 * 要函数不重复则添加
 * LCUI_CustomAddTask(ADD_MODE_NOT_REPEAT, task);
 * 添加新的，不管是否有重复的
 * LCUI_CustomAddTask(ADD_MODE_ADD_NEW, task);
 * 有相同函数则覆盖，没有则新增
 * LCUI_CustomAddTask(ADD_MODE_REPLACE, task);
 * */
int LCUI_CustomAddTask( int mode, LCUI_Task *task )
{
	int ret;
	LCUIMutex_Lock( &MainApp.task_list_mutex );
	ret = Tasks_CustomAdd( &MainApp.task_list, mode, task );
	if( ret == 0 ) {
		LCUICond_Broadcast( &MainApp.loop_cond );
	} else if( ret == -1 ) {
		DestroyTask( task );
	}
	LCUIMutex_Unlock( &MainApp.task_list_mutex );
	return ret;
}

/** 锁住任务的运行 */
void LCUI_LockRunTask(void)
{
	LCUIMutex_Lock( &MainApp.task_run_mutex );
}

/** 解锁任务的运行 */
void LCUI_UnlockRunTask(void)
{
	LCUIMutex_Unlock( &MainApp.task_run_mutex );
}

/** 添加任务 */
int LCUI_AddTask( LCUI_Task *task )
{
	LCUIMutex_Lock( &MainApp.task_list_mutex );
	if( !LinkedList_AddDataCopy( &MainApp.task_list, task ) ) {
		LCUIMutex_Unlock( &MainApp.task_list_mutex );
		return -2;
	}
	LCUIMutex_Unlock( &MainApp.task_list_mutex );
	LCUICond_Broadcast( &MainApp.loop_cond );
	return 0;
}

/** 从程序任务队列中删除有指定回调函数的任务 */
int LCUI_RemoveTask( CallBackFunc task_func, LCUI_BOOL need_lock )
{
	int n;
	LCUI_Task *exist_task;

	if( need_lock ) {
		LCUIMutex_Lock( &MainApp.task_list_mutex );
	}
	n = LinkedList_GetTotal( &MainApp.task_list );
	for ( ; n>0; --n ) { 
		exist_task = (LCUI_Task*)LinkedList_Get( &MainApp.task_list );
		if( exist_task && exist_task->func == task_func ) {
			LinkedList_Delete( &MainApp.task_list );
		} else {
			LinkedList_ToNext( &MainApp.task_list );
		}
	}
	if( need_lock ) {
		LCUIMutex_Unlock( &MainApp.task_list_mutex );
	}
	return n;
}
/**************************** Task End ********************************/


/** 初始化程序数据结构体 */
static void LCUIApp_Init(void)
{
	LCUICond_Init( &MainApp.loop_cond );
	LCUIMutex_Init( &MainApp.task_run_mutex );
	LCUIMutex_Init( &MainApp.task_list_mutex );
	LinkedList_Init( &MainApp.task_list, sizeof(LCUI_Task) );
	LinkedList_SetDestroyFunc( &MainApp.task_list, DestroyTask );
	MainApp.main_loop_depth = 0;
}

/* 销毁程序占用的资源 */
static void LCUIApp_Destroy(void)
{
	MainApp.main_loop_depth = 0;
	MainApp.main_loop_running = FALSE;
	LCUICond_Broadcast( &MainApp.loop_cond );
	LCUIMutex_Destroy( &MainApp.task_run_mutex );
	LCUIMutex_Destroy( &MainApp.task_list_mutex );
}

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
	if(System.state == ACTIVE) {
		return TRUE;
	}
	return FALSE;
}

/** 检测当前是否在主线程上 */
LCUI_BOOL LCUI_IsOnMainThread(void)
{
	return (System.main_tid != LCUIThread_SelfID());
}

/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */
int LCUI_Init( int w, int h, int mode )
{
	/* 如果LCUI没有初始化过 */
	if( System.is_inited ) {
		return -1;
	}
	System.is_inited = TRUE;
	System.func_atexit = NULL;
	System.exit_code = 0;
	System.state = ACTIVE;
	System.main_tid = LCUIThread_SelfID();
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
	return 0;
}

/** 释放LCUI占用的资源 */
static int LCUI_Destroy( void )
{
	System.state = KILLED;
	if( System.func_atexit ) {
		System.func_atexit();
	}
	LCUIModule_IME_End();
	LCUIModule_Event_Exit();
	LCUIModule_Cursor_End();
	LCUIModule_Widget_End();
	LCUIModule_Font_End();
	LCUIModule_Timer_Exit();
	LCUIModule_Keyboard_End();
	//LCUIModule_Mouse_End();
	//LCUIModule_TouchScreen_End();
	LCUIModule_Device_End();
	LCUIModule_Video_End();
	LCUIApp_Destroy();
	return System.exit_code;
}

void LCUI_Quit(void)
{
	System.state = KILLED;
	LCUI_QuitAllMainLoops();
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
	LCUI_MainLoop *loop;
	loop = LCUI_MainLoop_New();
	LCUI_MainLoop_Run( loop );
	return LCUI_Destroy();
}

/** 注册一个函数，以在LCUI程序退出时调用 */
void LCUI_AtQuit( void (*callback_func)(void) )
{
	System.func_atexit = callback_func;
}

/* 获取LCUI的版本 */
int LCUI_GetSelfVersion( char *out )
{
	return sprintf(out, "%s", LCUI_VERSION);
}