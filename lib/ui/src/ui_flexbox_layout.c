/*
 * lib/ui/src/ui_flexbox_layout.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

/**
 * 弹性布局
 *
 * 布局算法参考 W3C 的文档：https://www.w3.org/TR/css-flexbox/#layout-algorithm
 */

// #define UI_DEBUG_ENABLED
#include <ui.h>
#include <ui/style.h>
#include <css/computed.h>
#include "ui_diff.h"
#include "ui_debug.h"
#include "ui_widget_style.h"
#include "ui_widget.h"

typedef struct ui_flexbox_line {
        size_t index;
        float main_size;
        float cross_axis;
        float cross_size;
        float sum_of_grow_value;
        float sum_of_shrink_value;
        size_t count_of_auto_margin_items;

        /** list_t<ui_widget_t*> items */
        list_t items;
} ui_flexbox_line_t;

typedef struct ui_flexbox_layout_context_t {
        ui_widget_t *widget;
        bool column_direction;
        ui_sizehint_t sizehint;
        float cross_size;

        /** list_t<ui_flexbox_line_t*> lines */
        list_t lines;
} ui_flexbox_layout_context_t;

static ui_flexbox_line_t *ui_flexbox_line_create(void)
{
        ui_flexbox_line_t *line;

        line = malloc(sizeof(ui_flexbox_line_t));
        line->main_size = 0;
        line->cross_size = 0;
        line->sum_of_grow_value = 0;
        line->sum_of_shrink_value = 0;
        line->count_of_auto_margin_items = 0;
        list_create(&line->items);
        return line;
}

static void ui_flexbox_line_destroy(void *arg)
{
        ui_flexbox_line_t *line = arg;

        list_destroy(&line->items, NULL);
        free(line);
}

static void ui_flexbox_line_load_item(ui_flexbox_line_t *line,
                                      ui_widget_t *item)
{
        if (item->computed_style.flex_grow > 0) {
                line->sum_of_grow_value += item->computed_style.flex_grow;
        }
        if (item->computed_style.flex_shrink > 0) {
                line->sum_of_shrink_value += item->computed_style.flex_shrink;
        }
        list_append(&line->items, item);
}

static ui_flexbox_line_t *ui_flexbox_layout_next_line(
    ui_flexbox_layout_context_t *ctx)
{
        ui_flexbox_line_t *next_line = ui_flexbox_line_create();
        next_line->index = ctx->lines.length;
        list_append(&ctx->lines, next_line);
        return next_line;
}

static void ui_reset_row_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;

        CSS_COPY_LENGTH(s, &item->specified_style, flex_basis);
        if (s->type_bits.width == CSS_WIDTH_AUTO) {
                s->type_bits.width = CSS_WIDTH_FIT_CONTENT;
        }
        if (s->type_bits.flex_basis == CSS_FLEX_BASIS_AUTO) {
                if (!IS_CSS_FIXED_LENGTH(s, width)) {
                        s->type_bits.flex_basis = CSS_FLEX_BASIS_CONTENT;
                        return;
                }
                CSS_SET_FIXED_LENGTH(s, flex_basis, s->width);
        }
}

static void ui_reset_column_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;

        CSS_COPY_LENGTH(s, &item->specified_style, flex_basis);
        if (s->type_bits.height == CSS_HEIGHT_AUTO) {
                s->type_bits.height = CSS_HEIGHT_FIT_CONTENT;
        }
        if (s->type_bits.flex_basis == CSS_FLEX_BASIS_AUTO) {
                if (!IS_CSS_FIXED_LENGTH(s, height)) {
                        s->type_bits.flex_basis = CSS_FLEX_BASIS_CONTENT;
                        return;
                }
                CSS_SET_FIXED_LENGTH(s, flex_basis, s->height);
        }
}

static void ui_compute_row_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;
        float main_inner_size = css_padding_x(s) + css_border_x(s);

        if (IS_CSS_PERCENTAGE(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    s->flex_basis * item->parent->content_box.width / 100);
        } else if (!IS_CSS_FIXED_LENGTH(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    item->max_content_width +
                        (is_css_border_box_sizing(s) ? main_inner_size : 0));
        }
        s->flex_basis = ui_widget_fix_width(item, s->flex_basis);
        if (is_css_border_box_sizing(s)) {
                if (s->flex_basis < main_inner_size) {
                        s->flex_basis = main_inner_size;
                }
        }
}

