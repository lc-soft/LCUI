/* ***************************************************************************
 * LCUI_Main.c -- LCUI's kernel
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Main.c -- LCUI 的核心
 *
 * 版权所有 (C) 2012 归属于 
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
#include LC_GRAPH_H
#include LC_RES_H
#include LC_DISPLAY_H
#include LC_CURSOR_H 
#include LC_INPUT_H
#include LC_MISC_H
#include LC_ERROR_H
#include LC_FONT_H 
#include LC_WIDGET_H

#include <unistd.h>

LCUI_System LCUI_Sys; 

static clock_t start_time;
void count_time()
{
	start_time = clock();
}

void end_count_time()
{
	printf("%ldms\n",clock()-start_time);
}

/*----------------------------- Device -------------------------------*/
typedef struct _dev_func_data
{
	BOOL (*init_func)();
	BOOL (*proc_func)();
	BOOL (*destroy_func)();
}
dev_func_data;

static void dev_list_init( LCUI_Queue *dev_list )
{
	Queue_Init( dev_list, sizeof(dev_func_data), NULL );
}

/* 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
int LCUI_Dev_Add(	BOOL (*init_func)(), 
			BOOL (*proc_func)(), 
			BOOL (*destroy_func)() )
{
	dev_func_data data;
	
	if( init_func ) {
		init_func();
	}
	data.init_func = init_func;
	data.proc_func = proc_func;
	data.destroy_func = destroy_func;
	if( 0<= Queue_Add( &LCUI_Sys.dev_list, &data ) ) {
		return 0;
	}
	return -1;
}

static void *proc_dev_list ( void *arg )
{
	LCUI_Queue *dev_list;
	dev_func_data *data_ptr;
	int total, i, result, sleep_time = 1000;
	
	dev_list = (LCUI_Queue *)arg;
	while( LCUI_Active() ) {
		result = 0;
		total = Queue_Get_Total( dev_list );
		for(i=0; i<total; ++i) {
			data_ptr = Queue_Get( dev_list, i );
			if( !data_ptr || !data_ptr->proc_func ) {
				continue;
			}
			result += data_ptr->proc_func();
		}
		if( result > 0 ) {
			sleep_time = 1000;
		} else {
			usleep( sleep_time );
			if( sleep_time < 100000 ) {
				sleep_time += 1000;
			}
		}
	}
	thread_exit(NULL);
}

/* 初始化设备 */
int LCUI_Dev_Init()
{
	dev_list_init( &LCUI_Sys.dev_list );
	return thread_create( &LCUI_Sys.dev_thread, NULL, 
			proc_dev_list, &LCUI_Sys.dev_list );
}

void LCUI_Dev_Destroy()
{
	int total, i;
	dev_func_data *data_ptr;
	LCUI_Queue *dev_list;
	
	dev_list = &LCUI_Sys.dev_list; 
	total = Queue_Get_Total( dev_list );
	for(i=0; i<total; ++i) {
		data_ptr = Queue_Get( dev_list, i );
		if( !data_ptr || !data_ptr->destroy_func ) {
			continue;
		}
		data_ptr->destroy_func();
	}
}
/*--------------------------- End Device -----------------------------*/

/*----------------------------- Timer --------------------------------*/
typedef struct _timer_data
{
	int status;		/* 状态 */
	BOOL reuse;		/* 是否重复使用该定时器 */
	LCUI_ID app_id;	/* 所属程序ID */
	long int id;		/* 定时器ID */
	long int total_ms;	/* 定时总时间（单位：毫秒） */
	long int cur_ms;	/* 当前剩下的等待时间 */
	void (*callback_func)(); /* 回调函数 */ 
}
timer_data;

/*----------------------------- Private ------------------------------*/
/* 功能：初始化定时器列表 */
static void timer_list_init( LCUI_Queue *timer_list )
{
	Queue_Init( timer_list, sizeof(timer_data), NULL );
	/* 使用链表 */
	Queue_Set_DataMode( timer_list, QUEUE_DATA_MODE_LINKED_LIST );
}
/* 功能：销毁定时器列表 */
static void timer_list_destroy( LCUI_Queue *timer_list )
{
	Destroy_Queue( timer_list );
}

/* 功能：对定时器列表进行排序 */
static void timer_list_sort( LCUI_Queue *timer_list )
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
			}
		}
	}
	Queue_UnLock( timer_list );
}

