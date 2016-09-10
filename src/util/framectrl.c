/* ***************************************************************************
 * framectrl.c -- frame control, limit the maximum number of tasks for program
 * in 1 sec.
 *
 * Copyright (C) 2014-2016 by Liu Chao <lc-soft@live.cn>
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
 * framectrl.c -- 帧数控制，能够限制程序在一秒内执行的最大任务数量。
 *
 * 版权所有 (C) 2014-2016 归属于 刘超 <lc-soft@live.cn>
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

#define LCUI_UTIL_FRAMECTRL_C

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

enum FrameControlState {
	STATE_RUN,
	STATE_PAUSE,
	STATE_QUIT
};

typedef struct FrameControlRec_ {
	int state;
	LCUI_Cond cond;
	LCUI_Mutex mutex;
	unsigned int temp_fps;
	unsigned int current_fps;
	unsigned int one_frame_remain_time;
	unsigned int pause_time;
	int64_t prev_frame_start_time;
	int64_t prev_fps_update_time;
} FrameControlRec;

FrameControl FrameControl_Create( void )
{
	FrameControl ctx;
	ctx = NEW( FrameControlRec, 1 );
	ctx->temp_fps = 0;
	ctx->current_fps = 0;
	ctx->pause_time = 0;
	ctx->one_frame_remain_time = 10;
	ctx->prev_frame_start_time = LCUI_GetTime();
	ctx->prev_fps_update_time = LCUI_GetTime();
	LCUICond_Init( &ctx->cond );
	LCUIMutex_Init( &ctx->mutex );
	return ctx;
}

void FrameControl_Destroy( FrameControl ctx )
{
	LCUIMutex_Unlock( &ctx->mutex );
	LCUICond_Destroy( &ctx->cond );
	LCUIMutex_Destroy( &ctx->mutex );
	free( ctx );
}

void FrameControl_SetMaxFPS( FrameControl ctx, unsigned int fps )
{
	ctx->one_frame_remain_time = (int)(1000.0/fps);
}

int FrameControl_GetFPS( FrameControl ctx )
{
	return ctx->current_fps;
}

void FrameControl_Remain( FrameControl ctx )
{
	int64_t current_time;
	unsigned int n_ms, lost_ms;

	if( ctx->state == STATE_QUIT ) {
		return;
	}
	lost_ms = 0;
	current_time = LCUI_GetTime();
	LCUIMutex_Lock( &ctx->mutex );
	n_ms = (unsigned int)(current_time - ctx->prev_frame_start_time);
	if( n_ms > ctx->one_frame_remain_time ) {
		goto normal_exit;
	}
	n_ms = ctx->one_frame_remain_time - n_ms;
	if( n_ms < 1 ) {
		goto normal_exit;
	}
	/* 睡眠一段时间 */
	while( lost_ms < n_ms && ctx->state == STATE_RUN ) {
		LCUICond_TimedWait( &ctx->cond, &ctx->mutex, n_ms - lost_ms );
		lost_ms = (unsigned int)LCUI_GetTimeDelta( current_time );
	}
	/* 睡眠结束后，如果当前状态为 PAUSE，则说明睡眠是因为要暂停而终止的 */
	if( ctx->state == STATE_PAUSE ) {
		current_time = LCUI_GetTime();
		/* 等待状态改为“继续” */
		while( ctx->state == STATE_PAUSE ) {
			LCUICond_Wait( &ctx->cond, &ctx->mutex );
		}
		lost_ms = (unsigned int)LCUI_GetTimeDelta( current_time );
		ctx->pause_time = lost_ms;
		ctx->prev_frame_start_time += lost_ms;
		LCUIMutex_Unlock( &ctx->mutex );
		return;
	}

normal_exit:;
	current_time = LCUI_GetTime();
	if( current_time - ctx->prev_fps_update_time >= 1000 ) {
		ctx->current_fps = ctx->temp_fps;
		ctx->prev_fps_update_time = current_time;
		ctx->temp_fps = 0;
	}
	ctx->prev_frame_start_time = current_time;
	++ctx->temp_fps;
	LCUIMutex_Unlock( &ctx->mutex );
}

void FrameControl_Pause( FrameControl ctx, LCUI_BOOL need_pause )
{
	if( ctx->state == STATE_RUN && need_pause ) {
		LCUIMutex_Lock( &ctx->mutex );
		ctx->state = STATE_PAUSE;
		LCUICond_Signal( &ctx->cond );
		LCUIMutex_Unlock( &ctx->mutex );
	} else if( ctx->state == STATE_PAUSE && !need_pause ) {
		LCUIMutex_Lock( &ctx->mutex );
		ctx->state = STATE_RUN;
		LCUICond_Signal( &ctx->cond );
		LCUIMutex_Unlock( &ctx->mutex );
	}
}