static void ui_compute_column_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;
        float main_inner_size = css_padding_y(s) + css_border_y(s);
        
        if (IS_CSS_PERCENTAGE(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    s->flex_basis * item->parent->content_box.height / 100);
        } else if (!IS_CSS_FIXED_LENGTH(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    item->max_content_height +
                        (is_css_border_box_sizing(s) ? main_inner_size : 0));
        }
        s->flex_basis = ui_widget_fix_height(item, s->flex_basis);
        if (is_css_border_box_sizing(s)) {
                if (s->flex_basis < main_inner_size) {
                        s->flex_basis = main_inner_size;
                }
        }
}

static void ui_flexbox_layout_compute_justify_content(
    ui_flexbox_layout_context_t *ctx, ui_flexbox_line_t *line,
    float *start_axis, float *space)
{
        float free_space;

        if (ctx->column_direction) {
                *start_axis = ctx->widget->computed_style.padding_top;
                free_space = ctx->widget->content_box.height;
        } else {
                *start_axis = ctx->widget->computed_style.padding_left;
                free_space = ctx->widget->content_box.width;
        }
        *space = 0;
        free_space -= line->main_size;
        switch (ctx->widget->computed_style.type_bits.justify_content) {
        case CSS_JUSTIFY_CONTENT_SPACE_BETWEEN:
                if (line->items.length > 1) {
                        *space = free_space / (line->items.length - 1);
                }
                *start_axis -= *space;
                break;
        case CSS_JUSTIFY_CONTENT_SPACE_AROUND:
                *space = free_space / line->items.length;
                *start_axis -= *space * 0.5f;
                break;
        case CSS_JUSTIFY_CONTENT_SPACE_EVENLY:
                *space = free_space / (line->items.length + 1);
                *start_axis += *space;
                break;
        case CSS_JUSTIFY_CONTENT_FLEX_END:
                *start_axis += free_space;
                break;
        case CSS_JUSTIFY_CONTENT_CENTER:
                *start_axis += free_space * 0.5f;
                break;
        default:
                break;
        }
}

static void ui_flexbox_row_layout_load_main_size(
    ui_flexbox_layout_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;
        float main_size = 0;
        float min_main_size = 0;

#ifdef UI_DEBUG_ENABLED
        unsigned child_index = 0;

        UI_DEBUG_MSG("%s: begin, main_size_fixed?=%d", __FUNCTION__,
                     IS_CSS_FIXED_LENGTH(s, width));
        ui_debug_msg_indent++;
#endif
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                ui_widget_reset_layout(child);
                ui_reset_row_item_flex_basis(child);
                if (IS_CSS_FIXED_LENGTH(cs, flex_basis)) {
                        min_main_size += cs->flex_basis;
                        if (!is_css_border_box_sizing(cs)) {
                                min_main_size +=
                                    css_padding_x(cs) + css_border_x(cs);
                        }
                } else {
                        min_main_size += css_padding_x(cs) + css_border_x(cs);
                }
                ui_compute_row_item_flex_basis(child);
                main_size += cs->flex_basis;
                if (!is_css_border_box_sizing(cs)) {
                        main_size += css_padding_x(cs) + css_border_x(cs);
                }
                min_main_size += css_margin_x(cs);
                main_size += css_margin_x(cs);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG(
                            "children[%u]=%s, size=%s, max_content_size=(%g, "
                            "%g), max_width(%d)=%g, flex_basis(%d)=%g",
                            child_index, str, size_str,
                            child->max_content_width, child->max_content_height,
                            cs->type_bits.max_width, cs->max_width,
                            cs->type_bits.flex_basis, cs->flex_basis);
                        child_index++;
                }
#endif
        }
        if (main_size < ctx->sizehint.max_width) {
                main_size = ctx->sizehint.max_width;
        }
        if (ctx->sizehint.available_width > 0 &&
            main_size > ctx->sizehint.available_width) {
                main_size = ctx->sizehint.available_width;
        }
        if (main_size < min_main_size) {
                main_size = min_main_size;
        }
        if (!IS_CSS_FIXED_LENGTH(s, width)) {
                ui_widget_set_content_width(
                    ctx->widget, css_content_box_width_to_width(s, main_size));
        }
