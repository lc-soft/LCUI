/* ***************************************************************************
 * widget_event.c -- LCUI widget event module.
 *
 * Copyright (C) 2012-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2017 归属于 刘超 <lc-soft@live.cn>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/ime.h>
#include <LCUI/input.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>

#define DBLCLICK_INTERVAL	500

typedef struct TouchCapturerRec_ {
	LinkedList points;
	LCUI_Widget widget;
	LinkedListNode node;
} TouchCapturerRec, *TouchCapturer;

typedef struct WidgetEventHandlerRec_ {
	LCUI_WidgetEventFunc func;
	void *data;
	void (*destroy_data)(void*);
} WidgetEventHandlerRec, *WidgetEventHandler;

typedef struct LCUI_WidgetEventPackRec_ {
	void *data;			/**< 额外数据 */
	void (*destroy_data)(void*);	/**< 数据的销毁函数 */
	LCUI_Widget widget;		/**< 当前处理该事件的部件 */
	LCUI_WidgetEventRec event;	/**< 事件数据 */
} LCUI_WidgetEventPackRec, *LCUI_WidgetEventPack;

enum WidgetStatusType {
	WST_HOVER,
	WST_ACTIVE,
	WST_FOCUS,
	WST_TOTAL
};

/** 部件事件记录结点 */
typedef struct WidgetEventRecordRec_ {
	LCUI_Widget widget;	/**< 所属部件 */
	LinkedList records;	/**< 事件记录列表 */
} WidgetEventRecordRec, *WidgetEventRecord;

/** 事件标识号与名称的映射记录 */
typedef struct EventMappingRec_ {
	int id;
	char *name;
} EventMappingRec, *EventMapping;

/** 鼠标点击记录 */
typedef struct ClickRecord_ {
	int64_t time;		/**< 时间 */
	int x, y;		/**< 坐标 */
	int interval;		/**< 与上次点击时的时间间隔 */
	LCUI_Widget widget;	/**< 被点击的部件 */
} ClickRecord;

/** 当前功能模块的相关数据 */
static struct LCUIWidgetEvnetModule {
	LCUI_Widget mouse_capturer;		/**< 占用鼠标的部件 */
	LinkedList touch_capturers;		/**< 触点占用记录 */
	LCUI_Widget targets[WST_TOTAL];		/**< 相关的部件 */
	LinkedList events;			/**< 已绑定的事件 */
	LinkedList event_mappings;		/**< 事件标识号和名称映射记录列表  */
	RBTree event_records;			/**< 当前正执行的事件的记录 */
	RBTree event_names;			/**< 事件名称表，以标识号作为索引 */
	Dict *event_ids;			/**< 事件标识号表，以事件名称作为索引 */
	int base_event_id;			/**< 事件标识号计数器 */
	ClickRecord click;			/**< 上次鼠标点击记录 */
	LCUI_Mutex mutex;			/**< 互斥锁 */
} self;

static void DestroyEventMapping( void *data )
{
	EventMapping mapping = data;
	free( mapping->name );
	free( mapping );
}

static int CompareWidgetEventRecord( void *data, const void *keydata )
{
	WidgetEventRecord record = data;
	if( record->widget == (LCUI_Widget)keydata ) {
		return 0;
	} else if( record->widget < (LCUI_Widget)keydata ) {
		return -1;
	} else {
		return 1;
	}
}

static void DestroyWidgetEventHandler( void *arg )
{
	WidgetEventHandler handler = arg;
	if( handler->data && handler->destroy_data ) {
		handler->destroy_data( handler->data );
	}
	handler->data = NULL;
	free( handler );
}

/**
 * 添加事件记录
 * 记录当前待处理的事件和目标部件，方便在部件被销毁时清除待处理的事件
 */
static void Widget_AddEventRecord( LCUI_Widget widget,
				   LCUI_WidgetEventPack pack )
{
	WidgetEventRecord record;
	LCUIMutex_Lock( &self.mutex );
	record = RBTree_CustomGetData( &self.event_records, widget );
	if( !record ) {
		record = NEW( WidgetEventRecordRec, 1 );
		LinkedList_Init( &record->records );
		record->widget = widget;
		RBTree_CustomInsert( &self.event_records, widget, record );
	}
	LinkedList_Append( &record->records, pack );
	LCUIMutex_Unlock( &self.mutex );
}

