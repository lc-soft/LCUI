/* ***************************************************************************
 * widget_base.c -- the widget base operation set.
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
 * ****************************************************************************/

/* ****************************************************************************
 * widget_base.c -- 部件的基本操作集。
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
 * ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>

static struct LCUIWidgetModule {
	LCUI_Widget root;		/**< 根级部件 */
	Dict *ids;			/**< 各种部件的ID索引 */
	LCUI_Mutex mutex;		/**< 互斥锁 */
	DictType dt_ids;		/**< 部件ID映射表的类型模板 */
	DictType dt_attributes;		/**< 部件属性表的类型模板 */
} LCUIWidget;

#define StrList_Destroy freestrs

static inline float ToBorderBoxWidth( LCUI_Widget w, float content_width )
{
	return content_width + w->padding.left + w->padding.right
		+ w->computed_style.border.left.width
		+ w->computed_style.border.right.width;
}

static inline float ToBorderBoxHeight( LCUI_Widget w, float content_height )
{
	return content_height + w->padding.top + w->padding.bottom
		+ w->computed_style.border.top.width
		+ w->computed_style.border.bottom.width;
}

static inline float ToContentBoxWidth( LCUI_Widget w, float width )
{
	return width - w->padding.left - w->padding.right
		- w->computed_style.border.left.width
		- w->computed_style.border.right.width;
}

static inline float ToContentBoxHeight( LCUI_Widget w, float height )
{
	return height - w->padding.top - w->padding.bottom
		- w->computed_style.border.top.width
		- w->computed_style.border.bottom.width;
}

LCUI_Widget LCUIWidget_GetRoot( void )
{
	return LCUIWidget.root;
}

/** 刷新部件的状态 */
static void Widget_UpdateStatus( LCUI_Widget widget )
{
	LCUI_Widget child;
	if( !widget->parent ) {
		return;
	}
	if( widget->index == widget->parent->children.length - 1 ) {
		Widget_AddStatus( widget, "last-child" );
		child = Widget_GetPrev( widget );
		if( child ) {
			Widget_RemoveStatus( child, "last-child" );
		}
	}
	if( widget->index == 0 ) {
		Widget_AddStatus( widget, "first-child" );
		child = Widget_GetNext( widget );
		if( child ) {
			Widget_RemoveStatus( child, "first-child" );
		}
	}
}

int Widget_Unlink( LCUI_Widget widget )
{
	LCUI_Widget child;
	LinkedListNode *node, *snode;
	if( !widget->parent ) {
		return -1;
	}
	node = &widget->node;
	snode = &widget->node_show;
	if( widget->index == widget->parent->children.length - 1 ) {
		Widget_RemoveStatus( widget, "last-child" );
		child = Widget_GetPrev( widget );
		if( child ) {
			Widget_AddStatus( child, "last-child" );
		}
	}
	if( widget->index == 0 ) {
		Widget_RemoveStatus( widget, "first-child" );
		child = Widget_GetNext( widget );
		if( child ) {
			Widget_AddStatus( child, "first-child" );
		}
	}
	/** 修改它后面的部件的 index 值 */
	node = node->next;
	while( node ) {
		child = node->data;
		child->index -= 1;
		node = node->next;
	}
	node = &widget->node;
	LinkedList_Unlink( &widget->parent->children, node );
	LinkedList_Unlink( &widget->parent->children_show, snode );
	Widget_PostSurfaceEvent( widget, WET_REMOVE, TRUE );
	widget->parent = NULL;
	return 0;
}

int Widget_Append( LCUI_Widget parent, LCUI_Widget widget )
{
	LCUI_Widget child;
	LinkedListNode *node, *snode;
	if( !parent || !widget ) {
		return -1;
	}
	if( parent == widget ) {
		return -2;
	}
	Widget_Unlink( widget );
	widget->parent = parent;
	widget->state = WSTATE_CREATED;
	widget->index = parent->children.length;
	node = &widget->node;
	snode = &widget->node_show;
	LinkedList_AppendNode( &parent->children, node );
	LinkedList_AppendNode( &parent->children_show, snode );
	/** 修改它后面的部件的 index 值 */
	node = node->next;
	while( node ) {
		child = node->data;
		child->index += 1;
		node = node->next;
	}
	Widget_UpdateStyle( widget, TRUE );
	Widget_UpdateChildrenStyle( widget, TRUE );
	Widget_PostSurfaceEvent( widget, WET_ADD, TRUE );
	Widget_UpdateTaskStatus( widget );
	Widget_UpdateStatus( widget );
	Widget_UpdateLayout( parent );
	return 0;
}

int Widget_Prepend( LCUI_Widget parent, LCUI_Widget widget )
{
	LCUI_Widget child;
	LinkedListNode *node, *snode;
	if( !parent || !widget ) {
		return -1;
	}
	if( parent == widget ) {
		return -2;
	}
	child = widget->parent;
	Widget_Unlink( widget );
	widget->index = 0;
	widget->parent = parent;
	widget->state = WSTATE_CREATED;
	node = &widget->node;
	snode = &widget->node_show;
	LinkedList_InsertNode( &parent->children, 0, node );
	LinkedList_InsertNode( &parent->children_show, 0, snode );
	/** 修改它后面的部件的 index 值 */
	node = node->next;
	while( node ) {
		child = node->data;
		child->index += 1;
		node = node->next;
	}
	Widget_PostSurfaceEvent( widget, WET_ADD, TRUE );
	Widget_AddTaskForChildren( widget, WTT_REFRESH_STYLE );
	Widget_UpdateTaskStatus( widget );
	Widget_UpdateStatus( widget );
	Widget_UpdateLayout( parent );
	return 0;
}

int Widget_Unwrap( LCUI_Widget widget )
{
	int i;
	LCUI_Widget child;
	LinkedList *list, *list_show;
	LinkedListNode *target, *node, *prev, *snode;

	if( !widget->parent ) {
		return -1;
	}
	list = &widget->parent->children;
	list_show = &widget->parent->children_show;
	if( widget->children.length > 0 ) {
		node = LinkedList_GetNode( &widget->children, 0 );
		Widget_RemoveStatus( node->data, "first-child" );
		node = LinkedList_GetNodeAtTail( &widget->children, 0 );
		Widget_RemoveStatus( node->data, "last-child" );
	}
	node = &widget->node;
	i = widget->children.length;
	target = node->prev;
	node = widget->children.tail.prev;
	while( i-- > 0 ) {
		prev = node->prev;
		child = node->data;
		snode = &child->node_show;
		LinkedList_Unlink( &widget->children, node );
		LinkedList_Unlink( &widget->children_show, snode );
		child->parent = widget->parent;
		LinkedList_Link( list, target, node );
		LinkedList_AppendNode( list_show, snode );
		Widget_AddTaskForChildren( child, WTT_REFRESH_STYLE );
		Widget_UpdateTaskStatus( child );
		node = prev;
	}
	if( widget->index == 0 ) {
		Widget_AddStatus( target->next->data, "first-child" );
	}
	if( widget->index == list->length - 1 ) {
		node = LinkedList_GetNodeAtTail( list, 0 );
		Widget_AddStatus( node->data, "last-child" );
	}
	Widget_Destroy( widget );
	return 0;
}