#ifdef UI_DEBUG_ENABLED
        {
                ui_debug_msg_indent--;
                UI_WIDGET_SIZE_STR(ctx->widget, size_str);
                UI_DEBUG_MSG("%s: end, main_size=%g, size=%s", __FUNCTION__,
                             main_size, size_str);
        }
#endif
}

static void ui_flexbox_column_layout_load_main_size(
    ui_flexbox_layout_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;
        float main_size = 0, min_main_size = 0;

#ifdef UI_DEBUG_ENABLED
        unsigned child_index = 0;
        UI_DEBUG_MSG("%s: begin, main_size_fixed?=%d", __FUNCTION__,
                     IS_CSS_FIXED_LENGTH(s, height));
        ui_debug_msg_indent++;
#endif
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                ui_widget_reset_layout(child);
                ui_reset_column_item_flex_basis(child);
                if (IS_CSS_FIXED_LENGTH(cs, flex_basis)) {
                        min_main_size += cs->flex_basis;
                        if (!is_css_border_box_sizing(cs)) {
                                min_main_size +=
                                    css_padding_y(cs) + css_border_y(cs);
                        }
                } else {
                        min_main_size += css_padding_y(cs) + css_border_y(cs);
                }
                ui_compute_column_item_flex_basis(child);
                main_size += cs->flex_basis;
                if (cs->type_bits.box_sizing == CSS_BOX_SIZING_CONTENT_BOX) {
                        main_size += css_padding_y(cs) + css_border_y(cs);
                }
                min_main_size += css_margin_y(cs);
                main_size += css_margin_y(cs);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG(
                            "children[%u]=%s, size=%s, max_content_size=(%g, "
                            "%g), flex_basis(%d)=%g",
                            child_index, str, size_str,
                            child->max_content_width, child->max_content_height,
                            cs->type_bits.flex_basis, cs->flex_basis);
                        child_index++;
                }
#endif
        }
        if (main_size < ctx->sizehint.max_height) {
                main_size = ctx->sizehint.max_height;
        }
        if (ctx->sizehint.available_height > 0 &&
            main_size > ctx->sizehint.available_height) {
                main_size = ctx->sizehint.available_height;
        }
        if (main_size < min_main_size) {
                main_size = min_main_size;
        }
        if (!IS_CSS_FIXED_LENGTH(s, height)) {
                ui_widget_set_content_height(ctx->widget, main_size);
        }
#ifdef UI_DEBUG_ENABLED
        {
                ui_debug_msg_indent--;
                UI_WIDGET_SIZE_STR(ctx->widget, size_str);
                UI_DEBUG_MSG("%s: end, main_size=%g, size=%s", __FUNCTION__,
                             main_size, size_str);
        }
#endif
}

static void ui_apply_row_item_main_size(ui_widget_t *item, float space,
                                        float flex_space, float margin_space)
{
        css_computed_style_t *cs = &item->computed_style;

        CSS_SET_FIXED_LENGTH(cs, width, cs->flex_basis);
        if (space >= 0) {
                if (cs->flex_grow > 0) {
                        CSS_SET_FIXED_LENGTH(
                            cs, width,
                            cs->flex_basis + flex_space * cs->flex_grow);
                }
                if (cs->type_bits.margin_left == CSS_MARGIN_AUTO) {
                        CSS_SET_FIXED_LENGTH(cs, margin_left, margin_space);
                }
                if (cs->type_bits.margin_right == CSS_MARGIN_AUTO) {
                        CSS_SET_FIXED_LENGTH(cs, margin_right, margin_space);
                }
        } else if (cs->flex_shrink > 0) {
                CSS_SET_FIXED_LENGTH(
                    cs, width, cs->flex_basis + flex_space * cs->flex_shrink);
        }
        ui_widget_reflow_if_width_changed(item);
}

