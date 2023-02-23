#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H

#include "common.h"
#include "types.h"

PD_INLINE pd_color_t pd_color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	pd_color_t color = { .a = a, .r = r, .g = g, .b = b };
	return color;
}

// cancel RGB macro from wingdi.h
#ifdef RGB
#undef RGB
#endif

#define ARGB pd_color
#define RGB(R, G, B) pd_color(255, R, G, B)

#endif
