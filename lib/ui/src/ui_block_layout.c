/*
 * lib/ui/src/ui_block_layout.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

// #define UI_DEBUG_ENABLED
#include <ui/base.h>
#include <ui/style.h>
#include <css/computed.h>
#include "ui_diff.h"
#include "ui_debug.h"
#include "ui_widget_style.h"
#include "ui_widget.h"

typedef struct ui_block_row {
        size_t index;
        float width;
        float height;

        /** list_t<ui_widget_t*> */
        list_t items;
} ui_block_row_t;

typedef struct ui_block_layout_context_t {
        ui_widget_t *widget;

        float content_height;
        float hint_content_width, hint_content_height;

        /*
         * list_t<ui_block_row_t> rows
         * Element rows in the static layout flow
         */
        list_t rows;
} ui_block_layout_context_t;

static ui_block_row_t *ui_block_row_create(void)
{
        ui_block_row_t *row;

        row = malloc(sizeof(ui_block_row_t));
        row->width = 0;
        row->height = 0;
        list_create(&row->items);
        return row;
}

static void ui_block_row_destroy(void *arg)
{
        ui_block_row_t *row = arg;

        list_destroy(&row->items, NULL);
        free(row);
}

static ui_block_row_t *ui_block_layout_next_row(ui_block_layout_context_t *ctx,
                                                ui_block_row_t *row)
{
        if (row) {
                ctx->content_height += row->height;
        }
        row = ui_block_row_create();
        row->index = ctx->rows.length;
        list_append(&ctx->rows, row);
        return row;
}

static void ui_compute_item_size(ui_widget_t *item, float *width, float *height)
{
        css_computed_style_t *s = &item->computed_style;
        float inner_width = css_padding_x(s) + css_border_x(s);
        float inner_height = css_padding_y(s) + css_border_y(s);
        bool is_border_box = is_css_border_box_sizing(s);

        *width = item->max_content_width + (is_border_box ? inner_width : 0);
        *height = item->max_content_height + (is_border_box ? inner_height : 0);
        if (IS_CSS_FIXED_LENGTH(s, width)) {
                *width = s->width;
                if (IS_CSS_FIXED_LENGTH(s, height)) {
                        *height = s->height;
                } else {
                        *height = 0;
                }
        }
        if (is_border_box) {
                if (*width < inner_width) {
                        *width = inner_width;
                }
                if (*height < inner_height) {
                        *height = inner_height;
                }
        }
        *width += css_margin_x(s);
        *height += css_margin_y(s);
}

static void ui_block_layout_load_width(ui_block_layout_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;

        float max_width = 0;
        float line_max_width = 0;
        float max_content_width = ctx->hint_content_width;
        bool max_width_valid = ui_widget_get_max_width(ctx->widget, &max_width);

        if (max_width_valid &&
            s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                max_width -= css_padding_x(s) + css_border_x(s);
        }
        if (IS_CSS_FIXED_LENGTH(s, width) && max_width > s->width) {
                max_width = css_width_to_content_box_width(s, s->width);
                max_width_valid = true;
        }
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                switch (cs->type_bits.display) {
                case CSS_DISPLAY_FLEX:
                case CSS_DISPLAY_BLOCK:
                        if (line_max_width > max_content_width) {
                                max_content_width = line_max_width;
                        }
                        line_max_width = 0;
                        break;
                default:
                        break;
                }
                line_max_width += child->outer_box.width;
                if (max_width_valid && line_max_width > max_width) {
                        line_max_width = max_width;
                        break;
                }
                if (line_max_width > max_content_width) {
                        max_content_width = line_max_width;
                }
        }
        if (!IS_CSS_FIXED_LENGTH(s, width)) {
                ui_widget_set_content_width(ctx->widget, max_content_width);
        }
#ifdef UI_DEBUG_ENABLED
        UI_DEBUG_MSG("%s: max_width(%d)=%g, max_content_width=%g", __FUNCTION__,
                     max_width_valid, max_width, max_content_width);
#endif
}

