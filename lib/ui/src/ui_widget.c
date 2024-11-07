/*
 * lib/ui/src/ui_widget.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <string.h>
#include <assert.h>
#include <css/style_value.h>
#include <css/computed.h>
#include <ui.h>
#include <ui/mutation_observer.h>
#include "ui_widget.h"
#include "ui_widget_id.h"
#include "ui_widget_attributes.h"
#include "ui_widget_classes.h"
#include "ui_widget_prototype.h"
#include "ui_widget_status.h"
#include "ui_widget_observer.h"
#include "ui_widget_style.h"
#include "ui_events.h"

#define SHADOW_WIDTH(s) (s->box_shadow_blur + s->box_shadow_spread)

static void ui_widget_destroy_children(ui_widget_t *w);

static void ui_widget_init(ui_widget_t *w)
{
        memset(w, 0, sizeof(ui_widget_t));
        w->tab_index = -1;
        w->state = UI_WIDGET_STATE_CREATED;
        list_create(&w->children);
        list_create(&w->stacking_context);
        w->node.data = w;
        w->node_show.data = w;
        w->node.next = w->node.prev = NULL;
        w->node_show.next = w->node_show.prev = NULL;
}

void ui_widget_destroy(ui_widget_t *w)
{
        if (w->parent) {
                ui_widget_request_reflow(w->parent);
                ui_widget_unlink(w);
        }
        ui_widget_destroy_listeners(w);
        ui_widget_destroy_children(w);
        ui_widget_destroy_prototype(w);
        if (w->title) {
                free(w->title);
                w->title = NULL;
        }
        ui_widget_destroy_id(w);
        ui_widget_destroy_style(w);
        ui_widget_destroy_attrs(w);
        ui_widget_destroy_classes(w);
        ui_widget_destroy_status(w);
        free(w->extra);
        free(w);
}

static void ui_widget_on_destroy_child(void *arg)
{
        ui_widget_destroy(arg);
}

static void ui_widget_destroy_children(ui_widget_t *w)
{
        /* 先释放显示列表，后销毁部件列表，因为部件在这两个链表中的节点是和它共用
         * 一块内存空间的，销毁部件列表会把部件释放掉，所以把这个操作放在后面 */
        list_destroy_without_node(&w->stacking_context, NULL);
        list_destroy_without_node(&w->children, ui_widget_on_destroy_child);
}

ui_widget_t *ui_create_widget(const char *type)
{
        ui_widget_t *widget = malloc(sizeof(ui_widget_t));

        ui_widget_init(widget);
        widget->proto = ui_get_widget_prototype(type);
        if (widget->proto->name) {
                widget->type = widget->proto->name;
        } else if (type) {
                widget->type = strdup2(type);
        }
        widget->proto->init(widget);
        ui_widget_request_refresh_style(widget);
        return widget;
}

ui_widget_t *ui_create_widget_with_prototype(const ui_widget_prototype_t *proto)
{
        ui_widget_t *widget = malloc(sizeof(ui_widget_t));

        ui_widget_init(widget);
        widget->proto = proto;
        widget->type = widget->proto->name;
        widget->proto->init(widget);
        ui_widget_request_refresh_style(widget);
        return widget;
}

void ui_widget_add_state(ui_widget_t *w, ui_widget_state_t state)
{
        /* 如果部件还处于未准备完毕的状态 */
        if (w->state < UI_WIDGET_STATE_READY) {
                w->state |= state;
                /* 如果部件已经准备完毕则触发 ready 事件 */
                if (w->state == UI_WIDGET_STATE_READY) {
                        ui_event_t e = { 0 };
                        e.type = UI_EVENT_READY;
                        e.cancel_bubble = true;
                        ui_widget_post_event(w, &e, NULL, NULL);
                        w->state = UI_WIDGET_STATE_NORMAL;
                }
        }
}

void ui_widget_set_title(ui_widget_t *w, const wchar_t *title)
{
        size_t len;
        wchar_t *new_title, *old_title;
        ui_mutation_record_t *record;

        len = wcslen(title) + 1;
        new_title = (wchar_t *)malloc(sizeof(wchar_t) * len);
        if (!new_title) {
                return;
        }
        wcsncpy(new_title, title, len);
        old_title = w->title;
        w->title = new_title;
        if (old_title) {
                free(old_title);
        }
        if (ui_widget_has_observer(w, UI_MUTATION_RECORD_TYPE_PROPERTIES)) {
                record = ui_mutation_record_create(
                    w, UI_MUTATION_RECORD_TYPE_PROPERTIES);
                record->property_name = strdup2("title");
                ui_widget_add_mutation_record(w, record);
                ui_mutation_record_destroy(record);
        }
}

