#define __IN_FRAME_CONTROL_SOURCE_FILE__

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
	unsigned int temp_fps;
	unsigned int current_fps;
	unsigned int one_frame_remain_time;
	unsigned int pause_time;
	int64_t prev_frame_start_time;
	int64_t prev_fps_update_time;
};

/** 初始化帧数控制 */
void FrameControl_Init( FrameCtrlCtx *ctx )
{
	ctx->temp_fps = 0;
	ctx->current_fps = 0;
	ctx->pause_time = 0;
	ctx->one_frame_remain_time = 10;
	ctx->prev_frame_start_time = LCUI_GetTickCount();
	ctx->prev_fps_update_time = LCUI_GetTickCount();
	LCUICond_Init( &ctx->wait_continue );
	LCUICond_Init( &ctx->wait_pause );
}

/** 设置最大FPS（帧数/秒） */
void FrameControl_SetMaxFPS( FrameCtrlCtx *ctx, unsigned int fps )
{
	ctx->one_frame_remain_time = (int)(1000.0/fps);
}

/** 让当前帧停留一定时间 */
void FrameControl_Remain( FrameCtrlCtx *ctx )
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
	/** 进行睡眠，直到需要暂停为止 */
	LCUICond_TimedWait( &ctx->wait_pause, n_ms );
	/** 睡眠结束后，如果当前状态不为PAUSE，则说明睡眠不是因为要暂停而终止的 */
	if( ctx->state != FRAME_CTRL_STATE_PAUSE ) {
		goto normal_exit;
	}
	/** 需要暂停，进行睡眠，直到需要继续为止 */
	lost_ms = LCUICond_TimedWait( &ctx->wait_continue, INT_MAX );
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
void FrameControl_Pause( FrameCtrlCtx *ctx, LCUI_BOOL need_pause )
{
	if( ctx->state == FRAME_CTRL_STATE_RUN && need_pause ) {
		LCUICond_Broadcast( &ctx->wait_pause );
		ctx->state = FRAME_CTRL_STATE_PAUSE;
	}
	else if( ctx->state == FRAME_CTRL_STATE_PAUSE && !need_pause ){
		LCUICond_Broadcast( &ctx->wait_continue );
		ctx->state = FRAME_CTRL_STATE_RUN;
	}
}
