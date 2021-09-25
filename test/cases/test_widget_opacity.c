#include <stdio.h>
#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/builder.h>
#include "ctest.h"

#define PARENT_OPACITY 0.8f
#define CHILD_OPACITY 0.5f

static struct {
	LCUI_Widget parent;
	LCUI_Widget child;
	LCUI_Widget text;
} self;

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

static int check_color(pd_color_t a, pd_color_t b)
{
	return abs(a.r - b.r) < 2 && abs(a.g - b.g) < 2 && abs(a.b - b.b) < 2;
}

static void check_widget_opactiy(void)
{
	pd_canvas_t canvas;
	pd_color_t color;
	pd_color_t tmp;
	pd_color_t expected_color;
	pd_color_t child_bgcolor = RGB(0, 255, 0);
	pd_color_t child_footer_bgcolor = RGB(255, 255, 255);
	pd_color_t parent_bgcolor = RGB(255, 0, 0);
	pd_color_t parent_bcolor = RGB(0, 0, 0);
	pd_color_t bgcolor = RGB(255, 255, 255);
	pd_rect_t rect = { 0, 0, 400, 256 };
	pd_paint_context_t paint;

	pd_graph_init(&canvas);
	pd_graph_create(&canvas, rect.width, rect.height);
	pd_graph_fill_rect(&canvas, bgcolor, NULL, FALSE);

	paint.with_alpha = FALSE;
	paint.rect.width = 400;
	paint.rect.height = 256;
	paint.rect.x = paint.rect.y = 0;
	pd_graph_quote(&paint.canvas, &canvas, &rect);

	Widget_SetOpacity(self.parent, 0.8f);
	Widget_Resize(self.parent, 512, 256);
	Widget_UpdateStyle(self.child, TRUE);
	Widget_UpdateStyle(self.parent, TRUE);
	Widget_Update(self.child);
	Widget_Update(self.parent);
	Widget_Update(self.parent);

	Widget_Render(self.parent, &paint);

	expected_color = bgcolor;
	graph_get_pixel(&canvas, 10, 10, color);
	PIXEL_BLEND(&expected_color, &parent_bcolor,
		    (int)(PARENT_OPACITY * 255));
	it_b("check parent border color", check_color(expected_color, color),
	     TRUE);

	expected_color = bgcolor;
	graph_get_pixel(&canvas, 30, 30, color);
	PIXEL_BLEND(&expected_color, &parent_bgcolor,
		    (int)(PARENT_OPACITY * 255));
	it_b("check parent background color",
	     check_color(expected_color, color), TRUE);

	tmp = parent_bgcolor;
	expected_color = bgcolor;
	graph_get_pixel(&canvas, 60, 90, color);
	PIXEL_BLEND(&tmp, &child_bgcolor, (int)(CHILD_OPACITY * 255));
	PIXEL_BLEND(&expected_color, &tmp, (int)(PARENT_OPACITY * 255));
	it_b("check child 1 background color",
	     check_color(expected_color, color), TRUE);

	tmp = parent_bgcolor;
	expected_color = bgcolor;
	graph_get_pixel(&canvas, 60, 120, color);
	PIXEL_BLEND(&tmp, &child_footer_bgcolor, (int)(CHILD_OPACITY * 255));
	PIXEL_BLEND(&expected_color, &tmp, (int)(PARENT_OPACITY * 255));
	it_b("check child 1 footer background color",
	     check_color(expected_color, color), TRUE);

	expected_color = bgcolor;
	graph_get_pixel(&canvas, 220, 90, color);
	PIXEL_BLEND(&expected_color, &child_bgcolor,
		    (int)(PARENT_OPACITY * 255));
	it_b("check child 2 background color",
	     check_color(expected_color, color), TRUE);
	expected_color = child_footer_bgcolor;
	graph_get_pixel(&canvas, 220, 120, color);
	it_b("check child 2 footer background color",
	     check_color(expected_color, color), TRUE);

	pd_graph_free(&canvas);
}

void test_widget_opacity(void)
{
	LCUI_Init();

	build();
	describe("check widget opacity", check_widget_opactiy);
	LCUI_Destroy();
}
