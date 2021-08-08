#include "config.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <LCUI/app.h>
#include <LCUI/platform.h>
#include <LCUI/ui.h>
#include <LCUI/thread.h>
#include <LCUI/worker.h>
#include <LCUI/timer.h>
#include <LCUI/font.h>

#define LCUI_WORKER_NUM 4

/** LCUI 应用程序数据 */
static struct lcui_app_t {
	step_timer_t timer;
	LCUI_Worker main_worker;
	LCUI_Worker workers[LCUI_WORKER_NUM];
	int worker_next;
} lcui_app;

#ifdef LCUI_PLATFORM_WIN_DESKTOP

static void win32_logger_log_a(const char *str)
{
	OutputDebugStringA(str);
}

static void win32_logger_log_w(const wchar_t *wcs)
{
	OutputDebugStringW(wcs);
}

#endif

const char *lcui_get_version(void)
{
	return PACKAGE_VERSION;
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

uint32_t lcui_get_fps(void)
{
	return lcui_app.timer.frames_per_second;
}

void lcui_init_app(void)
{
	int i;
	step_timer_init(&lcui_app.timer);
	lcui_app.main_worker = LCUIWorker_New();
	for (i = 0; i < LCUI_WORKER_NUM; ++i) {
		lcui_app.workers[i] = LCUIWorker_New();
		LCUIWorker_RunAsync(lcui_app.workers[i]);
	}
	lcui_app.timer.target_elapsed_time = 0;
}

void lcui_set_frame_rate_cap(unsigned rate_cap)
{
	if (rate_cap > 0) {
		lcui_app.timer.target_elapsed_time = 1000 / rate_cap;
		lcui_app.timer.is_fixed_time_step = TRUE;
	} else {
		lcui_app.timer.is_fixed_time_step = FALSE;
	}
}

static void lcui_destroy_app(void)
{
	int i;

	for (i = 0; i < LCUI_WORKER_NUM; ++i) {
		LCUIWorker_Destroy(lcui_app.workers[i]);
		lcui_app.workers[i] = NULL;
	}
	LCUIWorker_Destroy(lcui_app.main_worker);
	lcui_app.main_worker = NULL;
}

static void lcui_print_info(void)
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

static void lcui_app_on_tick(step_timer_t *timer, void *data)
{
	lcui_render_ui();
	app_present();
}

static int lcui_dispatch_app_event(app_event_t *e)
{
	if (e->type == APP_EVENT_QUIT) {
		return 0;
	}
	LCUIWorker_RunTask(lcui_app.main_worker);
	lcui_process_timers();
	lcui_dispatch_ui_event(e);
	lcui_update_ui();
	step_timer_tick(&lcui_app.timer, lcui_app_on_tick, NULL);
	return 0;
}

int lcui_process_events(app_process_events_option_t option)
{
	return app_process_native_events(option);
}

void lcui_init_base(void)
{
#ifdef LCUI_PLATFORM_WIN_DESKTOP
	logger_set_handler(win32_logger_log_a);
	logger_set_handler_w(win32_logger_log_w);
#endif
	lcui_init_timers();
	lcui_reset_settings();
}

void lcui_init(void)
{
	lcui_print_info();
	app_init(L"LCUI Application");
	app_set_event_dispatcher(lcui_dispatch_app_event);
	lcui_init_base();
	lcui_init_app();
	lcui_init_ui();
}

void lcui_destroy(void)
{
	lcui_destroy_app();
	lcui_destroy_ui();
	lcui_destroy_timers();
	app_destroy();
}

void lcui_exit(int code)
{
	app_exit(code);
}

void lcui_quit(void)
{
	lcui_exit(0);
}

int lcui_run(void)
{
	return lcui_process_events(APP_PROCESS_EVENTS_UNTIL_QUIT);
}

int lcui_main(void)
{
	int exit_code = lcui_run();
	lcui_destroy();
	return exit_code;
}
