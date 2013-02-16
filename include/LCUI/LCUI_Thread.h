/* ***************************************************************************
 * LCUI_Thread.h -- basic thread management
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Thread.h -- 基本的线程管理
 *
 * 版权所有 (C) 2012 归属于 
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

/* 
 * 说明：用于实现对线程的管理，由于LCUI还未实现多进程通信，只能靠线程ID来区分哪个操作是
 * 哪个程序的，目前打算是将程序以线程的形式运行，而不是以进程的形式运行，主线程为运行平台，
 * 其它程序就在该平台上运行，虽然能实现数据的共享，但是，一旦某个程序出现错误，整个进程就
 * 会终止。
 * 在使用本文件内的函数时，会对线程关系树（我觉得叫这个名字好一些）进行相应操作，比如：
 * 创建线程，就会先得到创建时的父线程ID，搜索这个树中的每个结点，如果有匹配的线程ID，就
 * 将线这个子程ID加入这个父线程的子线程队列中；如果没有匹配的，那就新增一个至主队列中。
 * 而撤销线程，也类似，先查找，后移除。
 * 为确保LCUI在结束后，不会因为还有其它线程在使用LCUI已经释放的资源，而导致段错误，LCUI
 * 会在退出前，撤销所有已记录的线程，然后进行资源释放。
 * */

#ifndef __LCUI_THREAD_H__
#define __LCUI_THREAD_H__ 

#ifndef LCUI_THREAD_PTHREAD
#define LCUI_THREAD_PTHREAD
#endif

#ifdef LCUI_THREAD_PTHREAD
	#include <pthread.h>
	typedef pthread_mutex_t mutex_t;
	typedef pthread_t thread_t;
#elif LCUI_THREAD_WIN32
	#include <windows.h>
	typedef HANDLE mutex_t;
	typedef HANDLE thread_t;
#else
	#error 'Need thread implementation for this platform'
#endif

LCUI_BEGIN_HEADER

thread_t thread_self( void );

int thread_create(	thread_t *__newthread,
			void *(*__start_routine) (void *),
			void *__arg );

int thread_cancel( thread_t __th );

int thread_join( thread_t __th, void **__thread_return );

void thread_exit( void *__retval ) __attribute__ ((__noreturn__));


int thread_mutex_init( mutex_t *mutex );

int thread_mutex_destroy( mutex_t *mutex );

int thread_mutex_lock( mutex_t *mutex );

int thread_mutex_unlock( mutex_t *mutex );


/* 获取指定线程的根线程ID */
thread_t LCUIThread_GetRootThreadID( thread_t tid );

/* 创建并运行一个线程 */
int LCUIThread_Create( thread_t *tidp, void *(*start_rtn)(void*), void * arg );

int LCUIThread_Join(thread_t thread, void **retval);
/* 功能：等待一个线程的结束 */

int LCUIThread_Cancel(thread_t thread);
/* 功能：撤销一个线程 */

void LCUIThread_Exit(void* retval)  __attribute__ ((__noreturn__));
/*
 * 功能：终止调用它的线程并返回一个指向某个对象的指针
 * 说明：线程通过调用LCUIThread_Exit函数终止执行，就如同进程在结
 * 束时调用exit函数一样。
 * */

/* 撤销指定ID的程序的全部子线程 */
int LCUIApp_CancelAllThreads( LCUI_ID app_id );

/* 初始化线程模块 */
void LCUIModule_Thread_Init( void );

/* 停用线程模块 */
void LCUIModule_Thread_End( void );

LCUI_END_HEADER


#ifdef __cplusplus
#include LC_THREAD_HPP
#endif

#endif