/** 删除事件记录 */
static int Widget_DeleteEventRecord( LCUI_Widget widget,
				     LCUI_WidgetEventPack pack )
{
	int ret = 0;
	WidgetEventRecord record;
	LinkedListNode *node, *prev;
	LCUIMutex_Lock( &self.mutex );
	record = RBTree_CustomGetData( &self.event_records, widget );
	if( !record ) {
		LCUIMutex_Unlock( &self.mutex );
		return -1;
	}
	for( LinkedList_Each( node, &record->records ) ) {
		prev = node->prev;
		if( node->data == pack ) {
			LinkedList_DeleteNode( &record->records, node );
			node = prev;
			ret = 1;
		}
	}
	LCUIMutex_Unlock( &self.mutex );
	return ret;
}

/** 将原始事件转换成部件事件 */
static void WidgetEventTranslator( LCUI_Event e, LCUI_WidgetEventPack pack )
{
	WidgetEventHandler handler = e->data;
	LCUI_Widget w = pack->widget;
	if( !w ) {
		return;
	}
	pack->event.type = e->type;
	pack->event.data = handler->data;
	handler->func( w, &pack->event, pack->data );
	if( pack->event.cancel_bubble || !w->parent ) {
		return;
	}
	w = w->parent;
	pack->widget = w;
	/** 向父级部件冒泡传递事件 */
	EventTrigger_Trigger( w->trigger, e->type, pack );
}

/** 复制部件事件 */
static int CopyWidgetEvent( LCUI_WidgetEvent dst,
			    const LCUI_WidgetEvent src )
{
	int n;
	size_t size;

	*dst = *src;
	switch( src->type ) {
	case WET_TOUCH:
		if( dst->touch.n_points <= 0 ) {
			break;
		}
		n = dst->touch.n_points;
		size = sizeof( LCUI_TouchPointRec ) * n;
		dst->touch.points = malloc( size );
		if( !dst->touch.points ) {
			return -ENOMEM;
		}
		memcpy( dst->touch.points,
			src->touch.points, size );
		break;
	case WET_TEXTINPUT:
		if( !dst->text.text ) {
			break;
		}
		dst->text.text = NEW( wchar_t, dst->text.length + 1 );
		if( !dst->text.text ) {
			return -ENOMEM;
		}
		wcsncpy( dst->text.text, src->text.text,
			 dst->text.length + 1 );
	default:break;
	}
	return 0;
}

static void DestroyWidgetEvent( LCUI_WidgetEvent e )
{
	switch( e->type ) {
	case WET_TOUCH:
		if( e->touch.points ) {
			free( e->touch.points );
		}
		e->touch.points = NULL;
		e->touch.n_points = 0;
		break;
	case WET_TEXTINPUT:
		if( e->text.text ) {
			free( e->text.text );
		}
		e->text.text = NULL;
		e->text.length = 0;
		break;
	}
}

/** 销毁部件事件包 */
static void DestroyWidgetEventPack( void *arg )
{
	LCUI_WidgetEventPack pack = arg;
	/* 如果删除成功则说明有记录，需要销毁数据 */
	if( Widget_DeleteEventRecord( pack->event.target, pack ) == 1 ) {
		if( pack->data && pack->destroy_data ) {
			pack->destroy_data( pack->data );
		}
		DestroyWidgetEvent( &pack->event );
		pack->data = NULL;
		free( pack );
	}
}

static void DestroyTouchCapturer( void *arg )
{
	TouchCapturer tc = arg;
	LinkedList_Clear( &tc->points, free );
	tc->widget = NULL;
	free( tc );
}

#define TouchCapturers_Clear(LIST) \
LinkedList_ClearData( LIST, DestroyTouchCapturer )

static int TouchCapturers_Add( LinkedList *list, LCUI_Widget w, int point_id )
{
	int *data;
	TouchCapturer tc = NULL;
	LinkedListNode *node, *ptnode;
	if( point_id < 0 ) {
		tc = NEW( TouchCapturerRec, 1 );
		tc->widget = w;
		LinkedList_Init( &tc->points );
		TouchCapturers_Clear( list );
		LinkedList_Append( list, tc );
		return 0;
	}
	/* 获取该部件的触点捕捉记录 */
	for( LinkedList_Each( node, list ) ) {
		tc = node->data;
		/* 清除与该触点绑定的其它捕捉记录 */
		for( LinkedList_Each( ptnode, &tc->points ) ) {
			if( point_id == *(int*)ptnode->data ) {
				if( tc->widget == w ) {
					return 0;
				}
				return -1;
			}
		}
		if( tc->widget == w ) {
			break;
		}
	}
	/* 如果没有该部件的触点捕捉记录 */
	if( !tc ) {
		tc = NEW( TouchCapturerRec, 1 );
		tc->widget = w;
		tc->node.data = tc;
		LinkedList_Init( &tc->points );
		LinkedList_AppendNode( list, &tc->node );
	}
	/* 追加触点捕捉记录 */
	data = NEW( int, 1 );
	*data = point_id;
	LinkedList_Append( &tc->points, data );
	return 0;
}

