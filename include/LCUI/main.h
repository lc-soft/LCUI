/*
 * main.h -- The main functions for the LCUI normal work.
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

#ifndef LCUI_MAIN_H
#define LCUI_MAIN_H

LCUI_BEGIN_HEADER

#ifndef LCUI_MAIN_C
typedef void *LCUI_MainLoop;
#else
typedef struct LCUI_MainLoopRec_ *LCUI_MainLoop;
#endif

/** 处理当前所有事件 */
LCUI_API size_t LCUI_ProcessEvents(void);

/**
 * 添加任务
 * 该任务将会添加至主线程中执行
 */
LCUI_API LCUI_BOOL lcui_post_task(LCUI_Task task);

/**
 * 添加异步任务
 * 该任务将会添加至指定 id 的工作线程中执行
 * @param[in] task 任务数据
 * @param[in] target_worker_id 目标工作线程的编号
 */
LCUI_API void lcui_post_async_task(LCUI_Task task, int target_worker_id);

/**
 * 添加异步任务
 * 该任务将会添加至工作线程中执行
 */
LCUI_API int LCUI_PostAsyncTask(LCUI_Task task);

/** lcui_post_task 的简化版本 */
#define LCUI_PostSimpleTask(FUNC, ARG1, ARG2)             \
	do {                                              \
		LCUI_TaskRec _ui_task = { 0 };            \
		_ui_task.arg[0] = (void *)ARG1;           \
		_ui_task.arg[1] = (void *)ARG2;           \
		_ui_task.func = (LCUI_AppTaskFunc)(FUNC); \
		lcui_post_task(&_ui_task);                 \
	} while (0);

LCUI_API void LCUI_RunFrame(void);

LCUI_API void LCUI_RunFrameWithProfile(LCUI_FrameProfile profile);

/* 新建一个主循环 */
LCUI_API LCUI_MainLoop LCUIMainLoop_New(void);

/* 运行目标循环 */
LCUI_API int LCUIMainLoop_Run(LCUI_MainLoop loop);

/* 标记目标主循环需要退出 */
LCUI_API void LCUIMainLoop_Quit(LCUI_MainLoop loop);

LCUI_API void LCUIMainLoop_Destroy(LCUI_MainLoop loop);

LCUI_API void lcui_init_base(void);

/** 初始化 LCUI 各项功能 */
LCUI_API void lcui_init(void);

LCUI_API int lcui_main(void);

/** 获取LCUI的版本 */
LCUI_API const char *LCUI_GetVersion(void);

/** 释放LCUI占用的资源 */
LCUI_API int lcui_destroy(void);

/** 退出LCUI，释放LCUI占用的资源 */
LCUI_API void lcui_quit(void);

/** 退出 LCUI，并设置退出码 */
LCUI_API void lcui_exit(int code);

LCUI_END_HEADER

#endif
