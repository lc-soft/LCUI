#include <LCUI_Build.h>
#include LC_LCUI_H
#include <unistd.h>

/*----------------------------- Timer --------------------------------*/
/*----------------------------- Private ------------------------------*/
/* 功能：初始化定时器列表 */
static void 
timer_list_init( LCUI_Queue *timer_list )
{
	Queue_Init( timer_list, sizeof(timer_data), NULL );
	/* 使用链表 */
	Queue_Set_DataMode( timer_list, QUEUE_DATA_MODE_LINKED_LIST );
}
/* 功能：销毁定时器列表 */
static void 
timer_list_destroy( LCUI_Queue *timer_list )
{
	Destroy_Queue( timer_list );
}

/* 功能：对定时器列表进行排序 */
static void 
timer_list_sort( LCUI_Queue *timer_list )
{
	int i, j, total;
	timer_data *a_timer, *b_timer;
	
	Queue_Lock( timer_list );
	total = Queue_Get_Total( timer_list );
	/* 使用的是选择排序,按剩余等待时间从少到多排序 */
	for(i=0; i<total; ++i) {
		a_timer = Queue_Get( timer_list, i );
		if( !a_timer ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			b_timer = Queue_Get( timer_list, j );
			if( !b_timer ) {
				continue; 
			}
			if( b_timer->cur_ms < a_timer->cur_ms ) {
				Queue_Swap( timer_list , j, i);
				a_timer = b_timer;
			}
		}
	}
	Queue_UnLock( timer_list );
}

/* 功能：将各个定时器的等待时间与指定时间相减 */
static void 
timer_list_sub( LCUI_Queue *timer_list, int time )
{
	timer_data *timer;
	int i, total;
	
	Queue_Lock( timer_list );
	total = Queue_Get_Total( timer_list );
	
	for(i=0; i<total; ++i) {
		timer = Queue_Get( timer_list , i);
		/* 忽略无效的定时器，或者状态为暂停的定时器 */
		if( !timer || timer->state == 0) {
			continue;
		}
		timer->cur_ms -= time;
	}
	Queue_UnLock( timer_list );
}

static timer_data *
timer_list_update( LCUI_Queue *timer_list )
/* 功能：更新定时器列表中的定时器 */
{
	int i, total;
	timer_data *timer = NULL;
	
	total = Queue_Get_Total( timer_list ); 
	for(i=0; i<total; ++i){
		timer = Queue_Get( timer_list , i);
		if(timer->state == 1) {
			break;
		}
	}
	if(i >= total || !timer ) {
		return NULL; 
	}
	if(timer->cur_ms > 0) {
		usleep( timer->cur_ms*1000 ); 
	}
	/* 减少列表中所有定时器的剩余等待时间 */
	timer_list_sub( timer_list, timer->cur_ms );
	timer->cur_ms = timer->total_ms;
	timer_list_sort( timer_list ); /* 重新排序 */
	return timer;
}

static BOOL timer_thread_active = TRUE;

/* 一个线程，用于处理定时器 */
static void *
timer_list_process( void *arg )
{
	int sleep_time = 1000;
	LCUI_Func func_data;
	LCUI_Queue *timer_list;
	timer_data *timer;
	
	timer_list = (LCUI_Queue*)arg;
	func_data.arg[0] = NULL;
	func_data.arg[1] = NULL;
	
	while( !LCUI_Active() ) {
		usleep(10000);
	}
	while( LCUI_Active() || timer_thread_active ) { 
		timer = timer_list_update( timer_list );
		if( !timer ) {
			usleep( sleep_time );
			if(sleep_time < 500000) {
				sleep_time += 1000;
			}
			continue;
		}
		sleep_time = 1000;
		func_data.id = timer->app_id;
		func_data.func = timer->callback_func;
		/* 添加该任务至指定程序的任务队列，添加模式是覆盖 */
		AppTasks_CustomAdd( ADD_MODE_REPLACE, &func_data );
	}
	LCUI_Thread_Exit(NULL);
}

static timer_data *
find_timer( int timer_id )
{
	int i, total;
	timer_data *timer = NULL;
	Queue_Lock( &LCUI_Sys.timer_list );
	total = Queue_Get_Total( &LCUI_Sys.timer_list );
	for(i=0; i<total; ++i) {
		timer = Queue_Get( &LCUI_Sys.timer_list, i );
		if( !timer ) {
			continue;
		}
		if( timer->id == timer_id ) { 
			break;
		}
	}
	Queue_UnLock( &LCUI_Sys.timer_list ); 
	return timer;
}
/*--------------------------- End Private ----------------------------*/

/*----------------------------- Public -------------------------------*/
/* 
 * 功能：设置定时器，在指定的时间后调用指定回调函数 
 * 说明：时间单位为毫秒，调用后会返回该定时器的标识符; 
 * 如果要用于循环定时处理某些任务，可将 reuse 置为 1，否则置于 0。
 * */
int set_timer( long int n_ms, void (*callback_func)(void), BOOL reuse )
{
	timer_data timer;
	timer.state = 1;
	timer.total_ms = timer.cur_ms = n_ms;
	timer.callback_func = callback_func;
	timer.reuse = reuse;
	timer.id = rand();
	timer.app_id = LCUIApp_GetSelf()->id;
	if( 0 > Queue_Add( &LCUI_Sys.timer_list, &timer ) ) {
		return -1;
	}
	return timer.id;
}

/*
 * 功能：释放定时器
 * 说明：当不需要定时器时，可以使用该函数释放定时器占用的资源
 * 返回值：正常返回0，指定ID的定时器不存在则返回-1.
 * */
int free_timer( int timer_id )
{
	int i, total;
	timer_data *timer;
	
	Queue_Lock( &LCUI_Sys.timer_list );
	total = Queue_Get_Total( &LCUI_Sys.timer_list );
	for(i=0; i<total; ++i) {
		timer = Queue_Get( &LCUI_Sys.timer_list, i );
		if( !timer ) {
			continue;
		}
		if( timer->id == timer_id ) {
			Queue_Delete( &LCUI_Sys.timer_list, i );
			break;
		}
	}
	Queue_UnLock( &LCUI_Sys.timer_list );
	if( i < total ) {
		return 0;
	}
	return -1;
}

/*
 * 功能：暂停定时器的使用 
 * 说明：一般用于往复定时的定时器
 * */
int pause_timer( int timer_id )
{
	timer_data *timer;
	timer = find_timer( timer_id );
	if( timer ) {
		timer->state = 0;
		return 0;
	}
	return -1;
}

int continue_timer( int timer_id )
/* 继续使用定时器 */
{
	timer_data *timer;
	timer = find_timer( timer_id );
	if( timer ) {
		timer->state = 1;
		return 0;
	}
	return -1;
}

/* 重设定时器的时间 */
int reset_timer( int timer_id, long int n_ms ) 
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
int timer_thread_start( thread_t *tid, LCUI_Queue *list )
{
	/* 初始化列表 */
	timer_list_init( list );
	timer_thread_active = TRUE;
	/* 创建用于处理定时器列表的线程 */
	return thread_create( tid, NULL, timer_list_process, list );
}

/* 停止定时器的处理线程，并销毁定时器列表 */
void timer_thread_destroy( thread_t tid, LCUI_Queue *list )
{
	timer_thread_active = FALSE;
	/* 等待定时器处理线程的退出 */
	thread_join( tid, NULL ); 
	/* 销毁定时器列表 */
	timer_list_destroy( list ); 
}
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/

