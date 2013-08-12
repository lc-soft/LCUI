/* ***************************************************************************
 * timer.c -- The timer module for LCUI
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
 * timer.c -- LCUI 的定时器模块
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
#include <LCUI_Build.h>
#include LC_LCUI_H

#include <time.h>

#define STATE_RUN	1
#define STATE_PAUSE	0

/*----------------------------- Timer --------------------------------*/

typedef struct _timer_data {
	int state;			/**< 状态 */
	LCUI_BOOL reuse;		/**< 是否重复使用该定时器 */
	LCUI_ID app_id;			/**< 所属程序ID */
	long int id;			/**< 定时器ID */
	long int total_ms;		/**< 定时总时间（单位：毫秒） */
	long int cur_ms;		/**< 当前剩下的等待时间 */
	void (*callback_func)(void*);	/**< 回调函数 */
	void *arg;			/**< 函数的参数 */
} timer_data;

static LCUI_Queue global_timer_list;		/**< 定时器列表 */
static LCUI_BOOL timer_thread_active = FALSE;	/**< 定时器线程是否活动 */
static LCUI_Sleeper global_timer_waiter;	/**< 用于实现定时器睡眠的等待者 */

#ifdef LCUI_BUILD_IN_WIN32

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#define TIME_WRAP_VALUE	(~(int64_t)0)

static BOOL hires_timer_available;	/**< 标志，指示高精度计数器是否可用 */
static double hires_ticks_per_second;	/**< 高精度计数器每秒的滴答数 */

LCUI_API void LCUI_StartTicks( void )
{
	LARGE_INTEGER hires;

	if( QueryPerformanceFrequency(&hires) ) {
		hires_timer_available = TRUE;
		hires_ticks_per_second = hires.QuadPart;
	} else {
		hires_timer_available = FALSE;
		timeBeginPeriod(1);
	}
}

LCUI_API int64_t LCUI_GetTickCount( void )
{
	LARGE_INTEGER hires_now;
	
	if (hires_timer_available) {
		QueryPerformanceCounter(&hires_now);
		hires_now.QuadPart *= 1000;
		hires_now.QuadPart /= hires_ticks_per_second;
		return (int64_t)hires_now.QuadPart;
	}
	return (int64_t)timeGetTime();
}

LCUI_API int64_t LCUI_GetTicks( int64_t start_ticks )
{
	int64_t now_ticks;

	now_ticks = LCUI_GetTickCount();
	if ( now_ticks < start_ticks ) {
		return (TIME_WRAP_VALUE-start_ticks) + now_ticks;
	}
	return now_ticks - start_ticks;
}

#elif defined LCUI_BUILD_IN_LINUX

#endif

/*----------------------------- Private ------------------------------*/

/** 初始化定时器列表 */
static void TimerList_Init( LCUI_Queue *timer_list )
{
	Queue_Init( timer_list, sizeof(timer_data), NULL );
	/* 使用链表 */
	Queue_SetDataMode( timer_list, QUEUE_DATA_MODE_LINKED_LIST );
}

/** 销毁定时器列表 */
static void TimerList_Destroy( LCUI_Queue *timer_list )
{
	Queue_Destroy( timer_list );
}

/** 对定时器列表进行排序,不使用互斥锁 */
static void TimerList_NoLockSort( LCUI_Queue *timer_list )
{
	int i, j, total;
	timer_data *a_timer, *b_timer;

	total = Queue_GetTotal( timer_list );
	/* 使用的是选择排序,按剩余等待时间从少到多排序 */
	for(i=0; i<total; ++i) {
		a_timer = (timer_data*)Queue_Get( timer_list, i );
		if( !a_timer ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			b_timer = (timer_data*)Queue_Get( timer_list, j );
			if( !b_timer ) {
				continue;
			}
			if( b_timer->cur_ms < a_timer->cur_ms ) {
				Queue_Swap( timer_list , j, i);
				a_timer = b_timer;
			}
		}
	}
}

/** 对定时器列表进行排序 */
static void TimerList_Sort( LCUI_Queue *timer_list )
{
	Queue_Lock( timer_list );
	TimerList_NoLockSort( timer_list );
	Queue_Unlock( timer_list );
}

