#include <LCUI.h>
#include <ctest-custom.h>

void test_textedit(void)
{
	ui_widget_t* w;
	wchar_t wcs[64];

	lcui_init();

	w = ui_create_widget("textedit");

	ui_textedit_set_text_w(w, L"hello");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after ui_textedit_set_text_w",
	     ui_textedit_get_text_length(w) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check ui_textedit_get_text_w after ui_textedit_set_text_w",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_widget_set_text(w, "hello");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after ui_textedit_set_text",
	     ui_textedit_get_text_length(w) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check ui_textedit_get_text_w after ui_textedit_set_text",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_textedit_append_text_w(w, L"world");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after ui_textedit_append_text_w",
	     ui_textedit_get_text_length(w) == wcslen(L"helloworld"), TRUE);
	ctest_euqal_bool("check ui_textedit_get_text_w after ui_textedit_append_text_w",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"helloworld"), TRUE);
	ctest_euqal_bool("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"helloworld", wcs) == 0, TRUE);

	ui_textedit_move_caret(w, 0, 5);
	ui_textedit_insert_text_w(w, L", ");
	ui_widget_update(w);
	ui_textedit_move_caret(w, 0, 100);
	ui_textedit_insert_text_w(w, L"!");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after inserting text",
	     ui_textedit_get_text_length(w) == wcslen(L"hello, world!"), TRUE);
	ctest_euqal_bool("check ui_textedit_get_text_w after inserting text",
	     ui_textedit_get_text_w(w, 0, 64, wcs) == wcslen(L"hello, world!"),
	     TRUE);
	ctest_euqal_bool("check string retrieved from ui_textedit_get_text_w",
	     wcscmp(L"hello, world!", wcs) == 0, TRUE);
	ui_widget_destroy(w);
	lcui_quit();
	lcui_main();
}
