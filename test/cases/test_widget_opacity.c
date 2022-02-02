#include <stdio.h>
#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/ui.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/css.h>
#include <LCUI/ui/builder.h>
#include <LCUI/graph.h>
#include "ctest.h"

#define PARENT_OPACITY 0.8f
#define CHILD_OPACITY 0.5f

static struct {
	ui_widget_t* parent;
	ui_widget_t* child;
	ui_widget_t* text;
} self;

static void build(void)
{
	ui_widget_t *pack, *root;

	pack = ui_load_xml("test_widget_opacity.xml");
	root = ui_root();
	ui_widget_append(root, pack);
	ui_widget_unwrap(pack);
	self.parent = ui_get_widget("parent");
	self.child = ui_get_widget("child");
	self.text = ui_get_widget("current-opacity");
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

	pd_canvas_init(&canvas);
	pd_canvas_create(&canvas, rect.width, rect.height);
	pd_canvas_fill_rect(&canvas, bgcolor, NULL, FALSE);

	paint.with_alpha = FALSE;
	paint.rect.width = 400;
	paint.rect.height = 256;
	paint.rect.x = paint.rect.y = 0;
	pd_canvas_quote(&paint.canvas, &canvas, &rect);

	ui_widget_set_opacity(self.parent, 0.8f);
	ui_widget_resize(self.parent, 512, 256);
	ui_widget_refresh_style(self.child);
	ui_widget_refresh_style(self.parent);
	ui_widget_update(self.child);
	ui_widget_update(self.parent);
	ui_widget_update(self.parent);

	ui_widget_render(self.parent, &paint);

	expected_color = bgcolor;
	pd_canvas_get_pixel(&canvas, 10, 10, color);
	pd_pixel_blend(&expected_color, &parent_bcolor,
		    (int)(PARENT_OPACITY * 255));
	it_b("check parent border color", check_color(expected_color, color),
	     TRUE);

	expected_color = bgcolor;
	pd_canvas_get_pixel(&canvas, 30, 30, color);
	pd_pixel_blend(&expected_color, &parent_bgcolor,
		    (int)(PARENT_OPACITY * 255));
	it_b("check parent background color",
	     check_color(expected_color, color), TRUE);

	tmp = parent_bgcolor;
	expected_color = bgcolor;
	pd_canvas_get_pixel(&canvas, 60, 90, color);
	pd_pixel_blend(&tmp, &child_bgcolor, (int)(CHILD_OPACITY * 255));
	pd_pixel_blend(&expected_color, &tmp, (int)(PARENT_OPACITY * 255));
	it_b("check child 1 background color",
	     check_color(expected_color, color), TRUE);

	tmp = parent_bgcolor;
	expected_color = bgcolor;
	pd_canvas_get_pixel(&canvas, 60, 120, color);
	pd_pixel_blend(&tmp, &child_footer_bgcolor, (int)(CHILD_OPACITY * 255));
	pd_pixel_blend(&expected_color, &tmp, (int)(PARENT_OPACITY * 255));
	it_b("check child 1 footer background color",
	     check_color(expected_color, color), TRUE);

	expected_color = bgcolor;
	pd_canvas_get_pixel(&canvas, 220, 90, color);
	pd_pixel_blend(&expected_color, &child_bgcolor,
		    (int)(PARENT_OPACITY * 255));
	it_b("check child 2 background color",
	     check_color(expected_color, color), TRUE);
	expected_color = child_footer_bgcolor;
	pd_canvas_get_pixel(&canvas, 220, 120, color);
	it_b("check child 2 footer background color",
	     check_color(expected_color, color), TRUE);

	pd_canvas_free(&canvas);
}

void test_widget_opacity(void)
{
	lcui_init();

	build();
	describe("check widget opacity", check_widget_opactiy);
	lcui_destroy();
}
