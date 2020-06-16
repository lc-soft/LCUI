#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textedit.h>
#include "test.h"
#include "libtest.h"

void test_textedit(void)
{
	LCUI_Widget w;
	LCUI_Object value;
	wchar_t wcs[64];

	LCUI_InitFontLibrary();
	LCUI_InitWidget();

	w = LCUIWidget_New("textedit");

	TextEdit_SetTextW(w, L"hello");
	Widget_Update(w);
	it_b("check TextEdit_GetLength after TextEdit_SetTextW",
	     TextEdit_GetTextLength(w) == wcslen(L"hello"), TRUE);
	it_b("check TextEdit_GetTextW after TextEdit_SetTextW",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	Widget_SetText(w, "hello");
	Widget_Update(w);
	it_b("check TextEdit_GetLength after TextEdit_SetText",
	     TextEdit_GetTextLength(w) == wcslen(L"hello"), TRUE);
	it_b("check TextEdit_GetTextW after TextEdit_SetText",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	TextEdit_AppendTextW(w, L"world");
	Widget_Update(w);
	it_b("check TextEdit_GetLength after TextEdit_AppendTextW",
	     TextEdit_GetTextLength(w) == wcslen(L"helloworld"), TRUE);
	it_b("check TextEdit_GetTextW after TextEdit_AppendTextW",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"helloworld"), TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"helloworld", wcs) == 0, TRUE);

	TextEdit_MoveCaret(w, 0, 5);
	TextEdit_InsertTextW(w, L", ");
	Widget_Update(w);
	TextEdit_MoveCaret(w, 0, 100);
	TextEdit_InsertTextW(w, L"!");
	Widget_Update(w);
	it_b("check TextEdit_GetLength after inserting text",
	     TextEdit_GetTextLength(w) == wcslen(L"hello, world!"), TRUE);
	it_b("check TextEdit_GetTextW after inserting text",
	     TextEdit_GetTextW(w, 0, 64, wcs) == wcslen(L"hello, world!"),
	     TRUE);
	it_b("check string retrieved from TextEdit_GetTextW",
	     wcscmp(L"hello, world!", wcs) == 0, TRUE);

	// test property binding
	value = String_New("property name is 'value'");
	Widget_BindProperty(w, "value", value);
	Widget_Update(w);
	it_b("check property binding length",
	     wcslen(L"property name is 'value'") ==
		 TextEdit_GetTextW(w, 0, 64, wcs),
	     TRUE);
	it_b("check property binding value",
	     wcscmp(L"property name is 'value'", wcs) == 0, TRUE);

	// change property value
	String_SetValue(value, "hello");
	Widget_Update(w);
	it_b("check length after property value change",
	     wcslen(L"hello") == TextEdit_GetTextW(w, 0, 64, wcs), TRUE);
	it_b("check value after property value change",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	// unbind property
	Widget_BindProperty(w, "value", NULL);
	Widget_Update(w);
	it_b("check TextEdit_GetTextLength after unbinding property",
	     0 == TextEdit_GetTextLength(w), TRUE);

	Widget_Destroy(w);
	Object_Delete(value);

	LCUI_FreeWidget();
	LCUI_FreeFontLibrary();
}
