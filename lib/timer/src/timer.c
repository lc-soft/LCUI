﻿/* timer.c -- timer support.
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
#include <yutil.h>

/*----------------------------- Timer --------------------------------*/

static timer_list_t* main_timer_list = NULL;
static LCUI_Mutex mutex;     /**< 定时器记录操作互斥锁 */

/*----------------------------- Private ------------------------------*/

int lcui_destroy_timer(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_destroy(main_timer_list, timer_id);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

int lcui_pause_timer(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_pause(main_timer_list, timer_id);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

int lcui_continue_timer(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_continue(main_timer_list, timer_id);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

int lcui_reset_timer(int timer_id, long int n_ms)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_reset(main_timer_list, timer_id, n_ms);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

int lcui_set_timeout(long int n_ms, void(*callback)(void *), void *arg)
{
	return timer_list_add_timeout(main_timer_list, n_ms, callback, arg);
}

int lcui_set_interval(long int n_ms, void(*callback)(void *), void *arg)
{
	return timer_list_add_interval(main_timer_list, n_ms, callback, arg);
}

size_t lcui_process_timers()
{
	size_t count = 0;
	LCUIMutex_Lock(&mutex);
	count = timer_list_process(main_timer_list);
	LCUIMutex_Unlock(&mutex);
	return count;
}

void lcui_init_timers(void)
{
	LCUIMutex_Init(&mutex);
	main_timer_list = timer_list_create();
}

void lcui_destroy_timers()
{
	if(!main_timer_list){
		return;
	}
	timer_list_destroy(main_timer_list);
	LCUIMutex_Destroy(&mutex);
	main_timer_list = NULL;
}
