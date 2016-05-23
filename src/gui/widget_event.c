/* ***************************************************************************
 * widget_event.c -- LCUI widget event module.
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>

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
	LCUI_BOOL is_direct_run;	/**< 是否直接处理该事件 */
} LCUI_WidgetEventPackRec, *LCUI_WidgetEventPack;

enum WidgetStatusType {
	WST_HOVER,
	WST_ACTIVE,
	WST_TOTAL
};

static struct LCUIWidgetEvnetModule {
	LCUI_Widget targets[WST_TOTAL];		/**< 相关的部件 */
	LCUI_RBTree event_names;		/**< 事件标识号 -> 名称映射表 */
	Dict *event_ids;			/**< 事件名称 -> 标识号映射表 */
	int base_event_id;			/**< 事件标识号计数器 */
} self;

static void DestroyWidgetEventHandler( void *arg )
{
	WidgetEventHandler handler = arg;
	if( handler->data && handler->destroy_data ) {
		handler->destroy_data( handler->data );
	}
	handler->data = NULL;
	free( handler );
}

/** 将原始事件转换成部件事件 */
static void WidgetEventTranslator( LCUI_Event e, LCUI_WidgetEventPack pack )
{
	WidgetEventHandler handler = e->data;
	LCUI_Widget w = pack->widget;
	pack->event.type = e->type;
	pack->event.data = handler->data;
	handler->func( w, &pack->event, pack->data );
	while( w && w->state != WSTATUS_DELETED ) {
		w = w->parent;
	}
	if( w && w->state == WSTATUS_DELETED ) {
		pack->event.cancel_bubble = TRUE;
	}
	w = pack->widget;
	if( pack->event.cancel_bubble || !w->parent ) {
		if( pack->data && pack->destroy_data ) {
			pack->destroy_data( pack->data );
		}
		return;
	}
	pack->event.x += w->box.border.x;
	pack->event.y += w->box.border.y;
	w = w->parent;
	pack->event.x += w->padding.left;
	pack->event.y += w->padding.top;
	pack->widget = w;
	/** 向父级部件冒泡传递事件 */
	EventTrigger_Trigger( w->trigger, e->type, pack );
}

/** 响应原始事件 */
static void OnWidgetEvent( LCUI_Event e, void *arg )
{
	LCUI_AppTaskRec task;
	LCUI_WidgetEventPack pack = arg;
	/* 如果需要直接执行 */
	if( pack->is_direct_run ) {
		WidgetEventTranslator( e, pack );
		return;
	}
	/* 准备任务 */
	task.func = (LCUI_AppTaskFunc)WidgetEventTranslator;
	task.arg[0] = malloc( sizeof( LCUI_EventRec ) );
	task.arg[1] = malloc( sizeof( LCUI_WidgetEventPackRec ) );
	/* 这两个参数都需要在任务执行完后释放 */
	task.destroy_arg[0] = free;
	task.destroy_arg[1] = free;
	/* 复制所需数据，因为在本函数退出后，这两个参数会被销毁 */
	*((LCUI_Event)task.arg[0]) = *e;
	*((LCUI_WidgetEventPack)task.arg[1]) = *pack;
	/* 把任务扔给当前跑主循环的线程 */
	LCUI_PostTask( &task );
}

int LCUIWidget_SetEventName( int event_id, const char *event_name )
{
	int *id;
	char *name;
	if( Dict_FetchValue( self.event_ids, event_name ) ) {
		return -1;
	}
	id = malloc( sizeof( int ) );
	name = strdup( event_name );
	*id = event_id;
	RBTree_Insert( &self.event_names, event_id, name );
	return Dict_Add( self.event_ids, name, id );
}

int LCUIWidget_AllocEventId( void )
{
	return self.base_event_id++;
}

const char *LCUIWidget_GetEventName( int event_id )
{
	return RBTree_GetData( &self.event_names, event_id );
}

int LCUIWidget_GetEventId( const char *event_name )
{
	int *val = Dict_FetchValue( self.event_ids, event_name );
	if( val ) {
		return *val;
	}
	return -1;
}

