#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <stdio.h>

int main(void)
{
	int i, j;
	LCUI_Graph canvas;
	LCUI_Graph fore_canvas;
	LCUI_Color color;
	LCUI_Rect rect;

	Graph_Init(&canvas);
	Graph_Init(&fore_canvas);
	Graph_Create(&canvas, 160, 160);
	// 画背景
	rect.x = 0;
	rect.y = 0;
	rect.width = 160;
	rect.height = 40;
	Graph_FillRect(&canvas, RGB(255, 221, 0), &rect, FALSE);
	rect.y += 40;
	Graph_FillRect(&canvas, RGB(102, 204, 0), &rect, FALSE);
	rect.y += 40;
	Graph_FillRect(&canvas, RGB(0, 153, 255), &rect, FALSE);
	rect.y += 40;
	Graph_FillRect(&canvas, RGB(255, 51, 0), &rect, FALSE);
	color.red = 255;
	color.green = 255;
	color.blue = 255;
	fore_canvas.color_type = LCUI_COLOR_TYPE_ARGB;
	Graph_Create(&fore_canvas, 15, 30);
	// 画半透明矩形
	for (i = 0; i < 10; ++i) {
		color.alpha = (unsigned char)(255 * (i + 1) / 10.0);
		Graph_FillRect(&fore_canvas, color, NULL, TRUE);
		for (j = 0; j < 4; ++j) {
			Graph_Mix(&canvas, &fore_canvas, 5 + i * 15, 5 + j * 40, TRUE);
		}
	}
	LCUI_WritePNGFile("test_fill_rect_with_rgba.png", &canvas);
	Graph_Free(&fore_canvas);
	Graph_Free(&canvas);
	return 0;
}
