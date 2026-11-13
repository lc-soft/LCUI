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
#include <float.h>
#include <math.h>
#include "ui_diff.h"
#include "ui_debug.h"
#include "ui_widget_style.h"
#include "ui_widget.h"
#include "ui_resizer.h"
#include "ui_widget_layout.h"

typedef struct ui_flexbox_line {
        size_t index;
        float main_size;
        float intrinsic_non_percentage_size;
        float percentage_non_percentage_size;
        float intrinsic_percentage;
        float intrinsic_gap_percentage;
        float intrinsic_min_size;
        float intrinsic_container_min_size;
        float cross_axis;
        float cross_size;
        float sum_of_grow_value;
        float sum_of_shrink_value;
        size_t count_of_in_flow_items;
        size_t count_of_auto_margin_items;

        /** list_t<ui_widget_t*> items */
        list_t items;
} ui_flexbox_line_t;

typedef struct ui_flexbox_context {
        ui_widget_t *widget;
        bool column_direction;
        ui_resizer_t *resizer;
        float cross_size;
        float main_gap;
        float cross_gap;
        uint8_t main_gap_type;
        css_numeric_value_t main_gap_value;
        css_unit_t main_gap_unit;
        bool has_intrinsic_lines;
        /* 主轴可用尺寸上限与交叉轴提示尺寸，在 gap 重解析后一次性快照
         * （对应旧 apply_main_size 入口处的读取），供各趟使用。 */
        float max_main_size;
        float hint_cross_size;

        /** list_t<ui_flexbox_line_t*> lines */
        list_t lines;
        /** list_t<ui_widget_t*> absolute children */
        list_t out_of_flow_items;
} ui_flexbox_context_t;

typedef struct ui_flexbox_main_contribution {
        float non_percentage_size;
        float percentage;
        float min_size;
        float container_min_size;
        bool has_percentage_size;
} ui_flexbox_main_contribution_t;

static float ui_ceil_layout_unit(float value)
{
        if (value > FLT_MAX / 64.f || value < -FLT_MAX / 64.f) {
                return value;
        }
        return ceilf(value * 64.f) / 64.f;
}

static ui_flexbox_line_t *ui_flexbox_line_create(void)
{
        ui_flexbox_line_t *line;

        line = malloc(sizeof(ui_flexbox_line_t));
        line->main_size = 0;
        line->intrinsic_non_percentage_size = 0;
        line->percentage_non_percentage_size = 0;
        line->intrinsic_percentage = 0;
        line->intrinsic_gap_percentage = 0;
        line->intrinsic_min_size = 0;
        line->intrinsic_container_min_size = 0;
        line->cross_size = 0;
        line->sum_of_grow_value = 0;
        line->sum_of_shrink_value = 0;
        line->count_of_in_flow_items = 0;
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
        line->count_of_in_flow_items++;
        list_append(&line->items, item);
}

static ui_flexbox_line_t *ui_flexbox_next_line(ui_flexbox_context_t *ctx)
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

static float ui_flexbox_get_main_gap_percentage(ui_flexbox_context_t *ctx)
{
        if (ctx->main_gap_type != CSS_GAP_SET ||
            ctx->main_gap_unit != CSS_UNIT_PERCENT) {
                return 0;
        }
        return ctx->main_gap_value / 100.f;
}

static bool ui_flexbox_item_get_main_percentage(ui_flexbox_context_t *ctx,
                                                ui_widget_t *item,
                                                float *percentage)
{
        css_computed_style_t *s = &item->specified_style;

        if (IS_CSS_PERCENTAGE(s, flex_basis)) {
                *percentage = s->flex_basis / 100.f;
                return true;
        }
        if (s->type_bits.flex_basis != CSS_FLEX_BASIS_AUTO) {
                return false;
        }
        if (ctx->column_direction) {
                if (!IS_CSS_PERCENTAGE(s, height)) {
                        return false;
                }
                *percentage = s->height / 100.f;
                return true;
        }
        if (!IS_CSS_PERCENTAGE(s, width)) {
                return false;
        }
        *percentage = s->width / 100.f;
        return true;
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
                    css_width_from_cbox(s, item->unconstrained_content_width));
        }
        s->flex_basis = ui_widget_fix_width(item, s->flex_basis);
}

/* 计算一个 auto 宽度子项在给定可用宽度下的 fit-content 内容宽度：
 * 自有内容尺寸（sizehint）与子项布局占用的空间尺寸按轴取最大值，再受
 * 可用宽与 max-width 钳制。sizehint 只负责自有内容层，子项空间由布局
 * 引擎计算；两者在 LCUI 中是同一内容区的不同表达，因此取 max 而非相加。 */
static float ui_flexbox_fit_content_width(ui_widget_t *item, float avail_width)
{
        ui_sizehint_t hint = { 0 };
        float own_content;
        float max_content;
        float max_width;

        ui_widget_get_sizehint(item, &hint);
        own_content = hint.max_width;
        max_content = y_max(own_content, item->unconstrained_content_width);
        /* 与 used width 的 clamp 保持一致：max-width 也限制 fit-content。 */
        if (ui_widget_get_max_width(item, &max_width)) {
                max_content =
                    y_min(max_content, css_width_to_content_box_width(
                                           &item->computed_style, max_width));
        }
        if (avail_width <= 0.0f) {
                /* 父级尚未完成布局时可用宽可能为 0；此时不能用 0 去钳制，
                 * 否则会把 min-content 当成内在宽缓存下来。 */
                return y_max(item->min_content_width, max_content);
        }
        return y_max(item->min_content_width, y_min(max_content, avail_width));
}

/* 尝试用子项 sizehint 组合测量 flex column 容器的高度，不触发 reflow。
 * avail_width 是容器的内容宽；子项按其内容宽约束测折行高。
 * 仅当所有 in-flow 子项都有 sizehint 时成功，否则返回 -1。 */
