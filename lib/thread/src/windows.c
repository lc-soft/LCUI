/*
 * lib/thread/src/windows.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifdef _WIN32
#include <errno.h>
#include <windows.h>
#include <process.h>
#include <yutil.h>
#include "thread.h"

typedef union thread_mutex_record_t {
	HANDLE handle;
} thread_mutex_record_t;

typedef union thread_cond_record_t {
	HANDLE handle;
} thread_cond_record_t;

typedef struct thread_info_t {
	HANDLE handle;
	thread_t tid;
	void (*func)(void *);
	void *arg;
	void *retval;
	int has_waiter;
	list_node_t node;
} thread_info_t;

static struct thread_manager_module_t {
	int active;
	thread_mutex_t mutex;
	list_t threads;
} thread_manager;

/** 初始化一个条件变量 */
int thread_cond_init(thread_cond_t *cond)
{
	HANDLE handle;

	handle = CreateEvent(NULL, false, false, NULL);
	if (handle == 0) {
		return -1;
	}
	*cond = malloc(sizeof(thread_cond_record_t));
	if (!*cond) {
		return -1;
	}
	(*cond)->handle = handle;
	return 0;
}

/** 销毁一个条件变量 */
int thread_cond_destroy(thread_cond_t *cond)
{
	CloseHandle((*cond)->handle);
	free(*cond);
	*cond = NULL;
	return 0;
}

/** 阻塞当前线程，等待条件成立 */
int thread_cond_wait(thread_cond_t *cond, thread_mutex_t *mutex)
{
	int ret;
	thread_mutex_unlock(mutex);
	ret = WaitForSingleObject((*cond)->handle, INFINITE);
	switch (ret) {
	case WAIT_TIMEOUT:
		ret = ETIMEDOUT;
		break;
	case WAIT_OBJECT_0:
		ret = 0;
		break;
	case WAIT_FAILED:
	default:
		ret = GetLastError();
		break;
	}
	thread_mutex_lock(mutex);
	return ret;
}

/** 计时阻塞当前线程，等待条件成立 */
int thread_cond_timedwait(thread_cond_t *cond, thread_mutex_t *mutex,
			  unsigned int ms)
{
	int ret;
	thread_mutex_unlock(mutex);
	ret = WaitForSingleObject((*cond)->handle, ms);
	switch (ret) {
	case WAIT_TIMEOUT:
		ret = ETIMEDOUT;
		break;
	case WAIT_OBJECT_0:
		ret = 0;
		break;
	case WAIT_FAILED:
	default:
		ret = GetLastError();
		break;
	}
	thread_mutex_lock(mutex);
	return ret;
}

/** 唤醒一个阻塞等待条件成立的线程 */
int thread_cond_signal(thread_cond_t *cond)
{
	/* 当编译为 Windows 运行时组件时，直接改为广播 */
#ifdef LIBPLAT_UWP
	return thread_cond_broadcast(cond);
#else
	if (PulseEvent((*cond)->handle)) {
		return 0;
	}
	return -1;
#endif
}

/** 唤醒所有阻塞等待条件成立的线程 */
int thread_cond_broadcast(thread_cond_t *cond)
{
	if (SetEvent((*cond)->handle)) {
		return 0;
	}
	return -1;
}

int thread_mutex_init(thread_mutex_t *mutex)
{
	*mutex = malloc(sizeof(thread_mutex_record_t));
	if (!*mutex) {
		return -1;
	}
	(*mutex)->handle = CreateMutex(NULL, false, NULL);
	return 0;
}

/* Free the mutex */
void thread_mutex_destroy(thread_mutex_t *mutex)
{
	CloseHandle((*mutex)->handle);
	free(*mutex);
	*mutex = NULL;
}

/* Try lock the mutex */
int thread_mutex_trylock(thread_mutex_t *mutex)
{
	switch (WaitForSingleObject((*mutex)->handle, 0)) {
	case WAIT_FAILED:
		return -1;
	case WAIT_OBJECT_0:
		break;
	default:
		return -2;
	}
	return 0;
}

