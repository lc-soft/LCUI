
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_RES_H
#include LC_DISPLAY_H
#include LC_CURSOR_H 
#include LC_INPUT_H
#include LC_ERROR_H
#include LC_FONT_H 
#include LC_WIDGET_H

#include <unistd.h>
#include <signal.h>

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

/************************* App Management *****************************/
/* 根据程序的ID，获取指向程序数据结构的指针 */
LCUI_App *LCUIApp_Find( LCUI_ID id )
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

/* 获取指向程序数据的指针 */
LCUI_App* LCUIApp_GetSelf( void )
{
	thread_t id;
	Thread_TreeNode *ttn;
	
	id = thread_self(); /* 获取本线程ID */  
	if(id == LCUI_Sys.display_thread 
	|| id == LCUI_Sys.dev_thread
	|| id == LCUI_Sys.self_id
	|| id == LCUI_Sys.timer_thread )
	{/* 由于内核及其它线程ID没有被记录，只有直接返回LCUI主程序的线程ID了 */
		return LCUIApp_Find(LCUI_Sys.self_id);
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
	
	return LCUIApp_Find(ttn->tid);
}

/* 获取程序ID */
LCUI_ID LCUIApp_GetSelfID( void )
{
	return thread_self();
}

void LCUI_App_Init( LCUI_App *app )
/* 功能：初始化程序数据结构体 */
{
	app->id = 0;
	app->stop_loop = FALSE;
	AppTasks_Init( &app->tasks );
	//EventSlots_Init(&app->key_event);
	WidgetLib_Init(&app->widget_lib);
	app->encoding_type = ENCODEING_TYPE_UTF8;
}

static void LCUI_Quit( void )
/*
 * 功能：退出LCUI
 * 说明：在没有任何LCUI程序时，LCUI会调用本函数来恢复运行LCUI前的现场。
 * */
{
	LCUI_Sys.state = KILLED;	/* 状态标志置为KILLED */
	LCUI_Font_Free ();		/* 释放LCUI的默认字体数据占用的内存资源 */
	Disable_Graph_Display();	/* 禁用图形显示 */ 
	Disable_Mouse_Input();		/* 禁用鼠标输入 */ 
	Disable_TouchScreen_Input();	/* 禁用触屏支持 */ 
	Disable_Key_Input();		/* 禁用按键输入 */ 
	/* 停止事件循环 */
	LCUI_StopEventThread();
	LCUI_DestroyEvents();
	/* 销毁事件槽记录 */
	Destroy_Queue( &LCUI_Sys.sys_event_slots );
	Destroy_Queue( &LCUI_Sys.user_event_slots );
	timer_thread_destroy( LCUI_Sys.timer_thread, &LCUI_Sys.timer_list );
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
	LCUIApp_DestroyAllWidgets(app->id); /* 销毁这个程序的所有部件 */
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
	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("App_Quit(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	} 
	
	return LCUI_AppList_Delete(app->id); 
}

BOOL MainLoop_Active( LCUI_App *app )
{
	return !app->stop_loop;
}
/* 退出主循环 */
void LCUI_StopMainLoop( void )
{ 
	LCUI_App *app;
	app = LCUIApp_GetSelf();
	app->stop_loop = TRUE;
}
/*********************** App Management End ***************************/


static void LCUI_ShowCopyrightText()
/* 功能：打印LCUI的信息 */
{
	printf(
	"============| LCUI v0.13.0 |============\n"
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
	if(LCUI_Sys.state == ACTIVE) {
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
		/* 标记已经初始化 */
		LCUI_Sys.init = TRUE;
		LCUI_Sys.state = ACTIVE;
		/* 初始化随机数种子 */
		srand(time(NULL));
		/* 打印版权信息 */
		LCUI_ShowCopyrightText();
		/* 初始化根线程结点 */
		Thread_TreeNode_Init (&LCUI_Sys.thread_tree);	
		/* 当前线程ID作为根结点 */
		LCUI_Sys.thread_tree.tid = thread_self();
		/* 保存线程ID */
		LCUI_Sys.self_id = thread_self();
		
		/* 初始化事件槽记录 */
		EventSlots_Init( &LCUI_Sys.sys_event_slots );
		EventSlots_Init( &LCUI_Sys.user_event_slots );
		
		LCUI_EventsInit();
		LCUI_StartEventThread();
		
		/* 初始化默认的字体数据 */
		LCUI_Font_Init(&LCUI_Sys.default_font);
		/* 初始化LCUI程序数据 */
		LCUI_AppList_Init( &LCUI_Sys.app_list );
		/* 初始化屏幕区域更新队列 */ 
		RectQueue_Init( &LCUI_Sys.invalid_area );
		/* 初始化部件队列 */
		WidgetQueue_Init( &LCUI_Sys.widget_list ); 
		/* 让定时器处理模块开始工作 */
		timer_thread_start(	&LCUI_Sys.timer_thread, 
					&LCUI_Sys.timer_list );
		/* 初始化用于储存已按下的键的键值队列 */
		Queue_Init( &LCUI_Sys.press_key, sizeof(int), NULL );
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

static BOOL
Have_Task( LCUI_App *app )
/* 功能：检测是否有任务 */
{
	if( !app ) {
		return FALSE; 
	}
	if(Queue_Get_Total(&app->tasks) > 0) {
		return TRUE; 
	}
	return FALSE;
}

static int 
Run_Task( LCUI_App *app )
/* 功能：执行任务 */
{ 
	static LCUI_Task *task;
	task = (LCUI_Task*)Queue_Get( &app->tasks, 0 );
	//clock_t start = clock();
	//printf("run task %p\n", task->func);
	/* 调用函数指针指向的函数，并传递参数 */
	task->func( task->arg[0], task->arg[1] );
	/* 若需要在调用回调函数后销毁参数 */
	if( task->destroy_arg[0] ) {
		free( task->arg[0] );
	}
	if( task->destroy_arg[1] ) {
		free( task->arg[1] );
	}
	//printf("task %p use time: %ldus\n", task->func, clock()-start);
	return Queue_Delete(&app->tasks, 0);
}

int LCUI_Main ()
/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
{
	LCUI_App *app;
	int idle_time = 1500;
	
	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("LCUI_Main(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	/* 循环条件是程序不需要关闭 */ 
	while( MainLoop_Active( app ) ) {
		if(Empty_Widget()) {
			break;
		}
		/* 如果有需要执行的任务 */
		if( Have_Task(app) ) {
			idle_time = 1500;
			Run_Task( app ); 
		} else { /* 否则暂停一段时间 */
			usleep (idle_time);
			if (idle_time < MAX_APP_IDLE_TIME) {
				idle_time += 1500;
			}
		}
	}
	/* 直接关闭程序，并释放资源 */ 
	return App_Quit ();
}

int Get_LCUI_Version(char *out)
/* 功能：获取LCUI的版本 */
{
	return sprintf(out, "%s", LCUI_VERSION);
}
