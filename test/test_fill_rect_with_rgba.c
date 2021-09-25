#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <stdio.h>

int main(void)
{
	int i, j;
	pd_canvas_t canvas;
	pd_canvas_t fore_canvas;
	pd_color_t color;
	pd_rect_t rect;

	pd_graph_init(&canvas);
	pd_graph_init(&fore_canvas);
	pd_graph_create(&canvas, 160, 160);
	// 画背景
	rect.x = 0;
	rect.y = 0;
	rect.width = 160;
	rect.height = 40;
	pd_graph_fill_rect(&canvas, RGB(255, 221, 0), &rect, FALSE);
	rect.y += 40;
	pd_graph_fill_rect(&canvas, RGB(102, 204, 0), &rect, FALSE);
	rect.y += 40;
	pd_graph_fill_rect(&canvas, RGB(0, 153, 255), &rect, FALSE);
	rect.y += 40;
	pd_graph_fill_rect(&canvas, RGB(255, 51, 0), &rect, FALSE);
	color.red = 255;
	color.green = 255;
	color.blue = 255;
	fore_canvas.color_type = LCUI_COLOR_TYPE_ARGB;
	pd_graph_create(&fore_canvas, 15, 30);
	// 画半透明矩形
	for (i = 0; i < 10; ++i) {
		color.alpha = (unsigned char)(255 * (i + 1) / 10.0);
		pd_graph_fill_rect(&fore_canvas, color, NULL, TRUE);
		for (j = 0; j < 4; ++j) {
			pd_graph_mix(&canvas, &fore_canvas, 5 + i * 15, 5 + j * 40, TRUE);
		}
	}
	LCUI_WritePNGFile("test_fill_rect_with_rgba.png", &canvas);
	pd_graph_free(&fore_canvas);
	pd_graph_free(&canvas);
	return 0;
}
