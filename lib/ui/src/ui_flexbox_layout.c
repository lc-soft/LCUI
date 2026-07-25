/*
 * lib/ui/src/ui_flexbox_layout.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
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
#include "ui_resizer.h"

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

typedef struct ui_flexbox_layout_context {
        ui_widget_t *widget;
        bool column_direction;
        ui_resizer_t *resizer;
        float cross_size;
        float main_gap;
        float cross_gap;

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

/* 将 gap 值解析为像素长度，percentage 引用容器对应轴上的内容尺寸 */
static float ui_flexbox_resolve_gap(uint8_t type, css_numeric_value_t value,
                                    css_unit_t unit, float reference)
{
        if (type != CSS_GAP_SET) {
                return 0;
        }
        if (unit == CSS_UNIT_PERCENT) {
                return value * reference / 100.f;
        }
        return value;
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
        s->flex_basis = ui_widget_fix_width(item, s->flex_basis);
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
        s->flex_basis = ui_widget_fix_height(item, s->flex_basis);
}

static void ui_compute_row_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;

        if (IS_CSS_PERCENTAGE(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    s->flex_basis * item->parent->content_box.width / 100);
        } else if (!IS_CSS_FIXED_LENGTH(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    css_width_from_cbox(s, item->max_content_width));
        }
        s->flex_basis = ui_widget_fix_width(item, s->flex_basis);
}

static void ui_compute_column_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;

        if (IS_CSS_PERCENTAGE(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    s->flex_basis * item->parent->content_box.height / 100);
        } else if (!IS_CSS_FIXED_LENGTH(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    css_height_from_cbox(s, item->max_content_height));
        }
        s->flex_basis = ui_widget_fix_height(item, s->flex_basis);
}

/* 重置项目主轴方向上的尺寸与 flex_basis，返回其外框主轴尺寸 */
static float ui_flexbox_item_reset_main_size(ui_flexbox_layout_context_t *ctx,
                                             ui_widget_t *item)
{
        css_computed_style_t *cs = &item->computed_style;

        if (ctx->column_direction) {
                ui_widget_reset_height(item);
                ui_reset_column_item_flex_basis(item);
                ui_compute_column_item_flex_basis(item);
                return css_obox_height(cs, cs->flex_basis);
        }
        ui_widget_reset_width(item);
        ui_reset_row_item_flex_basis(item);
        ui_compute_row_item_flex_basis(item);
        return css_obox_width(cs, cs->flex_basis);
}

/* 返回项目主轴方向上基于 min_content_* 的最小外框尺寸 */
static float ui_flexbox_item_min_main_size(ui_flexbox_layout_context_t *ctx,
                                           ui_widget_t *item)
{
        css_computed_style_t *cs = &item->computed_style;

        if (ctx->column_direction) {
                return css_obox_height(
                    cs, css_height_from_cbox(cs, item->min_content_height));
        }
        return css_obox_width(cs,
                              css_width_from_cbox(cs, item->min_content_width));
}

