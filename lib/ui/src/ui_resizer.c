/*
 * lib/ui/src/ui_resizer.c
 *
 * Copyright (c) 2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui.h>
#include <css.h>
#include "ui_widget_style.h"
#include "ui_widget.h"
#include "ui_resizer.h"

LIBUI_INLINE bool ui_widget_is_wrap(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        if (ui_widget_has_flex_display(w)) {
                return s->type_bits.flex_wrap != CSS_FLEX_WRAP_NOWRAP;
        }
        return s->type_bits.white_space == CSS_WHITE_SPACE_NORMAL ||
               s->type_bits.white_space == CSS_WHITE_SPACE_PRE_WRAP;
}

void ui_widget_get_sizehint(ui_widget_t *w, ui_sizehint_t *hint)
{
        hint->min_width = 0;
        hint->min_height = 0;
        hint->max_width = 0;
        hint->max_height = 0;
        w->proto->sizehint(w, hint);
}

static float ui_widget_get_available_content_width(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        float width = w->parent->content_box.width - css_margin_x(s) -
                      css_border_x(s) - css_padding_x(s);

        if (width < 0) {
                width = 0;
        }
        return width;
}

void ui_widget_set_width_fit_content(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        float width = w->max_content_width;

        if (ui_widget_is_wrap(w)) {
                width = ui_widget_get_available_content_width(w);
                width = y_min(w->max_content_width, width);
        }
        width = y_max(w->min_content_width, width);
        CSS_SET_FIXED_LENGTH(s, width, css_width_from_cbox(s, width));
}

void ui_widget_set_width_fill_available(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        float width = ui_widget_get_available_content_width(w);

        CSS_SET_FIXED_LENGTH(s, width, css_width_from_cbox(s, width));
}

void ui_resizer_load_row_minmaxinfo(ui_resizer_t *resizer)
{
        float limit;
        css_computed_style_t *s = &resizer->target->computed_style;

        resizer->main_size = resizer->hint.max_width;
        resizer->cross_size = resizer->hint.max_height;
        resizer->min_main_size = resizer->hint.min_width;
        resizer->min_cross_size = resizer->hint.min_height;
        resizer->line_main_size = 0;
        if (ui_widget_get_max_width(resizer->target, &limit)) {
                resizer->max_main_size = css_cbox_width(s, limit);
        }
        if (IS_CSS_FIXED_LENGTH(s, width) &&
            resizer->max_main_size > s->width) {
                resizer->max_main_size = css_cbox_width(s, s->width);
        }
        if (ui_widget_get_min_width(resizer->target, &limit)) {
                limit = css_cbox_width(s, limit);
                if (resizer->max_main_size < limit) {
                        resizer->max_main_size = limit;
                }
        }
}

void ui_resizer_load_column_minmaxinfo(ui_resizer_t *resizer)
{
        float limit;
        css_computed_style_t *s = &resizer->target->computed_style;

        resizer->main_size = resizer->hint.max_height;
        resizer->cross_size = resizer->hint.max_width;
        resizer->min_main_size = resizer->hint.min_height;
        resizer->min_cross_size = resizer->hint.min_width;
        if (ui_widget_get_max_height(resizer->target, &limit)) {
                resizer->max_main_size = css_cbox_height(s, limit);
        }
        if (IS_CSS_FIXED_LENGTH(s, height) &&
            resizer->max_main_size > s->height) {
                resizer->max_main_size = css_cbox_height(s, s->height);
        }
        if (ui_widget_get_min_height(resizer->target, &limit)) {
                limit = css_cbox_height(s, limit);
                if (resizer->max_main_size < limit) {
                        resizer->max_main_size = limit;
                }
        }
}

void ui_resizer_init(ui_resizer_t *resizer, ui_widget_t *target)
{
        resizer->target = target;
        resizer->wrap = ui_widget_is_wrap(target);
        resizer->line_min_main_size = 0;
        resizer->line_min_cross_size = 0;
        resizer->line_main_size = 0;
        resizer->line_cross_size = 0;
        resizer->min_main_size = 0;
        resizer->min_cross_size = 0;
        resizer->main_size = 0;
        resizer->max_main_size = -1;
        ui_widget_get_sizehint(target, &resizer->hint);
}

static void ui_resizer_update_min_size(ui_resizer_t *resizer)
{
        resizer->min_main_size =
            y_max(resizer->min_main_size, resizer->line_min_main_size);
        resizer->min_cross_size += resizer->line_min_cross_size;
        resizer->line_min_main_size = 0;
        resizer->line_min_cross_size = 0;
}

void ui_resizer_update(ui_resizer_t *resizer)
{
        resizer->main_size = y_max(resizer->line_main_size, resizer->main_size);
        resizer->cross_size += resizer->line_cross_size;
        resizer->line_main_size = 0;
        resizer->line_cross_size = 0;
        ui_resizer_update_min_size(resizer);
}

void ui_resizer_load_item_main_size(ui_resizer_t *resizer, float main_size,
                                    float min_main_size)
{
        if (resizer->wrap) {
                resizer->line_min_main_size = min_main_size;
                ui_resizer_update_min_size(resizer);
        } else {
                resizer->line_min_main_size += min_main_size;
                if (resizer->max_main_size >= 0 &&
                    resizer->line_min_main_size > 0 &&
                    resizer->max_main_size < resizer->line_min_main_size) {
                        resizer->line_min_main_size -= min_main_size;
                        resizer->min_main_size =
                            y_max(resizer->min_main_size,
                                  resizer->line_min_main_size);
                        resizer->line_min_main_size = min_main_size;
                }
        }
        resizer->line_main_size += main_size;
        if (resizer->max_main_size >= 0 && resizer->line_main_size > 0 &&
            resizer->max_main_size < resizer->line_main_size) {
                resizer->line_main_size -= main_size;
                resizer->main_size =
                    y_max(resizer->main_size, resizer->line_main_size);
                resizer->line_main_size = main_size;
        }
}

void ui_resizer_load_item_cross_size(ui_resizer_t *resizer, float cross_size,
                                     float min_cross_size)
{
        resizer->line_min_cross_size =
            y_max(resizer->line_min_cross_size, min_cross_size);
        resizer->line_cross_size = y_max(resizer->line_cross_size, cross_size);
}

void ui_resizer_commit_row_main_size(ui_resizer_t *resizer)
{
        css_computed_style_t *s = &resizer->target->computed_style;

        ui_resizer_update(resizer);
        if (s->type_bits.min_width == CSS_MIN_WIDTH_MIN_CONTENT) {
                CSS_SET_FIXED_LENGTH(s, min_width, resizer->min_main_size);
        }
        if (!IS_CSS_FIXED_LENGTH(s, width)) {
                ui_widget_set_content_width(resizer->target,
                                            resizer->main_size);
                ui_widget_get_sizehint(resizer->target, &resizer->hint);
        }
        ui_widget_update_box_width(resizer->target);
}

void ui_resizer_commit_column_main_size(ui_resizer_t *resizer)
{
        css_computed_style_t *s = &resizer->target->computed_style;

        ui_resizer_update(resizer);
        if (s->type_bits.min_height == CSS_MIN_HEIGHT_MIN_CONTENT) {
                CSS_SET_FIXED_LENGTH(s, min_height, resizer->min_main_size);
        }
        if (!IS_CSS_FIXED_LENGTH(s, height)) {
                ui_widget_set_content_height(resizer->target,
                                             resizer->main_size);
                ui_widget_get_sizehint(resizer->target, &resizer->hint);
        }
        ui_widget_update_box_height(resizer->target);
}
