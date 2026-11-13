/*
 * lib/ui/src/ui_diff.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
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

#define UI_STYLE_DEP_WORDS ((STYLE_KEY_TOTAL + 31) / 32)

/* Index of the lowest set bit of a non-zero word (word != 0 preconditions). */
static unsigned ui_style_dep_ctz(uint32_t word)
{
        unsigned n = 0;

        if ((word & 0xffffU) == 0) {
                n += 16;
                word >>= 16;
        }
        if ((word & 0xffU) == 0) {
                n += 8;
                word >>= 8;
        }
        if ((word & 0xfU) == 0) {
                n += 4;
                word >>= 4;
        }
        if ((word & 0x3U) == 0) {
                n += 2;
                word >>= 2;
        }
        if ((word & 0x1U) == 0) {
                n += 1;
        }
        return n;
}

static bool ui_style_dep_has(const uint32_t *bits, css_prop_key_t key)
{
        return (bits[key / 32] >> (key % 32)) & 1U;
}

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
        unsigned i;
        bool needed = false;
        list_node_t *node;
        ui_widget_t *child;

        diff->style = w->computed_style;
        diff->visible = ui_widget_is_visible(w);

        /* Union of every direct child's dependency bitmap. This is the exact
         * set of properties this node's inheritance can affect; when it is
         * empty propagation is impossible, so we skip the snapshot entirely. */
        for (i = 0; i < UI_STYLE_DEP_WORDS; ++i) {
                diff->dep_union[i] = 0;
        }
        for (list_each(node, &w->children)) {
                child = node->data;
                for (i = 0; i < UI_STYLE_DEP_WORDS; ++i) {
                        diff->dep_union[i] |= child->style_dependencies[i];
                }
        }
        for (i = 0; i < UI_STYLE_DEP_WORDS; ++i) {
                needed |= diff->dep_union[i] != 0;
        }
        if (!needed) {
                diff->has_style_snapshot = false;
                return;
        }

        /* Value-copy the pre-cascade specified_style. The copy aliases the
         * three owning heap fields; cascade will free w->specified_style's
         * copies, so make the snapshot independent only for the owning
         * properties children actually depend on, and drop the alias for the
         * rest so the snapshot never frees memory it does not own. */
        diff->specified_style = w->specified_style;
        if (ui_style_dep_has(diff->dep_union, css_prop_font_family)) {
                css_computed_style_copy_property(&diff->specified_style,
                                                 &w->specified_style,
                                                 css_prop_font_family);
        } else {
                diff->specified_style.font_family = NULL;
        }
        if (ui_style_dep_has(diff->dep_union, css_prop_background_image)) {
                css_computed_style_copy_property(&diff->specified_style,
                                                 &w->specified_style,
                                                 css_prop_background_image);
        } else {
                diff->specified_style.background_image = NULL;
        }
        if (ui_style_dep_has(diff->dep_union, css_prop_content)) {
                css_computed_style_copy_property(&diff->specified_style,
                                                 &w->specified_style,
                                                 css_prop_content);
        } else {
                diff->specified_style.content = NULL;
        }
        diff->has_style_snapshot = true;
}

static void ui_style_diff_update_dependent_children(ui_style_diff_t *diff,
                                                    ui_widget_t *w)
{
        uint32_t changed[UI_STYLE_DEP_WORDS];
        unsigned word, bit, prop;
        bool any_changed = false;
        list_node_t *node;
        ui_widget_t *child;

        if (!diff->has_style_snapshot) {
                return;
        }

        /* Compare only the unioned dependencies, extracting set bits word by
         * word with a ctz loop; unchanged properties truncate propagation. */
        memset(changed, 0, sizeof(changed));
        for (word = 0; word < UI_STYLE_DEP_WORDS; ++word) {
                uint32_t u = diff->dep_union[word];
                while (u != 0) {
                        bit = ui_style_dep_ctz(u);
                        prop = word * 32 + bit;
                        if (!css_computed_style_property_equal(
                                &diff->specified_style, &w->specified_style,
                                (css_prop_key_t)prop)) {
                                changed[word] |= 1U << bit;
                                any_changed = true;
                        }
                        u &= u - 1;
                }
        }
        if (any_changed) {
                for (list_each(node, &w->children)) {
                        child = node->data;
                        for (word = 0; word < UI_STYLE_DEP_WORDS; ++word) {
                                if (child->style_dependencies[word] &
                                    changed[word]) {
                                        ui_widget_request_update_style(child);
                                        break;
                                }
                        }
                }
        }

        css_computed_style_destroy(&diff->specified_style);
        diff->has_style_snapshot = false;
}