static float ui_flexbox_item_max_main_size(ui_flexbox_layout_context_t *ctx,
                                           ui_widget_t *item)
{
        css_computed_style_t *cs = &item->computed_style;

        if (ctx->column_direction) {
                return css_obox_height(
                    cs, css_height_from_cbox(cs, item->max_content_height));
        }
        return css_obox_width(cs,
                              css_width_from_cbox(cs, item->max_content_width));
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

static void ui_apply_row_item_main_size(ui_widget_t *item, float flex_space,
                                        float margin_space)
{
        css_computed_style_t *cs = &item->computed_style;

        CSS_SET_FIXED_LENGTH(cs, width, cs->flex_basis);
        if (flex_space >= 0) {
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

static void ui_apply_column_item_main_size(ui_widget_t *item, float flex_space,
                                           float margin_space)
{
        css_computed_style_t *cs = &item->computed_style;

        CSS_SET_FIXED_LENGTH(cs, height, cs->flex_basis);
        if (flex_space >= 0) {
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

static void ui_flexbox_layout_load_main_size(ui_flexbox_layout_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *cs;
        float main_size, min_main_size;
        unsigned child_index = 0;
        unsigned in_flow_count = 0;

        UI_DEBUG_BEGIN;
        UI_DEBUG_MSG(
            "%s: begin, main_size_fixed?=%d", __FUNCTION__,
            ctx->column_direction
                ? IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, height)
                : IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, width));
        UI_DEBUG_INDENT_INC;
        UI_DEBUG_END;
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                ui_widget_reset_layout(child);
                main_size = ui_flexbox_item_reset_main_size(ctx, child);
                main_size =
                    y_max(main_size, ui_flexbox_item_max_main_size(ctx, child));
                min_main_size = cs->flex_shrink > 0
                                    ? ui_flexbox_item_min_main_size(ctx, child)
                                    : main_size;
                if (in_flow_count > 0 && ctx->main_gap > 0) {
                        ui_resizer_load_item_main_size(
                            ctx->resizer, ctx->main_gap, ctx->main_gap);
                }
                ui_resizer_load_item_main_size(ctx->resizer, main_size,
                                               min_main_size);
                in_flow_count++;
                UI_DEBUG_BEGIN;
                UI_WIDGET_STR(child, str);
                UI_WIDGET_SIZE_STR(child, size_str);
                UI_DEBUG_MSG("children[%u]=%s, size=%s, flex_basis(%d)=%g",
                             child_index, str, size_str,
                             cs->type_bits.flex_basis, cs->flex_basis);
                child_index++;
                UI_DEBUG_END;
        }
        if (ctx->column_direction) {
                ui_resizer_commit_column_main_size(ctx->resizer);
        } else {
                ui_resizer_commit_row_main_size(ctx->resizer);
        }
        UI_DEBUG_BEGIN;
        UI_DEBUG_INDENT_DEC;
        UI_WIDGET_SIZE_STR(ctx->widget, size_str);
        UI_DEBUG_MSG("%s: end, size=%s", __FUNCTION__, size_str);
        UI_DEBUG_END;
}

static void ui_flexbox_layout_apply_line(ui_flexbox_layout_context_t *ctx,
                                         ui_flexbox_line_t *line,
                                         float flex_space, float margin_space)
{
        list_node_t *node;
        ui_widget_t *child;
        float item_main_size, item_cross_size;
        unsigned child_index = 0;
        unsigned in_flow_count = 0;

        line->main_size = 0;
        line->cross_size = 0;
        for (list_each(node, &line->items)) {
                child = node->data;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                UI_DEBUG_BEGIN;
                UI_WIDGET_STR(child, str);
                UI_WIDGET_SIZE_STR(child, size_str);
                UI_DEBUG_MSG("[%u] %s: size=%s, flex-shrink=%g, "
                             "flex-grow=%g",
                             child_index, str, size_str,
                             child->computed_style.flex_shrink,
                             child->computed_style.flex_grow);
                UI_DEBUG_INDENT_INC;
                UI_DEBUG_END;
                if (ctx->column_direction) {
                        ui_apply_column_item_main_size(child, flex_space,
                                                       margin_space);
                        item_main_size = child->outer_box.height;
                        if (IS_CSS_FIXED_LENGTH(&child->computed_style,
                                                width)) {
                                item_cross_size = child->outer_box.width;
                        } else {
                                item_cross_size = css_obox_width(
                                    &child->computed_style,
                                    css_width_from_cbox(
                                        &child->computed_style,
                                        child->max_content_width));
                        }
                } else {
                        ui_apply_row_item_main_size(child, flex_space,
                                                    margin_space);
                        item_main_size = child->outer_box.width;
                        item_cross_size = child->outer_box.height;
                }
                UI_DEBUG_BEGIN;
                UI_DEBUG_INDENT_DEC;
                UI_WIDGET_STR(child, str);
                UI_WIDGET_SIZE_STR(child, size_str);
                UI_DEBUG_MSG("[%u] %s: size=%s, main_size=%g, "
                             "cross_size=%g",
                             child_index, str, size_str, item_main_size,
                             item_cross_size);
                child_index++;
                UI_DEBUG_END;
                if (line->cross_size < item_cross_size) {
                        line->cross_size = item_cross_size;
                }
                if (in_flow_count > 0) {
                        line->main_size += ctx->main_gap;
                }
                line->main_size += item_main_size;
                in_flow_count++;
        }
}

static void ui_flexbox_layout_apply_main_size(ui_flexbox_layout_context_t *ctx)
{
        list_node_t *node, *line_node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;
        bool column = ctx->column_direction;

        float main_size;
        float max_main_size = column ? ctx->widget->content_box.height
                                     : ctx->widget->content_box.width;
        float hint_cross_size = column ? ctx->resizer->hint.max_width
                                       : ctx->resizer->hint.max_height;
        float space, flex_space, margin_space;
        unsigned child_index = 0;

        UI_DEBUG_BEGIN;
        UI_DEBUG_MSG("%s: begin, max_main_size=%g", __FUNCTION__,
                     max_main_size);
        UI_DEBUG_INDENT_INC;
        UI_DEBUG_END;
        line = ui_flexbox_layout_next_line(ctx);
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                if (!ui_widget_in_layout_flow(child)) {
                        UI_DEBUG_BEGIN;
                        UI_WIDGET_STR(child, str);
                        UI_DEBUG_MSG("line[%zu]: children[%u]=%s, skip",
                                     line->index, child_index, str);
                        child_index++;
                        UI_DEBUG_END;
                        list_append(&line->items, child);
                        continue;
                }
                cs = &child->computed_style;
                main_size = ui_flexbox_item_reset_main_size(ctx, child);
                UI_DEBUG_BEGIN;
                UI_WIDGET_STR(child, str);
                UI_WIDGET_SIZE_STR(child, size_str);
                UI_DEBUG_MSG("line[%zu]: children[%u]=%s, size=%s, "
                             "flex_basis(%d)=%g, main_size=%g",
                             line->index, child_index, str, size_str,
                             cs->type_bits.flex_basis, cs->flex_basis,
                             main_size);
                child_index++;
                UI_DEBUG_END;
                if (s->type_bits.flex_wrap == CSS_FLEX_WRAP_WRAP &&
                    line->main_size > 0 &&
                    line->main_size + ctx->main_gap + main_size >
                        max_main_size) {
                        line = ui_flexbox_layout_next_line(ctx);
                }
                if (line->items.length > 0) {
                        line->main_size += ctx->main_gap;
                }
                line->main_size += main_size;
                if (column) {
                        if (cs->type_bits.margin_top == CSS_MARGIN_AUTO) {
                                line->count_of_auto_margin_items++;
                        }
                        if (cs->type_bits.margin_bottom == CSS_MARGIN_AUTO) {
                                line->count_of_auto_margin_items++;
                        }
                } else {
                        if (cs->type_bits.margin_left == CSS_MARGIN_AUTO) {
                                line->count_of_auto_margin_items++;
                        }
                        if (cs->type_bits.margin_right == CSS_MARGIN_AUTO) {
                                line->count_of_auto_margin_items++;
                        }
                }
                ui_flexbox_line_load_item(line, child);
        }
        ctx->cross_size = 0;
        UI_DEBUG_BEGIN;
        UI_DEBUG_MSG("%s: all lines loaded", __FUNCTION__);
        UI_DEBUG_END;
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
                UI_DEBUG_BEGIN;
                UI_DEBUG_MSG("line[%zu]: main_size=%g, space=%g, "
                             "flex_space=%g, margin_space=%g",
                             line->index, line->main_size, space, flex_space,
                             margin_space);
                UI_DEBUG_INDENT_INC;
                UI_DEBUG_END;
                ui_flexbox_layout_apply_line(ctx, line, flex_space,
                                             margin_space);
                UI_DEBUG_BEGIN;
                UI_DEBUG_INDENT_DEC;
                UI_DEBUG_MSG("line[%zu]: cross_size=%g", line->index,
                             line->cross_size);
                UI_DEBUG_END;
                if (line->index > 0) {
                        ctx->cross_size += ctx->cross_gap;
                }
                ctx->cross_size += line->cross_size;
        }
        if (ctx->cross_size < hint_cross_size) {
                ctx->cross_size = hint_cross_size;
        }
        if (column) {
                if (!IS_CSS_FIXED_LENGTH(s, width)) {
                        ui_widget_set_content_width(ctx->widget,
                                                    ctx->cross_size);
                }
        } else {
                if (!IS_CSS_FIXED_LENGTH(s, height)) {
                        ui_widget_set_content_height(ctx->widget,
                                                     ctx->cross_size);
                }
        }
        UI_DEBUG_BEGIN;
        UI_DEBUG_INDENT_DEC;
        UI_DEBUG_MSG("%s: end, cross_size=%g", __FUNCTION__, ctx->cross_size);
        UI_DEBUG_END;
}

