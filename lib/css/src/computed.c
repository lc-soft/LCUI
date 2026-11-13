/*
 * lib/css/src/computed.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <css/utils.h>
#include <css/computed.h>
#include <css/properties.h>
#include <css/style_value.h>
#include <css/style_decl.h>
#include "properties.h"

static uint8_t get_padding_top(const css_computed_style_t *s,
                               css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->padding_top;
        *unit = s->unit_bits.padding_top;
        return s->type_bits.padding_top;
}

static void set_padding_top(css_computed_style_t *s, css_numeric_value_t value,
                            css_unit_t unit)
{
        s->padding_top = value;
        s->unit_bits.padding_top = unit;
}

static uint8_t get_padding_right(const css_computed_style_t *s,
                                 css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->padding_right;
        *unit = s->unit_bits.padding_right;
        return s->type_bits.padding_right;
}

static void set_padding_right(css_computed_style_t *s,
                              css_numeric_value_t value, css_unit_t unit)
{
        s->padding_right = value;
        s->unit_bits.padding_right = unit;
}

static uint8_t get_padding_bottom(const css_computed_style_t *s,
                                  css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->padding_bottom;
        *unit = s->unit_bits.padding_bottom;
        return s->type_bits.padding_bottom;
}

static void set_padding_bottom(css_computed_style_t *s,
                               css_numeric_value_t value, css_unit_t unit)
{
        s->padding_bottom = value;
        s->unit_bits.padding_bottom = unit;
}

static uint8_t get_padding_left(const css_computed_style_t *s,
                                css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->padding_left;
        *unit = s->unit_bits.padding_left;
        return s->type_bits.padding_left;
}

static void set_padding_left(css_computed_style_t *s, css_numeric_value_t value,
                             css_unit_t unit)
{
        s->padding_left = value;
        s->unit_bits.padding_left = unit;
}

static uint8_t get_margin_top(const css_computed_style_t *s,
                              css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->margin_top;
        *unit = s->unit_bits.margin_top;
        return s->type_bits.margin_top;
}

static void set_margin_top(css_computed_style_t *s, css_numeric_value_t value,
                           css_unit_t unit)
{
        s->margin_top = value;
        s->unit_bits.margin_top = unit;
}

static uint8_t get_margin_right(const css_computed_style_t *s,
                                css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->margin_right;
        *unit = s->unit_bits.margin_right;
        return s->type_bits.margin_right;
}

static void set_margin_right(css_computed_style_t *s, css_numeric_value_t value,
                             css_unit_t unit)
{
        s->margin_right = value;
        s->unit_bits.margin_right = unit;
}

static uint8_t get_margin_bottom(const css_computed_style_t *s,
                                 css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->margin_bottom;
        *unit = s->unit_bits.margin_bottom;
        return s->type_bits.margin_bottom;
}

static void set_margin_bottom(css_computed_style_t *s,
                              css_numeric_value_t value, css_unit_t unit)
{
        s->margin_bottom = value;
        s->unit_bits.margin_bottom = unit;
}

static uint8_t get_margin_left(const css_computed_style_t *s,
                               css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->margin_left;
        *unit = s->unit_bits.margin_left;
        return s->type_bits.margin_left;
}

static void set_margin_left(css_computed_style_t *s, css_numeric_value_t value,
                            css_unit_t unit)
{
        s->margin_left = value;
        s->unit_bits.margin_left = unit;
}

static uint8_t get_top(const css_computed_style_t *s,
                       css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->top;
        *unit = s->unit_bits.top;
        return s->type_bits.top;
}

static void set_top(css_computed_style_t *s, css_numeric_value_t value,
                    css_unit_t unit)
{
        s->top = value;
        s->unit_bits.top = unit;
}

static uint8_t get_right(const css_computed_style_t *s,
                         css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->right;
        *unit = s->unit_bits.right;
        return s->type_bits.right;
}

static void set_right(css_computed_style_t *s, css_numeric_value_t value,
                      css_unit_t unit)
{
        s->right = value;
        s->unit_bits.right = unit;
}

static uint8_t get_bottom(const css_computed_style_t *s,
                          css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->bottom;
        *unit = s->unit_bits.bottom;
        return s->type_bits.bottom;
}

static void set_bottom(css_computed_style_t *s, css_numeric_value_t value,
                       css_unit_t unit)
{
        s->bottom = value;
        s->unit_bits.bottom = unit;
}

static uint8_t get_left(const css_computed_style_t *s,
                        css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->left;
        *unit = s->unit_bits.left;
        return s->type_bits.left;
}

static void set_left(css_computed_style_t *s, css_numeric_value_t value,
                     css_unit_t unit)
{
        s->left = value;
        s->unit_bits.left = unit;
}

static uint8_t get_min_width(const css_computed_style_t *s,
                             css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->min_width;
        *unit = s->unit_bits.min_width;
        return s->type_bits.min_width;
}

static uint8_t get_max_width(const css_computed_style_t *s,
                             css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->max_width;
        *unit = s->unit_bits.max_width;
        return s->type_bits.max_width;
}

static void set_min_width(css_computed_style_t *s, css_numeric_value_t value,
                          css_unit_t unit)
{
        s->min_width = value;
        s->unit_bits.min_width = unit;
}

static uint8_t get_min_height(const css_computed_style_t *s,
                              css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->min_height;
        *unit = s->unit_bits.min_height;
        return s->type_bits.min_height;
}

static uint8_t get_max_height(const css_computed_style_t *s,
                              css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->max_height;
        *unit = s->unit_bits.max_height;
        return s->type_bits.max_height;
}

static void set_min_height(css_computed_style_t *s, css_numeric_value_t value,
                           css_unit_t unit)
{
        s->min_height = value;
        s->unit_bits.min_height = unit;
}

static void set_max_width(css_computed_style_t *s, css_numeric_value_t value,
                          css_unit_t unit)
{
        s->max_width = value;
        s->unit_bits.max_width = unit;
}

static void set_max_height(css_computed_style_t *s, css_numeric_value_t value,
                           css_unit_t unit)
{
        s->max_height = value;
        s->unit_bits.max_height = unit;
}

static uint8_t get_width(const css_computed_style_t *s,
                         css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->width;
        *unit = s->unit_bits.width;
        return s->type_bits.width;
}

static void set_width(css_computed_style_t *s, css_numeric_value_t value,
                      css_unit_t unit)
{
        s->width = value;
        s->unit_bits.width = unit;
}

static uint8_t get_height(const css_computed_style_t *s,
                          css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->height;
        *unit = s->unit_bits.height;
        return s->type_bits.height;
}

static void set_height(css_computed_style_t *s, css_numeric_value_t value,
                       css_unit_t unit)
{
        s->height = value;
        s->unit_bits.height = unit;
}

static uint8_t get_flex_basis(const css_computed_style_t *s,
                              css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->flex_basis;
        *unit = s->unit_bits.flex_basis;
        return s->type_bits.flex_basis;
}

static void set_flex_basis(css_computed_style_t *s, css_numeric_value_t value,
                           css_unit_t unit)
{
        s->flex_basis = value;
        s->unit_bits.flex_basis = unit;
}

static uint8_t get_text_underline_offset(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit)
{
        *value = s->text_underline_offset;
        *unit = s->unit_bits.text_underline_offset;
        return s->type_bits.text_underline_offset;
}

static void set_text_underline_offset(css_computed_style_t *s,
                                      css_numeric_value_t value,
                                      css_unit_t unit)
{
        s->text_underline_offset = value;
        s->unit_bits.text_underline_offset = unit;
}

static uint8_t get_text_decoration_thickness(const css_computed_style_t *s,
                                             css_numeric_value_t *value,
                                             css_unit_t *unit)
{
        *value = s->text_decoration_thickness;
        *unit = s->unit_bits.text_decoration_thickness;
        return s->type_bits.text_decoration_thickness;
}

static void set_text_decoration_thickness(css_computed_style_t *s,
                                          css_numeric_value_t value,
                                          css_unit_t unit)
{
        s->text_decoration_thickness = value;
        s->unit_bits.text_decoration_thickness = unit;
}

uint8_t css_computed_padding_top(const css_computed_style_t *s,
                                 css_numeric_value_t *value, css_unit_t *unit)
{
        return get_padding_top(s, value, unit);
}

uint8_t css_computed_padding_right(const css_computed_style_t *s,
                                   css_numeric_value_t *value, css_unit_t *unit)
{
        return get_padding_right(s, value, unit);
}

uint8_t css_computed_padding_bottom(const css_computed_style_t *s,
                                    css_numeric_value_t *value,
                                    css_unit_t *unit)
{
        return get_padding_bottom(s, value, unit);
}

uint8_t css_computed_padding_left(const css_computed_style_t *s,
                                  css_numeric_value_t *value, css_unit_t *unit)
{
        return get_padding_left(s, value, unit);
}

uint8_t css_computed_margin_top(const css_computed_style_t *s,
                                css_numeric_value_t *value, css_unit_t *unit)
{
        return get_margin_top(s, value, unit);
}

uint8_t css_computed_margin_right(const css_computed_style_t *s,
                                  css_numeric_value_t *value, css_unit_t *unit)
{
        return get_margin_right(s, value, unit);
}

uint8_t css_computed_margin_bottom(const css_computed_style_t *s,
                                   css_numeric_value_t *value, css_unit_t *unit)
{
        return get_margin_bottom(s, value, unit);
}

uint8_t css_computed_margin_left(const css_computed_style_t *s,
                                 css_numeric_value_t *value, css_unit_t *unit)
{
        return get_margin_left(s, value, unit);
}

uint8_t css_computed_top(const css_computed_style_t *s,
                         css_numeric_value_t *value, css_unit_t *unit)
{
        uint8_t type = get_top(s, value, unit);

        if (s->type_bits.position == CSS_POSITION_STATIC) {
                return CSS_TOP_AUTO;
        }
        return type;
}

uint8_t css_computed_right(const css_computed_style_t *s,
                           css_numeric_value_t *value, css_unit_t *unit)
{
        uint8_t type = get_right(s, value, unit);

        if (s->type_bits.position == CSS_POSITION_STATIC) {
                return CSS_RIGHT_AUTO;
        }
        return type;
}

uint8_t css_computed_bottom(const css_computed_style_t *s,
                            css_numeric_value_t *value, css_unit_t *unit)
{
        uint8_t type = get_bottom(s, value, unit);

        if (s->type_bits.position == CSS_POSITION_STATIC) {
                return CSS_BOTTOM_AUTO;
        }
        return type;
}

uint8_t css_computed_left(const css_computed_style_t *s,
                          css_numeric_value_t *value, css_unit_t *unit)
{
        uint8_t type = get_left(s, value, unit);

        if (s->type_bits.position == CSS_POSITION_STATIC) {
                return CSS_LEFT_AUTO;
        }
        return type;
}

uint8_t css_computed_display(const css_computed_style_t *s)
{
        return s->type_bits.display;
}

uint8_t css_computed_flex_direction(const css_computed_style_t *s)
{
        return s->type_bits.flex_direction;
}

uint8_t css_computed_vertical_align(const css_computed_style_t *s)
{
        return s->type_bits.vertical_align;
}

uint8_t css_computed_box_sizing(const css_computed_style_t *s)
{
        return s->type_bits.box_sizing;
}

uint8_t css_computed_position(const css_computed_style_t *s)
{
        return s->type_bits.position;
}

uint8_t css_computed_min_width(const css_computed_style_t *s,
                               css_numeric_value_t *value, css_unit_t *unit)
{
        if (s->type_bits.min_width == CSS_MIN_WIDTH_AUTO) {
                if (s->type_bits.display == CSS_DISPLAY_FLEX ||
                    s->type_bits.display == CSS_DISPLAY_INLINE_FLEX) {
                        *value = s->min_width;
                        *unit = s->unit_bits.min_width;
                        return s->type_bits.min_width;
                }
                *value = 0;
                *unit = CSS_UNIT_PX;
        } else {
                *value = s->min_width;
                *unit = s->unit_bits.min_width;
        }
        return s->type_bits.min_width;
}

uint8_t css_computed_min_height(const css_computed_style_t *s,
                                css_numeric_value_t *value, css_unit_t *unit)
{
        if (s->type_bits.min_height == CSS_MIN_HEIGHT_AUTO) {
                if (s->type_bits.display == CSS_DISPLAY_FLEX ||
                    s->type_bits.display == CSS_DISPLAY_INLINE_FLEX) {
                        *value = s->min_height;
                        *unit = s->unit_bits.min_height;
                        return s->type_bits.min_height;
                }
                *value = 0;
                *unit = CSS_UNIT_PX;
        } else {
                *value = s->min_height;
                *unit = s->unit_bits.min_height;
        }
        return s->type_bits.min_height;
}

uint8_t css_computed_max_width(const css_computed_style_t *s,
                               css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->max_width;
        *unit = s->unit_bits.max_width;
        return s->type_bits.max_width;
}

uint8_t css_computed_max_height(const css_computed_style_t *s,
                                css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->max_height;
        *unit = s->unit_bits.max_height;
        return s->type_bits.max_height;
}

uint8_t css_computed_width(const css_computed_style_t *s,
                           css_numeric_value_t *value, css_unit_t *unit)
{
        return get_width(s, value, unit);
}

uint8_t css_computed_height(const css_computed_style_t *s,
                            css_numeric_value_t *value, css_unit_t *unit)
{
        return get_height(s, value, unit);
}

uint8_t css_computed_box_shadow_x(const css_computed_style_t *s,
                                  css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->box_shadow_x;
        *unit = s->unit_bits.box_shadow_x;
        return CSS_LENGTH_SET;
}

static void set_box_shadow_x(css_computed_style_t *s, css_numeric_value_t value,
                             css_unit_t unit)
{
        s->box_shadow_x = value;
        s->unit_bits.box_shadow_x = unit;
}

uint8_t css_computed_box_shadow_y(const css_computed_style_t *s,
                                  css_numeric_value_t *value, css_unit_t *unit)
{
        *value = s->box_shadow_y;
        *unit = s->unit_bits.box_shadow_y;
        return CSS_LENGTH_SET;
}

static void set_box_shadow_y(css_computed_style_t *s, css_numeric_value_t value,
                             css_unit_t unit)
{
        s->box_shadow_y = value;
        s->unit_bits.box_shadow_y = unit;
}

uint8_t css_computed_box_shadow_blur(const css_computed_style_t *s,
                                     css_numeric_value_t *value,
                                     css_unit_t *unit)
{
        *value = s->box_shadow_blur;
        *unit = s->unit_bits.box_shadow_blur;
        return CSS_LENGTH_SET;
}

static void set_box_shadow_blur(css_computed_style_t *s,
                                css_numeric_value_t value, css_unit_t unit)
{
        s->box_shadow_blur = value;
        s->unit_bits.box_shadow_blur = unit;
}

uint8_t css_computed_box_shadow_spread(const css_computed_style_t *s,
                                       css_numeric_value_t *value,
                                       css_unit_t *unit)
{
        *value = s->box_shadow_spread;
        *unit = s->unit_bits.box_shadow_spread;
        return CSS_LENGTH_SET;
}

static void set_box_shadow_spread(css_computed_style_t *s,
                                  css_numeric_value_t value, css_unit_t unit)
{
        s->box_shadow_spread = value;
        s->unit_bits.box_shadow_spread = unit;
}

uint8_t css_computed_box_shadow_color(const css_computed_style_t *s,
                                      css_color_value_t *value)
{
        *value = s->box_shadow_color;
        return s->type_bits.box_shadow;
}

uint8_t css_computed_background_position_x(const css_computed_style_t *s,
                                           css_numeric_value_t *value,
                                           css_unit_t *unit)
{
        *value = s->background_position_x;
        *unit = s->unit_bits.background_position_x;
        return CSS_LENGTH_SET;
}

static void set_background_position_x(css_computed_style_t *s,
                                      css_numeric_value_t value,
                                      css_unit_t unit)
{
        s->background_position_x = value;
        s->unit_bits.background_position_x = unit;
}

uint8_t css_computed_background_position_y(const css_computed_style_t *s,
                                           css_numeric_value_t *value,
                                           css_unit_t *unit)
{
        *value = s->background_position_y;
        *unit = s->unit_bits.background_position_y;
        return CSS_LENGTH_SET;
}

static void set_background_position_y(css_computed_style_t *s,
                                      css_numeric_value_t value,
                                      css_unit_t unit)
{
        s->background_position_y = value;
        s->unit_bits.background_position_y = unit;
}

uint8_t css_computed_background_width(const css_computed_style_t *s,
                                      css_numeric_value_t *value,
                                      css_unit_t *unit)
{
        *value = s->background_width;
        *unit = s->unit_bits.background_width;
        return CSS_LENGTH_SET;
}

static void set_background_width(css_computed_style_t *s,
                                 css_numeric_value_t value, css_unit_t unit)
{
        s->background_width = value;
        s->unit_bits.background_width = unit;
}

uint8_t css_computed_background_height(const css_computed_style_t *s,
                                       css_numeric_value_t *value,
                                       css_unit_t *unit)
{
        *value = s->background_height;
        *unit = s->unit_bits.background_height;
        return CSS_LENGTH_SET;
}

static void set_background_height(css_computed_style_t *s,
                                  css_numeric_value_t value, css_unit_t unit)
{
        s->background_height = value;
        s->unit_bits.background_height = unit;
}

uint8_t css_computed_background_image(const css_computed_style_t *s,
                                      css_image_value_t *value)
{
        *value = s->background_image;
        if (s->background_image) {
                return CSS_BACKGROUND_IMAGE_IMAGE;
        }
        return CSS_BACKGROUND_IMAGE_NONE;
}

uint8_t css_computed_background_color(const css_computed_style_t *s,
                                      css_color_value_t *value)
{
        *value = s->background_color;
        return CSS_BACKGROUND_COLOR_COLOR;
}

uint8_t css_computed_border_top_width(const css_computed_style_t *s,
                                      css_numeric_value_t *value,
                                      css_unit_t *unit)
{
        *value = s->border_top_width;
        *unit = s->unit_bits.border_top_width;
        return CSS_BORDER_WIDTH_WIDTH;
}

static void set_border_top_width(css_computed_style_t *s,
                                 css_numeric_value_t value, css_unit_t unit)
{
        s->border_top_width = value;
        s->unit_bits.border_top_width = unit;
}

uint8_t css_computed_border_right_width(const css_computed_style_t *s,
                                        css_numeric_value_t *value,
                                        css_unit_t *unit)
{
        *value = s->border_right_width;
        *unit = s->unit_bits.border_right_width;
        return CSS_BORDER_WIDTH_WIDTH;
}

static void set_border_right_width(css_computed_style_t *s,
                                   css_numeric_value_t value, css_unit_t unit)
{
        s->border_right_width = value;
        s->unit_bits.border_right_width = unit;
}

uint8_t css_computed_border_bottom_width(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit)
{
        *value = s->border_bottom_width;
        *unit = s->unit_bits.border_bottom_width;
        return CSS_BORDER_WIDTH_WIDTH;
}

static void set_border_bottom_width(css_computed_style_t *s,
                                    css_numeric_value_t value, css_unit_t unit)
{
        s->border_bottom_width = value;
        s->unit_bits.border_bottom_width = unit;
}

uint8_t css_computed_border_left_width(const css_computed_style_t *s,
                                       css_numeric_value_t *value,
                                       css_unit_t *unit)
{
        *value = s->border_left_width;
        *unit = s->unit_bits.border_left_width;
        return CSS_BORDER_WIDTH_WIDTH;
}

static void set_border_left_width(css_computed_style_t *s,
                                  css_numeric_value_t value, css_unit_t unit)
{
        s->border_left_width = value;
        s->unit_bits.border_left_width = unit;
}

uint8_t css_computed_border_top_left_radius(const css_computed_style_t *s,
                                            css_numeric_value_t *value,
                                            css_unit_t *unit)
{
        *value = s->border_top_left_radius;
        *unit = s->unit_bits.border_top_left_radius;
        return CSS_BORDER_RADIUS_SET;
}

static void set_border_top_left_radius(css_computed_style_t *s,
                                       css_numeric_value_t value,
                                       css_unit_t unit)
{
        s->border_top_left_radius = value;
        s->unit_bits.border_top_left_radius = unit;
}

uint8_t css_computed_border_top_right_radius(const css_computed_style_t *s,
                                             css_numeric_value_t *value,
                                             css_unit_t *unit)
{
        *value = s->border_top_right_radius;
        *unit = s->unit_bits.border_top_right_radius;
        return CSS_BORDER_RADIUS_SET;
}

static void set_border_top_right_radius(css_computed_style_t *s,
                                        css_numeric_value_t value,
                                        css_unit_t unit)
{
        s->border_top_right_radius = value;
        s->unit_bits.border_top_right_radius = unit;
}

uint8_t css_computed_border_bottom_left_radius(const css_computed_style_t *s,
                                               css_numeric_value_t *value,
                                               css_unit_t *unit)
{
        *value = s->border_bottom_left_radius;
        *unit = s->unit_bits.border_bottom_left_radius;
        return CSS_BORDER_RADIUS_SET;
}

static void set_border_bottom_left_radius(css_computed_style_t *s,
                                          css_numeric_value_t value,
                                          css_unit_t unit)
{
        s->border_bottom_left_radius = value;
        s->unit_bits.border_bottom_left_radius = unit;
}

uint8_t css_computed_border_bottom_right_radius(const css_computed_style_t *s,
                                                css_numeric_value_t *value,
                                                css_unit_t *unit)
{
        *value = s->border_bottom_right_radius;
        *unit = s->unit_bits.border_bottom_right_radius;
        return CSS_BORDER_RADIUS_SET;
}

static void set_border_bottom_right_radius(css_computed_style_t *s,
                                           css_numeric_value_t value,
                                           css_unit_t unit)
{
        s->border_bottom_right_radius = value;
        s->unit_bits.border_bottom_right_radius = unit;
}

uint8_t css_computed_border_top_style(const css_computed_style_t *s)
{
        return s->type_bits.border_top_style;
}

uint8_t css_computed_border_right_style(const css_computed_style_t *s)
{
        return s->type_bits.border_right_style;
}

uint8_t css_computed_border_bottom_style(const css_computed_style_t *s)
{
        return s->type_bits.border_bottom_style;
}

uint8_t css_computed_border_left_style(const css_computed_style_t *s)
{
        return s->type_bits.border_left_style;
}

uint8_t css_computed_border_top_color(const css_computed_style_t *s,
                                      css_color_value_t *value)
{
        *value = s->border_top_color;
        return CSS_BORDER_COLOR_COLOR;
}

uint8_t css_computed_border_right_color(const css_computed_style_t *s,
                                        css_color_value_t *value)
{
        *value = s->border_right_color;
        return CSS_BORDER_COLOR_COLOR;
}

uint8_t css_computed_border_bottom_color(const css_computed_style_t *s,
                                         css_color_value_t *value)
{
        *value = s->border_bottom_color;
        return CSS_BORDER_COLOR_COLOR;
}

uint8_t css_computed_border_left_color(const css_computed_style_t *s,
                                       css_color_value_t *value)
{
        *value = s->border_left_color;
        return CSS_BORDER_COLOR_COLOR;
}

uint8_t css_computed_visibility(const css_computed_style_t *s)
{
        return s->type_bits.visibility;
}

static bool css_computed_value_is_inherit(const css_style_value_t *value)
{
        return value->type == CSS_ARRAY_VALUE &&
               css_style_value_get_array_length(value) == 1 &&
               value->array_value[0].type == CSS_KEYWORD_VALUE &&
               value->array_value[0].keyword_value == CSS_KEYWORD_INHERIT;
}

int css_cascade_style_with_parent(const css_style_decl_t *style,
                                  const css_computed_style_t *parent,
                                  css_computed_style_t *computed)
{
        unsigned i;
        list_node_t *node;
        css_prop_t *prop;
        const css_propdef_t *propdef;
        unsigned count = css_get_prop_count();
        bool *setted_props = calloc(count, sizeof(bool));

        for (list_each(node, style)) {
                prop = node->data;
                if (setted_props[prop->key] ||
                    prop->value.type <= CSS_INVALID_VALUE) {
                        continue;
                }
                setted_props[prop->key] = true;
                propdef = css_get_propdef(prop->key);
                assert(propdef && propdef->cascade);
                if (css_computed_value_is_inherit(&prop->value)) {
                        if (parent) {
                                css_computed_style_copy_property(
                                    computed, parent, prop->key);
                        } else {
                                propdef->cascade(
                                    propdef->initial_value.array_value,
                                    computed);
                        }
                        continue;
                }
                propdef->cascade(prop->value.array_value, computed);
                if (prop->key == css_prop_font_size) {
                        css_compute_font_size_value(parent, computed);
                }
        }
        for (i = 0; i < count; ++i) {
                if (setted_props[i]) {
                        continue;
                }
                propdef = css_get_propdef((int)i);
                if (parent && propdef->inherited) {
                        css_computed_style_copy_property(computed, parent,
                                                         (css_prop_key_t)i);
                } else {
                        propdef->cascade(propdef->initial_value.array_value,
                                         computed);
                }
        }
        free(setted_props);
        return 0;
}

int css_cascade_style(const css_style_decl_t *style,
                      css_computed_style_t *computed)
{
        return css_cascade_style_with_parent(style, NULL, computed);
}

static int compute_absolute_length(
    css_computed_style_t *s, css_metrics_t *m,
    uint8_t (*getter)(const css_computed_style_t *, css_numeric_value_t *,
                      css_unit_t *),
    void (*setter)(css_computed_style_t *, css_numeric_value_t, css_unit_t))
{
        css_unit_t unit;
        css_numeric_value_t value;

        if (getter(s, &value, &unit) != CSS_LENGTH_SET) {
                return 0;
        }
        switch (unit) {
        case CSS_UNIT_PX:
                break;
        case CSS_UNIT_DIP:
                value = value * css_metrics_actual_scale(m) * m->density;
                break;
        case CSS_UNIT_SP:
                value = value * css_metrics_actual_scale(m) * m->scaled_density;
                break;
        case CSS_UNIT_PT:
                value = value * m->dpi / 72.0f;
                break;
        default:
                return -1;
        }
        setter(s, value, CSS_UNIT_PX);
        return 0;
}

static int compute_absolute_css_length(
    css_computed_style_t *s, css_metrics_t *m,
    uint8_t (*getter)(const css_computed_style_t *, css_numeric_value_t *,
                      css_unit_t *),
    void (*setter)(css_computed_style_t *, css_numeric_value_t, css_unit_t))
{
        css_unit_t unit;
        css_numeric_value_t value;

        if (getter(s, &value, &unit) != CSS_LENGTH_SET) {
                return 0;
        }
        switch (unit) {
        case CSS_UNIT_PX:
                break;
        case CSS_UNIT_DIP:
                value *= m->density;
                break;
        case CSS_UNIT_SP:
                value *= m->scaled_density;
                break;
        case CSS_UNIT_PT:
                value *= 96.f / 72.f;
                break;
        default:
                return -1;
        }
        setter(s, value, CSS_UNIT_PX);
        return 0;
}

static uint8_t compute_content_box_width(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit)
{
        uint8_t type = css_computed_width(s, value, unit);

        if (type != CSS_WIDTH_SET && *unit == CSS_UNIT_PERCENT) {
                return type;
        }
        *value = s->width;
        if (s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                *value -= css_border_x(s) + css_padding_x(s);
        }
        return type;
}

static bool compute_content_box_fixed_width(const css_computed_style_t *s,
                                            css_numeric_value_t *value)
{
        css_unit_t unit;
        return compute_content_box_width(s, value, &unit) == CSS_WIDTH_SET &&
               unit == CSS_UNIT_PX;
}

static uint8_t compute_content_box_height(const css_computed_style_t *s,
                                          css_numeric_value_t *value,
                                          css_unit_t *unit)
{
        uint8_t type = css_computed_height(s, value, unit);

        if (type != CSS_HEIGHT_SET && *unit == CSS_UNIT_PERCENT) {
                return type;
        }
        *value = s->height;
        if (s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                *value -= css_border_y(s) + css_padding_y(s);
        }
        return type;
}

static bool compute_content_box_fixed_height(const css_computed_style_t *s,
                                             css_numeric_value_t *value)
{
        css_unit_t unit;
        return compute_content_box_height(s, value, &unit) == CSS_HEIGHT_SET &&
               unit == CSS_UNIT_PX;
}

static uint8_t compute_padding_box_width(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit)
{
        uint8_t type = css_computed_width(s, value, unit);

        if (type == CSS_WIDTH_SET && *unit != CSS_UNIT_PERCENT) {
                if (s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                        *value -= css_border_x(s);
                } else {
                        *value += css_padding_x(s);
                }
        }
        return type;
}

static uint8_t compute_padding_box_height(const css_computed_style_t *s,
                                          css_numeric_value_t *value,
                                          css_unit_t *unit)
{
        uint8_t type = css_computed_height(s, value, unit);

        if (type == CSS_WIDTH_SET && *unit != CSS_UNIT_PERCENT) {
                if (s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                        *value -= css_border_y(s);
                        ;
                } else {
                        *value += css_padding_y(s);
                }
        }
        return type;
}

static bool compute_padding_box_fixed_width(const css_computed_style_t *s,
                                            css_numeric_value_t *value)
{
        css_unit_t unit;
        return compute_padding_box_width(s, value, &unit) == CSS_WIDTH_SET &&
               unit == CSS_UNIT_PX;
}

static bool compute_padding_box_fixed_height(const css_computed_style_t *s,
                                             css_numeric_value_t *value)
{
        css_unit_t unit;
        return compute_padding_box_height(s, value, &unit) == CSS_HEIGHT_SET &&
               unit == CSS_UNIT_PX;
}

static void compute_absolute_length_x(
    const css_computed_style_t *parent, css_computed_style_t *s,
    css_metrics_t *m,
    uint8_t (*getter)(const css_computed_style_t *, css_numeric_value_t *,
                      css_unit_t *),
    void (*setter)(css_computed_style_t *, css_numeric_value_t, css_unit_t))
{
        css_unit_t unit;
        css_numeric_value_t value;

        compute_absolute_length(s, m, getter, setter);
        switch (getter(s, &value, &unit)) {
        case CSS_LENGTH_SET:
                if (unit != CSS_UNIT_PERCENT || !parent ||
                    !IS_CSS_FIXED_LENGTH(parent, width)) {
                        break;
                }
                switch (css_computed_position(s)) {
                case CSS_POSITION_ABSOLUTE:
                case CSS_POSITION_FIXED:
                        if (parent->type_bits.box_sizing ==
                            CSS_BOX_SIZING_CONTENT_BOX) {
                                value *= parent->width + css_border_x(parent) +
                                         css_padding_x(parent);
                                break;
                        }
                default:
                        value *= parent->width;
                        break;
                }
                setter(s, value / 100.f, CSS_UNIT_PX);
                break;
        default:
                break;
        }
}

static void compute_absolute_length_y(
    const css_computed_style_t *parent, css_computed_style_t *s,
    css_metrics_t *m,
    uint8_t (*getter)(const css_computed_style_t *, css_numeric_value_t *,
                      css_unit_t *),
    void (*setter)(css_computed_style_t *, css_numeric_value_t, css_unit_t))
{
        css_unit_t unit;
        css_numeric_value_t value;

        compute_absolute_length(s, m, getter, setter);
        switch (getter(s, &value, &unit)) {
        case CSS_LENGTH_SET:
                if (unit != CSS_UNIT_PERCENT || !parent ||
                    !IS_CSS_FIXED_LENGTH(parent, height)) {
                        break;
                }
                switch (css_computed_position(s)) {
                case CSS_POSITION_ABSOLUTE:
                case CSS_POSITION_FIXED:
                        if (parent->type_bits.box_sizing ==
                            CSS_BOX_SIZING_CONTENT_BOX) {
                                value *= parent->height + css_border_y(parent) +
                                         css_padding_y(parent);
                                break;
                        }
                default:
                        value *= parent->height;
                        break;
                }
                setter(s, value / 100.f, CSS_UNIT_PX);
                break;
        default:
                break;
        }
}

static void compute_absolute_width(const css_computed_style_t *parent,
                                   css_computed_style_t *s, css_metrics_t *m)
{
        css_unit_t unit;
        css_numeric_value_t value;
        css_numeric_value_t parent_value;

        compute_absolute_length(s, m, get_width, set_width);
        switch (get_width(s, &value, &unit)) {
        case CSS_WIDTH_AUTO:
                if (css_computed_position(s) > CSS_POSITION_RELATIVE) {
                        if (IS_CSS_FIXED_LENGTH(s, left) &&
                            IS_CSS_FIXED_LENGTH(s, right) &&
                            compute_padding_box_fixed_width(parent,
                                                            &parent_value)) {
                                CSS_SET_FIXED_LENGTH(
                                    s, width,
                                    parent_value - s->left - s->right);
                                break;
                        }
                        s->type_bits.width = CSS_WIDTH_FIT_CONTENT;
                        break;
                }
                if (is_css_display_inline(s) || !parent) {
                        s->type_bits.width = CSS_WIDTH_FIT_CONTENT;
                        break;
                }
                break;
        case CSS_WIDTH_SET:
                if (unit != CSS_UNIT_PERCENT) {
                        break;
                }
                if (!parent) {
                        s->type_bits.width = CSS_WIDTH_FIT_CONTENT;
                        break;
                }
                if (!compute_content_box_fixed_width(parent, &parent_value)) {
                        /* A percentage width against an indefinite containing
                         * block behaves like auto for in-flow block boxes:
                         * the parent layout resolves it.  Inline and
                         * out-of-flow boxes shrink to fit instead.  Leaving
                         * the raw percentage number as pixels would size the
                         * box to e.g. 100px and make the subtree flicker. */
                        if (is_css_display_inline(s) ||
                            css_computed_position(s) > CSS_POSITION_RELATIVE) {
                                s->type_bits.width = CSS_WIDTH_FIT_CONTENT;
                        } else {
                                s->type_bits.width = CSS_WIDTH_AUTO;
                        }
                        break;
                }
                if (css_computed_position(s) > CSS_POSITION_RELATIVE) {
                        parent_value += css_padding_x(parent);
                }
                CSS_SET_FIXED_LENGTH(s, width, parent_value * value / 100.0f);
                break;
        default:
                break;
        }
}

