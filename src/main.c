﻿/* main.c -- The main functions for the LCUI normal work
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
#define LCUI_MAIN_C
#include "config.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/worker.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
#include <LCUI/input.h>
#include <LCUI/ime.h>
#include <LCUI/platform.h>
#include <LCUI/display.h>
#include <LCUI/settings.h>
#ifdef LCUI_EVENTS_H
#include LCUI_EVENTS_H
#endif
#ifdef LCUI_MOUSE_H
#include LCUI_MOUSE_H
#endif
#ifdef LCUI_KEYBOARD_H
#include LCUI_KEYBOARD_H
#endif
#ifdef LCUI_DISPLAY_H
#include LCUI_DISPLAY_H
#endif
#include <LCUI/font.h>

#define STATE_ACTIVE 1
#define STATE_KILLED 0

typedef struct LCUI_MainLoopRec_ {
	int state;       /**< 主循环的状态 */
	LCUI_Thread tid; /**< 当前运行该主循环的线程的ID */
} LCUI_MainLoopRec;

/** 主循环的状态 */
enum LCUI_MainLoopState { STATE_PAUSED, STATE_RUNNING, STATE_EXITED };

typedef struct SysEventHandlerRec_ {
	LCUI_SysEventFunc func;
	void *data;
	void (*destroy_data)(void *);
} SysEventHandlerRec, *SysEventHandler;

typedef struct SysEventPackRec_ {
	LCUI_SysEvent event;
	void *arg;
} SysEventPackRec, *SysEventPack;

/* clang-format off */

/** LCUI 系统相关数据 */
static struct LCUI_System {
	int state;				/**< 状态 */
	int mode;				/**< LCUI的运行模式 */
	int exit_code;				/**< 退出码 */
	LCUI_Thread thread;			/**< 主线程 */
	struct {
		LCUI_EventTrigger trigger;	/**< 系统事件容器 */
		LCUI_Mutex mutex;		/**< 互斥锁 */
	} event;
} System;

#define LCUI_WORKER_NUM 4

/** LCUI 应用程序数据 */
static struct LCUI_App {
	LCUI_BOOL active;			/**< 是否已经初始化并处于活动状态 */
	LCUI_Mutex loop_mutex;			/**< 互斥锁，确保一次只允许一个线程跑主循环 */
	LCUI_Cond loop_changed;			/**< 条件变量，用于指示当前运行的主循环是否改变 */
	LCUI_MainLoop loop;			/**< 当前运行的主循环 */
	list_t loops;			/**< 主循环列表 */
	StepTimer timer;			/**< 渲染循环计数器 */
	LCUI_AppDriver driver;			/**< 程序事件驱动支持 */
	LCUI_BOOL driver_ready;			/**< 事件驱动支持是否已经准备就绪 */
	LCUI_Worker main_worker;		/**< 主工作线程 */
	LCUI_Worker workers[LCUI_WORKER_NUM];	/**< 普通工作线程 */
	int worker_next;			/**< 下一个工作线程编号 */
	LCUI_SettingsRec settings;
	LCUI_ProfileRec profile;
	LCUI_FrameProfile frame;
	int settings_change_handler_id;
} MainApp;

/* clang-format on */
static void LCUIProfile_Init(LCUI_Profile profile)
{
	memset(profile, 0, sizeof(LCUI_ProfileRec));
	profile->start_time = clock();
}

static void LCUIProfile_Print(LCUI_Profile profile)
{
	unsigned i;
	LCUI_FrameProfile frame;

	logger_debug("\nframes_count: %zu, time: %ld\n", profile->frames_count,
		     profile->end_time - profile->start_time);
	for (i = 0; i < profile->frames_count; ++i) {
		frame = &profile->frames[i];
		logger_debug("=== frame [%u/%u] ===\n", i + 1,
			     profile->frames_count);
		logger_debug("timers.count: %zu\ntimers.time: %ldms\n",
			     frame->timers_count, frame->timers_time);
		logger_debug("events.count: %zu\nevents.time: %ldms\n",
			     frame->events_count, frame->events_time);
		logger_debug("widget_tasks.time: %ldms\n"
			     "widget_tasks.update_count: %u\n"
			     "widget_tasks.refresh_count: %u\n"
			     "widget_tasks.layout_count: %u\n"
			     "widget_tasks.user_task_count: %u\n"
			     "widget_tasks.destroy_count: %u\n"
			     "widget_tasks.destroy_time: %ldms\n",
			     frame->widget_tasks.time,
			     frame->widget_tasks.update_count,
			     frame->widget_tasks.refresh_count,
			     frame->widget_tasks.layout_count,
			     frame->widget_tasks.user_task_count,
			     frame->widget_tasks.destroy_count,
			     frame->widget_tasks.destroy_time);
		logger_debug("render: %zu, %ldms, %ldms\n", frame->render_count,
			     frame->render_time, frame->present_time);
	}
}

