#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/display.h>
#include <LCUI/timer.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define BLOCK_COUNT 60

static struct TestStatus {
	size_t color_index;
	unsigned int fps;
	LCUI_Widget box;
} self;

void UpdateWidgetStyle(LCUI_Widget w, void *arg)
{
	size_t index;
	LCUI_Color color;

	index = w->index % BLOCK_COUNT + w->index / BLOCK_COUNT;
	index = ((index + self.color_index) * 256 / BLOCK_COUNT) % 512;
	color.red = 255;
	color.green = (unsigned char)(index > 255 ? 511 - index : index);
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

	sprintf(str, "[size=24px]FPS: %d[/size]", self.fps);
	TextView_SetText(arg, str);
	self.fps = 0;
}

void InitModal(void)
{
	LCUI_Widget dimmer;
	LCUI_Widget dialog;

	dimmer = LCUIWidget_New(NULL);
	dialog = LCUIWidget_New(NULL);
	Widget_Resize(dialog, 400, 400);
	Widget_SetStyleString(dialog, "margin", "100px auto");
	Widget_SetStyleString(dialog, "border-radius", "6px");
	Widget_SetStyleString(dialog, "opacity", "0.9");
	Widget_SetBoxShadow(dialog, 0, 4, 8, ARGB(100, 0, 0, 0));
	Widget_SetStyleString(dialog, "background-color", "#fff");

	Widget_SetStyle(dimmer, key_top, 0, px);
	Widget_SetStyle(dimmer, key_left, 0, px);
	Widget_SetStyle(dimmer, key_width, 1.0f, scale);
	Widget_SetStyle(dimmer, key_height, 1.0f, scale);
	Widget_SetStyle(dimmer, key_position, SV_ABSOLUTE, style);
	Widget_SetStyleString(dimmer, "background-color", "rgba(0,0,0,0.5)");

	Widget_Append(dimmer, dialog);
	Widget_Append(LCUIWidget_GetRoot(), dimmer);
}

void InitBackground(void)
{
	size_t i;
	size_t c;
	size_t n = BLOCK_COUNT;
	LCUI_Widget w;
	LCUI_Widget root;
	LCUI_Color color;
	LCUI_WidgetRulesRec rules = { 0 };
	const float width = SCREEN_WIDTH * 1.0f / n;
	const float height = SCREEN_HEIGHT * 1.0f / n;

	color.red = 255;
	color.green = 0;
	color.blue = 0;
	color.alpha = 255;
	root = LCUIWidget_GetRoot();
	self.box = LCUIWidget_New(NULL);
	for (i = 0; i < n * n; ++i) {
		if (i % n == 0) {
			++self.color_index;
		}
		w = LCUIWidget_New(NULL);
		c = ((i % n + i / n) * 256 / n) % 512;
		color.green = (unsigned char)(c > 255 ? 511 - c : c);
		Widget_Resize(w, width, height);
		Widget_SetStyle(w, key_display, SV_INLINE_BLOCK, style);
		Widget_SetStyle(w, key_background_color, color, color);
		Widget_Append(self.box, w);
	}
	rules.cache_children_style = TRUE;
	rules.ignore_classes_change = TRUE;
	rules.ignore_status_change = TRUE;
	rules.max_update_children_count = -1;
	rules.max_render_children_count = 0;
	Widget_GenerateHash(self.box);
	Widget_SetRules(self.box, &rules);
	Widget_Append(root, self.box);
}

void InitRenderStatus(void)
{
	LCUI_Widget root;
	LCUI_Widget status;
	LCUI_Color white = RGB(255, 255, 255);
	LCUI_Color black = RGB(0, 0, 0);

	root = LCUIWidget_GetRoot();
	status = LCUIWidget_New("textview");
	Widget_SetStyle(status, key_top, 10, px);
	Widget_SetStyle(status, key_right, 10, px);
	Widget_SetStyle(status, key_position, SV_ABSOLUTE, style);
	Widget_SetStyle(status, key_background_color, black, color);
	Widget_SetPadding(status, 10, 15, 10, 15);
	TextView_SetColor(status, white);
	Widget_Append(root, status);
	UpdateRenderStatus(status);
	LCUI_SetInterval(1000, UpdateRenderStatus, status);
}

int main(void)
{
	size_t i;
	int64_t t;

	Logger_SetLevel(LOGGER_LEVEL_WARNING);
	LCUI_Init();
	LCUIDisplay_SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	InitBackground();
	InitModal();
	InitRenderStatus();
	printf("running rendering performance test\n");
	LCUI_RunFrame();
	t = LCUI_GetTime();
	self.color_index = 0;
	for (i = 0; i < 120; ++i) {
		UpdateFrame(self.box);
		LCUI_ProcessTimers();
		LCUI_ProcessEvents();
		LCUIWidget_Update();
		LCUIDisplay_Update();
		LCUIDisplay_Render();
		LCUIDisplay_Present();
		++self.fps;
		++self.color_index;
	}
	t = LCUI_GetTimeDelta(t);
	Logger_Warning(
	    "rendered %zu frames in %.2lfs, rendering speed is %.2lf fps\n", i,
	    t / 1000.f, i * 1000.f / t);
	return 0;
}
