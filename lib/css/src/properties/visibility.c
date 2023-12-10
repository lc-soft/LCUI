/*
 * lib/css/src/properties/visibility.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_visibility(const css_style_array_value_t input,
			   css_computed_style_t* computed)
{
	uint8_t value = CSS_VISIBILITY_VISIBLE;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_HIDDEN:
		value = CSS_VISIBILITY_HIDDEN;
		break;
	default:
		break;
	}
	computed->type_bits.visibility = value;
	return 0;
}
