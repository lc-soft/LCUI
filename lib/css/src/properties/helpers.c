/*
 * lib/css/src/properties/helpers.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "helpers.h"
#include "../properties.h"

int css_cascade_length_auto(const css_style_array_value_t input,
			    css_computed_style_t* computed,
			    void (*setter)(css_computed_style_t*, uint8_t,
					   css_numeric_value_t, css_unit_t))
{
	switch (input[0].type) {
	case CSS_UNIT_VALUE:
		setter(computed, CSS_LENGTH_SET, input[0].unit_value.value,
		       input[0].unit_value.unit);
		break;
	case CSS_KEYWORD_VALUE:
		if (input[0].keyword_value == CSS_KEYWORD_AUTO) {
			setter(computed, CSS_LENGTH_AUTO, 0, CSS_UNIT_PX);
			break;
		}
		return -1;
	case CSS_NUMERIC_VALUE:
		if (input[0].numeric_value == 0) {
			setter(computed, CSS_LENGTH_SET, 0, CSS_UNIT_PX);
			break;
		}
		return -1;
	default:
		return -1;
	}
	return 0;
}

int css_cascade_bg_border_color(const css_style_array_value_t input,
				css_computed_style_t* computed,
				void (*setter)(css_computed_style_t*, uint8_t,
					       css_color_value_t))
{
	switch (input[0].type) {
	case CSS_COLOR_VALUE:
		setter(computed, CSS_BACKGROUND_COLOR_COLOR,
		       input[0].color_value);
		break;
	default:
		return -1;
	}
	return 0;
}