static void compute_absolute_height(const css_computed_style_t *parent,
                                    css_computed_style_t *s, css_metrics_t *m)
{
        css_unit_t unit;
        css_numeric_value_t value;
        css_numeric_value_t parent_value;

        compute_absolute_length(s, m, get_height, set_height);
        switch (get_height(s, &value, &unit)) {
        case CSS_HEIGHT_AUTO:
                if (css_computed_position(s) > CSS_POSITION_RELATIVE) {
                        if (IS_CSS_FIXED_LENGTH(s, top) &&
                            IS_CSS_FIXED_LENGTH(s, bottom) &&
                            compute_padding_box_fixed_height(parent,
                                                             &parent_value)) {
                                CSS_SET_FIXED_LENGTH(
                                    s, height,
                                    parent_value - s->top - s->bottom);
                                break;
                        }
                        s->type_bits.height = CSS_HEIGHT_FIT_CONTENT;
                        break;
                }
                if (is_css_display_inline(s) || !parent) {
                        s->type_bits.height = CSS_HEIGHT_FIT_CONTENT;
                }
                break;
        case CSS_HEIGHT_SET:
                if (unit != CSS_UNIT_PERCENT) {
                        break;
                }
                if (!parent ||
                    !compute_content_box_fixed_height(parent, &parent_value)) {
                        s->type_bits.height = CSS_HEIGHT_FIT_CONTENT;
                        break;
                }
                if (css_computed_position(s) > CSS_POSITION_RELATIVE) {
                        parent_value += css_padding_y(parent);
                }
                CSS_SET_FIXED_LENGTH(s, height, parent_value * value / 100.0f);
                break;
        default:
                break;
        }
}

