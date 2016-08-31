/* ****//***********************************************************************
 * widget_task.c -- LCUI widget task module.
 *
 * Copyright (C) 2014-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2014-2016 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

typedef void (*callback)(LCUI_Widget);

/** 部件任务模块数据 */
static struct WidgetTaskModule {
	size_t count;
	int64_t timeout;
	LCUI_BOOL is_timeout;
	LinkedList trash;			/**< 待删除的部件列表 */
	callback handlers[WTT_TOTAL_NUM];	/**< 任务处理器 */
} self;

static void HandleRefreshStyle( LCUI_Widget w )
{
	Widget_ExecUpdateStyle( w, TRUE );
	w->task.buffer[WTT_UPDATE_STYLE] = FALSE;
}

static void HandleUpdateStyle( LCUI_Widget w )
{
	Widget_ExecUpdateStyle( w, FALSE );
}

static void HandleSetTitle( LCUI_Widget w )
{
	Widget_PostSurfaceEvent( w, WET_TITLE );
}

/** 处理主体刷新（标记主体区域为脏矩形，但不包括阴影区域） */
static void HandleBody( LCUI_Widget w )
{
	Widget_InvalidateArea( w, NULL, SV_BORDER_BOX );
}

/** 处理刷新（标记整个部件区域为脏矩形） */
static void HandleRefresh( LCUI_Widget w )
{
	DEBUG_MSG( "refresh\n" );
	Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
}

/** 更新当前任务状态，确保部件的任务能够被处理到 */
void Widget_UpdateTaskStatus( LCUI_Widget widget )
{
	int i;
	for( i = 0; i < WTT_TOTAL_NUM; ++i ) {
		if( widget->task.buffer[i] ) {
			widget->task.for_self = TRUE;
			Widget_AddTask( widget, widget->task.buffer[i] );
		}
	}
}

void Widget_AddTaskForChildren( LCUI_Widget widget, int task )
{
	LCUI_Widget child;
	LinkedListNode *node;
	widget->task.for_children = TRUE;
	for( LinkedList_Each( node, &widget->children ) ) {
		child = node->data;
		Widget_AddTask( child, task );
		Widget_AddTaskForChildren( child, task );
	}
}

void Widget_AddTask( LCUI_Widget widget, int task )
{
	if( widget->state == WSTATE_DELETED ) {
		return;
	}
	widget->task.for_self = TRUE;
	widget->task.buffer[task] = TRUE;
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while( widget && !widget->task.for_children ) {
		widget->task.for_children = TRUE;
		widget = widget->parent;
	}
}

/** 映射任务处理器 */
static void MapTaskHandler(void)
{
	self.handlers[WTT_VISIBLE] = Widget_UpdateVisibility;
	self.handlers[WTT_POSITION] = Widget_UpdatePosition;
	self.handlers[WTT_RESIZE] = Widget_UpdateSize;
	self.handlers[WTT_SHADOW] = Widget_UpdateBoxShadow;
	self.handlers[WTT_BORDER] = Widget_UpdateBorder;
	self.handlers[WTT_OPACITY] = Widget_UpdateOpacity;
	self.handlers[WTT_MARGIN] = Widget_UpdateMargin;
	self.handlers[WTT_BODY] = HandleBody;
	self.handlers[WTT_TITLE] = HandleSetTitle;
	self.handlers[WTT_REFRESH] = HandleRefresh;
	self.handlers[WTT_UPDATE_STYLE] = HandleUpdateStyle;
	self.handlers[WTT_REFRESH_STYLE] = HandleRefreshStyle;
	self.handlers[WTT_BACKGROUND] = Widget_UpdateBackground;
	self.handlers[WTT_LAYOUT] = Widget_ExecUpdateLayout;
	self.handlers[WTT_ZINDEX] = Widget_ExecUpdateZIndex;
	self.handlers[WTT_PROPS] = Widget_UpdateProps;
}

