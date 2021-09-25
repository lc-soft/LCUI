/* main.c -- The main functions for the LCUI normal work
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
#include <LCUI.h>
#include <app.h>
#include <LCUI/thread.h>
#include <LCUI/worker.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
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

/* clang-format off */

#define LCUI_WORKER_NUM 4

/** LCUI 应用程序数据 */
static struct lcui_app_t {
	int exit_code;
	step_timer_t timer;
	LCUI_Worker main_worker;		/**< 主工作线程 */
	LCUI_Worker workers[LCUI_WORKER_NUM];	/**< 普通工作线程 */
	int worker_next;			/**< 下一个工作线程编号 */

	LCUI_SettingsRec settings;
} lcui_app;

static void on_settings_change(app_event_t *e, void *arg)
{
	Settings_Init(&lcui_app.settings);
	lcui_app.timer.target_elapsed_time = 1000.f / lcui_app.settings.frame_rate_cap;
}

LCUI_BOOL lcui_post_task(LCUI_Task task)
{
	if (!lcui_app.main_worker) {
		return FALSE;
	}
	LCUIWorker_PostTask(lcui_app.main_worker, task);
	return TRUE;
}

void lcui_post_async_task(LCUI_Task task, int worker_id)
{
	if (worker_id < 0) {
		if (lcui_app.worker_next >= LCUI_WORKER_NUM) {
			lcui_app.worker_next = 0;
		}
		worker_id = lcui_app.worker_next;
		lcui_app.worker_next += 1;
	}
	if (worker_id >= LCUI_WORKER_NUM) {
		worker_id = 0;
	}
	LCUIWorker_PostTask(lcui_app.workers[worker_id], task);
}

int lcui_get_fps(void)
{
	return lcui_app.timer.frames_this_second;
}

void lcui_init_app(void)
{
	int i;
	step_timer_init(&lcui_app.timer);
	LCUI_ResetSettings();
	lcui_app.settings_change_handler_id = LCUI_BindEvent(
	    LCUI_SETTINGS_CHANGE, on_settings_change, NULL, NULL);
	Settings_Init(&lcui_app.settings);
	lcui_app.main_worker = LCUIWorker_New();
	for (i = 0; i < LCUI_WORKER_NUM; ++i) {
		lcui_app.workers[i] = LCUIWorker_New();
		LCUIWorker_RunAsync(lcui_app.workers[i]);
	}
	lcui_app.timer.target_elapsed_time = 1000.f / lcui_app.settings.frame_rate_cap;
}

static void lcui_destroy_app(void)
{
	int i;
	LCUI_MainLoop loop;
	LinkedListNode *node;

	LCUI_UnbindEvent(lcui_app.settings_change_handler_id);
	for (i = 0; i < LCUI_WORKER_NUM; ++i) {
		LCUIWorker_Destroy(lcui_app.workers[i]);
		lcui_app.workers[i] = NULL;
	}
	LCUIWorker_Destroy(lcui_app.main_worker);
	lcui_app.main_worker = NULL;
}

static void lcui_print_info(void)
{
	Logger_Log(LOGGER_LEVEL_INFO,
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

void lcui_init_ui_preset_widgets(void)
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

void lcui_init_base(void)
{
#ifdef LCUI_BUILD_IN_WIN32
	Logger_SetHandler(Win32Logger_LogA);
	Logger_SetHandlerW(Win32Logger_LogW);
#endif
	lcui_app.exit_code = 0;
	lcui_print_info();
	LCUI_InitFontLibrary();
	LCUI_InitTimer();
	LCUI_InitCursor();
	LCUI_InitWidget();
	LCUI_InitMetrics();
	lcui_init_ui_preset_widgets();
}

void lcui_init(void)
{
	lcui_init_base();
	lcui_init_app();
	app_init(L"LCUI Application");
	app_init_events();
	app_init_ime();
	switch (app_get_id()) {
	case APP_ID_LINUX_X11:
	case APP_ID_UWP:
	case APP_ID_WIN32:
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

int lcui_destroy(void)
{
	lcui_destroy_app();
	app_destroy_ime();
	LCUI_FreeKeyboard();
	LCUI_FreeWidget();
	LCUI_FreeCursor();
	LCUI_FreeFontLibrary();
	LCUI_FreeTimer();
	LCUI_FreeEvent();
	return lcui_app.exit_code;
}

void lcui_quit(void)
{
	app_event_t e = { 0 };
	e.type = APP_EVENT_QUIT;
	app_post_event(&e);
}

void lcui_exit(int code)
{
	lcui_app.exit_code = code;
	lcui_quit();
}

static void lcui_app_on_tick(step_timer_t *timer, void *data)
{
	LCUIDisplay_Update();
	LCUIDisplay_Render();
	LCUIDisplay_Present();
}

int lcui_poll_event(app_event_t *e)
{
	LCUI_ProcessTimers();
	app_process_native_events();
	while (LCUIWorker_RunTask(lcui_app.main_worker));
	return app_poll_event(e);
}

int lcui_process_event(app_event_t *e)
{
	app_process_event(&e);
	LCUIWidget_Update();
	step_timer_tick(&lcui_app.timer, lcui_app_on_tick, NULL);
}

int lcui_main(void)
{
	LCUI_BOOL active = TRUE;
	app_event_t e = { 0 };

	while (active) {
		while (lcui_poll_event(&e)) {
			lcui_process_event(&e);
			if (e.type = APP_EVENT_QUIT) {
				active = FALSE;
				break;
			}
			app_event_destroy(&e);
		}
	}
	return lcui_destroy();
}