static float ui_flexbox_compute_item_layout(ui_flexbox_layout_context_t *ctx,
                                            ui_widget_t *item, float main_axis,
                                            float cross_axis,
                                            css_align_items_t align,
                                            float line_max_cross_size)
{
        css_computed_style_t *s = &item->computed_style;
        bool column = ctx->column_direction;
        float item_outer_cross;

        if (!ui_widget_in_layout_flow(item)) {
                item->layout_x = column ? cross_axis : main_axis;
                item->layout_y = column ? main_axis : cross_axis;
                ui_widget_reset_layout(item);
                ui_widget_reflow_if_width_changed(item);
                ui_widget_reflow_if_height_changed(item);
                ui_widget_update_box_position(item);
                return 0;
        }
        if (column) {
                if (IS_CSS_PERCENTAGE(&item->specified_style, width)) {
                        ui_widget_reset_width(item);
                } else if (align == CSS_ALIGN_ITEMS_STRETCH &&
                           item->specified_style.type_bits.width ==
                               CSS_WIDTH_AUTO) {
                        CSS_SET_FIXED_LENGTH(
                            s, width,
                            css_border_box_width_to_width(
                                s, line_max_cross_size - css_margin_x(s)));
                }
                ui_widget_reflow_if_width_changed(item);
                item_outer_cross = item->outer_box.width;
        } else {
                if (IS_CSS_PERCENTAGE(&item->specified_style, height)) {
                        ui_widget_reset_height(item);
                } else if (align == CSS_ALIGN_ITEMS_STRETCH &&
                           item->specified_style.type_bits.height ==
                               CSS_HEIGHT_AUTO) {
                        CSS_SET_FIXED_LENGTH(
                            s, height,
                            css_border_box_height_to_height(
                                s, line_max_cross_size - css_margin_y(s)));
                }
                ui_widget_reflow_if_height_changed(item);
                item_outer_cross = item->outer_box.height;
        }
        switch (align) {
        case CSS_ALIGN_ITEMS_CENTER:
                cross_axis += (line_max_cross_size - item_outer_cross) * 0.5f;
                break;
        case CSS_ALIGN_ITEMS_FLEX_END:
                cross_axis += line_max_cross_size - item_outer_cross;
                break;
        default:
                break;
        }
        item->layout_x = column ? cross_axis : main_axis;
        item->layout_y = column ? main_axis : cross_axis;
        UI_DEBUG_BEGIN;
        UI_WIDGET_STR(item, str);
        UI_DEBUG_MSG("%s: %s: x=%g, y=%g", __FUNCTION__, str, item->layout_x,
                     item->layout_y);
        UI_DEBUG_END;
        ui_widget_update_box_position(item);
        return column ? item->outer_box.height : item->outer_box.width;
}

