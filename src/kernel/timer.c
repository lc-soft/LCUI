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
	int64_t start_time;		/**< 定时器启动时的时间 */
	int64_t pause_time;		/**< 定时器暂停时的时间 */
	long int total_ms;		/**< 定时时间（单位：毫秒） */
	long int pause_ms;		/**< 定时器处于暂停状态的时长（单位：毫秒） */
	void (*callback_func)(void*);	/**< 回调函数 */
	void *arg;			/**< 函数的参数 */
} timer_data;

static LCUI_Queue global_timer_list;		/**< 定时器列表 */
static LCUI_BOOL timer_thread_active = FALSE;	/**< 定时器线程是否活动 */
static LCUI_Sleeper timer_sleeper;		/**< 用于实现定时器睡眠的等待者 */

#define TIME_WRAP_VALUE (~(int64_t)0)

#ifdef LCUI_BUILD_IN_WIN32

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

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

#elif defined LCUI_BUILD_IN_LINUX
#include <sys/time.h>

LCUI_API void LCUI_StartTicks( void )
{
	return;
}

LCUI_API int64_t LCUI_GetTickCount( void )
{
	int64_t t;
	struct timeval tv;

	gettimeofday( &tv, NULL );
	t = tv.tv_sec*1000 + tv.tv_usec/1000;
	return t;
}

#endif

LCUI_API int64_t LCUI_GetTicks( int64_t start_ticks )
{
	int64_t now_ticks;

	now_ticks = LCUI_GetTickCount();
	if ( now_ticks < start_ticks ) {
		return (TIME_WRAP_VALUE-start_ticks) + now_ticks;
	}
	return now_ticks - start_ticks;
}


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

/** 更新定时器在定时器列表中的位置 */
static void TimerList_UpdateTimerPos(	LCUI_Queue *timer_list,
					timer_data *p_timer )
{
	int n, src_i=-1, des_i=-1;
	int64_t time_left, tmp_time_left;
	timer_data *p_tmp_timer;
	/* 计算该定时器的剩余定时时长 */
	time_left = LCUI_GetTicks( p_timer->start_time );
	time_left -= p_timer->pause_ms;
	time_left = p_timer->total_ms - time_left;
	/* 锁上定时器列表 */
	Queue_Lock( &global_timer_list );
	n = Queue_GetTotal( &global_timer_list );
	while(n--) {
		p_tmp_timer = (timer_data*)Queue_Get( &global_timer_list, n );
		if( !p_tmp_timer ) {
			continue;
		}
		/* 若找到自己的位置，则记录 */
		if( p_tmp_timer->id == p_timer->id ) {
			src_i = n;
			/* 如果已经找到目标位置，则退出循环 */
			if( des_i != -1 ) {
				break;
			}
			continue;
		}
		tmp_time_left = LCUI_GetTicks( p_tmp_timer->start_time );
		tmp_time_left -= p_tmp_timer->pause_ms;
		tmp_time_left = p_tmp_timer->total_ms - tmp_time_left;
		/* 若该定时器的剩余定时时长不大于当前定时器，则记录 */
		if( des_i == -1 && time_left >= tmp_time_left ) {
			DEBUG_MSG("src timer: %d, pos: %d, , cur_ms: %I64dms, des timer: %d, pos: %d, cur_ms: %I64dms\n",
				p_timer->id, src_i, LCUI_GetTicks(p_timer->start_time), 
				p_tmp_timer->id, des_i, LCUI_GetTicks(p_tmp_timer->start_time) );
			des_i = n;
			/* 如果已经找到源位置，则退出循环 */
			if( src_i != -1 ) {
				break;
			}
		}
	}
	/* 若目标位置无效，则将末尾作为目标位置 */
	if( des_i == -1 ) {
		DEBUG_MSG("tip\n");
		des_i = Queue_GetTotal( &global_timer_list )-1;
	}
	/* 若源位置和目标位置有效，则开始移动 */
	if( src_i != -1 ) {
		DEBUG_MSG("src: %d, des: %d\n", src_i, des_i );
		Queue_Move( &global_timer_list, des_i, src_i );
	}
	Queue_Unlock( &global_timer_list );
}

#ifdef DEBUG
/** 打印列表中的定时器信息 */
static void TimerList_Print( LCUI_Queue *timer_list )
{
	int i, total;
	timer_data *timer;

	total = Queue_GetTotal( timer_list );
	_DEBUG_MSG("timer list(%d) start:\n", total);
	for(i=0; i<total; ++i) {
		timer = (timer_data*)Queue_Get( timer_list , i);
		if( !timer ) {
			continue;
		}
		_DEBUG_MSG("[%02d] %ld, cur_ms: %dms, total_ms: %dms\n",
			i, timer->id, timer->total_ms - (long int)LCUI_GetTicks(timer->start_time), timer->total_ms );
	}
	_DEBUG_MSG("timer list end\n\n");
}
#endif