static void compute_absolute_flex_basis(const css_computed_style_t *parent,
                                        css_computed_style_t *s,
                                        css_metrics_t *m)
{
        compute_absolute_length(s, m, get_flex_basis, set_flex_basis);
}

void css_compute_absolute_values(const css_computed_style_t *parent,
                                 css_computed_style_t *s, css_metrics_t *m)
{
        compute_absolute_length_x(parent, s, m, get_left, set_left);
        compute_absolute_length_x(parent, s, m, get_right, set_right);
        compute_absolute_length_y(parent, s, m, get_top, set_top);
        compute_absolute_length_y(parent, s, m, get_bottom, set_bottom);

        compute_absolute_length_x(parent, s, m, get_margin_left,
                                  set_margin_left);
        compute_absolute_length_x(parent, s, m, get_margin_right,
                                  set_margin_right);
        compute_absolute_length_y(parent, s, m, get_margin_top, set_margin_top);
        compute_absolute_length_y(parent, s, m, get_margin_bottom,
                                  set_margin_bottom);

        compute_absolute_length_x(parent, s, m, get_padding_left,
                                  set_padding_left);
        compute_absolute_length_x(parent, s, m, get_padding_right,
                                  set_padding_right);
        compute_absolute_length_y(parent, s, m, get_padding_top,
                                  set_padding_top);
        compute_absolute_length_y(parent, s, m, get_padding_bottom,
                                  set_padding_bottom);

        compute_absolute_length_x(parent, s, m, get_min_width, set_min_width);
        compute_absolute_length_x(parent, s, m, get_max_width, set_max_width);
        compute_absolute_length_y(parent, s, m, get_min_height, set_min_height);
        compute_absolute_length_y(parent, s, m, get_max_height, set_max_height);

        compute_absolute_width(parent, s, m);
        compute_absolute_height(parent, s, m);

        compute_absolute_flex_basis(parent, s, m);

        compute_absolute_length(s, m, css_computed_background_position_x,
                                set_background_position_x);
        compute_absolute_length(s, m, css_computed_background_position_y,
                                set_background_position_y);
        compute_absolute_length(s, m, css_computed_background_width,
                                set_background_width);
        compute_absolute_length(s, m, css_computed_background_height,
                                set_background_height);

        compute_absolute_length(s, m, css_computed_border_top_width,
                                set_border_top_width);
        compute_absolute_length(s, m, css_computed_border_right_width,
                                set_border_right_width);
        compute_absolute_length(s, m, css_computed_border_bottom_width,
                                set_border_bottom_width);
        compute_absolute_length(s, m, css_computed_border_left_width,
                                set_border_left_width);
        compute_absolute_length(s, m, css_computed_border_top_left_radius,
                                set_border_top_left_radius);
        compute_absolute_length(s, m, css_computed_border_top_right_radius,
                                set_border_top_right_radius);
        compute_absolute_length(s, m, css_computed_border_bottom_left_radius,
                                set_border_bottom_left_radius);
        compute_absolute_length(s, m, css_computed_border_bottom_right_radius,
                                set_border_bottom_right_radius);

        compute_absolute_length(s, m, css_computed_box_shadow_x,
                                set_box_shadow_x);
        compute_absolute_length(s, m, css_computed_box_shadow_y,
                                set_box_shadow_y);
        compute_absolute_length(s, m, css_computed_box_shadow_blur,
                                set_box_shadow_blur);
        compute_absolute_length(s, m, css_computed_box_shadow_spread,
                                set_box_shadow_spread);
        compute_absolute_css_length(s, m, get_text_underline_offset,
                                    set_text_underline_offset);
        compute_absolute_css_length(s, m, get_text_decoration_thickness,
                                    set_text_decoration_thickness);
}

