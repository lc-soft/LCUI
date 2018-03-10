/*
 * widget_task.c -- LCUI widget task module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

/** 部件任务模块数据 */
static struct WidgetTaskModule {
	LinkedList trash;				/**< 待删除的部件列表 */
	LCUI_WidgetFunction handlers[LCUI_WTASK_TOTAL_NUM];	/**< 任务处理器 */
	unsigned int update_count;			/**< 刷新次数 */
} self;

static void HandleRefreshStyle( LCUI_Widget w )
{
	Widget_ExecUpdateStyle( w, TRUE );
	w->task.buffer[LCUI_WTASK_UPDATE_STYLE] = FALSE;
}

static void HandleUpdateStyle( LCUI_Widget w )
{
	Widget_ExecUpdateStyle( w, FALSE );
}

static void HandleSetTitle( LCUI_Widget w )
{
	Widget_PostSurfaceEvent( w, LCUI_WEVENT_TITLE, TRUE );
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

void Widget_UpdateTaskStatus( LCUI_Widget widget )
{
	int i;
	for( i = 0; i < LCUI_WTASK_TOTAL_NUM; ++i ) {
		if( widget->task.buffer[i] ) {
			break;
		}
	}
	if( i >= LCUI_WTASK_TOTAL_NUM ) {
		return;
	}
	widget->task.for_self = TRUE;
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
	for( LinkedList_Each( node, &widget->children ) ) {
		child = node->data;
		Widget_AddTask( child, task );
		Widget_AddTaskForChildren( child, task );
	}
}

void Widget_AddTask( LCUI_Widget widget, int task )
{
	if( widget->state == LCUI_WSTATE_DELETED ) {
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

static void MapTaskHandler(void)
{
#define SetHandler(NAME, HANDLER) self.handlers[LCUI_WTASK_##NAME] = HANDLER
	SetHandler( VISIBLE, Widget_UpdateVisibility );
	SetHandler( POSITION, Widget_UpdatePosition );
	SetHandler( RESIZE, Widget_UpdateSize );
	SetHandler( RESIZE_WITH_SURFACE, Widget_UpdateSizeWithSurface );
	SetHandler( SHADOW, Widget_UpdateBoxShadow );
	SetHandler( BORDER, Widget_UpdateBorder );
	SetHandler( OPACITY, Widget_UpdateOpacity );
	SetHandler( MARGIN, Widget_UpdateMargin );
	SetHandler( BODY, HandleBody );
	SetHandler( TITLE, HandleSetTitle );
	SetHandler( REFRESH, HandleRefresh );
	SetHandler( UPDATE_STYLE, HandleUpdateStyle );
	SetHandler( REFRESH_STYLE, HandleRefreshStyle );
	SetHandler( BACKGROUND, Widget_UpdateBackground );
	SetHandler( LAYOUT, Widget_ExecUpdateLayout );
	SetHandler( ZINDEX, Widget_ExecUpdateZIndex );
	SetHandler( DISPLAY, Widget_UpdateDisplay );
	SetHandler( PROPS, Widget_UpdateProps );
}

static void LCUIWidget_ClearTrash( void )
{
	LinkedListNode *node;
	node = self.trash.head.next;
	while( node ) {
		LinkedListNode *next = node->next;
		LinkedList_Unlink( &self.trash, node );
		Widget_ExecDestroy( node->data );
		node = next;
	}
}

void LCUIWidget_InitTasks( void )
{
	MapTaskHandler();
	LinkedList_Init( &self.trash );
}

void LCUIWidget_FreeTasks( void )
{
	LCUIWidget_ClearTrash();
}

void Widget_AddToTrash( LCUI_Widget w )
{
	LCUI_WidgetEventRec e = { 0 };
	e.type = LCUI_WEVENT_UNLINK;
	w->state = LCUI_WSTATE_DELETED;
	Widget_TriggerEvent( w, &e, NULL );
	if( !w->parent ) {
		return;
	}
	LinkedList_Unlink( &w->parent->children, &w->node );
	LinkedList_Unlink( &w->parent->children_show, &w->node_show );
	LinkedList_AppendNode( &self.trash, &w->node );
	Widget_PostSurfaceEvent( w, LCUI_WEVENT_UNLINK, TRUE );
}

int Widget_Update( LCUI_Widget w )
{
	int i;
	LCUI_BOOL *buffer;
	LinkedListNode *node, *next;

	/* 如果该部件没有任务需要处理 */
	if( !w->task.for_self ) {
		goto proc_children_task;
	}
	w->task.for_self = FALSE;
	buffer = w->task.buffer;
	/* 如果有用户自定义任务 */
	if( buffer[LCUI_WTASK_USER] && w->proto && w->proto->runtask ) {
		w->proto->runtask( w );
	}
	for( i = 0; i < LCUI_WTASK_USER; ++i ) {
		if( buffer[i] ) {
			buffer[i] = FALSE;
			if( self.handlers[i] ) {
				self.handlers[i]( w );
			}
		} else {
			buffer[i] = FALSE;
		}
	}
	Widget_AddState( w, LCUI_WSTATE_UPDATED );

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
		if(  Widget_Update( child ) ) {
			w->task.for_children = TRUE;
		}
		node = next;
	}
	return w->task.for_self || w->task.for_children;
}

void LCUIWidget_Update( void )
{
	int count = 0;
	LCUI_Widget root;
	/* 前两次更新需要主动刷新所有部件的样式，主要是为了省去在应用程序里手动调用
	 * LCUIWidget_RefreshStyle() 的麻烦 */
	if( self.update_count < 2 ) {
		LCUIWidget_RefreshStyle();
		self.update_count += 1;
	}
	root = LCUIWidget_GetRoot();
	while( Widget_Update( root ) && count++ < 5 );
	LCUIWidget_ClearTrash();
}

void LCUIWidget_RefreshStyle( void )
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	Widget_UpdateStyle( root, TRUE );
	Widget_AddTaskForChildren( root, LCUI_WTASK_REFRESH_STYLE );
}