/** 构造函数 */
static void Widget_Init( LCUI_Widget widget )
{
	ZEROSET( widget, LCUI_Widget );
	widget->state = WSTATE_CREATED;
	widget->trigger = EventTrigger();
	widget->style = StyleSheet();
	widget->custom_style = StyleSheet();
	widget->inherited_style = StyleSheet();
	widget->computed_style.opacity = 1.0;
	widget->computed_style.visible = TRUE;
	widget->computed_style.focusable = FALSE;
	widget->computed_style.display = SV_BLOCK;
	widget->computed_style.position = SV_STATIC;
	widget->computed_style.pointer_events = SV_AUTO;
	widget->computed_style.box_sizing = SV_CONTENT_BOX;
	widget->computed_style.margin.top.type = SVT_PX;
	widget->computed_style.margin.right.type = SVT_PX;
	widget->computed_style.margin.bottom.type = SVT_PX;
	widget->computed_style.margin.left.type = SVT_PX;
	widget->computed_style.padding.top.type = SVT_PX;
	widget->computed_style.padding.right.type = SVT_PX;
	widget->computed_style.padding.bottom.type = SVT_PX;
	widget->computed_style.padding.left.type = SVT_PX;
	Widget_InitBackground( widget );
	LinkedList_Init( &widget->children );
	LinkedList_Init( &widget->children_show );
}

LCUI_Widget LCUIWidget_New( const char *type )
{
	ASSIGN( widget, LCUI_Widget );
	Widget_Init( widget );
	widget->node.data = widget;
	widget->node_show.data = widget;
	widget->node.next = widget->node.prev = NULL;
	widget->node_show.next = widget->node_show.prev = NULL;
	if( type ) {
		widget->proto = LCUIWidget_GetPrototype( type );
		if( widget->proto ) {
			widget->type = widget->proto->name;
			widget->proto->init( widget );
		} else {
			widget->type = strdup2( type );
		}
	}
	Widget_AddTask( widget, WTT_REFRESH_STYLE );
	return widget;
}

static void Widget_OnDestroy( void *arg )
{
	Widget_ExecDestroy( arg );
}

void Widget_ExecDestroy( LCUI_Widget widget )
{
	LCUI_WidgetEventRec e = { WET_DESTROY, 0 };
	Widget_TriggerEvent( widget, &e, NULL );
	Widget_ReleaseMouseCapture( widget );
	Widget_ReleaseTouchCapture( widget, -1 );
	Widget_StopEventPropagation( widget );
	Widget_DestroyBackground( widget );
	LCUIWidget_ClearEventTarget( widget );
	/* 先释放显示列表，后销毁部件列表，因为部件在这两个链表中的节点是和它共用
	 * 一块内存空间的，销毁部件列表会把部件释放掉，所以把这个操作放在后面 */
	LinkedList_ClearData( &widget->children_show, NULL );
	LinkedList_ClearData( &widget->children, Widget_OnDestroy );
	Widget_ClearPrototype( widget );
	Widget_SetId( widget, NULL );
	StyleSheet_Delete( widget->inherited_style );
	StyleSheet_Delete( widget->custom_style );
	StyleSheet_Delete( widget->style );
	if( widget->parent ) {
		Widget_UpdateLayout( widget->parent );
	}
	if( widget->title ) {
		free( widget->title );
		widget->title = NULL;
	}
	widget->attributes ? Dict_Release( widget->attributes ) : 0;
	widget->classes ? StrList_Destroy( widget->classes ) : 0;
	widget->status ? StrList_Destroy( widget->status ) : 0;
	EventTrigger_Destroy( widget->trigger );
	widget->trigger = NULL;
	free( widget );
}

void Widget_Destroy( LCUI_Widget w )
{
	LCUI_Widget root = w;
	while( root->parent ) {
		root = root->parent;
	}
	if( root != LCUIWidget.root ) {
		LCUI_WidgetEventRec e = { 0 };
		e.type = WET_REMOVE;
		w->state = WSTATE_DELETED;
		Widget_TriggerEvent( w, &e, NULL );
		Widget_ExecDestroy( w );
		return;
	}
	if( w->parent ) {
		LCUI_Widget child;
		LinkedListNode *node;
		node = &w->node;
		node = node->next;
		while( node ) {
			child = node->data;
			child->index -= 1;
			node = node->next;
		}
		if( w->computed_style.position != SV_ABSOLUTE ) {
			Widget_UpdateLayout( w->parent );
		}
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
		Widget_AddToTrash( w );
	}
}

void Widget_Empty( LCUI_Widget w )
{
	LCUI_Widget root = w;

	while( root->parent ) {
		root = root->parent;
	}
	if( root == LCUIWidget.root ) {
		LinkedListNode *next, *node;
		node = w->children.head.next;
		while( node ) {
			next = node->next;
			Widget_AddToTrash( node->data );
			node = next;
		}
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
		Widget_AddTask( w, WTT_LAYOUT );
	} else {
		LinkedList_ClearData( &w->children_show, NULL );
		LinkedList_ClearData( &w->children, Widget_OnDestroy );
	}
}

LCUI_Widget Widget_At( LCUI_Widget widget, int ix, int iy )
{
	float x, y;
	LCUI_BOOL is_hit;
	LinkedListNode *node;
	LCUI_Widget target = widget, c = NULL;
	if( !widget ) {
		return NULL;
	}
	x = 1.0f * ix;
	y = 1.0f * iy;
	do {
		is_hit = FALSE;
		for( LinkedList_Each( node, &target->children_show ) ) {
			c = node->data;
			if( !c->computed_style.visible ) {
				continue;
			}
			ix = iround( x );
			iy = iround( y );
			if( LCUIRect_HasPoint(&c->box.border, ix, iy) ) {
				target = c;
				x -= c->box.padding.x;
				y -= c->box.padding.y;
				is_hit = TRUE;
				break;
			}
		}
	} while( is_hit );
	return (target == widget) ? NULL:target;
}

void Widget_GetOffset( LCUI_Widget w, LCUI_Widget parent,
		       float *offset_x, float *offset_y )
{
	float x = 0, y = 0;
	while( w && w != parent ) {
		x += w->box.border.x;
		y += w->box.border.y;
		w = w->parent;
	}
	*offset_x = x;
	*offset_y = y;
}

LCUI_Widget LCUIWidget_GetById( const char *id )
{
	LinkedList *list;
	LCUI_Widget w = NULL;

	if( !id ) {
		return NULL;
	}
	LCUIMutex_Lock( &LCUIWidget.mutex );
	list = Dict_FetchValue( LCUIWidget.ids, id );
	if( list ) {
		w = LinkedList_Get( list, 0 );
	}
	LCUIMutex_Unlock( &LCUIWidget.mutex );
	return w;
}

LCUI_Widget Widget_GetPrev( LCUI_Widget w )
{
	LinkedListNode *node = &w->node;
	if( node->prev && node != w->parent->children.head.next ) {
		return node->prev->data;
	}
	return NULL;
}