static void ui_apply_column_item_main_size(ui_widget_t *item, float space,
                                           float flex_space, float margin_space)
{
        css_computed_style_t *cs = &item->computed_style;

        CSS_SET_FIXED_LENGTH(cs, height, cs->flex_basis);
        if (space >= 0) {
                if (cs->flex_grow > 0) {
                        CSS_SET_FIXED_LENGTH(
                            cs, height,
                            cs->flex_basis + flex_space * cs->flex_grow);
                }
                if (cs->type_bits.margin_top == CSS_MARGIN_AUTO) {
                        CSS_SET_FIXED_LENGTH(cs, margin_top, margin_space);
                }
                if (cs->type_bits.margin_bottom == CSS_MARGIN_AUTO) {
                        CSS_SET_FIXED_LENGTH(cs, margin_bottom, margin_space);
                }
        } else if (cs->flex_shrink > 0) {
                CSS_SET_FIXED_LENGTH(
                    cs, height, cs->flex_basis + flex_space * cs->flex_shrink);
        }
        ui_widget_reflow_if_height_changed(item);
}

static void ui_flexbox_row_layout_apply_main_size(
    ui_flexbox_layout_context_t *ctx)
{
        list_node_t *node, *line_node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;

        float main_size;
        float max_main_size = ctx->widget->content_box.width;
        float space, flex_space, margin_space;

#ifdef UI_DEBUG_ENABLED
        unsigned child_index = 0;

        {
                UI_DEBUG_MSG("%s: begin, max_main_size=%g", __FUNCTION__,
                             max_main_size);
                ui_debug_msg_indent++;
        }
#endif

        line = ui_flexbox_layout_next_line(ctx);
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                if (!ui_widget_in_layout_flow(child)) {
#ifdef UI_DEBUG_ENABLED
                        {
                                UI_WIDGET_STR(child, str);
                                UI_DEBUG_MSG("line[%zu]: children[%u]=%s, skip",
                                             line->index, child_index, str);
                        }
                        child_index++;
#endif
                        list_append(&line->items, child);
                        continue;
                }
                cs = &child->computed_style;
                ui_widget_reset_width(child);
                ui_reset_row_item_flex_basis(child);
                ui_compute_row_item_flex_basis(child);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("line[%zu]: children[%u]=%s, size=%s, "
                                     "flex_basis(%d)=%g",
                                     line->index, child_index, str, size_str,
                                     cs->type_bits.flex_basis, cs->flex_basis);
                }
#endif
                main_size = cs->flex_basis + css_margin_x(cs);
                if (!is_css_border_box_sizing(cs)) {
                        main_size += css_padding_x(cs) + css_border_x(cs);
                }
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("line[%zu]: children[%u]=%s, size=%s, "
                                     "flex_basis(%d)=%g, main_size=%g, "
                                     "outer_box.width=%g",
                                     line->index, child_index, str, size_str,
                                     cs->type_bits.flex_basis, cs->flex_basis,
                                     main_size, child->outer_box.width);
                }
                child_index++;
#endif
                if (s->type_bits.flex_wrap == CSS_FLEX_WRAP_WRAP &&
                    line->main_size > 0 &&
                    line->main_size + main_size > max_main_size) {
                        line = ui_flexbox_layout_next_line(ctx);
                }
                line->main_size += main_size;
                if (cs->type_bits.margin_left == CSS_MARGIN_AUTO) {
                        line->count_of_auto_margin_items++;
                }
                if (cs->type_bits.margin_right == CSS_MARGIN_AUTO) {
                        line->count_of_auto_margin_items++;
                }
                ui_flexbox_line_load_item(line, child);
        }
        ctx->cross_size = 0;
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: all lines loaded", __FUNCTION__);
        ui_debug_msg_indent++;
#endif
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                space = max_main_size - line->main_size;
                flex_space = 0;
                margin_space = 0;
                if (space >= 0) {
                        if (line->sum_of_grow_value > 0) {
                                flex_space = space / line->sum_of_grow_value;
                        } else if (line->count_of_auto_margin_items > 0) {
                                margin_space =
                                    space / line->count_of_auto_margin_items;
                        }
                } else if (line->sum_of_shrink_value > 0) {
                        flex_space = space / line->sum_of_shrink_value;
                }
