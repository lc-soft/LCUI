/*
 * src/widgets/scrollarea.c
 *
 * Copyright (c) 2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui.h>
#include <LCUI/widgets/scrollarea.h>

typedef struct ui_scrollarea {
        float scroll_top;
        float scroll_left;
        float scroll_width;
        float scroll_height;
        float old_scroll_top;
        float old_scroll_left;
        int touch_point_id;
        bool is_draggable;
        bool is_dragging;
        float mouse_x, mouse_y;
        ui_scrollarea_direction_t direction;
        ui_mutation_observer_t *observer;
} ui_scrollarea_t;

typedef struct ui_scrollarea_content {
        ui_mutation_observer_t *observer;
} ui_scrollarea_content_t;

static ui_widget_prototype_t *ui_scrollarea_proto;
static ui_widget_prototype_t *ui_scrollarea_content_proto;

static const char *ui_scrollarea_css ="\
scrollarea-content {\
    min-width: min-content;\
}";

static void ui_scrollarea_emit_scroll(ui_widget_t *w)
{
        ui_event_t e = { 0 };

        ui_event_init(&e, "scroll");
        e.cancel_bubble = true;
        ui_widget_emit_event(w, e, NULL);
}

static void ui_scrollarea_content_update(ui_widget_t *w)
{
        if (w->parent) {
                ui_scrollarea_update(w->parent);
                ui_scrollarea_emit_scroll(w->parent);
        }
}

static void ui_scrollarea_content_destroy(ui_widget_t *w)
{
        ui_scrollarea_content_t *that =
            ui_widget_get_data(w, ui_scrollarea_content_proto);

        ui_mutation_observer_destroy(that->observer);
}

static void ui_scrollarea_content_on_mutation(ui_mutation_list_t *list,
                                              ui_mutation_observer_t *observer,
                                              void *arg)
{
        list_node_t *node;
        ui_mutation_record_t *mutation;

        for (list_each(node, list)) {
                mutation = node->data;
                if (mutation->type == UI_MUTATION_RECORD_TYPE_PROPERTIES &&
                    (strcmp(mutation->property_name, "width") == 0 ||
                     strcmp(mutation->property_name, "height") == 0)) {
                        ui_scrollarea_content_update(arg);
                        break;
                }
        }
}

static void ui_scrollarea_content_on_link(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        ui_scrollarea_content_update(w);
}

static void ui_scrollarea_content_init(ui_widget_t *w)
{
        ui_mutation_observer_init_t options = { .properties = true };
        ui_scrollarea_content_t *that = ui_widget_add_data(
            w, ui_scrollarea_content_proto, sizeof(ui_scrollarea_content_t));

        that->observer =
            ui_mutation_observer_create(ui_scrollarea_content_on_mutation, w);
        ui_widget_on(w, "link", ui_scrollarea_content_on_link, NULL);
        ui_mutation_observer_observe(that->observer, w, options);
}

void ui_scrollarea_set_wheel_scroll_direction(
    ui_widget_t *w, ui_scrollarea_direction_t direction)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        that->direction = direction;
}

void ui_scrollarea_update(ui_widget_t *w)
{
        list_node_t *node;
        ui_widget_t *content = NULL;
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        for (list_each(node, &w->children)) {
                if (ui_check_widget_prototype(node->data,
                                              ui_scrollarea_content_proto)) {
                        content = node->data;
                        break;
                }
        }
        if (!content) {
                logger_error(
                    "[ui-scrollarea] <scrollarea-content> not found\n");
                return;
        }
        that->scroll_width = content->outer_box.width;
        that->scroll_height = content->outer_box.height;
        if (that->scroll_top + w->content_box.height > that->scroll_height) {
                that->scroll_top = that->scroll_height - w->content_box.height;
        }
        if (that->scroll_top < 0) {
                that->scroll_top = 0;
        }
        if (that->scroll_left + w->content_box.width > that->scroll_width) {
                that->scroll_left = that->scroll_width - w->content_box.width;
        }
        if (that->scroll_left < 0) {
                that->scroll_left = 0;
        }
        DEBUG_MSG("[ui-scrollarea] scroll_width=%g, scroll_height=%g, "
                  "scroll_top=%g, scroll_left=%g\n",
                  that->scroll_width, that->scroll_height, that->scroll_top,
                  that->scroll_left);
        ui_widget_set_style_keyword_value(content, css_prop_position,
                                          CSS_KEYWORD_RELATIVE);
        ui_widget_set_style_unit_value(content, css_prop_top, -that->scroll_top,
                                       CSS_UNIT_PX);
        ui_widget_set_style_unit_value(content, css_prop_left,
                                       -that->scroll_left, CSS_UNIT_PX);
        ui_widget_request_reflow(w);
}

static void ui_scrollarea_on_mutation(ui_mutation_list_t *list,
                                      ui_mutation_observer_t *observer,
                                      void *arg)
{
        list_node_t *node;
        ui_mutation_record_t *mutation;

        for (list_each(node, list)) {
                mutation = node->data;
                if (mutation->type == UI_MUTATION_RECORD_TYPE_PROPERTIES &&
                    (strcmp(mutation->property_name, "width") == 0 ||
                     strcmp(mutation->property_name, "height") == 0)) {
                        ui_scrollarea_update(arg);
                        ui_scrollarea_emit_scroll(arg);
                        break;
                }
        }
}

static void ui_scrollarea_on_wheel(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        switch (that->direction) {
        case UI_SCROLLAREA_VERTICAL:
                that->scroll_top -= e->wheel.delta_y;
                break;
        case UI_SCROLLAREA_HORIZONTAL:
                that->scroll_left -= e->wheel.delta_y;
                break;
        default:
                that->scroll_top -= e->wheel.delta_y;
                that->scroll_left -= e->wheel.delta_x;
                break;
        }
        ui_scrollarea_update(w);
        ui_scrollarea_emit_scroll(w);
}

static void ui_scrollarea_on_touch(ui_widget_t *w, ui_event_t *e, void *arg)
{
        unsigned i;
        ui_touch_point_t *point;
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        if (e->touch.n_points < 1) {
                return;
        }
        if (that->touch_point_id == -1) {
                point = &e->touch.points[0];
                /* 如果这个触点的状态不是 TOUCHDOWN，则说明是上次触控拖拽操
                 * 作时的多余触点，直接忽略这次触控事件 */
                if (point->state != UI_EVENT_TOUCHDOWN) {
                        return;
                }
                that->touch_point_id = point->id;
        } else {
                for (point = NULL, i = 0; i < e->touch.n_points; ++i) {
                        point = &e->touch.points[i];
                        if (point->id == that->touch_point_id) {
                                break;
                        }
                }
                if (!point) {
                        return;
                }
        }
        switch (point->state) {
        case UI_EVENT_TOUCHDOWN:
                if (that->is_dragging) {
                        return;
                }
                that->old_scroll_top = that->scroll_top;
                that->old_scroll_left = that->scroll_left;
                that->mouse_x = point->x;
                that->mouse_y = point->y;
                that->is_draggable = true;
                break;
        case UI_EVENT_TOUCHUP:
                that->touch_point_id = -1;
                that->is_dragging = false;
                ui_widget_release_touch_capture(w, -1);
                ui_widget_block_event(w, false);
                break;
        case UI_EVENT_TOUCHMOVE:
                if (!that->is_draggable) {
                        break;
                }
                e->cancel_bubble = true;
                that->scroll_top =
                    that->old_scroll_top + that->mouse_y - point->y;
                that->scroll_left =
                    that->old_scroll_left + that->mouse_x - point->x;
                ui_scrollarea_update(w);
                ui_scrollarea_emit_scroll(w);
                if (!that->is_dragging) {
                        that->is_dragging = true;
                        ui_clear_event_target(NULL);
                        ui_widget_block_event(w, true);
                        ui_widget_set_touch_capture(w, point->id);
                        break;
                }
        default:
                break;
        }
}