static void css_computed_style_destroy_font_family(char **font_family)
{
        unsigned i;

        if (!font_family) {
                return;
        }
        for (i = 0; font_family[i]; ++i) {
                free(font_family[i]);
        }
        free(font_family);
}

static char **css_computed_style_copy_font_family(char *const *font_family)
{
        unsigned i, count;
        char **copy;

        if (!font_family) {
                return NULL;
        }
        for (count = 0; font_family[count]; ++count) {
        }
        copy = calloc(count + 1, sizeof(char *));
        if (!copy) {
                return NULL;
        }
        for (i = 0; i < count; ++i) {
                copy[i] = y_strdup(font_family[i]);
        }
        return copy;
}

/*
 * Property copy/equality is generated from a single table (P0). Every
 * css_prop_key_t appears exactly once; the field layout is resolved at
 * compile time (C bitfields are not addressable at runtime), and a
 * a compile-time assert ties the table length to STYLE_KEY_TOTAL so that a
 * newly added property which is not registered here fails the build instead of
 * silently falling into the default branch.
 */

/* Regular forms: the struct field name is identical to the css_prop_* key
 * (NAME == FIELD convention); the key is passed through as the field. */
#define CSS_PROP_COPY_LENGTH(D, S, K) CSS_COPY_LENGTH(D, S, K)
#define CSS_PROP_COPY_VALUE_UNIT(D, S, K) CSS_COPY_VALUE_UNIT(D, S, K)
#define CSS_PROP_COPY_VALUE_TYPE(D, S, K) CSS_COPY_VALUE_TYPE(D, S, K)
#define CSS_PROP_COPY_VALUE(D, S, K) CSS_COPY_VALUE(D, S, K)
#define CSS_PROP_COPY_TYPE_BITS(D, S, K) CSS_COPY_TYPE_BITS(D, S, K)

