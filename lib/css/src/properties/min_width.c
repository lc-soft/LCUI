/*
 * lib/css/src/properties/min_width.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"
#include "./helpers.h"

static void set_min_width(css_computed_style_t* computed, uint8_t keyword,
                          css_numeric_value_t value, css_unit_t unit)
{
        computed->min_width = value;
        computed->unit_bits.min_width = unit;
        computed->type_bits.min_width = keyword;
}

int css_cascade_min_width(const css_style_array_value_t input,
                          css_computed_style_t* computed)
{
        if (input[0].type == CSS_KEYWORD_VALUE &&
            input[0].keyword_value == CSS_KEYWORD_MIN_CONTENT) {
                set_min_width(computed, CSS_MIN_WIDTH_MIN_CONTENT, 0, CSS_UNIT_PX);
                return 0;
        }
        return css_cascade_length_auto(input, computed, set_min_width);
}
