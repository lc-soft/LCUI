#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/draw/border.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include "test.h"

int test_border1(void)
{
	int ret = 0;
	LCUI_Widget root, box;

	LCUI_Init();
	box = LCUIBuilder_LoadFile("test_border.xml");
	if (!box) {
		LCUI_Destroy();
		return ret;
	}
	root = LCUIWidget_GetRoot();
	Widget_Append(root, box);
	Widget_Unwrap(box);
	return LCUI_Main();
}

int test_border(void)
{
	LCUI_Border b;
	LCUI_Rect rect;
	LCUI_Graph canvas;
	LCUI_PaintContextRec paint;

	b.top.width = 10;
	b.right.width = 20;
	b.bottom.width = 20;
	b.left.width = 20;
	b.top.style = SV_SOLID;
	b.right.style = SV_SOLID;
	b.bottom.style = SV_SOLID;
	b.left.style = SV_SOLID;
	b.top.color = RGB(255, 0, 0);
	b.right.color = RGB(0, 0, 0);
	b.bottom.color = RGB(0, 0, 0);
	b.left.color = RGB(0, 255, 0);
	b.bottom_left_radius = 40;
	b.bottom_right_radius = 40;
	b.top_left_radius = 40;
	b.top_right_radius = 40;

	rect.x = 20;
	rect.y = 20;
	rect.width = 100;
	rect.height = 100;

	Graph_Init(&canvas);
	canvas.color_type = LCUI_COLOR_TYPE_ARGB8888;
	Graph_Create(&canvas, 200, 200);
	Graph_FillRect(&canvas, RGB(255, 255, 255), NULL, TRUE);
	Graph_Quote(&paint.canvas, &canvas, &rect);
	paint.rect = rect;
	Border_Paint(&b, &rect, &paint);
	LCUI_WritePNGFile("test_border.png", &canvas);
	Graph_Free(&canvas);
	return 0;
}
