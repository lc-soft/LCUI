/* ***************************************************************************
 * timer.c -- timer support.
 * 
 * Copyright (C) 2013-2014 by Liu Chao <lc-soft@live.cn>
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
 * timer.c -- 定时器支持
 *
 * 版权所有 (C) 2013-2014 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/misc/delay.h>
#include LC_THREAD_H

#include <time.h>

#define STATE_RUN	1
#define STATE_PAUSE	0

/*----------------------------- Timer --------------------------------*/

typedef struct TimerDataRec_ {
	int state;			/**< 状态 */
	LCUI_BOOL reuse;		/**< 是否重复使用该定时器 */
	long int id;			/**< 定时器ID */
	int64_t start_time;		/**< 定时器启动时的时间 */
	int64_t pause_time;		/**< 定时器暂停时的时间 */
	long int total_ms;		/**< 定时时间（单位：毫秒） */
	long int pause_ms;		/**< 定时器处于暂停状态的时长（单位：毫秒） */
	void (*func)(void*);		/**< 回调函数 */
	void *arg;			/**< 函数的参数 */
} TimerData;

static LinkedList timer_list;			/**< 定时器数据记录 */
static LCUI_BOOL is_running = FALSE;		/**< 定时器线程是否正在运行 */
static LCUI_Cond timer_sleep_cond;		/**< 用于控制定时器睡眠的条件变量 */
static LCUI_Mutex timer_mutex;
static LCUI_Thread timer_thread_id;

#define TIME_WRAP_VALUE (~(int64_t)0)

#ifdef LCUI_BUILD_IN_WIN32

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

static BOOL hires_timer_available;	/**< 标志，指示高精度计数器是否可用 */
static double hires_ticks_per_second;	/**< 高精度计数器每秒的滴答数 */

void LCUI_StartTicks( void )
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

int64_t LCUI_GetTickCount( void )
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

void LCUI_StartTicks( void )
{
	return;
}

int64_t LCUI_GetTickCount( void )
{
	int64_t t;
	struct timeval tv;

	gettimeofday( &tv, NULL );
	t = tv.tv_sec*1000 + tv.tv_usec/1000;
	return t;
}

#endif

int64_t LCUI_GetTicks( int64_t start_ticks )
{
	int64_t now_ticks;

	now_ticks = LCUI_GetTickCount();
	if ( now_ticks < start_ticks ) {
		return (TIME_WRAP_VALUE-start_ticks) + now_ticks;
	}
	return now_ticks - start_ticks;
}


/*----------------------------- Private ------------------------------*/