static float ui_flexbox_try_measure_container_height(ui_widget_t *item,
                                                     float avail_width)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *s = &item->computed_style;
        ui_sizehint_t child_hint;
        float child_avail;
        float child_height;
        float height = 0;
        unsigned count = 0;

        if (s->type_bits.display != CSS_DISPLAY_FLEX ||
            s->type_bits.flex_direction != CSS_FLEX_DIRECTION_COLUMN) {
                return -1;
        }
        for (list_each(node, &item->children)) {
                child = node->data;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                /* 先清除上一轮布局写入的宽高与 auto margin，再读取
                 * 盒模型和 sizehint，避免把 used size 当作内在尺寸。 */
                ui_widget_reset_width(child);
                ui_widget_reset_height(child);
                child_avail = avail_width -
                              css_margin_x(&child->computed_style) -
                              css_border_x(&child->computed_style) -
                              css_padding_x(&child->computed_style);
                if (child_avail < 0) {
                        child_avail = 0;
                }
                if (!ui_widget_sizehint_is_exact(child)) {
                        return -1;
                }
                ui_widget_set_available_width_constraint(
                    child, child_avail, item, item->layout_generation);
                ui_widget_get_sizehint(child, &child_hint);
                if (IS_CSS_FIXED_LENGTH(&child->specified_style, height)) {
                        /* sizehint 描述文本等内容层，不包含声明的固定
                         * height。固定高子项必须按 used outer height 计入，
                         * 否则 32px 按钮会被误算成 20px 文本行。 */
                        child_height = css_obox_height(
                            &child->computed_style,
                            ui_widget_fix_height(child,
                                                 child->computed_style.height));
                } else {
                        child_height = css_obox_height(
                            &child->computed_style,
                            css_height_from_cbox(&child->computed_style,
                                                 child_hint.max_height));
                }
                if (count > 0) {
                        height += s->row_gap;
                }
                height += child_height;
                count++;
        }
        if (count == 0) {
                return -1;
        }
        /* 与 reflow 路径的 measured_content_height 一样，这里返回容器
         * content-box 高度；父级的 padding/border 由 flex basis 的盒模型
         * 换算统一加入，不能在此重复计算。 */
        return height;
}

/* 在给定的可用宽度约束下，测量一个无 sizehint 的 flex 容器子项的主轴
 * （高度）内在尺寸。
 *
 * 普通容器（如 empty-header）的原型 sizehint 为空操作，其高度取决于子内容
 * 在约束宽度下的折行；在 definite 宽度下回流一次即可让其子内容收到约束并
 * 折行，从而得到正确的高度。avail_width 已在约束记录阶段按最终交叉轴
 * 规则解析，快路径与 reflow 共用该宽度。 */
static float ui_flexbox_measure_item_height_at_width(ui_widget_t *item,
                                                     float avail_width)
{
        css_computed_style_t *s = &item->computed_style;
        float content_width;

        /* Auto 宽度只会在本函数中被写成固定值，因此固定宽可作为
         * “本次父布局已测量”的标记，避免后续 basis 趟再次回流。 */
        if (item->specified_style.type_bits.width == CSS_WIDTH_AUTO &&
            IS_CSS_FIXED_LENGTH(s, width) && item->has_measured &&
            fabsf(item->measured_content_width - avail_width) < 0.01f) {
                return item->measured_content_height;
        }
        /* 约束记录阶段已经按最终交叉轴规则解析内容宽，快路径、
         * reflow 与缓存键都直接使用同一个值。 */
        content_width = avail_width;
        /* 请求重排时已按依赖链清除缓存，因此这里只按测量宽度命中。
         * 不能检查 should_reflow：该标志在父级嵌套测量的各 basis 趟之间
         * 仍然置位，会把刚得到的同轮结果误判为过期。 */
        if (item->has_measured &&
            fabsf(item->measured_content_width - content_width) < 0.01f) {
                return item->measured_content_height;
        }
        /* 优先用 sizehint 组合测量，避免嵌套 reflow。 */
        {
                float no_reflow_height =
                    ui_flexbox_try_measure_container_height(item,
                                                            content_width);

                if (no_reflow_height >= 0) {
                        item->measured_content_width = content_width;
                        item->measured_content_height = no_reflow_height;
                        item->has_measured = true;
                        return no_reflow_height;
                }
        }
        CSS_SET_FIXED_LENGTH(s, width, css_width_from_cbox(s, content_width));
        /* 提交宽度，使子项回流时 record_width_constraint 读取到正确的
         * content_box.width（否则会用陈旧的宽度记录后代约束）。 */
        ui_widget_update_box_width(item);
        ui_widget_reflow(item);
        item->measured_content_width = content_width;
        item->measured_content_height = item->content_box.height;
        item->has_measured = true;
        return item->content_box.height;
}

static void ui_compute_column_item_flex_basis(ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;

        if (IS_CSS_PERCENTAGE(s, flex_basis)) {
                CSS_SET_FIXED_LENGTH(
                    s, flex_basis,
                    s->flex_basis * item->parent->content_box.height / 100);
        } else if (!IS_CSS_FIXED_LENGTH(s, flex_basis)) {
                float intrinsic = 0;
                float own_height = 0;
                float avail_width = 0;
                bool has_constraint;
                ui_sizehint_t hint = { 0 };

                /* 自有内容尺寸与子项布局尺寸按轴取最大值。sizehint 只负责
                 * 自有内容层；容器的主轴尺寸由子项布局推导。 */
                has_constraint =
                    ui_widget_get_available_width(item, &avail_width);
                ui_widget_get_sizehint(item, &hint);
                own_height = hint.max_height;
                if (has_constraint && ui_widget_sizehint_is_exact(item)) {
                        intrinsic = own_height;
                } else if (has_constraint &&
                           (ui_widget_has_flex_display(item) ||
                            ui_widget_has_block_display(item)) &&
                           (item->specified_style.type_bits.width ==
                                CSS_WIDTH_AUTO ||
                            IS_CSS_PERCENTAGE(&item->specified_style, width))) {
                        /* Auto/percentage-width block/flex children have a
                         * definite cross size in this column. Measure their
                         * stacked children at that width instead of reusing
                         * the unconstrained height from an earlier layout. */
                        intrinsic = y_max(
                            own_height, ui_flexbox_measure_item_height_at_width(
                                            item, avail_width));
                } else {
                        intrinsic = y_max(own_height,
                                          item->unconstrained_content_height);
                }
                CSS_SET_FIXED_LENGTH(s, flex_basis,
                                     css_height_from_cbox(s, intrinsic));
        }
        s->flex_basis = ui_widget_fix_height(item, s->flex_basis);
}

