/*
 * lib/css/src/properties/white_space.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_white_space(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	computed->type_bits.white_space = CSS_WHITE_SPACE_NORMAL;
	switch (input[0].keyword_value) {
	case CSS_KEYWORD_NOWRAP:
		computed->type_bits.white_space = CSS_WHITE_SPACE_NOWRAP;
		break;
	default:
		break;
	}
	return 0;
}
