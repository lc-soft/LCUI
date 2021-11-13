#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/timer.h>
#include <LCUI/input.h>
#include <LCUI/display.h>
#include "ctest.h"

// static void OnRefreshScreen(void *arg)
// {
// 	LCUIDisplay_InvalidateArea(NULL);
// }

// static void OnQuit(void *arg)
// {
// 	LCUI_Quit();
// }

// static void OnBtnClick(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
// {
// 	LCUI_MainLoop loop;

// 	loop = LCUIMainLoop_New();
// 	lcui_set_timeout(10, OnRefreshScreen, NULL);
// 	lcui_set_timeout(50, OnQuit, NULL);
// 	LCUIMainLoop_Run(loop);
// }

// static void OnTriggerBtnClick(void *arg)
// {
// 	LCUI_SysEventRec e;

// 	e.type = LCUI_MOUSEDOWN;
// 	e.button.button = LCUI_KEY_LEFTBUTTON;
// 	e.button.x = 5;
// 	e.button.y = 5;
// 	LCUI_TriggerEvent(&e, NULL);

// 	e.type = LCUI_MOUSEUP;
// 	LCUI_TriggerEvent(&e, NULL);
// }

// static void ObserverThread(void *arg)
// {
// 	int i;
// 	LCUI_BOOL *exited = arg;

// 	for (i = 0; i < 20 && !*exited; ++i) {
// 		sleep_ms(100);
// 	}
// 	it_b("main loop should exit within 2000ms", *exited, TRUE);
// 	if (!*exited) {
// 		exit(-print_test_result());
// 		return;
// 	}
// 	LCUIThread_Exit(NULL);
// }

static void OnCopyText()
{
	LCUI_SysEventRec copy_ev;
	copy_ev.type = LCUI_KEY_DOWN;
	copy_ev.key.code = (int) LCUI_KEY_C;
	copy_ev.key.ctrl_key = TRUE;

	LCUI_TriggerEvent(&copy_ev, NULL);
}

void test_clipboard(void)
{
	LCUI_Thread tid;
	LCUI_Widget root, text1, text2;
	wchar_t wstr = L"helloworld";

	LCUI_Init();
	//
	text1 = LCUIWidget_New("textedit");
	text2 = LCUIWidget_New("textedit");
	root = LCUIWidget_GetRoot();
	Widget_Append(root, text1);
	Widget_Append(root, text2);

	TextEdit_SetTextW(text1, wstr);
	LCUIWidget_SetFocus(text1);

	lcui_set_timeout(200, OnCopyText, text1);
	
	LCUI_Main();
	// LCUIThread_Join(tid, NULL);
}
