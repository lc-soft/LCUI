/*
 * lib/css/src/properties/line_height.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"
#include "./helpers.h"

int css_cascade_line_height(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	switch (input[0].type) {
	case CSS_UNIT_VALUE:
		computed->line_height = input[0].unit_value.value;
		computed->unit_bits.line_height = input[0].unit_value.unit;
		computed->type_bits.line_height = CSS_LINE_HEIGHT_DIMENSION;
		break;
	case CSS_NUMERIC_VALUE:
		computed->line_height = input[0].numeric_value;
		computed->type_bits.line_height = CSS_LINE_HEIGHT_NUMBER;
		break;
	case CSS_KEYWORD_VALUE:
		if (input[0].keyword_value == CSS_KEYWORD_NORMAL) {
			computed->type_bits.line_height = CSS_LINE_HEIGHT_NORMAL;
			break;
		}
	default:
		return -1;
	}
	return 0;
}