static LCUI_FrameProfile LCUIProfile_BeginFrame(LCUI_Profile profile,
						LCUI_Settings settings)
{
	LCUI_FrameProfile frame;

	frame = &profile->frames[profile->frames_count];
	if (profile->frames_count > (unsigned)settings->frame_rate_cap) {
		profile->frames_count = 0;
	}
	memset(frame, 0, sizeof(LCUI_FrameProfileRec));
	return frame;
}

static void LCUIProfile_EndFrame(LCUI_Profile profile, LCUI_Settings settings)
{
	profile->frames_count += 1;
	profile->end_time = clock();
	if (profile->end_time - profile->start_time >= CLOCKS_PER_SEC) {
		if (profile->frames_count < (unsigned)settings->frame_rate_cap / 4) {
			LCUIProfile_Print(profile);
		}
		profile->frames_count = 0;
		profile->start_time = profile->end_time;
	}
}

static void OnSettingsChangeEvent(LCUI_SysEvent e, void *arg)
{
	Settings_Init(&MainApp.settings);
	StepTimer_SetFrameLimit(MainApp.timer, MainApp.settings.frame_rate_cap);
}

void LCUI_RunFrameWithProfile(LCUI_FrameProfile profile)
{
	profile->timers_time = clock();
	profile->timers_count = lcui_process_timers();
	profile->timers_time = clock() - profile->timers_time;

	profile->events_time = clock();
	profile->events_count = LCUI_ProcessEvents();
	profile->events_time = clock() - profile->events_time;

	LCUICursor_Update();
	LCUIWidget_UpdateWithProfile(&profile->widget_tasks);

	profile->render_time = clock();
	LCUIDisplay_Update();
	profile->render_count = LCUIDisplay_Render();
	profile->render_time = clock() - profile->render_time;

	profile->present_time = clock();
	LCUIDisplay_Present();
	profile->present_time = clock() - profile->present_time;
}

void LCUI_RunFrame(void)
{
	lcui_process_timers();
	LCUI_ProcessEvents();
	LCUICursor_Update();
	LCUIWidget_Update();
	LCUIDisplay_Update();
	LCUIDisplay_Render();
	LCUIDisplay_Present();
}

static void LCUI_InitEvent(void)
{
	LCUIMutex_Init(&System.event.mutex);
	System.event.trigger = EventTrigger();
}

static void LCUI_FreeEvent(void)
{
	LCUIMutex_Destroy(&System.event.mutex);
	EventTrigger_Destroy(System.event.trigger);
	System.event.trigger = NULL;
}

static void OnEvent(LCUI_Event e, void *arg)
{
	SysEventHandler handler = e->data;
	SysEventPack pack = arg;
	pack->event->type = e->type;
	pack->event->data = handler->data;
	handler->func(pack->event, pack->arg);
}

static void DestroySysEventHandler(void *arg)
{
	SysEventHandler handler = arg;
	if (handler->data && handler->destroy_data) {
		handler->destroy_data(handler->data);
	}
	handler->data = NULL;
	free(arg);
}

int LCUI_BindEvent(int id, LCUI_SysEventFunc func, void *data,
		   void (*destroy_data)(void *))
{
	int ret;
	SysEventHandler handler;
	if (System.state != STATE_ACTIVE) {
		return -1;
	}
	handler = NEW(SysEventHandlerRec, 1);
	handler->func = func;
	handler->data = data;
	handler->destroy_data = destroy_data;
	LCUIMutex_Lock(&System.event.mutex);
	ret = EventTrigger_Bind(System.event.trigger, id, OnEvent, handler,
				DestroySysEventHandler);
	LCUIMutex_Unlock(&System.event.mutex);
	return ret;
}

int LCUI_UnbindEvent(int handler_id)
{
	int ret;
	if (System.state != STATE_ACTIVE) {
		return -1;
	}
	LCUIMutex_Lock(&System.event.mutex);
	ret = EventTrigger_Unbind2(System.event.trigger, handler_id);
	LCUIMutex_Unlock(&System.event.mutex);
	return ret;
}

int LCUI_TriggerEvent(LCUI_SysEvent e, void *arg)
{
	if (System.state != STATE_ACTIVE) {
		return -1;
	}
	int ret;
	SysEventPackRec pack;
	pack.arg = arg;
	pack.event = e;
	LCUIMutex_Lock(&System.event.mutex);
	ret = EventTrigger_Trigger(System.event.trigger, e->type, &pack);
	LCUIMutex_Unlock(&System.event.mutex);
	return ret;
}

