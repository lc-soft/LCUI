/* ***************************************************************************
 * widget_event.c -- LCUI widget event module.
 * 
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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
//#define DEBUG
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>

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

enum WidgetStatusType { 
	WST_HOVER,
	WST_ACTIVE,
	WST_TOTAL
};

static struct ModuleContext { 
	LinkedList pending_list;		/**< 待处理部件列表（按事件触发时间先后排列） */
	LCUI_Widget targets[WST_TOTAL];		/**< 相关的部件 */
} self;

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
static void WidgetEventHandler( LCUI_Event *e, LCUI_WidgetEventTask *task )
{
	LCUI_Widget w;
	LCUI_WidgetEventPack *pack;
	pack = (LCUI_WidgetEventPack*)e->data;
	pack->event.data = task->data;
	pack->event.type = e->id;
	pack->event.type_name = e->name;
	w = pack->widget;
	task->func( w, &pack->event, pack->data );
	if( pack->event.cancel_bubble ) {
		return;
	}
	if( !w->parent ) {
		return;
	}
	pack->event.x += w->box.border.x;
	pack->event.y += w->box.border.y;
	w = w->parent;
	pack->event.x += w->padding.left;
	pack->event.y += w->padding.top;
	pack->widget = w;
	/** 向父级部件冒泡传递事件 */
	LCUIEventBox_Send( w->event, e->name, pack );
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

int Widget_AddEvent( LCUI_Widget widget, const char *event_name, int id )
{
	return LCUIEventBox_AddEvent( &widget->event, event_name, id );
}

int Widget_BindEvent(	LCUI_Widget widget, const char *event_name,
			void(*func)(LCUI_Widget,LCUI_WidgetEvent*, void*), 
			void *func_data, void (*destroy_data)(void*) )
{
	LCUI_WidgetEventTask *task;
	task = NEW(LCUI_WidgetEventTask, 1);
	task->func = func;
	task->data = func_data;
	task->data_destroy = destroy_data;
	DEBUG_MSG("event: %s, task: %p\n", event_name, task);
	return LCUIEventBox_Bind( 
		widget->event, event_name, 
		OnWidgetEvent, task, DestroyWidgetEventTask
	);
}

int Widget_UnbindEvent( LCUI_Widget widget, const char *event_name )
{
	return 0;
}

int Widget_UnbindEventById( LCUI_Widget widget, int id )
{
	return 0;
}

static LCUI_Widget Widget_GetNextAt( LCUI_Widget widget, int x, int y )
{
	LCUI_Widget w;
	LinkedListNode *node;
	node = Widget_GetShowNode( widget );
	for( node = node->next; node; node = node->next ) {
		w = node->data;
		/* 如果忽略事件处理，则向它底层的兄弟部件传播事件 */
		if( w->computed_style.pointer_events == SV_NONE ) {
			continue;
		}
		if( !w->computed_style.visible ) {
			continue;
		}
		if( !LCUIRect_HasPoint( &w->box.border, x, y ) ) {
			continue;
		}
		return w;
	}
	return NULL;
}

int Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent *e, void *data )
{
	int ret;
	LCUI_WidgetEventPack *pack;
	pack = NEW( LCUI_WidgetEventPack, 1 );
	pack->is_direct_run = FALSE;
	pack->widget = widget;
	pack->data = data;
	pack->event = *e;
	switch( e->type ) {
	case WET_CLICK:
	case WET_MOUSEDOWN:
	case WET_MOUSEMOVE:
	case WET_MOUSEUP:
	case WET_MOUSEOVER:
	case WET_MOUSEOUT:
		if( widget->computed_style.pointer_events == SV_NONE ) {
			break;
		}
	default:
		ret = LCUIEventBox_Post( widget->event, e->type_name,
					 pack, NULL );
		if( ret == 0 ) {
			LinkedList_Append( &self.pending_list, widget );
			return 0;
		}
		free( pack );
		if( !widget->parent ) {
			return -1;
		}
		/* 如果事件投递失败，则向父级部件冒泡 */
		return Widget_PostEvent( widget->parent, e, data );
	}
	if( !widget->parent ) {
		free( pack );
		return -1;
	}
	/* 转换成相对于父级部件内容框的坐标 */
	pack->event.x += widget->box.border.x;
	pack->event.y += widget->box.border.y;
	/* 从当前部件后面找到当前坐标点命中的兄弟部件 */
	widget = Widget_GetNextAt( widget, pack->event.x, pack->event.y );
	/* 找不到就向父级部件冒泡 */
	if( !widget ) {
		free( pack );
		return Widget_PostEvent( widget->parent, e, data );
	}
	pack->event.x -= widget->box.border.x;
	pack->event.y -= widget->box.border.y;
	LinkedList_Append( &self.pending_list, widget );
	return LCUIEventBox_Post( widget->event, e->type_name, pack, NULL );
}

