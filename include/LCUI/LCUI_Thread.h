/* ***************************************************************************
 * LCUI_Thread.h -- basic thread management
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
 * LCUI_Thread.h -- 基本的线程管理
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

/* 
 * 说明：
 * 在使用本文件内的函数时，会对线程关系树（我觉得叫这个名字好一些）进行相应操作，比如：
 * 创建线程，就会先得到创建时的父线程ID，搜索这个树中的每个结点，如果有匹配的线程ID，就
 * 将线这个子程ID加入这个父线程的子线程队列中；如果没有匹配的，那就新增一个至主队列中。
 * 而撤销线程，也类似，先查找，后移除。
 * 为确保LCUI在结束后，不会因为还有其它线程在使用LCUI已经释放的资源，而导致段错误，LCUI
 * 会在退出前，撤销所有已记录的线程，然后进行资源释放。
 * */

#ifndef __LCUI_THREAD_H__
#define __LCUI_THREAD_H__ 

#ifdef LCUI_THREAD_PTHREAD
#include <pthread.h>
typedef pthread_t LCUI_Thread;
typedef pthread_mutex_t LCUI_Mutex;
#else
#ifdef LCUI_THREAD_WIN32
#include <windows.h>
typedef HANDLE LCUI_Mutex;
typedef unsigned int LCUI_Thread;
#else
#error 'Need thread implementation for this platform'
#endif
#endif

LCUI_BEGIN_HEADER

/* init the mutex */
LCUI_API int
LCUIMutex_Init( LCUI_Mutex *mutex );

/* Free the mutex */
LCUI_API void
LCUIMutex_Destroy( LCUI_Mutex *mutex );

/* Lock the mutex */
LCUI_API int
LCUIMutex_Lock( LCUI_Mutex *mutex );

/* Unlock the mutex */
LCUI_API int
LCUIMutex_Unlock( LCUI_Mutex *mutex );


LCUI_API int
_LCUIThread_Create( LCUI_Thread *thread, void(*func)(void*), void *arg );

LCUI_API LCUI_Thread
LCUIThread_SelfID( void );

LCUI_API void
_LCUIThread_Exit( void *retval );

LCUI_API void
_LCUIThread_Cancel( LCUI_Thread thread );

LCUI_API int
_LCUIThread_Join( LCUI_Thread thread, void **retval );


/* 获取指定线程的根线程ID */
LCUI_API LCUI_Thread
LCUIThread_GetRootThreadID( LCUI_Thread tid );

/* 打印各个线程的信息 */
LCUI_API void
LCUIThread_PrintInfo( void );

/* 创建并运行一个线程 */
LCUI_API int
LCUIThread_Create( LCUI_Thread *tidp, void (*start_rtn)(void*), void * arg );

/* 等待一个线程的结束，并释放该线程的资源 */
LCUI_API int
LCUIThread_Join( LCUI_Thread thread, void **retval );

/* 撤销一个线程 */
LCUI_API void
LCUIThread_Cancel( LCUI_Thread thread );

/* 记录指针作为返回值，并退出线程 */
LCUI_API void
LCUIThread_Exit( void* retval );

/* 撤销指定ID的程序的全部子线程 */
LCUI_API int
LCUIApp_CancelAllThreads( LCUI_ID app_id );

/* 注册程序主线程 */
LCUI_API void
LCUIApp_RegisterMainThread( LCUI_ID app_id );

/* 初始化线程模块 */
LCUI_API void
LCUIModule_Thread_Init( void );

/* 停用线程模块 */
LCUI_API void
LCUIModule_Thread_End( void );

LCUI_END_HEADER


#ifdef __cplusplus
#include LC_THREAD_HPP
#endif

#endif