void ui_widget_set_text(ui_widget_t *w, const char *text)
{
        if (w->proto && w->proto->settext) {
                w->proto->settext(w, text);
        }
}

ui_widget_extra_data_t *ui_create_extra_data(ui_widget_t *widget)
{
        widget->extra = calloc(sizeof(ui_widget_extra_data_t), 1);
        if (widget->extra) {
                return widget->extra;
        }
        return NULL;
}

void ui_widget_get_offset(ui_widget_t *w, ui_widget_t *parent, float *offset_x,
                          float *offset_y)
{
        float x = 0, y = 0;
        while (w != parent) {
                x += w->border_box.x;
                y += w->border_box.y;
                w = w->parent;
                if (w) {
                        x += w->padding_box.x - w->border_box.x;
                        y += w->padding_box.y - w->border_box.y;
                } else {
                        break;
                }
        }
        if (offset_x) {
                *offset_x = x;
        }
        if (offset_y) {
                *offset_y = y;
        }
}

bool ui_widget_in_viewport(ui_widget_t *w)
{
        list_node_t *node;
        ui_rect_t rect;
        ui_widget_t *self, *parent, *child;
        css_computed_style_t *style;

        rect = w->padding_box;
        /* If the size of the widget is not fixed, then set the maximum size to
         * avoid it being judged invisible all the time. */
        if (rect.width < 1 && !IS_CSS_FIXED_LENGTH(&w->computed_style, width)) {
                rect.width = w->parent->padding_box.width;
        }
        if (rect.height < 1 &&
            !IS_CSS_FIXED_LENGTH(&w->computed_style, height)) {
                rect.height = w->parent->padding_box.height;
        }
        for (self = w, parent = w->parent; parent;
             self = parent, parent = parent->parent) {
                if (!ui_widget_is_visible(parent)) {
                        return false;
                }
                for (node = self->node_show.prev; node && node->prev;
                     node = node->prev) {
                        child = node->data;
                        style = &child->computed_style;
                        if (child->state < UI_WIDGET_STATE_LAYOUTED ||
                            child == self || !ui_widget_is_visible(child)) {
                                continue;
                        }
                        DEBUG_MSG("rect: (%g,%g,%g,%g), child rect: "
                                  "(%g,%g,%g,%g), child: %s %s\n",
                                  rect.x, rect.y, rect.width, rect.height,
                                  child->border_box.x, child->border_box.y,
                                  child->border_box.width,
                                  child->border_box.height, child->type,
                                  child->id);
                        if (!ui_rect_is_include(&child->border_box, &rect)) {
                                continue;
                        }
                        if (style->opacity == 1.0f &&
                            css_color_alpha(style->background_color) == 255) {
                                return false;
                        }
                }
                rect.x += parent->padding_box.x;
                rect.y += parent->padding_box.y;
                ui_rect_correct(&rect, parent->padding_box.width,
                                parent->padding_box.height);
                if (rect.width < 1 || rect.height < 1) {
                        return false;
                }
        }
        return true;
}

static void ui_widget_update_canvas_box_width(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;
        const float x = fabsf(s->box_shadow_x);

        w->canvas_box.width =
            w->border_box.width + SHADOW_WIDTH(s) + y_max(x, SHADOW_WIDTH(s));
}

static void ui_widget_update_canvas_box_height(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;
        const float y = fabsf(s->box_shadow_x);

        w->canvas_box.height =
            w->border_box.height + SHADOW_WIDTH(s) + y_max(y, SHADOW_WIDTH(s));
}

static void ui_widget_update_canvas_box_x(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;

        w->canvas_box.x =
            w->border_box.x - y_max(0, SHADOW_WIDTH(s) - s->box_shadow_x);
}

static void ui_widget_update_canvas_box_y(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;

        w->canvas_box.y =
            w->border_box.y - y_max(0, SHADOW_WIDTH(s) - s->box_shadow_y);
}

