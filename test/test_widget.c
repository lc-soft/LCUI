#include <stdio.h>
#include <time.h>
#include <LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/util/logger.h>

int main(void)
{
	clock_t c;
	size_t i, n = 100000;
	double sec;

	ui_widget_t* box, w;

	LCUI_InitFontLibrary();
	LCUI_InitWidget();
	box = ui_create_widget(NULL);

	c = clock();
	Logger_Debug("start create %zu widgets...\n", n);
	ui_widget_append(ui_root(), box);
	for (i = 0; i < n; ++i) {
		w = ui_create_widget("textview");
		TextView_SetTextW(w, L"hello, test");
		ui_widget_append(box, w);
	}
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been created, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 1s\n");

	Logger_Debug("start update %zu widgets...\n", n);
	c = clock();
	ui_update();
	ui_update();
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been updated, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 6s\n");

	Logger_Debug("start remove %zu widgets...\n", n);
	c = clock();
	ui_widget_empty(box);
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been removed, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 0.5s\n");

	Logger_Debug("start destroy %zu widgets...\n", n);
	c = clock();
	ui_update();
	sec = (clock() - c) * 1.0 / CLOCKS_PER_SEC;
	Logger_Debug("%zu widgets have been destroyed, which took %gs\n", n, sec);
	Logger_Debug("it should take less than 1s\n");

	LCUI_FreeWidget();
	LCUI_FreeFontLibrary();
	return 0;
}
