/* ***************************************************************************
 * timer.c -- timer support.
 *
 * Copyright (C) 2013-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2016 归属于 刘超 <lc-soft@live.cn>
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
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/timer.h>

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
	LinkedListNode node;		/**< 位于定时器列表中的节点 */
} TimerRec, *Timer;

static struct TimerModule {
	int id_count;			/**< 定时器ID计数 */
	LinkedList timer_list;		/**< 定时器数据记录 */
	LCUI_BOOL is_running;		/**< 定时器线程是否正在运行 */
	LCUI_Cond sleep_cond;		/**< 用于控制定时器睡眠的条件变量 */
	LCUI_Mutex mutex;		/**< 定时器记录操作互斥锁 */
	LCUI_Thread tid;		/**< 定时器处理线程ID */
} self;

/*----------------------------- Private ------------------------------*/

/** 更新定时器在定时器列表中的位置 */
static void TimerList_AddNode( LinkedListNode *node )
{
	Timer timer;
	int64_t t, tt;
	LinkedListNode *cur;
	/* 计算该定时器的剩余定时时长 */
	timer = node->data;
	t = LCUI_GetTimeDelta( timer->start_time );
	t = timer->total_ms - t + timer->pause_ms;
	for( LinkedList_Each( cur, &self.timer_list ) ) {
		timer = cur->data;
		tt = LCUI_GetTimeDelta( timer->start_time );
		tt = timer->total_ms - tt + timer->pause_ms;
		if( t <= tt ) {
			LinkedList_Link( &self.timer_list, cur->prev, node );
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
	int i = 0;
	Timer timer;
	LinkedListNode *node;
	_DEBUG_MSG("timer list(%d) start:\n", self.timer_list.length);
	for( LinkedList_Each( node, &self.timer_list ) {
		timer = node->data;
		_DEBUG_MSG("[%02d] %ld, func: %p, cur_ms: %ldms, total_ms: %ldms\n",
			i++, timer->id, timer->func, timer->total_ms - (long int)LCUI_GetTimeDelta(timer->start_time), timer->total_ms );
	}
	_DEBUG_MSG("timer list end\n\n");
}
#endif


/** 定时器线程，用于处理列表中各个定时器 */
static void TimerThread( void *arg )
{
	long int n_ms;
	int64_t lost_ms;
	LinkedListNode *node;
	LCUI_AppTaskRec task = {0};
	LCUIMutex_Lock( &self.mutex );
	while( self.is_running ) {
		Timer timer = NULL;
		for( LinkedList_Each( node, &self.timer_list ) ) {
			timer = node->data;
			if( timer && timer->state == STATE_RUN ) {
				break;
			}
		}
		/* 没有要处理的定时器，停留一段时间再进行下次循环 */
		if( !node ) {
			LCUIMutex_Unlock( &self.mutex );
			LCUI_MSleep( 10 );
			LCUIMutex_Lock( &self.mutex );
			continue;
		}
		lost_ms = LCUI_GetTimeDelta( timer->start_time );
		/* 减去处于暂停状态的时长 */
		lost_ms -= timer->pause_ms;
		/* 若流失的时间未达到总定时时长，则睡眠一段时间 */
		if( lost_ms < timer->total_ms ) {
			n_ms = timer->total_ms - lost_ms;
			LCUICond_TimedWait( &self.sleep_cond,
					    &self.mutex, n_ms );
			continue;
		}
		/* 准备任务数据 */
		task.func = (LCUI_AppTaskFunc)timer->func;
		task.arg[0] = timer->arg;
		/* 若需要重复使用，则重置剩余等待时间 */
		LinkedList_Unlink( &self.timer_list, node );
		if( timer->reuse ) {
			timer->pause_ms = 0;
			timer->start_time = LCUI_GetTime();
			TimerList_AddNode( node );
		} else {
			free( timer );
		}
		/* 添加该任务至指定程序的任务队列 */
		LCUI_PostTask( &task );
	}
	LCUIMutex_Unlock( &self.mutex );
	LCUIThread_Exit( NULL );
}

static Timer TimerList_Find( int timer_id )
{
	Timer timer;
	LinkedListNode *node;
	for( LinkedList_Each( node, &self.timer_list ) ) {
		timer = node->data;
		if( timer && timer->id == timer_id ) {
			return timer;
		}
	}
	return NULL;
}
/*--------------------------- End Private ----------------------------*/

/*----------------------------- Public -------------------------------*/

int LCUITimer_Set( long int n_ms, void (*func)(void*),
		   void *arg, LCUI_BOOL reuse )
{
	Timer timer;
	if( !self.is_running ) {
		return -1;
	}
	LCUIMutex_Lock( &self.mutex );
	timer = malloc( sizeof(TimerRec) );
	timer->arg = arg;
	timer->func = func;
	timer->reuse = reuse;
	timer->pause_ms = 0;
	timer->total_ms = n_ms;
	timer->state = STATE_RUN;
	timer->id = ++self.id_count;
	timer->start_time = LCUI_GetTime();
	timer->node.next = NULL;
	timer->node.prev = NULL;
	timer->node.data = timer;
	TimerList_AddNode( &timer->node );
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	DEBUG_MSG("set timer, id: %ld, total_ms: %ld\n", timer->id, timer->total_ms);
	return timer->id;
}

int LCUITimer_Free( int timer_id )
{
	Timer timer;
	if( !self.is_running ) {
		return -1;
	}
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( !timer ) {
		LCUIMutex_Unlock( &self.mutex );
		return -1;
	}
	LinkedList_Unlink( &self.timer_list, &timer->node );
	free( timer );
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return 0;
}

int LCUITimer_Pause( int timer_id )
{
	Timer timer;
	if( !self.is_running ) {
		return -1;
	}
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 记录暂停时的时间 */
		timer->pause_time = LCUI_GetTime();
		timer->state = STATE_PAUSE;
	}
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return timer ? 0:-1;
}

int LCUITimer_Continue( int timer_id )
{
	Timer timer;
	if( !self.is_running ) {
		return -1;
	}
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		/* 计算处于暂停状态的时长 */
		timer->pause_ms += (long int)LCUI_GetTimeDelta( timer->pause_time );
		timer->state = STATE_RUN;
	}
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return timer ? 0:-1;
}

int LCUITimer_Reset( int timer_id, long int n_ms )
{
	Timer timer;
	if( !self.is_running ) {
		return -1;
	}
	LCUIMutex_Lock( &self.mutex );
	timer = TimerList_Find( timer_id );
	if( timer ) {
		timer->pause_ms = 0;
		timer->total_ms = n_ms;
		timer->start_time = LCUI_GetTime();
	}
	LCUICond_Signal( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	return timer ? 0:-1;
}

void LCUI_InitTimer( void )
{
	LCUITime_Init();
	LCUIMutex_Init( &self.mutex );
	LCUICond_Init( &self.sleep_cond );
	LinkedList_Init( &self.timer_list );
	LCUIThread_Create( &self.tid, TimerThread, NULL );
	self.is_running = TRUE;
}

void LCUI_ExitTimer( void )
{
	self.is_running = FALSE;
	LCUIMutex_Lock( &self.mutex );
	LCUICond_Broadcast( &self.sleep_cond );
	LCUIMutex_Unlock( &self.mutex );
	LCUIThread_Join( self.tid, NULL );
	LinkedList_ClearData( &self.timer_list, free );
	LCUICond_Destroy( &self.sleep_cond );
	LCUIMutex_Destroy( &self.mutex );
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/
