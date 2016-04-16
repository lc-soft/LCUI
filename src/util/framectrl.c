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

#define __IN_FRAME_CONTROL_SOURCE_FILE__

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

#define FRAME_CTRL_STATE_RUN	0
#define FRAME_CTRL_STATE_PAUSE	1
#define FRAME_CTRL_STATE_QUIT	2

struct FrameControlContext {
	int state;
	LCUI_Cond wait_continue;
	LCUI_Cond wait_pause;
	LCUI_Mutex mutex;
	unsigned int temp_fps;
	unsigned int current_fps;
	unsigned int one_frame_remain_time;
	unsigned int pause_time;
	int64_t prev_frame_start_time;
	int64_t prev_fps_update_time;
};

/** 新建帧数控制实例 */
FrameCtrlCtx FrameControl_Create( void )
{
	FrameCtrlCtx ctx;
	ctx = (FrameCtrlCtx)malloc(sizeof(struct FrameControlContext));
	ctx->temp_fps = 0;
	ctx->current_fps = 0;
	ctx->pause_time = 0;
	ctx->one_frame_remain_time = 10;
	ctx->prev_frame_start_time = LCUI_GetTickCount();
	ctx->prev_fps_update_time = LCUI_GetTickCount();
	LCUICond_Init( &ctx->wait_continue );
	LCUICond_Init( &ctx->wait_pause );
	LCUIMutex_Init( &ctx->mutex );
	LCUIMutex_Lock( &ctx->mutex );
	return ctx;
}

/** 销毁帧数控制相关资源 */
void FrameControl_Destroy( FrameCtrlCtx ctx )
{
	LCUIMutex_Unlock( &ctx->mutex );
	LCUICond_Destroy( &ctx->wait_continue );
	LCUICond_Destroy( &ctx->wait_pause );
	LCUIMutex_Destroy( &ctx->mutex );
	free( ctx );
}

/** 设置最大FPS（帧数/秒） */
void FrameControl_SetMaxFPS( FrameCtrlCtx ctx, unsigned int fps )
{
	ctx->one_frame_remain_time = (int)(1000.0/fps);
}

/** 获取当前FPS */
int FrameControl_GetFPS( FrameCtrlCtx ctx )
{
	return ctx->current_fps;
}

/** 让当前帧停留一定时间 */
void FrameControl_Remain( FrameCtrlCtx ctx )
{
	unsigned int n_ms, lost_ms;
	int64_t current_time;

	if( ctx->state == FRAME_CTRL_STATE_QUIT ) {
		return;
	}
	current_time = LCUI_GetTickCount();
	n_ms = (int)(current_time - ctx->prev_frame_start_time);
	if( n_ms > ctx->one_frame_remain_time ) {
		goto normal_exit;
	}
	n_ms = ctx->one_frame_remain_time - n_ms;
	if( n_ms < 1 ) {
		goto normal_exit;
	}
	/* 进行睡眠，直到需要暂停为止 */
	LCUICond_TimedWait( &ctx->wait_pause, &ctx->mutex, n_ms );
	/* 睡眠结束后，如果当前状态不为PAUSE，则说明睡眠不是因为要暂停而终止的 */
	if( ctx->state != FRAME_CTRL_STATE_PAUSE ) {
		goto normal_exit;
	}
	current_time = LCUI_GetTickCount();
	/* 需要暂停，进行睡眠，直到需要继续为止 */
	LCUICond_Wait( &ctx->wait_continue, &ctx->mutex );
	lost_ms = (unsigned int)LCUI_GetTicks( current_time );
	ctx->pause_time = lost_ms;
	ctx->prev_frame_start_time += lost_ms;
	return;

normal_exit:;
	current_time = LCUI_GetTickCount();
	if( current_time - ctx->prev_fps_update_time >= 1000 ) {
		ctx->current_fps = ctx->temp_fps;
		ctx->prev_fps_update_time = current_time;
		ctx->temp_fps = 0;
	}
	ctx->prev_frame_start_time = current_time;
	++ctx->temp_fps;
}

/** 暂停数据帧的更新 */
void FrameControl_Pause( FrameCtrlCtx ctx, LCUI_BOOL need_pause )
{
	if( ctx->state == FRAME_CTRL_STATE_RUN && need_pause ) {
		LCUIMutex_Lock( &ctx->mutex );
		ctx->state = FRAME_CTRL_STATE_PAUSE;
		LCUICond_Signal( &ctx->wait_pause );
		LCUIMutex_Unlock( &ctx->mutex );
	}
	else if( ctx->state == FRAME_CTRL_STATE_PAUSE && !need_pause ){
		LCUIMutex_Lock( &ctx->mutex );
		ctx->state = FRAME_CTRL_STATE_RUN;
		LCUICond_Signal( &ctx->wait_continue );
		LCUIMutex_Unlock( &ctx->mutex );
	}
}