static int TouchCapturers_Delete( LinkedList *list, LCUI_Widget w, int point_id )
{
	TouchCapturer tc = NULL;
	LinkedListNode *node, *ptnode;
	for( LinkedList_Each( node, list ) ) {
		tc = node->data;
		if( tc->widget == w ) {
			break;
		}
	}
	if( !tc || tc->widget != w ) {
		return -1;
	}
	if( point_id < 0 ) {
		LinkedList_Clear( &tc->points, free );
	} else {
		for( LinkedList_Each( ptnode, &tc->points ) ) {
			if( *(int*)ptnode->data == point_id ) {
				free( node->data );
				LinkedList_DeleteNode( &tc->points, ptnode );
			}
		}
	}
	if( tc->points.length == 0 ) {
		LinkedList_Unlink( &self.touch_capturers, &tc->node );
		free( tc );
	}
	return 0;
}

int LCUIWidget_SetEventName( int event_id, const char *event_name )
{
	int ret;
	EventMapping mapping;
	LCUIMutex_Lock( &self.mutex );
	if( Dict_FetchValue( self.event_ids, event_name ) ) {
		LCUIMutex_Unlock( &self.mutex );
		return -1;
	}
	mapping = malloc( sizeof( EventMappingRec ) );
	mapping->name = strdup( event_name );
	mapping->id = event_id;
	LinkedList_Append( &self.event_mappings, mapping );
	RBTree_Insert( &self.event_names, event_id, mapping );
	ret = Dict_Add( self.event_ids, mapping->name, mapping );
	LCUIMutex_Unlock( &self.mutex );
	return ret;
}

int LCUIWidget_AllocEventId( void )
{
	return self.base_event_id++;
}

const char *LCUIWidget_GetEventName( int event_id )
{
	EventMapping mapping;
	LCUIMutex_Lock( &self.mutex );
	mapping = RBTree_GetData( &self.event_names, event_id );
	LCUIMutex_Unlock( &self.mutex );
	return mapping ? mapping->name : NULL;
}

int LCUIWidget_GetEventId( const char *event_name )
{
	EventMapping mapping;
	LCUIMutex_Lock( &self.mutex );
	mapping = Dict_FetchValue( self.event_ids, event_name );
	LCUIMutex_Unlock( &self.mutex );
	return mapping ? mapping->id : -1;
}

int Widget_BindEventById( LCUI_Widget widget, int event_id,
			  LCUI_WidgetEventFunc func, void *data,
			  void( *destroy_data )(void*) )
{
	WidgetEventHandler handler;
	handler = NEW( WidgetEventHandlerRec, 1 );
	handler->func = func;
	handler->data = data;
	handler->destroy_data = destroy_data;
	return EventTrigger_Bind( widget->trigger, event_id,
				  (LCUI_EventFunc)WidgetEventTranslator,
				  handler, DestroyWidgetEventHandler );
}

int Widget_BindEvent( LCUI_Widget widget, const char *event_name,
		      LCUI_WidgetEventFunc func, void *data,
		      void (*destroy_data)(void*) )
{
	int id = LCUIWidget_GetEventId( event_name );
	if( id < 0 ) {
		return -1;
	}
	return Widget_BindEventById( widget, id, func, data, destroy_data );
}

static int CompareEventHandlerKey( void *key, void *func_data )
{
	WidgetEventHandler handler = func_data;
	if( key == handler->func ) {
		return 1;
	}
	return 0;
}

int Widget_UnbindEventById( LCUI_Widget widget, int event_id,
			    LCUI_WidgetEventFunc func )
{
	return EventTrigger_Unbind3( widget->trigger, event_id,
				     CompareEventHandlerKey, func );
}

