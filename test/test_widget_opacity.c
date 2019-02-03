#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/image.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/builder.h>
#include "test.h"

#define PARENT_OPACITY	0.8f
#define CHILD_OPACITY	0.5f

static struct {
	LCUI_Widget parent;
	LCUI_Widget child;
	LCUI_Widget text;
} self;

#ifdef LCUI_BUILD_IN_WIN32
static void LoggerHandler(const char *str)
{
	OutputDebugStringA(str);
}

static void LoggerHandlerW(const wchar_t *str)
{
	OutputDebugStringW(str);
}
#endif

static void build(void)
{
	LCUI_Widget pack, root;

	pack = LCUIBuilder_LoadFile("test_widget_opacity.xml");
	root = LCUIWidget_GetRoot();
	Widget_Append(root, pack);
	Widget_Unwrap(pack);

	self.parent = LCUIWidget_GetById("parent");
	self.child = LCUIWidget_GetById("child");
	self.text = LCUIWidget_GetById("current-opacity");
}

static int check_color(LCUI_Color a, LCUI_Color b)
{
	return abs(a.r - b.r) < 2 && abs(a.g - b.g) < 2 && abs(a.b - b.b) < 2;
}

static int check_widget_opactiy(void)
{
	int ret = 0;
	LCUI_Graph canvas;
	LCUI_Color color;
	LCUI_Color tmp;
	LCUI_Color expected_color;
	LCUI_Color child_bgcolor = RGB(0, 255, 0);
	LCUI_Color child_footer_bgcolor = RGB(255, 255, 255);
	LCUI_Color parent_bgcolor = RGB(255, 0, 0);
	LCUI_Color parent_bcolor = RGB(0, 0, 0);
	LCUI_Color bgcolor = RGB(255, 255, 255);
	LCUI_Rect rect = { 0, 0, 400, 256 };
	LCUI_PaintContextRec paint;

	Graph_Init(&canvas);
	Graph_Create(&canvas, rect.width, rect.height);
	Graph_FillRect(&canvas, bgcolor, NULL, FALSE);

	paint.with_alpha = FALSE;
	paint.rect.width = 400;
	paint.rect.height = 256;
	paint.rect.x = paint.rect.y = 0;
	Graph_Quote(&paint.canvas, &canvas, &rect);

	Widget_SetOpacity(self.parent, 0.8f);
	Widget_Resize(self.parent, 512, 256);
	Widget_UpdateStyle(self.child, TRUE);
	Widget_UpdateStyle(self.parent, TRUE);
	Widget_Update(self.child);
	Widget_Update(self.parent);
	Widget_Update(self.parent);

	Widget_Render(self.parent, &paint);

	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 10, 10, color);
	PIXEL_BLEND(&expected_color, &parent_bcolor, (int)(PARENT_OPACITY * 255));
	CHECK_WITH_TEXT("check parent border color",
			check_color(expected_color, color));

	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 30, 30, color);
	PIXEL_BLEND(&expected_color, &parent_bgcolor, (int)(PARENT_OPACITY * 255));
	CHECK_WITH_TEXT("check parent background color",
			check_color(expected_color, color));

	tmp = parent_bgcolor;
	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 60, 90, color);
	PIXEL_BLEND(&tmp, &child_bgcolor, (int)(CHILD_OPACITY * 255));
	PIXEL_BLEND(&expected_color, &tmp, (int)(PARENT_OPACITY * 255));
	CHECK_WITH_TEXT("check child 1 background color",
			check_color(expected_color, color));

	tmp = parent_bgcolor;
	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 60, 120, color);
	PIXEL_BLEND(&tmp, &child_footer_bgcolor, (int)(CHILD_OPACITY * 255));
	PIXEL_BLEND(&expected_color, &tmp, (int)(PARENT_OPACITY * 255));
	CHECK_WITH_TEXT("check child 1 footer background color",
			check_color(expected_color, color));

	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 220, 90, color);
	PIXEL_BLEND(&expected_color, &child_bgcolor,
		    (int)(PARENT_OPACITY * 255));
	CHECK_WITH_TEXT("check child 2 background color",
			check_color(expected_color, color));

	expected_color = child_footer_bgcolor;
	Graph_GetPixel(&canvas, 220, 120, color);
	CHECK_WITH_TEXT("check child 2 footer background color",
			check_color(expected_color, color));

	LCUI_WritePNGFile("test_widget_opacity.png", &canvas);
	Graph_Free(&canvas);
	return ret;
}

int test_widget_opacity(void)
{
	int ret;

	LCUI_Init();

	build();
	ret = check_widget_opactiy();
	LCUI_Destroy();
	return ret;
}

#ifdef PREVIEW_MODE

int tests_count = 0;

static void OnOpacityPlus(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	wchar_t str[32];
	float opacity = self.parent->computed_style.opacity - 0.1f;

	if (opacity < 0.1f) {
		opacity = 0.1f;
	}
	swprintf(str, 32, L"%.1f", opacity);
	Widget_SetOpacity(self.parent, opacity);
	TextView_SetTextW(self.text, str);
}

static void OnOpacityMinus(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	wchar_t str[32];
	float opacity = self.parent->computed_style.opacity + 0.1f;

	if (opacity > 1.0f) {
		opacity = 1.0f;
	}
	swprintf(str, 32, L"%.1f", opacity);
	Widget_SetOpacity(self.parent, opacity);
	TextView_SetTextW(self.text, str);
}

int main(void)
{
	LCUI_Widget btn_plus, btn_minus;

#ifdef LCUI_BUILD_IN_WIN32
	Logger_SetHandler(LoggerHandler);
	Logger_SetHandlerW(LoggerHandlerW);
#endif
	LCUI_Init();

	build();

	btn_plus = LCUIWidget_GetById("btn-plus");
	btn_minus = LCUIWidget_GetById("btn-minus");
	Widget_BindEvent(btn_plus, "click", OnOpacityPlus, NULL, NULL);
	Widget_BindEvent(btn_minus, "click", OnOpacityMinus, NULL, NULL);

	return LCUI_Main();
}

#endif
