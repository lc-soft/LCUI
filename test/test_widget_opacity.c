#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/image.h>
#include "test.h"

#define OPACITY 0.1

/* clang-format off */

static struct {
	LCUI_Widget parent;
	LCUI_Widget child;
} self;

static const char *css = CodeToString(

.parent {
	opacity: 0.1;
	padding: 20px;
	margin: 20px;
	display: inline-block;
	background-color: #f00;
	border: 20px solid #000;
}

.child {
	width: 100px;
	height: 100px;
	background-color: #0f0;
}

);

/* clang-format on */

static void build(void)
{
	self.child = LCUIWidget_New(NULL);
	self.parent = LCUIWidget_New(NULL);
	Widget_SetId(self.parent, "parent");
	Widget_SetId(self.child, "child");
	Widget_AddClass(self.parent, "parent");
	Widget_AddClass(self.child, "child");
	Widget_Append(self.parent, self.child);
	LCUI_LoadCSSString(css, __FILE__);
}

static void destroy(void)
{
	Widget_Destroy(self.parent);
}

static int check_widget_opactiy(void)
{
	int ret = 0;
	LCUI_Graph canvas;
	LCUI_Color color;
	LCUI_Color expected_color;
	LCUI_Color child_bgcolor = RGB(0, 255, 0);
	LCUI_Color parent_bgcolor = RGB(255, 0, 0);
	LCUI_Color parent_bcolor = RGB(0, 0, 0);
	LCUI_Color bgcolor = RGB(255, 255, 255);
	LCUI_Rect rect = { 0, 0, 220, 220 };
	LCUI_PaintContextRec paint;

	Graph_Init(&canvas);
	Graph_Create(&canvas, rect.width, rect.height);
	Graph_FillRect(&canvas, bgcolor, NULL, FALSE);

	paint.with_alpha = FALSE;
	paint.rect.x = paint.rect.y = 0;
	paint.rect.width = paint.rect.height = 180;
	Graph_Quote(&paint.canvas, &canvas, &rect);

	Widget_UpdateStyle(self.child, TRUE);
	Widget_UpdateStyle(self.parent, TRUE);
	Widget_Update(self.child);
	Widget_Update(self.parent);

	Widget_Render(self.parent, &paint);

	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 10, 10, color);
	PIXEL_BLEND(&expected_color, &parent_bcolor, (int)(OPACITY * 255));
	CHECK_WITH_TEXT("check parent border color",
			expected_color.value == color.value);

	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 30, 30, color);
	PIXEL_BLEND(&expected_color, &parent_bgcolor, (int)(OPACITY * 255));
	CHECK_WITH_TEXT("check parent background color",
			expected_color.value == color.value);

	expected_color = bgcolor;
	Graph_GetPixel(&canvas, 90, 90, color);
	PIXEL_BLEND(&expected_color, &child_bgcolor, (int)(OPACITY * 255));
	CHECK_WITH_TEXT("check child background color",
			expected_color.value == color.value);

	Graph_Free(&canvas);
	return ret;
}

int test_widget_opacity(void)
{
	int ret;

	LCUI_Init();

	build();
	ret = check_widget_opactiy();
	destroy();

	LCUI_Destroy();
	return ret;
}

#ifdef PREVIEW_MODE

int tests_count = 0;

int main(void)
{
	LCUI_Widget root;

	LCUI_Init();

	build();

	root = LCUIWidget_GetRoot();
	Widget_Append(root, self.parent);

	return LCUI_Main();
}

#endif
