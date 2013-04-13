/* ***************************************************************************
 * task.h -- The app task queue operation set.
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
 * task.h -- 程序任务队列操作集
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
#ifndef __LCUI_KERNEL_TASK_H__
#define __LCUI_KERNEL_TASK_H__

LCUI_BEGIN_HEADER

typedef struct {
	/* 
	 * 函数ID，部件库需要这ID标识函数类型，往程序的任务队列添加
	 * 任务也需要该ID来标识目标程序ID 
	 * */
	LCUI_ID id;
	void (*func)(void*,void*);  /* 函数指针 */
	
	/* 以下参数该怎么传给回调函数，具体要看是如何处理事件的 */  
	void *arg[2];		/* 传给函数的两个参数 */
	LCUI_BOOL destroy_arg[2];	/* 指定是否在调用完回调函数后，销毁参数 */
} LCUI_Func, LCUI_Task;

LCUI_EXPORT(void) AppTasks_Init( LCUI_Queue *tasks );

/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Task结构体中的成员变量 id，保存的是目标程序的id
 */
LCUI_EXPORT(int) AppTasks_Add( LCUI_Task *task );

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
LCUI_EXPORT(int) AppTasks_CustomAdd( int mode, LCUI_Task *task );

LCUI_END_HEADER

#endif
