/*
 * lib/pandagl/src/tile.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>

int pd_canvas_tile(pd_canvas_t *buff, const pd_canvas_t *canvas,
		   pd_bool_t replace, pd_bool_t with_alpha)
{
	int ret = 0;
	unsigned x, y;

	if (!pd_canvas_is_valid(canvas) || !pd_canvas_is_valid(buff)) {
		return -1;
	}
	for (y = 0; y < buff->height; y += canvas->height) {
		for (x = 0; x < buff->width; x += canvas->width) {
			if (replace) {
				ret += pd_canvas_replace(buff, canvas, y, x);
				continue;
			}
			ret += pd_canvas_mix(buff, canvas, y, x, with_alpha);
		}
	}
	return ret;
}
