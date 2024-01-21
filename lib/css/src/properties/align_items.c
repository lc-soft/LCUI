/*
 * lib/css/src/properties/align_items.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_align_items(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	uint8_t  value = CSS_ALIGN_ITEMS_STRETCH;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_CENTER:
		value = CSS_ALIGN_ITEMS_CENTER;
		break;
	case CSS_KEYWORD_FLEX_START:
		value = CSS_ALIGN_ITEMS_FLEX_START;
		break;
	case CSS_KEYWORD_FLEX_END:
		value = CSS_ALIGN_ITEMS_FLEX_END;
		break;
	default:
		break;
	}
	computed->type_bits.align_items = value;
	return 0;
}