/* 重置项目主轴方向上的尺寸与 flex_basis，返回其外框主轴尺寸 */
static float ui_flexbox_item_reset_main_size(ui_flexbox_context_t *ctx,
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
static float ui_flexbox_item_min_main_size(ui_flexbox_context_t *ctx,
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

static float ui_flexbox_item_max_main_size(ui_flexbox_context_t *ctx,
                                           ui_widget_t *item)
{
        css_computed_style_t *cs = &item->computed_style;

        if (ctx->column_direction) {
                float avail_width;

                if (item->has_measured &&
                    ui_widget_get_available_width(item, &avail_width) &&
                    fabsf(item->measured_content_width - avail_width) < 0.01f) {
                        /* 同宽测量是当前的内在高度；旧的无约束缓存可能
                         * 更大，不能在内容收起后再次把它作为下限。 */
                        return css_obox_height(
                            cs, css_height_from_cbox(
                                    cs, item->measured_content_height));
                }
                return css_obox_height(
                    cs, css_height_from_cbox(
                            cs, item->unconstrained_content_height));
        }
        return css_obox_width(
            cs, css_width_from_cbox(cs, item->unconstrained_content_width));
}

static ui_flexbox_main_contribution_t ui_flexbox_item_get_main_contribution(
    ui_flexbox_context_t *ctx, ui_widget_t *item)
{
        ui_flexbox_main_contribution_t contribution = { 0 };
        css_computed_style_t *cs = &item->computed_style;
        float intrinsic_size;
        float required_size;
        float fixed_size;

        if (ui_flexbox_item_get_main_percentage(ctx, item,
                                                &contribution.percentage)) {
                contribution.has_percentage_size = true;
                intrinsic_size = ui_flexbox_item_max_main_size(ctx, item);
                if (ctx->column_direction) {
                        contribution.non_percentage_size =
                            css_obox_height(cs, 0);
                        fixed_size = ui_widget_fix_height(item, 0);
                        contribution.min_size = css_obox_height(cs, fixed_size);
                        required_size = css_height_from_bbox(
                            cs, intrinsic_size - css_margin_y(cs));
                        fixed_size = ui_widget_fix_height(item, required_size);
                        fixed_size = css_obox_height(cs, fixed_size);
                } else {
                        contribution.non_percentage_size =
                            css_obox_width(cs, 0);
                        fixed_size = ui_widget_fix_width(item, 0);
                        contribution.min_size = css_obox_width(cs, fixed_size);
                        required_size = css_width_from_bbox(
                            cs, intrinsic_size - css_margin_x(cs));
                        fixed_size = ui_widget_fix_width(item, required_size);
                        fixed_size = css_obox_width(cs, fixed_size);
                }
                if (contribution.percentage > 0 &&
                    contribution.min_size + 0.0001f < intrinsic_size &&
                    fixed_size + 0.0001f >= intrinsic_size) {
                        contribution.container_min_size = ui_ceil_layout_unit(
                            required_size / contribution.percentage);
                }
                return contribution;
        }
        contribution.min_size = ui_flexbox_item_min_main_size(ctx, item);
        contribution.non_percentage_size =
            ui_flexbox_item_reset_main_size(ctx, item);
        contribution.non_percentage_size =
            y_max(contribution.non_percentage_size,
                  ui_flexbox_item_max_main_size(ctx, item));
        return contribution;
}

static bool ui_flexbox_line_should_wrap_percentage(
    ui_flexbox_context_t *ctx, ui_flexbox_line_t *line,
    ui_flexbox_main_contribution_t contribution)
{
        float gap_percentage = ui_flexbox_get_main_gap_percentage(ctx);
        float percentage;
        float non_percentage_size = line->intrinsic_non_percentage_size +
                                    line->percentage_non_percentage_size +
                                    contribution.non_percentage_size;

        if (ctx->widget->computed_style.type_bits.flex_wrap !=
                CSS_FLEX_WRAP_WRAP ||
            line->count_of_in_flow_items == 0) {
                return false;
        }
        percentage = line->intrinsic_percentage + contribution.percentage +
                     gap_percentage;
        if (percentage > 1.0001f) {
                return true;
        }
        if (ctx->main_gap_unit != CSS_UNIT_PERCENT) {
                non_percentage_size += ctx->main_gap;
        }
        return 1.f - percentage <= 0.0001f && non_percentage_size > 0.0001f;
}

static void ui_flexbox_line_load_main_contribution(
    ui_flexbox_context_t *ctx, ui_flexbox_line_t *line, ui_widget_t *item,
    ui_flexbox_main_contribution_t contribution)
{
        if (line->count_of_in_flow_items > 0) {
                if (ctx->main_gap_unit == CSS_UNIT_PERCENT) {
                        float percentage =
                            ui_flexbox_get_main_gap_percentage(ctx);

                        line->intrinsic_percentage += percentage;
                        line->intrinsic_gap_percentage += percentage;
                } else {
                        line->intrinsic_non_percentage_size += ctx->main_gap;
                        line->intrinsic_min_size += ctx->main_gap;
                }
        }
        if (contribution.has_percentage_size) {
                line->percentage_non_percentage_size +=
                    contribution.non_percentage_size;
        } else {
                line->intrinsic_non_percentage_size +=
                    contribution.non_percentage_size;
        }
        line->intrinsic_percentage += contribution.percentage;
        line->intrinsic_min_size += contribution.min_size;
        line->intrinsic_container_min_size =
            y_max(line->intrinsic_container_min_size,
                  contribution.container_min_size);
        ui_flexbox_line_load_item(line, item);
}

static float ui_flexbox_line_intrinsic_main_size(ui_flexbox_context_t *ctx,
                                                 ui_flexbox_line_t *line)
{
        list_node_t *node;
        ui_widget_t *item;
        ui_flexbox_main_contribution_t contribution;
        float denominator;
        float main_size;
        float next_size;
        size_t i;

        main_size =
            y_max(line->intrinsic_min_size, line->intrinsic_container_min_size);
        for (i = 0; i <= line->count_of_in_flow_items; ++i) {
                denominator = 1.f - line->intrinsic_gap_percentage;
                next_size = line->intrinsic_non_percentage_size;
                for (list_each(node, &line->items)) {
                        item = node->data;
                        if (!ui_widget_in_layout_flow(item) ||
                            !ui_flexbox_item_get_main_percentage(
                                ctx, item, &contribution.percentage)) {
                                continue;
                        }
                        contribution =
                            ui_flexbox_item_get_main_contribution(ctx, item);
                        if (contribution.percentage * main_size +
                                contribution.non_percentage_size >=
                            contribution.min_size) {
                                denominator -= contribution.percentage;
                                next_size += contribution.non_percentage_size;
                        } else {
                                next_size += contribution.min_size;
                        }
                }
                if (denominator > 0.0001f) {
                        next_size /= denominator;
                } else {
                        next_size = main_size;
                }
                next_size = y_max(next_size, line->intrinsic_min_size);
                next_size =
                    y_max(next_size, line->intrinsic_container_min_size);
                if (next_size <= main_size + 0.0001f) {
                        break;
                }
                main_size = next_size;
        }
        return main_size;
}

static void ui_flexbox_compute_justify_content(ui_flexbox_context_t *ctx,
                                               ui_flexbox_line_t *line,
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
        /* 把主轴分配结果记录为宽度约束，供子项单独 reflow 时重推导。 */
        if (item->parent) {
                ui_widget_set_available_width_constraint(
                    item, css_width_to_content_box_width(cs, cs->width),
                    item->parent, item->parent->layout_generation);
        }
        ui_widget_reflow_if_width_changed(item);
}

/* 写入列方向子项的主轴尺寸，不触发回流。
 * 主轴（高）与交叉轴（宽）分别在主轴趟和交叉趟写入，回流效果一致，
 * 故统一推迟到交叉趟的合并回流（ui_widget_reflow_if_size_changed），
 * 保证每子项每次父布局至多回流一次。 */
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
}

/* 第一趟：收集流内子项并测量主轴尺寸。
 *
 * 先记录宽度约束，再按文档顺序测量各流内子项；内在分行路径由
 * ui_flexbox_collect_intrinsic_lines 单独处理。
 */
/* 解析子项用于内在测高的内容宽。百分比先由 compute_style 解析为
 * CSS width；auto 则按交叉轴规则选择 stretch 或 fit-content。随后与
 * used width 一样应用 min/max，最后换算为内容宽，确保后续各测量路径
 * 与实际盒模型提交使用同一输入。 */
static float ui_flexbox_resolve_measure_width(ui_flexbox_context_t *ctx,
                                              ui_widget_t *item)
{
        css_computed_style_t *s = &item->computed_style;
        css_computed_style_t *specified = &item->specified_style;
        float avail;
        float width;

        if (IS_CSS_PERCENTAGE(specified, width)) {
                width = s->width;
        } else {
                avail = ctx->widget->content_box.width - css_margin_x(s) -
                        css_border_x(s) - css_padding_x(s);
                if (avail < 0) {
                        avail = 0;
                }
                if (ctx->widget->computed_style.type_bits.align_items ==
                        CSS_ALIGN_ITEMS_STRETCH &&
                    s->type_bits.margin_left != CSS_MARGIN_AUTO &&
                    s->type_bits.margin_right != CSS_MARGIN_AUTO) {
                        width = css_width_from_cbox(s, avail);
                } else {
                        width = css_width_from_cbox(
                            s, ui_flexbox_fit_content_width(item, avail));
                }
        }
        width = ui_widget_fix_width(item, width);
        width = css_width_to_content_box_width(s, width);
        return y_max(0, width);
}

/* 为单个 in-flow 且宽度 auto/percentage 的子项记录父级可用宽约束。
 * 必须在 ui_widget_reset_layout(child) 之后调用，避免读到上一次布局
 * 写入的 auto margin。 */
static void ui_flexbox_record_width_constraint(ui_flexbox_context_t *ctx,
                                               ui_widget_t *child)
{
        float avail;
        bool auto_width =
            child->specified_style.type_bits.width == CSS_WIDTH_AUTO;
        bool percentage_width =
            IS_CSS_PERCENTAGE(&child->specified_style, width);

        if (!ctx->column_direction ||
            !IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, width) ||
            (!auto_width && !percentage_width)) {
                return;
        }
        avail = ui_flexbox_resolve_measure_width(ctx, child);
        ui_widget_set_available_width_constraint(
            child, avail, ctx->widget, ctx->widget->layout_generation);
}