int Widget_UnbindEvent( LCUI_Widget widget, const char *event_name,
			LCUI_WidgetEventFunc func )
{
	int id = LCUIWidget_GetEventId( event_name );
	if( id < 0 ) {
		return -1;
	}
	return Widget_UnbindEventById( widget, id, func );
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

static int Widget_TriggerEventEx( LCUI_Widget widget, LCUI_WidgetEventPack pack )
{
	LCUI_WidgetEvent e = &pack->event;
	pack->widget = widget;
	switch( e->type ) {
	case WET_CLICK:
	case WET_MOUSEDOWN:
	case WET_MOUSEUP:
	case WET_MOUSEMOVE:
	case WET_MOUSEOVER:
	case WET_MOUSEOUT:
		if( widget->computed_style.pointer_events == SV_NONE ) {
			break;
		}
	default:
		if( 0 < EventTrigger_Trigger( widget->trigger, 
					      e->type, pack ) ) {
			return 0;
		}
		if( !widget->parent || e->cancel_bubble ) {
			return -1;
		}
		/* 如果事件投递失败，则向父级部件冒泡 */
		return Widget_TriggerEventEx( widget->parent, pack );
	}
	if( !widget->parent || e->cancel_bubble ) {
		return -1;
	}
	while( widget->computed_style.pointer_events == SV_NONE ) {
		LCUI_Widget w;
		LCUI_BOOL is_pointer_event = TRUE;
		int x, y, pointer_x, pointer_y;

		switch( e->type ) {
		case WET_CLICK:
		case WET_MOUSEDOWN:
		case WET_MOUSEUP:
			pointer_x = e->button.x;
			pointer_y = e->button.y;
			break;
		case WET_MOUSEMOVE:
		case WET_MOUSEOVER:
		case WET_MOUSEOUT:
			pointer_x = e->motion.x;
			pointer_y = e->motion.y;
			break;
		default: 
			is_pointer_event = FALSE;
			break;
		}
		if( !is_pointer_event ) {
			break;
		}
		Widget_GetAbsXY( widget->parent, NULL, &x, &y );
		/* 转换成相对于父级部件内容框的坐标 */
		x = pointer_x - x;
		y = pointer_y - y;
		/* 从当前部件后面找到当前坐标点命中的兄弟部件 */
		w = Widget_GetNextAt( widget, x, y );
		if( !w ) {
			break;
		}
		return EventTrigger_Trigger( w->trigger, e->type, pack );
	}
	return Widget_TriggerEventEx( widget->parent, pack );
}

static void OnWidgetEvent( LCUI_Event e, LCUI_WidgetEventPack pack )
{
	if( pack->widget ) {
		Widget_TriggerEventEx( pack->widget, pack );
	}
}

LCUI_BOOL Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent ev,
			    void *data, void( *destroy_data )(void*) )
{
	LCUI_Event sys_ev;
	LCUI_AppTaskRec task;
	LCUI_WidgetEventPack pack;
	if( widget->state == WSTATE_DELETED ) {
		return FALSE;
	}
	if( !ev->target ) {
		ev->target = widget;
	}
	/* 准备任务 */
	task.func = (LCUI_AppTaskFunc)OnWidgetEvent;
	task.arg[0] = malloc( sizeof( LCUI_EventRec ) );
	task.arg[1] = malloc( sizeof( LCUI_WidgetEventPackRec ) );
	/* 这两个参数都需要在任务执行完后释放 */
	task.destroy_arg[1] = DestroyWidgetEventPack;
	task.destroy_arg[0] = free;
	sys_ev = task.arg[0];
	pack = task.arg[1];
	sys_ev->data = pack;
	sys_ev->type = ev->type;
	pack->data = data;
	pack->widget = widget;
	pack->destroy_data = destroy_data;
	CopyWidgetEvent( &pack->event, ev );
	Widget_AddEventRecord( widget, pack );
	/* 把任务扔给当前跑主循环的线程 */
	return LCUI_PostTask( &task );
}

int Widget_TriggerEvent( LCUI_Widget widget, LCUI_WidgetEvent e, void *data )
{
	LCUI_WidgetEventPackRec pack;
	if( !e->target ) {
		e->target = widget;
	}
	pack.event = *e;
	pack.data = data;
	pack.widget = widget;
	pack.destroy_data = NULL;
	return Widget_TriggerEventEx( widget, &pack );
}

int Widget_StopEventPropagation(LCUI_Widget widget )
{
	LinkedListNode *node;
	WidgetEventRecord record;
	LCUI_WidgetEventPack pack;
	LCUIMutex_Lock( &self.mutex );
	record = RBTree_CustomGetData( &self.event_records, widget );
	if( !record ) {
		LCUIMutex_Unlock( &self.mutex );
		return -1;
	}
	for( LinkedList_Each( node, &record->records ) ) {
		pack = node->data;
		pack->event.cancel_bubble = TRUE;
	}
	LCUIMutex_Unlock( &self.mutex );
	return 0;
}

