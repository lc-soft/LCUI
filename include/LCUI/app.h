#ifndef LCUI_H
#define LCUI_H

#include <LCUI/def.h>
#include <LCUI/platform.h>
#include <LCUI/worker.h>

#define LCUI_MAX_FRAMES_PER_SEC 120
#define LCUI_MAX_FRAME_MSEC ((int)(1000.0 / LCUI_MAX_FRAMES_PER_SEC + 0.5))

LCUI_BEGIN_HEADER

// Settings

typedef struct lcui_settings_t {
	int frame_rate_cap;
	int parallel_rendering_threads;
	LCUI_BOOL record_profile;
	LCUI_BOOL fps_meter;
	LCUI_BOOL paint_flashing;
} lcui_settings_t;

/* Initialize settings with the current global settings. */
LCUI_API void lcui_get_settings(lcui_settings_t *settings);

/* Update global settings with the given input. */
LCUI_API void lcui_apply_settings(lcui_settings_t *settings);

/* Reset global settings to their defaults. */
LCUI_API void lcui_reset_settings(void);

// Timers

LCUI_API int lcui_destroy_timer(int timer_id);
LCUI_API int lcui_pause_timer(int timer_id);
LCUI_API int lcui_continue_timer(int timer_id);
LCUI_API int lcui_reset_timer(int timer_id, long int n_ms);
LCUI_API int lcui_set_timeout(long int n_ms, void (*callback)(void *),
			      void *arg);
LCUI_API int lcui_set_interval(long int n_ms, void (*callback)(void *),
			       void *arg);

// Tasks

typedef void (*LCUI_AppTaskFunc)(void *, void *);

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

/** lcui_post_task 的简化版本 */
#define lcui_post_simple_task(FUNC, ARG1, ARG2)           \
	do {                                              \
		LCUI_TaskRec _ui_task = { 0 };            \
		_ui_task.arg[0] = (void *)ARG1;           \
		_ui_task.arg[1] = (void *)ARG2;           \
		_ui_task.func = (LCUI_AppTaskFunc)(FUNC); \
		lcui_post_task(&_ui_task);                \
	} while (0);

// UI

typedef enum lcui_display_mode_t {
	LCUI_DISPLAY_MODE_DEFAULT,
	LCUI_DISPLAY_MODE_WINDOWED,
	LCUI_DISPLAY_MODE_FULLSCREEN,
	LCUI_DISPLAY_MODE_SEAMLESS,
} lcui_display_mode_t;

LCUI_API void lcui_init_ui(void);
LCUI_API void lcui_destroy_ui(void);
LCUI_API void lcui_update_ui(void);
LCUI_API size_t lcui_render_ui(void);
LCUI_API void lcui_preset_ui(void);
LCUI_API void lcui_dispatch_ui_event(app_event_t *app_event);
LCUI_API void lcui_set_ui_display_mode(lcui_display_mode_t mode);

// Event

LCUI_API int lcui_get_event(app_event_t *e);
LCUI_API int lcui_process_event(app_event_t *e);
LCUI_API int lcui_process_events(app_process_events_option_t option);

INLINE int lcui_process_all_events(void)
{
	return lcui_process_events(APP_PROCESS_EVENTS_ALL_IF_PRESENT);
}

// Base

LCUI_API uint32_t lcui_get_fps(void);

LCUI_API void lcui_set_frame_rate_cap(unsigned rate_cap);

LCUI_API void lcui_init_base(void);

/** 初始化 LCUI 各项功能 */
LCUI_API void lcui_init(void);

LCUI_API int lcui_main(void);

LCUI_API int lcui_run(void);

/** 获取LCUI的版本 */
LCUI_API const char *lcui_get_version(void);

/** 释放LCUI占用的资源 */
LCUI_API void lcui_destroy(void);

/** 退出LCUI，释放LCUI占用的资源 */
LCUI_API void lcui_quit(void);

/** 退出 LCUI，并设置退出码 */
LCUI_API void lcui_exit(int code);

LCUI_END_HEADER

#endif
