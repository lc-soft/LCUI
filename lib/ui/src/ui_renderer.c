/*
 * lib/ui/src/ui_renderer.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <yutil.h>
#include <pandagl.h>
#include <css/style_value.h>
#include <ui/base.h>
#include <ui/metrics.h>
#include <ui/prototype.h>
#include <ui/rect.h>
#include "ui_widget_box.h"
#include "ui_widget_border.h"
#include "ui_widget_background.h"
#include "ui_widget_box_shadow.h"
#include "ui_widget_prototype.h"

// #define DEBUG_FRAME_RENDER
#define MAX_VISIBLE_WIDTH 20000
#define MAX_VISIBLE_HEIGHT 20000

#ifdef DEBUG_FRAME_RENDER
#endif

typedef struct ui_renderer_t {
        /* target widget position, it relative to root canvas */
        float x, y;

        /* content area top spacing, it relative to widget canvas */
        float content_top;

        /* content area left spacing, it relative to widget canvas */
        float content_left;

        /* target widget */
        ui_widget_t *target;

        /* computed actual style */
        ui_widget_actual_style_t *style;

        /* current target widget paint context */
        pd_context_t *paint;

        /* root paint context */
        pd_context_t *root_paint;

        /* content canvas */
        pd_canvas_t content_graph;

        /* target widget canvas */
        pd_canvas_t self_graph;

        /* layer canvas, used to mix content and self canvas with widget
         * opacity */
        pd_canvas_t layer_graph;

        /* actual paint rectangle in widget canvas rectangle, it relative to
         * root canvas */
        pd_rect_t actual_paint_rect;

        /* actual paint rectangle in widget content rectangle, it relative to
         * root canvas */
        pd_rect_t actual_content_rect;

        /* Paint rectangle in widget content rectangle, it relative to
         * root canvas */
        ui_rect_t content_rect;

        bool has_content_graph;
        bool has_self_graph;
        bool has_layer_graph;
        bool can_render_self;
        bool can_render_content;
} ui_renderer_t;

/** 判断部件是否有可绘制内容 */
static bool ui_widget_is_paintable(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;

        if (css_color_alpha(s->background_color) > 0 || s->background_image ||
            s->border_top_width > 0 || s->border_right_width > 0 ||
            s->border_bottom_width > 0 || s->border_left_width > 0 ||
            s->type_bits.box_shadow == CSS_BOX_SHADOW_SET) {
                return true;
        }
        return w->proto != ui_get_widget_prototype(NULL);
}

static bool ui_widget_has_round_border(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;

        return s->border_top_left_radius || s->border_top_right_radius ||
               s->border_bottom_left_radius || s->border_bottom_right_radius;
}

void ui_widget_expose_dirty_rect(ui_widget_t *w)
{
        while (w->parent) {
                w->parent->rendering.has_child_dirty_rect = true;
                w = w->parent;
        }
}

bool ui_widget_mark_dirty_rect(ui_widget_t *w, ui_rect_t *in_rect,
                               ui_box_type_t box_type)
{
        ui_rect_t rect;

        if (!ui_widget_is_visible(w)) {
                return false;
        }
        if (in_rect) {
                if (w->rendering.dirty_rect_type == UI_DIRTY_RECT_TYPE_FULL) {
                        return false;
                }
                rect = *in_rect;
                switch (box_type) {
                case UI_BOX_TYPE_GRAPH_BOX:
                        ui_rect_correct(&rect, w->canvas_box.width,
                                        w->canvas_box.height);
                        break;
                case UI_BOX_TYPE_BORDER_BOX:
                        ui_rect_correct(&rect, w->border_box.width,
                                        w->border_box.height);
                        rect.x += w->border_box.x - w->canvas_box.x;
                        rect.y += w->border_box.y - w->canvas_box.y;
                        break;
                case UI_BOX_TYPE_PADDING_BOX:
                        ui_rect_correct(&rect, w->padding_box.width,
                                        w->padding_box.height);
                        rect.x += w->padding_box.x - w->canvas_box.x;
                        rect.y += w->padding_box.y - w->canvas_box.y;
                        break;
                case UI_BOX_TYPE_CONTENT_BOX:
                default:
                        ui_rect_correct(&rect, w->content_box.width,
                                        w->content_box.height);
                        rect.x += w->content_box.x - w->canvas_box.x;
                        rect.y += w->content_box.y - w->canvas_box.y;
                        break;
                }
                if (w->rendering.dirty_rect_type > UI_DIRTY_RECT_TYPE_NONE) {
                        ui_rect_merge(&w->rendering.dirty_rect, &rect, &w->rendering.dirty_rect);
                } else {
                        w->rendering.dirty_rect = rect;
                }
                w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_CUSTOM;
        } else {
                w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
        }
        ui_widget_expose_dirty_rect(w);
        return true;
}

