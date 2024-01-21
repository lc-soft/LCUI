/*
 * lib/css/src/properties/z_index.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_z_index(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	if (input[0].type == CSS_NUMERIC_VALUE) {
		computed->z_index = input[0].numeric_value;
	} else {
		computed->z_index = 0;
	}
	computed->type_bits.z_index = CSS_Z_INDEX_SET;
	return 0;
}