static void ui_scrollarea_init(ui_widget_t *w)
{
        ui_scrollarea_t *that = UI_WDIGET_ADD_DATA(w, ui_scrollarea);

        that->scroll_top = 0;
        that->scroll_left = 0;
        that->scroll_width = 0;
        that->scroll_height = 0;
        that->old_scroll_top = 0;
        that->old_scroll_left = 0;
        that->touch_point_id = -1;
        that->is_draggable = false;
        that->is_dragging = false;
        that->mouse_x = 0;
        that->mouse_y = 0;
        that->direction = UI_SCROLLAREA_VERTICAL;
        that->observer =
            ui_mutation_observer_create(ui_scrollarea_on_mutation, w);
        ui_mutation_observer_observe(
            that->observer, w,
            (ui_mutation_observer_init_t){ .properties = true });
        ui_widget_on(w, "mousewheel", ui_scrollarea_on_wheel, NULL);
        ui_widget_on(w, "touchdown", ui_scrollarea_on_touch, NULL);
        ui_widget_on(w, "touchmove", ui_scrollarea_on_touch, NULL);
        ui_widget_on(w, "touchup", ui_scrollarea_on_touch, NULL);
}

static void ui_scrollarea_destroy(ui_widget_t *w)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        ui_mutation_observer_destroy(that->observer);
}

void ui_scrollarea_set_scroll_top(ui_widget_t *w, float value)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        that->scroll_top = value;
        ui_scrollarea_update(w);
        ui_scrollarea_emit_scroll(w);
}

void ui_scrollarea_set_scroll_left(ui_widget_t *w, float value)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        that->scroll_left = value;
        ui_scrollarea_update(w);
        ui_scrollarea_emit_scroll(w);
}

float ui_scrollarea_get_scroll_top(ui_widget_t *w)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        return that->scroll_top;
}

float ui_scrollarea_get_scroll_left(ui_widget_t *w)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        return that->scroll_left;
}

float ui_scrollarea_get_scroll_width(ui_widget_t *w)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        return that->scroll_width;
}

float ui_scrollarea_get_scroll_height(ui_widget_t *w)
{
        ui_scrollarea_t *that = ui_widget_get_data(w, ui_scrollarea_proto);

        return that->scroll_height;
}

ui_widget_t *ui_create_scrollarea(void)
{
        return ui_create_widget_with_prototype(ui_scrollarea_proto);
}

ui_widget_t *ui_create_scrollarea_content(void)
{
        return ui_create_widget_with_prototype(ui_scrollarea_content_proto);
}

void ui_register_scrollarea(void)
{
        ui_scrollarea_proto = ui_create_widget_prototype("scrollarea", NULL);
        ui_scrollarea_proto->init = ui_scrollarea_init;
        ui_scrollarea_proto->destroy = ui_scrollarea_destroy;

        ui_scrollarea_content_proto =
            ui_create_widget_prototype("scrollarea-content", NULL);
        ui_scrollarea_content_proto->init = ui_scrollarea_content_init;
        ui_scrollarea_content_proto->destroy = ui_scrollarea_content_destroy;
        ui_load_css_string(ui_scrollarea_css, __FILE__);
}