static void ui_block_layout_apply_width(ui_block_layout_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        ui_block_row_t *row;
        css_computed_style_t *cs;
        float space;
        float content_width = ctx->widget->content_box.width;
#ifdef UI_DEBUG_ENABLED
        size_t child_index = 0;

        UI_DEBUG_MSG("%s: begin, content_width=%g", __FUNCTION__,
                     content_width);
        ui_debug_msg_indent++;
#endif

        row = ui_block_layout_next_row(ctx, NULL);
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (!ui_widget_in_layout_flow(child)) {
#ifdef UI_DEBUG_ENABLED
                        UI_WIDGET_STR(child, str);
                        UI_DEBUG_MSG("row[%zu][%zu] %s: skip", row->index,
                                     child_index, str);
#endif
                        list_append(&row->items, child);
                        continue;
                }
                ui_widget_reset_layout(child);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("row[%zu][%zu] %s: before apply, size=%s, "
                                     "content_box_size=(%g, %g)",
                                     row->index, child_index, str, size_str,
                                     child->content_box.width,
                                     child->content_box.height);
                }
#endif
                switch (cs->type_bits.display) {
                case CSS_DISPLAY_INLINE_BLOCK:
                case CSS_DISPLAY_INLINE_FLEX:
                        ui_widget_reflow_if_width_changed(child);
                        if (row->width > 0 && row->width +
                                                      child->outer_box.width -
                                                      content_width >
                                                  0.4f) {
                                row = ui_block_layout_next_row(ctx, row);
                        }
                        row->width += child->outer_box.width;
                        if (child->outer_box.height > row->height) {
                                row->height = child->outer_box.height;
                        }
                        list_append(&row->items, child);
                        break;
                case CSS_DISPLAY_FLEX:
                case CSS_DISPLAY_BLOCK:
                        if (row->items.length > 0) {
                                row = ui_block_layout_next_row(ctx, row);
                        }
                        if (cs->type_bits.width == CSS_WIDTH_AUTO) {
                                CSS_SET_FIXED_LENGTH(
                                    cs, width,
                                    css_border_box_width_to_width(
                                        cs, content_width - css_margin_x(cs)));
#ifdef UI_DEBUG_ENABLED
                                {
                                        UI_WIDGET_STR(child, str);
                                        UI_DEBUG_MSG("row[%zu][%zu] %s: "
                                                     "width=%g (stretch)",
                                                     row->index, child_index,
                                                     str,
                                                     child->outer_box.width);
                                }
#endif
                        }
                        ui_widget_reflow_if_width_changed(child);
                        if (cs->type_bits.margin_left == CSS_MARGIN_AUTO) {
                                space = content_width - child->border_box.width;
                                if (cs->type_bits.margin_right ==
                                    CSS_MARGIN_AUTO) {
                                        CSS_SET_FIXED_LENGTH(cs, margin_left,
                                                             space / 2);
                                        CSS_SET_FIXED_LENGTH(cs, margin_right,
                                                             space / 2);
                                } else {
                                        CSS_SET_FIXED_LENGTH(
                                            cs, margin_left,
                                            space - cs->margin_right);
                                }
                        }
                        row->width = child->outer_box.width;
                        if (child->outer_box.height > row->height) {
                                row->height = child->outer_box.height;
                        }
                        list_append(&row->items, child);
                        row = ui_block_layout_next_row(ctx, row);
                        break;
                default:
                        continue;
                }
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("row[%zu][%zu] %s: after apply, size=%s, "
                                     "content_box_size=(%g, %g)",
                                     row->index, child_index, str, size_str,
                                     child->content_box.width,
                                     child->content_box.height);
                        child_index++;
                }
#endif
        }
        ctx->content_height += row->height;
        if (ctx->content_height < ctx->hint_content_height) {
                ctx->content_height = ctx->hint_content_height;
        }
        if (!IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, height)) {
                ui_widget_set_content_height(ctx->widget, ctx->content_height);
        }
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: end, content_height=%g", __FUNCTION__,
                     ctx->content_height);
#endif
}

