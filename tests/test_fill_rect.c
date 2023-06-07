#include <LCUI.h>
#include <pandagl.h>

int main(void)
{
	int i, j;
	pd_canvas_t canvas;
	pd_color_t color;
	pd_rect_t rect;

	pd_canvas_init(&canvas);
	pd_canvas_create(&canvas, 150, 150);
	for (i = 0; i < 6; ++i) {
		for (j = 0; j < 6; ++j) {
			color.red = (unsigned char)(255 - 42.5 * i);
			color.green = (unsigned char)(255 - 42.5 * j);
			color.blue = 0;
			rect.x = j * 25;
			rect.y = i * 25;
			rect.width = 25;
			rect.height = 25;
			pd_canvas_fill_rect(&canvas, color, rect);
		}
	}
	pd_write_png_file("test_fill_rect.png", &canvas);
	pd_canvas_destroy(&canvas);
	return 0;
}
