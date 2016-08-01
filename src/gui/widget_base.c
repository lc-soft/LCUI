/* ***************************************************************************
 * widget_base.c -- the widget base operation set.
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
 * ****************************************************************************/

/* ****************************************************************************
 * widget_base.c -- 部件的基本操作集。
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
 * ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

#undef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))

static struct LCUIWidgetModule {
	LCUI_Widget root;	/** 根级部件 */
	LCUI_RBTree ids;	/** 各种部件的ID索引 */
} LCUIWidget;

LCUI_Widget LCUIWidget_GetRoot(void)
{
	return LCUIWidget.root;
}

/** 刷新在追加部件后的状态，例如 :first-child 和 :last-child */
static void Widget_UpdateStatusAfterAppend( LCUI_Widget w,
					    LCUI_BOOL is_remove_mode )
{
	LinkedListNode *node = Widget_GetNode( w );
	if( w->index == 0 ) {
		if( is_remove_mode ) {
			Widget_RemoveStatus( w, "first-child" );
			if( node->next ) {
				Widget_AddStatus( node->next->data, 
						  "first-child" );
			}
		} else {
			Widget_AddStatus( w, "first-child" );
		}
	}
	if( w->index == w->parent->children.length - 1 ) {
		if( is_remove_mode ) {
			Widget_RemoveStatus( w, "last-child" );
			if( w->parent->children.length > 1 ) {
				Widget_AddStatus( node->prev->data, 
						  "last-child" );
			}
		} else {
			Widget_AddStatus( w, "last-child" );
			if( w->parent->children.length > 1 ) {
				Widget_RemoveStatus( node->prev->data, 
						     "last-child" );
			}
		}
	}
}

int Widget_Append( LCUI_Widget parent, LCUI_Widget widget )
{
	LCUI_Widget child;
	LinkedListNode *node, *snode;
	if( !parent || !widget || parent == widget->parent ) {
		return -1;
	}
	if( parent == widget ) {
		return -2;
	}
	node = Widget_GetNode( widget );
	snode = Widget_GetShowNode( widget );
	if( widget->parent ) {
		Widget_UpdateStatusAfterAppend( widget, TRUE );
		/** 修改它后面的部件的 index 值 */
		node = node->next;
		while( node ) {
			child = node->data;
			child->index -= 1;
			node = node->next;
		}
		node = Widget_GetNode( widget );
		LinkedList_Unlink( &widget->parent->children, node );
		LinkedList_Unlink( &widget->parent->children_show, snode );
		Widget_PostSurfaceEvent( widget, WET_REMOVE );
	}
	widget->parent = parent;
	widget->state = WSTATE_CREATED;
	widget->index = parent->children.length;
	LinkedList_AppendNode( &parent->children, node );
	LinkedList_AppendNode( &parent->children_show, snode );
	Widget_PostSurfaceEvent( widget, WET_ADD );
	Widget_AddTaskForChildren( widget, WTT_REFRESH_STYLE );
	Widget_UpdateTaskStatus( widget );
	Widget_UpdateLayout( parent );
	Widget_UpdateStatusAfterAppend( widget, FALSE );
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
		node = LinkedList_GetNode( &widget->children, -1 );
		Widget_RemoveStatus( node->data, "last-child" );
	}
	node = Widget_GetNode( widget );
	i = widget->children.length;
	target = node->prev;
	node = widget->children.tail.prev;
	while( i-- > 0 ) {
		prev = node->prev;
		child = node->data;
		snode = Widget_GetShowNode( child );
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
		node = LinkedList_GetNode( list, -1 );
		Widget_AddStatus( node->data, "last-child" );
	}
	Widget_Destroy( widget );
	return 0;
}

/** 构造函数 */
static void Widget_Init( LCUI_Widget widget )
{
	memset( widget, 0, sizeof(struct LCUI_WidgetRec_));
	widget->state = WSTATE_CREATED;
	widget->trigger = EventTrigger();
	widget->style = StyleSheet();
	widget->custom_style = StyleSheet();
	widget->cached_style = StyleSheet();
	widget->inherited_style = StyleSheet();
	widget->computed_style.opacity = 1.0;
	widget->computed_style.visible = TRUE;
	widget->computed_style.focusable = TRUE;
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
	Background_Init( &widget->computed_style.background );
	BoxShadow_Init( &widget->computed_style.shadow );
	Border_Init( &widget->computed_style.border );
	LinkedList_Init( &widget->children );
	LinkedList_Init( &widget->children_show );
	LinkedList_Init( &widget->children_trash );
	LinkedList_Init( &widget->dirty_rects );
	LCUIMutex_Init( &widget->mutex );
	Graph_Init( &widget->graph );
}

