#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/util/logger.h>
#include <stdio.h>
#include <time.h>
#include "test.h"

int main(void)
{
	clock_t c;
	size_t i, n = 100000;
	double sec;

	LCUI_Widget box, w;

	LCUI_InitFontLibrary();
	LCUI_InitWidget();
	box = LCUIWidget_New(NULL);

	c = clock();
	Logger_Debug("start create %zu widgets...\n", n);
	Widget_Append(LCUIWidget_GetRoot(), box);
	for (i = 0; i < n; ++i) {
		w = LCUIWidget_New("textview");
		TextView_SetTextW(w, L"hello, test");
		Widget_Append(box, w);
	}
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been created, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 1s\n");

	Logger_Debug("start update %zu widgets...\n", n);
	c = clock();
	LCUIWidget_Update();
	LCUIWidget_Update();
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been updated, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 6s\n");

	Logger_Debug("start remove %zu widgets...\n", n);
	c = clock();
	Widget_Empty(box);
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been removed, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 0.5s\n");

	Logger_Debug("start destroy %zu widgets...\n", n);
	c = clock();
	LCUIWidget_Update();
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been destroyed, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 1s\n");

	LCUI_FreeWidget();
	LCUI_FreeFontLibrary();
	return 0;
}
