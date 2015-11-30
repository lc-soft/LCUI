/* ***************************************************************************
 * widget_task.c -- LCUI widget task module.
 *
 * Copyright (C) 2014-2015 by Liu Chao <lc-soft@live.cn>
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
 * widget_task.c -- LCUI部件任务处理模块
 *
 * 版权所有 (C) 2014-2015 归属于 刘超 <lc-soft@live.cn>
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
#define __IN_WIDGET_TASK_SOURCE_FILE__

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/gui/widget.h>

#undef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))

struct LCUI_WidgetTaskBoxRec_ {
	LCUI_BOOL	for_self;			/**< 标志，指示当前部件是否有待处理的任务 */
	LCUI_BOOL	for_children;			/**< 标志，指示是否有待处理的子级部件 */
	LCUI_BOOL	buffer[WTT_TOTAL_NUM];		/**< 记录缓存 */
};

static void HandleTopLevelWidgetEvent( LCUI_Widget w, int event_type )
{
	if( w->parent == LCUIRootWidget || w == LCUIRootWidget ) {
		int *n;
		LCUI_WidgetEvent e;

		n = (int*)&event_type;
		e.type_name = "TopLevelWidget";
		e.target = w;
		DEBUG_MSG("widget: %s, post event: %d\n", w->type,event_type );
		Widget_PostEvent( LCUIRootWidget, &e, *((int**)n) );
	}
}

