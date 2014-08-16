#ifndef  __LC_FRAME_CONTROL_H__
#define __LC_FRAME_CONTROL_H__

LCUI_BEGIN_HEADER

#ifdef __IN_FRAME_CONTROL_SOURCE_FILE__
typedef struct FrameControlContext FrameCtrlCtx;
#else
typedef void* FrameCtrlCtx;
#endif

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

