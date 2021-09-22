#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

int main(void)
{
	int i, j;
	LCUI_Graph canvas;
	LCUI_Color color;
	LCUI_Rect rect;

	Graph_Init(&canvas);
	Graph_Create(&canvas, 150, 150);
	for (i = 0; i < 6; ++i) {
		for (j = 0; j < 6; ++j) {
			color.red = (unsigned char)(255 - 42.5 * i);
			color.green = (unsigned char)(255 - 42.5 * j);
			color.blue = 0;
			rect.x = j * 25;
			rect.y = i * 25;
			rect.width = 25;
			rect.height = 25;
			Graph_FillRect(&canvas, color, &rect, FALSE);
		}
	}
	LCUI_WritePNGFile("test_fill_rect.png", &canvas);
	Graph_Free(&canvas);
	return 0;
}
