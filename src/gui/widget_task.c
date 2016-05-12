/* ***************************************************************************
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

//#define DEBUG
#define __IN_WIDGET_TASK_SOURCE_FILE__

#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

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
	_DEBUG_MSG("widget: %s\n", w->type);
	Widget_PostSurfaceEvent( w, WET_TITLE );
}

/** 处理透明度 */
static void HandleOpacity( LCUI_Widget w )
{

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

/** 更新当前任务状态，确保部件的任务能够被处理到 */
void Widget_UpdateTaskStatus( LCUI_Widget widget )
{
	int i;
	for( i=0; i<WTT_TOTAL_NUM && !widget->task.for_self; ++i ) {
		if( widget->task.buffer[i] ) {
			widget->task.for_self = TRUE;
		}
	}
	if( !widget->task.for_self ) {
		return;
	}
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while( widget && !widget->task.for_children ) {
		widget->task.for_children = TRUE;
		widget = widget->parent;
	}
}

void Widget_AddTaskForChildren( LCUI_Widget widget, int task )
{
	LCUI_Widget child;
	LinkedListNode *node;
	widget->task.for_children = TRUE;
	LinkedList_ForEach( node, &widget->children ) {
		child = node->data;
		Widget_AddTask( child, task );
		Widget_AddTaskForChildren( child, task );
	}
}

static struct LCUIWidgetTaskModule {
	Dict *widget_dict;
	LinkedList widget_queue;
	LinkedList deleted_widgets;
	LCUI_Mutex mutex;
} self;

static unsigned int WidgetDict_HashFunction( const void *key )
{
	return (unsigned int)key;
}

static int WidgetDict_KeyCompare( void *privdata, const void *key1, 
				  const void *key2 )
{
	return 0;
}

typedef struct WidgetTaskRecordRec_ {
	LCUI_Widget widget;
	LinkedListNode node;
} WidgetTaskRecordRec, *WidgetTaskRecord;

static DictType WidgetDict = {
	WidgetDict_HashFunction, NULL, NULL,
	NULL, NULL, NULL
};

static void Widget_AddRecord( LCUI_Widget w )
{
	LinkedListNode *node;
	WidgetTaskRecord rec;
	LCUIMutex_Lock( &self.mutex );
	rec = Dict_FetchValue( self.widget_dict, w );
	if( rec ) {
		LCUIMutex_Unlock( &self.mutex );
		return;
	}
	node = Widget_GetShowNode( w );
	rec = malloc( sizeof( WidgetTaskRecordRec ) );
	rec->widget = w;
	rec->node.data = rec;
	rec->node.prev = NULL;
	rec->node.next = NULL;
	Dict_Add( self.widget_dict, w, rec );
	LinkedList_AppendNode( &self.widget_queue, &rec->node );
	LCUIMutex_Unlock( &self.mutex );
}

static void Widget_DeleteRecord( LCUI_Widget w )
{
	WidgetTaskRecord rec;
	LCUIMutex_Lock( &self.mutex );
	rec = Dict_FetchValue( self.widget_dict, w );
	if( !rec ) {
		LCUIMutex_Unlock( &self.mutex );
		return;
	}
	LinkedList_Unlink( &self.widget_queue, &rec->node );
	Dict_Delete( self.widget_dict, w );
	if( Dict_FetchValue( self.widget_dict, w ) ) {
		abort();
	}
	free( rec );
	LCUIMutex_Unlock( &self.mutex );
}

void Widget_AddTask( LCUI_Widget widget, int task )
{
	if( widget->deleted ) {
		return;
	}
	widget->task.buffer[task] = TRUE;
	Widget_AddRecord( widget );
	return;
	widget->task.for_self = TRUE;
	DEBUG_MSG("widget: %s, for_self: %d, for_childen: %d, task_id: %d\n",
	widget->type, widget->task.for_self, widget->task.for_children, task);
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while( widget && !widget->task.for_children ) {
		DEBUG_MSG("widget: %s\n", widget->type );
		widget->task.for_children = TRUE;
		widget = widget->parent;
	}
}

typedef void (*callback)(LCUI_Widget);

static callback task_handlers[WTT_TOTAL_NUM];

/** 映射任务处理器 */
static void MapTaskHandler(void)
{
	task_handlers[WTT_VISIBLE] = Widget_UpdateVisibility;
	task_handlers[WTT_POSITION] = Widget_UpdatePosition;
	task_handlers[WTT_RESIZE] = Widget_UpdateSize;
	task_handlers[WTT_SHADOW] = Widget_UpdateBoxShadow;
	task_handlers[WTT_BORDER] = Widget_UpdateBorder;
	task_handlers[WTT_OPACITY] = HandleOpacity;
	task_handlers[WTT_BODY] = HandleBody;
	task_handlers[WTT_TITLE] = HandleSetTitle;
	task_handlers[WTT_REFRESH] = HandleRefresh;
	task_handlers[WTT_UPDATE_STYLE] = HandleUpdateStyle;
	task_handlers[WTT_REFRESH_STYLE] = HandleRefreshStyle;
	task_handlers[WTT_CACHE_STYLE] = HandleCacheStyle;
	task_handlers[WTT_BACKGROUND] = Widget_ComputeBackgroundStyle;
	task_handlers[WTT_LAYOUT] = Widget_ExecUpdateLayout;
	task_handlers[WTT_ZINDEX] = Widget_ExecUpdateZIndex;
}

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_InitTask(void)
{
	MapTaskHandler();
	LinkedList_Init( &self.widget_queue );
	LinkedList_Init( &self.deleted_widgets );
	LCUIMutex_Init( &self.mutex );
	self.widget_dict = Dict_Create( &WidgetDict, NULL );
}

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_ExitTask(void)
{

}

/** 处理部件中当前积累的任务 */
int Widget_Update( LCUI_Widget w )
{
	int ret;
	LinkedListNode *node;

	if( w->deleted ) {
		return 0;
	}
	/* 如果该部件有任务需要处理 */
	ret = LCUIMutex_TryLock( &w->mutex );
	if( ret == 0 ) {
		int i;
		LCUI_BOOL *buffer;
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
				if( task_handlers[i] ) {
					task_handlers[i]( w );
				}
			} else {
				buffer[i] = FALSE;
			}
		}
		LCUIMutex_Unlock( &w->mutex );
	}
	/* 删除无用部件 */
	node = w->children_trash.head.next;
	while( node ) {
		LinkedListNode *next = node->next;
		LinkedList_Unlink( &w->children_trash, node );
		LinkedList_Append( &self.deleted_widgets, node->data );
		Widget_DeleteRecord( node->data );
		Widget_ExecDestroy( node->data );
		node = next;
	}
	return ret;
}

/** 处理一次当前积累的部件任务 */
void LCUIWidget_StepTask(void)
{
	int ret;
	WidgetTaskRecord rec;
	LinkedListNode *node, *prev;
	prev = &self.widget_queue.head;
	node = self.widget_queue.head.next;
	while( node ) {
		rec = node->data;
		ret = Widget_Update( rec->widget );
		if( ret == 0 ) {
			Widget_DeleteRecord( rec->widget );
			node = prev->next;
		} else {
			node = node->next;
		}
	}
}