static void ui_widget_collect_dirty_rect(ui_widget_t *w, list_t *rects, float x,
                                         float y, ui_rect_t visible_area)
{
        ui_rect_t rect;
        pd_rect_t *actual_rect;
        list_node_t *node;

        do {
                if (w->parent && w->parent->rendering.dirty_rect_type ==
                                     UI_DIRTY_RECT_TYPE_FULL) {
                        w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
                        break;
                }
                if (w->rendering.dirty_rect_type == UI_DIRTY_RECT_TYPE_FULL) {
                        rect = w->canvas_box;
                } else if (w->rendering.dirty_rect_type ==
                           UI_DIRTY_RECT_TYPE_CUSTOM) {
                        rect = w->rendering.dirty_rect;
                        rect.x += w->canvas_box.x;
                        rect.y += w->canvas_box.y;
                } else {
                        break;
                }
                rect.x += x;
                rect.y += y;
                ui_rect_overlap(&rect, &visible_area, &rect);
                if (rect.width > 0 && rect.height > 0) {
                        actual_rect = malloc(sizeof(pd_rect_t));
                        ui_compute_rect(actual_rect, &rect);
                        list_append(rects, actual_rect);
                }
        } while (0);
        if (w->rendering.has_child_dirty_rect) {
                visible_area.x -= x;
                visible_area.y -= y;
                ui_rect_overlap(&visible_area, &w->padding_box, &visible_area);
                visible_area.x += x;
                visible_area.y += y;
                for (list_each(node, &w->stacking_context)) {
                        ui_widget_collect_dirty_rect(
                            node->data, rects, x + w->padding_box.x,
                            y + w->padding_box.y, visible_area);
                }
        }
        w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
        w->rendering.has_child_dirty_rect = false;
}

size_t ui_widget_get_dirty_rects(ui_widget_t *w, list_t *rects)
{
        pd_rect_t *rect;
        list_node_t *node;

        float scale = ui_metrics.scale;
        int x = y_iround(w->padding_box.x * scale);
        int y = y_iround(w->padding_box.y * scale);

        ui_widget_collect_dirty_rect(w, rects, 0, 0, w->padding_box);
        for (list_each(node, rects)) {
                rect = node->data;
                rect->x -= x;
                rect->y -= y;
        }
        return rects->length;
}

/** 当前部件的绘制函数 */
static void ui_widget_on_paint(ui_widget_t *w, pd_context_t *paint,
                               ui_widget_actual_style_t *style)
{
        ui_widget_paint_background(w, paint, style);
        ui_widget_paint_border(w, paint, style);
        ui_widget_paint_box_shadow(w, paint, style);
        if (w->proto && w->proto->paint) {
                w->proto->paint(w, paint, style);
        }
}