int LCUI_CreateTouchEvent(LCUI_SysEvent e, LCUI_TouchPoint points, int n_points)
{
	e->type = LCUI_TOUCH;
	e->touch.n_points = n_points;
	e->touch.points = NEW(LCUI_TouchPointRec, n_points);
	if (!e->touch.points) {
		return -ENOMEM;
	}
	for (n_points -= 1; n_points >= 0; --n_points) {
		e->touch.points[n_points] = points[n_points];
	}
	return 0;
}

void LCUI_DestroyEvent(LCUI_SysEvent e)
{
	switch (e->type) {
	case LCUI_TOUCH:
		if (e->touch.points) {
			free(e->touch.points);
		}
		e->touch.points = NULL;
		e->touch.n_points = 0;
		break;
	case LCUI_TEXTINPUT:
		if (e->text.text) {
			free(e->text.text);
		}
		e->text.text = NULL;
		e->text.length = 0;
		break;
	}
	e->type = LCUI_NONE;
}

size_t LCUI_ProcessEvents(void)
{
	size_t count = 0;

	if (MainApp.driver_ready) {
		MainApp.driver->ProcessEvents();
	}
	while (LCUIWorker_RunTask(MainApp.main_worker)) {
		++count;
	}
	return count;
}

LCUI_BOOL LCUI_PostTask(LCUI_Task task)
{
	if (!MainApp.main_worker) {
		return FALSE;
	}
	LCUIWorker_PostTask(MainApp.main_worker, task);
	return TRUE;
}

void LCUI_PostAsyncTaskTo(LCUI_Task task, int worker_id)
{
	int id = 0;
	if (!MainApp.active) {
		LCUITask_Run(task);
		LCUITask_Destroy(task);
		return;
	}
	if (id >= LCUI_WORKER_NUM) {
		id = 0;
	}
	LCUIWorker_PostTask(MainApp.workers[id], task);
}

int LCUI_PostAsyncTask(LCUI_Task task)
{
	int id;
	if (MainApp.worker_next >= LCUI_WORKER_NUM) {
		MainApp.worker_next = 0;
	}
	id = MainApp.worker_next;
	LCUI_PostAsyncTaskTo(task, id);
	MainApp.worker_next += 1;
	return id;
}

/* 新建一个主循环 */
LCUI_MainLoop LCUIMainLoop_New(void)
{
	LCUI_MainLoop loop;
	loop = NEW(LCUI_MainLoopRec, 1);
	loop->state = STATE_PAUSED;
	loop->tid = 0;
	return loop;
}

/** 运行目标主循环 */
int LCUIMainLoop_Run(LCUI_MainLoop loop)
{
	LCUI_BOOL at_same_thread = FALSE;
	if (loop->state == STATE_RUNNING) {
		DEBUG_MSG("error: main-loop already running.\n");
		return -1;
	}
	loop->state = STATE_RUNNING;
	loop->tid = LCUIThread_SelfID();
	if (MainApp.loop) {
		at_same_thread = MainApp.loop->tid == loop->tid;
	}
	DEBUG_MSG("at_same_thread: %d\n", at_same_thread);
	if (!at_same_thread) {
		LCUIMutex_Lock(&MainApp.loop_mutex);
		list_insert(&MainApp.loops, 0, loop);
		LCUIMutex_Unlock(&MainApp.loop_mutex);
	} else {
		list_insert(&MainApp.loops, 0, loop);
	}
	DEBUG_MSG("loop: %p, enter\n", loop);
	MainApp.loop = loop;
	while (loop->state != STATE_EXITED) {
		if (MainApp.settings.record_profile) {
			MainApp.frame = LCUIProfile_BeginFrame(
			    &MainApp.profile, &MainApp.settings);
			LCUI_RunFrameWithProfile(MainApp.frame);
			LCUIProfile_EndFrame(&MainApp.profile,
					     &MainApp.settings);
		} else {
			LCUI_RunFrame();
		}

		StepTimer_Remain(MainApp.timer);
		/* 如果当前运行的主循环不是自己 */
		while (MainApp.loop != loop) {
			loop->state = STATE_PAUSED;
			LCUICond_Wait(&MainApp.loop_changed,
				      &MainApp.loop_mutex);
		}
	}
	loop->state = STATE_EXITED;
	DEBUG_MSG("loop: %p, exit\n", loop);
	LCUIMainLoop_Destroy(loop);
	list_delete(&MainApp.loops, 0);
	/* 获取处于列表表头的主循环 */
	loop = list_get(&MainApp.loops, 0);
	/* 改变当前运行的主循环 */
	MainApp.loop = loop;
	LCUICond_Broadcast(&MainApp.loop_changed);
	return 0;
}

