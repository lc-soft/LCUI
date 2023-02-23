#include <LCUI.h>
#include <pandagl.h>
#include <stdio.h>

int main(void)
{
	int i, j;
	pd_canvas_t canvas;
	pd_canvas_t fore_canvas;
	pd_color_t color;
	pd_rect_t rect;

	pd_canvas_init(&canvas);
	pd_canvas_init(&fore_canvas);
	pd_canvas_create(&canvas, 160, 160);
	// 画背景
	rect.x = 0;
	rect.y = 0;
	rect.width = 160;
	rect.height = 40;
	pd_canvas_fill_rect(&canvas, RGB(255, 221, 0), rect);
	rect.y += 40;
	pd_canvas_fill_rect(&canvas, RGB(102, 204, 0), rect);
	rect.y += 40;
	pd_canvas_fill_rect(&canvas, RGB(0, 153, 255), rect);
	rect.y += 40;
	pd_canvas_fill_rect(&canvas, RGB(255, 51, 0), rect);
	color.red = 255;
	color.green = 255;
	color.blue = 255;
	fore_canvas.color_type = PD_COLOR_TYPE_ARGB;
	pd_canvas_create(&fore_canvas, 15, 30);
	// 画半透明矩形
	for (i = 0; i < 10; ++i) {
		color.alpha = (unsigned char)(255 * (i + 1) / 10.0);
		pd_canvas_fill(&fore_canvas, color);
		for (j = 0; j < 4; ++j) {
			pd_canvas_mix(&canvas, &fore_canvas, 5 + i * 15, 5 + j * 40, TRUE);
		}
	}
	pd_write_png_file("test_fill_rect_with_rgba.png", &canvas);
	pd_canvas_destroy(&fore_canvas);
	pd_canvas_destroy(&canvas);
	return 0;
}