static ui_renderer_t *ui_renderer_create(ui_widget_t *w, pd_context_t *paint,
                                         ui_widget_actual_style_t *style,
                                         ui_renderer_t *parent)
{
        ui_renderer_t *that = malloc(sizeof(ui_renderer_t));

        if (!that) {
                return NULL;
        }
        that->target = w;
        that->style = style;
        that->paint = paint;
        that->has_self_graph = false;
        that->has_layer_graph = false;
        that->has_content_graph = false;
        if (parent) {
                that->root_paint = parent->root_paint;
                that->x = parent->x + parent->content_left + w->canvas_box.x;
                that->y = parent->y + parent->content_top + w->canvas_box.y;
        } else {
                that->x = that->y = 0;
                that->root_paint = that->paint;
        }
        if (w->computed_style.opacity < 1.0) {
                that->has_self_graph = true;
                that->has_content_graph = true;
                that->has_layer_graph = true;
        } else if (ui_widget_has_round_border(w)) {
                that->has_content_graph = true;
        }
        pd_canvas_init(&that->self_graph);
        pd_canvas_init(&that->layer_graph);
        pd_canvas_init(&that->content_graph);
        that->layer_graph.color_type = PD_COLOR_TYPE_ARGB;
        that->can_render_self = ui_widget_is_paintable(w);
        if (that->can_render_self) {
                that->self_graph.color_type = PD_COLOR_TYPE_ARGB;
                pd_canvas_create(&that->self_graph, that->paint->rect.width,
                                 that->paint->rect.height);
        }
        /* get content rectangle left spacing and top */
        that->content_left = w->padding_box.x - w->canvas_box.x;
        that->content_top = w->padding_box.y - w->canvas_box.y;
        /* convert position of paint rectangle to root canvas relative */
        that->actual_paint_rect.x =
            that->style->canvas_box.x + that->paint->rect.x;
        that->actual_paint_rect.y =
            that->style->canvas_box.y + that->paint->rect.y;
        that->actual_paint_rect.width = that->paint->rect.width;
        that->actual_paint_rect.height = that->paint->rect.height;
        DEBUG_MSG("[%s] paint_rect: (%d, %d, %d, %d)\n", w->id,
                  that->actual_paint_rect.x, that->actual_paint_rect.y,
                  that->actual_paint_rect.width,
                  that->actual_paint_rect.height);
        /* get actual paint rectangle in widget content rectangle */
        that->can_render_content =
            pd_rect_overlap(&that->style->padding_box, &that->actual_paint_rect,
                            &that->actual_content_rect);
        ;
        ui_convert_rect(&that->actual_content_rect, &that->content_rect,
                        1.0f / ui_metrics.scale);
        DEBUG_MSG("[%s] content_rect: (%d, %d, %d, %d)\n", w->id,
                  that->actual_content_rect.x, that->actual_content_rect.y,
                  that->actual_content_rect.width,
                  that->actual_content_rect.height);
        DEBUG_MSG("[%s] content_box: (%d, %d, %d, %d)\n", w->id,
                  style->content_box.x, style->content_box.y,
                  style->content_box.width, style->content_box.height);
        DEBUG_MSG("[%s] border_box: (%d, %d, %d, %d)\n", w->id,
                  style->border_box.x, style->border_box.y,
                  style->border_box.width, style->border_box.height);
        DEBUG_MSG(
            "[%s][%d/%d] content_rect: (%d,%d,%d,%d), "
            "canvas_rect: (%d,%d,%d,%d)\n",
            w->id, w->index, w->parent ? w->parent->stacking_context.length : 1,
            that->actual_content_rect.x, that->actual_content_rect.y,
            that->actual_content_rect.width, that->actual_content_rect.height,
            that->paint->canvas.quote.left, that->paint->canvas.quote.top,
            that->paint->canvas.width, that->paint->canvas.height);
        if (!that->can_render_content) {
                return that;
        }
        if (that->has_content_graph) {
                that->content_graph.color_type = PD_COLOR_TYPE_ARGB;
                pd_canvas_create(&that->content_graph,
                                 that->actual_content_rect.width,
                                 that->actual_content_rect.height);
        }
        return that;
}

static void ui_renderer_destroy(ui_renderer_t *renderer)
{
        pd_canvas_destroy(&renderer->layer_graph);
        pd_canvas_destroy(&renderer->self_graph);
        pd_canvas_destroy(&renderer->content_graph);
        free(renderer);
}

LIBUI_INLINE void ui_widget_compute_box(ui_widget_t *w,
                                        ui_widget_actual_style_t *s)
{
        s->content_box.x = ui_compute(s->x + w->content_box.x);
        s->content_box.y = ui_compute(s->y + w->content_box.y);
        s->content_box.width = ui_compute(w->content_box.width);
        s->content_box.height = ui_compute(w->content_box.height);

        s->padding_box.x = ui_compute(s->x + w->padding_box.x);
        s->padding_box.y = ui_compute(s->y + w->padding_box.y);
        s->padding_box.width = ui_compute(w->padding_box.width);
        s->padding_box.height = ui_compute(w->padding_box.height);

        s->border_box.x = ui_compute(s->x + w->border_box.x);
        s->border_box.y = ui_compute(s->y + w->border_box.y);
        s->border_box.width = ui_compute(w->border_box.width);
        s->border_box.height = ui_compute(w->border_box.height);

        s->canvas_box.x = ui_compute(s->x + w->canvas_box.x);
        s->canvas_box.y = ui_compute(s->y + w->canvas_box.y);
        s->canvas_box.width = ui_compute(w->canvas_box.width);
        s->canvas_box.height = ui_compute(w->canvas_box.height);
}

static size_t ui_renderer_render(ui_renderer_t *renderer);