int Widget_BindEventById( LCUI_Widget widget, int event_id,
			  LCUI_WidgetEventFunc func, void *data,
			  void (*destroy_data)(void*) )
{
	WidgetEventHandler handler;
	handler = NEW(WidgetEventHandlerRec, 1);
	handler->func = func;
	handler->data = data;
	handler->destroy_data = destroy_data;
	DEBUG_MSG("event: %s, task: %p\n", event_name, task);
	return EventTrigger_Bind( widget->trigger, event_id, OnWidgetEvent,
				  handler, DestroyWidgetEventHandler);
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

static int Widget_TriggerEventEx( LCUI_Widget widget, LCUI_WidgetEvent e,
				  void *data, void (*destroy_data)(void*),
				  LCUI_BOOL direct_run )
{
	int ret;
	LCUI_Widget w;
	LCUI_WidgetEventPackRec pack;
	pack.event = *e;
	pack.data = data;
	pack.widget = widget;
	pack.is_direct_run = direct_run;
	pack.destroy_data = destroy_data;
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
		ret = EventTrigger_Trigger( widget->trigger, e->type, &pack );
		if( ret <= 0 ) {
			if( !widget->parent || e->cancel_bubble ) {
				return -1;
			}
			/* 如果事件投递失败，则向父级部件冒泡 */
			return Widget_PostEvent( widget->parent, e,
						 data, destroy_data );
		}
	}
	if( !widget->parent ) {
		return -1;
	}
	/* 转换成相对于父级部件内容框的坐标 */
	pack.event.x += widget->box.border.x;
	pack.event.y += widget->box.border.y;
	/* 从当前部件后面找到当前坐标点命中的兄弟部件 */
	w = Widget_GetNextAt( widget, pack.event.x, pack.event.y );
	/* 找不到就向父级部件冒泡 */
	if( !w ) {
		return Widget_PostEvent( widget->parent, e,
					 data, destroy_data );
	}
	pack.event.x -= widget->box.border.x;
	pack.event.y -= widget->box.border.y;
	return EventTrigger_Trigger( widget->trigger, e->type, &pack );
}

int Widget_PostEvent( LCUI_Widget widget, LCUI_WidgetEvent e, void *data,
		      void (*destroy_data)(void*))
{
	return Widget_TriggerEventEx( widget, e, data, destroy_data, FALSE );
}

int Widget_TriggerEvent( LCUI_Widget widget, LCUI_WidgetEvent e, void *data )
{
	return Widget_TriggerEventEx( widget, e, data, NULL, TRUE );
}

