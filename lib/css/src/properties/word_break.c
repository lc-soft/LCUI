/*
 * lib/css/src/properties/word_break.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_word_break(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	computed->type_bits.word_break = CSS_WORD_BREAK_NORMAL;
	switch (input[0].keyword_value) {
	case CSS_KEYWORD_BREAK_ALL:
		computed->type_bits.word_break = CSS_WORD_BREAK_BREAK_ALL;
		break;
	default:
		break;
	}
	return 0;
}
