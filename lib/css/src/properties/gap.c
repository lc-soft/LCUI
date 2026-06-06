/*
 * lib/css/src/properties/gap.c
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

static void set_row_gap(css_computed_style_t *computed, uint8_t keyword,
                        css_numeric_value_t value, css_unit_t unit)
{
        computed->row_gap = value;
        computed->unit_bits.row_gap = unit;
        computed->type_bits.row_gap = keyword;
}

static void set_column_gap(css_computed_style_t *computed, uint8_t keyword,
                           css_numeric_value_t value, css_unit_t unit)
{
        computed->column_gap = value;
        computed->unit_bits.column_gap = unit;
        computed->type_bits.column_gap = keyword;
}

/* 仅接受 <length-percentage> 与 normal 关键字；不接受 auto */
static int cascade_gap_length(const css_style_array_value_t input,
                              css_computed_style_t *computed,
                              void (*setter)(css_computed_style_t *, uint8_t,
                                             css_numeric_value_t, css_unit_t))
{
        switch (input[0].type) {
        case CSS_UNIT_VALUE:
                setter(computed, CSS_GAP_SET, input[0].unit_value.value,
                       input[0].unit_value.unit);
                break;
        case CSS_KEYWORD_VALUE:
                if (input[0].keyword_value == CSS_KEYWORD_NORMAL) {
                        setter(computed, CSS_GAP_NORMAL, 0, CSS_UNIT_PX);
                        break;
                }
                return -1;
        case CSS_NUMERIC_VALUE:
                if (input[0].numeric_value == 0) {
                        setter(computed, CSS_GAP_SET, 0, CSS_UNIT_PX);
                        break;
                }
                return -1;
        default:
                return -1;
        }
        return 0;
}

int css_cascade_row_gap(const css_style_array_value_t input,
                        css_computed_style_t *computed)
{
        return cascade_gap_length(input, computed, set_row_gap);
}

int css_cascade_column_gap(const css_style_array_value_t input,
                           css_computed_style_t *computed)
{
        return cascade_gap_length(input, computed, set_column_gap);
}

int css_parse_gap(const css_propdef_t *propdef, const char *input,
                  css_style_decl_t *s)
{
        css_style_value_t value;
        int len = css_parse_value(propdef->valdef, input, &value);

        if (len <= 0) {
                return len;
        }
        switch (css_style_value_get_array_length(&value)) {
        case 1:
                css_style_decl_add(s, css_prop_row_gap, value.array_value);
                css_style_decl_add(s, css_prop_column_gap, value.array_value);
                break;
        case 2:
                css_style_decl_add(s, css_prop_row_gap, value.array_value);
                css_style_decl_add(s, css_prop_column_gap,
                                   value.array_value + 1);
                break;
        default:
                break;
        }
        css_style_value_destroy(&value);
        return len;
}
