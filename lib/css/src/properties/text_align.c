/*
 * lib/css/src/properties/text_align.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_text_align(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	uint8_t value = CSS_TEXT_ALIGN_LEFT;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_CENTER:
		value = CSS_TEXT_ALIGN_CENTER;
		break;
	case CSS_KEYWORD_RIGHT:
		value = CSS_TEXT_ALIGN_RIGHT;
		break;
	default:
		break;
	}
	computed->type_bits.text_align = value;
	return 0;
}