#define CSS_PROP_EQ_LENGTH(A, B, K) CSS_LENGTH_EQUAL(A, B, K)
#define CSS_PROP_EQ_VALUE_UNIT(A, B, K) CSS_VALUE_UNIT_EQUAL(A, B, K)
#define CSS_PROP_EQ_VALUE_TYPE(A, B, K) CSS_VALUE_TYPE_EQUAL(A, B, K)
#define CSS_PROP_EQ_VALUE(A, B, K) CSS_VALUE_EQUAL(A, B, K)
#define CSS_PROP_EQ_TYPE_BITS(A, B, K) CSS_TYPE_BITS_EQUAL(A, B, K)

/* Owned string duplicated with y_strdup, plus its type_bits classifier. */
#define CSS_PROP_COPY_STRING(D, S, K)                          \
        do {                                                   \
                char *_str = (S)->K ? y_strdup((S)->K) : NULL; \
                if ((D)->K != (S)->K) {                        \
                        free((D)->K);                          \
                }                                              \
                (D)->K = _str;                                 \
                (D)->type_bits.K = (S)->type_bits.K;           \
        } while (0)
#define CSS_PROP_EQ_STRING(A, B, K) CSS_STRING_TYPE_EQUAL(A, B, K)

/* Owned font-family array with dedicated copy/destroy and element compare. */
#define CSS_PROP_COPY_FONT_FAMILY(D, S, K)                                 \
        do {                                                               \
                char **_ff =                                               \
                    css_computed_style_copy_font_family((S)->font_family); \
                if ((D)->font_family != (S)->font_family) {                \
                        css_computed_style_destroy_font_family(            \
                            (D)->font_family);                             \
                }                                                          \
                (D)->font_family = _ff;                                    \
                (D)->type_bits.font_family = (S)->type_bits.font_family;   \
        } while (0)
