/* ***************************************************************************
 * widget_task.c -- LCUI widget task module.
 * 
 * Copyright (C) 2014 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2014 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/widget_build.h>

typedef struct TaskRecord {
	LCUI_BOOL is_valid;			/**< 是否有任务 */
	LCUI_WidgetTask data;			/**< 任务数据 */
} TaskRecord;	/**< 任务记录 */

typedef struct TaskNode {
	LCUI_Widget target;			/**< 目标部件 */
	TaskRecord tasks[WTT_TOTAL_NUM];	/**< 任务记录 */
} TaskNode;	/**< 任务结点 */

static struct {
	int n;				/**< 当前使用的记录 */
	LCUI_RBTree buffer[2];		/**< 两个记录缓存 */
} self;		/**< 模块自身所需的数据 */

/** 处理位置移动 */
static void handleMove( LCUI_Widget w, LCUI_WidgetTask *t )
{
	LCUI_Rect rect;
	Widget_GetRect( w, &rect );
	Widget_InvalidateArea( w->parent, &rect );
	rect.x = t->move.x;
	rect.y = t->move.y;
	Widget_InvalidateArea( w->parent, &rect );
}

/** 处理尺寸调整 */
static void handleResize( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理可见性 */
static void handleVisibility( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理透明度 */
static void handleOpacity( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理阴影（标记阴影区域为脏矩形，但不包括主体区域） */
static void handleShadow( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理主体刷新（标记主体区域为脏矩形，但不包括阴影区域） */
static void handleBody( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理刷新（标记整个部件区域为脏矩形） */
static void handleRefresh( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

static int CustomKeyJudge( void *node_data, const void *key_data )
{
	return ((TaskNode*)node_data)->target > (const LCUI_Widget)key_data;
}

/** 添加任务 */
int Widget_AddTask( LCUI_Widget widget, LCUI_WidgetTask *data )
{
	TaskNode *tn;
	LCUI_RBTree *buffer = &self.buffer[self.n == 1 ? 0:1];
	LCUI_RBTreeNode *node = RBTree_CustomSearch( buffer, widget );

	if( node ) {
		tn = (TaskNode*)node->data;
	} else {
		int i;
		tn = (TaskNode*)malloc(sizeof(TaskNode));
		tn->target = widget;
		for( i=0; i<WTT_TOTAL_NUM; ++i ) {
			tn->tasks[i].is_valid = FALSE;
		}
		node = RBTree_CustomInsert( buffer, widget, tn );
	}
	tn->tasks[data->type].is_valid = TRUE;
	tn->tasks[data->type].data = *data;

	return 0;
}

typedef void (*callback)(LCUI_Widget, LCUI_WidgetTask*);

static callback task_handlers[WTT_TOTAL_NUM];

/** 映射任务处理器 */
static void MapTaskHandler(void)
{
	/** 不能用C99标准中的初始化方式真蛋疼... */
	task_handlers[WTT_SHOW] = handleVisibility;
	task_handlers[WTT_MOVE] = handleMove;
	task_handlers[WTT_RESIZE] = handleResize;
	task_handlers[WTT_SHADOW] = handleShadow;
	task_handlers[WTT_OPACITY] = handleOpacity;
	task_handlers[WTT_BODY] = handleBody;
	task_handlers[WTT_REFRESH] = handleRefresh;
}

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_Task_Init(void)
{
	self.n = 0;
	RBTree_Init( &self.buffer[0] );
	RBTree_Init( &self.buffer[1] );
	RBTree_OnJudge( &self.buffer[0], CustomKeyJudge );
	RBTree_OnJudge( &self.buffer[1], CustomKeyJudge );
	MapTaskHandler();
}

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_Task_Destroy(void)
{
	RBTree_Destroy( &self.buffer[0] );
	RBTree_Destroy( &self.buffer[1] );
}

/** 处理一次当前积累的部件任务 */
void LCUIWidget_Task_Step(void)
{
	int i;
	LCUI_RBTree *buffer;
	LCUI_RBTreeNode *node;
	callback func;
	TaskNode *tn;
	/* 切换前后台记录 */
	self.n = self.n == 1 ? 0:1;
	buffer = &self.buffer[self.n];
	node = RBTree_First( buffer );
	while( node ) {
		tn = (TaskNode*)node->data;
		for( i=0; i<WTT_TOTAL_NUM; ++i ) {
			if( !tn->tasks[i].is_valid ) {
				continue;
			}
			if( tn->tasks[i].data.type >= WTT_USER ) {
				LCUI_WidgetClass *wc;
				wc = LCUIWidget_GetClass( tn->target->type_name );
				func = wc->task_handler;
			} else {
				func = task_handlers[tn->tasks[i].data.type];
			}
			func ? func(tn->target, &tn->tasks[i].data) : FALSE;
			tn->tasks[i].is_valid = FALSE;
		}
		node = RBTree_Next( node );
		RBTree_CustomErase( buffer, tn->target );
	}
}
