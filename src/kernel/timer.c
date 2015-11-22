/* ***************************************************************************
 * timer.c -- timer support.
 * 
 * Copyright (C) 2013-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2015 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/linkedlist.h>
#include <LCUI/misc/delay.h>
#include <LCUI/thread.h>

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

static struct TimerModule {
	LinkedList timer_list;		/**< 定时器数据记录 */
	LCUI_BOOL is_running;		/**< 定时器线程是否正在运行 */
	LCUI_Cond sleep_cond;		/**< 用于控制定时器睡眠的条件变量 */
	LCUI_Mutex mutex;		/**< 定时器记录操作互斥锁 */
	LCUI_Thread thread_id;		/**< 定时器处理线程ID */
} self;

#define TIME_WRAP_VALUE (~(int64_t)0)
#define TimerNode(timer) (LinkedListNode*)(((char*)timer) + sizeof(TimerData))

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
static void TimerList_AddNode( LinkedListNode *node )
{
	int64_t t, tt;
	TimerData *timer;
	LinkedListNode *cur;
	/* 计算该定时器的剩余定时时长 */
	timer = (TimerData*)node->data;
	t = LCUI_GetTicks( timer->start_time );
	t = timer->total_ms - t + timer->pause_ms;
	LinkedList_ForEach( cur, &self.timer_list ) {
		timer = (TimerData*)cur->data;
		tt = LCUI_GetTicks( timer->start_time );
		tt = timer->total_ms - tt + timer->pause_ms;
		if( t <= tt ) {
			LinkedList_Link( &self.timer_list, cur, node );
			return;
		}
	}
	LinkedList_AppendNode( &self.timer_list, node );
}

//#define DEBUG_TIMER
#ifdef DEBUG_TIMER
/** 打印列表中的定时器信息 */
static void TimerList_Print( void )
{
	int i;
	TimerData *timer;

	_DEBUG_MSG("timer list(%d) start:\n", n);
	LinkedList_ForEach( timer, 0, &self.timer_list ) {
		if( !timer ) {
			continue;
		}
		_DEBUG_MSG("[%02d] %ld, func: %p, cur_ms: %ldms, total_ms: %ldms\n",
			i, timer->id, timer->func, timer->total_ms - (long int)LCUI_GetTicks(timer->start_time), timer->total_ms );
	}
	_DEBUG_MSG("timer list end\n\n");
}
#endif

/** 定时器线程，用于处理列表中各个定时器 */
static void TimerThread( void *arg )
{
	long int n_ms;
	LCUI_Task task = {0};
	TimerData *timer = NULL;
	LinkedListNode *node, *prev;
	int64_t lost_ms;

	self.is_running = TRUE;
	DEBUG_MSG("start\n");
	LCUIMutex_Lock( &self.mutex );
	while( self.is_running ) {
		LinkedList_ForEach( node, &self.timer_list ) {
			timer = (TimerData*)node->data;
			if( timer && timer->state == STATE_RUN ) {
				break;
			}
		}
		/* 没有要处理的定时器，停留一段时间再进行下次循环 */
		if( !node ) {
			LCUIMutex_Unlock( &self.mutex );
			LCUI_MSleep(10);
			LCUIMutex_Lock( &self.mutex );
			continue;
		}
		lost_ms = LCUI_GetTicks( timer->start_time );
		/* 减去处于暂停状态的时长 */
		lost_ms -= timer->pause_ms;
		/* 若流失的时间未达到总定时时长 */
		if( lost_ms < timer->total_ms ) {
			n_ms = timer->total_ms - lost_ms;
			/* 开始睡眠 */
			LCUICond_TimedWait( &self.sleep_cond, &self.mutex, n_ms );
			continue;
		}
		/* 准备任务数据 */
		task.func = (CallBackFunc)timer->func;
		task.arg[0] = timer->arg;
		DEBUG_MSG("timer: %ld, start_time: %ldms, cur_time: %ldms, cur_ms: %ld, total_ms: %ld\n", 
			timer->id, timer->start_time, LCUI_GetTickCount(), timer->total_ms-lost_ms, timer->total_ms);
		/* 若需要重复使用，则重置剩余等待时间 */
		if( timer->reuse ) {
			timer->start_time = LCUI_GetTickCount();
			timer->pause_ms = 0;
			LinkedList_Unlink( &self.timer_list, node );
			TimerList_AddNode( node );
		} else {
			prev = node->prev;
			LinkedList_DeleteNode( &self.timer_list, node );
			free( node->data );
			node = prev;
		}
		DEBUG_MSG( "add task: %p, timer id: %ld\n", task.func, timer->id );
		/* 添加该任务至指定程序的任务队列 */
		LCUI_AddTask( &task );
	}
	LCUIMutex_Unlock( &self.mutex );
	LCUIThread_Exit(NULL);
}

static TimerData *TimerList_Find( int timer_id )
{
	TimerData *timer;
	LinkedListNode *node;
	LinkedList_ForEach( node, &self.timer_list ) {
		timer = (TimerData*)node->data;
		if( timer && timer->id == timer_id ) {
			return timer;
		}
	}
	return NULL;
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
	TimerData *timer;
	LinkedListNode *node;
	static int id = 100;

	LCUIMutex_Lock( &self.mutex );
	timer = (TimerData*)malloc( sizeof(TimerData) +
				    sizeof(LinkedListNode) );
	node = TimerNode( timer );
	timer->id = ++id;
	timer->state = STATE_RUN;
	timer->reuse = reuse;
	timer->total_ms = n_ms;
	timer->pause_ms = 0;
	timer->start_time = LCUI_GetTickCount();
	timer->func = func;
	timer->arg = arg;
	node->data = timer;
	TimerList_AddNode( node );
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	DEBUG_MSG("set timer, id: %ld, total_ms: %ld\n", timer.id, timer.total_ms);
	return timer->id;
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
	TimerData *timer;
	LinkedListNode *node;
	
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( !timer ) {
		return -1;
	}
	node = TimerNode( timer );
	LinkedList_DeleteNode( &self.timer_list, node );
	free( timer );
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return 0;
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
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 记录暂停时的时间 */
		timer->pause_time = LCUI_GetTickCount();
		timer->state = STATE_PAUSE;
	}
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return timer ? 0:-1;
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
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 计算处于暂停状态的时长 */
		timer->pause_ms += (long int)LCUI_GetTicks( timer->pause_time );
		timer->state = STATE_RUN;
	}
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return timer ? 0:-1;
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
	
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		timer->start_time = LCUI_GetTickCount();
		timer->pause_ms = 0;
		timer->total_ms = n_ms;
	}
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return timer ? 0:-1;
}

/* 初始化定时器模块 */
void LCUI_InitTimer( void )
{
	LCUI_StartTicks();
	LinkedList_Init( &self.timer_list );
	LCUICond_Init( &self.sleep_cond );
	LCUIMutex_Init( &self.mutex );
	LCUIThread_Create( &self.thread_id, TimerThread, NULL );
}

/* 停用定时器模块 */
void LCUI_ExitTimer( void )
{
	self.is_running = FALSE;
	LCUICond_Broadcast( &self.sleep_cond );
	LCUIThread_Join( self.thread_id, NULL );
	LCUICond_Destroy( &self.sleep_cond );
	LCUIMutex_Destroy( &self.mutex );
	LinkedList_Clear( &self.timer_list, free );
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/