static void ui_flexbox_layout_reflow_lines(ui_flexbox_layout_context_t *ctx)
{
        list_node_t *line_node, *node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;
        bool column = ctx->column_direction;
        unsigned item_index;

        float space;
        float main_axis;
        float cross_axis = column ? s->padding_left : s->padding_top;
        float cross_free_space = (column ? ctx->widget->content_box.width
                                         : ctx->widget->content_box.height) -
                                 ctx->cross_size;
        float cross_space =
            ctx->lines.length > 0 ? cross_free_space / ctx->lines.length : 0;
        float line_max_cross_size;

        UI_DEBUG_BEGIN;
        UI_WIDGET_STR(ctx->widget, str);
        UI_DEBUG_MSG("%s: %s: begin, cross_space=%g", __FUNCTION__, str,
                     cross_space);
        UI_DEBUG_END;
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                if (line->index > 0) {
                        cross_axis += ctx->cross_gap;
                }
                line_max_cross_size = line->cross_size + cross_space;
                ui_flexbox_layout_compute_justify_content(ctx, line, &main_axis,
                                                          &space);
                UI_DEBUG_BEGIN;
                UI_DEBUG_MSG("line[%zu]: main_axis=%g, cross_axis=%g, "
                             "space=%g, line_max_cross_size=%g",
                             line->index, main_axis, cross_axis, space,
                             line_max_cross_size);
                UI_DEBUG_INDENT_INC;
                UI_DEBUG_END;
                item_index = 0;
                for (list_each(node, &line->items)) {
                        child = node->data;
                        main_axis += space;
                        if (item_index > 0 && ui_widget_in_layout_flow(child)) {
                                main_axis += ctx->main_gap;
                        }
                        main_axis += ui_flexbox_compute_item_layout(
                            ctx, child, main_axis, cross_axis,
                            s->type_bits.align_items, line_max_cross_size);
                        if (ui_widget_in_layout_flow(child)) {
                                item_index++;
                        }
                }
                cross_axis += line_max_cross_size;
                UI_DEBUG_BEGIN;
                UI_DEBUG_INDENT_DEC;
                UI_DEBUG_END;
        }
        UI_DEBUG_BEGIN;
        UI_WIDGET_STR(ctx->widget, str);
        UI_DEBUG_MSG("%s: %s: end", __FUNCTION__, str);
        UI_DEBUG_END;
}

