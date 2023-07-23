#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_PUBLIC pd_color_t pd_color(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

PD_END_DECLS

#define pd_argb pd_color
#define pd_rgb(R, G, B) pd_color(255, R, G, B)

#endif
