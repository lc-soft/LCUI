/* ***************************************************************************
 * steptimer.c -- step timer, Mainly used to control the count of frames per
 * second for the rendering loop.
 *
 * Copyright (C) 2014-2017 by Liu Chao <lc-soft@live.cn>
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
 * steptimer.c -- 步进计时器，主要用于控制渲染循环每秒更新的帧数。
 *
 * 版权所有 (C) 2014-2017 归属于 刘超 <lc-soft@live.cn>
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
