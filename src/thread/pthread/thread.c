/* thread.c -- The pthread edition thread opreation set.
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

#include <stdlib.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

#ifdef LCUI_THREAD_PTHREAD

typedef struct LCUI_ThreadContextRec {
	void (*func)(void*);
	void *arg;
	LCUI_Thread tid;
	LinkedListNode node;
} LCUI_ThreadContextRec, *LCUI_ThreadContext;

static struct LCUIThreadModule {
	LCUI_BOOL is_inited;
	LCUI_Mutex mutex;
	LinkedList threads;
} self;

static void *LCUIThread_Run( void *arg )
{
	LCUI_ThreadContext ctx = arg;
	ctx->func( ctx->arg );
	LCUIMutex_Lock( &self.mutex );
	LinkedList_Unlink( &self.threads, &ctx->node );
	LCUIMutex_Unlock( &self.mutex );
	free( ctx );
	pthread_exit( NULL );
}

static LCUI_ThreadContext LCUIThread_Find( LCUI_Thread tid )
{
	LinkedListNode *node;
	LCUI_ThreadContext ctx;
	for( LinkedList_Each( node, &self.threads ) ) {
		ctx = node->data;
		if( ctx && ctx->tid == tid ) {
			return ctx;
		}
	}
	return NULL;
}

static LCUI_ThreadContext LCUIThread_Get( LCUI_Thread tid )
{
	LCUI_ThreadContext ctx;
	LCUIMutex_Lock( &self.mutex );
	ctx = LCUIThread_Find( tid );
	if( ctx ) {
		LinkedList_Unlink( &self.threads, &ctx->node );
	}
	LCUIMutex_Unlock( &self.mutex );
	return ctx;
}

int LCUIThread_Create( LCUI_Thread *thread, void(*func)(void*), void *arg )
{
	int ret;
	LCUI_ThreadContext ctx;
	if( !self.is_inited ) {
		LinkedList_Init( &self.threads );
		LCUIMutex_Init( &self.mutex );
		self.is_inited = TRUE;
	}
	ctx = NEW( LCUI_ThreadContextRec, 1 );
	if( !ctx ) {
		return -ENOMEM;
	}
	ctx->arg = arg;
	ctx->func = func;
	ctx->node.data = ctx;
	ret = pthread_create( &ctx->tid, NULL, LCUIThread_Run, ctx );
	if( ret != 0 ) {
		free( ctx );
		return ret;
	}
	LCUIMutex_Lock( &self.mutex );
	LinkedList_AppendNode( &self.threads, &ctx->node );
	LCUIMutex_Unlock( &self.mutex );
	*thread = ctx->tid;
	return ret;
}

LCUI_Thread LCUIThread_SelfID( void )
{
	return pthread_self();
}

void LCUIThread_Exit( void *retval )
{
	LCUI_Thread tid;
	LCUI_ThreadContext ctx;
	tid = LCUIThread_SelfID();
	ctx = LCUIThread_Get( tid );
	if( ctx ) {
		free( ctx );
	}
	pthread_exit( retval );
}

void LCUIThread_Cancel( LCUI_Thread thread )
{
#ifdef PTHREAD_CANCEL_ASYNCHRONOUS
	pthread_cancel( thread );
#else
	pthread_kill( thread, SIGKILL );
#endif
}

int LCUIThread_Join( LCUI_Thread thread, void **retval )
{
	return pthread_join( thread, retval );
}
#endif
