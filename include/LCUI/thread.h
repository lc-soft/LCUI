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
typedef HANDLE LCUI_Cond;
typedef unsigned int LCUI_Thread;
#else
#error 'Need thread implementation for this platform'
#endif
#endif

LCUI_BEGIN_HEADER

/*----------------------------- Mutex <START> -------------------------------*/

/* init the mutex */
LCUI_API int LCUIMutex_Init( LCUI_Mutex *mutex );

/* Free the mutex */
LCUI_API void LCUIMutex_Destroy( LCUI_Mutex *mutex );

/* Try lock the mutex */
LCUI_API int LCUIMutex_TryLock( LCUI_Mutex *mutex );

/* Lock the mutex */
LCUI_API int LCUIMutex_Lock( LCUI_Mutex *mutex );

/* Unlock the mutex */
LCUI_API int LCUIMutex_Unlock( LCUI_Mutex *mutex );

/*------------------------------- Mutex <END> -------------------------------*/

/*------------------------------ Cond <START> -------------------------------*/

/** 新建一个条件变量 */
LCUI_API int LCUICond_Init( LCUI_Cond *cond );

/** 销毁一个条件变量 */
LCUI_API void LCUICond_Destroy( LCUI_Cond *cond );

/** 阻塞当前线程，等待条件成立 */
LCUI_API unsigned int LCUICond_Wait( LCUI_Cond *cond );

/** 计时阻塞当前线程，等待条件成立 */
LCUI_API unsigned int LCUICond_TimedWait( LCUI_Cond *cond, unsigned int ms );

/** 唤醒所有阻塞等待条件成立的线程 */
LCUI_API int LCUICond_Broadcast( LCUI_Cond *cond );

/*------------------------------- Cond <END> --------------------------------*/


/*----------------------------- Thread <START> ------------------------------*/

LCUI_API LCUI_Thread LCUIThread_SelfID( void );

/* 创建并运行一个线程 */
LCUI_API int LCUIThread_Create( LCUI_Thread *tidp, void (*start_rtn)(void*), void *arg );

/* 等待一个线程的结束，并释放该线程的资源 */
LCUI_API int LCUIThread_Join( LCUI_Thread thread, void **retval );

/* 撤销一个线程 */
LCUI_API void LCUIThread_Cancel( LCUI_Thread thread );

/* 记录指针作为返回值，并退出线程 */
LCUI_API void LCUIThread_Exit( void* retval );

/*------------------------------ Thread <END> -------------------------------*/

LCUI_END_HEADER

#endif