static size_t ui_renderer_render_children(ui_renderer_t *that)
{
        size_t total = 0, count = 0;
        ui_widget_t *child;
        pd_rect_t paint_rect;
        ui_rect_t child_rect;
        list_node_t *node;
        pd_context_t child_paint;
        ui_renderer_t *renderer;
        ui_widget_actual_style_t style;

        /* Render the child widgets from bottom to top in stack order */
        for (list_each_reverse(node, &that->target->stacking_context)) {
                child = node->data;
                if (!ui_widget_is_visible(child) ||
                    child->state != UI_WIDGET_STATE_NORMAL) {
                        continue;
                }
                if (that->target->extra &&
                    that->target->extra->rules.max_render_children_count &&
                    count >
                        that->target->extra->rules.max_render_children_count) {
                        break;
                }
                /*
                 * The actual style calculation is time consuming, so here we
                 * use the existing properties to determine whether we need to
                 * render.
                 */
                style.x = that->x + that->content_left;
                style.y = that->y + that->content_top;
                child_rect.x = style.x + child->canvas_box.x;
                child_rect.y = style.y + child->canvas_box.y;
                child_rect.width = child->canvas_box.width;
                child_rect.height = child->canvas_box.height;
                if (!ui_rect_overlap(&that->content_rect, &child_rect,
                                     &child_rect)) {
                        continue;
                }
                ui_widget_compute_box(child, &style);
                DEBUG_MSG("content: %g, %g\n", that->content_left,
                          that->content_top);
                DEBUG_MSG("content rect: (%d, %d, %d, %d)\n",
                          that->actual_content_rect.x,
                          that->actual_content_rect.y,
                          that->actual_content_rect.width,
                          that->actual_content_rect.height);
                DEBUG_MSG("child canvas rect: (%d, %d, %d, %d)\n",
                          style.canvas_box.x, style.canvas_box.y,
                          style.canvas_box.width, style.canvas_box.height);
                if (!pd_rect_overlap(&that->actual_content_rect,
                                     &style.canvas_box, &paint_rect)) {
                        continue;
                }
                ++count;
                child_paint.rect = paint_rect;
                child_paint.rect.x -= style.canvas_box.x;
                child_paint.rect.y -= style.canvas_box.y;
                if (that->has_content_graph) {
                        child_paint.with_alpha = true;
                        paint_rect.x -= that->actual_content_rect.x;
                        paint_rect.y -= that->actual_content_rect.y;
                        pd_canvas_quote(&child_paint.canvas,
                                        &that->content_graph, &paint_rect);
                } else {
                        child_paint.with_alpha = that->paint->with_alpha;
                        paint_rect.x -= that->actual_paint_rect.x;
                        paint_rect.y -= that->actual_paint_rect.y;
                        pd_canvas_quote(&child_paint.canvas,
                                        &that->paint->canvas, &paint_rect);
                }
                DEBUG_MSG("child paint rect: (%d, %d, %d, %d)\n", paint_rect.x,
                          paint_rect.y, paint_rect.width, paint_rect.height);
                renderer =
                    ui_renderer_create(child, &child_paint, &style, that);
                total += ui_renderer_render(renderer);
                ui_renderer_destroy(renderer);
        }
        return total;
}

