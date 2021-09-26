#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/timer.h>
#include <LCUI/input.h>
#include <LCUI/display.h>
#include "ctest.h"

static void OnRefreshScreen(void *arg)
{
	LCUIDisplay_InvalidateArea(NULL);
}

static void OnQuit(void *arg)
{
	LCUI_Quit();
}

static void OnBtnClick(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	LCUI_MainLoop loop;

	loop = LCUIMainLoop_New();
	lcui_set_timeout(10, OnRefreshScreen, NULL);
	lcui_set_timeout(50, OnQuit, NULL);
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

	for (i = 0; i < 20 && !*exited; ++i) {
		sleep_ms(100);
	}
	it_b("main loop should exit within 2000ms", *exited, TRUE);
	if (!*exited) {
		exit(-print_test_result());
		return;
	}
	LCUIThread_Exit(NULL);
}

void test_mainloop(void)
{
	LCUI_Thread tid;
	LCUI_Widget root, btn;
	LCUI_BOOL exited = FALSE;

	LCUI_Init();
	btn = LCUIWidget_New("button");
	root = LCUIWidget_GetRoot();
	Button_SetText(btn, "button");
	Widget_BindEvent(btn, "click", OnBtnClick, NULL, NULL);
	Widget_Append(root, btn);
	/* Observe whether the main loop has exited in a new thread */
	LCUIThread_Create(&tid, ObserverThread, &exited);
	/* Trigger the click event after the first frame is updated */
	lcui_set_timeout(50, OnTriggerBtnClick, btn);
	LCUI_Main();
	exited = TRUE;
	LCUIThread_Join(tid, NULL);
}
