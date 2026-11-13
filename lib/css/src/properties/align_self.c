/*
 * lib/css/src/properties/align_self.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_align_self(const css_style_array_value_t input,
                           css_computed_style_t *computed)
{
        uint8_t value = CSS_ALIGN_SELF_AUTO;

        switch (input[0].keyword_value) {
        case CSS_KEYWORD_STRETCH:
        case CSS_KEYWORD_NORMAL:
                value = CSS_ALIGN_SELF_STRETCH;
                break;
        case CSS_KEYWORD_FLEX_START:
                value = CSS_ALIGN_SELF_FLEX_START;
                break;
        case CSS_KEYWORD_FLEX_END:
                value = CSS_ALIGN_SELF_FLEX_END;
                break;
        case CSS_KEYWORD_CENTER:
                value = CSS_ALIGN_SELF_CENTER;
                break;
        default:
                break;
        }
        computed->type_bits.align_self = value;
        return 0;
}
