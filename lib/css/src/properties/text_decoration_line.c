/*
 * lib/css/src/properties/text_decoration_line.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_text_decoration_line(const css_style_array_value_t input,
                                     css_computed_style_t *computed)
{
        if (input[0].type != CSS_KEYWORD_VALUE) {
                return -1;
        }
        switch (input[0].keyword_value) {
        case CSS_KEYWORD_NONE:
                computed->type_bits.text_decoration = CSS_TEXT_DECORATION_NONE;
                break;
        case CSS_KEYWORD_UNDERLINE:
                computed->type_bits.text_decoration =
                    CSS_TEXT_DECORATION_UNDERLINE;
                break;
        default:
                return -1;
        }
        return 0;
}
