#include <LCUI.h>
#include <LCUI/font.h>
#include <LCUI/ui.h>
#include <LCUI/gui/widget/textedit.h>
#include "ctest.h"

void test_textedit(void)
{
	ui_widget_t* w;
	LCUI_Object value;
	wchar_t wcs[64];

	lcui_init();

	w = ui_create_widget("textedit");

	TextEdit_SetTextW(w, L"hello");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after TextEdit_SetTextW",
	     TextEdit_GetTextLength(w) == wcslen(L"hello"), TRUE);
	it_b("check TextEdit_GetTextW after TextEdit_SetTextW",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_widget_set_text(w, "hello");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after TextEdit_SetText",
	     TextEdit_GetTextLength(w) == wcslen(L"hello"), TRUE);
	it_b("check TextEdit_GetTextW after TextEdit_SetText",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	TextEdit_AppendTextW(w, L"world");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after TextEdit_AppendTextW",
	     TextEdit_GetTextLength(w) == wcslen(L"helloworld"), TRUE);
	it_b("check TextEdit_GetTextW after TextEdit_AppendTextW",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"helloworld"), TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"helloworld", wcs) == 0, TRUE);

	TextEdit_MoveCaret(w, 0, 5);
	TextEdit_InsertTextW(w, L", ");
	ui_widget_update(w);
	TextEdit_MoveCaret(w, 0, 100);
	TextEdit_InsertTextW(w, L"!");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after inserting text",
	     TextEdit_GetTextLength(w) == wcslen(L"hello, world!"), TRUE);
	it_b("check TextEdit_GetTextW after inserting text",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"hello, world!"),
	     TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"hello, world!", wcs) == 0, TRUE);

	// test property binding
	value = String_New("property name is 'value'");
	ui_widget_bind_property(w, "value", value);
	ui_widget_update(w);
	it_b("check property binding length",
	     wcslen(L"property name is 'value'") ==
		 TextEdit_GetTextW(w, 0, 64, wcs),
	     TRUE);
	it_b("check property binding value",
	     wcscmp(L"property name is 'value'", wcs) == 0, TRUE);

	// change property value
	String_SetValue(value, "hello");
	ui_widget_update(w);
	it_b("check length after property value change",
	     wcslen(L"hello") == TextEdit_GetTextW(w, 0, 64, wcs), TRUE);
	it_b("check value after property value change",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	// unbind property
	ui_widget_bind_property(w, "value", NULL);
	ui_widget_update(w);
	it_b("check TextEdit_GetTextLength after unbinding property",
	     0 == TextEdit_GetTextLength(w), TRUE);

	ui_widget_remove(w);
	Object_Delete(value);

	lcui_destroy();
}
