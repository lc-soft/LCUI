/* ***************************************************************************
 * widget_event.c -- LCUI widget event module.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * widget_event.c -- LCUI部件事件模块
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/widget_build.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>

#define NEW_ONE(type) (type*)malloc(sizeof(type))

typedef struct LCUI_WidgetEventPack {
	void *data;			/**< 额外数据 */
	LCUI_Widget widget;		/**< 当前处理该事件的部件 */
	LCUI_WidgetEvent event;		/**< 事件数据 */
	LCUI_BOOL is_direct_run;	/**< 是否直接处理该事件 */
} LCUI_WidgetEventPack;

typedef struct LCUI_WidgetEventTask {
	void (*func)(LCUI_Widget, LCUI_WidgetEvent*, void*);
	void *data;
	void (*data_destroy)(void*);
} LCUI_WidgetEventTask;

static LCUI_RBTree widget_mark_tree;	/**< 记录当前已经标记的部件 */
static LinkedList widget_list;		/**< 待处理部件列表（按事件触发时间先后排列） */

static int CompareWidgetMark( void *data, const void *key )
{
	return data == key ? 0:(data > key ? 1:-1);
}

static void DestroyWidgetEventTask( void *arg )
{
	LCUI_WidgetEventTask *task = (LCUI_WidgetEventTask*)arg;
	if( task->data ) {
		task->data_destroy ? task->data_destroy( task->data ):FALSE;
		free( task->data );
		task->data = NULL;
	}
}

/** 将原始事件转换成部件事件 */
static void WidgetEventHandler( LCUI_Event *event, LCUI_WidgetEventTask *task )
{
	LCUI_Widget widget;
	LCUI_WidgetEventPack *pack = (LCUI_WidgetEventPack*)event->data;

	pack->event.data = task->data;
	pack->event.type = event->id;
	pack->event.type_name = event->name;
	DEBUG_MSG("event: %s, task: %p\n", event->name, task);

	switch( event->id ) {
	case LCUI_INPUT:
	case LCUI_MOUSEUP:
	case LCUI_MOUSEDOWN:
	case LCUI_KEYUP:
	case LCUI_KEYDOWN:
	case LCUI_KEYPRESS:
	default: break;
	}
	
	widget = pack->widget;
	task->func( widget, &pack->event, pack->data );
	if( pack->event.cancel_bubble ) {
		return;
	}
	/* 开始进行事件冒泡传播 */
	while( widget ) {
		int i = 0, n = 0;
		LinkedList *children;
		
		if( widget->parent ) {
			children = &widget->parent->children_show;	
		} else {
			children = &LCUIRootWidget->children_show;
		}
		n = LinkedList_GetTotal( children );
		/* 确定自己的显示位置，忽略显示在它前面的部件 */
		for( i=0; i<n; ++i ) {
			LinkedList_Goto( children, i );
			if( widget == (LCUI_Widget)LinkedList_Get(children) ) {
				break;
			}
		}
		/* 向后面的部件传播事件 */
		for( i=i+1; i<n; ++i ) {
			LinkedList_Goto( children, i );
			widget = (LCUI_Widget)LinkedList_Get( children );
			if( !widget->style.visible ) {
				continue;
			}
			task->func( widget, &pack->event, pack->data );
			if( pack->event.cancel_bubble ) {
				return;
			}
		}
		/* 开始向父级部件传播事件 */
		widget = widget->parent;
	}
}

