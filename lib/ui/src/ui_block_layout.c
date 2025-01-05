/*
 * lib/ui/src/ui_block_layout.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
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
#include "ui_resizer.h"

typedef struct ui_block_row {
        size_t index;
        float width;
        float height;

        /** list_t<ui_widget_t*> */
        list_t items;
} ui_block_row_t;

typedef struct ui_block_layout_context {
        ui_widget_t *widget;
        ui_resizer_t *resizer;

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
        row = ui_block_row_create();
        row->index = ctx->rows.length;
        list_append(&ctx->rows, row);
        return row;
}

static void ui_resizer_load_block_item_width(ui_resizer_t *resizer,
                                             ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;

        ui_resizer_load_item_main_size(
            resizer,
            css_obox_width(s, css_width_from_cbox(s, item->max_content_width)),
            css_obox_width(s, css_width_from_cbox(s, item->min_content_width)));
        ui_resizer_load_item_cross_size(
            resizer,
            css_obox_height(s,
                            css_height_from_cbox(s, item->max_content_height)),
            css_obox_height(s,
                            css_height_from_cbox(s, item->min_content_height)));
}

static void ui_block_layout_load_width(ui_block_layout_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        ui_resizer_t *resizer = ctx->resizer;
        css_computed_style_t *cs;

#ifdef UI_DEBUG_ENABLED
        size_t child_index = 0;
        size_t line_index = 0;

        UI_DEBUG_MSG("%s: begin, max_content_width=%g", __FUNCTION__,
                     resizer->max_main_size);
        ui_debug_msg_indent++;
#endif
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                ui_widget_reset_layout(child);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("line[%zu]: min_width=%g, width=%g, "
                                     "children[%zu]=%s",
                                     line_index, resizer->line_min_main_size,
                                     resizer->line_main_size, child_index, str);
                        UI_DEBUG_MSG(
                            "line[%zu]: load item, size=%s, "
                            "min_content_size=(%g, %g), "
                            "max_content_size=(%g, %g)",
                            line_index, size_str, child->min_content_width,
                            child->min_content_height, child->max_content_width,
                            child->max_content_height);
                }
#endif
                if (!ui_widget_in_layout_flow(child)) {
#ifdef UI_DEBUG_ENABLED
                        UI_DEBUG_MSG("line[%zu]: skip item", line_index);
                        child_index++;
#endif
                        continue;
                }
                switch (cs->type_bits.display) {
                case CSS_DISPLAY_FLEX:
                case CSS_DISPLAY_BLOCK:
                        ui_resizer_update(resizer);
#ifdef UI_DEBUG_ENABLED
                        UI_DEBUG_MSG("line[%zu]: next line, "
                                     "min_content_width=%g, content_width=%g",
                                     line_index, resizer->line_min_main_size,
                                     resizer->line_main_size);
                        line_index++;
#endif
                        break;
                default:
                        break;
                }
                ui_resizer_load_block_item_width(resizer, child);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_DEBUG_MSG(
                            "line[%zu]: load end, min_width=%g, width=%g",
                            line_index, resizer->line_min_main_size,
                            resizer->line_main_size);
                        child_index++;
                }
#endif
        }
        ui_resizer_commit_row_main_size(resizer);
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: end, min_content_width=%g, "
                     "max_content_width=%g, content_width=%g",
                     __FUNCTION__, resizer->min_main_size,
                     resizer->max_main_size, resizer->main_size);
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
        float content_height = 0;
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
                ui_widget_reset_width(child);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG(
                            "row[%zu][%zu] %s: before apply, size=%s, "
                            "content_box_size=(%g, %g), min_content_width=%g, "
                            "max_content_width=%g",
                            row->index, child_index, str, size_str,
                            child->content_box.width, child->content_box.height,
                            child->min_content_width, child->max_content_width);
                }
#endif
                switch (cs->type_bits.display) {
                case CSS_DISPLAY_INLINE_BLOCK:
                case CSS_DISPLAY_INLINE_FLEX:
                        if (!IS_CSS_FIXED_LENGTH(cs, width)) {
                                ui_widget_set_width_fit_content(child);
                        }
                        ui_widget_reflow_if_width_changed(child);
                        if (row->width > 0 && row->width +
                                                      child->outer_box.width -
                                                      content_width >
                                                  0.4f) {
                                content_height += row->height;
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
                                content_height += row->height;
                                row = ui_block_layout_next_row(ctx, row);
                        }
                        if (cs->type_bits.width == CSS_WIDTH_AUTO) {
                                ui_widget_set_width_fill_available(child);
#ifdef UI_DEBUG_ENABLED
                                {
                                        UI_WIDGET_STR(child, str);
                                        UI_DEBUG_MSG("row[%zu][%zu] %s: "
                                                     "width=%g (stretch)",
                                                     row->index, child_index,
                                                     str, cs->width);
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
                        content_height += row->height;
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
        content_height += row->height;
        if (content_height < ctx->resizer->hint.max_height) {
                content_height = ctx->resizer->hint.max_height;
        }
        if (!IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, height)) {
                ui_widget_set_content_height(ctx->widget, content_height);
        }
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: end, content_height=%g", __FUNCTION__,
                     content_height);
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
                UI_DEBUG_MSG(
                    "row[%zu]: update begin, x=%g, y=%g, width=%g, height=%g",
                    row->index, x, row_y, row->width, row->height);
                ui_debug_msg_indent++;
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
                        if (IS_CSS_PERCENTAGE(&child->specified_style,
                                              height)) {
                                ui_widget_reset_height(child);
                                ui_widget_reflow_if_height_changed(child);
                        }
                        if (ui_widget_is_inline(child)) {
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
#ifdef UI_DEBUG_ENABLED
                ui_debug_msg_indent--;
                UI_DEBUG_MSG("row[%zu]: update end", row->index);
#endif
                row_y += row->height;
        }
}

void ui_block_layout_reflow(ui_widget_t *w, ui_resizer_t *resizer)
{
        ui_block_layout_context_t ctx = { 0 };

        ctx.widget = w;
        ctx.resizer = resizer;
        ui_resizer_init(resizer, w);
        ui_resizer_load_row_minmaxinfo(resizer);

#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(w, str);
                UI_WIDGET_SIZE_STR(w, size_str);
                UI_WIDGET_MIN_SIZE_STR(w, min_size_str);
                UI_DEBUG_MSG("%s: %s: begin, size=%s, min_size=%s, "
                             "content_size=(%g, %g), "
                             "hint_content_size=(%g, %g)",
                             __FUNCTION__, str, size_str, min_size_str,
                             w->content_box.width, w->content_box.height,
                             ctx.resizer->hint.max_width,
                             ctx.resizer->cross_size);
                ui_debug_msg_indent++;
        }
#endif

        ui_block_layout_load_width(&ctx);
        ui_block_layout_apply_width(&ctx);
        ui_block_layout_update(&ctx);
        list_destroy(&ctx.rows, ui_block_row_destroy);
        w->proto->resize(w, w->content_box.width, w->content_box.height);

#ifdef UI_DEBUG_ENABLED
        {
                ui_debug_msg_indent--;
                UI_WIDGET_STR(w, str);
                UI_WIDGET_SIZE_STR(w, size_str);
                UI_DEBUG_MSG(
                    "%s: %s: end, fixed_size=%s, min_content_size=(%g, %g)",
                    __FUNCTION__, str, size_str, resizer->min_main_size,
                    resizer->min_cross_size);
        }
#endif
}
