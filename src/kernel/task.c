/* ***************************************************************************
 * task.c -- The app task queue operation set.
 * 
 * Copyright (C) 2013 by
 * Liu Chao
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
 * task.c -- 程序任务队列操作集
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
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
#include <LCUI_Build.h>
#include LC_LCUI_H

/* 销毁程序任务 */
static void
Destroy_Task( void *arg )
{
	LCUI_Task *task;
	task = (LCUI_Task *)arg;
	if( task->destroy_arg[0] && task->arg[0] ) {
		free( task->arg[0] );
		task->arg[0] = NULL;
	}
	if( task->destroy_arg[1] && task->arg[1] ) {
		free( task->arg[1] );
		task->arg[1] = NULL;
	}
}

LCUI_EXPORT(void)
AppTasks_Init( LCUI_Queue *tasks )
{
	Queue_Init( tasks, sizeof(LCUI_Task), Destroy_Task );
}

/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Task结构体中的成员变量 id，保存的是目标程序的id
 */
LCUI_EXPORT(int)
AppTasks_Add( LCUI_Task *task )
{
	LCUI_App *app;
	app = LCUIApp_Find( task->id );
	if( !app ) {
		return -1;
	}
	if(Queue_Add( &app->tasks, task ) < 0 ) {
		return -2;
	}
	return 0;
}

static int 
Tasks_CustomAdd( LCUI_Queue *tasks, int mode, LCUI_Task *task )
{
	int total, i;
	LCUI_Task *tmp_task;
	
	total = Queue_GetTotal(tasks);
	/* 如果模式是“添加新的”模式 */
	if( mode == ADD_MODE_ADD_NEW ) {
		Queue_Add(tasks, task); 
		return 0;
	}
	for (i=0; i < total; ++i) { 
		tmp_task = Queue_Get(tasks, i);
		/* 如果指针无效，或者函数指针已有记录 */
		if( !tmp_task || tmp_task->func != task->func ) {
			continue;
		}
		/* 如果要求的是不重复模式 */ 
		if(Check_Option(mode, ADD_MODE_NOT_REPEAT)) {
			/* 如果要求是第1个参数不能重复 */
			if(Check_Option(mode, AND_ARG_F)) {
				/* 如果要求是第2个参数也不能重复 */
				if(Check_Option(mode, AND_ARG_S)) {
					/* 如果函数以及参数1和2都一样 */ 
					if(tmp_task->arg[0] == task->arg[0] 
					&& tmp_task->arg[1] == task->arg[1]) {
						Destroy_Task( task );
						return -1; 
					}
				} else {/* 否则，只是要求函数以及第1个参数不能全部重复 */
					if(tmp_task->arg[0] == task->arg[0]) { 
						Destroy_Task( task );
						return -1; 
					}
				}
			}/* 否则，如果只是要求是第2个参数不能重复 */
			else if(Check_Option(mode, AND_ARG_S)) {
				if(tmp_task->arg[1] == task->arg[1] ) {
					Destroy_Task( task );
					return -1; 
				}
			} else {/* 否则，只是要求函数不同 */ 
				Destroy_Task( task );
				return -1; 
			}
		}/* 如果要求的是替换模式 */
		else if(Check_Option(mode, ADD_MODE_REPLACE)) {
			/* 如果要求是第1个参数相同 */
			if( Check_Option(mode, AND_ARG_F) ) {
				/* 如果要求是第2个参数也相同 */
				if( Check_Option(mode, AND_ARG_S) ) {
					if(tmp_task->arg[0] == task->arg[0] 
					&& tmp_task->arg[1] == task->arg[1]
					) {
						break; 
					}
				} else {/* 否则，只是要求函数以及第1个参数全部相同 */
					if(tmp_task->arg[0] == task->arg[0]) {
						break; 
					}
				}
			}/* 否则，如果只是要求第2个参数不能相同 */
			else if(Check_Option(mode, AND_ARG_S)) {
				if(tmp_task->arg[1] == task->arg[1]) {
					break; 
				}
			} else { 
				break; 
			}
		}
	}
	
	if(i == total) {
		Queue_Add(tasks, task); 
	} else {
		Queue_Replace( tasks, i, task ); 
	}
	return 0;
}

/*
 * 功能：使用自定义方式添加程序任务
 * 用法示例：
 * 在函数的各参数与队列中的函数及各参数不重复时，添加它
 * AppTasks_CustomAdd(ADD_MODE_NOT_REPEAT | AND_ARG_F | AND_ARG_S, task);
 * 只要函数和参数1不重复则添加
 * AppTasks_CustomAdd(ADD_MODE_NOT_REPEAT | AND_ARG_F, task);
 * 要函数不重复则添加
 * AppTasks_CustomAdd(ADD_MODE_NOT_REPEAT, task);
 * 添加新的，不管是否有重复的
 * AppTasks_CustomAdd(ADD_MODE_ADD_NEW, task);
 * 有相同函数则覆盖，没有则新增
 * AppTasks_CustomAdd(ADD_MODE_REPLACE, task);
 * */
LCUI_EXPORT(int)
AppTasks_CustomAdd( int mode, LCUI_Task *task )
{
	/* 先获取程序数据结构体指针 */
	LCUI_App *app;
	
	if( task->id == (LCUI_ID)0 ) {
		app = LCUIApp_GetSelf();
	} else {
		app = LCUIApp_Find( task->id );
	}
	if( !app ) {
		return -1;
	}
	return Tasks_CustomAdd( &app->tasks, mode, task );
}
/**************************** Task End ********************************/
