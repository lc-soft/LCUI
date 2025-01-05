/*
 * lib/css/src/properties/align_content.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_align_content(const css_style_array_value_t input,
			      css_computed_style_t* computed)
{
	uint8_t value = CSS_ALIGN_CONTENT_FLEX_START;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_CENTER:
		value = CSS_ALIGN_CONTENT_CENTER;
		break;
	case CSS_KEYWORD_FLEX_START:
		value = CSS_ALIGN_CONTENT_FLEX_START;
		break;
	case CSS_KEYWORD_FLEX_END:
		value = CSS_ALIGN_CONTENT_FLEX_END;
		break;
	case CSS_KEYWORD_SPACE_AROUND:
		value = CSS_ALIGN_CONTENT_SPACE_AROUND;
		break;
	case CSS_KEYWORD_SPACE_BETWEEN:
		value = CSS_ALIGN_CONTENT_SPACE_BETWEEN;
		break;
	case CSS_KEYWORD_SPACE_EVENLY:
		value = CSS_ALIGN_CONTENT_SPACE_EVENLY;
		break;
	default:
		break;
	}
	computed->type_bits.align_content = value;
	return 0;
}