int Widget_SendEvent( LCUI_Widget widget, LCUI_WidgetEvent *e, void *data )
{
	int ret;
	LCUI_WidgetEventPack pack;
	pack.data = data;
	pack.event = *e;
	pack.widget = widget;
	pack.is_direct_run = TRUE;
	switch( e->type ) {
	case WET_CLICK:
	case WET_MOUSEDOWN:
	case WET_MOUSEMOVE:
	case WET_MOUSEUP:
	case WET_MOUSEOVER:
	case WET_MOUSEOUT:
		if( widget->computed_style.pointer_events == SV_NONE ) {
			break;
		}
	default:
		ret = LCUIEventBox_Send( widget->event, e->type_name, &pack );
		if( ret == 0 ) {
			return 0;
		}
		if( !widget->parent ) {
			return -1;
		}
		return Widget_SendEvent( widget->parent, e, data );
	}
	if( !widget->parent ) {
		return -1;
	}
	pack.event.x += widget->box.border.x;
	pack.event.y += widget->box.border.y;
	widget = Widget_GetNextAt( widget, pack.event.x, pack.event.y );
	if( !widget ) {
		return Widget_SendEvent( widget->parent, e, data );
	}
	pack.event.x -= widget->box.border.x;
	pack.event.y -= widget->box.border.y;
	return LCUIEventBox_Send( widget->event, e->type_name, &pack );
}

/** 更新状态 */
static void Widget_UpdateStatus( LCUI_Widget widget, int type )
{
	int depth = 0, i;
	LCUI_WidgetEvent e;
	const char *sname = (type == WST_HOVER ? "hover":"active");
	LCUI_Widget w, root, new_w = widget, old_w = self.targets[type];

	if( self.targets[type] == widget ) {
		return;
	}
	root = LCUIWidget_GetRoot();
	self.targets[type] = widget;
	/* 先计算两个部件的嵌套深度的差值 */
	for( w = new_w; w != root && w; w = w->parent ) {
		++depth;
	}
	for( w = old_w; w != root && w; w = w->parent ) {
		--depth;
	}
	i = depth > 0 ? depth : -depth;
	/* 然后向父级遍历，直到两个部件的父级部件相同为止 */
	while( new_w != old_w ) {
		/* 如果是新部件嵌套得较深，则先遍历几次直到深度
		 * 相同时再一起遍历 */
		if( new_w && (i == 0 || (i > 0 && depth > 0)) ) {
			Widget_AddStatus( new_w, sname );
			if( type == WST_HOVER ) {
				e.target = w;
				e.type = WET_MOUSEOVER;
				e.cancel_bubble = FALSE;
				e.type_name = "mouseover";
				Widget_PostEvent( new_w, &e, NULL );
			}
			new_w = new_w->parent;
		}
		if( old_w && (i == 0 || (i > 0 && depth < 0)) ) {
			Widget_RemoveStatus( old_w, sname );
			if( type == WST_HOVER ) {
				e.target = old_w;
				e.type = WET_MOUSEOUT;
				e.cancel_bubble = FALSE;
				e.type_name = "mouseout";
				Widget_PostEvent( old_w, &e, NULL );
			}
			old_w = old_w->parent;
		}
		i > 0 ? --i:0;
	}
}