void LCUIMainLoop_Quit(LCUI_MainLoop loop)
{
	loop->state = STATE_EXITED;
}

void LCUIMainLoop_Destroy(LCUI_MainLoop loop)
{
	free(loop);
}

int LCUI_GetFrameCount(void)
{
	return StepTimer_GetFrameCount(MainApp.timer);
}

void LCUI_InitApp(LCUI_AppDriver app)
{
	int i;
	if (MainApp.driver_ready) {
		return;
	}
	MainApp.driver_ready = FALSE;
	MainApp.timer = StepTimer_Create();
	LCUICond_Init(&MainApp.loop_changed);
	LCUIMutex_Init(&MainApp.loop_mutex);
	list_create(&MainApp.loops);
	LCUIProfile_Init(&MainApp.profile);
	LCUI_ResetSettings();
	MainApp.settings_change_handler_id = LCUI_BindEvent(
	    LCUI_SETTINGS_CHANGE, OnSettingsChangeEvent, NULL, NULL);
	Settings_Init(&MainApp.settings);
	MainApp.main_worker = LCUIWorker_New();
	for (i = 0; i < LCUI_WORKER_NUM; ++i) {
		MainApp.workers[i] = LCUIWorker_New();
		LCUIWorker_RunAsync(MainApp.workers[i]);
	}
	StepTimer_SetFrameLimit(MainApp.timer, MainApp.settings.frame_rate_cap);
	if (!app) {
		app = LCUI_CreateAppDriver();
		if (!app) {
			return;
		}
	}
	MainApp.active = TRUE;
	MainApp.driver = app;
	MainApp.driver_ready = TRUE;
}

LCUI_AppDriverId LCUI_GetAppId(void)
{
	if (MainApp.driver) {
		return MainApp.driver->id;
	}
	return LCUI_APP_UNKNOWN;
}

static void OnDeleteMainLoop(void *arg)
{
	LCUIMainLoop_Destroy(arg);
}

static void LCUI_FreeApp(void)
{
	int i;
	LCUI_MainLoop loop;
	list_node_t *node;
	MainApp.active = FALSE;
	LCUI_UnbindEvent(MainApp.settings_change_handler_id);
	MainApp.settings_change_handler_id = -1;
	for (list_each(node, &MainApp.loops)) {
		loop = node->data;
		LCUIMainLoop_Quit(loop);
		LCUIThread_Join(loop->tid, NULL);
	}
	StepTimer_Destroy(MainApp.timer);
	LCUIMutex_Destroy(&MainApp.loop_mutex);
	LCUICond_Destroy(&MainApp.loop_changed);
	list_destroy(&MainApp.loops, OnDeleteMainLoop);
	if (MainApp.driver_ready) {
		LCUI_DestroyAppDriver(MainApp.driver);
	}
	MainApp.driver_ready = FALSE;
	for (i = 0; i < LCUI_WORKER_NUM; ++i) {
		LCUIWorker_Destroy(MainApp.workers[i]);
		MainApp.workers[i] = NULL;
	}
	LCUIWorker_Destroy(MainApp.main_worker);
	MainApp.main_worker = NULL;
}

int LCUI_BindSysEvent(int event_id, LCUI_EventFunc func, void *data,
		      void (*destroy_data)(void *))
{
	if (MainApp.driver_ready) {
		return MainApp.driver->BindSysEvent(event_id, func, data,
						    destroy_data);
	}
	return -1;
}

int LCUI_UnbindSysEvent(int event_id, LCUI_EventFunc func)
{
	if (MainApp.driver_ready) {
		return MainApp.driver->UnbindSysEvent(event_id, func);
	}
	return -1;
}

void *LCUI_GetAppData(void)
{
	if (MainApp.driver_ready) {
		return MainApp.driver->GetData();
	}
	return NULL;
}

static void LCUIApp_QuitAllMainLoop(void)
{
	LCUI_MainLoop loop;
	list_node_t *node;
	for (list_each(node, &MainApp.loops)) {
		loop = node->data;
		if (loop) {
			loop->state = STATE_EXITED;
		}
	}
}

