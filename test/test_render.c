#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/display.h>
#include <LCUI/timer.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

void UpdateWidgetStyle(LCUI_Widget w, void *arg)
{
	LCUI_Color color;

	color.red = 255;
	color.green = (unsigned char)(rand() % 255);
	color.blue = 0;
	color.alpha = 255;
	Widget_SetStyle(w, key_background_color, color, color);
}

void UpdateFrame(void *arg)
{
	Widget_Each(arg, UpdateWidgetStyle, NULL);
	LCUIWidget_RefreshStyle();
}

void UpdateRenderStatus(void *arg)
{
	char str[32];

	sprintf(str, "[size=24px]FPS: %d[/size]", LCUI_GetFrameCount());
	TextView_SetText(arg, str);
}

int main(void)
{
	size_t i;
	time_t t;
	LCUI_Widget root;
	LCUI_Widget w;
	LCUI_Widget box;
	LCUI_Widget status;
	LCUI_Color white = RGB(255, 255, 255);
	LCUI_Color black = RGB(0, 0, 0);

	srand(time(NULL));
	Logger_SetLevel(LOGGER_LEVEL_WARNING);
	LCUI_Init();
	LCUIDisplay_SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	root = LCUIWidget_GetRoot();
	box = LCUIWidget_New(NULL);
	for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT / 100; ++i) {
		w = LCUIWidget_New(NULL);
		Widget_Resize(w, 10.0f, 10.0f);
		Widget_SetStyle(w, key_display, SV_INLINE_BLOCK, style);
		Widget_Append(box, w);
	}

	status = LCUIWidget_New("textview");
	Widget_SetStyle(status, key_top, 10, px);
	Widget_SetStyle(status, key_right, 10, px);
	Widget_SetStyle(status, key_position, SV_ABSOLUTE, style);
	Widget_SetStyle(status, key_background_color, black, color);
	Widget_SetPadding(status, 10, 15, 10, 15);
	TextView_SetColor(status, white);
	Widget_Append(root, box);
	Widget_Append(root, status);

#ifdef WITH_WINDOW
	LCUI_SetInterval(LCUI_MAX_FRAME_MSEC, UpdateFrame, box);
	LCUI_SetInterval(1000, UpdateRenderStatus, status);
	return LCUI_Main();
#else
	printf("running rendering performance test\n");
	t = clock();
	for (i = 0; i < 120; ++i) {
		UpdateFrame(box);
		LCUIWidget_Update();
		LCUIDisplay_Update();
		LCUIDisplay_Render();
		LCUIDisplay_Present();
	}
	t = clock() - t;
	printf("rendered %zu frames in %.2lfs, rendering speed is %.2lf fps\n",
	       i, t * 1.0 / CLOCKS_PER_SEC, i * CLOCKS_PER_SEC * 1.0 / t);
	return 0;
#endif
}
