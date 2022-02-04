#include <LCUI.h>
#include <LCUI/font.h>
#include <LCUI/ui.h>
#include <LCUI/ui/widgets/textedit.h>
#include "ctest.h"

void test_textedit(void)
{
	ui_widget_t* w;
	LCUI_Object value;
	wchar_t wcs[64];

	lcui_init();

	w = ui_create_widget("textedit");

	ui_textedit_set_text_w(w, L"hello");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after ui_textedit_set_text_w",
	     ui_textedit_get_text_length(w) == wcslen(L"hello"), TRUE);
	it_b("check ui_textedit_get_text_w after ui_textedit_set_text_w",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	it_b("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_widget_set_text(w, "hello");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after ui_textedit_set_text",
	     ui_textedit_get_text_length(w) == wcslen(L"hello"), TRUE);
	it_b("check ui_textedit_get_text_w after ui_textedit_set_text",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	it_b("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_textedit_append_text_w(w, L"world");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after ui_textedit_append_text_w",
	     ui_textedit_get_text_length(w) == wcslen(L"helloworld"), TRUE);
	it_b("check ui_textedit_get_text_w after ui_textedit_append_text_w",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"helloworld"), TRUE);
	it_b("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"helloworld", wcs) == 0, TRUE);

	ui_textedit_move_caret(w, 0, 5);
	ui_textedit_insert_text_w(w, L", ");
	ui_widget_update(w);
	ui_textedit_move_caret(w, 0, 100);
	ui_textedit_insert_text_w(w, L"!");
	ui_widget_update(w);
	it_b("check TextEdit_GetLength after inserting text",
	     ui_textedit_get_text_length(w) == wcslen(L"hello, world!"), TRUE);
	it_b("check ui_textedit_get_text_w after inserting text",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"hello, world!"),
	     TRUE);
	it_b("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"hello, world!", wcs) == 0, TRUE);

	// test property binding
	value = String_New("property name is 'value'");
	ui_widget_bind_property(w, "value", value);
	ui_widget_update(w);
	it_b("check property binding length",
	     wcslen(L"property name is 'value'") ==
		 ui_textedit_get_text_w(w, 0, 64, wcs),
	     TRUE);
	it_b("check property binding value",
	     wcscmp(L"property name is 'value'", wcs) == 0, TRUE);

	// change property value
	String_SetValue(value, "hello");
	ui_widget_update(w);
	it_b("check length after property value change",
	     wcslen(L"hello") == ui_textedit_get_text_w(w, 0, 64, wcs), TRUE);
	it_b("check value after property value change",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	// unbind property
	ui_widget_bind_property(w, "value", NULL);
	ui_widget_update(w);
	it_b("check ui_textedit_get_text_length after unbinding property",
	     0 == ui_textedit_get_text_length(w), TRUE);

	ui_widget_remove(w);
	Object_Delete(value);

	lcui_destroy();
}
