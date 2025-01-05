/*
 * lib/css/src/properties/position.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_position(const css_style_array_value_t input,
			 css_computed_style_t* computed)
{
	uint8_t value = CSS_POSITION_STATIC;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_ABSOLUTE:
		value = CSS_POSITION_ABSOLUTE;
		break;
	case CSS_KEYWORD_RELATIVE:
		value = CSS_POSITION_RELATIVE;
		break;
	default:
		break;
	}
	computed->type_bits.position = value;
	return 0;
}