/** 更新定时器在定时器列表中的位置 */
static void TimerList_UpdateTimerPos( TimerData *timer )
{
	int n, src_i=-1, des_i=-1;
	int64_t time_left, tmp_time_left;
	TimerData *tmp_timer;
	/* 计算该定时器的剩余定时时长 */
	time_left = LCUI_GetTicks( timer->start_time );
	time_left -= timer->pause_ms;
	time_left = timer->total_ms - time_left;
	/* 锁上定时器列表 */
	LCUIMutex_Lock( &timer_mutex );
	n = LinkedList_GetTotal( &timer_list );
	for(; n-- >= 0; LinkedList_ToNext(&timer_list) ) {
		tmp_timer = (TimerData*)LinkedList_Get( &timer_list );
		if( !tmp_timer ) {
			continue;
		}
		/* 若找到自己的位置，则记录 */
		if( tmp_timer->id == timer->id ) {
			src_i = n;
			/* 如果已经找到目标位置，则退出循环 */
			if( des_i != -1 ) {
				break;
			}
			continue;
		}
		tmp_time_left = LCUI_GetTicks( tmp_timer->start_time );
		tmp_time_left -= tmp_timer->pause_ms;
		tmp_time_left = tmp_timer->total_ms - tmp_time_left;
		/* 若该定时器的剩余定时时长不大于当前定时器，则记录 */
		if( des_i == -1 && time_left >= tmp_time_left ) {
			DEBUG_MSG("src timer: %d, pos: %d, , cur_ms: %I64dms, des timer: %d, pos: %d, cur_ms: %I64dms\n",
				timer->id, src_i, LCUI_GetTicks(timer->start_time), 
				tmp_timer->id, des_i, LCUI_GetTicks(tmp_timer->start_time) );
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
		des_i = 0;
	}
	/* 若源位置和目标位置有效，则开始移动 */
	if( src_i != -1 ) {
		DEBUG_MSG("src: %d, des: %d\n", src_i, des_i );
		LinkedList_Goto( &timer_list, src_i );
		LinkedList_MoveTo( &timer_list, des_i );
	}
	LCUIMutex_Unlock( &timer_mutex );
}

#define DEBUG_TIMER
#ifdef DEBUG_TIMER
/** 打印列表中的定时器信息 */
static void TimerList_Print( void )
{
	int i, n;
	TimerData *timer;

	n = LinkedList_GetTotal( &timer_list );
	_DEBUG_MSG("timer list(%d) start:\n", n);
	for( i=0; i<n; ++i, LinkedList_ToNext(&timer_list) ) {
		timer = (TimerData*)LinkedList_Get( &timer_list );
		if( !timer ) {
			continue;
		}
		_DEBUG_MSG("[%02d] %ld, func: %p, cur_ms: %dms, total_ms: %dms\n",
			i, timer->id, timer->func, timer->total_ms - (long int)LCUI_GetTicks(timer->start_time), timer->total_ms );
	}
	_DEBUG_MSG("timer list end\n\n");
}
#endif
/** 记录等待获取互斥锁的线程数 */
static int threads_of_wait_lock = 0;

/** 获取定时器列表的互斥锁 */
static void TimerList_GetLock(void)
{
	++threads_of_wait_lock;
	LCUICond_Broadcast( &timer_sleep_cond );
	LCUIMutex_Lock( &timer_mutex );
	if( threads_of_wait_lock > 0 ) {
		--threads_of_wait_lock;
	}
}

/** 释放定时器列表的互斥锁 */
static void TimerList_FreeLock(void)
{
	LCUIMutex_Unlock( &timer_mutex );
}

/** 等待其它线程获得线程锁，主要供定时器线程调用 */
static void TimerList_WaitOtherThreadGetLock(void)
{
	while(threads_of_wait_lock > 0);
}

/** 定时器线程，用于处理列表中各个定时器 */
static void TimerThread( void *arg )
{
	int i, n;
	long int n_ms;
	LCUI_Func func_data;
	TimerData *timer = NULL;
	int64_t lost_ms;

	func_data.arg[0] = NULL;
	func_data.arg[1] = NULL;
	while( is_running ) {
		LCUIMutex_Lock( &timer_mutex );
		n = LinkedList_GetTotal( &timer_list );
		for( i=0; i<n; ++i, LinkedList_ToNext(&timer_list) ) {
			timer = (TimerData*)LinkedList_Get( &timer_list );
			if( !timer ) {
				continue;
			}
			if( timer->state == STATE_RUN ) {
				break;
			}
		}
		LCUIMutex_Unlock( &timer_mutex );
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
			LCUIMutex_Lock( &timer_mutex );
			n_ms = timer->total_ms - lost_ms;
			/* 开始睡眠 */
			LCUICond_TimedWait( &timer_sleep_cond, n_ms );
			LCUIMutex_Unlock( &timer_mutex );
			TimerList_WaitOtherThreadGetLock();
			continue;
		}
		/* 准备任务数据 */
		func_data.func = (CallBackFunc)timer->func;
		func_data.arg[0] = timer->arg;
		func_data.destroy_arg[0] = FALSE;
		DEBUG_MSG("timer: %d, start_time: %I64dms, cur_time: %I64dms, cur_ms: %I64d, total_ms: %ld\n", 
			timer->id, timer->start_time, LCUI_GetTickCount(), timer->total_ms-lost_ms, timer->total_ms);
		/* 若需要重复使用，则重置剩余等待时间 */
		if( timer->reuse ) {
			timer->start_time = LCUI_GetTickCount();
			timer->pause_ms = 0;
			TimerList_UpdateTimerPos( timer );
		} else { /* 否则，释放该定时器 */
			LCUITimer_Free( timer->id );
		}
		DEBUG_MSG("add task: %p, timer id: %d\n", func_data.func, timer->id);
		/* 添加该任务至指定程序的任务队列 */
		LCUI_AddTask( &func_data );
	}
	LCUIThread_Exit(NULL);
}

