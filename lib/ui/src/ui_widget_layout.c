/*
 * lib/ui/src/ui_widget_layout.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

// #define UI_DEBUG_ENABLED
#include <css/computed.h>
#include <ui/base.h>
#include <ui/events.h>
#include <ui/style.h>
#include "ui_debug.h"
#include "ui_block_layout.h"
#include "ui_flexbox_layout.h"
#include "ui_widget.h"
#include "ui_widget_style.h"

/**
 * 重置布局相关属性，并重新计算样式
 */
void ui_widget_reset_layout(ui_widget_t *w)
{
        css_computed_style_t *src = &w->specified_style;
        css_computed_style_t *dest = &w->computed_style;

        CSS_COPY_LENGTH(dest, src, width);
        CSS_COPY_LENGTH(dest, src, min_width);
        CSS_COPY_LENGTH(dest, src, max_width);
        CSS_COPY_LENGTH(dest, src, left);
        CSS_COPY_LENGTH(dest, src, right);
        CSS_COPY_LENGTH(dest, src, padding_left);
        CSS_COPY_LENGTH(dest, src, padding_right);
        CSS_COPY_LENGTH(dest, src, margin_left);
        CSS_COPY_LENGTH(dest, src, margin_right);
        CSS_COPY_LENGTH(dest, src, height);
        CSS_COPY_LENGTH(dest, src, min_height);
        CSS_COPY_LENGTH(dest, src, max_height);
        CSS_COPY_LENGTH(dest, src, top);
        CSS_COPY_LENGTH(dest, src, bottom);
        CSS_COPY_LENGTH(dest, src, padding_top);
        CSS_COPY_LENGTH(dest, src, padding_bottom);
        CSS_COPY_LENGTH(dest, src, margin_top);
        CSS_COPY_LENGTH(dest, src, margin_bottom);
        CSS_COPY_LENGTH(dest, src, flex_basis);
        ui_widget_compute_style(w);
}

void ui_widget_reset_width(ui_widget_t *w)
{
        css_computed_style_t *src = &w->specified_style;
        css_computed_style_t *dest = &w->computed_style;

        CSS_COPY_LENGTH(dest, src, width);
        CSS_COPY_LENGTH(dest, src, min_width);
        CSS_COPY_LENGTH(dest, src, max_width);
        CSS_COPY_LENGTH(dest, src, left);
        CSS_COPY_LENGTH(dest, src, right);
        CSS_COPY_LENGTH(dest, src, padding_left);
        CSS_COPY_LENGTH(dest, src, padding_right);
        CSS_COPY_LENGTH(dest, src, margin_left);
        CSS_COPY_LENGTH(dest, src, margin_right);
        ui_widget_compute_style(w);
}

void ui_widget_reset_height(ui_widget_t *w)
{
        css_computed_style_t *src = &w->specified_style;
        css_computed_style_t *dest = &w->computed_style;

        CSS_COPY_LENGTH(dest, src, height);
        CSS_COPY_LENGTH(dest, src, min_height);
        CSS_COPY_LENGTH(dest, src, max_height);
        CSS_COPY_LENGTH(dest, src, top);
        CSS_COPY_LENGTH(dest, src, bottom);
        CSS_COPY_LENGTH(dest, src, padding_top);
        CSS_COPY_LENGTH(dest, src, padding_bottom);
        CSS_COPY_LENGTH(dest, src, margin_top);
        CSS_COPY_LENGTH(dest, src, margin_bottom);
        ui_widget_compute_style(w);
}

void ui_widget_reflow_if_width_changed(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        int content_box_width = (int)(w->content_box.width * 64.f);
        int padding_box_width = (int)(w->padding_box.width * 64.f);

        if (s->type_bits.width == CSS_WIDTH_FIT_CONTENT) {
                ui_widget_set_computed_width(
                    w, css_content_box_width_to_width(s, w->max_content_width));
        }
        ui_widget_update_box_width(w);
        if (content_box_width != (int)(w->content_box.width * 64.f) ||
            padding_box_width != (int)(w->padding_box.width * 64.f)) {
#ifdef UI_DEBUG_ENABLED
                UI_WIDGET_STR(w, str);
                UI_DEBUG_MSG(
                    "%s: %s: width changed, content_box_width[%g => %g], "
                    "padding_box_width[%g => %g]",
                    __FUNCTION__, str, content_box_width / 64.f,
                    w->content_box.width, padding_box_width / 64.f,
                    w->padding_box.width);
#endif
                ui_widget_reflow(w);
        }
}

void ui_widget_reflow_if_height_changed(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        int content_box_height = (int)(w->content_box.height * 64.f);
        int padding_box_height = (int)(w->padding_box.height * 64.f);

        if (s->type_bits.height == CSS_HEIGHT_FIT_CONTENT) {
                ui_widget_set_computed_height(
                    w,
                    css_content_box_height_to_height(s, w->max_content_height));
        }
        ui_widget_update_box_height(w);
        if (content_box_height != (int)(w->content_box.height * 64.f) ||
            padding_box_height != (int)(w->padding_box.height * 64.f)) {
#ifdef UI_DEBUG_ENABLED
                UI_WIDGET_STR(w, str);
                UI_DEBUG_MSG(
                    "%s: %s: height changed, content_box_height[%g => %g], "
                    "padding_box_height[%g => %g]",
                    __FUNCTION__, str, content_box_height / 64.f,
                    w->content_box.height, padding_box_height / 64.f,
                    w->padding_box.height);
#endif
                ui_widget_reflow(w);
        }
}

void ui_widget_reflow(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        ui_event_t ev = { .type = UI_EVENT_AFTERLAYOUT, .cancel_bubble = true };

        switch (w->computed_style.type_bits.display) {
        case CSS_DISPLAY_BLOCK:
        case CSS_DISPLAY_INLINE_BLOCK:
                ui_block_layout_reflow(w);
                break;
        case CSS_DISPLAY_FLEX:
        case CSS_DISPLAY_INLINE_FLEX:
                ui_flexbox_layout_reflow(w);
                break;
        case CSS_DISPLAY_NONE:
        default:
                break;
        }
#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(w, str);
                UI_DEBUG_MSG("%s: %s: max_content_size=(%g, %g)", __FUNCTION__,
                             str, w->max_content_width, w->max_content_height);
        }
#endif
        ui_widget_post_event(w, &ev, NULL, NULL);
        ui_widget_add_state(w, UI_WIDGET_STATE_LAYOUTED);
}
