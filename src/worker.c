/* ***************************************************************************
 * worker.c -- worker threading and task
 *
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
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
 * worker.c -- 工作线程与任务
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
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

#define LCUI_WORKER_C

#include <errno.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/worker.h>

typedef struct LCUI_WorkerRec_ {
	LCUI_BOOL active;		/**< 是否处于活动状态 */
	LinkedList tasks;		/**< 任务队列 */
	LCUI_Mutex mutex;		/**< 互斥锁 */
	LCUI_Cond cond;			/**< 条件变量 */
	LCUI_Thread thread;		/**< 所在的线程 */
} LCUI_WorkerRec;

LCUI_Worker LCUIWorker_New( void )
{
	LCUI_Worker worker = NEW( LCUI_WorkerRec, 1 );
	LCUIMutex_Init( &worker->mutex );
	LCUICond_Init( &worker->cond );
	LinkedList_Init( &worker->tasks );
	worker->active = FALSE;
	worker->thread = 0;
	return worker;
}

void LCUIWorker_PostTask( LCUI_Worker worker, LCUI_Task task )
{
	LCUI_Task newtask;
	newtask = NEW( LCUI_TaskRec, 1 );
	*newtask = *task;
	LCUIMutex_Lock( &worker->mutex );
	LinkedList_Append( &worker->tasks, newtask );
	LCUICond_Signal( &worker->cond );
	LCUIMutex_Unlock( &worker->mutex );
}

LCUI_BOOL LCUIWorker_RunTask( LCUI_Worker worker )
{
	LCUI_Task task;
	LinkedListNode *node;
	LCUIMutex_Lock( &worker->mutex );
	node = LinkedList_GetNode( &worker->tasks, 0 );
	if( node ) {
		task = node->data;
		LinkedList_Unlink( &worker->tasks, node );
		LCUIMutex_Unlock( &worker->mutex );
		LCUITask_Run( task );
		LCUITask_Destroy( task );
		free( task );
		free( node );
		return TRUE;
	}
	LCUIMutex_Unlock( &worker->mutex );
	return FALSE;
}

static void LCUIWorker_Thread( void *arg )
{
	LCUI_Worker worker = arg;
	LCUIMutex_Lock( &worker->mutex );
	while( worker->active ) {
		if( LCUIWorker_RunTask( worker ) ) {
			continue;
		}
		if( worker->active ) {
			LCUICond_Wait( &worker->cond, &worker->mutex );
		}
	}
	LCUIMutex_Unlock( &worker->mutex );
	LCUIThread_Exit( NULL );
}

int LCUIWorker_RunAsync( LCUI_Worker worker )
{
	if( worker->thread != 0 ) {
		return -EEXIST;
	}
	worker->active = TRUE;
	LCUIThread_Create( &worker->thread, LCUIWorker_Thread, worker );
	LOG( "[worker] worker %u is running\n", worker->thread );
	return 0;
}

static void OnDeleteTask( void *arg )
{
	LCUITask_Destroy( arg );
	free( arg );
}

void LCUIWorker_Destroy( LCUI_Worker worker )
{
	if( worker->thread != 0 ) {
		LOG( "[worker] worker %u is stopping...\n", worker->thread );
		LCUIMutex_Lock( &worker->mutex );
		worker->active = FALSE;
		LCUICond_Signal( &worker->cond );
		LCUIMutex_Unlock( &worker->mutex );
		LCUIThread_Join( worker->thread, NULL );
		LOG( "[worker] worker %u has stopped\n", worker->thread );
		worker->thread = 0;
	}
	LCUIMutex_Destroy( &worker->mutex );
	LCUICond_Destroy( &worker->cond );
	LinkedList_Clear( &worker->tasks, OnDeleteTask );
	free( worker );
}