LCUI_Widget Widget_GetNext( LCUI_Widget w )
{
	LinkedListNode *node = &w->node;
	if( node->next ) {
		return node->next->data;
	}
	return NULL;
}

LCUI_Widget Widget_GetChild( LCUI_Widget w, size_t index )
{
	LinkedListNode *node = LinkedList_GetNode( &w->children, index );
	if( node ) {
		return node->data;
	}
	return NULL;
}

int Widget_Top( LCUI_Widget w )
{
	return Widget_Append( LCUIWidget.root, w );
}

void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title )
{
	int len;
	wchar_t *new_title, *old_title;

	len = wcslen(title) + 1;
	new_title = (wchar_t*)malloc(sizeof(wchar_t)*len);
	if( !new_title ) {
		return;
	}
	wcsncpy( new_title, title, len );
	old_title = w->title;
	w->title = new_title;
	if( old_title ) {
		free( old_title );
	}
	Widget_AddTask( w, WTT_TITLE );
}

static int Widget_RemoveId( LCUI_Widget w )
{
	LinkedList *list;
	LinkedListNode *node;

	if( !w->id ) {
		return -1;
	}
	list = Dict_FetchValue( LCUIWidget.ids, w->id );
	if( !list ) {
		return -2;
	}
	for( LinkedList_Each( node, list ) ) {
		if( node->data == w ) {
			w->id = NULL;
			LinkedList_Unlink( list, node );
			LinkedListNode_Delete( node );
			return 0;
		}
	}
	return -3;
}

int Widget_SetId( LCUI_Widget w, const char *idstr )
{
	LinkedList *list;
	LCUIMutex_Lock( &LCUIWidget.mutex );
	Widget_RemoveId( w );
	if( !idstr ) {
		LCUIMutex_Unlock( &LCUIWidget.mutex );
		return -1;
	}
	w->id = strdup2( idstr );
	if( !w->id ) {
		goto error_exit;
	}
	list = Dict_FetchValue( LCUIWidget.ids, w->id );
	if( !list ) {
		list = malloc( sizeof( LinkedList ) );
		if( !list ) {
			goto error_exit;
		}
		LinkedList_Init( list );
		if( Dict_Add( LCUIWidget.ids, w->id, list ) != 0 ) {
			free( list );
			goto error_exit;
		}
	}
	if( !LinkedList_Append( list, w ) ) {
		goto error_exit;
	}
	LCUIMutex_Unlock( &LCUIWidget.mutex );
	return 0;

error_exit:
	LCUIMutex_Unlock( &LCUIWidget.mutex );
	if( w->id ) {
		free( w->id );
		w->id = NULL;
	}
	return -2;
}

void Widget_AddState( LCUI_Widget w, LCUI_WidgetState state )
{
	/* 如果部件还处于未准备完毕的状态 */
	if( w->state < WSTATE_READY ) {
		w->state |= state;
		/* 如果部件已经准备完毕则触发 ready 事件 */
		if( w->state == WSTATE_READY ) {
			LCUI_WidgetEventRec e = { 0 };
			e.type = WET_READY;
			e.cancel_bubble = TRUE;
			Widget_TriggerEvent( w, &e, NULL );
			w->state = WSTATE_NORMAL;
		}
	}
}

static float ComputeXMetric( LCUI_Widget w, int key )
{
	LCUI_Style s = &w->style->sheet[key];
	if( s->type == SVT_SCALE ) {
		if( !w->parent ) {
			return 0;
		}
		if( Widget_HasAbsolutePosition( w ) ) {
			return w->parent->box.padding.width * s->scale;
		}
		return w->parent->box.content.width * s->scale;
	}
	return LCUIMetrics_Compute( s->value, s->type );
}

static float ComputeYMetric( LCUI_Widget w, int key )
{
	LCUI_Style s = &w->style->sheet[key];
	if( s->type == SVT_SCALE ) {
		if( !w->parent ) {
			return 0;
		}
		if( Widget_HasAbsolutePosition( w ) ) {
			return w->parent->box.padding.height * s->scale;
		}
		return w->parent->box.content.height * s->scale;
	}
	return LCUIMetrics_Compute( s->value, s->type );
}

static int ComputeStyleOption( LCUI_Widget w, int key, int default_value )
{
	if( !w->style->sheet[key].is_valid ) {
		return default_value;
	}
	if( w->style->sheet[key].type != SVT_STYLE ) {
		return default_value;
	}
	return w->style->sheet[key].style;
}

void Widget_UpdateVisibility( LCUI_Widget w )
{
	LCUI_Style s = &w->style->sheet[key_visible];
	LCUI_BOOL visible = w->computed_style.visible;
	if( w->computed_style.display == SV_NONE ) {
		w->computed_style.visible = FALSE;
	} else if( s->is_valid && s->type == SVT_BOOL ) {
		w->computed_style.visible = s->val_bool;
	} else {
		w->computed_style.visible = TRUE;
	}
	if( visible == w->computed_style.visible ) {
		return;
	}
	if( w->parent ) {
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
	}
	visible = w->computed_style.visible;
	DEBUG_MSG( "visible: %s\n", visible ? "TRUE" : "FALSE" );
	Widget_PostSurfaceEvent( w, visible ? WET_SHOW : WET_HIDE, TRUE );
}

void Widget_UpdateDisplay( LCUI_Widget w )
{
	int display = w->computed_style.display;
	LCUI_Style s = &w->style->sheet[key_display];
	if( s->is_valid && s->type == SVT_STYLE ) {
		w->computed_style.display = s->style;
		if( w->computed_style.display == SV_NONE ) {
			w->computed_style.visible = FALSE;
		}
	} else {
		w->computed_style.display = SV_BLOCK;
	}
	if( w->computed_style.display == display ) {
		return;
	}
	if( w->parent && display == SV_NONE &&
	    w->computed_style.position != SV_ABSOLUTE ) {
		Widget_UpdateLayout( w->parent );
	}
	Widget_UpdateVisibility( w );
	Widget_UpdateLayout( w );
}

void Widget_UpdateOpacity( LCUI_Widget w )
{
	float opacity = 1.0;
	LCUI_Style s = &w->style->sheet[key_opacity];
	if( s->is_valid ) {
		switch( s->type ) {
		case SVT_VALUE: opacity = 1.0f * s->value; break;
		case SVT_SCALE: opacity = s->val_scale; break;
		default: opacity = 1.0f; break;
		}
		if( opacity > 1.0 ) {
			opacity = 1.0;
		} else if( opacity < 0.0 ) {
			opacity = 0.0;
		}
	}
	w->computed_style.opacity = opacity;
	Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
	DEBUG_MSG("opacity: %0.2f\n", opacity);
}

void Widget_UpdateZIndex( LCUI_Widget w )
{
	Widget_AddTask( w, WTT_ZINDEX );
}

