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

static LCUI_Queue global_timer_list;	/* 定时器列表 */
static LCUI_BOOL timer_thread_active = TRUE;

/*----------------------------- Private ------------------------------*/
/* 初始化定时器列表 */
static void timer_list_init( LCUI_Queue *timer_list )
{
	Queue_Init( timer_list, sizeof(timer_data), NULL );
	/* 使用链表 */
	Queue_SetDataMode( timer_list, QUEUE_DATA_MODE_LINKED_LIST );
}

/* 销毁定时器列表 */
static void timer_list_destroy( LCUI_Queue *timer_list )
{
	Queue_Destroy( timer_list );
}

/* 对定时器列表进行排序 */
static void timer_list_sort( LCUI_Queue *timer_list )
{
	int i, j, total;
	timer_data *a_timer, *b_timer;
	
	Queue_Lock( timer_list );
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
	Queue_Unlock( timer_list );
}

/* 将各个定时器的等待时间与指定时间相减 */
static void timer_list_sub( LCUI_Queue *timer_list, int time )
{
	timer_data *timer;
	int i, total;
	
	Queue_Lock( timer_list );
	total = Queue_GetTotal( timer_list );
	
	for(i=0; i<total; ++i) {
		timer = (timer_data*)Queue_Get( timer_list , i);
		/* 忽略无效的定时器，或者状态为暂停的定时器 */
		if( !timer || timer->state == 0) {
			continue;
		}
		timer->cur_ms -= time;
	}
	Queue_Unlock( timer_list );
}

/* 更新定时器列表中的定时器 */
static timer_data* timer_list_update( LCUI_Queue *timer_list )
{
	int i, total;
	timer_data *timer = NULL;
	
	total = Queue_GetTotal( timer_list ); 
	for(i=0; i<total; ++i){
		timer = (timer_data*)Queue_Get( timer_list , i);
		if(timer->state == 1) {
			break;
		}
	}
	if(i >= total || !timer ) {
		return NULL; 
	}
	if(timer->cur_ms > 0) {
		LCUI_MSleep( timer->cur_ms ); 
	}
	/* 减少列表中所有定时器的剩余等待时间 */
	timer_list_sub( timer_list, timer->cur_ms );
	timer->cur_ms = timer->total_ms;
	timer_list_sort( timer_list ); /* 重新排序 */
	return timer;
}

/** 处理列表中各个定时器 */
static void timer_list_process( void *arg )
{
	int sleep_time = 1;
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
		timer = timer_list_update( timer_list );
		if( !timer ) {
			LCUI_MSleep( sleep_time );
			if(sleep_time < 100) {
				sleep_time += 1;
			}
			continue;
		}
		sleep_time = 1;
		func_data.id = timer->app_id;
		func_data.func = (CallBackFunc)timer->callback_func;
		func_data.arg[0] = timer->arg;
		func_data.destroy_arg[0] = FALSE;
		/* 添加该任务至指定程序的任务队列，添加模式是覆盖 */
		AppTasks_CustomAdd( ADD_MODE_REPLACE, &func_data );
	}
	LCUIThread_Exit(NULL);
}

static timer_data *find_timer( int timer_id )
{
	int i, total;
	timer_data *timer = NULL;
	Queue_Lock( &global_timer_list );
	total = Queue_GetTotal( &global_timer_list );
	for(i=0; i<total; ++i) {
		timer = Queue_Get( &global_timer_list, i );
		if( !timer ) {
			continue;
		}
		if( timer->id == timer_id ) {
			break;
		}
	}
	Queue_Unlock( &global_timer_list ); 
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
	timer.state = 1;
	timer.total_ms = timer.cur_ms = n_ms;
	timer.callback_func = callback_func;
	timer.reuse = reuse;
	timer.id = rand();
	timer.app_id = LCUIApp_GetSelfID();
	if( 0 > Queue_Add( &global_timer_list, &timer ) ) {
		return -1;
	}
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
	timer = find_timer( timer_id );
	if( timer ) {
		timer->state = 0;
		return 0;
	}
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
	timer = find_timer( timer_id );
	if( timer ) {
		timer->state = 1;
		return 0;
	}
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
	timer = find_timer( timer_id );
	if( timer ) {
		timer->total_ms = timer->cur_ms = n_ms;
		return 0;
	}
	return -1;
}

/* 创建一个线程以处理定时器 */
static int timer_thread_start( LCUI_Thread *tid, LCUI_Queue *list )
{
	/* 初始化列表 */
	timer_list_init( list );
	timer_thread_active = TRUE;
	/* 创建用于处理定时器列表的线程 */
	return _LCUIThread_Create( tid, timer_list_process, list );
}

/* 停止定时器的处理线程，并销毁定时器列表 */
static void timer_thread_destroy( LCUI_Thread tid, LCUI_Queue *list )
{
	timer_thread_active = FALSE;
	/* 等待定时器处理线程的退出 */
	_LCUIThread_Join( tid, NULL ); 
	/* 销毁定时器列表 */
	timer_list_destroy( list ); 
}

/* 初始化定时器模块 */
LCUI_API void LCUIModule_Timer_Init( void )
{
	timer_thread_start( &LCUI_Sys.timer_thread, &global_timer_list );
}

/* 停用定时器模块 */
LCUI_API void LCUIModule_Timer_End( void )
{
	timer_thread_destroy( LCUI_Sys.timer_thread, &global_timer_list );
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/