void LCUIWidget_ClearEventTarget( LCUI_Widget widget )
{
	int i;
	for( i = 0; i < WST_TOTAL; ++i ) {
		if( self.targets[i] == widget ) {
			self.targets[i] = NULL;
		}
	}
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
	for( w = old_w; w != root && w; w = w->parent ) {
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

/** 响应系统的鼠标移动事件，向目标部件投递相关鼠标事件 */
static void OnMouseEvent( LCUI_SysEvent e, void *arg )
{
	LCUI_Pos pos;
	LCUI_WidgetEventRec ebuff;
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
		Widget_PostEvent( target, &ebuff, NULL, NULL );
		Widget_UpdateStatus( target, WST_ACTIVE );
		break;
	case LCUI_MOUSEUP:
		ebuff.type = WET_MOUSEUP;
		Widget_PostEvent( target, &ebuff, NULL, NULL );
		if( self.targets[WST_ACTIVE] == target ) {
			ebuff.type = WET_CLICK;
			Widget_PostEvent( target, &ebuff, NULL, NULL );
		}
		Widget_UpdateStatus( NULL, WST_ACTIVE );
		break;
	case LCUI_MOUSEMOVE:
		ebuff.type = WET_MOUSEMOVE;
		Widget_PostEvent( target, &ebuff, NULL, NULL );
		break;
	case LCUI_MOUSEWHEEL:
		ebuff.type = WET_MOUSEWHEEL;
		ebuff.z_delta = e->z_delta;
		Widget_PostEvent( target, &ebuff, NULL, NULL );
	default:return;
	}
	Widget_UpdateStatus( target, WST_HOVER );
}

/** 响应按键的按下 */
static void OnKeyDown( LCUI_SysEvent e, void *arg )
{

}

/** 响应按键的释放 */
static void OnKeyUp( LCUI_SysEvent e, void *arg )
{

}

/** 响应按键的输入 */
static void OnKeyPress( LCUI_SysEvent e, void *arg )
{

}

/** 响应输入法的输入 */
static void OnInput( LCUI_SysEvent e, void *arg )
{

}

int Widget_PostSurfaceEvent( LCUI_Widget w, int event_type )
{
	LCUI_Widget root;
	LCUI_WidgetEventRec e;
	int *n = (int*)&event_type;
	root = LCUIWidget_GetRoot();
	if( w->parent != root && w != root ) {
		return -1;
	}
	e.target = w;
	e.type = WET_SURFACE;
	DEBUG_MSG("widget: %s, post event: %d\n", w->type, event_type );
	return Widget_PostEvent( root, &e, *((int**)n), NULL );
}

static unsigned int Dict_KeyHash( const void *key )
{
	const char *buf = key;
	unsigned int hash = 5381;
	while( *buf ) {
		hash = ((hash << 5) + hash) + (*buf++);
	}
	return hash;
}

static int Dict_KeyCompare( void *privdata, const void *key1, const void *key2 )
{
	if( strcmp( key1, key2 ) == 0 ) {
		return 1;
	}
	return 0;
}

static void *Dict_KeyDup( void *privdata, const void *key )
{
	char *newkey = malloc( (wcslen( key ) + 1)*sizeof( char ) );
	strcpy( newkey, key );
	return newkey;
}

static void *Dict_ValueDup( void *privdata, const void *val )
{
	int *newval = malloc( sizeof( int ) );
	*newval = *((int*)val);
	return newval;
}

static void Dict_KeyDestructor( void *privdata, void *key )
{
	free( key );
}

static void Dict_ValueDestructor( void *privdata, void *val )
{
	free( val );
}

static DictType DictType_String = {
	Dict_KeyHash,
	Dict_KeyDup,
	Dict_ValueDup,
	Dict_KeyCompare,
	Dict_KeyDestructor,
	Dict_ValueDestructor
};

void LCUIWidget_InitEvent(void)
{
	int i, n;
	struct EventNameMapping {
		int id;
		const char *name;
	} mappings[] = {
		{ WET_READY, "ready" },
		{ WET_DESTROY, "destroy" },
		{ WET_MOUSEDOWN, "mousedown" },
		{ WET_MOUSEUP, "mouseup" },
		{ WET_MOUSEMOVE, "mousemove" },
		{ WET_MOUSEWHEEL, "mousewheel" },
		{ WET_CLICK, "click" },
		{ WET_MOUSEOUT, "mouseout" },
		{ WET_MOUSEOVER, "mouseover" },
		{ WET_RESIZE, "resize" },
		{ WET_AFTERLAYOUT, "afterlayout" },
		{ WET_FOCUS, "focus" },
		{ WET_BLUR, "blur" },
		{ WET_SHOW, "show" },
		{ WET_HIDE, "hide" },
		{ WET_SURFACE, "surface" },
		{ WET_TITLE, "title" }
	};
	RBTree_Init( &self.event_names );
	self.targets[WST_ACTIVE] = NULL;
	self.targets[WST_HOVER] = NULL;
	self.base_event_id = WET_USER + 1000;
	self.event_ids = Dict_Create( &DictType_String, NULL );
	n = sizeof( mappings ) / sizeof( mappings[0] );
	for( i = 0; i < n; ++i ) {
		LCUIWidget_SetEventName( mappings[i].id, mappings[i].name );
	}
	LCUI_BindEvent( LCUI_MOUSEDOWN, OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( LCUI_MOUSEMOVE, OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( LCUI_MOUSEUP, OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( LCUI_MOUSEWHEEL, OnMouseEvent, NULL, NULL );
	LCUI_BindEvent( LCUI_KEYUP, OnKeyUp, NULL, NULL );
	LCUI_BindEvent( LCUI_KEYDOWN, OnKeyDown, NULL, NULL );
	LCUI_BindEvent( LCUI_KEYPRESS, OnKeyPress, NULL, NULL );
	LCUI_BindEvent( LCUI_INPUT, OnInput, NULL, NULL );
}

void LCUIWidget_ExitEvent(void)
{
	RBTree_Destroy( &self.event_names );
	Dict_Release( self.event_ids );
}