void LCUIWidget_InitTask( void )
{
	MapTaskHandler();
	self.timeout = 0;
	LinkedList_Init( &self.trash );
}

void LCUIWidget_ExitTask( void )
{
	LinkedList_Clear( &self.trash, NULL );
}

void Widget_AddToTrash( LCUI_Widget w )
{
	LinkedListNode *snode, *node;

	w->state = WSTATE_DELETED;
	if( !w->parent ) {
		return;
	}
	node = Widget_GetNode( w );
	snode = Widget_GetShowNode( w );
	LinkedList_Unlink( &w->parent->children, node );
	LinkedList_Unlink( &w->parent->children_show, snode );
	LinkedList_AppendNode( &self.trash, node );
}

int Widget_UpdateEx( LCUI_Widget w, LCUI_BOOL has_timeout )
{
	int i;
	LCUI_BOOL *buffer;
	LinkedListNode *node, *next;

	/* 如果该部件没有任务需要处理、或者被其它线程占用 */
	if( !w->task.for_self || LCUIMutex_TryLock( &w->mutex ) != 0 ) {
		goto proc_children_task;
	}
	w->task.for_self = FALSE;
	buffer = w->task.buffer;
	/* 如果有用户自定义任务 */
	if( buffer[WTT_USER] ) {
		LCUI_WidgetClass *wc;
		wc = LCUIWidget_GetClass( w->type );
		wc ? wc->task_handler( w ) : FALSE;
	}
	for( i = 0; i < WTT_USER; ++i ) {
		if( buffer[i] ) {
			buffer[i] = FALSE;
			if( self.handlers[i] ) {
				self.handlers[i]( w );
			}
		} else {
			buffer[i] = FALSE;
		}
	}
	LCUIMutex_Unlock( &w->mutex );
	/* 如果部件还处于未准备完毕的状态 */
	if( w->state < WSTATE_READY ) {
		w->state |= WSTATE_UPDATED;
		/* 如果部件已经准备完毕则触发 ready 事件 */
		if( w->state == WSTATE_READY ) {
			LCUI_WidgetEventRec e;
			e.type = WET_READY;
			e.cancel_bubble = TRUE;
			Widget_TriggerEvent( w, &e, NULL );
			w->state = WSTATE_NORMAL;
		}
	}
	self.count += 1;

proc_children_task:

	if( !w->task.for_children ) {
		return w->task.for_self;
	}
	/* 如果子级部件中有待处理的部件，则递归进去 */
	w->task.for_children = FALSE;
	node = w->children.head.next;
	while( node ) {
		LCUI_Widget child = node->data;
		/* 如果当前部件有销毁任务，结点空间会连同部件一起被
		 * 释放，为避免因访问非法空间而出现异常，预先保存下
		 * 个结点。
		 */
		next = node->next;
		/* 如果该级部件的任务需要留到下次再处理 */
		if(  Widget_UpdateEx( child, has_timeout ) ) {
			w->task.for_children = TRUE;
		}
		if( has_timeout ) {
			if( !self.is_timeout && self.count >= 50 ) {
				self.count = 0;
				if( LCUI_GetTime() >= self.timeout ) {
					self.is_timeout = TRUE;
				}
			}
			if( self.is_timeout ) {
				break;
			}
		}
		node = next;
	}
	return w->task.for_self || w->task.for_children;
}

void LCUIWidget_StepTask( void )
{
	LinkedListNode *node;
	self.is_timeout = FALSE;
	self.timeout = LCUI_GetTime() + 20;
	Widget_UpdateEx( LCUIWidget_GetRoot(), TRUE );
	/* 删除无用部件 */
	node = self.trash.head.next;
	while( node ) {
		LinkedListNode *next = node->next;
		LinkedList_Unlink( &self.trash, node );
		Widget_ExecDestroy( node->data );
		node = next;
	}
}