static void ui_block_layout_update(ui_block_layout_context_t *ctx)
{
        list_node_t *node, *row_node;
        ui_widget_t *child;
        ui_block_row_t *row;
        float x, y, row_y;
#ifdef UI_DEBUG_ENABLED
        size_t child_index;
#endif

        row_y = ctx->widget->computed_style.padding_top;
        for (list_each(row_node, &ctx->rows)) {
                row = row_node->data;
                x = ctx->widget->computed_style.padding_left;
#ifdef UI_DEBUG_ENABLED
                child_index = 0;
                UI_DEBUG_MSG("row[%zu]: begin, x=%g, y=%g, width=%g, height=%g",
                             row->index, x, row_y, row->width, row->height);
#endif
                for (list_each(node, &row->items)) {
                        y = row_y;
                        child = node->data;
                        if (!ui_widget_in_layout_flow(child)) {
                                child->layout_x = x;
                                child->layout_y = y;
                                ui_widget_reset_layout(child);
                                ui_widget_reflow_if_width_changed(child);
                                ui_widget_reflow_if_height_changed(child);
                                ui_widget_update_box_position(child);
#ifdef UI_DEBUG_ENABLED
                                {
                                        UI_WIDGET_STR(child, str);
                                        UI_WIDGET_SIZE_STR(child, size_str);
                                        UI_DEBUG_MSG(
                                            "row[%zu][%zu]=%s, layout_x=%g, "
                                            "layout_y=%g, x=%g, y=%g, size=%s",
                                            row->index, child_index, str,
                                            child->layout_x, child->layout_y, x,
                                            y, size_str);
                                }
#endif
                                continue;
                        }
                        ui_widget_reset_height(child);
                        ui_widget_reflow_if_height_changed(child);
                        if (ui_widget_has_inline_block_display(child)) {
                                switch (css_computed_vertical_align(
                                    &child->computed_style)) {
                                case CSS_VERTICAL_ALIGN_MIDDLE:
                                        y += (row->height -
                                              child->border_box.height) /
                                             2.f;
                                        break;
                                case CSS_VERTICAL_ALIGN_BOTTOM:
                                        y += row->height -
                                             child->border_box.height;
                                default:
                                        break;
                                }
                        }
                        child->layout_x = x;
                        child->layout_y = y;
                        ui_widget_update_box_position(child);
#ifdef UI_DEBUG_ENABLED
                        {
                                UI_WIDGET_STR(child, str);
                                UI_WIDGET_SIZE_STR(child, size_str);
                                UI_DEBUG_MSG("row[%zu][%zu]=%s, "
                                             "x=%g, y=%g, size=%s",
                                             row->index, child_index, str, x, y,
                                             size_str);
                        }
                        child_index++;
#endif
                        x += child->outer_box.width;
                }
                row_y += row->height;
        }
}

void ui_block_layout_reflow(ui_widget_t *w)
{
        ui_block_layout_context_t ctx = { 0 };
        css_computed_style_t *s = &w->computed_style;
        css_computed_style_t *ps = &w->parent->computed_style;

        ctx.widget = w;
        ui_widget_get_hint_content_size(w, &ctx.hint_content_width,
                                        &ctx.hint_content_height);

#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(w, str);
                UI_WIDGET_SIZE_STR(w, size_str);
                UI_DEBUG_MSG("%s: %s: begin, size=%s, "
                             "hint_content_size=(%g, %g)",
                             __FUNCTION__, str, size_str,
                             ctx.hint_content_width, ctx.hint_content_height);
                ui_debug_msg_indent++;
        }
#endif

        ui_block_layout_load_width(&ctx);
        ui_block_layout_apply_width(&ctx);
        ui_block_layout_update(&ctx);
        w->proto->resize(w, w->content_box.width, w->content_box.height);

#ifdef UI_DEBUG_ENABLED
        {
                ui_debug_msg_indent--;
                UI_WIDGET_STR(w, str);
                UI_WIDGET_SIZE_STR(w, size_str);
                UI_DEBUG_MSG("%s: %s: end, fixed_size=%s", __FUNCTION__, str,
                             size_str);
        }
#endif
}