/** 更新状态 */
static void Widget_UpdateStatus( LCUI_Widget widget, int type )
{
	int depth = 0, i;
	LCUI_WidgetEventRec e;
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
	i = depth;
	for( w = old_w; w != root && w; w = w->parent ) {
		if( w->state == WSTATE_DELETED ) {
			depth = i;
			old_w = w;
		}
		--depth;
	}
	i = depth > 0 ? depth : -depth;
	/* 然后向父级遍历，直到两个部件的父级部件相同为止 */
	while( new_w != old_w ) {
		/* 如果是新部件嵌套得较深，则先遍历几次直到深度相同时再一起遍历 */
		if( new_w && (i == 0 || (i > 0 && depth > 0)) ) {
			Widget_AddStatus( new_w, sname );
			if( type == WST_HOVER ) {
				e.target = w;
				e.type = WET_MOUSEOVER;
				e.cancel_bubble = FALSE;
				Widget_PostEvent( new_w, &e, NULL, NULL );
			}
			new_w = new_w->parent;
		}
		if( old_w && (i == 0 || (i > 0 && depth < 0)) ) {
			Widget_RemoveStatus( old_w, sname );
			if( type == WST_HOVER ) {
				e.target = old_w;
				e.type = WET_MOUSEOUT;
				e.cancel_bubble = FALSE;
				Widget_PostEvent( old_w, &e, NULL, NULL );
			}
			old_w = old_w->parent;
		}
		i > 0 ? --i:0;
	}
}

void LCUIWidget_ClearEventTarget( LCUI_Widget widget )
{
	int i;
	LCUI_Widget w;
	LinkedListNode *node;
	WidgetEventRecord record;
	LCUI_WidgetEventPack pack;
	LCUIMutex_Lock( &self.mutex );
	record = RBTree_CustomGetData( &self.event_records, widget );
	if( record ) {
		for( LinkedList_Each( node, &record->records ) ) {
			pack = node->data;
			pack->widget = NULL;
			pack->event.cancel_bubble = TRUE;
		}
	}
	LCUIMutex_Unlock( &self.mutex );
	for( i = 0; i < WST_TOTAL; ++i ) {
		if( !widget ) {
			Widget_UpdateStatus( NULL, i );
			continue;
		}
		for( w = self.targets[i]; w; w = w->parent ) {
			if( w == widget ) {
				Widget_UpdateStatus( NULL, i );
				break;
			}
		}
	}
}

int LCUIWidget_SetFocus( LCUI_Widget widget )
{
	LCUI_Widget w;
	LCUI_WidgetEventRec ev;
	/* 开始处理焦点 */
	for( w = widget; w; w = w->parent ) {
		if( w->computed_style.pointer_events != SV_NONE &&
		    w->computed_style.focusable && !w->disabled ) {
			break;
		}
	}
	if( self.targets[WST_FOCUS] == w ) {
		return 0;
	}
	if( self.targets[WST_FOCUS] ) {
		ev.type = WET_BLUR;
		ev.target = self.targets[WST_FOCUS];
		Widget_RemoveStatus( ev.target, "focus" );
		Widget_PostEvent( ev.target, &ev, NULL, NULL );
	}
	if( !w || !w->computed_style.focusable || w->disabled ) {
		return -1;
	}
	ev.target = w;
	ev.type = WET_FOCUS;
	ev.cancel_bubble = FALSE;
	self.targets[WST_FOCUS] = w;
	Widget_AddStatus( ev.target, "focus" );
	Widget_PostEvent( ev.target, &ev, NULL, NULL );
	return 0;
}