/** 响应原始事件 */
static void OnWidgetEvent( LCUI_Event *event, void *arg )
{
	LCUI_Task task;
	LCUI_WidgetEventTask *p_wet = (LCUI_WidgetEventTask*)arg;
	LCUI_WidgetEventPack *p_wep = (LCUI_WidgetEventPack*)event->data;
	DEBUG_MSG("pack: %p, task: %p\n", p_wep, p_wet);
	/* 如果需要直接执行 */
	if( p_wep->is_direct_run ) {
		WidgetEventHandler( event, p_wet );
		return;
	}
	/* 准备任务 */
	task.func = (CallBackFunc)WidgetEventHandler;
	task.arg[0] = malloc(sizeof(LCUI_Event));
	task.arg[1] = malloc(sizeof(LCUI_WidgetEventPack));
	/* 这两个参数都需要在任务执行完后释放 */
	task.destroy_arg[0] = TRUE;
	task.destroy_arg[1] = TRUE;
	task.destroy_func[0] = NULL;
	task.destroy_func[1] = NULL;
	/* 复制所需数据，因为在本函数退出后，这两个参数会被销毁 */
	*((LCUI_Event*)task.arg[0]) = *event;
	*((LCUI_WidgetEventTask*)task.arg[1]) = *p_wet;
	/* 把任务扔给当前跑主循环的线程 */
	LCUI_AddTask( &task );
}

/**
 * 预先注册一个事件，并指定事件名和事件ID
 * 如果需要将多个事件绑定在同一个事件处理器上，并且，不想通过进行字符串比较来
 * 区分事件类型，则可以使用该函数，但需要注意的是，指定的事件ID最好不要与系统
 * 预置的部件事件ID相同（除非你是特意的），通常，部件事件ID号在 WIDGET_USER 
 * 以后的值都可以使用，例如：WET_USER + 1，WET_USER + 200。
 */
int Widget_RegisterEventWithId( LCUI_Widget widget, const char *event_name, int id )
{
	return LCUIEventBox_RegisterEventWithId( &widget->event, event_name, id );
}

/**
 * 为部件绑定事件
 * 需要提供事件的名称、事件处理器（回调函数）、附加数据、数据销毁函数。
 * 通常，事件处理器可能会需要更多的参数，这些参数可作为附加数据，每次
 * 调用事件处理器时，都可以根据附加数据进行相应的操作。
 * 附加数据会在解除事件绑定时被释放。
 */
int Widget_BindEvent(	LCUI_Widget widget, const char *event_name,
			void(*func)(LCUI_Widget,LCUI_WidgetEvent*, void*), 
			void *func_data, void (*destroy_data)(void*) )
{
	LCUI_WidgetEventTask *task;
	task = NEW_ONE(LCUI_WidgetEventTask);
	task->func = func;
	task->data = func_data;
	task->data_destroy = destroy_data;
	DEBUG_MSG("event: %s, task: %p\n", event_name, task);
	return LCUIEventBox_Bind( 
		widget->event, event_name, 
		OnWidgetEvent, task, DestroyWidgetEventTask
	);
}

/** 
 * 解除与事件的绑定
 * 这将解除所有与该事件绑定的事件处理器，当传入事件名为NULL时，将解除所有事件
 * 绑定。
 */
int Widget_UnbindEvent( LCUI_Widget widget, const char *event_name )
{
	return 0;
}

/** 
 * 解除指定的事件处理器的事件绑定
 * 需传入事件处理器的ID，该ID可在绑定事件时得到。
 */
int Widget_UnbindEventById( LCUI_Widget widget, int id )
{
	return 0;
}

/** 
 * 将事件投递给事件处理器，等待处理
 * 事件将会追加至事件队列中，等待下一轮的批处理时让对应的事件处理器进行处理
 */
int Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent *e, void *data )
{
	int ret;
	LCUI_WidgetEventPack *pack;

	pack = NEW_ONE(LCUI_WidgetEventPack);
	pack->data = data;
	pack->event = *e;
	pack->widget = widget;
	pack->is_direct_run = FALSE;
	DEBUG_MSG("pack: %p\n", pack);
	ret = LCUIEventBox_Post( widget->event, e->type_name, pack, NULL );
	if( !RBTree_CustomSearch( &widget_mark_tree, widget ) ) {
		RBTree_CustomInsert( &widget_mark_tree, widget, widget );
		LinkedList_AddData( &widget_list, widget );
	}
	return ret;
}