void Widget_ExecUpdateZIndex( LCUI_Widget w )
{
	int z_index;
	LinkedList *list;
	LinkedListNode *cnode, *csnode, *snode;
	LCUI_Style s = &w->style->sheet[key_z_index];
	if( s->is_valid && s->type == SVT_VALUE ) {
		z_index = s->val_int;
	} else {
		z_index = 0;
	}
	if( !w->parent ) {
		return;
	}
	if( w->state == WSTATE_NORMAL ) {
		if( w->computed_style.z_index == z_index ) {
			return;
		}
	}
	w->computed_style.z_index = z_index;
	snode = &w->node_show;
	list = &w->parent->children_show;
	LinkedList_Unlink( list, snode );
	for( LinkedList_Each( cnode, list ) ) {
		LCUI_Widget child = cnode->data;
		LCUI_WidgetStyle *ccs = &child->computed_style;

		csnode = &child->node_show;
		if( w->computed_style.z_index < ccs->z_index ) {
			continue;
		} else if( w->computed_style.z_index == ccs->z_index ) {
			if( w->computed_style.position == ccs->position ) {
				if( w->index < child->index ) {
					continue;
				}
			} else if( w->computed_style.position < ccs->position ) {
				continue;
			}
		}
		LinkedList_Link( list, csnode->prev, snode );
		break;
	}
	if( !cnode ) {
		LinkedList_AppendNode( list, snode );
	}
	if( w->computed_style.position != SV_STATIC ) {
		Widget_AddTask( w, WTT_REFRESH );
	}
}

/** 清除已计算的尺寸 */
static void Widget_ClearComputedSize( LCUI_Widget w )
{
	if( Widget_HasAutoStyle( w, key_width ) ) {
		w->width = 0;
		w->box.graph.width = 0;
		w->box.content.width = 0;
	}
	if( Widget_HasAutoStyle( w, key_height ) ) {
		w->height = 0;
		w->box.graph.height = 0;
		w->box.content.height = 0;
	}
}

static void Widget_UpdateChildrenSize( LCUI_Widget w )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &w->children ) ) {
		LCUI_Widget child = node->data;
		LCUI_StyleSheet s = child->style;
		if( Widget_HasFillAvailableWidth( child ) ) {
			Widget_AddTask( child, WTT_RESIZE );
		} else if( Widget_HasScaleSize( child ) ) {
			Widget_AddTask( child, WTT_RESIZE );
		}
		if( Widget_HasAbsolutePosition( child ) ) {
			if( s->sheet[key_right].is_valid ||
			    s->sheet[key_bottom].is_valid ||
			    CheckStyleType( s, key_left, scale ) ||
			    CheckStyleType( s, key_top, scale ) ) {
				Widget_AddTask( child, WTT_POSITION );
			}
		}
		if( CheckStyleValue( s, key_margin_left, AUTO ) ||
		    CheckStyleValue( s, key_margin_right, AUTO ) ) {
			Widget_AddTask( child, WTT_MARGIN );
		}
		if( child->computed_style.vertical_align != SV_TOP ) {
			Widget_AddTask( child, WTT_POSITION );
		}
	}
}

void Widget_UpdatePosition( LCUI_Widget w )
{
	LCUI_RectF rect;
	int position = ComputeStyleOption( w, key_position, SV_STATIC );
	int valign = ComputeStyleOption( w, key_vertical_align, SV_TOP );
	w->computed_style.vertical_align = valign;
	w->computed_style.left = ComputeXMetric( w, key_left );
	w->computed_style.right = ComputeXMetric( w, key_right );
	w->computed_style.top = ComputeYMetric( w, key_top );
	w->computed_style.bottom = ComputeYMetric( w, key_bottom );
	if( w->parent && w->computed_style.position != position ) {
		w->computed_style.position = position;
		Widget_UpdateLayout( w->parent );
		Widget_ClearComputedSize( w );
		Widget_UpdateChildrenSize( w );
		/* 当部件尺寸是按百分比动态计算的时候需要重新计算尺寸 */
		if( Widget_CheckStyleType( w, key_width, scale ) ||
		    Widget_CheckStyleType( w, key_height, scale ) ) {
			Widget_UpdateSize( w );
		}
	}
	w->computed_style.position = position;
	Widget_UpdateZIndex( w );
	rect = w->box.graph;
	w->x = w->origin_x;
	w->y = w->origin_y;
	switch( position ) {
	case SV_ABSOLUTE:
		w->x = w->y = 0;
		if( Widget_CheckStyleValid( w, key_left ) ) {
			w->x = w->computed_style.left;
		} else if( Widget_CheckStyleValid( w, key_right ) ) {
			if( w->parent ) {
				w->x = w->parent->box.border.width;
				w->x -= w->width;
			}
			w->x -= w->computed_style.right;
		}
		if( Widget_CheckStyleValid( w, key_top ) ) {
			w->y = w->computed_style.top;
		} else if( Widget_CheckStyleValid( w, key_bottom ) ) {
			if( w->parent ) {
				w->y = w->parent->box.border.height;
				w->y -= w->height;
			}
			w->y -= w->computed_style.bottom;
		}
		break;
	case SV_RELATIVE:
		if( Widget_CheckStyleValid( w, key_left ) ) {
			w->x += w->computed_style.left;
		} else if( Widget_CheckStyleValid( w, key_right ) ) {
			w->x -= w->computed_style.right;
		}
		if( Widget_CheckStyleValid( w, key_top ) ) {
			w->y += w->computed_style.top;
		} else if( Widget_CheckStyleValid( w, key_bottom ) ) {
			w->y -= w->computed_style.bottom;
		}
	default:
		if( w->parent ) {
			w->x += w->parent->padding.left;
			w->y += w->parent->padding.top;
		}
		break;
	}
	switch( valign ) {
	case SV_MIDDLE:
		if( !w->parent ) {
			break;
		}
		w->y += (w->parent->box.content.height - w->height) / 2;
		break;
	case SV_BOTTOM:
		if( !w->parent ) {
			break;
		}
		w->y += w->parent->box.content.height - w->height;
	case SV_TOP:
	default: break;
	}
	w->box.outer.x = w->x;
	w->box.outer.y = w->y;
	w->x += w->margin.left;
	w->y += w->margin.top;
	/* 以x、y为基础 */
	w->box.padding.x = w->x;
	w->box.padding.y = w->y;
	w->box.border.x = w->x;
	w->box.border.y = w->y;
	w->box.graph.x = w->x;
	w->box.graph.y = w->y;
	/* 计算各个框的坐标 */
	w->box.padding.x += w->computed_style.border.left.width;
	w->box.padding.y += w->computed_style.border.top.width;
	w->box.content.x = w->box.padding.x + w->padding.left;
	w->box.content.y = w->box.padding.y + w->padding.top;
	w->box.graph.x -= Widget_GetBoxShadowOffsetX( w );
	w->box.graph.y -= Widget_GetBoxShadowOffsetY( w );
	if( w->parent ) {
		/* 标记移动前后的区域 */
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
		Widget_InvalidateArea( w->parent, &rect, SV_PADDING_BOX );
	}
	/* 检测是否为顶级部件并做相应处理 */
	Widget_PostSurfaceEvent( w, WET_MOVE, TRUE );
}

/** 更新位图尺寸 */
static void Widget_UpdateGraphBox( LCUI_Widget w )
{
	LCUI_RectF *rg = &w->box.graph;
	rg->x = w->x - Widget_GetBoxShadowOffsetX( w );
	rg->y = w->y - Widget_GetBoxShadowOffsetY( w );
	rg->width = Widget_GetGraphWidth( w );
	rg->height = Widget_GetGraphHeight( w );
}

