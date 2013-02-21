
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

#include <time.h>

LCUI_System LCUI_Sys; 

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
	LCUI_Thread id;
	
	id = LCUIThread_SelfID(); /* 获取本线程ID */  
	if(id == LCUI_Sys.display_thread 
	|| id == LCUI_Sys.dev_thread
	|| id == LCUI_Sys.self_id
	|| id == LCUI_Sys.timer_thread )
	{/* 由于内核及其它线程ID没有被记录，只有直接返回LCUI主程序的线程ID了 */
		return LCUIApp_Find((LCUI_ID)LCUI_Sys.self_id);
	}
	id = LCUIThread_GetRootThreadID( id );
	return LCUIApp_Find( (LCUI_ID)id );
}

/* 获取程序ID */
LCUI_ID LCUIApp_GetSelfID( void )
{
	return (LCUI_ID)LCUIThread_SelfID();
}

void LCUI_App_Init( LCUI_App *app )
/* 功能：初始化程序数据结构体 */
{
	app->id = 0;
	AppTasks_Init( &app->tasks );
	//EventSlots_Init(&app->key_event);
	WidgetLib_Init(&app->widget_lib);
	app->encoding_type = ENCODEING_TYPE_UTF8;
}

static void LCUIModule_Cursor_End( void )
{
	Graph_Free( &LCUI_Sys.cursor.graph );
}

