#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <app.h>
#ifdef APP_PLATFORM_WIN_DESKTUP
#include <ime.h>

#pragma comment(lib, "Imm32.lib")

static LCUI_BOOL win32ime_process_key(int key, LCUI_BOOL is_pressed)
{
	return FALSE;
}

static void win32ime_to_text(int ch)
{
	wchar_t text[2] = { ch, 0 };
	ime_commit(text, 2);
}

static void win32ime_on_char(MSG *msg, void *arg)
{
	wchar_t text[2];

	text[0] = msg->wParam;
	text[1] = 0;
	ime_commit(text, 2);
}

static void win32ime_set_caret(int x, int y)
{
	HWND hwnd = GetActiveWindow();
	HIMC himc = ImmGetContext(hwnd);
	if (himc) {
		COMPOSITIONFORM composition;
		composition.dwStyle = CFS_POINT;
		composition.ptCurrentPos.x = x;
		composition.ptCurrentPos.y = y;
		ImmSetCompositionWindow(himc, &composition);
		ImmReleaseContext(hwnd, himc);
	}
}

static LCUI_BOOL win32ime_open(void)
{
	app_on_native_event(WM_CHAR, win32ime_on_char, NULL);
	return TRUE;
}

static LCUI_BOOL win32ime_close(void)
{
	app_off_native_event(WM_CHAR, win32ime_on_char);
	return TRUE;
}

int ime_add_win32(void)
{
	ime_handler_t handler;

	handler.prockey = win32ime_process_key;
	handler.totext = win32ime_to_text;
	handler.close = win32ime_close;
	handler.open = win32ime_open;
	handler.setcaret = win32ime_set_caret;
	return app_ime_add("Win32 Input Method", &handler);
}

#endif