/** 响应系统的鼠标移动事件，向目标部件投递相关鼠标事件 */
static void OnMouseEvent( LCUI_SysEvent sys_ev, void *arg )
{
	LCUI_Pos pos;
	LCUI_Widget target, w;
	LCUI_WidgetEventRec ev = { 0 };
	w = LCUIWidget_GetRoot();
	LCUICursor_GetPos( &pos );
	if( self.mouse_capturer ) {
		target = self.mouse_capturer;
	} else {
		target = Widget_At( w, pos.x, pos.y );
		if( !target ) {
			Widget_UpdateStatus( NULL, WST_HOVER );
			return;
		}
	}
	for( w = target; w; w = w->parent ) {
		if( w->event_blocked ) {
			return;
		}
	}
	ev.target = target;
	ev.cancel_bubble = FALSE;
	switch( sys_ev->type ) {
	case LCUI_MOUSEDOWN:
		ev.type = WET_MOUSEDOWN;
		ev.button.x = pos.x;
		ev.button.y = pos.y;
		ev.button.button = sys_ev->button.button;
		Widget_PostEvent( target, &ev, NULL, NULL );
		self.click.interval = DBLCLICK_INTERVAL;
		if( ev.button.button == LCUIKEY_LEFTBUTTON &&
		    self.click.widget == target ) {
			int delta;
			delta = (int)LCUI_GetTimeDelta( self.click.time );
			self.click.interval = delta;
		} else if( ev.button.button == LCUIKEY_LEFTBUTTON &&
			   self.click.widget != target ) {
			self.click.x = pos.x;
			self.click.y = pos.y;
		}
		self.click.time = LCUI_GetTime();
		self.click.widget = target;
		Widget_UpdateStatus( target, WST_ACTIVE );
		LCUIWidget_SetFocus( target );
		break;
	case LCUI_MOUSEUP:
		ev.type = WET_MOUSEUP;
		ev.button.x = pos.x;
		ev.button.y = pos.y;
		ev.button.button = sys_ev->button.button;
		Widget_PostEvent( target, &ev, NULL, NULL );
		if( self.targets[WST_ACTIVE] != target ||
		    ev.button.button != LCUIKEY_LEFTBUTTON ) {
			self.click.x = 0;
			self.click.y = 0;
			self.click.time = 0;
			self.click.widget = NULL;
			Widget_UpdateStatus( NULL, WST_ACTIVE );
			break;
		}
		ev.type = WET_CLICK;
		Widget_PostEvent( target, &ev, NULL, NULL );
		Widget_UpdateStatus( NULL, WST_ACTIVE );
		if(self.click.widget != target ) {
			self.click.x = 0;
			self.click.y = 0;
			self.click.time = 0;
			self.click.widget = NULL;
			break;
		}
		if( self.click.interval < DBLCLICK_INTERVAL ) {
			ev.type = WET_DBLCLICK;
			self.click.x = 0;
			self.click.y = 0;
			self.click.time = 0;
			self.click.widget = NULL;
			Widget_PostEvent( target, &ev, NULL, NULL );
		}
		Widget_UpdateStatus( NULL, WST_ACTIVE );
		break;
	case LCUI_MOUSEMOVE:
		ev.type = WET_MOUSEMOVE;
		ev.motion.x = pos.x;
		ev.motion.y = pos.y;
		if( abs( self.click.x - pos.x ) >= 8 ||
		    abs( self.click.y - pos.y ) >= 8 ) {
			self.click.time = 0;
			self.click.widget = NULL;
		}
		Widget_PostEvent( target, &ev, NULL, NULL );
		break;
	case LCUI_MOUSEWHEEL:
		ev.type = WET_MOUSEWHEEL;
		ev.wheel.x = pos.x;
		ev.wheel.y = pos.y;
		ev.wheel.delta = sys_ev->wheel.delta;
		Widget_PostEvent( target, &ev, NULL, NULL );
	default:return;
	}
	Widget_UpdateStatus( target, WST_HOVER );
}

static void OnKeyboardEvent( LCUI_SysEvent e, void *arg )
{
	LCUI_WidgetEventRec ev = { 0 };
	if( !self.targets[WST_FOCUS] ) {
		return;
	}
	switch( e->type ) {
	case LCUI_KEYDOWN: ev.type = WET_KEYDOWN; break;
	case LCUI_KEYUP: ev.type = WET_KEYUP; break;
	case LCUI_KEYPRESS: ev.type = WET_KEYPRESS; break;
	default: return;
	}
	ev.target = self.targets[WST_FOCUS];
	ev.key.code = e->key.code;
	ev.cancel_bubble = FALSE;
	Widget_TriggerEvent( ev.target, &ev, NULL );
}

/** 响应输入法的输入 */
static void OnTextInput( LCUI_SysEvent e, void *arg )
{
	LCUI_WidgetEventRec ev = { 0 };
	LCUI_Widget target = self.targets[WST_FOCUS];
	if( !target ) {
		return;
	}
	ev.target = target;
	ev.type = WET_TEXTINPUT;
	ev.cancel_bubble = FALSE;
	ev.text.length = e->text.length;
	ev.text.text = NEW( wchar_t, e->text.length + 1 );
	if( !ev.text.text ) {
		return;
	}
	wcsncpy( ev.text.text, e->text.text, e->text.length + 1 );
	Widget_TriggerEvent( ev.target, &ev, NULL );
	free( ev.text.text );
	ev.text.text = NULL;
	ev.text.length = 0;
}

