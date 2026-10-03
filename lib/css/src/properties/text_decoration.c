/*
 * lib/css/src/properties/text_decoration.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

/* The text-decoration shorthand expands into the longhand components
 * currently supported: line (underline/none) and thickness (auto/length).
 * The style and color components are not supported yet and are rejected
 * by the value definition. Expansion uses css_style_decl_set so that a
 * component keeps the last declaration within the same block, matching
 * the CSS ordering between shorthand and its longhands. */
int css_parse_text_decoration(const css_propdef_t *propdef, const char *input,
                              css_style_decl_t *s)
{
        unsigned i;
        int len;
        css_style_value_t value;
        css_style_value_t *item;

        len = css_parse_value(propdef->valdef, input, &value);
        if (len <= 0) {
                return len;
        }
        for (i = 0; i < css_style_value_get_array_length(&value); ++i) {
                item = &value.array_value[i];
                if (item->type == CSS_KEYWORD_VALUE &&
                    (item->keyword_value == CSS_KEYWORD_UNDERLINE ||
                     item->keyword_value == CSS_KEYWORD_NONE)) {
                        css_style_decl_set(s, css_prop_text_decoration_line,
                                           item);
                } else {
                        css_style_decl_set(
                            s, css_prop_text_decoration_thickness, item);
                }
        }
        css_style_value_destroy(&value);
        return len;
}
