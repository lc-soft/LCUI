#include <pandagl/color.h>

pd_color_t pd_color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	pd_color_t color = { .a = a, .r = r, .g = g, .b = b };
	return color;
}