static void LCUI_Quit( void )
/*
 * 功能：退出LCUI
 * 说明：在没有任何LCUI程序时，LCUI会调用本函数来恢复运行LCUI前的现场。
 * */
{
	LCUI_Sys.state = KILLED;	/* 状态标志置为KILLED */
	LCUIModule_Thread_End();
	LCUIModule_Event_End();
	LCUIModule_Cursor_End();
	LCUIModule_Widget_End();
	LCUIModule_Font_End();
	LCUIModule_Timer_End();
	LCUIModule_Keyboard_End();
	//LCUIModule_Mouse_End();
	//LCUIModule_TouchScreen_End();
	LCUIModule_Device_End();
	LCUIModule_Video_End();
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

static void LCUI_Destroy_App( void* arg )
/* 功能：销毁程序相关信息 */
{
	LCUI_App *app;
	app = (LCUI_App *)arg;
	if( !app ) {
		return;
	}
	LCUIApp_CancelAllThreads( app->id ); /* 撤销这个程序的所有线程 */
	LCUIApp_DestroyAllWidgets( app->id ); /* 销毁这个程序的所有部件 */
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
	app.id	= LCUIThread_SelfID(); /* 保存ID */ 
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

/*********************** App Management End ***************************/



/*--------------------------- Main Loop -------------------------------*/
static LCUI_BOOL init_mainloop_queue = FALSE;
static LCUI_Queue mainloop_queue;

static void LCUI_MainLoopQueue_Init( void )
{
	Queue_Init( &mainloop_queue, sizeof(LCUI_MainLoop), NULL );
}

/* 查找处于运行状态的主循环 */
static LCUI_MainLoop *
LCUI_MainLoopQueue_Find( void )
{
	int i, total;
	LCUI_MainLoop *loop;
	
	total = Queue_Get_Total( &mainloop_queue );
	for(i=0; i<total; ++i) {
		loop = Queue_Get( &mainloop_queue, i );
		if( loop->running ) {
			return loop;
		}
	}
	return NULL;
}

static void
LCUI_MainLoopQueue_Sort( void )
{
	int i, j, total;
	LCUI_MainLoop *cur_loop, *next_loop;
	
	total = Queue_Get_Total( &mainloop_queue );
	for(i=0; i<total; ++i) {
		cur_loop = Queue_Get( &mainloop_queue, i );
		if( !cur_loop ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			next_loop = Queue_Get( &mainloop_queue, j );
			if( !next_loop ) {
				continue; 
			}
			if( next_loop->level > cur_loop->level ) {
				Queue_Swap( &mainloop_queue, j, i);
				cur_loop = next_loop;
			}
		}
	}
}

/* 新建一个主循环 */
LCUI_MainLoop *LCUI_MainLoop_New( void )
{
	LCUI_MainLoop *loop;
	
	if( !init_mainloop_queue ) {
		LCUI_MainLoopQueue_Init();
		init_mainloop_queue = TRUE;
	}
	loop = malloc(sizeof(LCUI_MainLoop));
	if( !loop ) {
		return NULL;
	}
	loop->quit = FALSE;
	loop->level = Queue_Get_Total( &mainloop_queue );
	loop->running = FALSE;
	Queue_Add_Pointer( &mainloop_queue, loop );
	/* 重新对主循环队列进行排序 */
	LCUI_MainLoopQueue_Sort();
	return loop;
}

/* 设定主循环等级，level值越高，处理主循环退出时，也越早处理该循环 */
int LCUI_MainLoop_Level( LCUI_MainLoop *loop, int level )
{
	if( loop == NULL ) {
		return -1;
	}
	loop->level = level;
	LCUI_MainLoopQueue_Sort();
	return 0;
}


static LCUI_BOOL
LCUIApp_HaveTask( LCUI_App *app )
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
LCUIApp_RunTask( LCUI_App *app )
{ 
	LCUI_Task *task;
	task = Queue_Get( &app->tasks, 0 );
	if( task == NULL ) {
		return -1;
	}
	Queue_Delete_Pointer( &app->tasks, 0 );
	if( task->func == NULL ) {
		return -2;
	}
	/* 调用函数指针指向的函数，并传递参数 */
	task->func( task->arg[0], task->arg[1] );
	/* 若需要在调用回调函数后销毁参数 */
	if( task->destroy_arg[0] ) {
		free( task->arg[0] );
	}
	if( task->destroy_arg[1] ) {
		free( task->arg[1] );
	}
	free( task );
	return 0;
}

/* 运行目标循环 */
int LCUI_MainLoop_Run( LCUI_MainLoop *loop )
{
	LCUI_App *app;
	int idle_time = 1;
	
	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("%s(): %s", __FUNCTION__, APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	DEBUG_MSG("loop: %p, enter\n", loop);
	loop->running = TRUE;
	while( !loop->quit ) {
		if( LCUIApp_HaveTask(app) ) {
			idle_time = 1;
			LCUIApp_RunTask( app ); 
		} else {
			LCUI_MSleep (idle_time);
			if (idle_time < MAX_APP_IDLE_TIME) {
				idle_time += 1;
			}
		}
	}
	loop->running = FALSE;
	DEBUG_MSG("loop: %p, exit\n", loop);
	return 0;
}

/* 标记目标主循环需要退出 */
int LCUI_MainLoop_Quit( LCUI_MainLoop *loop )
{
	if( loop == NULL ) {
		loop = LCUI_MainLoopQueue_Find();
		if( loop == NULL ) {
			return -1;
		}
	}
	loop->quit = TRUE;
	return 0;
}
/*----------------------- End MainLoop -------------------------------*/


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

/* 初始化游标数据 */
static void LCUIModule_Cursor_Init( void )
{
	LCUI_Graph pic;
	Graph_Init( &pic );
	Load_Graph_Default_Cursor( &pic );/* 载入自带的游标的图形数据 */ 
	Set_Cursors_Graph( &pic );
}

LCUI_BOOL LCUI_Active()
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
		LCUI_Sys.init = TRUE;
		LCUI_Sys.state = ACTIVE;
		srand(time(NULL));
		LCUI_ShowCopyrightText();
		
		LCUI_AppList_Init( &LCUI_Sys.app_list );
		/* 注册程序 */
		temp = LCUI_AppList_Add();
		if(temp != 0) {
			printf(APP_ERROR_REGISTER_ERROR);
			abort();
		}
		/* 初始化各个模块 */
		LCUIModule_Thread_Init();
		LCUIModule_Event_Init();
		LCUIModule_Font_Init();
		LCUIModule_Timer_Init();
		LCUIModule_Device_Init();
		LCUIModule_Keyboard_Init();
		LCUIModule_Mouse_Init();
		LCUIModule_TouchScreen_Init();
		LCUIModule_Video_Init();
		LCUIModule_Cursor_Init();
		LCUIModule_Widget_Init();
		/* 让鼠标游标居中显示 */
		Set_Cursor_Pos( LCUIScreen_GetCenter() );  
		Show_Cursor();
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


/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
int LCUI_Main( void )
{
	LCUI_MainLoop *loop;
	loop = LCUI_MainLoop_New();
	LCUI_MainLoop_Run( loop );
	return App_Quit ();
}

/* 获取LCUI的版本 */
int LCUI_GetSelfVersion( char *out )
{
	return sprintf(out, "%s", LCUI_VERSION);
}