/** 定时器线程，用于处理列表中各个定时器 */
static void TimerThread( void *arg )
{
	int i, n;
	long int n_ms;
	LCUI_Func func_data;
	LCUI_Queue *timer_list;
	timer_data *timer = NULL;
	int64_t lost_ms;

	timer_list = (LCUI_Queue*)arg;
	func_data.arg[0] = NULL;
	func_data.arg[1] = NULL;

	while( !LCUI_Active() ) {
		LCUI_MSleep(10);
	}
	while( timer_thread_active ) {
		Queue_Lock( timer_list );
		n = Queue_GetTotal( timer_list );
		for(i=0; i<n; ++i) {
			timer = (timer_data*)Queue_Get( timer_list , i);
			if( !timer ) {
				continue;
			}
			if( timer->state == STATE_RUN ) {
				break;
			}
		}
		Queue_Unlock( timer_list );
		/* 没有要处理的定时器，停留一段时间再进行下次循环 */
		if(i >= n || !timer ) {
			LCUI_MSleep(10);
			continue;
		}
		lost_ms = LCUI_GetTicks( timer->start_time );
		/* 减去处于暂停状态的时长 */
		lost_ms -= timer->pause_ms;
		/* 若流失的时间未达到总定时时长 */
		if( lost_ms < timer->total_ms ) {
			Queue_Lock( timer_list );
			n_ms = timer->total_ms - lost_ms;
			/* 开始睡眠 */
			LCUISleeper_StartSleep( &timer_sleeper, n_ms );
			Queue_Unlock( timer_list );
			lost_ms = LCUI_GetTicks( timer->start_time );
			lost_ms -= timer->pause_ms;
			if( lost_ms < timer->total_ms ) {
				continue;
			}
		}
		DEBUG_MSG("timer: %d, start_time: %I64dms, cur_time: %I64dms, cur_ms: %I64d, total_ms: %ld\n", 
			timer->id, timer->start_time, LCUI_GetTickCount(), timer->total_ms-lost_ms, timer->total_ms);
		/* 准备任务数据 */
		func_data.id = timer->app_id;
		func_data.func = (CallBackFunc)timer->callback_func;
		func_data.arg[0] = timer->arg;
		func_data.destroy_arg[0] = FALSE;
		/* 添加该任务至指定程序的任务队列，添加模式是覆盖 */
		AppTasks_CustomAdd( ADD_MODE_REPLACE | AND_ARG_F, &func_data );
		Queue_Lock( timer_list );
		/* 若需要重复使用，则重置剩余等待时间 */
		if( timer->reuse ) {
			timer->start_time = LCUI_GetTickCount();
			timer->pause_ms = 0;
			TimerList_UpdateTimerPos( timer_list, timer );
		} else { /* 否则，释放该定时器 */
			LCUITimer_Free( timer->id );
		}
		Queue_Unlock( timer_list );
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
	int n;
	int64_t time_left;
	timer_data timer, *p_timer;
	static int id = 100;

	/* 打断定时器睡眠者的睡眠 */
	LCUISleeper_BreakSleep( &timer_sleeper );
	Queue_Lock( &global_timer_list );
	n = Queue_GetTotal( &global_timer_list );
	while(n--) {
		p_timer = (timer_data*)Queue_Get( &global_timer_list, n );
		if( !p_timer ) {
			continue;
		}
		time_left = LCUI_GetTicks( p_timer->start_time );
		time_left -= p_timer->pause_ms;
		time_left = p_timer->total_ms - time_left;
		if( time_left <= n_ms ) {
			break;
		}
	}

	timer.id = ++id;
	timer.app_id = LCUIApp_GetSelfID();
	timer.state = STATE_RUN;
	timer.reuse = reuse;
	timer.total_ms = n_ms;
	timer.pause_ms = 0;
	timer.start_time = LCUI_GetTickCount();
	timer.callback_func = callback_func;
	timer.arg = arg;

	Queue_Insert( &global_timer_list, n+1, &timer );
	Queue_Unlock( &global_timer_list );
	DEBUG_MSG("set timer, id: %d, total_ms: %d,app_id: %lu\n", timer.id, timer.total_ms, timer.app_id);
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
	
	LCUISleeper_BreakSleep( &timer_sleeper );
	Queue_Lock( &global_timer_list );
	total = Queue_GetTotal( &global_timer_list );
	for(i=0; i<total; ++i) {
		timer = (timer_data*)Queue_Get( &global_timer_list, i );
		if( !timer ) {
			continue;
		}
		if( timer->id == timer_id ) {
			DEBUG_MSG("delete timer: %d, n_ms: %d\n", timer->id, timer->total_ms);
			Queue_Delete( &global_timer_list, i );
			break;
		}
	}
	Queue_Unlock( &global_timer_list );
	if( i < total ) {
		LCUISleeper_BreakSleep( &timer_sleeper );
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
	
	LCUISleeper_BreakSleep( &timer_sleeper );
	Queue_Lock( &global_timer_list );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 记录暂停时的时间 */
		timer->pause_time = LCUI_GetTickCount();
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
	
	LCUISleeper_BreakSleep( &timer_sleeper );
	Queue_Lock( &global_timer_list );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 计算处于暂停状态的时长 */
		timer->pause_ms += (long int)LCUI_GetTicks( timer->pause_time );
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
	
	LCUISleeper_BreakSleep( &timer_sleeper );
	Queue_Lock( &global_timer_list );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		timer->start_time = LCUI_GetTickCount();
		timer->pause_ms = 0;
		timer->total_ms = n_ms;
		TimerList_UpdateTimerPos( &global_timer_list, timer );
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
	LCUISleeper_Create( &timer_sleeper );
	timer_thread_start( &LCUI_Sys.timer_thread, &global_timer_list );
}

/* 停用定时器模块 */
LCUI_API void LCUIModule_Timer_End( void )
{
	timer_thread_destroy( LCUI_Sys.timer_thread, &global_timer_list );
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/