static TimerData *TimerList_Find( int timer_id )
{
	int i, n;
	TimerData *timer = NULL;

	n = LinkedList_GetTotal( &timer_list );
	for( i=0; i<n; ++i ) {
		timer = (TimerData*)LinkedList_Get( &timer_list );
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
 * @param func
 *	用于响应定时器的回调函数
 * @param reuse
 *	指示该定时器是否重复使用，如果要用于循环定时处理某些
 *	任务，可将它置为 TRUE，否则置于 FALSE。
 * @return
 *	该定时器的标识符
 * */
int LCUITimer_Set( long int n_ms, void (*func)(void*),
			void *arg, LCUI_BOOL reuse )
{
	int n;
	int64_t time_left;
	TimerData timer, *timer_ptr;
	static int id = 100;

	TimerList_GetLock();
	n = LinkedList_GetTotal( &timer_list );
	for( ; n>0; --n, LinkedList_ToNext(&timer_list) ) {
		timer_ptr = (TimerData*)LinkedList_Get( &timer_list );
		if( !timer_ptr ) {
			continue;
		}
		time_left = LCUI_GetTicks( timer_ptr->start_time );
		time_left -= timer_ptr->pause_ms;
		time_left = timer_ptr->total_ms - time_left;
		if( time_left <= n_ms ) {
			break;
		}
	}
	
	timer.id = ++id;
	timer.state = STATE_RUN;
	timer.reuse = reuse;
	timer.total_ms = n_ms;
	timer.pause_ms = 0;
	timer.start_time = LCUI_GetTickCount();
	timer.func = func;
	timer.arg = arg;

	LinkedList_Goto( &timer_list, n+1 );
	LinkedList_InsertCopy( &timer_list, &timer );
	TimerList_FreeLock();
	DEBUG_MSG("set timer, id: %d, total_ms: %d,app_id: %lu\n", timer.id, timer.total_ms, timer.app_id);
	return timer.id;
}

/**
 * 释放定时器
 * 当不需要定时器时，可以使用该函数释放定时器占用的资源，并移除程序任务队列
 * 中还未处理的定时器任务
 * @param timer_id
 *	需要释放的定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
int LCUITimer_Free( int timer_id )
{
	int i, n;
	TimerData *timer;
	LCUI_BOOL need_lock;
	
	/* 如果当前与主循环不在同一线程上，则需要锁上程序任务锁 */
	need_lock = !LCUI_IsOnMainLoop();
	if( need_lock ) {
		LCUI_LockRunTask();
	}
	TimerList_GetLock();
	n = LinkedList_GetTotal( &timer_list );
	for( i=0; i<n; ++i ) {
		timer = (TimerData*)LinkedList_Get( &timer_list );
		/* 忽略无效或ID不一致的定时器 */
		if( !timer || timer->id != timer_id ) {
			continue;
		}
		/* 移除定时器任务，并且只在非主线程上时使用互斥锁 */
		LCUI_RemoveTask( (CallBackFunc)timer->func, need_lock );
		LinkedList_Delete( &timer_list );
		break;
	}
	TimerList_FreeLock();
	if( need_lock ) {
		LCUI_UnlockRunTask();
	}
	if( i < n ) {
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
int LCUITimer_Pause( int timer_id )
{
	TimerData *timer;
	TimerList_GetLock();
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 记录暂停时的时间 */
		timer->pause_time = LCUI_GetTickCount();
		timer->state = STATE_PAUSE;
		TimerList_FreeLock();
		return 0;
	}
	TimerList_FreeLock();
	return -1;
}

/**
 * 继续定时器的倒计时
 * @param timer_id
 *	目标定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
int LCUITimer_Continue( int timer_id )
{
	TimerData *timer;
	TimerList_GetLock();
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 计算处于暂停状态的时长 */
		timer->pause_ms += (long int)LCUI_GetTicks( timer->pause_time );
		timer->state = STATE_RUN;
		TimerList_FreeLock();
		return 0;
	}
	TimerList_FreeLock();
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
int LCUITimer_Reset( int timer_id, long int n_ms )
{
	TimerData *timer;
	
	TimerList_GetLock();
	timer = TimerList_Find( timer_id );
	if( timer ) {
		timer->start_time = LCUI_GetTickCount();
		timer->pause_ms = 0;
		timer->total_ms = n_ms;
		TimerList_UpdateTimerPos( timer );
		TimerList_FreeLock();
		return 0;
	}
	TimerList_FreeLock();
	return -1;
}

/* 初始化定时器模块 */
void LCUIModule_Timer_Init( void )
{
	LCUI_StartTicks();
	LinkedList_Init( &timer_list, sizeof(TimerData) );
	LinkedList_SetDataMemReuse( &timer_list, TRUE );
	LinkedList_SetDataNeedFree( &timer_list, TRUE );
	LCUICond_Init( &timer_sleep_cond );
	LCUIMutex_Init( &timer_mutex );
	LCUIThread_Create( &timer_thread_id, TimerThread, NULL );
}

/* 停用定时器模块 */
void LCUIModule_Timer_Exit( void )
{
	is_running = FALSE;
	LCUICond_Broadcast( &timer_sleep_cond );
	LCUIThread_Join( timer_thread_id, NULL );
	LCUICond_Destroy( &timer_sleep_cond );
	LCUIMutex_Destroy( &timer_mutex );
	LinkedList_Destroy( &timer_list );
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/