void ui_flexbox_layout_reflow(ui_widget_t *w, ui_resizer_t *resizer)
{
        ui_flexbox_layout_context_t ctx = { 0 };

        ctx.widget = w;
        ctx.resizer = resizer;
        ctx.column_direction = ui_widget_has_flex_column_direction(w);
        list_create(&ctx.lines);
        ui_resizer_init(resizer, w);
        {
                css_computed_style_t *s = &w->computed_style;
                float row_gap = ui_flexbox_resolve_gap(
                    s->type_bits.row_gap, s->row_gap, s->unit_bits.row_gap,
                    w->content_box.height);
                float column_gap = ui_flexbox_resolve_gap(
                    s->type_bits.column_gap, s->column_gap,
                    s->unit_bits.column_gap, w->content_box.width);
                if (ctx.column_direction) {
                        ctx.main_gap = row_gap;
                        ctx.cross_gap = column_gap;
                } else {
                        ctx.main_gap = column_gap;
                        ctx.cross_gap = row_gap;
                }
        }
        UI_DEBUG_BEGIN;
        UI_WIDGET_STR(w, str);
        UI_WIDGET_SIZE_STR(w, size_str);
        UI_DEBUG_MSG("%s: %s: begin, direction=%s, size=%s, "
                     "content_size=(%g, %g)",
                     __FUNCTION__, str, ctx.column_direction ? "column" : "row",
                     size_str, w->content_box.width, w->content_box.height);
        UI_DEBUG_INDENT_INC;
        UI_DEBUG_END;
        if (ctx.column_direction) {
                ui_resizer_load_column_minmaxinfo(resizer);
        } else {
                ui_resizer_load_row_minmaxinfo(resizer);
        }
        ui_flexbox_layout_load_main_size(&ctx);
        ui_flexbox_layout_apply_main_size(&ctx);
        ui_flexbox_layout_reflow_lines(&ctx);
        w->proto->resize(w, w->content_box.width, w->content_box.height);
        list_destroy(&ctx.lines, ui_flexbox_line_destroy);
        UI_DEBUG_BEGIN;
        UI_DEBUG_INDENT_DEC;
        UI_WIDGET_STR(w, str);
        UI_WIDGET_SIZE_STR(w, size_str);
        UI_DEBUG_MSG("%s: %s: end, size=%s", __FUNCTION__, str, size_str);
        UI_DEBUG_END;
}