#define CSS_PROP_EQ_FONT_FAMILY(A, B, K) css_font_family_equal(A, B)

/* Composite properties that touch several fields under one key. */
#define CSS_PROP_COPY_BACKGROUND_SIZE(D, S, K)            \
        do {                                              \
                CSS_COPY_LENGTH(D, S, background_width);  \
                CSS_COPY_LENGTH(D, S, background_height); \
        } while (0)
#define CSS_PROP_EQ_BACKGROUND_SIZE(A, B, K)         \
        (CSS_LENGTH_EQUAL(A, B, background_width) && \
         CSS_LENGTH_EQUAL(A, B, background_height))

#define CSS_PROP_COPY_BOX_SHADOW(D, S, K)                     \
        do {                                                  \
                CSS_COPY_VALUE_UNIT(D, S, box_shadow_x);      \
                CSS_COPY_VALUE_UNIT(D, S, box_shadow_y);      \
                CSS_COPY_VALUE_UNIT(D, S, box_shadow_blur);   \
                CSS_COPY_VALUE_UNIT(D, S, box_shadow_spread); \
                CSS_COPY_VALUE(D, S, box_shadow_color);       \
                CSS_COPY_TYPE_BITS(D, S, box_shadow);         \
        } while (0)
#define CSS_PROP_EQ_BOX_SHADOW(A, B, K)                   \
        (CSS_VALUE_UNIT_EQUAL(A, B, box_shadow_x) &&      \
         CSS_VALUE_UNIT_EQUAL(A, B, box_shadow_y) &&      \
         CSS_VALUE_UNIT_EQUAL(A, B, box_shadow_blur) &&   \
         CSS_VALUE_UNIT_EQUAL(A, B, box_shadow_spread) && \
         CSS_VALUE_EQUAL(A, B, box_shadow_color) &&       \
         CSS_TYPE_BITS_EQUAL(A, B, box_shadow))

