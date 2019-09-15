/*
 * thread.h -- basic thread management
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

#ifndef LCUI_THREAD_H
#define LCUI_THREAD_H

#ifdef _WIN32
#include <windows.h>
typedef HANDLE LCUI_Mutex;
typedef HANDLE LCUI_Cond;
typedef unsigned int LCUI_Thread;
#else
#include <pthread.h>
typedef pthread_t LCUI_Thread;
typedef pthread_mutex_t LCUI_Mutex;
typedef pthread_cond_t LCUI_Cond;
#endif

LCUI_BEGIN_HEADER

/*----------------------------- Mutex <START> -------------------------------*/

/* init the mutex */
LCUI_API int LCUIMutex_Init(LCUI_Mutex *mutex);

/* Free the mutex */
LCUI_API void LCUIMutex_Destroy(LCUI_Mutex *mutex);

/* Try lock the mutex */
LCUI_API int LCUIMutex_TryLock(LCUI_Mutex *mutex);

/* Lock the mutex */
LCUI_API int LCUIMutex_Lock(LCUI_Mutex *mutex);

/* Unlock the mutex */
LCUI_API int LCUIMutex_Unlock(LCUI_Mutex *mutex);

/*------------------------------- Mutex <END> -------------------------------*/

/*------------------------------ Cond <START> -------------------------------*/

/** 初始化一个条件变量 */
LCUI_API int LCUICond_Init(LCUI_Cond *cond);

/** 销毁一个条件变量 */
LCUI_API int LCUICond_Destroy(LCUI_Cond *cond);

/** 阻塞当前线程，等待条件成立 */
LCUI_API int LCUICond_Wait(LCUI_Cond *cond, LCUI_Mutex *mutex);

/** 计时阻塞当前线程，等待条件成立 */
LCUI_API int LCUICond_TimedWait(LCUI_Cond *cond, LCUI_Mutex *mutex, unsigned int ms);

/** 唤醒一个阻塞等待条件成立的线程 */
LCUI_API int LCUICond_Signal(LCUI_Cond *cond);

/** 唤醒所有阻塞等待条件成立的线程 */
LCUI_API int LCUICond_Broadcast(LCUI_Cond *cond);

/*------------------------------- Cond <END> --------------------------------*/


/*----------------------------- Thread <START> ------------------------------*/

LCUI_API LCUI_Thread LCUIThread_SelfID(void);

/* 创建并运行一个线程 */
LCUI_API int LCUIThread_Create(LCUI_Thread *tidp, void(*start_rtn)(void*), void *arg);

/* 等待一个线程的结束，并释放该线程的资源 */
LCUI_API int LCUIThread_Join(LCUI_Thread thread, void **retval);

/* 撤销一个线程 */
LCUI_API void LCUIThread_Cancel(LCUI_Thread thread);

/* 记录指针作为返回值，并退出线程 */
LCUI_API void LCUIThread_Exit(void* retval);

/*------------------------------ Thread <END> -------------------------------*/

LCUI_END_HEADER

#endif