/* 按内在内容把子项分入内在行并累计主轴尺寸。 */
static void ui_flexbox_collect_intrinsic_lines(ui_flexbox_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        ui_flexbox_line_t *line = NULL;
        ui_flexbox_main_contribution_t contribution;
        css_computed_style_t *cs;
        float main_size, min_main_size;
        float max_item_min_size = 0;

        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (ui_widget_is_display_none(child)) {
                        continue;
                }
                if (ui_widget_has_absolute_position(child)) {
                        list_append(&ctx->out_of_flow_items, child);
                        continue;
                }
                ui_widget_reset_layout(child);
                ui_flexbox_record_width_constraint(ctx, child);
                contribution =
                    ui_flexbox_item_get_main_contribution(ctx, child);
                min_main_size = contribution.min_size;
                if (cs->flex_shrink <= 0 && !contribution.has_percentage_size) {
                        min_main_size = y_max(min_main_size,
                                              contribution.non_percentage_size);
                }
                max_item_min_size = y_max(max_item_min_size, min_main_size);
                if (!line || ui_flexbox_line_should_wrap_percentage(
                                 ctx, line, contribution)) {
                        line = ui_flexbox_next_line(ctx);
                }
                ui_flexbox_line_load_main_contribution(ctx, line, child,
                                                       contribution);
        }
        for (list_each(node, &ctx->lines)) {
                line = node->data;
                main_size = ui_flexbox_line_intrinsic_main_size(ctx, line);
                ui_resizer_load_item_main_size(
                    ctx->resizer, main_size,
                    y_max(max_item_min_size,
                          line->intrinsic_container_min_size));
                ui_resizer_update(ctx->resizer);
        }
}