/* TYPE_BITS form whose storage field name differs from the key. */
#define CSS_PROP_COPY_TEXT_DECORATION(D, S, K) \
        CSS_COPY_TYPE_BITS(D, S, text_decoration)
#define CSS_PROP_EQ_TEXT_DECORATION(A, B, K) \
        CSS_TYPE_BITS_EQUAL(A, B, text_decoration)

/*
 * Single source of truth: KEY (css_prop_* suffix) + KIND (form above).
 * Listed in css_prop_key_t declaration order. background_size/box_shadow/
 * text_decoration_line/font_family/background_image/content use dedicated
 * kinds; everything else maps to a regular form.
 */
#define CSS_PROP_TABLE(X)                         \
        X(left, LENGTH)                           \
        X(right, LENGTH)                          \
        X(top, LENGTH)                            \
        X(bottom, LENGTH)                         \
        X(position, TYPE_BITS)                    \
        X(visibility, TYPE_BITS)                  \
        X(display, TYPE_BITS)                     \
        X(z_index, VALUE_TYPE)                    \
        X(opacity, VALUE_TYPE)                    \
        X(box_sizing, TYPE_BITS)                  \
        X(width, LENGTH)                          \
        X(height, LENGTH)                         \
        X(min_width, LENGTH)                      \
        X(min_height, LENGTH)                     \
        X(max_width, LENGTH)                      \
        X(max_height, LENGTH)                     \
        X(margin_top, LENGTH)                     \
        X(margin_right, LENGTH)                   \
        X(margin_bottom, LENGTH)                  \
        X(margin_left, LENGTH)                    \
        X(padding_top, LENGTH)                    \
        X(padding_right, LENGTH)                  \
        X(padding_bottom, LENGTH)                 \
        X(padding_left, LENGTH)                   \
        X(vertical_align, TYPE_BITS)              \
        X(border_top_width, VALUE_UNIT)           \
        X(border_top_style, TYPE_BITS)            \
        X(border_top_color, VALUE)                \
        X(border_right_width, VALUE_UNIT)         \
        X(border_right_style, TYPE_BITS)          \
        X(border_right_color, VALUE)              \
        X(border_bottom_width, VALUE_UNIT)        \
        X(border_bottom_style, TYPE_BITS)         \
        X(border_bottom_color, VALUE)             \
        X(border_left_width, VALUE_UNIT)          \
        X(border_left_style, TYPE_BITS)           \
        X(border_left_color, VALUE)               \
        X(border_top_left_radius, VALUE_UNIT)     \
        X(border_top_right_radius, VALUE_UNIT)    \
        X(border_bottom_left_radius, VALUE_UNIT)  \
        X(border_bottom_right_radius, VALUE_UNIT) \
        X(background_color, VALUE)                \
        X(background_image, STRING)               \
        X(background_size, BACKGROUND_SIZE)       \
        X(background_repeat, TYPE_BITS)           \
        X(background_clip, TYPE_BITS)             \
        X(background_position_x, LENGTH)          \
        X(background_position_y, LENGTH)          \
        X(box_shadow, BOX_SHADOW)                 \
        X(flex_basis, LENGTH)                     \
        X(flex_grow, VALUE_TYPE)                  \
        X(flex_shrink, VALUE_TYPE)                \
        X(flex_direction, TYPE_BITS)              \
        X(flex_wrap, TYPE_BITS)                   \
        X(justify_content, TYPE_BITS)             \
        X(align_content, TYPE_BITS)               \
        X(align_items, TYPE_BITS)                 \
        X(align_self, TYPE_BITS)                  \
        X(row_gap, LENGTH)                        \
        X(column_gap, LENGTH)                     \
        X(color, VALUE_TYPE)                      \
        X(font_size, LENGTH)                      \
        X(font_style, TYPE_BITS)                  \
        X(font_weight, TYPE_BITS)                 \
        X(font_family, FONT_FAMILY)               \
        X(line_height, LENGTH)                    \
        X(text_align, TYPE_BITS)                  \
        X(content, STRING)                        \
        X(white_space, TYPE_BITS)                 \
        X(word_break, TYPE_BITS)                  \
        X(pointer_events, TYPE_BITS)              \
        X(cursor, TYPE_BITS)                      \
        X(text_decoration_line, TEXT_DECORATION)  \
        X(text_underline_offset, LENGTH)          \
        X(text_decoration_thickness, LENGTH)

