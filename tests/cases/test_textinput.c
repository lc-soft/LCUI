#include <LCUI.h>
#include <ctest-custom.h>

void test_textinput(void)
{
	ui_widget_t* w;
	wchar_t wcs[64];

	lcui_init();

	w = ui_create_widget("textinput");

	ui_textinput_set_text_w(w, L"hello");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after ui_textinput_set_text_w",
	     ui_textinput_get_text_length(w) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check ui_textinput_get_text_w after ui_textinput_set_text_w",
	     ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check string retrieved from ui_textinput_get_text_w",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_widget_set_text(w, "hello");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after ui_textinput_set_text",
	     ui_textinput_get_text_length(w) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check ui_textinput_get_text_w after ui_textinput_set_text",
	     ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), TRUE);
	ctest_euqal_bool("check string retrieved from ui_textinput_get_text_w",
	     wcscmp(L"hello", wcs) == 0, TRUE);

	ui_textinput_append_text_w(w, L"world");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after ui_textinput_append_text_w",
	     ui_textinput_get_text_length(w) == wcslen(L"helloworld"), TRUE);
	ctest_euqal_bool("check ui_textinput_get_text_w after ui_textinput_append_text_w",
	     ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"helloworld"), TRUE);
	ctest_euqal_bool("check string retrieved from ui_textinput_get_text_w",
	     wcscmp(L"helloworld", wcs) == 0, TRUE);

	ui_textinput_move_caret(w, 0, 5);
	ui_textinput_insert_text_w(w, L", ");
	ui_widget_update(w);
	ui_textinput_move_caret(w, 0, 100);
	ui_textinput_insert_text_w(w, L"!");
	ui_widget_update(w);
	ctest_euqal_bool("check TextEdit_GetLength after inserting text",
	     ui_textinput_get_text_length(w) == wcslen(L"hello, world!"), TRUE);
	ctest_euqal_bool("check ui_textinput_get_text_w after inserting text",
	     ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello, world!"),
	     TRUE);
	ctest_euqal_bool("check string retrieved from ui_textinput_get_text_w",
	     wcscmp(L"hello, world!", wcs) == 0, TRUE);
	ui_widget_destroy(w);
	lcui_quit();
	lcui_main();
}