/* Lock the mutex */
int thread_mutex_lock(thread_mutex_t *mutex)
{
	if (WaitForSingleObject((*mutex)->handle, INFINITE) == WAIT_FAILED) {
		return -1;
	}
	return 0;
}

/* Unlock the mutex */
int thread_mutex_unlock(thread_mutex_t *mutex)
{
	if (ReleaseMutex((*mutex)->handle) == false) {
		return -1;
	}
	return 0;
}

static unsigned __stdcall run_thread(void *arg)
{
	thread_info_t *thread = arg;

	thread->func(thread->arg);
	return 0;
}

int thread_create(thread_t *tid, void (*func)(void *), void *arg)
{
	thread_info_t *thread;

	if (!thread_manager.active) {
		list_create(&thread_manager.threads);
		thread_mutex_init(&thread_manager.mutex);
		thread_manager.active = true;
	}
	thread = malloc(sizeof(thread_info_t));
	if (!thread) {
		return -ENOMEM;
	}
	thread->func = func;
	thread->arg = arg;
	thread->retval = NULL;
	thread->node.data = thread;
	thread->handle = (HANDLE)_beginthreadex(NULL, 0, run_thread, thread, 0,
						&thread->tid);
	if (thread->handle == 0) {
		*tid = 0;
		return -1;
	}
	thread_mutex_lock(&thread_manager.mutex);
	list_append_node(&thread_manager.threads, &thread->node);
	thread_mutex_unlock(&thread_manager.mutex);
	*tid = thread->tid;
	return 0;
}

static thread_info_t *get_thread_info(thread_t tid)
{
	thread_info_t *thread;
	list_node_t *node;

	for (list_each(node, &thread_manager.threads)) {
		thread = node->data;
		if (thread && thread->tid == tid) {
			return thread;
		}
	}
	return NULL;
}

static thread_info_t *unlink_thread_info(thread_t tid)
{
	thread_info_t *thread;

	thread_mutex_lock(&thread_manager.mutex);
	thread = get_thread_info(tid);
	if (thread) {
		list_unlink(&thread_manager.threads, &thread->node);
	}
	thread_mutex_unlock(&thread_manager.mutex);
	return thread;
}

static void thread_info_destroy(thread_info_t *thread)
{
	CloseHandle(thread->handle);
	thread->handle = NULL;
	free(thread);
}

thread_t thread_self(void)
{
	return GetCurrentThreadId();
}

void thread_exit(void *retval)
{
	thread_t tid;
	thread_info_t *thread;

	tid = thread_self();
	thread = unlink_thread_info(tid);
	if (!thread) {
		return;
	}
	thread->retval = retval;
	if (!thread->has_waiter) {
		thread_info_destroy(thread);
	}
	_endthread();
}

void thread_cancel(thread_t tid)
{
#ifdef LIBPLAT_UWP
	abort();
#else
	thread_info_t *thread;

	thread = unlink_thread_info(tid);
	if (thread) {
		TerminateThread(thread->handle, 0);
		thread_info_destroy(thread);
	}
#endif
}

int thread_join(thread_t tid, void **retval)
{
	DWORD code;
	thread_info_t *thread;

	thread_mutex_lock(&thread_manager.mutex);
	thread = get_thread_info(tid);
	if (thread == NULL) {
		thread_mutex_unlock(&thread_manager.mutex);
		return -1;
	}
	thread->has_waiter = true;
	thread_mutex_unlock(&thread_manager.mutex);
	do {
		code = 0;
		if (GetExitCodeThread(thread->handle, &code) == 0) {
			break;
		}
		if (code == STILL_ACTIVE) {
			WaitForSingleObject(thread->handle, 5000);
		} else {
			break;
		}
	} while (1);
	thread = unlink_thread_info(tid);
	if (thread) {
		if (retval) {
			*retval = thread->retval;
		}
		thread_info_destroy(thread);
		return 0;
	}
	return -1;
}

#endif
