/* ***************************************************************************
 * LCUI_Work.h -- LCUI's other work
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
 * LCUI_Work.h -- LCUI 的其它工作
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

#ifndef __LCUI_WORK_H__
#define __LCUI_WORK_H__

#define RELATIVE_POS	1 /* 相对坐标 */
#define NORMAL_POS	0 /*普通坐标 */

typedef LCUI_Queue LCUI_EventQueue;
typedef LCUI_Queue LCUI_FuncQueue;
typedef LCUI_Queue LCUI_TaskQueue;

typedef enum _WidgetEvent_ID
{
	EVENT_DRAG,	/* 部件的拖动事件 */
	EVENT_CLICKED,	/* 部件的点击事件 */
	EVENT_KEYBOARD,	/* 按键事件 */
	EVENT_FOCUS_IN,	/* 得到焦点 */
	EVENT_FOCUS_OUT	/* 失去焦点 */
}
WidgetEvent_ID; 

typedef struct _LCUI_DragEvent
{
	LCUI_Pos new_pos;	/* 部件的新全局坐标 */
	LCUI_Pos cursor_pos;	/* 鼠标游标的坐标 */
	int first_click;	/* 标志，是否为首次点击 */ 
	int end_click;		/* 标志，是否已经结束拖动 */
}
LCUI_DragEvent; 

typedef struct _LCUI_Func
{
	/* 
	 * 函数ID，部件库需要这ID标识函数类型，往程序的任务队列添加
	 * 任务也需要该ID来标识目标程序ID 
	 * */
	LCUI_ID id;
	void (*func)();  /* 函数指针 */
	
	/* 以下参数该怎么传给回调函数，具体要看是如何处理事件的 */  
	void *arg[2];		/* 传给函数的两个参数 */
	BOOL destroy_arg[2];	/* 指定是否在调用完回调函数后，销毁参数 */
}
LCUI_Func, LCUI_Task;


typedef struct _LCUI_Event
{
	int id;			/* 记录事件ID */
	LCUI_Queue func_data;	/* 记录被关联的回调函数数据 */
}
LCUI_Event;


LCUI_BEGIN_HEADER

/***************************** Func ***********************************/
void NULL_Func();
/* 功能：空函数，不做任何操作 */ 

void FuncQueue_Init(LCUI_Queue *queue);
/* 功能：初始化函数指针队列 */ 

/****************************** Task **********************************/

void Send_Task_To_App(LCUI_Func *func_data);
/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Func结构体中的成员变量 id，保存的是目标程序的id
 */ 

BOOL
Have_Task( LCUI_App *app );
/* 功能：检测是否有任务 */ 

int Run_Task(LCUI_App *app);
/* 功能：执行任务 */ 

int AppTask_Custom_Add(int mode, LCUI_Func *func_data);
/*
 * 功能：使用自定义方式添加程序任务
 * 用法示例：
 * 在函数的各参数与队列中的函数及各参数不重复时，添加它
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT | AND_ARG_F | AND_ARG_S, func_data);
 * 只要函数和参数1不重复则添加
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT | AND_ARG_F, func_data);
 * 要函数不重复则添加
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT, func_data);
 * 添加新的，不管是否有重复的
 * AppTask_Custom_Add(ADD_MODE_ADD_NEW, func_data);
 * 有相同函数则覆盖，没有则新增
 * AppTask_Custom_Add(ADD_MODE_REPLACE, func_data);
 * */ 
/**************************** Task End ********************************/


/***************************** Event ***********************************/ 
void EventQueue_Init(LCUI_EventQueue * queue);
/* 功能：初始化事件队列 */ 

BOOL Get_FuncData(LCUI_Func *p, void (*func) (), void *arg1, void *arg2);
/* 
 * 功能：将函数指针以及两个参数，转换成LCUI_Func类型的指针
 * 说明：此函数会申请内存空间，并返回指向该空间的指针
 * */ 

LCUI_Event *Find_Event(LCUI_EventQueue *queue, int event_id);
/* 功能：根据事件的ID，获取指向该事件的指针 */ 

int EventQueue_Add(LCUI_EventQueue *queue, int event_id, LCUI_Func *func);
/* 功能：记录事件及对应回调函数至队列 */ 

int LCUI_MouseEvent_Connect (void (*func) (), int event_id);
/* 
 * 功能：将函数与鼠标的相关事件相关联
 * 说明：当鼠标事件触发后，会先将已关联该事件的函数指针及相关事件的指针
 * 添加至程序的任务队列，等待程序在主循环中处理
 **/ 

int LCUI_Key_Event_Connect (int key_value, void (*func) (), void *arg);
/* 功能：将函数与按键的某个事件相连接，当这个按键按下后，就会调用这个函数 */ 

int Handle_Event(LCUI_EventQueue *queue, int event_id);
/* 
 * 功能：处理指定ID的事件
 * 说明：本函数会将事件队列中与指定ID的事件关联的回调函数 添加至程序的任务队列
 * */ 

/********************* 处理部件拖动/点击事件 ******************************/
int Widget_Drag_Event_Connect ( 
				LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, LCUI_DragEvent *)
);
/* 
 * 功能：将回调函数与部件的拖动事件进行连接 
 * 说明：建立连接后，但部件被点击，拖动，释放，都会调用回调函数
 * */ 

int Widget_Clicked_Event_Connect (
			LCUI_Widget *widget,
			void (*func)(LCUI_Widget*, void *), 
			void *arg
);
/* 
 * 功能：将回调函数与部件的拖动事件进行连接 
 * 说明：建立连接后，部件会将新位置作为第二参数，传给回调函数
 * */ 

void Widget_Event_Init();
/* 功能：初始化部件事件处理 */ 


/*--------------------------- Focus Proc ------------------------------*/
BOOL 
Set_Focus( LCUI_Widget *widget );
/* 
 * 功能：为部件设置焦点
 * 说明：上个获得焦点的部件会得到EVENT_FOCUS_OUT事件，而当前获得焦点的部件会得到
 * EVENT_FOCUS_IN事件。
 * */ 

BOOL 
Cancel_Focus( LCUI_Widget *widget );
/* 
 * 功能：取消指定部件的焦点
 * 说明：该部件会得到EVENT_FOCUS_OUT事件，并且，会将焦点转移至其它部件
 * */ 

BOOL
Reset_Focus( LCUI_Widget* widget );
/* 复位指定部件内的子部件的焦点 */ 

BOOL 
Widget_FocusIn_Event_Connect(	LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, void*), 
				void *arg );
/* 将回调函数与FOCUS_IN事件连接，当部件得到焦点时，会调用该回调函数 */ 

BOOL 
Widget_FocusOut_Event_Connect(	LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, void*), 
				void *arg );
/* 将回调函数与FOCUS_OUT事件连接，当部件失去焦点时，会调用该回调函数 */ 
/*------------------------- End Focus Proc ----------------------------*/

/*************************** Event End *********************************/

LCUI_END_HEADER

#endif

