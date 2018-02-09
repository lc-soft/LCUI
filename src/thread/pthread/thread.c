/* ***************************************************************************
 * thread.c -- the pthread edition thread opreation set.
 *
 * Copyright (C) 2013-2018 by Liu Chao <lc-soft@live.cn>
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
 * thread.c -- pthread版的线程操作集
 *
 * 版权所有 (C) 2013-2018 归属于 刘超 <lc-soft@live.cn>
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
