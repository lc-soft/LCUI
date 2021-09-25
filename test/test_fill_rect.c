#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

int main(void)
{
	int i, j;
	pd_canvas_t canvas;
	pd_color_t color;
	pd_rect_t rect;

	pd_graph_init(&canvas);
	pd_graph_create(&canvas, 150, 150);
	for (i = 0; i < 6; ++i) {
		for (j = 0; j < 6; ++j) {
			color.red = (unsigned char)(255 - 42.5 * i);
			color.green = (unsigned char)(255 - 42.5 * j);
			color.blue = 0;
			rect.x = j * 25;
			rect.y = i * 25;
			rect.width = 25;
			rect.height = 25;
			pd_graph_fill_rect(&canvas, color, &rect, FALSE);
		}
	}
	LCUI_WritePNGFile("test_fill_rect.png", &canvas);
	pd_graph_free(&canvas);
	return 0;
}
