/*
 * lib/ui/src/ui_widget_helper.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <assert.h>
#include <css.h>
#include <ui/base.h>
#include <ui/events.h>
#include <ui/style.h>
#include <ui/prototype.h>
#include "ui_widget_style.h"

bool ui_widget_is_visible(ui_widget_t *w)
{
        return css_computed_display(&w->computed_style) != CSS_DISPLAY_NONE &&
               css_computed_visibility(&w->computed_style) !=
                   CSS_VISIBILITY_HIDDEN;
}

void ui_widget_move(ui_widget_t *w, float left, float top)
{
        ui_widget_set_style_unit_value(w, css_prop_top, top, CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_prop_left, left, CSS_UNIT_PX);
}

void ui_widget_resize(ui_widget_t *w, float width, float height)
{
        ui_widget_set_style_unit_value(w, css_prop_width, width, CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_prop_height, height, CSS_UNIT_PX);
}

void ui_widget_set_background_image_url(ui_widget_t *w, const char *path)
{
        char *url;
        url = malloc(sizeof(char) * (strlen(path) + 10));
        sprintf(url, "url(%s)", path);
        ui_widget_set_style_string(w, "background-image", url);
        free(url);
}

void ui_widget_show(ui_widget_t *w)
{
        css_style_value_t v;

        ui_widget_get_style(w, css_prop_display, &v);
        if (v.type == CSS_ARRAY_VALUE &&
            v.array_value[0].type == CSS_KEYWORD_VALUE &&
            v.array_value[0].keyword_value == CSS_KEYWORD_NONE) {
                ui_widget_unset_style(w, css_prop_display);
                ui_widget_request_update_style(w);
        }
}

void ui_widget_hide(ui_widget_t *w)
{
        ui_widget_set_style_keyword_value(w, css_prop_display,
                                          CSS_KEYWORD_NONE);
        ui_widget_request_update_style(w);
}

ui_widget_t *ui_widget_get_closest(ui_widget_t *w, const char *type)
{
        ui_widget_t *target;

        for (target = w; target; target = target->parent) {
                if (ui_check_widget_type(target, type)) {
                        return target;
                }
        }
        return NULL;
}

static void ui_widget_collect_reference(ui_widget_t *w, void *arg)
{
        const char *ref = ui_widget_get_attr(w, "ref");

        if (ref) {
                dict_add(arg, (void *)ref, w);
        }
}

dict_t *ui_widget_collect_references(ui_widget_t *w)
{
        dict_t *dict;
        static dict_type_t t;

        dict_init_string_key_type(&t);
        dict = dict_create(&t, NULL);
        ui_widget_each(w, ui_widget_collect_reference, dict);
        return dict;
}

bool ui_widget_get_max_width(ui_widget_t *w, float *width)
{
        css_unit_t unit;

        return css_computed_max_width(&w->computed_style, width, &unit) ==
                   CSS_MAX_WIDTH_SET &&
               unit == CSS_UNIT_PX;
}

bool ui_widget_get_max_height(ui_widget_t *w, float *height)
{
        css_unit_t unit;

        return css_computed_max_height(&w->computed_style, height, &unit) ==
                   CSS_MAX_HEIGHT_SET &&
               unit == CSS_UNIT_PX;
}

bool ui_widget_get_min_width(ui_widget_t *w, float *width)
{
        css_unit_t unit;

        return css_computed_min_width(&w->computed_style, width, &unit) ==
                   CSS_MIN_WIDTH_SET &&
               unit == CSS_UNIT_PX;
}

bool ui_widget_get_min_height(ui_widget_t *w, float *height)
{
        css_unit_t unit;

        return css_computed_min_height(&w->computed_style, height, &unit) ==
                   CSS_MIN_HEIGHT_SET &&
               unit == CSS_UNIT_PX;
}

float ui_widget_fix_width(ui_widget_t *w, float width)
{
        css_computed_style_t *s = &w->computed_style;
        css_numeric_value_t limit;
        css_unit_t unit;

        if (css_computed_max_width(s, &limit, &unit) == CSS_MAX_WIDTH_SET &&
            unit == CSS_UNIT_PX && width > limit) {
                width = limit;
        }
        if (css_computed_min_width(s, &limit, &unit) == CSS_MIN_WIDTH_SET &&
            unit == CSS_UNIT_PX && width < limit) {
                width = limit;
        }
        if (is_css_border_box_sizing(s)) {
                if (width < css_border_x(s) + css_padding_x(s)) {
                        width = css_border_x(s) + css_padding_x(s);
                }
        } else {
                if (width < 0) {
                        width = 0;
                }
        }
        return width;
}

float ui_widget_fix_height(ui_widget_t *w, float height)
{
        css_computed_style_t *s = &w->computed_style;
        css_numeric_value_t limit;
        css_unit_t unit;

        if (css_computed_max_height(s, &limit, &unit) == CSS_MAX_HEIGHT_SET &&
            unit == CSS_UNIT_PX && height > limit) {
                height = limit;
        }
        if (css_computed_min_height(s, &limit, &unit) == CSS_MIN_HEIGHT_SET &&
            unit == CSS_UNIT_PX && height < limit) {
                height = limit;
        }
        if (is_css_border_box_sizing(s)) {
                if (height < css_border_y(s) + css_padding_y(s)) {
                        height = css_border_y(s) + css_padding_y(s);
                }
        } else {
                if (height < 0) {
                        height = 0;
                }
        }
        return height;
}