static void LCUI_ShowCopyrightText(void)
{
	logger_log(LOGGER_LEVEL_INFO,
		   "LCUI (LC's UI) version " PACKAGE_VERSION "\n"
#ifdef _MSC_VER
		   "Build tool: "
#if (_MSC_VER > 1912)
		   "MS VC++ (higher version)"
#elif (_MSC_VER >= 1910 && _MSC_VER <= 1912)
		   "MS VC++ 14.1 (VisualStudio 2017)"
#elif (_MSC_VER == 1900)
		   "MS VC++ 14.0 (VisualStudio 2015)"
#elif (_MSC_VER == 1800)
		   "MS VC++ 12.0 (VisualStudio 2013)"
#elif (_MSC_VER == 1700)
		   "MS VC++ 11.0 (VisualStudio 2012)"
#elif (_MSC_VER == 1600)
		   "MS VC++ 10.0 (VisualStudio 2010)"
#else
		   "MS VC++ (older version)"
#endif
		   "\n"
#endif
		   "Build at "__DATE__
		   " - "__TIME__
		   "\n"
		   "Copyright (C) 2012-2021 Liu Chao <root@lc-soft.io>.\n"
		   "This is open source software, licensed under MIT. \n"
		   "See source distribution for detailed copyright notices.\n"
		   "To learn more, visit http://www.lcui.org.\n\n");
}

LCUI_BOOL LCUI_IsActive(void)
{
	if (System.state == STATE_ACTIVE) {
		return TRUE;
	}
	return FALSE;
}

LCUI_BOOL LCUI_IsOnMainLoop(void)
{
	if (!MainApp.loop) {
		return FALSE;
	}
	return (MainApp.loop->tid == LCUIThread_SelfID());
}

#ifdef LCUI_BUILD_IN_WIN32

static void Win32Logger_LogA(const char *str)
{
	OutputDebugStringA(str);
}

static void Win32Logger_LogW(const wchar_t *wcs)
{
	OutputDebugStringW(wcs);
}

#endif

void LCUI_InitPresetWidgets(void)
{
	LCUIWidget_AddTextView();
	LCUIWidget_AddCanvas();
	LCUIWidget_AddAnchor();
	LCUIWidget_AddButton();
	LCUIWidget_AddSideBar();
	LCUIWidget_AddTScrollBar();
	LCUIWidget_AddTextCaret();
	LCUIWidget_AddTextEdit();
}

void LCUI_InitBase(void)
{
	if (System.state == STATE_ACTIVE) {
		return;
	}
#ifdef LCUI_BUILD_IN_WIN32
	logger_set_handler(Win32Logger_LogA);
	logger_set_handler_w(Win32Logger_LogW);
#endif
	System.exit_code = 0;
	System.state = STATE_ACTIVE;
	System.thread = LCUIThread_SelfID();
	LCUI_ShowCopyrightText();
	LCUI_InitEvent();
	LCUI_InitFontLibrary();
	lcui_init_timers();
	LCUI_InitCursor();
	LCUI_InitWidget();
	LCUI_InitMetrics();
	LCUI_InitPresetWidgets();
}

void LCUI_Init(void)
{
	LCUI_InitBase();
	LCUI_InitApp(NULL);
	LCUI_InitDisplay(NULL);
	LCUI_InitMouseDriver();
	LCUI_InitKeyboardDriver();
	LCUI_InitKeyboard();
	LCUI_InitIME();

	switch (LCUI_GetAppId()) {
	case LCUI_APP_LINUX_X11:
	case LCUI_APP_UWP:
	case LCUI_APP_WINDOWS:
		LCUICursor_Hide();
		break;
	default:
		break;
	}
}

const char *LCUI_GetVersion(void)
{
	return PACKAGE_VERSION;
}

int LCUI_Destroy(void)
{
	LCUI_SysEventRec e;
	e.type = LCUI_QUIT;
	LCUI_TriggerEvent(&e, NULL);
	System.state = STATE_KILLED;
	LCUI_FreeDisplay();
	LCUI_FreeMouseDriver();
	LCUI_FreeKeyboardDriver();
	LCUI_FreeApp();
	LCUI_FreeIME();
	LCUI_FreeKeyboard();
	LCUI_FreeWidget();
	LCUI_FreeCursor();
	LCUI_FreeFontLibrary();
	lcui_destroy_timers();
	LCUI_FreeEvent();
	return System.exit_code;
}

void LCUI_Quit(void)
{
	System.state = STATE_KILLED;
	LCUIApp_QuitAllMainLoop();
}

void LCUI_Exit(int code)
{
	System.exit_code = code;
	LCUI_Quit();
}

int LCUI_Main(void)
{
	LCUI_MainLoop loop;
	loop = LCUIMainLoop_New();
	LCUIMainLoop_Run(loop);
	return LCUI_Destroy();
}