/** 计算合适的内容框大小 */
static void Widget_ComputeContentSize( LCUI_Widget w,
				       float *out_width, float *out_height )
{
	LinkedListNode *node;
	float width = 0, height = 0, value;

	for( LinkedList_Each( node, &w->children_show ) ) {
		LCUI_Widget child = node->data;
		LCUI_WidgetBoxRect *box = &child->box;
		LCUI_WidgetStyle *style = &child->computed_style;
		/* 忽略不可见、绝对定位的部件 */
		if( style->display == SV_NONE || 
		    style->position == SV_ABSOLUTE ) {
			continue;
		}
		/* 对于宽度以百分比做单位的，计算尺寸时自动去除外间距框、
		 * 内间距框和边框占用的空间
		 */
		if( Widget_CheckStyleType( child, key_width, scale ) ) {
			if( style->box_sizing == SV_BORDER_BOX ) {
				value = box->border.x + box->border.width;
			} else {
				value = box->content.x + box->content.width;
				value -= box->content.x - box->border.x;
			}
			value -= box->outer.x - box->border.x;
		} else if( box->outer.width <= 0 ) {
			continue;
		} else {
			value = box->outer.x + box->outer.width;
		}
		if( value > width ) {
			width = value;
		}
		if( Widget_CheckStyleType( child, key_height, scale ) ) {
			if( style->box_sizing == SV_BORDER_BOX ) {
				value = box->border.y + box->border.height;
			} else {
				value = box->content.y + box->content.height;
				value -= box->content.y - box->border.y;
			}
			value -= box->outer.y - box->border.y;
		} else if( box->outer.height <= 0 ) {
			continue;
		} else {
			value = box->outer.y + box->outer.height;
		}
		if( value > height ) {
			height = value;
		}
	}
	/* 计算出来的尺寸是包含 padding-left 和 padding-top 的，因此需要减去它们 */
	width -= w->padding.left;
	height -= w->padding.top;
	if( out_width && *out_width <= 0 ) {
		*out_width = width;
	}
	if( out_height && *out_height <= 0 ) {
		*out_height = height;
	}
}

LCUI_BOOL Widget_HasAutoStyle( LCUI_Widget w, int key )
{
	return !Widget_CheckStyleValid( w, key ) ||
		Widget_CheckStyleType( w, key, AUTO );
}

LCUI_BOOL Widget_HasStaticWidthParent( LCUI_Widget widget )
{
	LCUI_Widget w;
	for( w = widget->parent; w; w = w->parent ) {
		if( w->computed_style.max_width >= 0 ) {
			return TRUE;
		}
		if( !Widget_HasAutoStyle( w, key_width ) ) {
			return TRUE;
		}
		if( Widget_HasAbsolutePosition( w ) ||
		    Widget_HasInlineBlockDisplay( w ) ) {
			return FALSE;
		}
	}
	return FALSE;
}

LCUI_BOOL Widget_HasFitContentWidth( LCUI_Widget w )
{
	if( !Widget_HasAutoStyle( w, key_width ) ) {
		return FALSE;
	}
	if( Widget_HasInlineBlockDisplay( w ) ) {
		return TRUE;
	}
	if( Widget_HasAbsolutePosition( w ) ||
	    !Widget_HasStaticWidthParent( w ) ) {
		return TRUE;
	}
	return FALSE;
}

/** 根据当前部件的内外间距，获取调整后宽度 */
static float Widget_GetAdjustedWidth( LCUI_Widget w, float width )
{
	/* 如果部件的宽度不具备自动填满剩余空间的特性，则不调整 */
	if( !Widget_HasFillAvailableWidth( w ) ) {
		return width;
	}
	if( !Widget_HasAutoStyle( w, key_margin_left ) ) {
		width -= w->margin.left;
	}
	if( !Widget_HasAutoStyle( w, key_margin_right ) ) {
		width -= w->margin.right;
	}
	/* 如果不是按内容框调整尺寸的，则不根据内间距调整宽度 */
	if( w->computed_style.box_sizing != SV_CONTENT_BOX ) {
		return width;
	}
	if( !Widget_HasAutoStyle( w, key_padding_left ) ) {
		width -= w->padding.left;
	}
	if( !Widget_HasAutoStyle( w, key_padding_right ) ) {
		width -= w->padding.right;
	}
	if( !Widget_HasAutoStyle( w, key_border_left_width ) ) {
		width -= w->computed_style.border.left.width;
	}
	if( !Widget_HasAutoStyle( w, key_border_right_width ) ) {
		width -= w->computed_style.border.right.width;
	}
	return width;
}

float Widget_ComputeFillAvailableWidth( LCUI_Widget w )
{
	float width;
	width = Widget_ComputeMaxAvaliableWidth( w );
	return Widget_GetAdjustedWidth( w, width );
}

static void Widget_ComputeLimitSize( LCUI_Widget w )
{
	LCUI_WidgetStyle *style;
	style = &w->computed_style;
	style->max_width = -1;
	style->min_width = -1;
	style->max_height = -1;
	style->min_height = -1;
	if( Widget_CheckStyleValid( w, key_max_width ) ) {
		style->max_width = ComputeXMetric( w, key_max_width );
	}
	if( Widget_CheckStyleValid( w, key_min_width ) ) {
		style->min_width = ComputeXMetric( w, key_min_width );
	}
	if( Widget_CheckStyleValid( w, key_max_height ) ) {
		style->max_height = ComputeYMetric( w, key_max_height );
	}
	if( Widget_CheckStyleValid( w, key_min_height ) ) {
		style->min_height = ComputeYMetric( w, key_min_height );
	}
}

static float GetLimitedWidth( LCUI_Widget w, float width )
{
	LCUI_WidgetStyle *style;
	style = &w->computed_style;
	if( style->max_width > -1 && width > style->max_width ) {
		width = style->max_width;
	}
	if( width < style->min_width ) {
		width = style->min_width;
	}
	return width;
}

static float GetLimitedHeight( LCUI_Widget w, float height )
{
	LCUI_WidgetStyle *style;
	style = &w->computed_style;
	if( style->max_height > -1 && height > style->max_height ) {
		height = style->max_height;
	}
	if( height < style->min_height ) {
		height = style->min_height;
	}
	return height;
}

