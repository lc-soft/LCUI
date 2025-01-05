/*
 * lib/css/src/properties/color.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "./helpers.h"

int css_cascade_color(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	switch (input[0].type) {
	case CSS_COLOR_VALUE:
		computed->color = input[0].color_value;
		computed->type_bits.color = CSS_COLOR_COLOR;
		break;
	default:
		return -1;
	}
	return 0;
}