bool ui_style_diff_has_layout_change(ui_style_diff_t *diff, ui_widget_t *w)
{
        css_computed_style_t *a = &diff->style;
        css_computed_style_t *b = &w->computed_style;

        return a->type_bits.display != b->type_bits.display ||
               a->type_bits.position != b->type_bits.position ||
               a->type_bits.box_sizing != b->type_bits.box_sizing ||
               a->type_bits.width != b->type_bits.width ||
               a->unit_bits.width != b->unit_bits.width ||
               a->width != b->width ||
               a->type_bits.height != b->type_bits.height ||
               a->unit_bits.height != b->unit_bits.height ||
               a->height != b->height ||
               a->type_bits.min_width != b->type_bits.min_width ||
               a->unit_bits.min_width != b->unit_bits.min_width ||
               a->min_width != b->min_width ||
               a->type_bits.min_height != b->type_bits.min_height ||
               a->unit_bits.min_height != b->unit_bits.min_height ||
               a->min_height != b->min_height ||
               a->type_bits.max_width != b->type_bits.max_width ||
               a->unit_bits.max_width != b->unit_bits.max_width ||
               a->max_width != b->max_width ||
               a->type_bits.max_height != b->type_bits.max_height ||
               a->unit_bits.max_height != b->unit_bits.max_height ||
               a->max_height != b->max_height ||
               a->type_bits.margin_left != b->type_bits.margin_left ||
               a->unit_bits.margin_left != b->unit_bits.margin_left ||
               a->margin_left != b->margin_left ||
               a->type_bits.margin_right != b->type_bits.margin_right ||
               a->unit_bits.margin_right != b->unit_bits.margin_right ||
               a->margin_right != b->margin_right ||
               a->type_bits.margin_top != b->type_bits.margin_top ||
               a->unit_bits.margin_top != b->unit_bits.margin_top ||
               a->margin_top != b->margin_top ||
               a->type_bits.margin_bottom != b->type_bits.margin_bottom ||
               a->unit_bits.margin_bottom != b->unit_bits.margin_bottom ||
               a->margin_bottom != b->margin_bottom ||
               a->type_bits.padding_left != b->type_bits.padding_left ||
               a->unit_bits.padding_left != b->unit_bits.padding_left ||
               a->padding_left != b->padding_left ||
               a->type_bits.padding_right != b->type_bits.padding_right ||
               a->unit_bits.padding_right != b->unit_bits.padding_right ||
               a->padding_right != b->padding_right ||
               a->type_bits.padding_top != b->type_bits.padding_top ||
               a->unit_bits.padding_top != b->unit_bits.padding_top ||
               a->padding_top != b->padding_top ||
               a->type_bits.padding_bottom != b->type_bits.padding_bottom ||
               a->unit_bits.padding_bottom != b->unit_bits.padding_bottom ||
               a->padding_bottom != b->padding_bottom ||
               a->type_bits.border_top_style != b->type_bits.border_top_style ||
               a->unit_bits.border_top_width != b->unit_bits.border_top_width ||
               a->border_top_width != b->border_top_width ||
               a->type_bits.border_right_style !=
                   b->type_bits.border_right_style ||
               a->unit_bits.border_right_width !=
                   b->unit_bits.border_right_width ||
               a->border_right_width != b->border_right_width ||
               a->type_bits.border_bottom_style !=
                   b->type_bits.border_bottom_style ||
               a->unit_bits.border_bottom_width !=
                   b->unit_bits.border_bottom_width ||
               a->border_bottom_width != b->border_bottom_width ||
               a->type_bits.border_left_style !=
                   b->type_bits.border_left_style ||
               a->unit_bits.border_left_width !=
                   b->unit_bits.border_left_width ||
               a->border_left_width != b->border_left_width ||
               a->type_bits.flex_direction != b->type_bits.flex_direction ||
               a->type_bits.flex_wrap != b->type_bits.flex_wrap ||
               a->type_bits.justify_content != b->type_bits.justify_content ||
               a->type_bits.align_content != b->type_bits.align_content ||
               a->type_bits.align_items != b->type_bits.align_items ||
               a->flex_grow != b->flex_grow ||
               a->flex_shrink != b->flex_shrink ||
               a->type_bits.flex_basis != b->type_bits.flex_basis ||
               a->unit_bits.flex_basis != b->unit_bits.flex_basis ||
               a->flex_basis != b->flex_basis ||
               a->type_bits.row_gap != b->type_bits.row_gap ||
               a->unit_bits.row_gap != b->unit_bits.row_gap ||
               a->row_gap != b->row_gap ||
               a->type_bits.column_gap != b->type_bits.column_gap ||
               a->unit_bits.column_gap != b->unit_bits.column_gap ||
               a->column_gap != b->column_gap;
}

void ui_style_diff_end(ui_style_diff_t *diff, ui_widget_t *w)
{
        ui_mutation_record_t *record;
        bool flow_changed =
            IS_PROP_TYPE_CHANGED(display) || IS_PROP_TYPE_CHANGED(position);
        bool inner_changed = diff->padding_box.width != w->padding_box.width ||
                             diff->padding_box.height != w->padding_box.height;

        ui_style_diff_update_dependent_children(diff, w);
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
                        UI_DEBUG_MSG("%s: %s: size=%s, reflow", __FUNCTION__,
                                     str, size_str);
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
                        record->property_name = y_strdup("visible");
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