/** 响应系统的鼠标移动事件，向目标部件投递相关鼠标事件 */
static void OnMouseEvent( LCUI_SystemEvent *e, void *arg )
{
	LCUI_Pos pos;
	LCUI_WidgetEvent ebuff;
	LCUI_Widget target, root;
	root = LCUIWidget_GetRoot();
	LCUICursor_GetPos( &pos );
	target = Widget_At( root, pos.x, pos.y );
	if( !target ) {
		Widget_UpdateStatus( NULL, WST_HOVER );
		return;
	}
	Widget_GetAbsXY( target, NULL, &ebuff.x, &ebuff.y );
	ebuff.cancel_bubble = FALSE;
	ebuff.x = pos.x - ebuff.x;
	ebuff.y = pos.y - ebuff.y;
	ebuff.screen_x = pos.x;
	ebuff.screen_y = pos.y;
	ebuff.target = target;
	ebuff.which = 0;
	switch( e->type ) {
	case LCUI_MOUSEDOWN:
		ebuff.type = WET_MOUSEDOWN;
		ebuff.type_name = "mousedown";
		Widget_PostEvent( target, &ebuff, NULL );
		Widget_UpdateStatus( target, WST_ACTIVE );
		break;
	case LCUI_MOUSEUP:
		ebuff.type = WET_MOUSEUP;
		ebuff.type_name = "mouseup";
		Widget_PostEvent( target, &ebuff, NULL );
		DEBUG_MSG( "target: %s, prev: %s\n", target->type, 
			    self.targets[WST_ACTIVE] ? self.targets[WST_ACTIVE]->type :"null" );
		if( self.targets[WST_ACTIVE] == target ) {
			ebuff.type = WET_CLICK;
			ebuff.type_name = "click";
			Widget_PostEvent( target, &ebuff, NULL );
		}
		Widget_UpdateStatus( NULL, WST_ACTIVE );
		break;
	case LCUI_MOUSEMOVE:
		ebuff.type = WET_MOUSEMOVE;
		ebuff.type_name = "mousemove";
		Widget_PostEvent( target, &ebuff, NULL );
		break;
	default:return;
	}
	Widget_UpdateStatus( target, WST_HOVER );
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

int Widget_PostSurfaceEvent( LCUI_Widget w, int event_type )
{
	LCUI_Widget root;
	LCUI_WidgetEvent e;
	int *n = (int*)&event_type;
	root = LCUIWidget_GetRoot();
	if( w->parent != root && w != root ) {
		return -1;
	}
	e.target = w;
	e.type_name = "surface";
	DEBUG_MSG("widget: %s, post event: %d\n", w->type,event_type );
	return Widget_PostEvent( root, &e, *((int**)n) );
}

void LCUIWidget_StepEvent(void)
{
	int count;
	LCUI_Widget widget;
	LinkedListNode *prev, *node;

	DEBUG_MSG("widget total: %d\n", n);
	LinkedList_ForEach( node, &self.pending_list ) {
		widget = node->data;
		prev = node->prev;
		LinkedList_DeleteNode( &self.pending_list, node );
		node = prev;
		DEBUG_MSG("dispatch event, widget: %p\n", widget->type);
		count = LCUIEventBox_Dispatch( widget->event );
		if( count == 0 ) {
			//WidgetEventHandler( )
		}
	}
	DEBUG_MSG("exit\n");
}

void LCUIWidget_InitEvent(void)
{
	LinkedList_Init( &self.pending_list );
	LCUI_BindEvent( "mousedown", OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( "mousemove", OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( "mouseup", OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( "keyup", OnKeyUp, NULL, NULL );
	LCUI_BindEvent( "keydown", OnKeyDown, NULL, NULL );
	LCUI_BindEvent( "keypress", OnKeyPress, NULL, NULL );
	LCUI_BindEvent( "input", OnInput, NULL, NULL );
	self.targets[WST_ACTIVE] = NULL;
	self.targets[WST_HOVER] = NULL;
}

void LCUIWidget_ExitEvent(void)
{
	
}