LCUI_Widget LCUIWidget_New( const char *type_name )
{
	LCUI_WidgetClass *wc;
	LinkedListNode *node;
	LCUI_Widget widget = malloc( sizeof(struct LCUI_WidgetRec_) +
				     sizeof(LinkedListNode)*2 );
	Widget_Init( widget );
	node = Widget_GetNode( widget );
	node->data = widget;
	node->next = node->prev = NULL;
	node = Widget_GetShowNode( widget );
	node->data = widget;
	node->next = node->prev = NULL;
	if( type_name ) {
		widget->type = strdup( type_name );
		wc = LCUIWidget_GetClass( type_name );
		if( wc ) {
			wc->methods.init( widget );
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
	LCUI_WidgetClass *wc;
	wc = LCUIWidget_GetClass( widget->type );
	Widget_ReleaseMouseCapture( widget );
	Widget_ReleaseTouchCapture( widget, -1 );
	Widget_StopEventPropagation( widget );
	LCUIWidget_ClearEventTarget( widget );
	if( wc && wc->methods.destroy ) {
		wc->methods.destroy( widget );
	}
	/* 先释放显示列表，后销毁部件列表，因为部件在这两个链表中的节点是和它共用
	 * 一块内存空间的，销毁部件列表会把部件释放掉，所以把这个操作放在后面 */
	LinkedList_ClearData( &widget->children_show, NULL );
	LinkedList_ClearData( &widget->children, Widget_OnDestroy );
	LinkedList_Clear( &widget->dirty_rects, free );
	Widget_PostSurfaceEvent( widget, WET_REMOVE );
	Widget_UpdateLayout( widget->parent );
	Widget_SetId( widget, NULL );
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
	w->state = WSTATE_DELETED;
	if( root != LCUIWidget.root ) {
		Widget_ExecDestroy( w );
		return;
	}
	if( w->parent ) {
		LCUI_Widget child;
		LinkedListNode *node, *snode;
		node = Widget_GetNode( w );
		node = node->next;
		while( node ) {
			child = node->data;
			child->index -= 1;
			node = node->next;
		}
		node = Widget_GetNode( w );
		snode = Widget_GetShowNode( w );
		LinkedList_Unlink( &w->parent->children, node );
		LinkedList_Unlink( &w->parent->children_show, snode );
		LinkedList_AppendNode( &w->parent->children_trash, node );
		if( w->computed_style.position != SV_ABSOLUTE ) {
			Widget_UpdateLayout( w->parent );
		}
		Widget_PushInvalidArea( w, NULL, SV_GRAPH_BOX );
	}
}

void Widget_Empty( LCUI_Widget w )
{
	LinkedListNode *node;
	LCUI_Widget root = w, child;
	while( root->parent ) {
		root = root->parent;
	}
	LinkedList_ClearData( &w->children_show, NULL );
	if( root == LCUIWidget.root ) {
		LinkedList_ForEach( node, &w->children ) {
			child = node->data;
			child->state = WSTATE_DELETED;
		}
		LinkedList_Concat( &w->children_trash, &w->children );
		Widget_PushInvalidArea( w, NULL, SV_GRAPH_BOX );
		Widget_AddTask( w, WTT_LAYOUT );
		return;
	} else {
		LinkedList list;
		LinkedListNode *prev;
		LinkedList_Init( &list );
		LinkedList_Concat( &list, &w->children );
		node = list.head.next;
		while( node ) {
			prev = node->prev;
			Widget_ExecDestroy( node->data );
			node = prev->next;
		}
	}
}

LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y )
{
	LCUI_BOOL is_hit;
	LinkedListNode *node;
	LCUI_Widget target = widget, c = NULL;
	if( !widget ) {
		return NULL;
	}
	do {
		is_hit = FALSE;
		LinkedList_ForEach( node, &target->children_show ) {
			c = node->data;
			if( !c->computed_style.visible ) {
				continue;
			}
			if( LCUIRect_HasPoint(&c->box.border, x, y) ) {
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

void Widget_GetAbsXY( LCUI_Widget w, LCUI_Widget parent, int *x, int *y )
{
	int tmp_x = 0, tmp_y = 0;
	while( w && w != parent ) {
		tmp_x += w->box.border.x;
		tmp_y += w->box.border.y;
		w = w->parent;
	}
	*x = tmp_x;
	*y = tmp_y;
}

LCUI_Widget LCUIWidget_GetById( const char *idstr )
{
	if( !idstr ) {
		return NULL;
	}
	return RBTree_CustomGetData( &LCUIWidget.ids, idstr );
}

int Widget_Top( LCUI_Widget w )
{
	DEBUG_MSG("tip\n");
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

int Widget_SetId( LCUI_Widget w, const char *idstr )
{
	if( w->id ) {
		RBTree_CustomErase( &LCUIWidget.ids, idstr );
		free( w->id );
		w->id = NULL;
	}
	if( !idstr ) {
		return -1;
	}
	w->id = strdup( idstr );
	if( RBTree_CustomInsert( &LCUIWidget.ids, idstr, w ) ) {
		return 0;
	}
	return -2;
}

/** 计算边框样式 */
static void ComputeBorderStyle( LCUI_StyleSheet ss, LCUI_Border *b )
{
	LCUI_Style style;
	int key = key_border_start + 1;
	for( ; key < key_border_end; ++key ) {
		style = &ss->sheet[key];
		if( !style->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_border_color:
			b->top.color = style->color;
			b->right.color = style->color;
			b->bottom.color = style->color;
			b->left.color = style->color;
			break;
		case key_border_style:
			b->top.style = style->value;
			b->right.style = style->value;
			b->bottom.style = style->value;
			b->left.style = style->value;
			break;
		case key_border_width:
			b->top.width = style->value;
			b->right.width = style->value;
			b->bottom.width = style->value;
			b->left.width = style->value;
			break;
		case key_border_top_color:
			b->top.color = style->color;
			break;
		case key_border_right_color:
			b->right.color = style->color;
			break;
		case key_border_bottom_color:
			b->bottom.color = style->color;
			break;
		case key_border_left_color:
			b->left.color = style->color;
			break;
		case key_border_top_width:
			b->top.width = style->value;
			break;
		case key_border_right_width:
			b->right.width = style->value;
			break;
		case key_border_bottom_width:
			b->bottom.width = style->value;
			break;
		case key_border_left_width:
			b->left.width = style->value;
			break;
		case key_border_top_style:
			b->top.style = style->value;
			break;
		case key_border_right_style:
			b->right.style = style->value;
			break;
		case key_border_bottom_style:
			b->bottom.style = style->value;
			break;
		case key_border_left_style:
			b->left.style = style->value;
			break;
		default: break;
		}
	}
}

void Widget_UpdateBorder( LCUI_Widget w )
{
	LCUI_Rect rect;
	LCUI_Border ob, *nb;
	ob = w->computed_style.border;
	ComputeBorderStyle( w->style, &w->computed_style.border );
	nb = &w->computed_style.border;
	/* 如果边框变化并未导致图层尺寸变化的话，则只重绘边框 */
	if( ob.top.width != nb->top.width || 
	    ob.right.width != nb->right.width ||
	    ob.bottom.width != nb->bottom.width ||
	    ob.left.width != nb->left.width ) {
		Widget_AddTask( w, WTT_RESIZE );
		Widget_AddTask( w, WTT_POSITION );
		return;
	}
	rect.x = rect.y = 0;
	rect.width = w->box.border.width;
	rect.width -= max( ob.top_right_radius, ob.right.width );
	rect.height = max( ob.top_left_radius, ob.top.width );
	/* 上 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
	rect.x = w->box.border.w;
	rect.width = max( ob.top_right_radius, ob.right.width );
	rect.x -= rect.width;
	rect.height = w->box.border.height;
	rect.height -= max( ob.bottom_right_radius, ob.bottom.width );
	/* 右 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
	rect.x = max( ob.bottom_left_radius, ob.left.width );
	rect.y = w->box.border.height;
	rect.width = w->box.border.width;
	rect.width -= rect.x;
	rect.height = max( ob.bottom_right_radius, ob.bottom.width );
	rect.y -= rect.height;
	/* 下 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
	rect.width = rect.x;
	rect.x = 0;
	rect.y = max( ob.top_left_radius, ob.left.width );
	rect.height = w->box.border.height;
	rect.height -= rect.y;
	/* 左 */
	Widget_InvalidateArea( w, &rect, SV_BORDER_BOX );
}

/** 计算矩形阴影样式 */
static void ComputeBoxShadowStyle( LCUI_StyleSheet ss, LCUI_BoxShadow *bsd )
{
	LCUI_Style style;
	int key = key_box_shadow_start + 1;
	memset( bsd, 0, sizeof( *bsd ) );
	for( ; key < key_box_shadow_end; ++key ) {
		style = &ss->sheet[key];
		if( !style->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_box_shadow_x: bsd->x = style->value; break;
		case key_box_shadow_y: bsd->y = style->value; break;
		case key_box_shadow_spread: bsd->spread = style->value; break;
		case key_box_shadow_blur: bsd->blur = style->value; break;
		case key_box_shadow_color: bsd->color = style->color; break;
		default: break;
		}
	}
}

void Widget_UpdateBoxShadow( LCUI_Widget w )
{
	LCUI_BoxShadow bs = w->computed_style.shadow;
	ComputeBoxShadowStyle( w->style, &w->computed_style.shadow );
	/* 如果阴影变化并未导致图层尺寸变化，则只重绘阴影 */
	if( bs.x == w->computed_style.shadow.x && 
	    bs.y == w->computed_style.shadow.y && 
	    bs.spread == w->computed_style.shadow.spread ) {
		LCUI_Rect rects[4];
		LCUIRect_CutFourRect( &w->box.border,
				      &w->box.graph, rects );
		Widget_InvalidateArea( w, &rects[0], SV_GRAPH_BOX );
		Widget_InvalidateArea( w, &rects[1], SV_GRAPH_BOX );
		Widget_InvalidateArea( w, &rects[2], SV_GRAPH_BOX );
		Widget_InvalidateArea( w, &rects[3], SV_GRAPH_BOX );
		return;
	}
	Widget_AddTask( w, WTT_RESIZE );
	Widget_AddTask( w, WTT_POSITION );
}

void Widget_UpdateVisibility( LCUI_Widget w )
{
	int display = w->computed_style.display;
	LCUI_Style s = &w->style->sheet[key_visible];
	LCUI_BOOL visible = w->computed_style.visible;
	if( w->computed_style.display == SV_NONE ) {
		visible = FALSE;
	}
	w->computed_style.visible = !(s->is_valid && !s->value);
	s = &w->style->sheet[key_display];
	if( s->is_valid ) {
		w->computed_style.display = s->style;
		if( w->computed_style.display == SV_NONE ) {
			w->computed_style.visible = FALSE;
		}
	} else {
		w->computed_style.display = SV_BLOCK;
	}
	if( visible == w->computed_style.visible ) {
		return;
	}
	visible = w->computed_style.visible;
	if( w->parent ) {
		Widget_PushInvalidArea( w, NULL, SV_GRAPH_BOX );
		if( w->computed_style.display != display &&
		    w->computed_style.position != SV_ABSOLUTE ) {
			Widget_UpdateLayout( w->parent );
		}
	}
	DEBUG_MSG( "visible: %s\n", visible ? "TRUE" : "FALSE" );
	Widget_PostSurfaceEvent( w, visible ? WET_SHOW : WET_HIDE );
}

void Widget_UpdateOpacity( LCUI_Widget w )
{
	float opacity = 1.0;
	LCUI_Style s = &w->style->sheet[key_opacity];
	if( s->is_valid ) {
		switch( s->type ) {
		case SVT_VALUE: opacity = 1.0 * s->value; break;
		case SVT_SCALE: opacity = s->val_scale; break;
		default: opacity = 1.0; break;
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
		z_index = s->value;
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
	snode = Widget_GetShowNode( w );
	list = &w->parent->children_show;
	LinkedList_Unlink( list, snode );
	LinkedList_ForEach( cnode, list ) {
		LCUI_Widget child = cnode->data;
		LCUI_WidgetStyle *ccs = &child->computed_style;
		csnode = Widget_GetShowNode( child );
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

static int ComputeXNumber( LCUI_Widget w, int key )
{
	LCUI_Style s = &w->style->sheet[key];
	switch( s->type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		}
		return w->parent->box.content.width * s->scale;
	case SVT_PX: return s->px;
	case SVT_NONE:
	case SVT_AUTO:
	default: break;
	}
	return 0;
}

static int ComputeYNumber( LCUI_Widget w, int key )
{
	LCUI_Style s = &w->style->sheet[key];
	switch( s->type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		}
		return w->parent->box.content.height * s->scale;
	case SVT_PX: return s->px;
	case SVT_NONE:
	case SVT_AUTO:
	default: break;
	}
	return 0;
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

void Widget_UpdatePosition( LCUI_Widget w )
{
	LCUI_Rect rect = w->box.graph;
	int position = ComputeStyleOption( w, key_position, SV_STATIC );
	int valign = ComputeStyleOption( w, key_vertical_align, SV_TOP );
	w->computed_style.vertical_align = valign;
	w->computed_style.left = ComputeXNumber( w, key_left );
	w->computed_style.top = ComputeXNumber( w, key_top );
	w->computed_style.right = ComputeYNumber( w, key_right );
	w->computed_style.bottom = ComputeYNumber( w, key_bottom );
	if( w->parent && w->computed_style.position != position ) {
		Widget_UpdateLayout( w->parent );
	}
	w->computed_style.position = position;
	Widget_UpdateZIndex( w );
	w->x = w->origin_x;
	w->y = w->origin_y;
	switch( position ) {
	case SV_ABSOLUTE:
		w->x = 0;
		w->y = 0;
		if( w->style->sheet[key_left].is_valid ) {
			w->x = w->computed_style.left;
		} else if( w->style->sheet[key_right].is_valid ) {
			if( w->parent ) {
				w->x = w->parent->box.border.width;
				w->x -= w->width;
			}
			w->x -= w->computed_style.right;
		}
		if( w->style->sheet[key_top].is_valid ) {
			w->y = w->computed_style.top;
		} else if( w->style->sheet[key_bottom].is_valid ) {
			if( w->parent ) {
				w->y = w->parent->box.border.height;
				w->y -= w->height;
			}
			w->y -= w->computed_style.bottom;
		}
		break;
	case SV_RELATIVE:
		if( w->style->sheet[key_left].is_valid ) {
			w->x -= w->computed_style.left;
		} else if( w->style->sheet[key_right].is_valid ) {
			w->x += w->computed_style.right;
		}
		if( w->style->sheet[key_top].is_valid ) {
			w->y += w->computed_style.top;
		} else if( w->style->sheet[key_bottom].is_valid ) {
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
	w->box.graph.x -= BoxShadow_GetBoxX( &w->computed_style.shadow );
	w->box.graph.y -= BoxShadow_GetBoxY( &w->computed_style.shadow );
	if( w->parent ) {
		DEBUG_MSG("new-rect: %d,%d,%d,%d\n", w->box.graph.x, w->box.graph.y, w->box.graph.w, w->box.graph.h);
		DEBUG_MSG("old-rect: %d,%d,%d,%d\n", rect.x, rect.y, rect.w, rect.h);
		/* 标记移动前后的区域 */
		Widget_PushInvalidArea( w, NULL, SV_GRAPH_BOX );
		Widget_PushInvalidArea( w->parent, &rect, SV_PADDING_BOX );
	}
	/* 检测是否为顶级部件并做相应处理 */
	Widget_PostSurfaceEvent( w, WET_MOVE );
}

/** 更新位图尺寸 */
static void Widget_UpdateGraphBox( LCUI_Widget w )
{
	LCUI_Rect *rb = &w->box.border;
	LCUI_Rect *rg = &w->box.graph;
	LCUI_BoxShadow *shadow = &w->computed_style.shadow;
	rg->x = w->x - BoxShadow_GetBoxX( shadow );
	rg->y = w->y - BoxShadow_GetBoxY( shadow );
	rg->width = BoxShadow_GetWidth( shadow, rb->width );
	rg->height = BoxShadow_GetHeight( shadow, rb->height );
	if( !w->enable_graph ) {
		Graph_Free( &w->graph );
		return;
	}
	/* 如果有会产生透明效果的样式 */
	if( w->computed_style.border.bottom_left_radius > 0
	    || w->computed_style.border.bottom_right_radius > 0
	    || w->computed_style.border.top_left_radius > 0
	    || w->computed_style.border.top_right_radius > 0
	    || w->computed_style.background.color.alpha < 255
	    || w->computed_style.shadow.blur > 0 ) {
		w->graph.color_type = COLOR_TYPE_ARGB;
	} else {
		w->graph.color_type = COLOR_TYPE_RGB;
	}
	Graph_Create( &w->graph, rg->width, rg->height );
}

/** 计算合适的内容框大小 */
static void Widget_ComputeContentSize( LCUI_Widget w, int *width, int *height )
{
	int n;
	LCUI_Style s;
	LCUI_Widget child;
	LinkedListNode *node;

	*width = *height = 0;
	LinkedList_ForEach( node, &w->children_show ) {
		child = node->data;
		/* 忽略不可见、绝对定位的部件 */
		if( !child->computed_style.visible ||
		    child->computed_style.position == SV_ABSOLUTE ) {
			continue;
		}
		/* 忽略使用百分比作为尺寸单位的部件 */
		s = &child->style->sheet[key_width];
		if( !s->is_valid || s->type != SVT_SCALE ) {
			n = child->box.outer.x + child->box.outer.width;
			if( n > *width ) {
				*width = n;
			}
		}
		s = &child->style->sheet[key_height];
		if( !s->is_valid || s->type != SVT_SCALE ) {
			n = child->box.outer.y + child->box.outer.height;
			if( n > *height ) {
				*height = n;
			}
		}
	}
	/* 计算出来的尺寸是包含 padding-left 和 padding-top 的，需要针对不同的对象来调整尺寸 */
	switch(w->computed_style.box_sizing) {
	case SV_BORDER_BOX:
		*width += w->padding.right;
		*height += w->padding.bottom;
		break;
	case SV_CONTENT_BOX:
		*width -= w->padding.left;
		*height -= w->padding.top;
	default: break;
	}
}

/** 计算尺寸 */
static void Widget_ComputeSize( LCUI_Widget w )
{
	int n;
	LCUI_Rect *box, *pbox = &w->box.padding;
	LCUI_Style sw = &w->style->sheet[key_width];
	LCUI_Style sh = &w->style->sheet[key_height];
	LCUI_Border *bbox = &w->computed_style.border;
	w->width = ComputeXNumber( w, key_width );
	w->height = ComputeYNumber( w, key_height );
	if( sw->type == SVT_AUTO || sh->type == SVT_AUTO ) {
		int width, height;
		LCUI_WidgetClass *wc = LCUIWidget_GetClass( w->type );
		if( wc && wc->methods.autosize ) {
			wc->methods.autosize( w, &width, &height );
		} else {
			Widget_ComputeContentSize( w, &width, &height );
		}
		if( sw->type == SVT_AUTO ) {
			w->width = width;
		}
		if( sh->type == SVT_AUTO ) {
			w->height = height;
		}
	}
	if( w->style->sheet[key_max_width].is_valid ) {
		n = ComputeXNumber( w, key_max_width );
		if( w->width > n ) {
			w->width = n;
		}
	}
	if( w->style->sheet[key_min_width].is_valid ) {
		n = ComputeXNumber( w, key_min_width );
		if( w->width < n ) {
			w->width = n;
		}
	}
	if( w->style->sheet[key_max_height].is_valid ) {
		n = ComputeYNumber( w, key_max_height );
		if( w->height > n ) {
			w->height = n;
		}
	}
	if( w->style->sheet[key_min_height].is_valid ) {
		n = ComputeYNumber( w, key_min_height );
		if( w->height < n ) {
			w->height = n;
		}
	}
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.content.width = w->width;
	w->box.content.height = w->height;
	w->box.padding.width = w->width;
	w->box.padding.height = w->height;
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
	/* 如果有父级部件，则处理 margin-left 和 margin-right 的值 */
	if( w->parent ) {
		int width = w->parent->box.content.width;
		int margin_left = SVT_AUTO, margin_right = SVT_AUTO;
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
	w->box.outer.width = w->box.border.width;
	w->box.outer.height = w->box.border.height;
	w->box.outer.width += w->margin.left + w->margin.right;
	w->box.outer.height += w->margin.top + w->margin.bottom;
}

static void Widget_SendResizeEvent( LCUI_Widget w )
{
	LCUI_Widget child;
	LCUI_WidgetEventRec e;
	LinkedListNode *node;
	e.target = w;
	e.data = NULL;
	e.type = WET_RESIZE;
	e.cancel_bubble = TRUE;
	Widget_TriggerEvent( w, &e, NULL );
	Widget_AddTask( w, WTT_REFRESH );
	Widget_PostSurfaceEvent( w, WET_RESIZE );
	LinkedList_ForEach( node, &w->children ) {
		child = node->data;
		if( child->style->sheet[key_width].type == SVT_SCALE || 
		    child->style->sheet[key_height].type == SVT_SCALE ) {
			Widget_AddTask( child, WTT_RESIZE );
		}
		if( child->computed_style.position == SV_ABSOLUTE &&
		    (child->style->sheet[key_right].is_valid ||
		      child->style->sheet[key_bottom].is_valid) ) {
			Widget_AddTask( child, WTT_POSITION );
		}
		if( child->computed_style.vertical_align != SV_TOP ) {
			Widget_AddTask( child, WTT_POSITION );
		}
	}
}

void Widget_UpdateSize( LCUI_Widget w )
{
	LCUI_Rect rect;
	int i, box_sizing;
	LCUI_BoundBox *mbox = &w->computed_style.margin;
	LCUI_BoundBox *pbox = &w->computed_style.padding;
	struct { 
		LCUI_Style sval;
		int *ival;
		int key;
	} pd_map[8] = {
		{ &pbox->top, &w->padding.top, key_padding_top },
		{ &pbox->right, &w->padding.right, key_padding_right },
		{ &pbox->bottom, &w->padding.bottom, key_padding_bottom },
		{ &pbox->left, &w->padding.left, key_padding_left },
		{ &mbox->top, &w->margin.top, key_margin_top },
		{ &mbox->right, &w->margin.right, key_margin_right },
		{ &mbox->bottom, &w->margin.bottom, key_margin_bottom },
		{ &mbox->left, &w->margin.left, key_margin_left }
	};
	rect = w->box.graph;
	/* 内边距的单位暂时都用 px  */
	for( i = 0; i < 8; ++i ) {
		LCUI_Style s = &w->style->sheet[pd_map[i].key];
		if( !s->is_valid || s->type != SVT_PX ) {
			pd_map[i].sval->type = SVT_PX;
			pd_map[i].sval->px = 0;
			*pd_map[i].ival = 0;
			continue;
		}
		*pd_map[i].sval = *s;
		*pd_map[i].ival = s->px;
	}
	box_sizing = ComputeStyleOption( w, key_box_sizing, SV_CONTENT_BOX );
	w->computed_style.box_sizing = box_sizing;
	Widget_ComputeSize( w );
	Widget_UpdateGraphBox( w );
	/* 若尺寸无变化则不继续处理 */
	if( rect.width == w->box.graph.width &&
	    rect.height == w->box.graph.height ) {
		return;
	}
	/* 若在变化前后的宽高中至少有一个为 0，则不继续处理 */
	if( (w->box.graph.width <= 0 || w->box.graph.height <= 0) &&
	    (rect.width <= 0 || rect.height <= 0) ) {
		return;
	}
	if( w->style->sheet[key_height].type != SVT_AUTO ) {
		Widget_UpdateLayout( w );
	}
	/* 如果是绝对定位，且指定了右边距，或者垂直对齐方式不为顶部对齐 */
	if( (w->computed_style.position == SV_ABSOLUTE &&
	    w->style->sheet[key_right].is_valid && 
	    w->style->sheet[key_right].type != SVT_AUTO) ||
	    w->computed_style.vertical_align != SV_TOP ) {
		Widget_UpdatePosition( w );
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

/** 设置内边距 */
void Widget_SetPadding( LCUI_Widget w, int top, int right, int bottom, int left )
{
	SetStyle( w->custom_style, key_padding_top, top, px );
	SetStyle( w->custom_style, key_padding_right, right, px );
	SetStyle( w->custom_style, key_padding_bottom, bottom, px );
	SetStyle( w->custom_style, key_padding_left, left, px );
}

/** 设置外边距 */
void Widget_SetMargin( LCUI_Widget w, int top, int right, int bottom, int left )
{
	SetStyle( w->custom_style, key_margin_top, top, px );
	SetStyle( w->custom_style, key_margin_right, right, px );
	SetStyle( w->custom_style, key_margin_bottom, bottom, px );
	SetStyle( w->custom_style, key_margin_left, left, px );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Move( LCUI_Widget w, int left, int top )
{
	SetStyle( w->custom_style, key_top, top, px );
	SetStyle( w->custom_style, key_left, left, px );
	DEBUG_MSG("top = %d, left = %d\n", top, left);
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Resize( LCUI_Widget w, int width, int height )
{
	SetStyle( w->custom_style, key_width, width, px );
	SetStyle( w->custom_style, key_height, height, px );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Show( LCUI_Widget w )
{
	SetStyle( w->custom_style, key_visible, TRUE, int );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Hide( LCUI_Widget w )
{
	SetStyle( w->custom_style, key_visible, FALSE, int );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetBackgroundColor( LCUI_Widget w, LCUI_Color color )
{
	w->computed_style.background.color = color;
}

void Widget_SetDisabled( LCUI_Widget w, LCUI_BOOL disabled )
{
	w->disabled = disabled;
	if( w->disabled ) {
		Widget_AddStatus( w, "disabled" );
	} else {
		Widget_RemoveStatus( w, "disabled" );
	}
}

void Widget_Lock( LCUI_Widget w )
{
	LCUIMutex_Lock( &w->mutex );
}

void Widget_Unlock( LCUI_Widget w )
{
	LCUIMutex_Unlock( &w->mutex );
}

static int StrList_AddOne( char ***strlist, const char *str )
{
	int i = 0;
	char **newlist;

	if( !*strlist ) {
		newlist = (char**)malloc( sizeof(char*) * 2 );
		goto check_done;
	}
	for( i = 0; (*strlist)[i]; ++i ) {
		if( strcmp((*strlist)[i], str) == 0 ) {
			return 0;
		}
	}
	newlist = (char**)realloc( *strlist, (i+2)*sizeof(char*) );
check_done:
	if( !newlist ) {
		return 0;
	}
	newlist[i] = strdup(str);
	newlist[i+1] = NULL;
	*strlist = newlist;
	return 1;
}

int StrList_Add( char ***strlist, const char *str )
{
	char buff[256];
	int count = 0, i, head;
	for( head = 0, i = 0; str[i]; ++i ) {
		if( str[i] != ' ' ) {
			continue;
		}
		if( i - 1 > head ) {
			strncpy( buff, &str[head], i - head );
			buff[i - head] = 0;
			count += StrList_AddOne( strlist, buff );
		}
		head = i + 1;
	}
	if( i - 1 > head ) {
		strncpy( buff, &str[head], i - head );
		buff[i - head] = 0;
		count += StrList_AddOne( strlist, buff );
	}
	return count;
}

LCUI_BOOL StrList_Has( char **strlist, const char *str )
{
	int i;
	if( !strlist ) {
		return FALSE;
	}
	for( i = 0; strlist[i]; ++i ) {
		if( strcmp(strlist[i], str) == 0 ) {
			return TRUE;
		}
	}
	return FALSE;
}

static int StrList_RemoveOne( char ***strlist, const char *str )
{
	int i, pos, len;
	char **newlist;

	if( !*strlist ) {
		return 0;
	}
	for( pos = -1, i = 0; (*strlist)[i]; ++i ) {
		if( strcmp( (*strlist)[i], str ) == 0 ) {
			pos = i;
		}
	}
	if( pos == -1 ) {
		return 0;
	}
	if( pos == 0 && i < 2 ) {
		free( *strlist );
		*strlist = NULL;
		return 1;
	}
	len = i - 1;
	newlist = (char**)malloc( i * sizeof(char*) );
	for( i = 0; i < pos; ++i ) {
		newlist[i] = (*strlist)[i];
	}
	for( i = pos; i < len; ++i ) {
		newlist[i] = (*strlist)[i+1];
	}
	newlist[i] = NULL;
	free( (*strlist)[pos] );
	free( *strlist );
	*strlist = newlist;
	return 1;
}

int StrList_Remove( char ***strlist, const char *str )
{
	char buff[256];
	int count = 0, i, head;

	for( head = 0, i = 0; str[i]; ++i ) {
		if( str[i] != ' ' ) {
			continue;
		}
		if( i - 1 > head ) {
			strncpy( buff, &str[head], i - head );
			buff[i - head] = 0;
			count += StrList_RemoveOne( strlist, buff );
		}
		head = i + 1;
	}
	if( i - 1 > head ) {
		strncpy( buff, &str[head], i - head );
		buff[i - head] = 0;
		count += StrList_RemoveOne( strlist, buff );
	}
	return count;
}

/** 为部件添加一个类 */
int Widget_AddClass( LCUI_Widget w, const char *class_name )
{
	if( StrList_Has( w->classes, class_name ) ) {
		return 1;
	}
	if( StrList_Add(&w->classes, class_name) <= 0 ) {
		return 0;
	}
	Widget_HandleChildrenStyleChange( w, 0, class_name );
	Widget_UpdateStyle( w, TRUE );
	return 1;
}

/** 判断部件是否包含指定的类 */
LCUI_BOOL Widget_HasClass( LCUI_Widget w, const char *class_name )
{
	return StrList_Has( w->classes, class_name );
}

/** 从部件中移除一个类 */
int Widget_RemoveClass( LCUI_Widget w, const char *class_name )
{
	if( StrList_Has( w->classes, class_name ) ) {
		Widget_HandleChildrenStyleChange( w, 0, class_name );
	}
	StrList_Remove( &w->classes, class_name );
	Widget_UpdateStyle( w, TRUE );
	return 1;
}

/** 为部件添加一个状态 */
int Widget_AddStatus( LCUI_Widget w, const char *status_name )
{
	if( StrList_Has( w->status, status_name ) ) {
		return 0;
	}
	if( StrList_Add( &w->status, status_name ) <= 0 ) {
		return 0;
	}
	Widget_UpdateStyle( w, TRUE );
	Widget_HandleChildrenStyleChange( w, 1, status_name );
	return 1;
}

/** 判断部件是否包含指定的状态 */
LCUI_BOOL Widget_HasStatus( LCUI_Widget w, const char *status_name )
{
	return StrList_Has( w->status, status_name );
}

/** 从部件中移除一个状态 */
int Widget_RemoveStatus( LCUI_Widget w, const char *status_name )
{
	if( StrList_Has( w->status, status_name ) ) {
		Widget_HandleChildrenStyleChange( w, 1, status_name );
	}
	StrList_Remove( &w->status, status_name );
	Widget_UpdateStyle( w, TRUE );
	return 1;
}

int Widget_ComputeMaxWidth( LCUI_Widget w )
{
	LCUI_Style s;
	LCUI_Widget child;
	float scale = 1.0;
	int width, padding = 0;
	width = LCUIWidget.root->box.padding.width;
	for( child = w; child->parent; child = child->parent ) {
		s = &child->style->sheet[key_width];
		switch( s->type ) {
		case SVT_PX:
			width = s->val_px;
			break;
		case SVT_SCALE:
			scale *= s->val_scale;
			if( child == w ) {
				break;
			}
			padding += child->padding.left;
			padding += child->padding.right;
		case SVT_AUTO:
		default: continue;
		}
		break;
	}
	width = scale * width;
	width -= padding;
	if( width < 0 ) {
		width = 0;
	}
	return width;
}

void Widget_LockLayout( LCUI_Widget w )
{
	w->layout_locked = TRUE;
}

void Widget_UnlockLayout( LCUI_Widget w )
{
	w->layout_locked = FALSE;
}

void Widget_UpdateLayout( LCUI_Widget w )
{
	if( !w->layout_locked ) {
		Widget_AddTask( w, WTT_LAYOUT );
	}
}

void Widget_ExecUpdateLayout( LCUI_Widget w )
{
	struct {
		int x, y;
		int line_height;
		LCUI_Widget prev;
		int prev_display;
		int max_width;
	} ctx = { 0 };
	LCUI_Widget child;
	LCUI_WidgetEventRec e;
	LinkedListNode *node;

	ctx.max_width = Widget_ComputeMaxWidth( w );
	LinkedList_ForEach( node, &w->children ) {
		child = node->data;
		if( child->computed_style.position != SV_STATIC &&
		    child->computed_style.position != SV_RELATIVE ) {
			/* 如果部件还处于未准备完毕的状态 */
			if( child->state < WSTATE_READY ) {
				child->state |= WSTATE_LAYOUTED;
				/* 如果部件已经准备完毕则触发 ready 事件 */
				if( child->state == WSTATE_READY ) {
					LCUI_WidgetEventRec e;
					e.type = WET_READY;
					e.cancel_bubble = TRUE;
					Widget_TriggerEvent( child, &e, NULL );
					child->state = WSTATE_NORMAL;
				}
			}
			continue;
		}
		switch( child->computed_style.display ) {
		case SV_BLOCK:
			ctx.x = 0;
			if( ctx.prev && ctx.prev_display != SV_BLOCK ) {
				ctx.y += ctx.line_height;
			}
			child->origin_x = ctx.x;
			child->origin_y = ctx.y;
			ctx.line_height = child->box.outer.height;
			ctx.y += child->box.outer.height;
			break;
		case SV_INLINE_BLOCK:
			if( ctx.prev && ctx.prev_display == SV_BLOCK ) {
				ctx.x = 0;
				ctx.line_height = 0;
			}
			child->origin_x = ctx.x;
			ctx.x += child->box.outer.width;
			if( ctx.x > ctx.max_width ) {
				child->origin_x = 0;
				ctx.y += ctx.line_height;
				ctx.x = child->box.outer.width;
			}
			child->origin_y = ctx.y;
			if( child->box.outer.height > ctx.line_height ) {
				ctx.line_height = child->box.outer.height;
			}
			break;
		case SV_NONE:
		default: continue;
		}
		Widget_UpdatePosition( child );
		if( child->state < WSTATE_READY ) {
			child->state |= WSTATE_LAYOUTED;
			if( child->state == WSTATE_READY ) {
				LCUI_WidgetEventRec e;
				e.type = WET_READY;
				e.cancel_bubble = TRUE;
				Widget_TriggerEvent( child, &e, NULL );
				child->state = WSTATE_NORMAL;
			}
		}
		ctx.prev = child;
		ctx.prev_display = child->computed_style.display;
	}
	if( w->style->sheet[key_width].type == SVT_AUTO
	    || w->style->sheet[key_height].type == SVT_AUTO ) {
		Widget_AddTask( w, WTT_RESIZE );
	}
	e.cancel_bubble = TRUE;
	e.type = WET_AFTERLAYOUT;
	Widget_TriggerEvent( w, &e, NULL );
}

static void _LCUIWidget_PrintTree( LCUI_Widget w, int depth, const char *prefix )
{
	int i, len;
	LCUI_Widget child;
	LinkedListNode *node;
	char str[16], child_prefix[512], classes[512];

	len = strlen(prefix);
	strcpy( child_prefix, prefix );
	LinkedList_ForEach( node, &w->children ) {
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
		classes[0] = 0;
		if( child->classes ) {
			for( i = 0; child->classes[i]; ++i ) {
				strcat( classes, child->classes[i] );
				strcat( classes, " ");
			}
		} else {
			strcpy( classes, "(null)" );
		}
		printf( "%s%s type: %s, class: %s, xy:(%d,%d), size:(%d,%d), "
			"visible: %s, padding: (%d,%d,%d,%d), margin: (%d,%d,%d,%d)\n",
			prefix, str, child->type, classes, child->x, child->y,
			child->width, child->height,
			child->computed_style.visible ? "true" : "false",
			child->padding.top, child->padding.right, child->padding.bottom,
			child->padding.left, child->margin.top, child->margin.right, 
			child->margin.bottom, child->margin.left );

		_LCUIWidget_PrintTree( child, depth+1, child_prefix );
	}
}

void Widget_PrintTree( LCUI_Widget w )
{
	w = w ? w : LCUIWidget.root;
	printf("%s, xy:(%d,%d), size:(%d,%d), visible: %s\n", 
		w->type, w->x, w->y, w->width, w->height,
		w->computed_style.visible ? "true":"false");
	_LCUIWidget_PrintTree( w, 0, "  " );
}

static int CompareWidgetId( void *data, const void *keydata )
{
	return strcmp(((LCUI_Widget)data)->id, (const char*)keydata);
}

extern void LCUIWidget_AddTextView( void );
extern void LCUIWidget_AddButton( void );
extern void LCUIWidget_AddSideBar( void );
extern void LCUIWidget_AddTScrollBar( void );
extern void LCUIWidget_AddTextEdit( void );

void LCUI_InitWidget( void )
{
	LCUIWidget_InitTask();
	LCUIWidget_InitEvent();
	LCUIWidget_InitLibrary();
	LCUIWidget_InitStyle();
	LCUIWidget_AddTextView();
	LCUIWidget_AddButton();
	LCUIWidget_AddSideBar();
	LCUIWidget_AddTScrollBar();
	LCUIWidget_AddTextEdit();
	RBTree_Init( &LCUIWidget.ids );
	RBTree_SetDataNeedFree( &LCUIWidget.ids, FALSE );
	RBTree_OnJudge( &LCUIWidget.ids, CompareWidgetId );
	LCUIWidget.root = LCUIWidget_New("root");
	Widget_SetTitleW( LCUIWidget.root, L"LCUI Display" );
}

void LCUI_ExitWidget(void)
{

}
