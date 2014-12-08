/* ***************************************************************************
 * widget_task.c -- LCUI widget TaskRecord module.
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

static int CustomKeyJudge( void *node_data, const void *key_data )
{
	return ((TaskNode*)node_data)->target > (const LCUI_Widget)key_data;
}

/** 添加任务 */
int Widget_AddTask( LCUI_Widget widget, LCUI_WidgetTask *data )
{
	TaskNode *task;
	LCUI_RBTree *buffer = &self.buffer[self.n == 1 ? 0:1];
	LCUI_RBTreeNode *node = RBTree_CustomSearch( buffer, widget );

	if( node ) {
		task = (TaskNode*)node->data;
	} else {
		int i;
		task = (TaskNode*)malloc(sizeof(TaskNode));
		task->target = widget;
		for( i=0; i<WTT_TOTAL_NUM; ++i ) {
			task->tasks[i].is_valid = FALSE;
		}
		node = RBTree_CustomInsert( buffer, widget, task );
	}
	task->tasks[data->type].is_valid = TRUE;
	task->tasks[data->type].data = *data;

	return 0;
}

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_Task_Init(void)
{
	self.n = 0;
	RBTree_Init( &self.buffer[0] );
	RBTree_Init( &self.buffer[1] );
	RBTree_SetJudgeFunc( &self.buffer[0], CustomKeyJudge );
	RBTree_SetJudgeFunc( &self.buffer[1], CustomKeyJudge );
}

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_Task_Destroy(void)
{
	RBTee_Destroy( &self.buffer[0] );
	RBTee_Destroy( &self.buffer[1] );
}

static void DispacthTask( LCUI_Widget target, LCUI_WidgetTask *task )
{
	switch(task->type) {
	case WTT_MOVE:
	case WTT_RESIZE:
	case WTT_SHOW:
	default: break;
	}
}

/** 处理一次当前积累的部件任务 */
void LCUIWidget_Task_Step(void)
{
	int i;
	LCUI_RBTree *buffer;
	LCUI_RBTreeNode *node;
	TaskNode *task;

	self.n = self.n == 1 ? 0:1;
	buffer = &self.buffer[self.n];
	node = RBTree_First( buffer );
	while( node ) {
		task = (TaskNode*)node->data;
		for( i=0; i<WTT_TOTAL_NUM; ++i ) {
			if( !task->tasks[i].is_valid ) {
				continue;
			}
			DispacthTask( task->target, &task->tasks[i].data );
			task->tasks[i].is_valid = FALSE;
		}
		node = RBTree_Next( node );
		RBTree_CustomErase( buffer, task->target );
	}
}
