/*
 * lib/css/src/properties/vertical_align.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_vertical_align(const css_style_array_value_t input,
			       css_computed_style_t* computed)
{
	css_vertical_align_t value;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_MIDDLE:
		value = CSS_VERTICAL_ALIGN_MIDDLE;
		break;
	case CSS_KEYWORD_BOTTOM:
		value = CSS_VERTICAL_ALIGN_BOTTOM;
		break;
	case CSS_KEYWORD_TOP:
	default:
		value = CSS_VERTICAL_ALIGN_TOP;
		break;
	}
	computed->type_bits.vertical_align = value;
	return 0;
}
