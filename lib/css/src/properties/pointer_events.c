/*
 * lib/css/src/properties/pointer_events.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_pointer_events(const css_style_array_value_t input,
			       css_computed_style_t* computed)
{
	uint8_t value = CSS_POINTER_EVENTS_AUTO;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_NONE:
		value = CSS_POINTER_EVENTS_NONE;
	default:
		break;
	}
	computed->type_bits.pointer_events = value;
	return 0;
}
