#include <stdlib.h>
#include <LCUI.h>
#include <ctest-custom.h>

static void OnRefreshScreen(void *arg)
{
	ui_refresh_style();
}

static void OnQuit(void *arg)
{
	lcui_quit();
}

static void OnBtnClick(ui_widget_t* w, ui_event_t* ui_event, void *arg)
{
	lcui_set_timeout(10, OnRefreshScreen, NULL);
	lcui_set_timeout(50, OnQuit, NULL);
	lcui_process_events(APP_PROCESS_EVENTS_UNTIL_QUIT);
	lcui_quit();
}

static void OnTriggerBtnClick(void *arg)
{
	app_event_t e;

	e.type = APP_EVENT_MOUSEDOWN;
	e.mouse.button = MOUSE_BUTTON_LEFT;
	e.mouse.x = 5;
	e.mouse.y = 5;
	app_post_event(&e);
	e.type = APP_EVENT_MOUSEUP;
	app_post_event(&e);
}

static void ObserverThread(void *arg)
{
	int i;
	LCUI_BOOL *exited = arg;

	for (i = 0; i < 20 && !*exited; ++i) {
		sleep_ms(100);
	}
	ctest_euqal_bool("main loop should exit within 2000ms", *exited, TRUE);
	if (!*exited) {
		exit(-ctest_finish());
		return;
	}
	thread_exit(NULL);
}

void test_mainloop(void)
{
	thread_t tid;
	ui_widget_t *btn;
	LCUI_BOOL exited = FALSE;

	lcui_init();
	btn = ui_create_widget("button");
	ui_button_set_text(btn, "button");
	ui_widget_on(btn, "click", OnBtnClick, NULL, NULL);
	ui_root_append(btn);
	/* Observe whether the main loop has exited in a new thread */
	thread_create(&tid, ObserverThread, &exited);
	/* Trigger the click event after the first frame is updated */
	lcui_set_timeout(50, OnTriggerBtnClick, btn);
	lcui_main();
	exited = TRUE;
	thread_join(tid, NULL);
}
