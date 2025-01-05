/*
 * lib/css/src/properties/box_sizing.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_box_sizing(const css_style_array_value_t input,
			   css_computed_style_t* computed)
{
	uint8_t value = CSS_BOX_SIZING_CONTENT_BOX;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_BORDER_BOX:
		value = CSS_BOX_SIZING_BORDER_BOX;
		break;
	default:
		break;
	}
	computed->type_bits.box_sizing = value;
	return 0;
}