static size_t ui_renderer_render(ui_renderer_t *renderer)
{
        size_t count = 0;
        pd_context_t self_paint;
        ui_renderer_t *that = renderer;

        int content_x = that->actual_content_rect.x - that->actual_paint_rect.x;
        int content_y = that->actual_content_rect.y - that->actual_paint_rect.y;

#ifdef DEBUG_FRAME_RENDER
        char filename[256];
        static size_t frame = 0;
#endif
        DEBUG_MSG("[%d] %s: start render\n", that->target->index,
                  that->target->type);
        /* 如果部件有需要绘制的内容 */
        if (that->can_render_self) {
                count += 1;
                self_paint = *that->paint;
                self_paint.with_alpha = true;
                self_paint.canvas = that->self_graph;
                ui_widget_on_paint(that->target, &self_paint, that->style);
#ifdef DEBUG_FRAME_RENDER
                sprintf(filename,
                        "frame-%zd-L%d-%s-self-paint-(%d,%d,%d,%d).png",
                        frame++, __LINE__, renderer->target->id,
                        self_paint.rect.x, self_paint.rect.y,
                        self_paint.rect.width, self_paint.rect.height);
                pd_write_png_file(filename, &self_paint.canvas);
#endif
                /* 若不需要缓存自身位图则直接绘制到画布上 */
                if (!that->has_self_graph) {
                        pd_canvas_mix(&that->paint->canvas, &that->self_graph,
                                      0, 0, that->paint->with_alpha);
#ifdef DEBUG_FRAME_RENDER
                        sprintf(filename, "frame-%zd-L%d-%s-root-canvas.png",
                                frame++, __LINE__, renderer->target->id);
                        pd_write_png_file(filename, &that->root_paint->canvas);
#endif
                }
        }
        if (that->can_render_content) {
                count += ui_renderer_render_children(that);
        }
        if (that->has_content_graph &&
            ui_widget_has_round_border(that->target)) {
                self_paint.rect = that->actual_content_rect;
                self_paint.rect.x -= that->style->canvas_box.x;
                self_paint.rect.y -= that->style->canvas_box.y;
                self_paint.canvas = that->content_graph;
                ui_widget_crop_content(that->target, &self_paint, that->style);
        }
        if (!that->has_layer_graph) {
                if (that->has_content_graph) {
                        pd_canvas_mix(&that->paint->canvas,
                                      &that->content_graph, content_x,
                                      content_y, true);
                }
#ifdef DEBUG_FRAME_RENDER
                sprintf(filename, "frame-%zd-L%d-%s-canvas.png", frame++,
                        __LINE__, renderer->target->id);
                pd_write_png_file(filename, &that->paint->canvas);
                sprintf(filename, "frame-%zd-L%d-%s-root-canvas.png", frame++,
                        __LINE__, renderer->target->id);
                pd_write_png_file(filename, &that->root_paint->canvas);
#endif
                DEBUG_MSG("[%d] %s: end render, count: %lu\n",
                          that->target->index, that->target->type, count);
                return count;
        }
        /* 若需要绘制的是当前部件图层，则先混合部件自身位图和内容位图，得出当
         * 前部件的图层，然后将该图层混合到输出的位图中
         */
        if (that->can_render_self) {
                pd_canvas_copy(&that->layer_graph, &that->self_graph);
                pd_canvas_mix(&that->layer_graph, &that->content_graph,
                              content_x, content_y, true);
#ifdef DEBUG_FRAME_RENDER
                sprintf(filename, "frame-%zd-L%d-%s-content-graph-%d-%d.png",
                        frame++, __LINE__, renderer->target->id, content_x,
                        content_y);
                pd_write_png_file(filename, &that->content_graph);
                sprintf(filename, "frame-%zd-L%d-%s-self-graph.png", frame++,
                        __LINE__, renderer->target->id);
                pd_write_png_file(filename, &that->layer_graph);
#endif
        } else {
                pd_canvas_create(&that->layer_graph, that->paint->rect.width,
                                 that->paint->rect.height);
                pd_canvas_replace(&that->layer_graph, &that->content_graph,
                                  content_x, content_y);
        }
        that->layer_graph.opacity = that->target->computed_style.opacity;
        pd_canvas_mix(&that->paint->canvas, &that->layer_graph, 0, 0,
                      that->paint->with_alpha);
#ifdef DEBUG_FRAME_RENDER
        sprintf(filename, "frame-%zd-%s-layer.png", frame++,
                renderer->target->id);
        pd_write_png_file(filename, &that->layer_graph);
        sprintf(filename, "frame-%zd-L%d-%s-root-canvas.png", frame++, __LINE__,
                renderer->target->id);
        pd_write_png_file(filename, &that->root_paint->canvas);
#endif
        DEBUG_MSG("[%d] %s: end render, count: %lu\n", that->target->index,
                  that->target->type, count);
        return count;
}

size_t ui_widget_render(ui_widget_t *w, pd_context_t *paint)
{
        size_t count;
        ui_renderer_t *ctx;
        ui_widget_actual_style_t style;

        /* reset widget position to relative paint rect */
        style.x = -1.f * ui_compute(w->canvas_box.x);
        style.y = -1.f * ui_compute(w->canvas_box.y);
        ui_widget_compute_box(w, &style);
        ctx = ui_renderer_create(w, paint, &style, NULL);
        DEBUG_MSG("[%d] %s: start render\n", ctx->target->index,
                  ctx->target->type);
        count = ui_renderer_render(ctx);
        DEBUG_MSG("[%d] %s: end render, count: %lu\n", ctx->target->index,
                  ctx->target->type, count);
        ui_renderer_destroy(ctx);
        return count;
}
