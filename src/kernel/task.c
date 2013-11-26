/* ***************************************************************************
 * task.c -- The app task queue operation set.
 * 
 * Copyright (C) 2012-2013 by
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
 * 版权所有 (C) 2012-2013 归属于
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

/** 销毁程序任务 */
static void DestroyTask( void *arg )
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

LCUI_API void AppTasks_Init( LCUI_Queue *tasks )
{
	Queue_Init( tasks, sizeof(LCUI_Task), DestroyTask );
}

/** 无锁模式添加任务 */
LCUI_API int AppTasks_NoLockAdd( LCUI_Task *task )
{
	LCUI_App *app;
	app = LCUIApp_Find( task->id );
	if( !app ) {
		return -1;
	}
	if( !Queue_Add( &app->tasks, task ) ) {
		Queue_Unlock( &app->tasks );
		return -2;
	}
	LCUISleeper_BreakSleep( &app->mainloop_sleeper );
	return 0;
}
/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Task结构体中的成员变量 id 保存的是目标程序的id
 */
LCUI_API int AppTasks_Add( LCUI_Task *task )
{
	LCUI_App *app;
	app = LCUIApp_Find( task->id );
	if( !app ) {
		return -1;
	}
	Queue_Lock( &app->tasks );
	if( !Queue_Add( &app->tasks, task ) ) {
		Queue_Unlock( &app->tasks );
		return -2;
	}
	Queue_Unlock( &app->tasks );
	LCUISleeper_BreakSleep( &app->mainloop_sleeper );
	return 0;
}

static LCUI_BOOL TaskEqual( LCUI_Task* t1, LCUI_Task* t2, int mode )
{
	if( t1->func != t2->func ) {
		return FALSE;
	}
	/* 如果要求是第1个参数不能重复 */
	if( HaveOption(mode, AND_ARG_F) ) {
		/* 如果要求是第2个参数也不能重复 */
		if( HaveOption(mode, AND_ARG_S) ) {
			/* 如果函数以及参数1和2都一样 */ 
			if(t1->arg[0] == t2->arg[0] 
			&& t1->arg[1] == t2->arg[1]) {
				return TRUE;
			}
			return FALSE;
		}
		/* 否则，只是要求函数以及第1个参数不能全部重复 */
		if( t1->arg[0] == t2->arg[0] ) { 
			return TRUE;
		}
		return FALSE;
	}
	/* 如果只是要求是第2个参数不能重复 */
	if( HaveOption(mode, AND_ARG_S) ) {
		if( t1->arg[1] == t2->arg[1] ) { 
			return TRUE;
		}
	}
	return FALSE;
}

/** 从程序任务队列中删除有指定回调函数的任务 */
LCUI_API int AppTasks_Delete(	LCUI_Queue *tasks,
				CallBackFunc task_func, 
				LCUI_BOOL need_lock )
{
	int n, i;
	LCUI_Task *exist_task;

	if( need_lock ) {
		Queue_Lock( tasks );
	}
	n = Queue_GetTotal( tasks );
	for (i=0; i<n; ++i) { 
		exist_task = (LCUI_Task*)Queue_Get( tasks, i );
		if( exist_task && exist_task->func == task_func ) {
			Queue_Delete( tasks, i );
			--n;
			--i;
		}
	}
	if( need_lock ) {
		Queue_Unlock( tasks );
	}
	return n;
}

static int Tasks_CustomAdd( LCUI_Queue *tasks, int mode, LCUI_Task *task )
{
	int n, i;
	LCUI_Task *exist_task = NULL;
	
	n = Queue_GetTotal(tasks);
	/* 如果模式是“添加新的”模式 */
	if( mode == ADD_MODE_ADD_NEW ) {
		Queue_Add( tasks, task );
		return 0;
	}
	for (i=0; i < n; ++i) { 
		exist_task = (LCUI_Task*)Queue_Get( tasks, i );
		/* 如果已存在的任务与当前任务匹配 */
		if( exist_task && TaskEqual( exist_task, task, mode ) ) {
			break;
		}
	}
	/* 如果没有已存在的任务与当前任务匹配 */
	if( i >= n || !exist_task ) {
		Queue_Add( tasks, task );
		return 0;
	}
	/* 如果任务不能重复 */
	if( HaveOption(mode, ADD_MODE_NOT_REPEAT) ) {
		n = -1;
	}
	/* 如果需要覆盖任务 */
	if( HaveOption(mode, ADD_MODE_REPLACE) )  {
		DestroyTask( exist_task );		/* 先销毁已存在的任务 */
		Queue_Replace( tasks, i, task );	/* 然后再覆盖 */
		n = 0;
	}
	return n;
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
LCUI_API int AppTasks_CustomAdd( int mode, LCUI_Task *task )
{
	int ret;
	LCUI_App *app;
	
	if( task->id == (LCUI_ID)0 ) {
		app = LCUIApp_GetSelf();
	} else {
		app = LCUIApp_Find( task->id );
	}
	if( !app ) {
		return -1;
	}
	Queue_Lock( &app->tasks );
	ret = Tasks_CustomAdd( &app->tasks, mode, task );
	if( ret == 0 ) {
		LCUISleeper_BreakSleep( &app->mainloop_sleeper );
	} else if( ret == -1 ) {
		DestroyTask( task );
	}
	Queue_Unlock( &app->tasks );
	return ret;
}
/**************************** Task End ********************************/
