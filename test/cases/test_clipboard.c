#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/gui/widget/textedit.h>
#include <LCUI/timer.h>
#include "ctest.h"

static void copy_text(void *arg)
{
	ui_widget_t *w = arg;
	app_event_t ev = { 0 };

	ev.type = APP_EVENT_KEYDOWN;
	ev.key.code = KEY_V;
	ev.key.ctrl_key = TRUE;
	ui_set_focus(w);
	app_process_event(&ev);
}

static void on_text1_focused(void *arg)
{
	ui_widget_t *w = arg;
	app_event_t ev = { 0 };

	ev.type = APP_EVENT_KEYDOWN;
	ev.key.code = KEY_C;
	ev.key.ctrl_key = TRUE;
	app_process_event(&ev);
}

static void on_check_text(ui_widget_t *w, ui_event_t *e, void *arg)
{
	clipboard_t *clipboard = arg;

	it_b("check the pasted text",
	     clipboard != NULL && wcscmp(clipboard->text, L"helloworld") == 0,
	     TRUE);
	lcui_quit();
}

void test_clipboard(void)
{
	ui_widget_t *text1, *text2;

	lcui_init();

	text1 = ui_create_widget("textedit");
	text2 = ui_create_widget("textedit");
	ui_root_append(text1);
	ui_root_append(text2);

	TextEdit_SetTextW(text1, L"helloworld");
	ui_set_focus(text1);

	lcui_set_timeout(50, on_text1_focused, text1);
	lcui_set_timeout(100, copy_text, text2);
	ui_widget_on(text2, "paste", on_check_text, NULL, NULL);

	lcui_main();
}