#ifdef UI_DEBUG_ENABLED
                UI_DEBUG_MSG("line[%zu]: main_size=%g, space=%g, "
                             "flex_space=%g, margin_space=%g",
                             line->index, line->main_size, space, flex_space,
                             margin_space);
                ui_debug_msg_indent++;
                child_index = 0;
#endif
                line->main_size = 0;
                line->cross_size = 0;
                for (list_each(node, &line->items)) {
                        child = node->data;
                        if (!ui_widget_in_layout_flow(child)) {
                                continue;
                        }
#ifdef UI_DEBUG_ENABLED
                        {
                                UI_WIDGET_STR(child, str);
                                UI_WIDGET_SIZE_STR(child, size_str);
                                UI_DEBUG_MSG("[%zu] %s: size=%s, "
                                             "flex-shrink=%g, flex-grow=%g, "
                                             "content_box_size=(%g, %g), "
                                             "outer_box_size=(%g, %g)",
                                             child_index, str, size_str,
                                             child->computed_style.flex_shrink,
                                             child->computed_style.flex_grow,
                                             child->content_box.width,
                                             child->content_box.height,
                                             child->outer_box.width,
                                             child->outer_box.height);
                                ui_debug_msg_indent++;
                        }
#endif
                        ui_apply_row_item_main_size(child, space, flex_space,
                                                    margin_space);
#ifdef UI_DEBUG_ENABLED
                        {
                                ui_debug_msg_indent--;
                                UI_WIDGET_STR(child, str);
                                UI_WIDGET_SIZE_STR(child, size_str);
                                UI_DEBUG_MSG(
                                    "[%zu] %s: size=%s, main_size=%g, "
                                    "cross_size=%g, content_box_size=(%g, %g)",
                                    child_index, str, size_str,
                                    child->outer_box.width,
                                    child->outer_box.height,
                                    child->content_box.width,
                                    child->content_box.height);
                        }
                        child_index++;
#endif
                        if (line->cross_size < child->outer_box.height) {
                                line->cross_size = child->outer_box.height;
                        }
                        line->main_size += child->outer_box.width;
                }
#ifdef UI_DEBUG_ENABLED
                --ui_debug_msg_indent;
                UI_DEBUG_MSG("line[%zu]: cross_size=%g", line->index,
                             line->cross_size);
#endif
                ctx->cross_size += line->cross_size;
        }
        if (ctx->cross_size < ctx->sizehint.max_height) {
                ctx->cross_size = ctx->sizehint.max_height;
        }
        if (!IS_CSS_FIXED_LENGTH(s, height)) {
                ui_widget_set_content_height(ctx->widget, ctx->cross_size);
        }
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: end, cross_size=%g", __FUNCTION__, ctx->cross_size);
#endif
}

static void ui_flexbox_column_layout_apply_main_size(
    ui_flexbox_layout_context_t *ctx)
{
        list_node_t *node, *line_node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;

        float main_size;
        float max_main_size = ctx->widget->content_box.height;
        float space, flex_space, margin_space;

#ifdef UI_DEBUG_ENABLED
        unsigned child_index = 0;

        {
                UI_DEBUG_MSG("%s: begin, max_main_size=%g", __FUNCTION__,
                             max_main_size);
                ui_debug_msg_indent++;
        }
#endif
        line = ui_flexbox_layout_next_line(ctx);
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                if (!ui_widget_in_layout_flow(child)) {
#ifdef UI_DEBUG_ENABLED
                        {
                                UI_WIDGET_STR(child, str);
                                UI_DEBUG_MSG("line[%zu]: children[%u]=%s, skip",
                                             line->index, child_index, str);
                        }
                        child_index++;
#endif
                        list_append(&line->items, child);
                        continue;
                }
                cs = &child->computed_style;
                ui_widget_reset_height(child);
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("line[%zu]: children[%u]=%s, size=%s, "
                                     "flex_basis(%d)=%g",
                                     line->index, child_index, str, size_str,
                                     cs->type_bits.flex_basis, cs->flex_basis);
                }
