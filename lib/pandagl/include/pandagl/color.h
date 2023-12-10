/*
 * lib/pandagl/include/pandagl/color.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_COLOR_H

#include "common.h"
#include "types.h"

PD_INLINE pd_color_t pd_color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	pd_color_t color = { .a = a, .r = r, .g = g, .b = b };
	return color;
}

#define pd_argb pd_color
#define pd_rgb(R, G, B) pd_color(255, R, G, B)

#endif
