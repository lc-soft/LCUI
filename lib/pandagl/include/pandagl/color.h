#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include "def.h"

INLINE pd_color_t pd_color(uchar_t a, uchar_t r, uchar_t g, uchar_t b)
{
	return a | r << 8 | g << 16 | b << 24;
}

#define ARGB pd_color
#define RGB(R, G, B) pd_color(255, R, G, B)

#endif