/* 功能：将各个定时器的等待时间与指定时间相减 */
static void timer_list_sub( LCUI_Queue *timer_list, int time )
{
	timer_data *timer;
	int i, total;
	
	Queue_Lock( timer_list );
	total = Queue_Get_Total( timer_list );
	
	for(i=0; i<total; ++i) {
		timer = Queue_Get( timer_list , i);
		/* 忽略无效的定时器，或者状态为暂停的定时器 */
		if( !timer || timer->status == 0) {
			continue;
		}
		timer->cur_ms -= time;
	}
	Queue_UnLock( timer_list );
}

static timer_data *timer_list_update( LCUI_Queue *timer_list )
/* 功能：更新定时器列表中的定时器 */
{
	int i, total;
	timer_data *timer = NULL;
	
	total = Queue_Get_Total( timer_list ); 
	for(i=0; i<total; ++i){
		timer = Queue_Get( timer_list , i);
		if(timer->status == 1) {
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

/* 一个线程，用于处理定时器 */
static void *timer_list_process( void *arg )
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
	while( LCUI_Active() ) { 
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
		AppTask_Custom_Add( ADD_MODE_REPLACE, &func_data );
	}
	LCUI_Thread_Exit(NULL);
}

timer_data *find_timer( int timer_id )
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
	timer.status = 1;
	timer.total_ms = timer.cur_ms = n_ms;
	timer.callback_func = callback_func;
	timer.reuse = reuse;
	timer.id = rand();
	timer.app_id = Get_Self_AppPointer()->id;
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
		timer->status = 0;
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
		timer->status = 1;
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
/*---------------------------- End Public -----------------------------*/

/*---------------------------- End Timer ------------------------------*/


/************************* App Management *****************************/
LCUI_App *Find_App(LCUI_ID id)
/* 功能：根据程序的ID，获取指向程序数据结构的指针 */
{
	LCUI_App *app; 
	int i, total;
	
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0) { /* 如果程序总数大于0 */
		for (i = 0; i < total; ++i) {
			app = Queue_Get(&LCUI_Sys.app_list, i);
			if(app->id == id) {
				return app;
			}
		}
	}
	
	return NULL;
}

LCUI_App* Get_Self_AppPointer()
/* 功能：获取程序的指针 */
{
	thread_t id;
	Thread_TreeNode *ttn;
	
	id = thread_self(); /* 获取本线程ID */  
	if(id == LCUI_Sys.display_thread 
	|| id == LCUI_Sys.dev_thread
	|| id == LCUI_Sys.self_id
	|| id == LCUI_Sys.timer_thread )
	{/* 由于内核及其它线程ID没有被记录，只有直接返回LCUI主程序的线程ID了 */
		return Find_App(LCUI_Sys.self_id);
	}
	/* 获取父线程的ID */
	ttn = Search_Thread_Tree(&LCUI_Sys.thread_tree, id);
	/* 
	 * 往父级遍历，直至父级指针为NULL，因为根线程是没
	 * 有父线程结点指针的，程序的线程ID都在根线程里的
	 * 子线程ID队列中 
	 * */
	while( ttn->parent ) { 
		ttn = ttn->parent; 
		if( ttn && !ttn->parent ) { 
			break;
		}
	}
	
	return Find_App(ttn->tid);
}


void LCUI_App_Init(LCUI_App *app)
/* 功能：初始化程序数据结构体 */
{
	app->id = 0;
	app->stop_loop = IS_FALSE;
	FuncQueue_Init(&app->task_queue); 
	//EventQueue_Init(&app->key_event);
	WidgetLib_Init(&app->widget_lib);
	app->encoding_type = ENCODEING_TYPE_UTF8;
}

static void LCUI_Quit ()
/*
 * 功能：退出LCUI
 * 说明：在没有任何LCUI程序时，LCUI会调用本函数来恢复运行LCUI前的现场。
 * */
{
	LCUI_Sys.status = KILLED;	/* 状态标志置为KILLED */
	LCUI_Font_Free ();		/* 释放LCUI的默认字体数据占用的内存资源 */
	Disable_Graph_Display();	/* 禁用图形显示 */ 
	Destroy_Queue(&LCUI_Sys.key_event);/* 销毁按键事件数据队列 */
	Disable_Mouse_Input();		/* 禁用鼠标输入 */ 
	Disable_TouchScreen_Input();	/* 禁用触屏支持 */ 
	Disable_Key_Input();		/* 禁用按键输入 */ 
	thread_join( LCUI_Sys.timer_thread, NULL ); /* 等待定时器处理线程的退出 */
	timer_list_destroy( &LCUI_Sys.timer_list ); /* 销毁定时器列表 */
}


static int LCUI_AppList_Delete (LCUI_ID app_id)
/* 功能：关闭一个LCUI程序 */
{
	int pos = -1;
	LCUI_App *app; 
	int i, total;  
	
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0) { /* 如果程序总数大于0， 查找程序信息所在队列的位置 */
		for (i = 0; i < total; ++i) {
			app = Queue_Get(&LCUI_Sys.app_list, i);
			if(app->id == app_id) {
				pos = i;
				break;
			}
		}
		if(pos < 0) {
			return -1;
		}
	} else {
		return -1;
	}
	/* 从程序显示顺序队列中删除这个程序ID */ 
	Queue_Delete (&LCUI_Sys.app_list, pos); 
	
	if (Queue_Empty(&LCUI_Sys.app_list)) {/* 如果程序列表为空 */  
		LCUI_Quit (); /* 退出LCUI */ 
	}
	return 0;
}