/** 计算边框样式 */
static void ComputeBorderStyle( LCUI_StyleSheet ss, LCUI_Border *b )
{
	LCUI_Style *style;
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

/** 计算矩形阴影样式 */
static void ComputeBoxShadowStyle( LCUI_StyleSheet ss, LCUI_BoxShadow *bsd )
{
	LCUI_Style *style;
	int key = key_box_shadow_start + 1;

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

static void HandleRefreshStyle( LCUI_Widget w )
{
	Widget_Update( w, TRUE );
	w->task->buffer[WTT_UPDATE_STYLE] = FALSE;
	w->task->buffer[WTT_CACHE_STYLE] = TRUE;
}

static void HandleUpdateStyle( LCUI_Widget w )
{
	Widget_Update( w, FALSE );
	w->task->buffer[WTT_CACHE_STYLE] = TRUE;
}

static void HandleCacheStyle( LCUI_Widget w )
{
	ClearStyleSheet( w->cached_style );
	ReplaceStyleSheet( w->cached_style, w->style );
}

/** 处理位置变化 */
static void HandlePosition( LCUI_Widget w )
{
	LCUI_Rect rect;
	rect = w->box.graph;
	Widget_ComputePosition( w );
	if( w->parent ) {
		DEBUG_MSG("new-rect: %d,%d,%d,%d\n", w->box.graph.x, w->box.graph.y, w->box.graph.w, w->box.graph.h);
		DEBUG_MSG("old-rect: %d,%d,%d,%d\n", rect.x, rect.y, rect.w, rect.h);
		/* 标记移动前后的区域 */
		Widget_InvalidateArea( w->parent, &w->box.graph, SV_CONTENT_BOX );
		Widget_InvalidateArea( w->parent, &rect, SV_CONTENT_BOX );
	}
	/* 检测是否为顶级部件并做相应处理 */
	HandleTopLevelWidgetEvent( w, WET_MOVE );
}

static void HandleSetTitle( LCUI_Widget w )
{
	_DEBUG_MSG("widget: %s\n", w->type);
	HandleTopLevelWidgetEvent( w, WET_TITLE );
}

/** 处理尺寸调整 */
static void HandleResize( LCUI_Widget w )
{
	int i;
	LCUI_Rect rect;
	LCUI_WidgetEvent e;
	struct { 
		LCUI_Style *sval;
		int *ival;
		int key;
	} pd_map[4] = {
		{ &w->computed_style.padding.top, &w->padding.top, key_padding_top },
		{ &w->computed_style.padding.right, &w->padding.right, key_padding_right },
		{ &w->computed_style.padding.bottom, &w->padding.bottom, key_padding_bottom },
		{ &w->computed_style.padding.left, &w->padding.left, key_padding_left }
	};
	rect = w->box.graph;
	/* 从样式表中获取尺寸 */
	w->computed_style.width = w->style->sheet[key_width];
	w->computed_style.height = w->style->sheet[key_height];
	/* 内边距的单位暂时都用 px  */
	for( i=0; i<4; ++i ) {
		if( !w->style->sheet[pd_map[i].key].is_valid
		 || w->style->sheet[pd_map[i].key].type != SVT_PX ) {
			pd_map[i].sval->type = SVT_PX;
			pd_map[i].sval->px = 0;
			*pd_map[i].ival = 0;
			continue;
		}
		*pd_map[i].sval = w->style->sheet[pd_map[i].key];
		*pd_map[i].ival = pd_map[i].sval->px;
	}
	Widget_ComputeSize( w );
	Widget_UpdateGraphBox( w );
	if( rect.width == w->box.graph.width
	 && rect.height == w->box.graph.height ) {
		return;
	}
	if( w->computed_style.width.type != SVT_AUTO ) {
		Widget_AddTask( w, WTT_LAYOUT );
	}
	if( w->parent ) {
		Widget_InvalidateArea( w->parent, &rect, SV_CONTENT_BOX );
		rect.width = w->box.graph.width;
		rect.height = w->box.graph.height;
		Widget_InvalidateArea( w->parent, &rect, SV_CONTENT_BOX );	
		if( w->parent->computed_style.width.type == SVT_AUTO
		 || w->parent->computed_style.height.type == SVT_AUTO ) {
			Widget_AddTask( w->parent, WTT_RESIZE );
		}
		if( w->computed_style.display != SV_NONE
		 && w->computed_style.position == SV_STATIC ) {
			Widget_AddTask( w->parent, WTT_LAYOUT );
		}
	}
	e.type_name = "resize";
	e.type = WET_RESIZE;
	e.cancel_bubble = TRUE;
	e.target = w;
	e.data = NULL;
	Widget_SendEvent( w, &e, NULL );
	Widget_AddTask( w, WTT_REFRESH );
	HandleTopLevelWidgetEvent( w, WET_RESIZE );
}

/** 处理可见性 */
static void HandleVisibility( LCUI_Widget w )
{
	LCUI_Style *s;
	LCUI_BOOL visible = TRUE;

	s = &w->cached_style->sheet[key_visible];
	if( w->computed_style.display == SV_NONE
	 || (s->is_valid && !s->value) ) {
		visible = FALSE;
	}
	s = &w->style->sheet[key_display];
	if( w->style->sheet[key_display].is_valid ) {
		w->computed_style.display = s->style;
	} else {
		w->computed_style.display = SV_BLOCK;
	}
	w->computed_style.visible = TRUE;
	s = &w->style->sheet[key_visible];
	if( w->computed_style.display == SV_NONE
	|| (s->is_valid && !s->value) ) {
		w->computed_style.visible = FALSE;
	}
	if( w->computed_style.visible == visible ) {
		return;
	}
	visible = w->computed_style.visible;
	if( w->parent ) {
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
		if( (w->computed_style.display == SV_BLOCK
		 || w->computed_style.display == SV_INLINE_BLOCK)
		 && w->computed_style.position == SV_STATIC
		 && w->computed_style.float_mode == SV_NONE ) {
			Widget_AddTask( w->parent, WTT_LAYOUT );
		}
	}
	DEBUG_MSG("visible: %s\n", visible ? "TRUE":"FALSE");
	HandleTopLevelWidgetEvent( w, visible ? WET_SHOW:WET_HIDE );
}

/** 处理透明度 */
static void HandleOpacity( LCUI_Widget w )
{

}

/** 处理阴影（标记阴影区域为脏矩形，但不包括主体区域） */
static void HandleShadow( LCUI_Widget w )
{
	LCUI_BoxShadow bs;

	_DEBUG_MSG("update shadow\n");
	bs = w->computed_style.shadow;
	ComputeBoxShadowStyle( w->style, &w->computed_style.shadow );
	/* 如果阴影变化并未导致图层尺寸变化，则只重绘阴影 */
	if( bs.x == w->computed_style.shadow.x && bs.y == w->computed_style.shadow.y
	 && bs.spread == w->computed_style.shadow.spread ) {
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

/** 处理主体刷新（标记主体区域为脏矩形，但不包括阴影区域） */
static void HandleBody( LCUI_Widget w )
{
	DEBUG_MSG( "body\n" );
	Widget_InvalidateArea( w, NULL, SV_BORDER_BOX );
}

/** 处理刷新（标记整个部件区域为脏矩形） */
static void HandleRefresh( LCUI_Widget w )
{
	DEBUG_MSG( "refresh\n" );
	Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
}

static void HandleDestroy( LCUI_Widget w )
{
	Widget_ExecDestroy( &w );
}

static void HandleBorder( LCUI_Widget w )
{
	LCUI_Rect rect;
	LCUI_Border ob, *nb;
	
	ob = w->computed_style.border;
	ComputeBorderStyle( w->style, &w->computed_style.border );
	nb = &w->computed_style.border;
	/* 如果边框变化并未导致图层尺寸变化的话，则只重绘边框 */
	if( ob.top.width != nb->top.width
	 || ob.right.width != nb->right.width
	 || ob.bottom.width != nb->bottom.width
	 || ob.left.width != nb->left.width ) {
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

/** 更新当前任务状态，确保部件的任务能够被处理到 */
void Widget_UpdateTaskStatus( LCUI_Widget widget )
{
	int i;
	for( i=0; i<WTT_TOTAL_NUM && !widget->task->for_self; ++i ) {
		if( widget->task->buffer[i] ) {
			widget->task->for_self = TRUE;
		}
	}
	if( !widget->task->for_self ) {
		return;
	}
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while( widget && !widget->task->for_children ) {
		widget->task->for_children = TRUE;
		widget = widget->parent;
	}
}

/** 添加任务并扩散到子级部件 */
void Widget_AddTaskToSpread( LCUI_Widget widget, int task_type )
{
	LinkedListNode *node;
	widget->task->buffer[task_type] = TRUE;
	widget->task->for_self = TRUE;
	widget->task->for_children = TRUE;
	LinkedList_ForEach( node, &widget->children ) {
		Widget_AddTaskToSpread( node->data, task_type );
	}
}

/** 添加任务 */
void Widget_AddTask( LCUI_Widget widget, int task_type )
{
	widget->task->buffer[task_type] = TRUE;
	widget->task->for_self = TRUE;
	DEBUG_MSG("widget: %s, for_self: %d, for_childen: %d, task_id: %d\n",
	widget->type, widget->task->for_self, widget->task->for_children, task_type);
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while( widget && !widget->task->for_children ) {
		DEBUG_MSG("widget: %s\n", widget->type );
		widget->task->for_children = TRUE;
		widget = widget->parent;
	}
}

typedef void (*callback)(LCUI_Widget);

static callback task_handlers[WTT_TOTAL_NUM];

/** 映射任务处理器 */
static void MapTaskHandler(void)
{
	task_handlers[WTT_DESTROY] = HandleDestroy;
	task_handlers[WTT_VISIBLE] = HandleVisibility;
	task_handlers[WTT_POSITION] = HandlePosition;
	task_handlers[WTT_RESIZE] = HandleResize;
	task_handlers[WTT_SHADOW] = HandleShadow;
	task_handlers[WTT_BORDER] = HandleBorder;
	task_handlers[WTT_OPACITY] = HandleOpacity;
	task_handlers[WTT_BODY] = HandleBody;
	task_handlers[WTT_TITLE] = HandleSetTitle;
	task_handlers[WTT_REFRESH] = HandleRefresh;
	task_handlers[WTT_UPDATE_STYLE] = HandleUpdateStyle;
	task_handlers[WTT_REFRESH_STYLE] = HandleRefreshStyle;
	task_handlers[WTT_CACHE_STYLE] = HandleCacheStyle;
	task_handlers[WTT_BACKGROUND] = Widget_ComputeBackgroundStyle;
	task_handlers[WTT_LAYOUT] = Widget_UpdateLayout;
}

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_InitTask(void)
{
	MapTaskHandler();
}

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_ExitTask(void)
{

}

/** 初始化部件的任务处理 */
void Widget_InitTaskBox( LCUI_Widget widget )
{
	int i;
	widget->task = (LCUI_WidgetTaskBox)
	malloc(sizeof(struct LCUI_WidgetTaskBoxRec_));
	widget->task->for_children = FALSE;
	widget->task->for_self = FALSE;
	for( i=0; i<WTT_TOTAL_NUM; ++i ) {
		widget->task->buffer[i] = FALSE;
	}
}

/** 销毁（释放）部件的任务处理功能的相关资源 */
void Widget_DestroyTaskBox( LCUI_Widget widget )
{
	free( widget->task );
	widget->task = NULL;
}

/** 处理部件的各种任务 */
static int Widget_ProcTask( LCUI_Widget w )
{
	int ret = 1, i;
	LCUI_BOOL *buffer;
	LCUI_Widget child;
	LinkedListNode *node, *next;

	DEBUG_MSG("1,widget: %s, for_self: %d, for_children: %d\n",
		   w->type, w->task->for_self, w->task->for_children);
	/* 如果该部件有任务需要处理 */
	if( w->task->for_self ) {
		ret = LCUIMutex_TryLock( &w->mutex );
		if( ret != 0 ) {
			ret = 1;
			goto skip_proc_self_task;
		}
		w->task->for_self = FALSE;
		buffer = w->task->buffer;
		/* 如果该部件需要销毁，其它任务就不用再处理了 */
		if( buffer[WTT_DESTROY] ) {
			buffer[WTT_DESTROY] = FALSE;
			task_handlers[WTT_DESTROY]( w );
			return -1;
		}
		/* 如果有用户自定义任务 */
		if( buffer[WTT_USER] ) {
			LCUI_WidgetClass *wc;
			wc = LCUIWidget_GetClass( w->type );
			wc ? wc->task_handler( w ):FALSE;
		}
		for( i=0; i<WTT_USER; ++i ) {
			DEBUG_MSG("task_id: %d, is_valid: %d\n", i, buffer[i]);
			if( buffer[i] ) {
				buffer[i] = FALSE;
				if( task_handlers[i] ) {
					task_handlers[i]( w );
				}
			} else {
				buffer[i] = FALSE;
			}
		}
		LCUIMutex_Unlock( &w->mutex );
	}

skip_proc_self_task:;

	/* 如果子级部件中有待处理的部件，则递归进去 */
	if( w->task->for_children ) {
		w->task->for_children = FALSE;
		node = w->children.head.next;
		while( node ) {
			child = node->data;
			/* 如果当前部件有销毁任务，结点空间会连同部件一起被
			 * 释放，为避免因访问非法空间而出现异常，预先保存下
			 * 个结点。
			 */
			next = node->next;
			ret = Widget_ProcTask( child );
			/* 如果该级部件的任务需要留到下次再处理 */
			if( ret == 1 ) {
				w->task->for_children = TRUE;
			}
			node = next;
		}
	}
	DEBUG_MSG("2,widget: %s, for_self: %d, for_children: %d\n",
		   w->type, w->task->for_self, w->task->for_children);
	return (w->task->for_self || w->task->for_children) ? 1:0;
}

/** 处理一次当前积累的部件任务 */
void LCUIWidget_StepTask(void)
{
	Widget_ProcTask( LCUIRootWidget );
}
