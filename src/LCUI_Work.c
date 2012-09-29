/* ***************************************************************************
 * LCUI_Work.c -- LCUI's other work
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
 * LCUI_Work.c -- LCUI 的其它工作
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
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WORK_H 
#include LC_MISC_H
#include LC_ERROR_H
#include LC_INPUT_H
#include LC_WIDGET_H
#include <unistd.h>

/***************************** Func ***********************************/
void NULL_Func()
/* 功能：空函数，不做任何操作 */
{
	return;
}

void FuncQueue_Init(LCUI_Queue *queue)
/* 功能：初始化函数指针队列 */
{
	Queue_Init(queue, sizeof(LCUI_Func), NULL);
}

/****************************** Task **********************************/

void Send_Task_To_App(LCUI_Func *func_data)
/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Func结构体中的成员变量 id，保存的是目标程序的id
 */
{ 
	LCUI_App *app;
	app = Find_App(func_data->id);
	if(app == NULL) {
		return;
	}
	Queue_Add(&app->task_queue, func_data);
}


int Have_Task(LCUI_App *app)
/* 功能：检测是否有任务 */
{
	if(app == NULL) {
		return 0; 
	}
	if(Queue_Get_Total(&app->task_queue) > 0) {
		return 1; 
	}
	return 0;
}

int Run_Task(LCUI_App *app)
/* 功能：执行任务 */
{ 
	LCUI_Task *task;
	task = (LCUI_Task*)Queue_Get(&app->task_queue, 0);
	/* 调用函数指针指向的函数，并传递参数 */
	task->func(task->arg[0], task->arg[1]);
	
	return Queue_Delete(&app->task_queue, 0);
}


int AppTask_Custom_Add(int mode, LCUI_Func *func_data)
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
{
	int total, i;
	/* 先获取程序数据结构体指针 */
	LCUI_App *app = Get_Self_AppPointer();
	LCUI_FuncQueue *queue =& app->task_queue;
	
	LCUI_Func *temp = NULL;
	
	total = Queue_Get_Total(queue);
	if(mode != ADD_MODE_ADD_NEW) {/* 如果模式不是“添加新的”模式 */
		//printf("mode: %d\n", mode);
		for (i = 0; i < total; ++i) {
			//printf("1\n");
			temp = Queue_Get(queue, i);
			/* 如果函数指针已有记录 */
			if(temp->func != func_data->func) {
				continue;
			}
			//printf("2\n");/* 如果要求的是不重复模式 */ 
			if(Check_Option(mode, ADD_MODE_NOT_REPEAT)) {
	//	printf("3\n");/* 如果要求是第1个参数不能重复 */
				if(Check_Option(mode, AND_ARG_F)) {
					//printf("ADD_MODE_NOT_REPEAT, AND_ARG_F\n");
					//printf("old:%p, new:%p\n", queue->queue[i].arg_f, arg_f);
					/* 如果要求是第2个参数也不能重复 */
					if(Check_Option(mode, AND_ARG_S)) {
						if(temp->arg[0] == func_data->arg[0] 
						&& temp->arg[1] == func_data->arg[1] 
						) {/* 如果实际上函数以及参数1和2都一样 */ 
							return -1; 
						}
					} else {/* 否则，只是要求函数以及第1个参数不能全部重复 */
						if(temp->arg[0] == func_data->arg[0])  
							return -1; 
					}
				}/* 否则，如果只是要求是第2个参数不能重复 */
				else if(Check_Option(mode, AND_ARG_S)) {
					if(temp->arg[1] == func_data->arg[1] ) {
						return -1; 
					}
				}
				else {/* 否则，只是要求函数不同 */ 
					return -1; 
				}
			}/* 如果要求的是替换模式 */
			else if(Check_Option(mode, ADD_MODE_REPLACE)) {
				//printf("ADD_MODE_REPLACE\n");
				/* 如果要求是第1个参数相同 */
				if( Check_Option(mode, AND_ARG_F) ) {
					/* 如果要求是第2个参数也相同 */
					if( Check_Option(mode, AND_ARG_S) ) {
						if(temp->arg[0] == func_data->arg[0] 
						&& temp->arg[1] == func_data->arg[1]
						) {
							break; 
						}
					} else {/* 否则，只是要求函数以及第1个参数全部相同 */
						if(temp->arg[0] == func_data->arg[0]) {
					//		printf("ARG_F\n");
							break; 
						}
					}
				}/* 否则，如果只是要求第2个参数不能相同 */
				else if(Check_Option(mode, AND_ARG_S)) {
					if(temp->arg[1] == func_data->arg[1]) {
						break; 
					}
				}
				else {/* 否则，只是要求函数不同 */ 
					break; 
				}
			}
		//	printf("4\n"); 
		}
		//printf("5\n");
		if(i == total) {
			Queue_Add(queue, func_data); 
		} else {
			Queue_Replace( queue, i, func_data ); 
		}
	} else {
		Queue_Add(queue, func_data); 
	}
	//		printf("6\n");
	return 0;
}
/**************************** Task End ********************************/