static void ui_flexbox_collect_in_flow_items(ui_flexbox_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *cs;
        float main_size, min_main_size;
        bool main_size_fixed;
        bool load_intrinsic_lines;
        unsigned child_index = 0;
        unsigned in_flow_count = 0;

        main_size_fixed =
            ctx->column_direction
                ? IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, height)
                : IS_CSS_FIXED_LENGTH(&ctx->widget->computed_style, width);
        load_intrinsic_lines =
            !main_size_fixed &&
            ctx->widget->computed_style.type_bits.flex_wrap ==
                CSS_FLEX_WRAP_WRAP;
        ctx->has_intrinsic_lines = load_intrinsic_lines;

        UI_DEBUG_BEGIN;
        UI_DEBUG_MSG("%s: begin, main_size_fixed?=%d", __FUNCTION__,
                     main_size_fixed);
        UI_DEBUG_INDENT_INC;
        UI_DEBUG_END;
        if (load_intrinsic_lines) {
                ui_flexbox_collect_intrinsic_lines(ctx);
                UI_DEBUG_BEGIN;
                UI_DEBUG_INDENT_DEC;
                UI_WIDGET_SIZE_STR(ctx->widget, size_str);
                UI_DEBUG_MSG("%s: end, size=%s", __FUNCTION__, size_str);
                UI_DEBUG_END;
                return;
        }
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                cs = &child->computed_style;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                ui_widget_reset_layout(child);
                ui_flexbox_record_width_constraint(ctx, child);
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
        UI_DEBUG_BEGIN;
        UI_DEBUG_INDENT_DEC;
        UI_WIDGET_SIZE_STR(ctx->widget, size_str);
        UI_DEBUG_MSG("%s: end, size=%s", __FUNCTION__, size_str);
        UI_DEBUG_END;
}