static void Widget_SetSize( LCUI_Widget w, float width, float height )
{
	LCUI_RectF *box, *pbox;
	LCUI_BorderStyle *bbox;

	w->width = width;
	w->height = height;
	w->width = GetLimitedWidth( w, width );
	w->height = GetLimitedHeight( w, height );
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.content.width = w->width;
	w->box.content.height = w->height;
	w->box.padding.width = w->width;
	w->box.padding.height = w->height;
	pbox = &w->box.padding;
	bbox = &w->computed_style.border;
	/* 如果是以边框盒作为尺寸调整对象，则需根据边框盒计算内容框尺寸 */
	if( w->computed_style.box_sizing == SV_BORDER_BOX ) {
		box = &w->box.content;
		pbox->width -= bbox->left.width + bbox->right.width;
		pbox->height -= bbox->top.width + bbox->bottom.width;
		box->width = pbox->width;
		box->height = pbox->height;
		box->width -= w->padding.left + w->padding.right;
		box->height -= w->padding.top + w->padding.bottom;
	} else {
		/* 否则是以内容框作为尺寸调整对象，需计算边框盒的尺寸 */
		box = &w->box.border;
		pbox->width += w->padding.left + w->padding.right;
		pbox->height += w->padding.top + w->padding.bottom;
		box->width = pbox->width;
		box->height = pbox->height;
		box->width += bbox->left.width + bbox->right.width;
		box->height += bbox->top.width + bbox->bottom.width;
	}
	w->width = w->box.border.width;
	w->height = w->box.border.height;
	w->box.outer.width = w->box.border.width;
	w->box.outer.height = w->box.border.height;
	w->box.outer.width += w->margin.left + w->margin.right;
	w->box.outer.height += w->margin.top + w->margin.bottom;
}

/**
 * 计算合适的宽度
 * @param height[in][out]
 *	内容区域高度。当部件有自定义的尺寸计算函数时，会同时计算出内容高度和宽度，
 *	为避免重复调用该函数，height 参数同时也是输出参数。
 * @return
 *	计算后的内容宽度
 */
static float Widget_ComputeAutoWidth( LCUI_Widget w, float *height )
{
	float width = 0, max_width = -1, content_width = 0;

	if( Widget_HasFillAvailableWidth( w ) ) {
		width = Widget_ComputeFillAvailableWidth( w );
		if( Widget_HasStaticWidthParent( w ) ) {
			goto finish;
		}
		max_width = width;
		width = 0;
	}
	if( w->proto && w->proto->autosize ) {
		w->proto->autosize( w, &width, height );
	}
	Widget_ComputeContentSize( w, &content_width, NULL );
	width = max( width, content_width );
	/* 转换为边框盒宽度 */
	if( w->computed_style.box_sizing == SV_BORDER_BOX ) {
		width = ToBorderBoxWidth( w, width );
		if( max_width != -1 ) {
			max_width = ToBorderBoxWidth( w, max_width );
		}
		if( w->proto && w->proto->autosize ) {
			*height = ToBorderBoxHeight( w, *height );
		}
	}
	/* 如果该部件和父部件的宽度都是自适应 */
	if( !Widget_HasStaticWidthParent( w->parent ) &&
	    width > 0 && !Widget_HasAbsolutePosition( w ) ) {
		/* 如果超出父部件的内容框宽度则让父部件重新布局 */
		if( width > w->parent->box.content.width ) {
			Widget_AddTask( w->parent, WTT_LAYOUT );
		} else if( Widget_HasFillAvailableWidth( w ) ) {
			width = w->parent->box.content.width;
		}
	}
	width = Widget_GetAdjustedWidth( w, width );

finish:
	if( max_width != -1 && width > max_width ) {
		width = max_width;
	}
	return width;
}

/**
 * 计算合适的高度
 * @param width[in][out]
 *	内容区域宽度。当部件有自定义的尺寸计算函数时，会同时计算出内容高度和宽度，
 *	为避免重复调用该函数，width 参数同时也是输出参数。
 * @return
 *	计算后的内容高度
 */
static float Widget_ComputeAutoHeight( LCUI_Widget w, float *width )
{
	float height = 0, content_height = 0;

	if( w->computed_style.box_sizing == SV_BORDER_BOX ) {
		*width = ToContentBoxWidth( w, *width );
	}
	if( w->proto && w->proto->autosize ) {
		w->proto->autosize( w, width, &height );
	}
	Widget_ComputeContentSize( w, NULL, &content_height );
	height = max( height, content_height );
	if( w->computed_style.box_sizing == SV_BORDER_BOX ) {
		height = ToBorderBoxHeight( w, height );
		*width = ToBorderBoxWidth( w, *width );
	}
	return height;
}

/** 计算尺寸 */
static void Widget_ComputeSize( LCUI_Widget w )
{
	LCUI_BorderStyle *bbox;
	float width, height, max_width = -1;

	Widget_ComputeLimitSize( w );
	width = ComputeXMetric( w, key_width );
	height = ComputeYMetric( w, key_height );
	bbox = &w->computed_style.border;
	/* 当宽度可被直接计算出来时，转换成内容宽度，作为计算自动尺寸时的初始宽度 */
	if( width > 0 ) {
		width = GetLimitedWidth( w, width );
	}
	if( height > 0 ) {
		height = GetLimitedHeight( w, height );
	}
	if( width <= 0 && Widget_HasAutoStyle( w, key_width ) ) {
		width = Widget_ComputeAutoWidth( w, &height );
	}
	if( height <= 0 && Widget_HasAutoStyle( w, key_height ) ) {
		height = Widget_ComputeAutoHeight( w, &width );
	}
	Widget_SetSize( w, width, height );
}

static void Widget_SendResizeEvent( LCUI_Widget w )
{
	LCUI_WidgetEventRec e;
	e.target = w;
	e.data = NULL;
	e.type = WET_RESIZE;
	e.cancel_bubble = TRUE;
	Widget_TriggerEvent( w, &e, NULL );
	Widget_AddTask( w, WTT_REFRESH );
	Widget_PostSurfaceEvent( w, WET_RESIZE, FALSE );
}

void Widget_UpdateMargin( LCUI_Widget w )
{
	int i;
	LCUI_BoundBox *mbox = &w->computed_style.margin;
	struct { 
		LCUI_Style sval;
		float *fval;
		int key;
	} pd_map[4] = {
		{ &mbox->top, &w->margin.top, key_margin_top },
		{ &mbox->right, &w->margin.right, key_margin_right },
		{ &mbox->bottom, &w->margin.bottom, key_margin_bottom },
		{ &mbox->left, &w->margin.left, key_margin_left }
	};
	for( i = 0; i < 4; ++i ) {
		LCUI_Style s = &w->style->sheet[pd_map[i].key];
		if( !s->is_valid ) {
			pd_map[i].sval->type = SVT_PX;
			pd_map[i].sval->px = 0.0;
			*pd_map[i].fval = 0.0;
			continue;
		}
		*pd_map[i].sval = *s;
		*pd_map[i].fval = LCUIMetrics_Compute( s->value, s->type );
	}
	/* 如果有父级部件，则处理 margin-left 和 margin-right 的值 */
	if( w->parent ) {
		float width = w->parent->box.content.width;
		float margin_left = SVT_AUTO, margin_right = SVT_AUTO;
		if( w->style->sheet[key_margin_left].is_valid ) {
			margin_left = w->style->sheet[key_margin_left].type;
		}
		if( w->style->sheet[key_margin_right].is_valid ) {
			margin_right = w->style->sheet[key_margin_right].type;
		}
		if( margin_left == SVT_AUTO ) {
			if( margin_right == SVT_AUTO ) {
				w->margin.left = (width - w->width) / 2;
				if( w->margin.left < 0 ) {
					w->margin.left = 0;
				}
				w->margin.right = w->margin.left;
			} else {
				w->margin.left = width - w->width;
				w->margin.left -= w->margin.right;
				if( w->margin.left < 0 ) {
					w->margin.left = 0;
				}
			}
		} else if( margin_right == SVT_AUTO ) {
			w->margin.right = width - w->width;
			w->margin.right -= w->margin.left;
			if( w->margin.right < 0 ) {
				w->margin.right = 0;
			}
		}
	}
	if( w->parent ) {
		if( w->parent->style->sheet[key_width].type == SVT_AUTO ||
		    w->parent->style->sheet[key_height].type == SVT_AUTO ) {
			Widget_AddTask( w->parent, WTT_RESIZE );
		}
		if( w->computed_style.display != SV_NONE &&
		    w->computed_style.position == SV_STATIC ) {
			Widget_UpdateLayout( w->parent );
		}
	}
	Widget_AddTask( w, WTT_POSITION );
}