/** 
 * 直接将事件发送至处理器 
 * 这将会直接调用与事件绑定的事件处理器（回调函数）
 */
int Widget_SendEvent( LCUI_Widget widget, LCUI_WidgetEvent *e, void *data )
{
	LCUI_WidgetEventPack pack;
	pack.data = data;
	pack.event = *e;
	pack.widget = widget;
	pack.is_direct_run = TRUE;
	return LCUIEventBox_Send( widget->event, e->type_name, &pack );
}

/** 响应系统的鼠标移动事件，向目标部件投递相关鼠标事件 */
static void OnMouseEvent( LCUI_SystemEvent *e, void *arg )
{
	LCUI_Widget target;
	LCUI_WidgetEvent ebuff;
	LCUI_Pos pos;

	LCUICursor_GetPos( &pos );
	target = Widget_At( NULL, pos.x, pos.y );
	if( !target ) {
		return;
	}
	ebuff.x = pos.x;
	ebuff.y = pos.y;
	ebuff.target = target;
	ebuff.which = 0;
	ebuff.cancel_bubble = FALSE;
	switch( e->type ) {
	case LCUI_MOUSEDOWN:
		ebuff.type = WET_MOUSEDOWN;
		ebuff.type_name = "mousedown";
	case LCUI_MOUSEUP:
		ebuff.type = WET_MOUSEUP;
		ebuff.type_name = "mouseup";
	case LCUI_MOUSEMOVE:
		ebuff.type = WET_MOUSEMOVE;
		ebuff.type_name = "mousemove";
	}
	Widget_PostEvent( target, &ebuff, NULL );
}

/** 响应按键的按下 */
static void OnKeyDown( LCUI_SystemEvent *e, void *arg )
{

}

/** 响应按键的释放 */
static void OnKeyUp( LCUI_SystemEvent *e, void *arg )
{

}

/** 响应按键的输入 */
static void OnKeyPress( LCUI_SystemEvent *e, void *arg )
{

}

/** 响应输入法的输入 */
static void OnInput( LCUI_SystemEvent *e, void *arg )
{

}

/** 处理一次当前积累的部件事件 */
void LCUIWidget_Event_Step(void)
{
	int i, n;
	LCUI_Widget widget;

	n = LinkedList_GetTotal( &widget_list );
	DEBUG_MSG("widget total: %d\n", n);
	for( i=0; i<n; ++i ) {
		LinkedList_Goto( &widget_list, 0 );
		widget = (LCUI_Widget)LinkedList_Get( &widget_list );
		LinkedList_Delete( &widget_list );
		DEBUG_MSG("dispatch event, widget: %p, is_root: %s\n", widget, widget == LCUIRootWidget ? "TURE":"FALSE");
		LCUIEventBox_Dispatch( widget->event );
		RBTree_CustomErase( &widget_mark_tree, widget );
	}
}

/** 初始化 LCUI 部件的事件系统 */
void LCUIWidget_Event_Init(void)
{
	RBTree_Init( &widget_mark_tree );
	RBTree_OnJudge( &widget_mark_tree, CompareWidgetMark );
	RBTree_SetDataNeedFree( &widget_mark_tree, FALSE );
	LinkedList_Init( &widget_list, sizeof(LCUI_Widget) );
	LinkedList_SetDataNeedFree( &widget_list, FALSE );
	LCUI_BindEvent( "mousedown", OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( "mousemove", OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( "mouseup", OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( "keyup", OnKeyUp, NULL, NULL );
	LCUI_BindEvent( "keydown", OnKeyDown, NULL, NULL );
	LCUI_BindEvent( "keypress", OnKeyPress, NULL, NULL );
	LCUI_BindEvent( "input", OnInput, NULL, NULL );
}

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void LCUIWidget_Event_Destroy(void)
{
	
}