#endif
                ui_compute_column_item_flex_basis(child);
                main_size = cs->flex_basis + css_margin_y(cs);
                if (cs->type_bits.box_sizing == CSS_BOX_SIZING_CONTENT_BOX) {
                        main_size += css_padding_y(cs) + css_border_y(cs);
                }
#ifdef UI_DEBUG_ENABLED
                {
                        UI_WIDGET_STR(child, str);
                        UI_WIDGET_SIZE_STR(child, size_str);
                        UI_DEBUG_MSG("line[%zu]: children[%u]=%s, size=%s, "
                                     "flex_basis(%d)=%g, main_size=%g, "
                                     "child->outer_box.height=%g",
                                     line->index, child_index, str, size_str,
                                     cs->type_bits.flex_basis, cs->flex_basis,
                                     main_size, child->outer_box.height);
                }
                child_index++;
#endif
                if (s->type_bits.flex_wrap == CSS_FLEX_WRAP_WRAP &&
                    line->main_size > 0 &&
                    line->main_size + main_size > max_main_size) {
                        line = ui_flexbox_layout_next_line(ctx);
                }
                line->main_size += main_size;
                if (cs->type_bits.margin_top == CSS_MARGIN_AUTO) {
                        line->count_of_auto_margin_items++;
                }
                if (cs->type_bits.margin_bottom == CSS_MARGIN_AUTO) {
                        line->count_of_auto_margin_items++;
                }
                ui_flexbox_line_load_item(line, child);
        }
        ctx->cross_size = 0;
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: all lines loaded", __FUNCTION__);
        ui_debug_msg_indent++;
#endif
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                space = max_main_size - line->main_size;
                flex_space = 0;
                margin_space = 0;
                if (space >= 0) {
                        if (line->sum_of_grow_value > 0) {
                                flex_space = space / line->sum_of_grow_value;
                        } else if (line->count_of_auto_margin_items > 0) {
                                margin_space =
                                    space / line->count_of_auto_margin_items;
                        }
                } else if (line->sum_of_shrink_value > 0) {
                        flex_space = space / line->sum_of_shrink_value;
                }
#ifdef UI_DEBUG_ENABLED
                UI_DEBUG_MSG("line[%zu]: main_size=%g, space=%g, "
                             "flex_space=%g, margin_space=%g",
                             line->index, line->main_size, space, flex_space,
                             margin_space);
                ui_debug_msg_indent++;
                child_index = 0;
#endif
                line->main_size = 0;
                line->cross_size = 0;
                for (list_each(node, &line->items)) {
                        child = node->data;
                        if (!ui_widget_in_layout_flow(child)) {
                                continue;
                        }
#ifdef UI_DEBUG_ENABLED
                        {
                                UI_WIDGET_STR(child, str);
                                UI_WIDGET_SIZE_STR(child, size_str);
                                UI_DEBUG_MSG("[%zu] %s: size=%s, "
                                             "flex-shrink=%g, flex-grow=%g",
                                             child_index, str, size_str,
                                             child->computed_style.flex_shrink,
                                             child->computed_style.flex_grow);
                                ui_debug_msg_indent++;
                        }
#endif
                        ui_apply_column_item_main_size(child, space, flex_space,
                                                       margin_space);
#ifdef UI_DEBUG_ENABLED
                        {
                                ui_debug_msg_indent--;
                                UI_WIDGET_STR(child, str);
                                UI_WIDGET_SIZE_STR(child, size_str);
                                UI_DEBUG_MSG("[%zu] %s: size=%s, main_size=%g, "
                                             "cross_size=%g",
                                             child_index, str, size_str,
                                             child->outer_box.height,
                                             child->outer_box.width);
                        }
                        child_index++;
#endif
                        if (line->cross_size < child->outer_box.width) {
                                line->cross_size = child->outer_box.width;
                        }
                        line->main_size += child->outer_box.height;
                }
#ifdef UI_DEBUG_ENABLED
                --ui_debug_msg_indent;
                UI_DEBUG_MSG("line[%zu]: cross_size=%g", line->index,
                             line->cross_size);
#endif
                ctx->cross_size += line->cross_size;
        }
        if (ctx->cross_size < ctx->sizehint.max_width) {
                ctx->cross_size = ctx->sizehint.max_width;
        }
        if (!IS_CSS_FIXED_LENGTH(s, width)) {
                ui_widget_set_content_width(ctx->widget, ctx->cross_size);
        }
