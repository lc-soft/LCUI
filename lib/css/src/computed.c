/*
 * lib/css/src/computed.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <css/utils.h>
#include <css/computed.h>
#include <css/properties.h>
#include <css/style_value.h>
#include <css/style_decl.h>

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

int css_cascade_style(const css_style_decl_t *style,
                      css_computed_style_t *computed)
{
        unsigned i;
        list_node_t *node;
        css_prop_t *prop;
        const css_propdef_t *propdef;
        unsigned count = css_get_prop_count();
        libcss_bool_t *setted_props = calloc(count, sizeof(libcss_bool_t));

        for (list_each(node, style)) {
                prop = node->data;
                if (setted_props[prop->key] ||
                    prop->value.type <= CSS_INVALID_VALUE) {
                        continue;
                }
                setted_props[prop->key] = LIBCSS_TRUE;
                propdef = css_get_propdef(prop->key);
                assert(propdef && propdef->cascade);
                propdef->cascade(prop->value.array_value, computed);
                // TODO: 确定 cascade() 返回值的用法
        }
        // 给其它属性设置初始值
        for (i = 0; i < count; ++i) {
                if (setted_props[i]) {
                        continue;
                }
                propdef = css_get_propdef((int)i);
                propdef->cascade(propdef->initial_value.array_value, computed);
        }
        free(setted_props);
        return 0;
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
                value = value * m->density;
                break;
        case CSS_UNIT_SP:
                value = value * m->scaled_density;
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

static libcss_bool_t compute_content_box_fixed_width(
    const css_computed_style_t *s, css_numeric_value_t *value)
{
        css_unit_t unit;
        return compute_content_box_width(s, value, &unit) == CSS_WIDTH_SET &&
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

static libcss_bool_t compute_padding_box_fixed_width(
    const css_computed_style_t *s, css_numeric_value_t *value)
{
        css_unit_t unit;
        return compute_padding_box_width(s, value, &unit) == CSS_WIDTH_SET &&
               unit == CSS_UNIT_PX;
}

static libcss_bool_t compute_padding_box_fixed_height(
    const css_computed_style_t *s, css_numeric_value_t *value)
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
                if (is_css_display_block(parent) &&
                    compute_content_box_fixed_width(parent, &parent_value)) {
                        value = parent_value - s->margin_left - s->margin_right;
                        if (s->type_bits.box_sizing ==
                            CSS_BOX_SIZING_CONTENT_BOX) {
                                value -= css_padding_x(s) + css_border_x(s);
                        }
                        CSS_SET_FIXED_LENGTH(s, width, value);
                        break;
                }
                break;
        case CSS_WIDTH_SET:
                if (unit != CSS_UNIT_PERCENT) {
                        return;
                }
                if (!parent) {
                        s->type_bits.width = CSS_WIDTH_FIT_CONTENT;
                        return;
                }
                if (css_computed_position(s) > CSS_POSITION_RELATIVE) {
                        if (!compute_padding_box_fixed_width(parent,
                                                             &parent_value)) {
                                break;
                        }
                        CSS_SET_FIXED_LENGTH(s, width,
                                             parent_value * value / 100.0f);
                        break;
                }
                if (IS_CSS_FIXED_LENGTH(parent, width)) {
                        CSS_SET_FIXED_LENGTH(s, width,
                                             parent->width * value / 100.0f);
                }
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
                        s->type_bits.height = CSS_WIDTH_FIT_CONTENT;
                }
                break;
        case CSS_HEIGHT_SET:
                if (unit != CSS_UNIT_PERCENT) {
                        break;
                }
                if (!parent) {
                        s->type_bits.height = CSS_HEIGHT_FIT_CONTENT;
                        break;
                }
                if (css_computed_position(s) > CSS_POSITION_RELATIVE) {
                        if (!compute_padding_box_fixed_height(parent,
                                                              &parent_value)) {
                                break;
                        }
                        CSS_SET_FIXED_LENGTH(s, height,
                                             parent_value * value / 100.0f);
                        break;
                }
                if (IS_CSS_FIXED_LENGTH(parent, height)) {
                        CSS_SET_FIXED_LENGTH(s, height,
                                             parent->height * value / 100.0f);
                        break;
                }
        default:
                break;
        }
}

static void compute_absolute_flex_basis(const css_computed_style_t *parent,
                                        css_computed_style_t *s,
                                        css_metrics_t *m)
{
        compute_absolute_length(s, m, get_flex_basis, set_flex_basis);
        if (parent &&
            parent->type_bits.flex_direction == CSS_FLEX_DIRECTION_COLUMN) {
                if (s->type_bits.flex_basis == CSS_FLEX_BASIS_AUTO) {
                        if (IS_CSS_FIXED_LENGTH(s, height)) {
                                CSS_SET_FIXED_LENGTH(s, flex_basis, s->height);
                        } else {
                                s->type_bits.flex_basis =
                                    CSS_FLEX_BASIS_CONTENT;
                        }
                }
                return;
        }
        if (s->type_bits.flex_basis == CSS_FLEX_BASIS_AUTO) {
                if (IS_CSS_FIXED_LENGTH(s, width)) {
                        CSS_SET_FIXED_LENGTH(s, flex_basis, s->width);
                } else {
                        // 交给布局引擎计算实际值
                        s->type_bits.flex_basis = CSS_FLEX_BASIS_CONTENT;
                }
        }
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
}

void css_computed_style_destroy(css_computed_style_t *s)
{
        unsigned i;

        if (s->background_image) {
                free(s->background_image);
        }
        if (s->font_family) {
                for (i = 0; s->font_family[i]; ++i) {
                        free(s->font_family[i]);
                        s->font_family[i] = NULL;
                }
                free(s->font_family);
        }
        if (s->content) {
                free(s->content);
        }
        s->background_image = NULL;
        s->font_family = NULL;
        s->content = NULL;
}
