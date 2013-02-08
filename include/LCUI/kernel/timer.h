

#ifndef __LCUI_KERNEL_TIMER_H__
#define __LCUI_KERNEL_TIMER_H__

typedef struct _timer_data
{
	int state;		/* 状态 */
	BOOL reuse;		/* 是否重复使用该定时器 */
	LCUI_ID app_id;	/* 所属程序ID */
	long int id;		/* 定时器ID */
	long int total_ms;	/* 定时总时间（单位：毫秒） */
	long int cur_ms;	/* 当前剩下的等待时间 */
	void (*callback_func)(); /* 回调函数 */ 
}
timer_data;

LCUI_BEGIN_HEADER

/* 
 * 功能：设置定时器，在指定的时间后调用指定回调函数 
 * 说明：时间单位为毫秒，调用后会返回该定时器的标识符; 
 * 如果要用于循环定时处理某些任务，可将 reuse 置为 1，否则置于 0。
 * */
int set_timer( long int n_ms, void (*callback_func)(void), BOOL reuse );

/*
 * 功能：释放定时器
 * 说明：当不需要定时器时，可以使用该函数释放定时器占用的资源
 * 返回值：正常返回0，指定ID的定时器不存在则返回-1.
 * */
int free_timer( int timer_id );

/*
 * 功能：暂停定时器的使用 
 * 说明：一般用于往复定时的定时器
 * */
int pause_timer( int timer_id );

int continue_timer( int timer_id );
/* 继续使用定时器 */

/* 重设定时器的时间 */
int reset_timer( int timer_id, long int n_ms );

/* 创建一个线程以处理定时器 */
int timer_thread_start( thread_t *tid, LCUI_Queue *list );

/* 停止定时器的处理线程，并销毁定时器列表 */
void timer_thread_destroy( thread_t tid, LCUI_Queue *list );

/* 初始化定时器模块 */
void LCUIModule_Timer_Init( void );

/* 停用定时器模块 */
void LCUIModule_Timer_End( void );

LCUI_END_HEADER

#endif