void ui_widget_update_box_position(ui_widget_t *w)
{
        float x = w->layout_x;
        float y = w->layout_y;
        const css_computed_style_t *s = &w->computed_style;

        switch (s->type_bits.position) {
        case CSS_POSITION_ABSOLUTE:
        case CSS_POSITION_FIXED:
                if (IS_CSS_FIXED_LENGTH(s, left)) {
                        x = s->left;
                } else if (IS_CSS_FIXED_LENGTH(s, right)) {
                        if (w->parent) {
                                x = w->parent->border_box.width -
                                    w->border_box.width;
                        }
                        x -= s->right;
                } else {
                        x = 0;
                }
                if (IS_CSS_FIXED_LENGTH(s, top)) {
                        y = s->top;
                } else if (IS_CSS_FIXED_LENGTH(s, bottom)) {
                        if (w->parent) {
                                y = w->parent->border_box.height -
                                    w->border_box.height;
                        }
                        y -= s->bottom;
                } else {
                        y = 0;
                }
                break;
        case CSS_POSITION_RELATIVE:
                if (IS_CSS_FIXED_LENGTH(s, left)) {
                        x += s->left;
                } else if (IS_CSS_FIXED_LENGTH(s, right)) {
                        x -= s->right;
                }
                if (IS_CSS_FIXED_LENGTH(s, top)) {
                        y += s->top;
                } else if (IS_CSS_FIXED_LENGTH(s, bottom)) {
                        y -= s->bottom;
                }
                break;
        case CSS_POSITION_STATIC:
        default:
                break;
        }
        w->outer_box.x = x;
        w->outer_box.y = y;
        w->border_box.x = x + s->margin_left;
        w->border_box.y = y + s->margin_top;
        w->padding_box.x = w->border_box.x + s->border_left_width;
        w->padding_box.y = w->border_box.y + s->border_top_width;
        w->content_box.x = w->padding_box.x + s->padding_left;
        w->content_box.y = w->padding_box.y + s->padding_top;
        ui_widget_update_canvas_box_x(w);
        ui_widget_update_canvas_box_y(w);
}

void ui_widget_update_box_width(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;

        if (s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                w->content_box.width =
                    s->width - css_padding_x(s) - css_border_x(s);
                w->border_box.width = s->width;
        } else {
                w->content_box.width = s->width;
                w->border_box.width =
                    s->width + css_padding_x(s) + css_border_x(s);
        }
        w->padding_box.width = w->content_box.width + css_padding_x(s);
        w->outer_box.width = w->border_box.width + css_margin_x(s);
        ui_widget_update_canvas_box_width(w);
}

void ui_widget_update_box_height(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;

        if (s->type_bits.box_sizing == CSS_BOX_SIZING_BORDER_BOX) {
                w->content_box.height =
                    s->height - css_padding_y(s) - css_border_y(s);
                w->border_box.height = s->height;
        } else {
                w->content_box.height = s->height;
                w->border_box.height =
                    s->height + css_padding_y(s) + css_border_y(s);
        }
        w->padding_box.height = w->content_box.height + css_padding_y(s);
        w->outer_box.height = w->border_box.height + css_margin_y(s);
        ui_widget_update_canvas_box_height(w);
}

void ui_widget_update_box_size(ui_widget_t *w)
{
        ui_widget_update_box_width(w);
        ui_widget_update_box_height(w);
}

void ui_widget_set_computed_width(ui_widget_t *w, float width)
{
        CSS_SET_FIXED_LENGTH(&w->computed_style, width,
                             ui_widget_fix_width(w, width));
}

void ui_widget_set_computed_height(ui_widget_t *w, float height)
{
        CSS_SET_FIXED_LENGTH(&w->computed_style, height,
                             ui_widget_fix_height(w, height));
}

void ui_widget_set_content_width(ui_widget_t *w, float width)
{
        ui_widget_set_computed_width(
            w, css_content_box_width_to_width(&w->computed_style, width));
        ui_widget_update_box_width(w);
}

void ui_widget_set_content_height(ui_widget_t *w, float height)
{
        ui_widget_set_computed_height(
            w, css_content_box_height_to_height(&w->computed_style, height));
        ui_widget_update_box_height(w);
}

void ui_widget_get_hint_content_size(ui_widget_t *w, float *width,
                                     float *height)
{
        float available_width;
        css_computed_style_t *s = &w->computed_style;
        css_computed_style_t *ps;

        w->proto->autosize(w, width, height);
        if (!w->parent || s->type_bits.position == CSS_POSITION_FIXED ||
            s->type_bits.position == CSS_POSITION_ABSOLUTE ||
            (s->type_bits.display != CSS_DISPLAY_BLOCK &&
             s->type_bits.display != CSS_DISPLAY_FLEX)) {
                return;
        }
        ps = &w->parent->computed_style;
        if (ps->type_bits.display != CSS_DISPLAY_BLOCK &&
            ps->type_bits.display != CSS_DISPLAY_INLINE_BLOCK) {
                return;
        }
        available_width = css_width_to_content_box_width(
            s, w->parent->content_box.width - css_margin_x(s));
        if (available_width > *width) {
                *width = available_width;
        }
}