void Widget_UpdateSize( LCUI_Widget w )
{
	LCUI_RectF rect;
	int i, box_sizing;
	LCUI_Rect2F padding = w->padding;
	LCUI_BoundBox *pbox = &w->computed_style.padding;
	struct {
		LCUI_Style sval;
		float *ival;
		int key;
	} pd_map[4] = {
		{ &pbox->top, &w->padding.top, key_padding_top },
		{ &pbox->right, &w->padding.right, key_padding_right },
		{ &pbox->bottom, &w->padding.bottom, key_padding_bottom },
		{ &pbox->left, &w->padding.left, key_padding_left }
	};
	rect = w->box.graph;
	/* 内边距的单位暂时都用 px  */
	for( i = 0; i < 4; ++i ) {
		LCUI_Style s = &w->style->sheet[pd_map[i].key];
		if( !s->is_valid ) {
			pd_map[i].sval->type = SVT_PX;
			pd_map[i].sval->px = 0.0;
			*pd_map[i].ival = 0.0;
			continue;
		}
		*pd_map[i].sval = *s;
		*pd_map[i].ival = LCUIMetrics_Compute( s->value, s->type );
	}
	box_sizing = ComputeStyleOption( w, key_box_sizing, SV_CONTENT_BOX );
	w->computed_style.box_sizing = box_sizing;
	Widget_ComputeSize( w );
	Widget_UpdateGraphBox( w );
	/* 如果左右外间距是 auto 类型的，则需要计算外间距 */
	if( w->style->sheet[key_margin_left].is_valid &&
	    w->style->sheet[key_margin_left].type == SVT_AUTO ) {
		Widget_UpdateMargin( w );
	} else if( w->style->sheet[key_margin_right].is_valid &&
		   w->style->sheet[key_margin_right].type == SVT_AUTO ) {
		Widget_UpdateMargin( w );
	}
	/* 若尺寸无变化则不继续处理 */
	if( rect.width == w->box.graph.width &&
	    rect.height == w->box.graph.height && 
	    padding.top == w->padding.top &&
	    padding.right == w->padding.right &&
	    padding.bottom == w->padding.bottom &&
	    padding.left == w->padding.left ) {
		return;
	}
	/* 若在变化前后的宽高中至少有一个为 0，则不继续处理 */
	if( (w->box.graph.width <= 0 || w->box.graph.height <= 0) &&
	    (rect.width <= 0 || rect.height <= 0) ) {
		return;
	}
	Widget_UpdateLayout( w );
	/* 如果垂直对齐方式不为顶部对齐 */
	if( w->computed_style.vertical_align != SV_TOP ) {
		Widget_UpdatePosition( w );
	} else if( w->computed_style.position == SV_ABSOLUTE ) {
		/* 如果是绝对定位，且指定了右间距或底间距 */
		if( !Widget_CheckStyleValue( w, key_right, AUTO ) ||
		    !Widget_CheckStyleValue( w, key_bottom, AUTO ) ) {
			Widget_UpdatePosition( w );
		}
	}
	if( w->parent ) {
		Widget_InvalidateArea( w->parent, &rect, SV_PADDING_BOX );
		rect.width = w->box.graph.width;
		rect.height = w->box.graph.height;
		Widget_InvalidateArea( w->parent, &rect, SV_PADDING_BOX );
		if( w->parent->style->sheet[key_width].type == SVT_AUTO
		    || w->parent->style->sheet[key_height].type == SVT_AUTO ) {
			Widget_AddTask( w->parent, WTT_RESIZE );
		}
		if( w->computed_style.display != SV_NONE &&
		    w->computed_style.position == SV_STATIC ) {
			Widget_UpdateLayout( w->parent );
		}
	}
	Widget_SendResizeEvent( w );
	Widget_UpdateChildrenSize( w );
}

void Widget_UpdateSizeWithSurface( LCUI_Widget w )
{
	Widget_UpdateSize( w );
	Widget_PostSurfaceEvent( w, WET_RESIZE, TRUE );
}

void Widget_UpdateProps( LCUI_Widget w )
{
	LCUI_Style s;
	int prop = ComputeStyleOption( w, key_pointer_events, SV_AUTO );
	w->computed_style.pointer_events = prop;
	s = &w->style->sheet[key_focusable];
	if( s->is_valid && s->type == SVT_BOOL && s->value == 0 ) {
		w->computed_style.focusable = FALSE;
	} else {
		w->computed_style.focusable = TRUE;
	}
}

int Widget_SetAttributeEx( LCUI_Widget w, const char *name, void *value,
			   int value_type, void( *value_destructor )(void*) )
{
	LCUI_WidgetAttribute attr;
	if( !w->attributes ) {
		w->attributes = Dict_Create( &LCUIWidget.dt_attributes, NULL );
	}
	attr = Dict_FetchValue( w->attributes, name );
	if( attr ) {
		if( attr->value.destructor ) {
			attr->value.destructor( attr->value.data );
		}
	} else {
		attr = NEW( LCUI_WidgetAttributeRec, 1 );
		attr->name = strdup2( name );
		Dict_Add( w->attributes, attr->name, attr );
	}
	attr->value.data = value;
	attr->value.type = value_type;
	attr->value.destructor = value_destructor;
	return 0;
}

int Widget_SetAttribute( LCUI_Widget w, const char *name, const char *value )
{
	char *value_str;
	if( strcmp( name, "disabled" ) == 0 ) {
		if( !value || strcmp( value, "false" ) != 0 ) {
			Widget_SetDisabled( w, TRUE );
		} else {
			Widget_SetDisabled( w, FALSE );
		}
		return 0;
	}
	if( !value ) {
		return Widget_SetAttributeEx( w, name, NULL, SVT_NONE, NULL );
	}
	value_str = strdup2( value );
	if( !value_str ) {
		return -ENOMEM;
	}
	return Widget_SetAttributeEx( w, name, value_str, SVT_STRING, free );
}

const char *Widget_GetAttribute( LCUI_Widget w, const char *name )
{
	LCUI_WidgetAttribute attr;
	if( !w->attributes ) {
		return NULL;
	}
	attr = Dict_FetchValue( w->attributes, name );
	if( attr ) {
		return attr->value.string;
	}
	return NULL;
}