/***************************** Event ***********************************/
static void Destroy_Event(void *arg)
/* 功能：销毁事件数据 */
{
	LCUI_Event *event = (LCUI_Event*)arg;
	if(event != NULL) {
		Destroy_Queue(&event->func_data); 
	}
}

void EventQueue_Init(LCUI_EventQueue * queue)
/* 功能：初始化事件队列 */
{
	/* 
	 * 由于LCUI_Event结构体的成员中有函数队列，销毁事件时需要把该队列销毁，所以需
	 * 要Destroy_Event()作为析构函数 
	 * */
	Queue_Init(queue, sizeof(LCUI_Event), Destroy_Event);
}


int Get_FuncData(LCUI_Func *p, void (*func) (), void *arg1, void *arg2)
/* 
 * 功能：将函数指针以及两个参数，转换成LCUI_Func类型的指针
 * 说明：此函数会申请内存空间，并返回指向该空间的指针
 *  */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	
	if(NULL == app) {
		printf("Get_FuncData(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	
	p->id = app->id;
	p->func = func;
	/* 
	 * 只是保存了指向参数的指针，如果该参数是局部变量，在声明它的函数退出后，该变量
	 * 的空间可能会无法访问。
	 *  */
	p->arg[0] = arg1;	
	p->arg[1] = arg2;
	
	return 0;
}

LCUI_Event *Find_Event(LCUI_EventQueue *queue, int event_id)
/* 功能：根据事件的ID，获取指向该事件的指针 */
{
	LCUI_Event *event; 
	int i, total;  
	total = Queue_Get_Total(queue);
	if (total > 0) {
		for (i = 0; i < total; ++i) {
			event = (LCUI_Event*)Queue_Get(queue, i);
			if(event->id == event_id)
				return event;
		}
	}
	
	return NULL;
}

int EventQueue_Add(LCUI_EventQueue *queue, int event_id, LCUI_Func *func)
/* 功能：记录事件及对应回调函数至队列 */
{
	LCUI_Event *event;
	
	event = Find_Event(queue, event_id);
	if (NULL == event) {/* 如果没有，就添加一个新事件类型 */ 
		int pos;
		LCUI_Event new_event;
		new_event.id = event_id;
		Queue_Init(&new_event.func_data, sizeof(LCUI_Func), NULL);
		pos = Queue_Add(queue, &new_event);/* 将新数据追加至队列 */
		event = Queue_Get(queue, pos);	/* 获取指向新增成员的指针 */
	}
	
	event->id = event_id; /* 保存事件ID */
	Queue_Add(&event->func_data, func);
	return 0;
}

int LCUI_MouseEvent_Connect (void (*func) (), int event_id)
/* 
 * 功能：将函数与鼠标的相关事件相关联
 * 说明：当鼠标事件触发后，会先将已关联该事件的函数指针及相关事件的指针
 * 添加至程序的任务队列，等待程序在主循环中处理
 **/
{
	LCUI_Func func_data;
	if(0 != Get_FuncData(&func_data, func, NULL, NULL) ) {
		return -1;
	}
	/* 
	 * 将函数指针及第一个参数加入至鼠标事件处理队列
	 * 中，等到处理鼠标事件时，会将按键状态作为该函
	 * 数的第二个参数并转移至程序的任务队列 
	 **/ 
	EventQueue_Add(&LCUI_Sys.mouse.event, event_id, &func_data); 
	return 0;
}


int LCUI_Key_Event_Connect (int key_value, void (*func) (), void *arg)
/* 功能：将函数与按键的某个事件相连接，当这个按键按下后，就会调用这个函数 */
{
	LCUI_Func func_data;
	LCUI_App *app = Get_Self_AppPointer();
	if(NULL == app) {
		printf("LCUI_Key_Event_Connect(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	
	Get_FuncData(&func_data, func, arg, NULL);
	return EventQueue_Add(&LCUI_Sys.key_event, key_value, &func_data);
}


int Handle_Event(LCUI_EventQueue *queue, int event_id)
/* 
 * 功能：处理指定ID的事件
 * 说明：本函数会将事件队列中与指定ID的事件关联的回调函数 添加至程序的任务队列
 * */
{
	LCUI_Event *event;
	LCUI_Func *func;
	int total, i;
	event = Find_Event(queue, event_id);
	if(NULL == event) {
		return -1;
	}
	total = Queue_Get_Total(&event->func_data);
	for (i = 0; i < total; ++i) {
		func = Queue_Get(&event->func_data, i);
		/* 添加至程序的任务队列 */ 
		AppTask_Custom_Add(ADD_MODE_ADD_NEW, func);
	}
	return 0;
}


/****************** 处理部件拖动/点击事件的相关代码 ************************/
static LCUI_Widget *click_widget = NULL, *overlay_widget = NULL;
static LCUI_Pos __offset_pos = {0, 0};  /* 点击部件时保存的偏移坐标 */ 
static LCUI_DragEvent drag_event;

int Widget_Drag_Event_Connect ( 
		LCUI_Widget *widget, 
		void (*func)(LCUI_Widget*, LCUI_DragEvent *)
)
/* 
 * 功能：将回调函数与部件的拖动事件进行连接 
 * 说明：建立连接后，当部件被点击，拖动，释放，都会调用回调函数
 * */
{
	LCUI_DragEvent *p;
	LCUI_Func func_data;
	
	if(widget == NULL) {
		return -1;
	}
	p = &drag_event;
	if(0 != Get_FuncData(&func_data, func, (void*)widget, p)) {
		return -1;
	}
	EventQueue_Add(&widget->event, EVENT_DRAG, &func_data);
	return 0;
}

int Widget_Clicked_Event_Connect (
			LCUI_Widget *widget,
			void (*func)(LCUI_Widget*, void *), 
			void *arg
)
/* 
 * 功能：将回调函数与部件的拖动事件进行连接 
 * 说明：建立连接后，部件会将新位置作为第二参数，传给回调函数
 * */
{
	LCUI_Func func_data;
	if(widget == NULL) return -1;
	if(0 != Get_FuncData(&func_data, func, (void*)widget, arg)) {
		return -1;
	}
	EventQueue_Add(&widget->event, EVENT_CLICKED, &func_data);
	return 0;
}

static LCUI_Widget *Widget_Find_Response_Status_Change(LCUI_Widget *widget)
/* 
 * 功能：查找能响应状态改变的部件 
 * 说明：此函数用于检查部件以及它的上级所有父部件，第一个有响应状态改变的部件的指针将会
 * 作为本函数的返回值。
 * */
{
	if(NULL == widget) {
		return NULL;
	}
	if(widget->response_flag == 1) {
		return widget;/* 如果部件响应状态改变，那就返回该部件的指针 */
	}
	if(widget->parent == NULL) {
		return NULL; /* 如果父部件为空，那就没找到，返回NULL */
	} else {/* 否则，在它的父级部件中找 */
		return Widget_Find_Response_Status_Change(widget->parent); 
	}
}

static LCUI_Widget *Widget_Find_Response_Event(LCUI_Widget *widget, int event_id)
/* 
 * 功能：查找能响应事件的部件 
 * 说明：此函数用于检查部件以及它的上级所有父部件，第一个有响应指定事件的部件，它的指针
 * 将会作为本函数的返回值
 * */
{ 
	LCUI_Event *event;
	if(NULL == widget) {
		return NULL;
	}
	event = Find_Event(&widget->event, event_id);
	if(event != NULL) {
		return widget;/* 如果部件响应状态改变，则返回该部件的指针 */
	}
	if(widget->parent == NULL) {/* 如果父部件为空，说明没找到，返回NULL */
		return NULL; 
	} else {/* 否则，在它的父级部件中找 */
		return Widget_Find_Response_Event(widget->parent, event_id); 
	}
}

static void Widget_Clicked(LCUI_MouseEvent *event)
/*
 * 功能：用于处理click事件，并保存被点击后的部件的指针
 * 说明：在鼠标左键被按下/释放时，都会调用这个函数
 **/
{
	LCUI_Widget *widget; 
	//printf("Widget_Clicked() :start!!\n"); 
	if(event == NULL) {
		return;
	}
	
	widget = event->widget;  
	//printf("event widget: ");
	//print_widget_info(widget);

	widget = Widget_Find_Response_Event(widget, EVENT_CLICKED);
	if(NULL == widget) {
		widget = Widget_Find_Response_Event(event->widget, EVENT_DRAG);  
	}
	//printf("widget :");
	//print_widget_info(widget);
	if( Mouse_LeftButton(event) == PRESSED ) {/* 如果是鼠标左键被按下 */ 
		/* widget必须指向能响应状态变化的部件，因此，需要查找 */
		click_widget = widget; 
		if(widget != NULL) {
			//printf("1\n");
			/* 用全局坐标减去部件的坐标，得到偏移位置 */ 
			__offset_pos = Pos_Sub(
				event->global_pos, 
				Get_Widget_Global_Pos( widget )
			);
			/* 得出部件的新位置 */
			drag_event.new_pos = Pos_Sub(event->global_pos, __offset_pos); 
			drag_event.first_click = 1; 
			drag_event.end_click = 0;
			/* 处理部件的拖动事件 */
			Handle_Event(&widget->event, EVENT_DRAG); 
		}
			//printf("2\n");
		widget = Widget_Find_Response_Status_Change(widget); 
		if(widget != NULL) {
			//printf("3\n");
			/* 如果当前鼠标指针覆盖到的部件已被启用 */  
			if( event->widget->enabled == IS_TRUE
			 && widget->enabled == IS_TRUE ) {
				Set_Widget_Status (widget, WIDGET_STATUS_CLICKING); 
			} else {
				Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
			}
		}
			//printf("4\n");
	}
	else if (Mouse_LeftButton (event) == FREE) {
		/* 否则，如果鼠标左键是释放状态 */
	//printf("5\n");
		if(click_widget != NULL) {
			__offset_pos = Pos_Sub(
				event->global_pos, 
				Get_Widget_Global_Pos( click_widget )
			); 
			drag_event.new_pos = Pos_Sub(event->global_pos, __offset_pos); 
			drag_event.first_click = 0; 
			drag_event.end_click = 1; 
			Handle_Event(&click_widget->event, EVENT_DRAG);
			/* 如果点击时和点击后都在同一个按钮部件内进行的 */
			if(click_widget == widget) {
				/* 
				 * 触发CLICKED事件，将部件中关联该事件的回调函数发送至
				 * 任务队列，使之在主循环中执行 
				 * */  
				//printf("send clicked event\n"); 
		//printf("6\n");
				if(widget->enabled == IS_TRUE)
					Handle_Event(&widget->event, EVENT_CLICKED);
				
				widget = Widget_Find_Response_Status_Change(event->widget);
				if(NULL != widget){
					if(widget->enabled == IS_TRUE) { 
						Set_Widget_Status (widget, WIDGET_STATUS_CLICKED);
						Set_Widget_Status (widget, WIDGET_STATUS_OVERLAY);
					} else {
						Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
					}
				}
		//printf("7\n");
			} else {/* 否则，将恢复之前点击的鼠标的状态 */
		//printf("8\n");
				widget = Widget_Find_Response_Status_Change(click_widget);
				if(widget != NULL) {
					if(widget->enabled == IS_TRUE)
						Set_Widget_Status (widget, WIDGET_STATUS_NORMAL);
					else
						Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
				}
			} 
		}
		click_widget = NULL; /* 指针赋值为空 */
	} 
	//printf("Widget_Clicked() :end!!\n");  
}

static void Tracking_Mouse_Move (LCUI_MouseEvent *event)
/* 
 * 功能：跟踪鼠标移动，处理触发的基本事件
 * 说明：这只是根据鼠标事件来处理部件状态的切换
 * */
{
	DEBUG_MSG("Tracking_Mouse_Move(): start\n");
	LCUI_Widget *widget;
	/* 获取当前鼠标游标覆盖到的部件的指针 */
	widget = Get_Cursor_Overlay_Widget(); 
	//print_widget_info(widget);
	//print_widget_info(overlay_widget);
	if(widget == NULL) {
		goto skip_widget_check;
	}
	/* 获取能响应状态改变的部件的指针 */
	widget = Widget_Find_Response_Status_Change(widget); 
	if( widget == NULL || overlay_widget == widget ) {
		goto skip_widget_check;
	} 
	DEBUG_MSG("check widget whether enabled\n");
	if (widget->enabled == IS_TRUE) { 
		DEBUG_MSG("widget not enabled");
		if( click_widget == NULL ) {
			DEBUG_MSG("leftbutton is free, widget overlay\n\n");
			Set_Widget_Status (widget, WIDGET_STATUS_OVERLAY);
		} 
	} else {
		Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
	}
	
	/* 如果之前有覆盖到的部件 */
	if (overlay_widget != NULL && click_widget == NULL ) { 
		/* 如果按钮部件被启用 */
		if (overlay_widget->enabled == IS_TRUE) {
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_NORMAL);
		} else {
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_DISABLE);
		}
	}
	/* 保存当前覆盖到的按钮部件指针 */
	overlay_widget = widget;
	return;

skip_widget_check:;

	//printf("6\n");
	if(overlay_widget != widget && click_widget == NULL) {
		/* 如果鼠标指针在之前有覆盖到的部件 */ 
		DEBUG_MSG("7\n");
		if (overlay_widget->enabled == IS_TRUE) {/* 如果部件可用，就让它恢复到普通状态 */
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_NORMAL); 
		} else {/* 否则，部件为不可用的状态 */
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_DISABLE);
		}
	}
	DEBUG_MSG("8\n");
	overlay_widget = widget;
	/* 触发部件拖动事件 */ 
	if(click_widget != NULL && Mouse_LeftButton (event) == PRESSED) {
		/* 如果之前点击过部件，并且现在鼠标左键还处于按下状态 */ 
		drag_event.new_pos = Pos_Sub(event->global_pos, __offset_pos); 
		Handle_Event(&click_widget->event, EVENT_DRAG);
		drag_event.first_click = 0; 
		drag_event.end_click = 0; 
		/* 处理部件的拖动事件 */
		Handle_Event(&click_widget->event, EVENT_DRAG);
	}
	DEBUG_MSG("Tracking_Mouse_Move(): end\n");
}

void Widget_Event_Init()
/* 功能：初始化部件事件处理 */
{
	LCUI_MouseEvent_Connect(Tracking_Mouse_Move, MOUSE_EVENT_MOVE);
	LCUI_MouseEvent_Connect(Widget_Clicked, MOUSE_EVENT_CLICK);
}

/*************************** Event End *********************************/
