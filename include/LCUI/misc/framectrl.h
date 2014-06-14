#ifndef  __LC_FRAME_CONTROL_H__
#define __LC_FRAME_CONTROL_H__

LCUI_BEGIN_HEADER

typedef struct FrameControlContext {
	int state;
	LCUI_Sleeper wait_continue;
	LCUI_Sleeper wait_pause;
	unsigned int temp_fps;
	unsigned int current_fps;
	unsigned int one_frame_remain_time;
	unsigned int pause_time;
	int64_t prev_frame_start_time;
	int64_t prev_fps_update_time;
} FrameCtrlCtx;

/** 初始化帧数控制 */
LCUI_API void FrameControl_Init( FrameCtrlCtx *ctx );

/** 设置最大FPS（帧数/秒） */
LCUI_API void FrameControl_SetMaxFPS( FrameCtrlCtx *ctx, unsigned int fps );

/** 让当前帧停留一定时间 */
LCUI_API void FrameControl_Remain( FrameCtrlCtx *ctx );

/** 暂停数据帧的更新 */
LCUI_API void FrameControl_Pause( FrameCtrlCtx *ctx, LCUI_BOOL need_pause );

LCUI_END_HEADER

#endif

