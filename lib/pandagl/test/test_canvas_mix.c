#include <stdio.h>
#include "test.h"
#include "ctest.h"
#include "../include/pandagl.h"

int color2str(char *str, const pd_color_t *color)
{
	return sprintf(str, "rgba(%d, %d, %d, %g)", color->r, color->g,
		       color->b, color->a / 255.f);
}

void test_canvas_mix(void)
{
	char rgba_str[64];
	pd_color_t *pixel;
	pd_color_t white = RGB(255, 255, 255);
	pd_color_t red = RGB(255, 0, 0);
	pd_color_t blue = ARGB(127, 0, 0, 255);
	pd_canvas_t white_layer;
	pd_canvas_t red_layer;
	pd_canvas_t blue_layer;

	pd_canvas_init(&white_layer);
	white_layer.color_type = PD_COLOR_TYPE_ARGB;
	pd_canvas_create(&white_layer, 100, 100);
	pd_canvas_fill(&white_layer, white);

	pd_canvas_init(&red_layer);
	red_layer.color_type = PD_COLOR_TYPE_ARGB;
	red_layer.opacity = 0.5;
	pd_canvas_create(&red_layer, 80, 80);
	pd_canvas_fill(&red_layer, red);

	pd_canvas_init(&blue_layer);
	blue_layer.color_type = PD_COLOR_TYPE_ARGB;
	pd_canvas_create(&blue_layer, 60, 60);
	pd_canvas_fill(&blue_layer, blue);

	pd_canvas_mix(&red_layer, &blue_layer, 10, 10, TRUE);
	pixel = pd_canvas_pixel_at(&red_layer, 15, 15);
	color2str(rgba_str, pixel);
	it_s("red_layer(15, 15)", rgba_str, "rgba(128, 0, 127, 1)");

	pd_canvas_mix(&white_layer, &red_layer, 10, 10, TRUE);
	pixel = pd_canvas_pixel_at(&white_layer, 0, 0);
	color2str(rgba_str, pixel);
	it_s("white_layer(0, 0)", rgba_str, "rgba(255, 255, 255, 1)");

	pixel = pd_canvas_pixel_at(&white_layer, 15, 15);
	color2str(rgba_str, pixel);
	it_s("white_layer(15, 15)", rgba_str, "rgba(255, 128, 128, 1)");

	pixel = pd_canvas_pixel_at(&white_layer, 25, 25);
	color2str(rgba_str, pixel);
	it_s("white_layer(25, 25)", rgba_str, "rgba(192, 128, 191, 1)");

	pd_canvas_destroy(&white_layer);
	pd_canvas_destroy(&red_layer);
	pd_canvas_destroy(&blue_layer);
}
