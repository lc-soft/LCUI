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

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
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

/************************* App Management *****************************/
LCUI_App *Find_App(LCUI_ID id)
/* 功能：根据程序的ID，获取指向程序数据结构的指针 */
{
	LCUI_App *app; 
	int i, total;  
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0) { /* 如果程序总数大于0 */
		for (i = 0; i < total; ++i) {
			app = (LCUI_App*)Queue_Get(&LCUI_Sys.app_list, i);
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
	LCUI_ID id;
	Thread_TreeNode *ttn;
	
	id = pthread_self(); /* 获取本线程ID */  
	if(id == LCUI_Sys.core_thread
	|| id == LCUI_Sys.key_thread
	|| id == LCUI_Sys.mouse.thread
	|| id == LCUI_Sys.ts.thread )
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
	while(ttn->parent != NULL) { 
		ttn = ttn->parent; 
		if(ttn != NULL && ttn->parent == NULL) 
			break;
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
	int err = 0;
	
	LCUI_Sys.status = KILLED;	/* 状态标志置为KILLED */
	LCUI_Font_Free ();		/* 释放LCUI的默认字体数据占用的内存资源 */
	 
	Disable_Graph_Display(); /* 等待Core线程退出 */ 
	Destroy_Queue(&LCUI_Sys.key_event);/* 销毁按键事件数据队列 */
	Disable_Mouse_Input();		/* 禁用鼠标输入 */ 
	Disable_TouchScreen_Input();	/* 禁用触屏支持 */ 
	Disable_Key_Input();		/* 禁用按键输入 */ 
	/* 恢复屏幕初始内容 */ 
	Graph_Display (&LCUI_Sys.screen.buff, Pos(0, 0));	
	/* 解除帧缓冲在内存中的映射 */
	err = munmap (LCUI_Sys.screen.fb_mem, LCUI_Sys.screen.smem_len);
	if (err != 0) {
		perror ("munmap()");
	}
	close (LCUI_Sys.screen.fb_dev_fd);  
	exit (err);
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
			app = (LCUI_App*)Queue_Get(&LCUI_Sys.app_list, i);
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
	
	if (Queue_Empty(&LCUI_Sys.app_list)) /* 如果程序列表为空 */  
		LCUI_Quit (); /* 退出LCUI */ 
	return 0;
}

static void LCUI_Destroy_App(LCUI_App *app)
/* 功能：销毁程序相关信息 */
{
	if(app == NULL) {
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
	LCUI_ID id = pthread_self();
	/* 初始化程序数据结构体 */
	LCUI_App_Init (&app);
	app.id	= id;	/* 保存ID */ 
	Queue_Add(&LCUI_Sys.app_list, &app);/* 添加至队列 */
	return 0;
}

static int App_Quit()
/* 功能：退出程序 */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if(NULL == app) {
		printf("App_Quit(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	} 
	
	return LCUI_AppList_Delete(app->id); 
}

void Main_Loop_Quit()
/* 功能：让程序退出主循环 */
{ 
	LCUI_App *app = Get_Self_AppPointer();
	if(NULL == app) {
		printf("Main_Loop_Quit(): "APP_ERROR_UNRECORDED_APP);
		return;
	}
	app->stop_loop = IS_TRUE; 
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

static void Mouse_Init(void)
/* 功能：初始化鼠标数据 */
{
	LCUI_Sys.mouse.fd = 0;  
	LCUI_Sys.mouse.status = REMOVE; /* 鼠标为移除状态 */
	LCUI_Sys.mouse.move_speed = 1; /* 移动数度为1 */
	EventQueue_Init(&LCUI_Sys.mouse.event);/* 初始化鼠标事件信息队列 */
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
	int result;

	/* 检测是否支持鼠标 */
	nobuff_printf("checking mouse support...");
	result = Check_Mouse_Support();
	if(result == 0) {
		printf("yes\n");
		/* 启用鼠标输入处理 */
		nobuff_printf("enable mouse input..."); 
		result = Enable_Mouse_Input();
		if(result == 0) {
			printf("success\n");
		} else {
			printf("fail\n");
		}
	} else {
		printf("no\n");
	}
	
	LCUI_Sys.ts.status = REMOVE;
	LCUI_Sys.ts.thread = 0;
	LCUI_Sys.ts.td = NULL;
	/* 启用触屏输入处理 */ 
	Enable_TouchScreen_Input();  
	
	Enable_Key_Input();
}

int LCUI_Active()
/* 功能：检测LCUI是否活动 */
{
	if(LCUI_Sys.status == ACTIVE) {
		return 1;
	}
	return 0;
}

//extern int debug_mark;
int LCUI_Init(int argc, char *argv[])
/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */
{
	int temp;
	if( !LCUI_Sys.init ) {/* 如果LCUI没有初始化过 */ 
		srand(time(NULL));/* 生成随机数需要用到，只调用一次即可 */
		LCUI_Sys.init = IS_TRUE;
		Print_LCUI_Copyright_Text();
		
		Thread_TreeNode_Init (&LCUI_Sys.thread_tree);	/* 初始化根线程结点 */
		LCUI_Sys.thread_tree.tid = pthread_self();	/* 当前线程ID作为根结点 */
		LCUI_Sys.self_id = pthread_self();		/* 保存线程ID */
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
		
		/* 记录程序信息 */
		temp = LCUI_AppList_Add();
		if(temp != 0) {
			exit(-1);
		}
		
		Enable_Graph_Display();	/* LCUI的核心开始工作 */
		Mouse_Init();	/* 初始化鼠标 */
		Cursor_Init();	/* 初始化鼠标游标 */
		LCUI_IO_Init();	/* 初始化输入输出设备 */ 
		Widget_Event_Init(); /* 初始化部件事件处理 */
		//debug_mark = 1;
		/* 鼠标游标居中 */
		Set_Cursor_Pos( Get_Screen_Center_Point() );  
		Show_Cursor();	/* 显示鼠标游标 */ 
	} else {
		temp = LCUI_AppList_Add();
		if(temp != 0) {
			exit(-1);
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
	
#ifdef NEED_CATCH_SCREEN
	//创建线程，用于截图。
	//pthread_t t;
	//LCUI_Thread_Create(&t, NULL, catch, NULL);
#endif
	app = Get_Self_AppPointer();
	if(app == NULL) {
		printf("LCUI_Main(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	/* 循环条件是程序不需要关闭 */ 
	while (Need_Main_Loop(app)) { 
		if(Empty_Widget()) {/* 没有部件，就不需要循环 */
			break;
		}
		/* 如果有需要执行的任务 */
		if(Have_Task(app)) {
			idle_time = 1500;
			Run_Task(app); 
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
