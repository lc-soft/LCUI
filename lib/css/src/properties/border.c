/*
 * lib/css/src/properties/border.c
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

#define CSS_CASCADE_BORDER_SIDE_STYLE(PROP_KEY)                        \
        int css_cascade_border_##PROP_KEY##_style(                     \
            const css_style_array_value_t input,                       \
            css_computed_style_t *computed)                            \
        {                                                              \
                uint8_t value = CSS_BORDER_STYLE_NONE;                 \
                                                                       \
                switch (input[0].keyword_value) {                      \
                case CSS_KEYWORD_SOLID:                                \
                        value = CSS_BORDER_STYLE_SOLID;                \
                        break;                                         \
                default:                                               \
                        break;                                         \
                }                                                      \
                computed->type_bits.border_##PROP_KEY##_style = value; \
                return 0;                                              \
        }

#define CSS_CASCADE_BORDER_ANGLE_RADIUS(PROP_KEY)                      \
        static void set_border_##PROP_KEY##_radius(                    \
            css_computed_style_t *computed, uint8_t keyword,           \
            css_numeric_value_t value, css_unit_t unit)                \
        {                                                              \
                computed->border_##PROP_KEY##_radius = value;          \
                computed->unit_bits.border_##PROP_KEY##_radius = unit; \
        }                                                              \
                                                                       \
        int css_cascade_border_##PROP_KEY##_radius(                    \
            const css_style_array_value_t input,                       \
            css_computed_style_t *computed)                            \
        {                                                              \
                return css_cascade_length_auto(                        \
                    input, computed, set_border_##PROP_KEY##_radius);  \
        }

#define CSS_CASCADE_BORDER_SIDE_WIDTH(PROP_KEY)                                \
        static void set_border_##PROP_KEY##_width(                             \
            css_computed_style_t *computed, uint8_t type,                      \
            css_numeric_value_t value, css_unit_t unit)                        \
        {                                                                      \
                computed->border_##PROP_KEY##_width = value;                   \
                computed->unit_bits.border_##PROP_KEY##_width = unit;          \
        }                                                                      \
                                                                               \
        int css_cascade_border_##PROP_KEY##_width(                             \
            const css_style_array_value_t input,                               \
            css_computed_style_t *computed)                                    \
        {                                                                      \
                return css_cascade_length_auto(input, computed,                \
                                               set_border_##PROP_KEY##_width); \
        }

#define CSS_CASCADE_BORDER_SIDE_COLOR(PROP_KEY)                      \
        static void set_border_##PROP_KEY##_color(                   \
            css_computed_style_t *computed, uint8_t type,            \
            css_color_value_t value)                                 \
        {                                                            \
                computed->border_##PROP_KEY##_color = value;         \
        }                                                            \
                                                                     \
        int css_cascade_border_##PROP_KEY##_color(                   \
            const css_style_array_value_t input,                     \
            css_computed_style_t *computed)                          \
        {                                                            \
                return css_cascade_bg_border_color(                  \
                    input, computed, set_border_##PROP_KEY##_color); \
        }

CSS_CASCADE_BORDER_SIDE_STYLE(top)
CSS_CASCADE_BORDER_SIDE_STYLE(right)
CSS_CASCADE_BORDER_SIDE_STYLE(bottom)
CSS_CASCADE_BORDER_SIDE_STYLE(left)
CSS_CASCADE_BORDER_SIDE_WIDTH(top)
CSS_CASCADE_BORDER_SIDE_WIDTH(right)
CSS_CASCADE_BORDER_SIDE_WIDTH(bottom)
CSS_CASCADE_BORDER_SIDE_WIDTH(left)
CSS_CASCADE_BORDER_SIDE_COLOR(top)
CSS_CASCADE_BORDER_SIDE_COLOR(right)
CSS_CASCADE_BORDER_SIDE_COLOR(bottom)
CSS_CASCADE_BORDER_SIDE_COLOR(left)
CSS_CASCADE_BORDER_ANGLE_RADIUS(top_left)
CSS_CASCADE_BORDER_ANGLE_RADIUS(top_right)
CSS_CASCADE_BORDER_ANGLE_RADIUS(bottom_left)
CSS_CASCADE_BORDER_ANGLE_RADIUS(bottom_right)

int css_parse_border_width(css_propdef_t *propdef, const char *input,
                           css_style_decl_t *s)
{
        css_style_value_t value;
        int len = css_parse_value(propdef->valdef, input, &value);

        if (len <= 0) {
                return len;
        }
        switch (css_style_value_get_array_length(&value)) {
        case 1:
                css_style_decl_add(s, css_prop_border_top_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_right_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_width,
                                   value.array_value);
                break;
        case 2:
                css_style_decl_add(s, css_prop_border_top_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_width,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_right_width,
                                   value.array_value + 1);
                break;
        case 3:
                css_style_decl_add(s, css_prop_border_top_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_width,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_right_width,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_width,
                                   value.array_value + 2);
                break;
        case 4:
                css_style_decl_add(s, css_prop_border_top_width,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_right_width,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_width,
                                   value.array_value + 2);
                css_style_decl_add(s, css_prop_border_left_width,
                                   value.array_value + 3);
                break;
        default:
                break;
        }
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border_style(css_propdef_t *propdef, const char *input,
                           css_style_decl_t *s)
{
        css_style_value_t value;
        int len = css_parse_value(propdef->valdef, input, &value);

        if (len <= 0) {
                return len;
        }
        switch (css_style_value_get_array_length(&value)) {
        case 1:
                css_style_decl_add(s, css_prop_border_top_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_right_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_style,
                                   value.array_value);
                break;
        case 2:
                css_style_decl_add(s, css_prop_border_top_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_style,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_right_style,
                                   value.array_value + 1);
                break;
        case 3:
                css_style_decl_add(s, css_prop_border_top_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_style,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_right_style,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_style,
                                   value.array_value + 2);
                break;
        case 4:
                css_style_decl_add(s, css_prop_border_top_style,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_right_style,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_style,
                                   value.array_value + 2);
                css_style_decl_add(s, css_prop_border_left_style,
                                   value.array_value + 3);
                break;
        default:
                break;
        }
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border_color(css_propdef_t *propdef, const char *input,
                           css_style_decl_t *s)
{
        css_style_value_t value;
        int len = css_parse_value(propdef->valdef, input, &value);

        if (len <= 0) {
                return len;
        }
        switch (css_style_value_get_array_length(&value)) {
        case 1:
                css_style_decl_add(s, css_prop_border_top_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_right_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_color,
                                   value.array_value);
                break;
        case 2:
                css_style_decl_add(s, css_prop_border_top_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_color,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_right_color,
                                   value.array_value + 1);
                break;
        case 3:
                css_style_decl_add(s, css_prop_border_top_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_left_color,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_right_color,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_color,
                                   value.array_value + 2);
                break;
        case 4:
                css_style_decl_add(s, css_prop_border_top_color,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_right_color,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_color,
                                   value.array_value + 2);
                css_style_decl_add(s, css_prop_border_left_color,
                                   value.array_value + 3);
                break;
        default:
                break;
        }
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border_radius(css_propdef_t *propdef, const char *input,
                            css_style_decl_t *s)
{
        css_style_value_t value;
        int len = css_parse_value(propdef->valdef, input, &value);

        if (len <= 0) {
                return len;
        }
        switch (css_style_value_get_array_length(&value)) {
        case 1:
                css_style_decl_add(s, css_prop_border_top_left_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_top_right_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_left_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_right_radius,
                                   value.array_value);
                break;
        case 2:
                css_style_decl_add(s, css_prop_border_top_left_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_bottom_right_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_top_right_radius,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_left_radius,
                                   value.array_value + 1);
                break;
        case 3:
                css_style_decl_add(s, css_prop_border_top_left_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_top_right_radius,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_left_radius,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_right_radius,
                                   value.array_value + 2);
                break;
        case 4:
                css_style_decl_add(s, css_prop_border_top_left_radius,
                                   value.array_value);
                css_style_decl_add(s, css_prop_border_top_right_radius,
                                   value.array_value + 1);
                css_style_decl_add(s, css_prop_border_bottom_right_radius,
                                   value.array_value + 2);
                css_style_decl_add(s, css_prop_border_bottom_left_radius,
                                   value.array_value + 3);
                break;
        default:
                break;
        }
        css_style_value_destroy(&value);
        return len;
}

static void css_init_border_value(css_style_value_t *width,
                                  css_style_value_t *style,
                                  css_style_value_t *color)
{
        width->type = CSS_UNIT_VALUE;
        width->unit_value.value = 0;
        width->unit_value.unit = CSS_UNIT_PX;
        style->type = CSS_KEYWORD_VALUE;
        style->keyword_value = CSS_KEYWORD_NONE;
        color->type = CSS_COLOR_VALUE;
        color->color_value = CSS_COLOR_TRANSPARENT;
}

static void css_add_border_top_value(css_style_decl_t *s,
                                     css_style_value_t *width,
                                     css_style_value_t *style,
                                     css_style_value_t *color)
{
        css_style_decl_add(s, css_prop_border_top_width, width);
        css_style_decl_add(s, css_prop_border_top_style, style);
        css_style_decl_add(s, css_prop_border_top_color, color);
}

static void css_add_border_right_value(css_style_decl_t *s,
                                       css_style_value_t *width,
                                       css_style_value_t *style,
                                       css_style_value_t *color)
{
        css_style_decl_add(s, css_prop_border_right_width, width);
        css_style_decl_add(s, css_prop_border_right_style, style);
        css_style_decl_add(s, css_prop_border_right_color, color);
}

static void css_add_border_bottom_value(css_style_decl_t *s,
                                        css_style_value_t *width,
                                        css_style_value_t *style,
                                        css_style_value_t *color)
{
        css_style_decl_add(s, css_prop_border_bottom_width, width);
        css_style_decl_add(s, css_prop_border_bottom_style, style);
        css_style_decl_add(s, css_prop_border_bottom_color, color);
}

static void css_add_border_left_value(css_style_decl_t *s,
                                      css_style_value_t *width,
                                      css_style_value_t *style,
                                      css_style_value_t *color)
{
        css_style_decl_add(s, css_prop_border_left_width, width);
        css_style_decl_add(s, css_prop_border_left_style, style);
        css_style_decl_add(s, css_prop_border_left_color, color);
}

static void css_add_border_value(css_style_decl_t *s, css_style_value_t *width,
                                 css_style_value_t *style,
                                 css_style_value_t *color)
{
        css_add_border_top_value(s, width, style, color);
        css_add_border_right_value(s, width, style, color);
        css_add_border_bottom_value(s, width, style, color);
        css_add_border_left_value(s, width, style, color);
}

static void css_parse_border_value(css_style_value_t *arr,
                                   css_style_value_t *width,
                                   css_style_value_t *style,
                                   css_style_value_t *color)
{
        int i;
        bool has_width = false;
        bool has_style = false;
        bool has_color = false;

        for (i = 0; i < 3; ++i) {
                if (!has_width && arr[i].type == CSS_UNIT_VALUE) {
                        *width = arr[i];
                        has_width = true;
                } else if (!has_style && arr[i].type == CSS_KEYWORD_VALUE) {
                        *style = arr[i];
                        has_style = true;
                } else if (!has_color && arr[i].type == CSS_COLOR_VALUE) {
                        *color = arr[i];
                        has_color = true;
                } else {
                        break;
                }
        }
}

int css_parse_border_top(css_propdef_t *propdef, const char *input,
                         css_style_decl_t *s)
{
        css_style_value_t value, width, style, color;
        int len = css_parse_value(propdef->valdef, input, &value);
        if (len <= 0) {
                return len;
        }
        css_init_border_value(&width, &style, &color);
        css_parse_border_value(value.array_value, &width, &style, &color);
        css_add_border_top_value(s, &width, &style, &color);
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border_right(css_propdef_t *propdef, const char *input,
                           css_style_decl_t *s)
{
        css_style_value_t value, width, style, color;
        int len = css_parse_value(propdef->valdef, input, &value);
        if (len <= 0) {
                return len;
        }
        css_init_border_value(&width, &style, &color);
        css_parse_border_value(value.array_value, &width, &style, &color);
        css_add_border_right_value(s, &width, &style, &color);
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border_bottom(css_propdef_t *propdef, const char *input,
                            css_style_decl_t *s)
{
        css_style_value_t value, width, style, color;
        int len = css_parse_value(propdef->valdef, input, &value);
        if (len <= 0) {
                return len;
        }
        css_init_border_value(&width, &style, &color);
        css_parse_border_value(value.array_value, &width, &style, &color);
        css_add_border_bottom_value(s, &width, &style, &color);
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border_left(css_propdef_t *propdef, const char *input,
                          css_style_decl_t *s)
{
        css_style_value_t value, width, style, color;
        int len = css_parse_value(propdef->valdef, input, &value);
        if (len <= 0) {
                return len;
        }
        css_init_border_value(&width, &style, &color);
        css_parse_border_value(value.array_value, &width, &style, &color);
        css_add_border_left_value(s, &width, &style, &color);
        css_style_value_destroy(&value);
        return len;
}

int css_parse_border(css_propdef_t *propdef, const char *input,
                     css_style_decl_t *s)
{
        css_style_value_t value, width, style, color;
        int len = css_parse_value(propdef->valdef, input, &value);
        if (len <= 0) {
                return len;
        }
        css_init_border_value(&width, &style, &color);
        css_parse_border_value(value.array_value, &width, &style, &color);
        css_add_border_value(s, &width, &style, &color);
        css_style_value_destroy(&value);
        return len;
}
