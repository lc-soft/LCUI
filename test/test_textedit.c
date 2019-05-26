#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textedit.h>
#include "test.h"

int test_textedit(void)
{
	int ret = 0;
	LCUI_Widget w;
	wchar_t wcs[64];

	LCUI_InitFontLibrary();
	LCUI_InitWidget();

	w = LCUIWidget_New("textedit");

	TextEdit_SetTextW(w, L"hello");
	Widget_Update(w);
	CHECK(wcslen(L"hello") == TextEdit_GetTextLength(w));
	CHECK(wcslen(L"hello") == TextEdit_GetTextW(w, 0, 64, wcs));
	CHECK(wcscmp(L"hello", wcs) == 0);

	Widget_SetText(w, "hello");
	Widget_Update(w);
	CHECK(wcslen(L"hello") == TextEdit_GetTextLength(w));
	CHECK(wcslen(L"hello") == TextEdit_GetTextW(w, 0, 64, wcs));
	CHECK(wcscmp(L"hello", wcs) == 0);

	TextEdit_AppendTextW(w, L"world");
	Widget_Update(w);
	CHECK(wcslen(L"helloworld") == TextEdit_GetTextLength(w));
	CHECK(wcslen(L"helloworld") == TextEdit_GetTextW(w, 0, 64, wcs));
	CHECK(wcscmp(L"helloworld", wcs) == 0);

	TextEdit_MoveCaret(w, 0, 5);
	TextEdit_InsertTextW(w, L", ");
	Widget_Update(w);
	TextEdit_MoveCaret(w, 0, 100);
	TextEdit_InsertTextW(w, L"!");
	Widget_Update(w);
	CHECK(wcslen(L"hello, world!") == TextEdit_GetTextLength(w));
	CHECK(wcslen(L"hello, world!") == TextEdit_GetTextW(w, 0, 64, wcs));
	CHECK(wcscmp(L"hello, world!", wcs) == 0);

	Widget_Destroy(w);
	LCUI_FreeWidget();
	LCUI_FreeFontLibrary();
	return ret;
}
