/*
 * lib/css/src/properties/display.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_display(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	uint8_t value = CSS_DISPLAY_BLOCK;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_NONE:
		value = CSS_DISPLAY_NONE;
		break;
	case CSS_KEYWORD_BLOCK:
		value = CSS_DISPLAY_BLOCK;
		break;
	case CSS_KEYWORD_INLINE_BLOCK:
		value = CSS_DISPLAY_INLINE_BLOCK;
		break;
	case CSS_KEYWORD_FLEX:
		value = CSS_DISPLAY_FLEX;
		break;
	default:
		break;
	}
	computed->type_bits.display = value;
	return 0;
}
