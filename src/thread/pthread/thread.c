/* ***************************************************************************
 * thread.c -- the pthread edition thread opreation set.
 *
 * Copyright (C) 2013-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2017 归属于 刘超 <lc-soft@live.cn>
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

typedef struct LCUI_ThreadTaskRec {
	void (*func)(void*);
	void *arg;
} LCUI_ThreadTaskRec, *LCUI_ThreadTask;

static void *LCUIThread_RunTask( void *arg )
{
	LCUI_ThreadTask task = arg;
	task->func( task->arg );
	free( task );
	pthread_exit( NULL );
}

int LCUIThread_Create( LCUI_Thread *thread, void(*func)(void*), void *arg )
{
	int ret;
	ASSIGN( task, LCUI_ThreadTask );
	if( !task ) {
		return -ENOMEM;
	}
	task->func = func;
	task->arg = arg;
	ret = pthread_create( thread, NULL, LCUIThread_RunTask, task );
	if( ret != 0 ) {
		free( task );
	}
	return ret;
}

LCUI_Thread LCUIThread_SelfID( void )
{
	return pthread_self();
}

void LCUIThread_Exit( void *retval )
{
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

// 以下是测试程序
#ifdef need_test_this_code
void test_thread(void *arg)
{
	char *ret_str;
	ret_str = malloc(200);
	sprintf( ret_str, "my tid is %lu", LCUIThread_SelfID() );
	printf( "[TID: %lu] %s\n", LCUIThread_SelfID(), (char*)arg );
	_LCUIThread_Exit( ret_str );
}

int main()
{
	LCUI_Thread thread[2];
	char *str="Hello, World.";
	char *ret_str[2];

	_LCUIThread_Create( &thread[0], test_thread, str );
	_LCUIThread_Create( &thread[1], test_thread, str );
	_LCUIThread_Join( thread[0], (void*)&ret_str[0] );
	_LCUIThread_Join( thread[1], (void*)&ret_str[1] );
	printf("ret_str[0]: %s\n", ret_str[0]);
	printf("ret_str[1]: %s\n", ret_str[1]);
	free(ret_str[0]);
	free(ret_str[1]);
	return 0;
}
#endif
#endif
