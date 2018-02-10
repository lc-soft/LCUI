/* thread.c -- The win32 edition thread opreation set.
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


#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

#ifdef LCUI_THREAD_WIN32
#include <process.h>
#include <windows.h>

typedef struct _LCUI_ThreadContextRec_ {
	HANDLE handle;
	unsigned int tid;
	void (*func)(void*);
	void *arg;
	void *retval;
	LCUI_BOOL has_waiter;
	LinkedListNode node;
} LCUI_ThreadContextRec, *LCUI_ThreadContext;

static struct LCUIThreadModule {
	LCUI_BOOL is_inited;
	LCUI_Mutex mutex;
	LinkedList threads;
} self;

static unsigned __stdcall run_thread(void *arg)
{
	LCUI_ThreadContext thread;
	thread = (LCUI_ThreadContext)arg;
	thread->func( thread->arg );
	return 0;
}

int LCUIThread_Create( LCUI_Thread *tid, void( *func )(void*), void *arg )
{
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
	ctx->func = func;
	ctx->arg = arg;
	ctx->retval = NULL;
	ctx->node.data = ctx;
	ctx->handle = (HANDLE)_beginthreadex( NULL, 0, run_thread,
					      ctx, 0, &ctx->tid );
	if( ctx->handle == 0 ) {
		*tid = 0;
		return -1;
	}
	LCUIMutex_Lock( &self.mutex );
	LinkedList_AppendNode( &self.threads, &ctx->node );
	LCUIMutex_Unlock( &self.mutex );
	*tid = ctx->tid;
	return 0;
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

static void LCUIThread_Destroy( LCUI_ThreadContext ctx )
{
	CloseHandle( ctx->handle );
	ctx->handle = NULL;
	free( ctx );
}

LCUI_Thread LCUIThread_SelfID( void )
{
	return GetCurrentThreadId();
}

void LCUIThread_Exit( void *retval )
{
	LCUI_Thread tid;
	LCUI_ThreadContext ctx;
	tid = LCUIThread_SelfID();
	ctx = LCUIThread_Get( tid );
	if( !ctx ) {
		return;
	}
	ctx->retval = retval;
	if( !ctx->has_waiter ) {
		LCUIThread_Destroy( ctx );
	}
	_endthread();
}

void LCUIThread_Cancel( LCUI_Thread tid )
{
#ifdef WINAPI_FAMILY_APP
	abort();
#else
	LCUI_ThreadContext ctx;
	ctx = LCUIThread_Get( tid );
	if( ctx ) {
		TerminateThread( ctx->handle, 0 );
		LCUIThread_Destroy( ctx );
	}
#endif
}

int LCUIThread_Join( LCUI_Thread thread, void **retval )
{
	DWORD code;
	LCUI_ThreadContext ctx;
	LCUIMutex_Lock( &self.mutex );
	ctx = LCUIThread_Find( thread );
	if( ctx == NULL ) {
		LCUIMutex_Unlock( &self.mutex );
		return -1;
	}
	ctx->has_waiter = TRUE;
	LCUIMutex_Unlock( &self.mutex );
	if( !GetExitCodeThread( ctx->handle, &code ) ) {
		WaitForSingleObject( ctx->handle, 5000 );
	}
	ctx = LCUIThread_Get( thread );
	if( ctx ) {
		if( retval ) {
			*retval = ctx->retval;
		}
		LCUIThread_Destroy( ctx );
		return 0;
	}
	return -1;
}

#endif