/** 缩短定时器列表中的各个定时器的等待时间，不使用互斥锁 */
static void TimerList_NoLockShortenTime( LCUI_Queue *timer_list, uint_t n_ms )
{
	int i, total;
	timer_data *timer;

	total = Queue_GetTotal( timer_list );
	for(i=0; i<total; ++i) {
		timer = (timer_data*)Queue_Get( timer_list , i);
		/* 忽略无效的定时器，或者状态为暂停的定时器 */
		if( !timer || timer->state == STATE_PAUSE ) {
			continue;
		}
		timer->cur_ms -= n_ms;
	}
}

/** 更新定时器列表中的定时器 */
static timer_data* TimerList_Update( LCUI_Queue *timer_list )
{
	int i, total;
	timer_data *timer = NULL;
	uint_t lost_ms;

	total = Queue_GetTotal( timer_list );
	for(i=0; i<total; ++i){
		timer = (timer_data*)Queue_Get( timer_list , i);
		if( timer == NULL ) {
			continue;
		}
		if( timer->state == STATE_RUN ) {
			break;
		}
	}
	/* 没有要处理的定时器，则返回-1 */
	if(i >= total || !timer ) {
		return (timer_data*)-1;
	}
	if(timer->cur_ms > 0) {
		Queue_Lock( timer_list );
		lost_ms = LCUISleeper_StartSleep( global_timer_waiter, timer->cur_ms );
		/* 减少列表中所有定时器的剩余等待时间 */
		TimerList_NoLockShortenTime( timer_list, lost_ms );
		Queue_Unlock( timer_list );
		DEBUG_MSG("timer id: %d, lost_time: %d, timer->cur_ms: %d,"
			" timer->total_ms: %d\n",
			timer->id, lost_time, timer->cur_ms, timer->total_ms );
	}
	/* 若定时器已经到了响应时间，则返回该定时器 */
	if( timer->cur_ms <= 0 ) {
		return timer;
	}
	/* 否则返回NULL，表示还没有要处理的定时器 */
	return NULL;
}

/** 定时器线程，用于处理列表中各个定时器 */
static void TimerThread( void *arg )
{
	LCUI_Func func_data;
	LCUI_Queue *timer_list;
	timer_data *timer;

	timer_list = (LCUI_Queue*)arg;
	func_data.arg[0] = NULL;
	func_data.arg[1] = NULL;

	while( !LCUI_Active() ) {
		LCUI_MSleep(10);
	}
	while( LCUI_Active() && timer_thread_active ) {
		timer = TimerList_Update( timer_list );
		if( timer == (timer_data*)-1 ) {
			LCUI_MSleep( 5 );
			continue;
		}
		if( timer == NULL ) {
			continue;
		}
		func_data.id = timer->app_id;
		func_data.func = (CallBackFunc)timer->callback_func;
		func_data.arg[0] = timer->arg;
		func_data.destroy_arg[0] = FALSE;
		/* 添加该任务至指定程序的任务队列，添加模式是覆盖 */
		AppTasks_CustomAdd( ADD_MODE_REPLACE | AND_ARG_F, &func_data );
		/* 若需要重复使用，则重置剩余等待时间 */
		if( timer->reuse ) {
			timer->cur_ms = timer->total_ms;
		} else { /* 否则，释放该定时器 */
			LCUITimer_Free( timer->id );
			DEBUG_MSG("delete timer: %d\n", timer->id);
		}
		/* 重新排序 */
		TimerList_Sort( timer_list );
	}
	LCUIThread_Exit(NULL);
}

static timer_data *TimerList_Find( int timer_id )
{
	int i, total;
	timer_data *timer = NULL;

	total = Queue_GetTotal( &global_timer_list );
	for(i=0; i<total; ++i) {
		timer = (timer_data*)Queue_Get( &global_timer_list, i );
		if( !timer ) {
			continue;
		}
		if( timer->id == timer_id ) {
			break;
		}
	}
	return timer;
}
/*--------------------------- End Private ----------------------------*/

/*----------------------------- Public -------------------------------*/

/**
 * 设置定时器
 * 定时器的作用是让一个任务在经过指定时间后才执行
 * @param n_ms
 *	等待的时间，单位为毫秒
 * @param callback_func
 *	用于响应定时器的回调函数
 * @param reuse
 *	指示该定时器是否重复使用，如果要用于循环定时处理某些
 *	任务，可将它置为 TRUE，否则置于 FALSE。
 * @return
 *	该定时器的标识符
 * */