static void ConvertTouchPoint( LCUI_TouchPoint point )
{
	switch( point->state ) {
	case LCUI_TOUCHDOWN: point->state = WET_TOUCHDOWN; break;
	case LCUI_TOUCHUP: point->state = WET_TOUCHUP; break;
	case LCUI_TOUCHMOVE: point->state = WET_TOUCHMOVE; break;
	default:break;
	}
}

/** 分发触控事件给对应的部件 */
static int DispatchTouchEvent( LinkedList *capturers, 
			       LCUI_TouchPoint points, int n_points )
{
	int i, count;
	LCUI_WidgetEventRec ev = { 0 };
	LCUI_Widget target, root, w;
	LinkedListNode *node, *ptnode;

	ev.type = WET_TOUCH;
	ev.cancel_bubble = FALSE;
	root = LCUIWidget_GetRoot();
	ev.touch.points = NEW( LCUI_TouchPointRec, n_points );
	/* 先将各个触点按命中的部件进行分组 */
	for( i = 0; i < n_points; ++i ) {
		target = Widget_At( root, points[i].x, points[i].y );
		if( !target ) {
			continue;
		}
		for( w = target; w; w = w->parent ) {
			if( w->event_blocked ) {
				break;
			}
		}
		if( w && w->event_blocked ) {
			continue;
		}
		TouchCapturers_Add( capturers, target, points[i].id );
	}
	count = 0;
	ev.touch.n_points = 0;
	/* 然后向命中的部件发送触控事件 */
	for( LinkedList_Each( node, capturers ) ) {
		TouchCapturer tc = node->data;
		for( i = 0; i < n_points; ++i ) {
			for( LinkedList_Each( ptnode, &tc->points ) ) {
				LCUI_TouchPoint point;
				if( points[i].id != *(int*)ptnode->data ) {
					continue;
				}
				point = &ev.touch.points[ev.touch.n_points];
				*point = points[i];
				ConvertTouchPoint( point );
				++ev.touch.n_points;
			}
		}
		if( ev.touch.n_points == 0 ) {
			continue;
		}
		Widget_PostEvent( tc->widget, &ev, NULL, NULL );
		ev.touch.n_points = 0;
		++count;
	}
	free( ev.touch.points );
	return count;
}

/** 响应系统触控事件 */
static void OnTouch( LCUI_SysEvent sys_ev, void *arg )
{
	int i, n;
	LinkedList capturers;
	LCUI_TouchPoint points;
	LinkedListNode *node, *ptnode;

	n = sys_ev->touch.n_points;
	points = sys_ev->touch.points;
	LinkedList_Init( &capturers );
	LCUIMutex_Lock( &self.mutex );
	/* 合并现有的触点捕捉记录 */
	for( LinkedList_Each( node, &self.touch_capturers ) ) {
		TouchCapturer tc = node->data;
		for( i = 0; i < n; ++i ) {
			/* 如果没有触点记录，则说明是捕获全部触点 */
			if( tc->points.length == 0 ) {
				TouchCapturers_Add( &capturers, tc->widget, 
						    points[i].id );
				continue;
			}
			for( LinkedList_Each( ptnode, &tc->points ) ) {
				if( points[i].id != *(int*)ptnode->data ) {
					continue;
				}
				TouchCapturers_Add( &capturers, tc->widget,
						    points[i].id );
			}
		}
	}
	DispatchTouchEvent( &capturers, points, n );
	TouchCapturers_Clear( &capturers );
	LCUIMutex_Unlock( &self.mutex );
}

void Widget_SetMouseCapture( LCUI_Widget w )
{
	self.mouse_capturer = w;
}

void Widget_ReleaseMouseCapture( LCUI_Widget w )
{
	self.mouse_capturer = NULL;
}

int Widget_SetTouchCapture( LCUI_Widget w, int point_id )
{
	int ret;
	LCUIMutex_Lock( &self.mutex );
	ret = TouchCapturers_Add( &self.touch_capturers, w, point_id );
	LCUIMutex_Unlock( &self.mutex );
	return ret;
}

int Widget_ReleaseTouchCapture( LCUI_Widget w, int point_id )
{
	int ret;
	LCUIMutex_Lock( &self.mutex );
	ret = TouchCapturers_Delete( &self.touch_capturers, w, point_id );
	LCUIMutex_Unlock( &self.mutex );
	return ret;
}

int Widget_PostSurfaceEvent( LCUI_Widget w, int event_type )
{
	LCUI_Widget root;
	LCUI_WidgetEventRec e = { 0 };
	int *n = (int*)&event_type;
	root = LCUIWidget_GetRoot();
	if( w->parent != root && w != root ) {
		return -1;
	}
	e.target = w;
	e.type = WET_SURFACE;
	e.cancel_bubble = TRUE;
	return Widget_PostEvent( root, &e, *((int**)n), NULL );
}