enum {
        CSS_PROP_TABLE_COUNT =
#define X(KEY, KIND) +1
            CSS_PROP_TABLE(X)
#undef X
            + 0
};
/*
 * Compile-time coverage gate. If this line fails to compile (MSVC reports
 * "error C2118: negative subscript"), a property was added to the
 * css_prop_key_t enum but is NOT registered in CSS_PROP_TABLE above.
 * FIX: add one `X(<name>, <KIND>)` row for every missing key so the table
 * lists exactly STYLE_KEY_TOTAL properties. The array is intentionally sized
 * -1 when the table is incomplete.
 */
typedef char css_prop_table_must_cover_every_css_prop_key
    [(CSS_PROP_TABLE_COUNT == STYLE_KEY_TOTAL) ? 1 : -1];

/* Element-wise font-family comparison (used by the FONT_FAMILY kind). */
static bool css_font_family_equal(const css_computed_style_t *a,
                                  const css_computed_style_t *b)
{
        unsigned i;

        if (!CSS_TYPE_BITS_EQUAL(a, b, font_family)) {
                return false;
        }
        if (!a->font_family || !b->font_family) {
                return a->font_family == b->font_family;
        }
        for (i = 0; a->font_family[i] && b->font_family[i]; ++i) {
                if (strcmp(a->font_family[i], b->font_family[i]) != 0) {
                        return false;
                }
        }
        return a->font_family[i] == NULL && b->font_family[i] == NULL;
}

void css_computed_style_copy_property(css_computed_style_t *dest,
                                      const css_computed_style_t *src,
                                      css_prop_key_t key)
{
        if (dest == src) {
                return;
        }
        switch (key) {
#define X(KEY, KIND)                                  \
        case css_prop_##KEY:                          \
                CSS_PROP_COPY_##KIND(dest, src, KEY); \
                return;
                CSS_PROP_TABLE(X)
#undef X
        default:
                assert(false);
                return;
        }
}

bool css_computed_style_property_equal(const css_computed_style_t *a,
                                       const css_computed_style_t *b,
                                       css_prop_key_t key)
{
        switch (key) {
#define X(KEY, KIND)         \
        case css_prop_##KEY: \
                return CSS_PROP_EQ_##KIND(a, b, KEY);
                CSS_PROP_TABLE(X)
#undef X
        default:
                assert(false);
                return false;
        }
}

void css_computed_style_destroy(css_computed_style_t *s)
{
        if (s->background_image) {
                free(s->background_image);
        }
        css_computed_style_destroy_font_family(s->font_family);
        if (s->content) {
                free(s->content);
        }
        s->background_image = NULL;
        s->font_family = NULL;
        s->content = NULL;
}
