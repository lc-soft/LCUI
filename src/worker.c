/* worker.c -- worker threading and task
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
