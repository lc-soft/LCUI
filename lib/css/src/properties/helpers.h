/*
 * lib/css/src/properties/helpers.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_CSS_SRC_PROPERTIES_HELPERS_H
#define LIB_CSS_SRC_PROPERTIES_HELPERS_H

#include <css/types.h>

int css_cascade_length_auto(const css_style_array_value_t input,
			    css_computed_style_t* computed,
			    void (*setter)(css_computed_style_t*, uint8_t,
					   css_numeric_value_t, css_unit_t));

int css_cascade_bg_border_color(const css_style_array_value_t input,
				css_computed_style_t* computed,
				void (*setter)(css_computed_style_t*, uint8_t,
					       css_color_value_t));
#endif
