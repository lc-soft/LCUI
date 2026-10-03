/*
 * lib/css/src/properties/text_decoration_thickness.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_text_decoration_thickness(const css_style_array_value_t input,
                                          css_computed_style_t *computed)
{
        switch (input[0].type) {
        case CSS_UNIT_VALUE:
                computed->text_decoration_thickness = input[0].unit_value.value;
                computed->unit_bits.text_decoration_thickness =
                    input[0].unit_value.unit;
                computed->type_bits.text_decoration_thickness =
                    CSS_TEXT_DECORATION_THICKNESS_SET;
                break;
        case CSS_KEYWORD_VALUE:
                if (input[0].keyword_value != CSS_KEYWORD_AUTO) {
                        return -1;
                }
                computed->text_decoration_thickness = 0;
                computed->unit_bits.text_decoration_thickness = CSS_UNIT_PX;
                computed->type_bits.text_decoration_thickness =
                    CSS_TEXT_DECORATION_THICKNESS_AUTO;
                break;
        default:
                return -1;
        }
        return 0;
}
