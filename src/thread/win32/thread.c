/* ***************************************************************************
 * thread.c -- the win32 edition thread opreation set.
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
 * ****************************************************************************/
 
/* ****************************************************************************
 * thread.c -- win32版的线程操作集
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
 * ****************************************************************************/

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
