/*
 * lib/css/src/properties/content.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_content(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	if (input[0].type == CSS_STRING_VALUE && input[0].string_value) {
		computed->content = strdup2(input[0].string_value);
		computed->type_bits.content = CSS_CONTENT_SET;
	} else {
		computed->content = NULL;
		computed->type_bits.content = CSS_CONTENT_NONE;
	}
	return 0;
}
