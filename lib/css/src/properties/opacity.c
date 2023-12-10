/*
 * lib/css/src/properties/opacity.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_opacity(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	computed->opacity = input[0].numeric_value;
	computed->type_bits.opacity = CSS_OPACITY_SET;
	return 0;
}