static void BindSysEvent( int e, LCUI_SysEventFunc func )
{
	int *id = malloc( sizeof( int ) );
	*id = LCUI_BindEvent( e, func, NULL, NULL );
	LinkedList_Append( &self.events, id );
}

void LCUIWidget_InitEvent(void)
{
	int i, n;
	struct EventNameMapping {
		int id;
		const char *name;
	} mappings[] = {
		{ WET_READY, "ready" },
		{ WET_REMOVE, "remove" },
		{ WET_DESTROY, "destroy" },
		{ WET_MOUSEDOWN, "mousedown" },
		{ WET_MOUSEUP, "mouseup" },
		{ WET_MOUSEMOVE, "mousemove" },
		{ WET_MOUSEWHEEL, "mousewheel" },
		{ WET_CLICK, "click" },
		{ WET_DBLCLICK, "dblclick" },
		{ WET_MOUSEOUT, "mouseout" },
		{ WET_MOUSEOVER, "mouseover" },
		{ WET_KEYDOWN, "keydown" },
		{ WET_KEYUP, "keyup" },
		{ WET_KEYPRESS, "keypress" },
		{ WET_TOUCH, "touch" },
		{ WET_TEXTINPUT, "textinput" },
		{ WET_TOUCHDOWN, "touchdown" },
		{ WET_TOUCHMOVE, "touchmove" },
		{ WET_TOUCHUP, "touchup" },
		{ WET_RESIZE, "resize" },
		{ WET_AFTERLAYOUT, "afterlayout" },
		{ WET_FOCUS, "focus" },
		{ WET_BLUR, "blur" },
		{ WET_SHOW, "show" },
		{ WET_HIDE, "hide" },
		{ WET_SURFACE, "surface" },
		{ WET_TITLE, "title" }
	};
	LCUIMutex_Init( &self.mutex );
	RBTree_Init( &self.event_names );
	RBTree_Init( &self.event_records );
	LinkedList_Init( &self.events );
	LinkedList_Init( &self.event_mappings );
	self.targets[WST_ACTIVE] = NULL;
	self.targets[WST_HOVER] = NULL;
	self.targets[WST_FOCUS] = NULL;
	self.mouse_capturer = NULL;
	self.click.x = 0;
	self.click.y = 0;
	self.click.time = 0;
	self.click.widget= NULL;
	self.click.interval = DBLCLICK_INTERVAL;
	self.base_event_id = WET_USER + 1000;
	self.event_ids = Dict_Create( &DictType_StringKey, NULL );
	n = sizeof( mappings ) / sizeof( mappings[0] );
	for( i = 0; i < n; ++i ) {
		LCUIWidget_SetEventName( mappings[i].id, mappings[i].name );
	}
	BindSysEvent( LCUI_MOUSEWHEEL, OnMouseEvent );
	BindSysEvent( LCUI_MOUSEDOWN, OnMouseEvent );
	BindSysEvent( LCUI_MOUSEMOVE, OnMouseEvent );
	BindSysEvent( LCUI_MOUSEUP, OnMouseEvent );
	BindSysEvent( LCUI_KEYPRESS, OnKeyboardEvent );
	BindSysEvent( LCUI_KEYDOWN, OnKeyboardEvent );
	BindSysEvent( LCUI_KEYUP, OnKeyboardEvent );
	BindSysEvent( LCUI_TOUCH, OnTouch );
	BindSysEvent( LCUI_TEXTINPUT, OnTextInput );
	RBTree_OnCompare( &self.event_records, CompareWidgetEventRecord );
	LinkedList_Init( &self.touch_capturers );
}

void LCUIWidget_ExitEvent( void )
{
	LinkedListNode *node;
	LCUIMutex_Lock( &self.mutex );
	for( LinkedList_Each( node, &self.events ) ) {
		int *id = node->data;
		LCUI_UnbindEvent( *id );
	}
	RBTree_Destroy( &self.event_names );
	RBTree_Destroy( &self.event_records );
	Dict_Release( self.event_ids );
	TouchCapturers_Clear( &self.touch_capturers );
	LinkedList_Clear( &self.events, free );
	LinkedList_Clear( &self.event_mappings, DestroyEventMapping );
	LCUIMutex_Unlock( &self.mutex );
	LCUIMutex_Destroy( &self.mutex );
}