#ifdef UI_DEBUG_ENABLED
        ui_debug_msg_indent--;
        UI_DEBUG_MSG("%s: end, cross_size=%g", __FUNCTION__, ctx->cross_size);
#endif
}

static float ui_compute_row_item_layout(ui_widget_t *item, float x, float y,
                                        css_align_items_t align,
                                        float line_max_cross_size)
{
        css_computed_style_t *s = &item->computed_style;

        if (!ui_widget_in_layout_flow(item)) {
                item->layout_x = x;
                item->layout_y = y;
                ui_widget_reset_layout(item);
                ui_widget_reflow_if_width_changed(item);
                ui_widget_reflow_if_height_changed(item);
                ui_widget_update_box_position(item);
                return 0;
        }
        if (align == CSS_ALIGN_ITEMS_STRETCH &&
            item->specified_style.type_bits.height == CSS_HEIGHT_AUTO) {
                CSS_SET_FIXED_LENGTH(
                    s, height,
                    css_border_box_height_to_height(
                        s, line_max_cross_size - css_margin_y(s)));
        }
        ui_widget_reflow_if_height_changed(item);
        switch (align) {
        case CSS_ALIGN_ITEMS_CENTER:
                y += (line_max_cross_size - item->outer_box.height) * 0.5f;
                break;
        case CSS_ALIGN_ITEMS_FLEX_END:
                y += line_max_cross_size - item->outer_box.height;
                break;
        default:
                break;
        }
#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(item, str);
                UI_DEBUG_MSG("%s: %s: x=%g, y=%g", __FUNCTION__, str, x, y);
        }
#endif
        item->layout_x = x;
        item->layout_y = y;
        ui_widget_update_box_position(item);
        return item->outer_box.width;
}

static float ui_compute_column_item_layout(ui_widget_t *item, float x, float y,
                                           css_align_items_t align,
                                           float line_max_cross_size)
{
        css_computed_style_t *s = &item->computed_style;
        if (!ui_widget_in_layout_flow(item)) {
                item->layout_x = x;
                item->layout_y = y;
                ui_widget_reset_layout(item);
                ui_widget_reflow_if_width_changed(item);
                ui_widget_reflow_if_height_changed(item);
                ui_widget_update_box_position(item);
                return 0;
        }
        if (align == CSS_ALIGN_ITEMS_STRETCH &&
            item->specified_style.type_bits.width == CSS_WIDTH_AUTO) {
                CSS_SET_FIXED_LENGTH(
                    s, width,
                    css_border_box_width_to_width(
                        s, line_max_cross_size - css_margin_x(s)));
        }
        ui_widget_reflow_if_width_changed(item);
        switch (align) {
        case CSS_ALIGN_ITEMS_CENTER:
                x += (line_max_cross_size - item->outer_box.width) * 0.5f;
                break;
        case CSS_ALIGN_ITEMS_FLEX_END:
                x += line_max_cross_size - item->outer_box.width;
                break;
        default:
                break;
        }
#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(item, str);
                UI_DEBUG_MSG("%s: %s: x=%g, y=%g", __FUNCTION__, str, x, y);
        }
#endif
        item->layout_x = x;
        item->layout_y = y;
        ui_widget_update_box_position(item);
        return item->outer_box.height;
}

static void ui_flexbox_row_layout_reflow(ui_flexbox_layout_context_t *ctx)
{
        list_node_t *line_node, *node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;

        float space;
        float main_axis;
        float cross_axis = s->padding_top;
        float cross_space =
            (ctx->widget->content_box.height - ctx->cross_size) /
            ctx->lines.length;
        float line_max_cross_size;

#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(ctx->widget, str);
                UI_DEBUG_MSG("%s: %s: begin, cross_space=%g", __FUNCTION__, str,
                             cross_space);
        }
#endif
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                line_max_cross_size = line->cross_size + cross_space;
                ui_flexbox_layout_compute_justify_content(ctx, line, &main_axis,
                                                          &space);
