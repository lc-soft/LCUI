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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

typedef void (*callback)(LCUI_Widget);

/** 部件任务索引记录 */
typedef struct WidgetTaskIndexRec_ {
	LCUI_Widget widget;		/**< 部件 */
	LinkedListNode node;		/**< 任务数据结点 */
} WidgetTaskIndexRec, *WidgetTaskIndex;

/** 部件任务模块数据 */
static struct WidgetTaskModule {
	LinkedList tasks;			/**< 任务队列 */
	LCUI_RBTree indexes;			/**< 索引记录 */
	LCUI_Mutex mutex;			/**< 互斥锁 */
	LinkedList trash;			/**< 待删除的部件列表 */
	callback handlers[WTT_TOTAL_NUM];	/**< 任务处理器 */
} self;

static void HandleRefreshStyle( LCUI_Widget w )
{
	Widget_ExecUpdateStyle( w, TRUE );
	w->task.buffer[WTT_UPDATE_STYLE] = FALSE;
	w->task.buffer[WTT_CACHE_STYLE] = TRUE;
}

static void HandleUpdateStyle( LCUI_Widget w )
{
	Widget_ExecUpdateStyle( w, FALSE );
	w->task.buffer[WTT_CACHE_STYLE] = TRUE;
}

static void HandleCacheStyle( LCUI_Widget w )
{
	StyleSheet_Clear( w->cached_style );
	StyleSheet_Replace( w->cached_style, w->style );
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
	for( LinkedList_Each( node, &widget->children ) ) {
		child = node->data;
		Widget_AddTask( child, task );
		Widget_AddTaskForChildren( child, task );
	}
}

void LCUIWidget_ClearTaskTarget( LCUI_Widget w )
{
	WidgetTaskIndex idx;
	LCUIMutex_Lock( &self.mutex );
	idx = RBTree_CustomGetData( &self.indexes, w );
	if( idx ) {
		LinkedList_Unlink( &self.tasks, &idx->node );
		RBTree_CustomErase( &self.indexes, w );
		free( idx );
	}
	LCUIMutex_Unlock( &self.mutex );
}

void Widget_AddTask( LCUI_Widget widget, int task )
{
	if( widget->state == WSTATE_DELETED ) {
		return;
	}
	widget->task.for_self = TRUE;
	widget->task.buffer[task] = TRUE;
	LCUIMutex_Lock( &self.mutex );
	if( !RBTree_CustomGetData( &self.indexes, widget ) ) {
		WidgetTaskIndex idx = NEW( WidgetTaskIndexRec, 1 );
		idx->widget = widget;
		idx->node.data = widget;
		RBTree_CustomInsert( &self.indexes, widget, idx );
		LinkedList_AppendNode( &self.tasks, &idx->node );
	}
	LCUIMutex_Unlock( &self.mutex );
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
	self.handlers[WTT_BODY] = HandleBody;
	self.handlers[WTT_TITLE] = HandleSetTitle;
	self.handlers[WTT_REFRESH] = HandleRefresh;
	self.handlers[WTT_UPDATE_STYLE] = HandleUpdateStyle;
	self.handlers[WTT_REFRESH_STYLE] = HandleRefreshStyle;
	self.handlers[WTT_CACHE_STYLE] = HandleCacheStyle;
	self.handlers[WTT_BACKGROUND] = Widget_UpdateBackground;
	self.handlers[WTT_LAYOUT] = Widget_ExecUpdateLayout;
	self.handlers[WTT_ZINDEX] = Widget_ExecUpdateZIndex;
	self.handlers[WTT_PROPS] = Widget_UpdateProps;
}

static int OnCompareIndex( void *data, const void *keydata )
{
	return ((WidgetTaskIndex)data)->widget - (LCUI_Widget)keydata;
}

void LCUIWidget_InitTask( void )
{
	MapTaskHandler();
	LCUIMutex_Init( &self.mutex );
	LinkedList_Init( &self.tasks );
	LinkedList_Init( &self.trash );
	RBTree_Init( &self.indexes );
	RBTree_OnCompare( &self.indexes, OnCompareIndex );
}

void LCUIWidget_ExitTask( void )
{
	LCUIMutex_Lock( &self.mutex );
	LinkedList_Clear( &self.tasks, NULL );
	RBTree_Destroy( &self.indexes );
	LCUIMutex_Unlock( &self.mutex );
	LCUIMutex_Destroy( &self.mutex );
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

int Widget_Update( LCUI_Widget w )
{
	int i;
	LCUI_BOOL *buffer;
	/* 如果该部件有任务需要处理 */
	if( w->task.for_self ) {
		if( LCUIMutex_TryLock( &w->mutex ) != 0 ) {
			return 1;
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
	}
	return w->task.for_self;
}

void LCUIWidget_StepTask( void )
{
	int i;
	WidgetTaskIndex idx;
	LinkedListNode *prev, *node;
	int64_t current_time = LCUI_GetTime();
	int64_t end_time = current_time + 30;
	size_t offset = offsetof( WidgetTaskIndexRec, node );

	while( current_time < end_time ) {
		i = 0, node = NULL;
		LCUIMutex_Lock( &self.mutex );
		for( LinkedList_Each( node, &self.tasks ) ) {
			if( i >= 50 ) {
				break;
			}
			prev = node->prev;
			if( Widget_Update( node->data ) ) {
				i += 1;
				continue;
			}
			idx = (WidgetTaskIndex)((char*)node - offset);
			RBTree_CustomErase( &self.indexes, node->data );
			LinkedList_Unlink( &self.tasks, node );
			free( idx );
			node = prev;
			i += 1;
		}
		/* 删除无用部件 */
		node = self.trash.head.next;
		while( node && node != &self.trash.head ) {
			prev = node->prev;
			LinkedList_Unlink( &self.trash, node );
			Widget_ExecDestroy( node->data );
			node = prev;
		}
		LCUIMutex_Unlock( &self.mutex );
		if( !node ) {
			break;
		}
		current_time = LCUI_GetTime();
	}
}