LCUI_BOOL Widget_CheckPrototype( LCUI_Widget w, LCUI_WidgetPrototypeC proto )
{
	LCUI_WidgetPrototypeC p;
	for( p = w->proto; p; p = p->proto ) {
		if( p == proto ) {
			return TRUE;
		}
	}
	return FALSE;
}

/** 为部件添加一个类 */
int Widget_AddClass( LCUI_Widget w, const char *class_name )
{
	if( strshas( w->classes, class_name ) ) {
		return 1;
	}
	if( strsadd( &w->classes, class_name ) <= 0 ) {
		return 0;
	}
	Widget_HandleChildrenStyleChange( w, 0, class_name );
	Widget_UpdateStyle( w, TRUE );
	return 1;
}

/** 判断部件是否包含指定的类 */
LCUI_BOOL Widget_HasClass( LCUI_Widget w, const char *class_name )
{
	if( strshas( w->classes, class_name ) ) {
		return TRUE;
	}
	return FALSE;
}

/** 从部件中移除一个类 */
int Widget_RemoveClass( LCUI_Widget w, const char *class_name )
{
	if( strshas( w->classes, class_name ) ) {
		Widget_HandleChildrenStyleChange( w, 0, class_name );
		strsdel( &w->classes, class_name );
		Widget_UpdateStyle( w, TRUE );
		return 1;
	}
	return 0;
}

int Widget_AddStatus( LCUI_Widget w, const char *status_name )
{
	if( strshas( w->status, status_name ) ) {
		return 0;
	}
	if( strsadd( &w->status, status_name ) <= 0 ) {
		return 0;
	}
	Widget_HandleChildrenStyleChange( w, 1, status_name );
	Widget_UpdateStyle( w, TRUE );
	return 1;
}

LCUI_BOOL Widget_HasStatus( LCUI_Widget w, const char *status_name )
{
	if( strshas( w->status, status_name ) ) {
		return TRUE;
	}
	return FALSE;
}

int Widget_RemoveStatus( LCUI_Widget w, const char *status_name )
{
	if( strshas( w->status, status_name ) ) {
		Widget_HandleChildrenStyleChange( w, 1, status_name );
		strsdel( &w->status, status_name );
		Widget_UpdateStyle( w, TRUE );
		return 1;
	}
	return 0;
}

float Widget_ComputeMaxAvaliableWidth( LCUI_Widget widget )
{
	LCUI_Widget w;
	float width = 0, padding = 0;
	for( w = widget->parent; w; w = w->parent ) {
		if( !Widget_HasAutoStyle( w, key_width ) ||
		    w->computed_style.max_width >= 0) {
			width = w->box.content.width;
			break;
		}
		padding += w->box.border.width - w->box.content.width;
	}
	width -= padding;
	if( Widget_HasAbsolutePosition( widget ) ) {
		width += widget->padding.left + widget->padding.right;
	}
	if( width < 0 ) {
		width = 0;
	}
	return width;
}

float Widget_ComputeMaxWidth( LCUI_Widget widget )
{
	float width;
	if( !Widget_HasAutoStyle( widget, key_width ) ) {
		return widget->box.border.width;
	}
	width = Widget_ComputeMaxAvaliableWidth( widget );
	if( !Widget_HasAutoStyle( widget, key_max_width ) ) {
		if( widget->computed_style.max_width > -1 &&
		    width < widget->computed_style.max_width ) {
			width = widget->computed_style.max_width;
		}
	}
	return width;
}

float Widget_ComputeMaxContentWidth( LCUI_Widget w )
{
	float width = Widget_ComputeMaxWidth( w );
	width = Widget_GetAdjustedWidth( w, width );
	width = ToContentBoxWidth( w, width );
	return width;
}

static void _LCUIWidget_PrintTree( LCUI_Widget w, int depth, const char *prefix )
{
	int len;
	LCUI_Widget child;
	LinkedListNode *node;
	LCUI_SelectorNode snode;
	char str[16], child_prefix[512];

	len = strlen(prefix);
	strcpy( child_prefix, prefix );
	for( LinkedList_Each( node, &w->children ) ) {
		if( node == w->children.tail.prev ) {
			strcpy( str, "└" );
			strcpy( &child_prefix[len], "    " );
		} else {
			strcpy( str, "├" );
			strcpy( &child_prefix[len], "│  " );
		} 
		strcat( str, "─" );
		child = node->data;
		if( child->children.length == 0 ) {
			strcat( str, "─" );
		} else {
			strcat( str, "┬" );
		}
		snode = Widget_GetSelectorNode( child );
		LOG( "%s%s %s, xy:(%g,%g), size:(%g,%g), "
		     "visible: %s, padding: (%g,%g,%g,%g), margin: (%g,%g,%g,%g)\n",
		     prefix, str, snode->fullname, child->x, child->y,
		     child->width, child->height,
		     child->computed_style.visible ? "true" : "false",
		     child->padding.top, child->padding.right, child->padding.bottom,
		     child->padding.left, child->margin.top, child->margin.right,
		     child->margin.bottom, child->margin.left );
		SelectorNode_Delete( snode );
		_LCUIWidget_PrintTree( child, depth+1, child_prefix );
	}
}

void Widget_PrintTree( LCUI_Widget w )
{
	LCUI_SelectorNode node;
	w = w ? w : LCUIWidget.root;
	node = Widget_GetSelectorNode( w );
	LOG( "%s, xy:(%g,%g), size:(%g,%g), visible: %s\n",
	     node->fullname, w->x, w->y, w->width, w->height,
	     w->computed_style.visible ? "true" : "false" );
	SelectorNode_Delete( node );
	_LCUIWidget_PrintTree( w, 0, "  " );
}

static void OnClearWidgetAttribute( void *privdata, void *data )
{
	LCUI_WidgetAttribute attr = data;
	if( attr->value.destructor ) {
		attr->value.destructor( attr->value.data );
	}
	free( attr->name );
	attr->name = NULL;
	attr->value.data = NULL;
	free( attr );
}

static void OnClearWidgetList( void *privdata, void *data )
{
	LinkedList *list = data;
	LinkedList_Clear( list, NULL );
	free( list );
}

void LCUIWidget_InitBase( void )
{
	LCUIMutex_Init( &LCUIWidget.mutex );
	LCUIWidget.root = LCUIWidget_New( "root" );
	LCUIWidget.dt_ids = DictType_StringCopyKey;
	LCUIWidget.dt_ids.valDestructor = OnClearWidgetList;
	LCUIWidget.dt_attributes = DictType_StringCopyKey;
	LCUIWidget.dt_attributes.valDestructor = OnClearWidgetAttribute;
	LCUIWidget.ids = Dict_Create( &LCUIWidget.dt_ids, NULL );
	Widget_SetTitleW( LCUIWidget.root, L"LCUI Display" );
}

void LCUIWidget_FreeRoot( void )
{
	Widget_ExecDestroy( LCUIWidget.root );
}

void LCUIWidget_FreeBase( void )
{
	Dict_Release( LCUIWidget.ids );
	LCUIMutex_Destroy( &LCUIWidget.mutex );
	LCUIWidget.ids = NULL;
	LCUIWidget.root = NULL;
}