/* 累加单个子项的 auto margin 数量到行统计。 */
static void ui_flexbox_line_add_auto_margin_count(ui_flexbox_context_t *ctx,
                                                  ui_flexbox_line_t *line,
                                                  ui_widget_t *item)
{
        css_computed_style_t *cs = &item->computed_style;

        if (ctx->column_direction) {
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
}

static void ui_flexbox_line_reset_main_size(ui_flexbox_context_t *ctx,
                                            ui_flexbox_line_t *line)
{
        list_node_t *node;
        ui_widget_t *child;
        css_computed_style_t *cs;
        float main_size;

        line->main_size = 0;
        line->cross_size = 0;
        line->sum_of_grow_value = 0;
        line->sum_of_shrink_value = 0;
        line->count_of_in_flow_items = 0;
        line->count_of_auto_margin_items = 0;
        for (list_each(node, &line->items)) {
                child = node->data;
                if (!ui_widget_in_layout_flow(child)) {
                        continue;
                }
                cs = &child->computed_style;
                main_size = ui_flexbox_item_reset_main_size(ctx, child);
                if (line->count_of_in_flow_items > 0) {
                        line->main_size += ctx->main_gap;
                }
                line->main_size += main_size;
                if (cs->flex_grow > 0) {
                        line->sum_of_grow_value += cs->flex_grow;
                }
                if (cs->flex_shrink > 0) {
                        line->sum_of_shrink_value += cs->flex_shrink;
                }
                ui_flexbox_line_add_auto_margin_count(ctx, line, child);
                line->count_of_in_flow_items++;
        }
}

static void ui_flexbox_apply_line(ui_flexbox_context_t *ctx,
                                  ui_flexbox_line_t *line, float flex_space,
                                  float margin_space)
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
                        css_computed_style_t *cs = &child->computed_style;

                        ui_apply_column_item_main_size(child, flex_space,
                                                       margin_space);
                        /* 从刚写入的固定高度推导外框高度。
                         * 主轴趟不再触发回流、outer_box 尚未更新，故按
                         * update_box_height 的换算（fix_height 钳制 + 盒模型 +
                         * 外边距）直接推导，使行主轴尺寸不依赖子项是否已回流；
                         * row 路径仍读取回流后的 outer_box。 */
                        item_main_size = css_obox_height(
                            cs, ui_widget_fix_height(child, cs->height));
                        if (IS_CSS_FIXED_LENGTH(&child->computed_style,
                                                width)) {
                                item_cross_size = child->outer_box.width;
                        } else {
                                item_cross_size = css_obox_width(
                                    &child->computed_style,
                                    css_width_from_cbox(
                                        &child->computed_style,
                                        child->unconstrained_content_width));
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

/* 第二趟：resolve_flex_basis，在已分好的行上重解析各子项的 flex basis
 * （内在路径）。
 *
 * 容器主轴尺寸提交后，行内每个流内子项的主轴 basis 需按新的 content
 * size 重新解析（百分比 basis 依赖它）。核心调用为
 * ui_flexbox_line_reset_main_size → ui_flexbox_item_reset_main_size。
 *
 * 注意（行为保持）：ui_flexbox_line_reset_main_size 在同一次逐子项遍历中
 * 一并重建各行的 main_size/grow/shrink/auto-margin 累计值，此处原样保留。
 * nowrap 路径本趟为空操作：该路径的 basis 重解析内联在
 * break_into_lines 中（分行判定需要按文档顺序消费重算后的 basis 值）。
 */
static void ui_flexbox_resolve_flex_basis(ui_flexbox_context_t *ctx)
{
        list_node_t *line_node;
        ui_flexbox_line_t *line;

        if (!ctx->has_intrinsic_lines) {
                return;
        }
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                ui_flexbox_line_reset_main_size(ctx, line);
        }
}

/* 第三趟：break_into_lines，将子项分入最终行（非内在路径）。
 *
 * 以提交后的主轴可用尺寸（ctx->max_main_size）为界分行。内在路径
 * 的行已在第一趟 collect_in_flow_items 中按百分比分好、并在第二趟
 * resolve_flex_basis 中重置完毕，本趟为空操作。两处分行点刻意保持分离：
 * 它们在不同时机、基于不同阶段的 basis 值运行，合并会改变 basis 的
 * 计算时机从而改变数值。
 *
 * 注意（行为保持）：逐子项的 flex basis 重解析（分行判定按文档顺序消费
 * 其返回值）与流外子项的归行逻辑原样内联在本趟。
 */
static void ui_flexbox_break_into_lines(ui_flexbox_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;
        css_computed_style_t *cs;
        float main_size;
        unsigned child_index = 0;

        if (ctx->has_intrinsic_lines) {
                return;
        }
        line = ui_flexbox_next_line(ctx);
        for (list_each(node, &ctx->widget->children)) {
                child = node->data;
                if (ui_widget_is_display_none(child)) {
                        child_index++;
                        continue;
                }
                if (ui_widget_has_absolute_position(child)) {
                        UI_DEBUG_BEGIN;
                        UI_WIDGET_STR(child, str);
                        UI_DEBUG_MSG("children[%u]=%s, out-of-flow",
                                     child_index, str);
                        child_index++;
                        UI_DEBUG_END;
                        list_append(&ctx->out_of_flow_items, child);
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
                        ctx->max_main_size) {
                        line = ui_flexbox_next_line(ctx);
                }
                if (line->count_of_in_flow_items > 0) {
                        line->main_size += ctx->main_gap;
                }
                line->main_size += main_size;
                line->count_of_in_flow_items++;
                ui_flexbox_line_add_auto_margin_count(ctx, line, child);
                ui_flexbox_line_load_item(line, child);
        }
}

/* 第四趟：resolve_main_sizes，分配各行主轴空间并落到子项上。
 *
 * 对每行计算剩余空间（grow/shrink/auto margin 分摊），再经
 * ui_flexbox_apply_line 将最终主轴尺寸写入各子项。
 *
 * 注意（行为保持）：apply_line 在应用主轴尺寸的同时顺带测量各行的交叉
 * 尺寸。column 路径自本任务起主轴趟不再触发回流，行主轴尺寸改由刚写入
 * 的固定高度直接推导（见 apply_line），子项的盒模型提交与回流统一推迟
 * 到第五趟交叉轴的合并回流（ui_widget_reflow_if_size_changed），每子项
 * 每次父布局至多回流一次；row 路径仍在 apply_line 内经 ui_widget_
 * reflow_if_width_changed 触发回流并读取回流后的 outer_box，保持不变。
 * 运行中的容器交叉尺寸累计也发生在本趟；容器交叉尺寸的提交属于第五趟
 * resolve_cross_sizes。
 */
static void ui_flexbox_resolve_main_sizes(ui_flexbox_context_t *ctx)
{
        list_node_t *line_node;
        ui_flexbox_line_t *line;
        float space, flex_space, margin_space;

        ctx->cross_size = 0;
        for (list_each(line_node, &ctx->lines)) {
                line = line_node->data;
                space = ctx->max_main_size - line->main_size;
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
                ui_flexbox_apply_line(ctx, line, flex_space, margin_space);
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
}

/* 第六趟：layout_out_of_flow，流外子项的布局。
 *
 * 原样抽取自 ui_flexbox_compute_item_layout 的流外分支。它必须保持逐项、
 * 按文档顺序内联在交叉轴定位循环（见 resolve_cross_sizes）中执行：后面的
 * 流内子项会推进 main_axis，而流外子项在此处对当时的坐标做快照，提前或
 * 延后执行都会改变其落点。
 */
static void ui_flexbox_out_of_flow(ui_flexbox_context_t *ctx, ui_widget_t *item,
                                   float main_axis, float cross_axis)
{
        bool column = ctx->column_direction;

        item->layout_x = column ? cross_axis : main_axis;
        item->layout_y = column ? main_axis : cross_axis;
        ui_widget_reset_layout(item);
        ui_widget_reflow_if_width_changed(item);
        ui_widget_reflow_if_height_changed(item);
        ui_widget_update_box_position(item);
}

static float ui_flexbox_compute_item_layout(ui_flexbox_context_t *ctx,
                                            ui_widget_t *item, float main_axis,
                                            float cross_axis,
                                            css_align_items_t align,
                                            float line_max_cross_size)
{
        css_computed_style_t *s = &item->computed_style;
        bool column = ctx->column_direction;
        float item_outer_cross;

        if (column) {
                if (IS_CSS_PERCENTAGE(&item->specified_style, width)) {
                        ui_widget_reset_width(item);
                } else if (align == CSS_ALIGN_ITEMS_STRETCH &&
                           item->specified_style.type_bits.width ==
                               CSS_WIDTH_AUTO &&
                           s->type_bits.margin_left != CSS_MARGIN_AUTO &&
                           s->type_bits.margin_right != CSS_MARGIN_AUTO) {
                        CSS_SET_FIXED_LENGTH(
                            s, width,
                            css_border_box_width_to_width(
                                s, line_max_cross_size - css_margin_x(s)));
                } else if (align == CSS_ALIGN_ITEMS_CENTER &&
                           item->specified_style.type_bits.width ==
                               CSS_WIDTH_AUTO) {
                        /* 居中的自动宽度子项按 fit-content 钳制到可用宽度
                         * （max-content 与可用宽度取小，并尊重 min-content），
                         * 使文本等子内容折行，而非以单行溢出居中。 */
                        float avail = 0;
                        float content_width;

                        if (ui_widget_get_available_width(item, &avail)) {
                                content_width =
                                    ui_flexbox_fit_content_width(item, avail);
                                CSS_SET_FIXED_LENGTH(
                                    s, width,
                                    css_width_from_cbox(s, content_width));
                        }
                }
                /* 合并回流：主轴趟写入的 height 与此处写入的 width 一并
                 * 提交（update_box_width/height），任一维度变化时仅回流
                 * 一次，覆盖两维。row 路径维持原有的单维回流不变。 */
                ui_widget_reflow_if_size_changed(item);
                item_outer_cross = item->outer_box.width;
        } else {
                if (IS_CSS_PERCENTAGE(&item->specified_style, height)) {
                        ui_widget_reset_height(item);
                } else if (align == CSS_ALIGN_ITEMS_STRETCH &&
                           item->specified_style.type_bits.height ==
                               CSS_HEIGHT_AUTO &&
                           s->type_bits.margin_top != CSS_MARGIN_AUTO &&
                           s->type_bits.margin_bottom != CSS_MARGIN_AUTO) {
                        CSS_SET_FIXED_LENGTH(
                            s, height,
                            css_border_box_height_to_height(
                                s, line_max_cross_size - css_margin_y(s)));
                }
                ui_widget_reflow_if_height_changed(item);
                item_outer_cross = item->outer_box.height;
        }
        {
                bool start_auto;
                bool end_auto;

                if (column) {
                        start_auto =
                            s->type_bits.margin_left == CSS_MARGIN_AUTO;
                        end_auto = s->type_bits.margin_right == CSS_MARGIN_AUTO;
                } else {
                        start_auto = s->type_bits.margin_top == CSS_MARGIN_AUTO;
                        end_auto =
                            s->type_bits.margin_bottom == CSS_MARGIN_AUTO;
                }
                if (start_auto || end_auto) {
                        float free = line_max_cross_size - item_outer_cross;

                        if (free < 0) {
                                free = 0;
                        }
                        if (start_auto && end_auto) {
                                cross_axis += free * 0.5f;
                        } else if (start_auto) {
                                cross_axis += free;
                        }
                } else {
                        switch (align) {
                        case CSS_ALIGN_ITEMS_CENTER:
                                cross_axis +=
                                    (line_max_cross_size - item_outer_cross) *
                                    0.5f;
                                break;
                        case CSS_ALIGN_ITEMS_FLEX_END:
                                cross_axis +=
                                    line_max_cross_size - item_outer_cross;
                                break;
                        default:
                                break;
                        }
                }
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

/* 第五趟：resolve_cross_sizes，提交交叉尺寸并定位所有子项。
 *
 * 先提交容器交叉尺寸（宽/高未显式指定时，取行交叉尺寸累计与提示尺寸
 * 的较大值），再逐行定位子项：主轴起点由 compute_justify_content（保留
 * 的辅助函数）决定，交叉方向按 align 拉伸/对齐。流外子项由第六趟
 * ui_flexbox_out_of_flow 在同一次逐项遍历中按文档顺序处理。
 *
 * 注意（行为保持）：交叉尺寸提交（原 apply_main_size 尾部）必须先于
 * cross_axis/cross_free_space 的读取执行——提交会改写 content_box 与
 * ctx->cross_size，读取时序与旧代码保持一致。
 */
static void ui_flexbox_resolve_cross_sizes(ui_flexbox_context_t *ctx)
{
        list_node_t *line_node, *node;
        ui_widget_t *child;
        ui_flexbox_line_t *line;
        css_computed_style_t *s = &ctx->widget->computed_style;
        bool column = ctx->column_direction;
        unsigned item_index;

        float space;
        float main_axis;
        float cross_axis;
        float cross_free_space;
        float cross_space;
        float line_max_cross_size;

        /* 合并交叉轴 hint（无论交叉尺寸是否固定都合并，保持旧行为）。 */
        ctx->cross_size = y_max(ctx->cross_size, ctx->hint_cross_size);
        if (column) {
                cross_axis = s->padding_left;
                if (!IS_CSS_FIXED_LENGTH(s, width)) {
                        ui_widget_set_content_width(ctx->widget,
                                                    ctx->cross_size);
                }
                cross_free_space = ctx->widget->content_box.width;
        } else {
                cross_axis = s->padding_top;
                if (!IS_CSS_FIXED_LENGTH(s, height)) {
                        ui_widget_set_content_height(ctx->widget,
                                                     ctx->cross_size);
                }
                cross_free_space = ctx->widget->content_box.height;
        }
        cross_free_space -= ctx->cross_size;
        cross_space =
            ctx->lines.length > 0 ? cross_free_space / ctx->lines.length : 0;

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
                ui_flexbox_compute_justify_content(ctx, line, &main_axis,
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
                        if (!ui_widget_in_layout_flow(child)) {
                                ui_flexbox_out_of_flow(ctx, child, main_axis,
                                                       cross_axis);
                        } else {
                                main_axis += ui_flexbox_compute_item_layout(
                                    ctx, child, main_axis, cross_axis,
                                    s->type_bits.align_items,
                                    line_max_cross_size);
                        }
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

/* 测量阶段：收集子项、提交主轴尺寸、重解析 flex basis。 */
static void ui_flexbox_measure(ui_flexbox_context_t *ctx)
{
        ui_widget_t *w = ctx->widget;

        ui_flexbox_collect_in_flow_items(ctx);
        if (ctx->column_direction) {
                ui_resizer_commit_column_main_size(ctx->resizer);
                ctx->main_gap = ui_flexbox_resolve_gap(
                    ctx->main_gap_type, ctx->main_gap_value, ctx->main_gap_unit,
                    w->content_box.height);
                ctx->max_main_size = w->content_box.height;
                ctx->hint_cross_size = ctx->resizer->hint.max_width;
        } else {
                ui_resizer_commit_row_main_size(ctx->resizer);
                ctx->main_gap = ui_flexbox_resolve_gap(
                    ctx->main_gap_type, ctx->main_gap_value, ctx->main_gap_unit,
                    w->content_box.width);
                ctx->max_main_size = w->content_box.width;
                ctx->hint_cross_size = ctx->resizer->hint.max_height;
        }
        ui_flexbox_resolve_flex_basis(ctx);
}

/* 第六趟：layout_out_of_flow，绝对定位子项的静态位置。
 *
 * CSS Flexbox §4.1：绝对定位子项不参与 flex 布局，其静态位置按“容器中唯一
 * flex item”计算，使用容器的 justify-content / align-items 和自身 used
 * size。这里在主布局结束后统一处理，避免污染 line->items 的 gap/justify
 * 计数，也避免在多趟遍历里重复访问。 */
static float ui_flexbox_static_main_offset(ui_flexbox_context_t *ctx,
                                           float item_size)
{
        css_computed_style_t *s = &ctx->widget->computed_style;
        float start = ctx->column_direction ? s->padding_top : s->padding_left;
        float free = (ctx->column_direction ? ctx->widget->content_box.height
                                            : ctx->widget->content_box.width) -
                     item_size;

        if (free < 0) {
                free = 0;
        }
        switch (s->type_bits.justify_content) {
        case CSS_JUSTIFY_CONTENT_FLEX_END:
                return start + free;
        case CSS_JUSTIFY_CONTENT_CENTER:
        case CSS_JUSTIFY_CONTENT_SPACE_AROUND:
        case CSS_JUSTIFY_CONTENT_SPACE_EVENLY:
                return start + free * 0.5f;
        default:
                /* flex-start, and space-between with a single item. */
                return start;
        }
}

static float ui_flexbox_static_cross_offset(ui_flexbox_context_t *ctx,
                                            float item_size)
{
        css_computed_style_t *s = &ctx->widget->computed_style;
        float start = ctx->column_direction ? s->padding_left : s->padding_top;
        float free = (ctx->column_direction ? ctx->widget->content_box.width
                                            : ctx->widget->content_box.height) -
                     item_size;

        if (free < 0) {
                free = 0;
        }
        switch (s->type_bits.align_items) {
        case CSS_ALIGN_ITEMS_FLEX_END:
                return start + free;
        case CSS_ALIGN_ITEMS_CENTER:
                return start + free * 0.5f;
        default:
                /* stretch / baseline / flex-start. */
                return start;
        }
}

static void ui_flexbox_layout_out_of_flow(ui_flexbox_context_t *ctx)
{
        list_node_t *node;
        ui_widget_t *item;
        float main_size;
        float cross_size;
        float main_axis;
        float cross_axis;

        for (list_each(node, &ctx->out_of_flow_items)) {
                item = node->data;
                ui_widget_reset_layout(item);
                ui_widget_reflow_if_width_changed(item);
                ui_widget_reflow_if_height_changed(item);
                main_size = ctx->column_direction ? item->outer_box.height
                                                  : item->outer_box.width;
                cross_size = ctx->column_direction ? item->outer_box.width
                                                   : item->outer_box.height;
                main_axis = ui_flexbox_static_main_offset(ctx, main_size);
                cross_axis = ui_flexbox_static_cross_offset(ctx, cross_size);
                item->layout_x = ctx->column_direction ? cross_axis : main_axis;
                item->layout_y = ctx->column_direction ? main_axis : cross_axis;
                ui_widget_update_box_position(item);
        }
}

/* 布局阶段：分行、分配主轴、分配交叉并定位。 */
static void ui_flexbox_layout(ui_flexbox_context_t *ctx)
{
        ui_flexbox_break_into_lines(ctx);
        ui_flexbox_resolve_main_sizes(ctx);
        ui_flexbox_resolve_cross_sizes(ctx);
}

void ui_flexbox_reflow(ui_widget_t *w, ui_resizer_t *resizer)
{
        ui_flexbox_context_t ctx = { 0 };
        css_computed_style_t *s = &w->computed_style;
        css_computed_style_t *ps =
            w->parent ? &w->parent->computed_style : NULL;
        float row_gap, column_gap;

        ctx.widget = w;
        ctx.resizer = resizer;
        ctx.column_direction = ui_widget_has_flex_column_direction(w);
        list_create(&ctx.lines);
        list_create(&ctx.out_of_flow_items);
        /* 每次回流递增布局代数，使旧一轮记录的可用宽度约束失效，
         * 本轮的约束记录以新代数为准。 */
        w->layout_generation++;
        /* 从持久宽度约束重推导居中列容器的交叉轴宽。
         * 独立回流时父级不在场，但父级记录的约束仍有效，据此重算
         * fit-content 宽并落地，使后代约束得以刷新、折行不丢失。 */
        if (ctx.column_direction &&
            w->specified_style.type_bits.width == CSS_WIDTH_AUTO &&
            !IS_CSS_FIXED_LENGTH(&w->computed_style, width) && ps &&
            (ps->type_bits.display == CSS_DISPLAY_FLEX ||
             ps->type_bits.display == CSS_DISPLAY_INLINE_FLEX) &&
            ps->type_bits.align_items == CSS_ALIGN_ITEMS_CENTER) {
                float avail = 0;

                if (ui_widget_get_available_width(w, &avail)) {
                        float content_width =
                            ui_flexbox_fit_content_width(w, avail);

                        CSS_SET_FIXED_LENGTH(
                            &w->computed_style, width,
                            css_width_from_cbox(&w->computed_style,
                                                content_width));
                        ui_widget_update_box_width(w);
                }
        }
        ui_resizer_init(resizer, w);
        /* 解析 row/column gap，并按主轴方向映射为 main/cross gap。 */
        row_gap =
            ui_flexbox_resolve_gap(s->type_bits.row_gap, s->row_gap,
                                   s->unit_bits.row_gap, w->content_box.height);
        column_gap = ui_flexbox_resolve_gap(
            s->type_bits.column_gap, s->column_gap, s->unit_bits.column_gap,
            w->content_box.width);
        if (ctx.column_direction) {
                ctx.main_gap_type = s->type_bits.row_gap;
                ctx.main_gap_value = s->row_gap;
                ctx.main_gap_unit = s->unit_bits.row_gap;
                ctx.main_gap = row_gap;
                ctx.cross_gap = column_gap;
        } else {
                ctx.main_gap_type = s->type_bits.column_gap;
                ctx.main_gap_value = s->column_gap;
                ctx.main_gap_unit = s->unit_bits.column_gap;
                ctx.main_gap = column_gap;
                ctx.cross_gap = row_gap;
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
        /* 测量阶段：收集子项、提交主轴尺寸、重解析 basis。 */
        ui_flexbox_measure(&ctx);
        /* 布局阶段：分行、分配主轴、分配交叉并定位。 */
        ui_flexbox_layout(&ctx);
        ui_flexbox_layout_out_of_flow(&ctx);
        w->proto->resize(w, w->content_box.width, w->content_box.height);
        list_destroy(&ctx.out_of_flow_items, NULL);
        list_destroy(&ctx.lines, ui_flexbox_line_destroy);
        UI_DEBUG_BEGIN;
        UI_DEBUG_INDENT_DEC;
        UI_WIDGET_STR(w, str);
        UI_WIDGET_SIZE_STR(w, size_str);
        UI_DEBUG_MSG("%s: %s: end, size=%s", __FUNCTION__, str, size_str);
        UI_DEBUG_END;
}