static void LCUI_Destroy_App(LCUI_App *app)
/* 功能：销毁程序相关信息 */
{
	if( !app ) {
		return;
	}
	
	LCUI_App_Thread_Cancel(app->id); /* 撤销这个程序的所有线程 */
	LCUI_Destroy_App_Widgets(app->id); /* 销毁这个程序的所有部件 */
}

static void LCUI_AppList_Init()
/* 功能：初始化程序数据表 */
{
	Queue_Init(&LCUI_Sys.app_list, sizeof(LCUI_App), LCUI_Destroy_App);
}

static int LCUI_AppList_Add ()
/* 
 * 功能：创建一个LCUI程序
 * 说明：此函数会将程序信息添加至程序列表
 * 返回值：成功则返回程序的ID，失败则返回-1
 **/
{
	LCUI_App app;
	
	/* 初始化程序数据结构体 */
	LCUI_App_Init (&app);
	app.id	= thread_self(); /* 保存ID */ 
	Queue_Add(&LCUI_Sys.app_list, &app);/* 添加至队列 */
	return 0;
}

static int App_Quit()
/* 功能：退出程序 */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if( !app ) {
		printf("App_Quit(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	} 
	
	return LCUI_AppList_Delete(app->id); 
}

void Main_Loop_Quit()
/* 功能：让程序退出主循环 */
{ 
	LCUI_App *app = Get_Self_AppPointer();
	if( !app ) {
		printf("Main_Loop_Quit(): "APP_ERROR_UNRECORDED_APP);
		return;
	}
	app->stop_loop = TRUE; 
}
/*********************** App Management End ***************************/


static void Print_LCUI_Copyright_Text()
/* 功能：打印LCUI的信息 */
{
	printf(
	"============| LCUI v0.12.6 |============\n"
	"Copyright (C) 2012 Liu Chao.\n"
	"Licensed under GPLv2.\n"
	"Report bugs to <lc-soft@live.cn>.\n"
	"Project Homepage: www.lcui.org.\n"
	"========================================\n" );
}

static void Cursor_Init()
/* 功能：初始化游标数据 */
{
	LCUI_Graph pic;
	Graph_Init(&pic);
	Load_Graph_Default_Cursor(&pic);/* 载入自带的游标的图形数据 */ 
	Set_Cursors_Graph(&pic); 
}

static void LCUI_IO_Init()
/* 功能：初始化输入输出功能 */
{
	Mouse_Init();
	Cursor_Init();
	Keyboard_Init();
	TouchScreen_Init();
}

BOOL LCUI_Active()
/* 功能：检测LCUI是否活动 */
{
	if(LCUI_Sys.status == ACTIVE) {
		return TRUE;
	}
	return FALSE;
}

//extern int debug_mark;
int LCUI_Init(int argc, char *argv[])
/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */
{
	int temp;
	/* 如果LCUI没有初始化过 */
	if( !LCUI_Sys.init ) {
		LCUI_Sys.init = TRUE;
		LCUI_Sys.status = ACTIVE;
		srand(time(NULL));
		Print_LCUI_Copyright_Text();
		timer_list_init( &LCUI_Sys.timer_list );	/* 初始化定时器列表 */
		Thread_TreeNode_Init (&LCUI_Sys.thread_tree);	/* 初始化根线程结点 */
		LCUI_Sys.thread_tree.tid = thread_self();	/* 当前线程ID作为根结点 */
		LCUI_Sys.self_id = thread_self();		/* 保存线程ID */
		
		LCUI_Sys.focus_widget = NULL; 
		
		/* 设定最大空闲时间 */
		LCUI_Sys.max_app_idle_time = MAX_APP_IDLE_TIME;
		LCUI_Sys.max_lcui_idle_time = MAX_LCUI_IDLE_TIME;
		EventQueue_Init(&LCUI_Sys.key_event);	/* 初始化按键事件队列 */
		LCUI_Font_Init (&LCUI_Sys.default_font);/* 初始化默认的字体数据 */
		LCUI_AppList_Init (&LCUI_Sys.app_list); /* 初始化LCUI程序数据 */
		RectQueue_Init (&LCUI_Sys.update_area);	/* 初始化屏幕区域更新队列 */ 
		WidgetQueue_Init (&LCUI_Sys.widget_list); /* 初始化部件队列 */
		/* 初始化用于储存已按下的键的键值队列 */
		Queue_Init(&LCUI_Sys.press_key, sizeof(int), NULL);
		/* 创建用于处理定时器列表的线程 */
		thread_create( &LCUI_Sys.timer_thread, NULL, 
			timer_list_process, &LCUI_Sys.timer_list );
		/* 记录程序信息 */
		temp = LCUI_AppList_Add();
		if(temp != 0) {
			printf(APP_ERROR_REGISTER_ERROR);
			abort();
		}
		
		LCUI_Dev_Init();
		LCUI_IO_Init();		/* 初始化输入输出设备 */ 
		Widget_Event_Init();	/* 初始化部件事件处理 */
		Enable_Graph_Display();	/* 启用图形输出 */ 
		/* 鼠标游标居中 */
		Set_Cursor_Pos( Get_Screen_Center_Point() );  
		Show_Cursor();	/* 显示鼠标游标 */ 
	} else {
		temp = LCUI_AppList_Add();
		if(temp != 0) {
			printf(APP_ERROR_REGISTER_ERROR);
			abort();
		}
	}
	/* 注册默认部件类型 */
	Register_Default_Widget_Type();
	return 0;
}

int Need_Main_Loop(LCUI_App *app)
/* 功能：检测主循环是否需要继续进行 */
{
	if( app->stop_loop ) {
		return 0;
	}
	return 1;
}

int LCUI_Main ()
/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
{
	LCUI_App *app;
	LCUI_ID idle_time = 1500;
	LCUI_Graph graph;
	
	Graph_Init(&graph);
//#define NEED_CATCH_SCREEN
#ifdef NEED_CATCH_SCREEN
	LCUI_Rect area;
	area.x = (Get_Screen_Width()-320)/2;
	area.y = (Get_Screen_Height()-240)/2;
	area.width = 320;
	area.height = 240;
	start_record_screen( area );
#endif
	app = Get_Self_AppPointer();
	if( !app ) {
		printf("LCUI_Main(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	/* 循环条件是程序不需要关闭 */ 
	while (Need_Main_Loop(app)) { 
		if(Empty_Widget()) {/* 没有部件，就不需要循环 */
			break;
		}
		/* 如果有需要执行的任务 */
		if( Have_Task(app) ) {
			idle_time = 1500;
			Run_Task( app ); 
		} else {/* 否则暂停一段时间 */
			usleep (idle_time);
			idle_time += 1500;	/* 每次循环的空闲时间越来越长 */
			if (idle_time >= LCUI_Sys.max_app_idle_time) {
				idle_time = LCUI_Sys.max_app_idle_time;
			}
		}
	}
	return App_Quit ();	/* 直接关闭程序，并释放资源 */ 
}

int Get_LCUI_Version(char *out)
/* 功能：获取LCUI的版本 */
{
	return sprintf(out, "%s", LCUI_VERSION);
}
