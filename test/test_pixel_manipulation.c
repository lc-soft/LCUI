﻿#include <math.h>
#include <LCUI.h>
#include <LCUI/image.h>

#define PixelManipulationBegin                                   \
	int x, y;                                                \
	pd_color_t pixel;                                        \
								 \
	for (y = rect.y; y < rect.y + rect.height; ++y) {        \
		for (x = rect.x; x < rect.x + rect.width; ++x) { \
			graph_get_pixel(graph, x, y, pixel);

#define PixelManipulationEnd                \
	graph_set_pixel(graph, x, y, pixel); \
	}                                   \
	}

void invert(pd_canvas_t *graph, pd_rect_t rect)
{
	PixelManipulationBegin;
	pixel.red = (unsigned char)(255 - pixel.red);
	pixel.green = (unsigned char)(255 - pixel.green);
	pixel.blue = (unsigned char)(255 - pixel.blue);
	PixelManipulationEnd;
}

void grayscale(pd_canvas_t *graph, pd_rect_t rect)
{
	unsigned char avg;

	PixelManipulationBegin;
	avg = (unsigned char)((pixel.red + pixel.green + pixel.blue) / 3);
	pixel.red = avg;
	pixel.green = avg;
	pixel.blue = avg;
	PixelManipulationEnd;
}

void sepia(pd_canvas_t *graph, pd_rect_t rect)
{
	PixelManipulationBegin;
	pixel.red = (unsigned char)min(
	    round(0.393 * pixel.red + 0.769 * pixel.green + 0.189 * pixel.blue),
	    255);
	pixel.green = (unsigned char)min(
	    round(0.349 * pixel.red + 0.686 * pixel.green + 0.168 * pixel.blue),
	    255);
	pixel.blue = (unsigned char)min(
	    round(0.272 * pixel.red + 0.534 * pixel.green + 0.131 * pixel.blue),
	    255);
	PixelManipulationEnd;
}

int main(void)
{
	int i;
	pd_canvas_t graph = { 0 };
	pd_rect_t rects[4];

	if (LCUI_ReadImageFile("dog.jpg", &graph) != 0) {
		return -1;
	}
	for (i = 0; i < 4; ++i) {
		rects[i].height = graph.height;
		rects[i].width = graph.width / 4;
		rects[i].x = i * rects[i].width;
		rects[i].y = 0;
	}
	sepia(&graph, rects[1]);
	grayscale(&graph, rects[2]);
	invert(&graph, rects[3]);
	LCUI_WritePNGFile("test_pixel_manipulation.png", &graph);
	return 0;
}
