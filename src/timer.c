/* timer.c -- timer support.
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

#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/timer.h>

#define STATE_RUN 1
#define STATE_PAUSE 0

/*----------------------------- Timer --------------------------------*/

typedef struct TimerRec_ {
	int state;			/**< 状态 */
	long int id;			/**< 定时器ID */
	LCUI_BOOL reuse;		/**< 是否重复使用该定时器 */

	int64_t start_time;		/**< 定时器启动时的时间 */
	int64_t pause_time;		/**< 定时器暂停时的时间 */
	long int total_ms;		/**< 定时时间（单位：毫秒） */
	long int pause_ms;		/**< 定时器处于暂停状态的时长（单位：毫秒） */

	void (*callback)(void *);	/**< 回调函数 */
	void *arg;			/**< 函数的参数 */

	LinkedListNode node;		/**< 位于定时器列表中的节点 */
} TimerRec, *Timer;

static struct TimerModule {
	int id_count;         /**< 定时器ID计数 */
	LCUI_BOOL active;     /**< 定时器线程是否正在运行 */
	LCUI_Mutex mutex;     /**< 定时器记录操作互斥锁 */
	LinkedList timers;    /**< 定时器数据记录 */
} self;

/*----------------------------- Private ------------------------------*/

/** 更新定时器在定时器列表中的位置 */
static void TimerList_AddNode(LinkedListNode *node)
{
	Timer timer;
	int64_t t, tt;
	LinkedListNode *cur;
	/* 计算该定时器的剩余定时时长 */
	timer = node->data;
	t = LCUI_GetTimeDelta(timer->start_time);
	t = timer->total_ms - t + timer->pause_ms;
	for (LinkedList_Each(cur, &self.timers)) {
		timer = cur->data;
		tt = LCUI_GetTimeDelta(timer->start_time);
		tt = timer->total_ms - tt + timer->pause_ms;
		if (t <= tt) {
			LinkedList_Link(&self.timers, cur->prev, node);
			return;
		}
	}
	LinkedList_AppendNode(&self.timers, node);
}

static Timer FindTimer(int timer_id)
{
	Timer timer;
	LinkedListNode *node;
	for (LinkedList_Each(node, &self.timers)) {
		timer = node->data;
		if (timer && timer->id == timer_id) {
			return timer;
		}
	}
	return NULL;
}

int LCUITimer_Set(long int n_ms, void(*func)(void *), void *arg,
		  LCUI_BOOL reuse)
{
	Timer timer;
	if (!self.active) {
		return -1;
	}
	LCUIMutex_Lock(&self.mutex);
	timer = malloc(sizeof(TimerRec));
	timer->arg = arg;
	timer->callback = func;
	timer->reuse = reuse;
	timer->pause_ms = 0;
	timer->total_ms = n_ms;
	timer->state = STATE_RUN;
	timer->id = ++self.id_count;
	timer->start_time = LCUI_GetTime();
	timer->node.next = NULL;
	timer->node.prev = NULL;
	timer->node.data = timer;
	TimerList_AddNode(&timer->node);
	LCUIMutex_Unlock(&self.mutex);
	DEBUG_MSG("set timer, id: %ld, total_ms: %ld\n", timer->id,
		  timer->total_ms);
	return timer->id;
}

int LCUITimer_Free(int timer_id)
{
	Timer timer;
	if (!self.active) {
		return -1;
	}
	LCUIMutex_Lock(&self.mutex);
	timer = FindTimer(timer_id);
	if (!timer) {
		LCUIMutex_Unlock(&self.mutex);
		return -1;
	}
	LinkedList_Unlink(&self.timers, &timer->node);
	free(timer);
	LCUIMutex_Unlock(&self.mutex);
	return 0;
}

int LCUITimer_Pause(int timer_id)
{
	Timer timer;
	if (!self.active) {
		return -1;
	}
	LCUIMutex_Lock(&self.mutex);
	timer = FindTimer(timer_id);
	if (timer) {
		/* 记录暂停时的时间 */
		timer->pause_time = LCUI_GetTime();
		timer->state = STATE_PAUSE;
	}
	LCUIMutex_Unlock(&self.mutex);
	return timer ? 0 : -1;
}

int LCUITimer_Continue(int timer_id)
{
	Timer timer;
	if (!self.active) {
		return -1;
	}
	LCUIMutex_Lock(&self.mutex);
	timer = FindTimer(timer_id);
	if (timer) {
		/* 计算处于暂停状态的时长 */
		timer->pause_ms +=
		    (long int)LCUI_GetTimeDelta(timer->pause_time);
		timer->state = STATE_RUN;
	}
	LCUIMutex_Unlock(&self.mutex);
	return timer ? 0 : -1;
}

int LCUITimer_Reset(int timer_id, long int n_ms)
{
	Timer timer;
	if (!self.active) {
		return -1;
	}
	LCUIMutex_Lock(&self.mutex);
	timer = FindTimer(timer_id);
	if (timer) {
		timer->pause_ms = 0;
		timer->total_ms = n_ms;
		timer->start_time = LCUI_GetTime();
	}
	LCUIMutex_Unlock(&self.mutex);
	return timer ? 0 : -1;
}

int LCUI_SetTimeout(long int n_ms, void(*callback)(void *), void *arg)
{
	return LCUITimer_Set(n_ms, callback, arg, FALSE);
}

int LCUI_SetInterval(long int n_ms, void(*callback)(void *), void *arg)
{
	return LCUITimer_Set(n_ms, callback, arg, TRUE);
}

size_t LCUI_ProcessTimers(void)
{
	size_t count = 0;
	long lost_ms;

	Timer timer = NULL;
	LinkedListNode *node;

	LCUIMutex_Lock(&self.mutex);
	while(self.active) {
		for (LinkedList_Each(node, &self.timers)) {
			timer = node->data;
			if (timer && timer->state == STATE_RUN) {
				break;
			}
		}
		if (!node) {
			break;
		}
		count += 1;
		lost_ms = (long)LCUI_GetTimeDelta(timer->start_time);
		/* 若流失的时间未达到总定时时长 */
		if (lost_ms - timer->pause_ms < timer->total_ms) {
			break;
		}
		/* 若需要重复使用，则重置剩余等待时间 */
		LinkedList_Unlink(&self.timers, node);
		LCUI_PostSimpleTask(timer->callback, timer->arg, NULL);
		if (timer->reuse) {
			timer->pause_ms = 0;
			timer->start_time = LCUI_GetTime();
			TimerList_AddNode(node);
		} else {
			free(timer);
		}
	}
	LCUIMutex_Unlock(&self.mutex);
	return count;
}

void LCUI_InitTimer(void)
{
	self.active = TRUE;
	LCUITime_Init();
	LCUIMutex_Init(&self.mutex);
	LinkedList_Init(&self.timers);
}

void LCUI_FreeTimer(void)
{
	if (!self.active) {
		return;
	}
	self.active = FALSE;
	LCUIMutex_Lock(&self.mutex);
	LCUIMutex_Unlock(&self.mutex);
	LinkedList_ClearData(&self.timers, free);
	LCUIMutex_Destroy(&self.mutex);
}
