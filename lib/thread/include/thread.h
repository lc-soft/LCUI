/*
 * lib/thread/include/thread.h: -- basic thread management
 *
 * Copyright (c) 2018-2022-2023-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_THREAD_INCLUDE_THREAD_H
#define LIB_THREAD_INCLUDE_THREAD_H

#define LIBTHREAD_VERSION "0.1.0-a"
#define LIBTHREAD_VERSION_MAJOR 0
#define LIBTHREAD_VERSION_MINOR 1
#define LIBTHREAD_VERSION_ALTER 0
#define LIBTHREAD_STATIC_BUILD 1

#ifndef LIBTHREAD_PUBLIC
#if defined(_MSC_VER) && !defined(LIBTHREAD_STATIC_BUILD)
#ifdef LIBTHREAD_DLL_EXPORT
#define LIBTHREAD_PUBLIC __declspec(dllexport)
#else
#define LIBTHREAD_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBTHREAD_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBTHREAD_PUBLIC extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long thread_t;
typedef union thread_mutex_record_t *thread_mutex_t;
typedef union thread_cond_record_t *thread_cond_t;

LIBTHREAD_PUBLIC int thread_mutex_init(thread_mutex_t *mutex);
LIBTHREAD_PUBLIC void thread_mutex_destroy(thread_mutex_t *mutex);
LIBTHREAD_PUBLIC int thread_mutex_trylock(thread_mutex_t *mutex);
LIBTHREAD_PUBLIC int thread_mutex_lock(thread_mutex_t *mutex);
LIBTHREAD_PUBLIC int thread_mutex_unlock(thread_mutex_t *mutex);

LIBTHREAD_PUBLIC int thread_cond_init(thread_cond_t *cond);
LIBTHREAD_PUBLIC int thread_cond_destroy(thread_cond_t *cond);
LIBTHREAD_PUBLIC int thread_cond_wait(thread_cond_t *cond,
                                      thread_mutex_t *mutex);
LIBTHREAD_PUBLIC int thread_cond_timedwait(thread_cond_t *cond,
                                           thread_mutex_t *mutex,
                                           unsigned int ms);
LIBTHREAD_PUBLIC int thread_cond_signal(thread_cond_t *cond);
LIBTHREAD_PUBLIC int thread_cond_broadcast(thread_cond_t *cond);

LIBTHREAD_PUBLIC thread_t thread_self(void);
LIBTHREAD_PUBLIC int thread_create(thread_t *tidp, void (*start_rtn)(void *),
                                   void *arg);
LIBTHREAD_PUBLIC int thread_join(thread_t thread, void **retval);
LIBTHREAD_PUBLIC void thread_cancel(thread_t thread);
LIBTHREAD_PUBLIC void thread_exit(void *retval);

#ifdef __cplusplus
}
#endif

#endif