#ifdef UI_DEBUG_ENABLED
                UI_DEBUG_MSG("line[%zu]: main_axis=%g, cross_axis=%g, "
                             "space=%g, line_max_cross_size=%g",
                             line->index, main_axis, cross_axis, space,
                             line_max_cross_size);
                ui_debug_msg_indent++;
#endif
                for (list_each(node, &line->items)) {
                        child = node->data;
                        main_axis += space;
                        main_axis += ui_compute_row_item_layout(
                            child, main_axis, cross_axis,
                            s->type_bits.align_items, line_max_cross_size);
                }
                cross_axis += line_max_cross_size;
#ifdef UI_DEBUG_ENABLED
                ui_debug_msg_indent--;
#endif
        }
#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(ctx->widget, str);
                UI_DEBUG_MSG("%s: %s: end", __FUNCTION__, str);
        }
#endif
}

static void ui_flexbox_column_layout_reflow(ui_flexbox_layout_context_t *ctx)
{
        list_node_t *line_node, *node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;

        float space;
        float main_axis;
        float cross_axis = s->padding_left;
        float cross_space = (ctx->widget->content_box.width - ctx->cross_size) /
                            ctx->lines.length;
        float line_max_cross_size;

#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(ctx->widget, str);
                UI_DEBUG_MSG("%s: %s: begin, cross_space=%g", __FUNCTION__, str,
                             cross_space);
        }
#endif
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                line_max_cross_size = line->cross_size + cross_space;
                ui_flexbox_layout_compute_justify_content(ctx, line, &main_axis,
                                                          &space);
#ifdef UI_DEBUG_ENABLED
                UI_DEBUG_MSG("line[%zu]: main_axis=%g, cross_axis=%g, "
                             "space=%g, line_max_cross_size=%g",
                             line->index, main_axis, cross_axis, space,
                             line_max_cross_size);
                ui_debug_msg_indent++;
#endif
                for (list_each(node, &line->items)) {
                        child = node->data;
                        main_axis += space;
                        main_axis += ui_compute_column_item_layout(
                            child, cross_axis, main_axis,
                            s->type_bits.align_items, line_max_cross_size);
                }
                cross_axis += line_max_cross_size;
#ifdef UI_DEBUG_ENABLED
                ui_debug_msg_indent--;
#endif
        }
#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(ctx->widget, str);
                UI_DEBUG_MSG("%s: %s: end", __FUNCTION__, str);
        }
#endif
}

void ui_flexbox_layout_reflow(ui_widget_t *w)
{
        ui_flexbox_layout_context_t ctx = { 0 };

        ctx.widget = w;
        ctx.column_direction = ui_widget_has_flex_column_direction(w);
        list_create(&ctx.lines);
        ui_widget_get_sizehint(w, &ctx.sizehint);
#ifdef UI_DEBUG_ENABLED
        {
                UI_WIDGET_STR(w, str);
                UI_WIDGET_SIZE_STR(w, size_str);
                UI_DEBUG_MSG("%s: %s: begin, direction=%s, size=%s, "
                             "hint_content_size=(%g,%g)",
                             __FUNCTION__, str,
                             ctx.column_direction ? "column" : "row", size_str,
                             ctx.sizehint.min_width,
                             ctx.sizehint.min_height);
                ui_debug_msg_indent++;
        }
#endif

        if (ctx.column_direction) {
                ui_flexbox_column_layout_load_main_size(&ctx);
                ui_flexbox_column_layout_apply_main_size(&ctx);
                ui_flexbox_column_layout_reflow(&ctx);
        } else {
                ui_flexbox_row_layout_load_main_size(&ctx);
                ui_flexbox_row_layout_apply_main_size(&ctx);
                ui_flexbox_row_layout_reflow(&ctx);
        }
        w->proto->resize(w, w->content_box.width, w->content_box.height);
        list_destroy(&ctx.lines, ui_flexbox_line_destroy);

#ifdef UI_DEBUG_ENABLED
        {
                ui_debug_msg_indent--;
                UI_WIDGET_STR(w, str);
                UI_WIDGET_SIZE_STR(w, size_str);
                UI_DEBUG_MSG("%s: %s: end, size=%s", __FUNCTION__, str,
                             size_str);
        }
#endif
}
