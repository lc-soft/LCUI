/*
 * lib/css/src/properties/font_weight.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_font_weight(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	uint8_t value = CSS_FONT_STYLE_NORMAL;

	switch (input[0].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_NORMAL:
			value = CSS_FONT_WEIGHT_NORMAL;
			break;
		case CSS_KEYWORD_BOLD:
			value = CSS_FONT_WEIGHT_BOLD;
			break;
		default:
			break;
		}
		break;
	case CSS_NUMERIC_VALUE:
		switch ((int)input[0].numeric_value) {
		case 100:
			value = CSS_FONT_WEIGHT_100;
			break;
		case 200:
			value = CSS_FONT_WEIGHT_200;
			break;
		case 300:
			value = CSS_FONT_WEIGHT_300;
			break;
		case 400:
			value = CSS_FONT_WEIGHT_400;
			break;
		case 500:
			value = CSS_FONT_WEIGHT_500;
			break;
		case 600:
			value = CSS_FONT_WEIGHT_600;
			break;
		case 700:
			value = CSS_FONT_WEIGHT_700;
			break;
		case 800:
			value = CSS_FONT_WEIGHT_800;
			break;
		case 900:
			value = CSS_FONT_WEIGHT_900;
			break;
		default:
			break;
		}
	default:
		break;
	}
	computed->type_bits.font_weight = value;
	return 0;
}
