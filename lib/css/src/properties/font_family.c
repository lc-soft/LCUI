/*
 * lib/css/src/properties/font_family.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"
#include "./helpers.h"

int css_cascade_font_family(const css_style_array_value_t input,
                            css_computed_style_t* computed)
{
        char* str;
        unsigned i, len;
	size_t str_len;
        const css_style_array_value_t arr = input[0].array_value;

        computed->font_family = NULL;
        computed->type_bits.font_family = CSS_FONT_FAMILY_INHERIT;
        if (input[0].type != CSS_ARRAY_VALUE) {
                return -1;
        }
        len = css_array_value_get_length(arr);
        if (len < 1) {
                return -1;
        }
        computed->font_family = malloc(sizeof(char*) * (len + 1));
        for (i = 0; i < len; ++i) {
                assert(arr[i].type == CSS_STRING_VALUE);
                str = arr[i].string_value;
                str_len = strlen(str);
                computed->font_family[i] = malloc(sizeof(char) * (str_len + 1));
                if (str[0] == str[str_len - 1] &&
                    (str[0] == '\'' || str[0] == '"')) {
                        strncpy(computed->font_family[i], str + 1, str_len);
                        computed->font_family[i][str_len - 2] = 0;
                } else {
                        strncpy(computed->font_family[i], str, str_len + 1);
                }
        }
        computed->font_family[len] = NULL;
        return 0;
}
