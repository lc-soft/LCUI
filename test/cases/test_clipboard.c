#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/gui/widget.h>

#include <LCUI/gui/widget/textedit.h>
#include <LCUI/timer.h>
#include <LCUI/input.h>
#include <LCUI/display.h>
#include "ctest.h"

static void CopyText(void *arg)
{
	LCUI_Widget w = arg;
	LCUIWidget_SetFocus(w);
	LCUI_SysEventRec ev;

	ev.type = LCUI_KEYDOWN;
	ev.key.code = (int)LCUI_KEY_V;
	ev.key.ctrl_key = TRUE;
	ev.key.shift_key = FALSE;
	LCUI_TriggerEvent(&ev, NULL);
}

static void OnText1Focused(void *arg)
{
	LCUI_Widget w = arg;
	LCUI_SysEventRec ev;
	ev.type = LCUI_KEYDOWN;
	ev.key.code = (int)LCUI_KEY_C;
	ev.key.ctrl_key = TRUE;
	ev.key.shift_key = FALSE;

	LCUI_TriggerEvent(&ev, NULL);
}

static void OnCheckText(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	wchar_t wcs[64];
	size_t len = TextEdit_GetTextW(w, 0, 64, wcs);
	it_b("check TextEdit_GetTextW after paste",
	     len == wcslen(L"helloworld"), TRUE);
	LCUI_Quit();
}

void test_clipboard(void)
{
	LCUI_Widget root, text1, text2;

	LCUI_Init();
	//
	text1 = LCUIWidget_New("textedit");
	text2 = LCUIWidget_New("textedit");
	root = LCUIWidget_GetRoot();
	Widget_Append(root, text1);
	Widget_Append(root, text2);

	TextEdit_SetTextW(text1, L"helloworld");
	LCUIWidget_SetFocus(text1);

	lcui_set_timeout(50, OnText1Focused, text1);
	lcui_set_timeout(100, CopyText, text2);
	Widget_BindEvent(text2, "change", OnCheckText, NULL, NULL);

	LCUI_Main();
}
