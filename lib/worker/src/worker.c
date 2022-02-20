/* worker.c -- worker threading and task
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

#define LCUI_WORKER_C

#include <errno.h>
#include <stdlib.h>
#include <LCUI/def.h>
#include "../include/worker.h"
#include <LCUI/util.h>
#include <LCUI/thread.h>

typedef struct LCUI_WorkerRec_ {
	LCUI_BOOL active;		/**< 是否处于活动状态 */
	list_t tasks;			/**< 任务队列 */
	thread_mutex_t mutex;		/**< 互斥锁 */
	thread_cond_t cond;		/**< 条件变量 */
	thread_t thread;		/**< 所在的线程 */
} LCUI_WorkerRec;

void LCUITask_Destroy(LCUI_Task task)
{
	if (task->destroy_arg[0] && task->arg[0]) {
		task->destroy_arg[0](task->arg[0]);
	}
	if (task->destroy_arg[1] && task->arg[1]) {
		task->destroy_arg[1](task->arg[1]);
	}
}

int LCUITask_Run(LCUI_Task task)
{
	if (task && task->func) {
		task->func(task->arg[0], task->arg[1]);
		return 0;
	}
	return -1;
}

LCUI_Worker LCUIWorker_New(void)
{
	LCUI_Worker worker = malloc(sizeof(LCUI_WorkerRec));
	thread_mutex_init(&worker->mutex);
	thread_cond_init(&worker->cond);
	list_create(&worker->tasks);
	worker->active = FALSE;
	worker->thread = 0;
	return worker;
}

void LCUIWorker_PostTask(LCUI_Worker worker, LCUI_Task task)
{
	LCUI_Task newtask;
	newtask = malloc(sizeof(LCUI_TaskRec));
	*newtask = *task;
	thread_mutex_lock(&worker->mutex);
	list_append(&worker->tasks, newtask);
	thread_cond_signal(&worker->cond);
	thread_mutex_unlock(&worker->mutex);
}

LCUI_Task LCUIWorker_GetTask(LCUI_Worker worker)
{
	LCUI_Task task;
	list_node_t *node;

	node = list_get_node(&worker->tasks, 0);
	if (!node) {
		return NULL;
	}
	task = node->data;
	list_unlink(&worker->tasks, node);
	free(node);
	return task;
}

LCUI_BOOL LCUIWorker_RunTask(LCUI_Worker worker)
{
	LCUI_Task task;

	thread_mutex_lock(&worker->mutex);
	task = LCUIWorker_GetTask(worker);
	thread_mutex_unlock(&worker->mutex);
	if (!task) {
		return FALSE;
	}
	LCUITask_Run(task);
	LCUITask_Destroy(task);
	free(task);
	return TRUE;
}

static void OnDeleteTask(void *arg)
{
	LCUITask_Destroy(arg);
	free(arg);
}

static void LCUIWorker_ExecDestroy(LCUI_Worker worker)
{
	list_destroy(&worker->tasks, OnDeleteTask);
	thread_mutex_unlock(&worker->mutex);
	thread_mutex_destroy(&worker->mutex);
	thread_cond_destroy(&worker->cond);
	free(worker);
}

static void LCUIWorker_Thread(void *arg)
{
	LCUI_Task task;
	LCUI_Worker worker = arg;

	thread_mutex_lock(&worker->mutex);
	while (worker->active) {
		task = LCUIWorker_GetTask(worker);
		if (task) {
			thread_mutex_unlock(&worker->mutex);
			LCUITask_Run(task);
			LCUITask_Destroy(task);
			free(task);
			thread_mutex_lock(&worker->mutex);
			continue;
		}
		if (worker->active) {
			thread_cond_wait(&worker->cond, &worker->mutex);
		}
	}
	LCUIWorker_ExecDestroy(worker);
	thread_exit(NULL);
}

int LCUIWorker_RunAsync(LCUI_Worker worker)
{
	if (worker->thread != 0) {
		return -EEXIST;
	}
	worker->active = TRUE;
	thread_create(&worker->thread, LCUIWorker_Thread, worker);
	logger_debug("[worker] worker %u is running\n", worker->thread);
	return 0;
}

void LCUIWorker_Destroy(LCUI_Worker worker)
{
	thread_t thread = worker->thread;

	if (worker->active) {
		logger_debug("[worker] worker %u is stopping...\n", thread);
		thread_mutex_lock(&worker->mutex);
		worker->active = FALSE;
		thread_cond_signal(&worker->cond);
		thread_mutex_unlock(&worker->mutex);
		thread_join(thread, NULL);
		logger_debug("[worker] worker %u has stopped\n", thread);
		return;
	}
	LCUIWorker_ExecDestroy(worker);
}
