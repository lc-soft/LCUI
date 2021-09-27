/*
 * steptimer.h -- step timer, Mainly used to control the count of frames per
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

#ifndef LCUI_UTIL_STEPTIMER_H
#define LCUI_UTIL_STEPTIMER_H

LCUI_BEGIN_HEADER

#ifdef LCUI_UTIL_STEPTIMER_C
typedef struct StepTimerRec_* StepTimer;
#else
typedef void* StepTimer;
#endif

/** 新建帧数控制实例 */
LCUI_API StepTimer StepTimer_Create(void);

/** 销毁帧数控制相关资源 */
LCUI_API void StepTimer_Destroy(StepTimer timer);

/** 设置最大FPS（帧数/秒） */
LCUI_API void StepTimer_SetFrameLimit(StepTimer timer, unsigned int max);

/** 获取当前FPS */
LCUI_API int StepTimer_GetFrameCount(StepTimer timer);

/** 让当前帧停留一定时间 */
LCUI_API void StepTimer_Remain(StepTimer timer);

/** 暂停数据帧的更新 */
LCUI_API void StepTimer_Pause(StepTimer timer, LCUI_BOOL need_pause);

LCUI_END_HEADER

#endif