LCUI_API int LCUITimer_Set(	long int n_ms,
				void (*callback_func)(void*),
				void *arg,
				LCUI_BOOL reuse )
{
	timer_data timer;
	
	timer.state = STATE_RUN;
	timer.total_ms = timer.cur_ms = n_ms;
	timer.callback_func = callback_func;
	timer.reuse = reuse;
	timer.id = rand();
	timer.app_id = LCUIApp_GetSelfID();
	timer.arg = arg;
	
	/* 打断定时器睡眠者的睡眠 */
	LCUISleeper_BreakSleep( global_timer_waiter );

	Queue_Lock( &global_timer_list );
	if( 0 > Queue_Add( &global_timer_list, &timer ) ) {
		Queue_Unlock( &global_timer_list );
		return -1;
	}
	DEBUG_MSG("set timer, id: %d, total_ms: %d\n", timer.id, timer.total_ms);
	TimerList_NoLockSort( &global_timer_list );
	Queue_Unlock( &global_timer_list );
	return timer.id;
}

/**
 * 释放定时器
 * 当不需要定时器时，可以使用该函数释放定时器占用的资源
 * @param timer_id
 *	需要释放的定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Free( int timer_id )
{
	int i, total;
	timer_data *timer;
	
	LCUISleeper_BreakSleep( global_timer_waiter );
	Queue_Lock( &global_timer_list );
	total = Queue_GetTotal( &global_timer_list );
	for(i=0; i<total; ++i) {
		timer = (timer_data*)Queue_Get( &global_timer_list, i );
		if( !timer ) {
			continue;
		}
		if( timer->id == timer_id ) {
			Queue_Delete( &global_timer_list, i );
			break;
		}
	}
	Queue_Unlock( &global_timer_list );
	if( i < total ) {
		LCUISleeper_BreakSleep( global_timer_waiter );
		return 0;
	}
	return -1;
}

/**
 * 暂停定时器的倒计时
 * 一般用于往复定时的定时器
 * @param timer_id
 *	目标定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Pause( int timer_id )
{
	timer_data *timer;
	
	LCUISleeper_BreakSleep( global_timer_waiter );
	Queue_Lock( &global_timer_list );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		timer->state = STATE_PAUSE;
		Queue_Unlock( &global_timer_list );
		return 0;
	}
	Queue_Unlock( &global_timer_list );
	return -1;
}

/**
 * 继续定时器的倒计时
 * @param timer_id
 *	目标定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Continue( int timer_id )
{
	timer_data *timer;
	
	LCUISleeper_BreakSleep( global_timer_waiter );
	Queue_Lock( &global_timer_list );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		timer->state = STATE_RUN;
		Queue_Unlock( &global_timer_list );
		return 0;
	}
	Queue_Unlock( &global_timer_list );
	return -1;
}

/**
 * 重设定时器的等待时间
 * @param timer_id
 *	需要释放的定时器的标识符
 * @param n_ms
 *	等待的时间，单位为毫秒
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Reset( int timer_id, long int n_ms )
{
	timer_data *timer;
	
	LCUISleeper_BreakSleep( global_timer_waiter );
	Queue_Lock( &global_timer_list );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 更新当前剩余等待时间 */
		timer->cur_ms = timer->total_ms;
		/* 保存新的总剩余等待时间 */
		timer->total_ms = n_ms;
		TimerList_Sort( &global_timer_list );
		Queue_Unlock( &global_timer_list );
		return 0;
	}
	Queue_Unlock( &global_timer_list );
	return -1;
}

/* 创建一个线程以处理定时器 */
static int timer_thread_start( LCUI_Thread *tid, LCUI_Queue *list )
{
	/* 初始化列表 */
	TimerList_Init( list );
	timer_thread_active = TRUE;
	/* 创建用于处理定时器列表的线程 */
	return _LCUIThread_Create( tid, TimerThread, list );
}

/* 停止定时器的处理线程，并销毁定时器列表 */
static void timer_thread_destroy( LCUI_Thread tid, LCUI_Queue *list )
{
	timer_thread_active = FALSE;
	/* 等待定时器处理线程的退出 */
	_LCUIThread_Join( tid, NULL );
	/* 销毁定时器列表 */
	TimerList_Destroy( list );
}

/* 初始化定时器模块 */
LCUI_API void LCUIModule_Timer_Init( void )
{
	LCUI_StartTicks();
	global_timer_waiter = LCUISleeper_New();
	timer_thread_start( &LCUI_Sys.timer_thread, &global_timer_list );
}

/* 停用定时器模块 */
LCUI_API void LCUIModule_Timer_End( void )
{
	timer_thread_destroy( LCUI_Sys.timer_thread, &global_timer_list );
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/

