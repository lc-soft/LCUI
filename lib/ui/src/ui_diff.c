/*
 * lib/ui/src/ui_diff.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

// #define UI_DEBUG_ENABLED
#include <string.h>
#include <css/computed.h>
#include <ui/base.h>
#include <ui/rect.h>
#include <ui/mutation_observer.h>
#include "ui_widget.h"
#include "ui_widget_style.h"
#include "ui_widget_observer.h"
#include "ui_diff.h"
#include "ui_debug.h"

#define IS_PROP_TYPE_CHANGED(PROP_KEY) \
        diff->style.type_bits.PROP_KEY != w->computed_style.type_bits.PROP_KEY

#define IS_PROP_VALUE_CHANGED(PROP_KEY) \
        diff->style.PROP_KEY != w->computed_style.PROP_KEY

#define IS_LENGTH_PROP_CHANGED(PROP_KEY) \
        (IS_PROP_TYPE_CHANGED(PROP_KEY) || IS_PROP_VALUE_CHANGED(PROP_KEY))

void ui_style_diff_init(ui_style_diff_t *diff, ui_widget_t *w)
{
        ui_widget_t *parent;

        diff->canvas_box = w->canvas_box;
        diff->padding_box = w->padding_box;
        diff->should_add_dirty_rect = false;
        for (parent = w->parent; parent; parent = parent->parent) {
                if (!ui_widget_is_visible(parent) ||
                    parent->rendering.dirty_rect_type ==
                        UI_DIRTY_RECT_TYPE_FULL) {
                        return;
                }
        }
        diff->should_add_dirty_rect = true;
}

void ui_style_diff_begin(ui_style_diff_t *diff, ui_widget_t *w)
{
        diff->style = w->computed_style;
        diff->visible = ui_widget_is_visible(w);
}

void ui_style_diff_end(ui_style_diff_t *diff, ui_widget_t *w)
{
        ui_mutation_record_t *record;
        bool flow_changed =
            IS_PROP_TYPE_CHANGED(display) || IS_PROP_TYPE_CHANGED(position);
        bool inner_changed = diff->padding_box.width != w->padding_box.width ||
                             diff->padding_box.height != w->padding_box.height;

        if (flow_changed || inner_changed ||
            (ui_widget_has_flex_display(w) &&
             (IS_PROP_TYPE_CHANGED(flex_wrap) ||
              IS_PROP_TYPE_CHANGED(flex_direction) ||
              IS_PROP_TYPE_CHANGED(justify_content) ||
              IS_PROP_TYPE_CHANGED(align_content) ||
              IS_PROP_TYPE_CHANGED(align_items)))) {
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(w, str);
                        UI_WIDGET_SIZE_STR(w, size_str);
                        UI_DEBUG_MSG("%s: %s: size=%s, reflow", __FUNCTION__, str, size_str);
                }
#endif
                ui_widget_request_reflow(w);
        }
        if (w->parent && (flow_changed || ui_widget_in_layout_flow(w))) {
                if (flow_changed || inner_changed ||
                    IS_LENGTH_PROP_CHANGED(margin_left) ||
                    IS_LENGTH_PROP_CHANGED(margin_right) ||
                    IS_LENGTH_PROP_CHANGED(margin_top) ||
                    IS_LENGTH_PROP_CHANGED(margin_bottom) ||
                    (ui_widget_has_flex_display(w->parent) &&
                     (IS_PROP_TYPE_CHANGED(flex_grow) ||
                      IS_PROP_TYPE_CHANGED(flex_shrink) ||
                      IS_PROP_TYPE_CHANGED(flex_basis)))) {
                        ui_widget_request_reflow(w->parent);
                }
        }
        if (ui_widget_is_visible(w) != diff->visible) {
                if (ui_widget_has_observer(
                        w, UI_MUTATION_RECORD_TYPE_PROPERTIES)) {
                        record = ui_mutation_record_create(
                            w, UI_MUTATION_RECORD_TYPE_PROPERTIES);
                        record->property_name = strdup2("visible");
                        ui_widget_add_mutation_record(w, record);
                        ui_mutation_record_destroy(record);
                }
                if (diff->should_add_dirty_rect) {
                        w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
                        w->rendering.dirty_rect = diff->canvas_box;
                        ui_widget_expose_dirty_rect(w);
#ifdef UI_DEBUG_ENABLED
                        {
                                UI_WIDGET_STR(w, str);
                                UI_WIDGET_SIZE_STR(w, size_str);
                                UI_DEBUG_MSG("%s: %s: size=%s, repaint all",
                                             __FUNCTION__, str, size_str);
                        }
#endif
                }
                return;
        }
        if (!diff->should_add_dirty_rect) {
                return;
        }
        if (ui_widget_is_visible(w) != diff->visible ||
            IS_PROP_VALUE_CHANGED(opacity) || IS_PROP_VALUE_CHANGED(z_index) ||
            IS_PROP_VALUE_CHANGED(box_shadow_color) ||
            IS_PROP_VALUE_CHANGED(box_shadow_x) ||
            IS_PROP_VALUE_CHANGED(box_shadow_y) ||
            IS_PROP_VALUE_CHANGED(box_shadow_blur) ||
            IS_PROP_VALUE_CHANGED(box_shadow_spread) ||
            IS_PROP_VALUE_CHANGED(border_top_width) ||
            IS_PROP_VALUE_CHANGED(border_right_width) ||
            IS_PROP_VALUE_CHANGED(border_bottom_width) ||
            IS_PROP_VALUE_CHANGED(border_left_width) ||
            IS_PROP_VALUE_CHANGED(border_top_color) ||
            IS_PROP_VALUE_CHANGED(border_right_color) ||
            IS_PROP_VALUE_CHANGED(border_bottom_color) ||
            IS_PROP_VALUE_CHANGED(border_left_color) ||
            IS_PROP_TYPE_CHANGED(border_top_style) ||
            IS_PROP_TYPE_CHANGED(border_right_style) ||
            IS_PROP_TYPE_CHANGED(border_bottom_style) ||
            IS_PROP_TYPE_CHANGED(border_left_style) ||
            IS_PROP_VALUE_CHANGED(border_top_left_radius) ||
            IS_PROP_VALUE_CHANGED(border_top_right_radius) ||
            IS_PROP_VALUE_CHANGED(border_bottom_left_radius) ||
            IS_PROP_VALUE_CHANGED(border_bottom_right_radius) ||
            IS_PROP_VALUE_CHANGED(background_color) ||
            IS_PROP_VALUE_CHANGED(background_image) ||
            IS_PROP_VALUE_CHANGED(background_position_x) ||
            IS_PROP_VALUE_CHANGED(background_position_y) ||
            IS_PROP_VALUE_CHANGED(background_width) ||
            IS_PROP_VALUE_CHANGED(background_height)) {
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(w, str);
                        UI_WIDGET_SIZE_STR(w, size_str);
                        UI_DEBUG_MSG("%s: %s: size=%s, repaint all",
                                     __FUNCTION__, str, size_str);
                }
#endif
                w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
                ui_widget_expose_dirty_rect(w);
        }
}
