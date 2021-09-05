#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/timer.h>
#include <LCUI/input.h>
#include <LCUI/display.h>
#include "test.h"

static void OnRefreshScreen(void *arg)
{
	LCUIDisplay_InvalidateArea(NULL);
}

static void OnQuit(void *arg)
{
	LCUI_Quit();
}

static void OnBtnClick(ui_widget_t* w, ui_event_t* e, void *arg)
{
	LCUI_MainLoop loop;

	loop = LCUIMainLoop_New();
	LCUI_SetTimeout(10, OnRefreshScreen, NULL);
	LCUI_SetTimeout(50, OnQuit, NULL);
	LCUIMainLoop_Run(loop);
}

static void OnTriggerBtnClick(void *arg)
{
	LCUI_SysEventRec e;

	e.type = LCUI_MOUSEDOWN;
	e.button.button = LCUI_KEY_LEFTBUTTON;
	e.button.x = 5;
	e.button.y = 5;
	LCUI_TriggerEvent(&e, NULL);

	e.type = LCUI_MOUSEUP;
	LCUI_TriggerEvent(&e, NULL);
}

static void ObserverThread(void *arg)
{
	int i;
	LCUI_BOOL *exited = arg;

	for (i = 0; i < 10 && !*exited; ++i) {
		LCUI_MSleep(100);
	}
	it_b("main loop should exit within 1000ms", *exited, TRUE);
	if (!*exited) {
		exit(-print_test_result());
		return;
	}
	LCUIThread_Exit(NULL);
}

void test_mainloop(void)
{
	LCUI_Thread tid;
	ui_widget_t* root, btn;
	LCUI_BOOL exited = FALSE;

	LCUI_Init();
	btn = ui_create_widget("button");
	root = ui_root();
	Button_SetText(btn, "button");
	ui_widget_on(btn, "click", OnBtnClick, NULL, NULL);
	ui_widget_append(root, btn);
	/* Observe whether the main loop has exited in a new thread */
	LCUIThread_Create(&tid, ObserverThread, &exited);
	/* Trigger the click event after the first frame is updated */
	LCUI_SetTimeout(50, OnTriggerBtnClick, btn);
	LCUI_Main();
	exited = TRUE;
	LCUIThread_Join(tid, NULL);
}
