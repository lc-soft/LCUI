/* steptimer.c -- Step timer, Mainly used to control the count of frames per
 * second for the rendering loop.
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

#define LCUI_UTIL_STEPTIMER_C

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

enum StepTimerState {
	STATE_RUN,
	STATE_PAUSE,
	STATE_QUIT
};

typedef struct StepTimerRec_ {
	int state;
	LCUI_Cond cond;
	LCUI_Mutex mutex;
	unsigned int temp_fps;
	unsigned int current_fps;
	unsigned int one_frame_remain_time;
	unsigned int pause_time;
	int64_t prev_frame_start_time;
	int64_t prev_fps_update_time;
} StepTimerRec;

StepTimer StepTimer_Create( void )
{
	StepTimer timer;
	timer = NEW( StepTimerRec, 1 );
	timer->temp_fps = 0;
	timer->current_fps = 0;
	timer->pause_time = 0;
	timer->one_frame_remain_time = 10;
	timer->prev_frame_start_time = LCUI_GetTime();
	timer->prev_fps_update_time = LCUI_GetTime();
	LCUICond_Init( &timer->cond );
	LCUIMutex_Init( &timer->mutex );
	return timer;
}

void StepTimer_Destroy( StepTimer timer )
{
	LCUICond_Destroy( &timer->cond );
	LCUIMutex_Destroy( &timer->mutex );
	free( timer );
}

void StepTimer_SetFrameLimit( StepTimer timer, unsigned int max )
{
	timer->one_frame_remain_time = (int)(1000.0 / max);
}

int StepTimer_GetFrameCount( StepTimer timer )
{
	return timer->current_fps;
}

void StepTimer_Remain( StepTimer timer )
{
	int64_t current_time;
	unsigned int n_ms, lost_ms;

	if( timer->state == STATE_QUIT ) {
		return;
	}
	lost_ms = 0;
	current_time = LCUI_GetTime();
	LCUIMutex_Lock( &timer->mutex );
	n_ms = (unsigned int)(current_time - timer->prev_frame_start_time);
	if( n_ms > timer->one_frame_remain_time ) {
		goto normal_exit;
	}
	n_ms = timer->one_frame_remain_time - n_ms;
	if( n_ms < 1 ) {
		goto normal_exit;
	}
	/* 睡眠一段时间 */
	while( lost_ms < n_ms && timer->state == STATE_RUN ) {
		LCUICond_TimedWait( &timer->cond, &timer->mutex, n_ms - lost_ms );
		lost_ms = (unsigned int)LCUI_GetTimeDelta( current_time );
	}
	/* 睡眠结束后，如果当前状态为 PAUSE，则说明睡眠是因为要暂停而终止的 */
	if( timer->state == STATE_PAUSE ) {
		current_time = LCUI_GetTime();
		/* 等待状态改为“继续” */
		while( timer->state == STATE_PAUSE ) {
			LCUICond_Wait( &timer->cond, &timer->mutex );
		}
		lost_ms = (unsigned int)LCUI_GetTimeDelta( current_time );
		timer->pause_time = lost_ms;
		timer->prev_frame_start_time += lost_ms;
		LCUIMutex_Unlock( &timer->mutex );
		return;
	}

normal_exit:;
	current_time = LCUI_GetTime();
	if( current_time - timer->prev_fps_update_time >= 1000 ) {
		timer->current_fps = timer->temp_fps;
		timer->prev_fps_update_time = current_time;
		timer->temp_fps = 0;
	}
	timer->prev_frame_start_time = current_time;
	++timer->temp_fps;
	LCUIMutex_Unlock( &timer->mutex );
}

void StepTimer_Pause( StepTimer timer, LCUI_BOOL need_pause )
{
	if( timer->state == STATE_RUN && need_pause ) {
		LCUIMutex_Lock( &timer->mutex );
		timer->state = STATE_PAUSE;
		LCUICond_Signal( &timer->cond );
		LCUIMutex_Unlock( &timer->mutex );
	} else if( timer->state == STATE_PAUSE && !need_pause ) {
		LCUIMutex_Lock( &timer->mutex );
		timer->state = STATE_RUN;
		LCUICond_Signal( &timer->cond );
		LCUIMutex_Unlock( &timer->mutex );
	}
}
