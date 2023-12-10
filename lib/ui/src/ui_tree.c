/*
 * lib/ui/src/ui_tree.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <assert.h>
#include <string.h>
#include <css/selector.h>
#include <ui/base.h>
#include <ui/style.h>
#include <ui/events.h>
#include <ui/rect.h>
#include <ui/mutation_observer.h>
#include "ui_widget_observer.h"
#include "ui_trash.h"

#define TYPE_CHILD_LIST UI_MUTATION_RECORD_TYPE_CHILD_LIST

int ui_widget_append(ui_widget_t *parent, ui_widget_t *widget)
{
        ui_event_t ev = { 0 };
        ui_mutation_record_t *record;

        if (!parent || !widget) {
                return -1;
        }
        if (parent == widget) {
                return -2;
        }
        ui_widget_unlink(widget);
        widget->parent = parent;
        widget->state = UI_WIDGET_STATE_CREATED;
        widget->index = parent->children.length;
        widget->parent->update.should_update_children = true;
        list_append_node(&parent->children, &widget->node);
        ev.cancel_bubble = true;
        ev.type = UI_EVENT_LINK;
        ui_widget_request_refresh_style(widget);
        ui_widget_refresh_children_style(widget);
        ui_widget_emit_event(widget, ev, NULL);
        ui_widget_update_status(widget);
        ui_widget_request_reflow(parent);
        if (ui_widget_has_observer(parent, TYPE_CHILD_LIST)) {
                record = ui_mutation_record_create(widget, TYPE_CHILD_LIST);
                list_append(&record->added_widgets, widget);
                ui_widget_add_mutation_record(parent, record);
                ui_mutation_record_destroy(record);
        }
        return 0;
}

int ui_widget_prepend(ui_widget_t *parent, ui_widget_t *widget)
{
        ui_widget_t *child;
        ui_mutation_record_t *record;
        ui_event_t ev = { 0 };
        list_node_t *node;

        if (!parent || !widget) {
                return -1;
        }
        if (parent == widget) {
                return -2;
        }
        child = widget->parent;
        ui_widget_unlink(widget);
        widget->index = 0;
        widget->parent = parent;
        widget->state = UI_WIDGET_STATE_CREATED;
        widget->parent->update.should_update_children = true;
        node = &widget->node;
        list_insert_node(&parent->children, 0, node);
        /** 修改它后面的部件的 index 值 */
        node = node->next;
        while (node) {
                child = node->data;
                child->index += 1;
                node = node->next;
        }
        ev.cancel_bubble = true;
        ev.type = UI_EVENT_LINK;
        ui_widget_emit_event(widget, ev, NULL);
        ui_widget_request_refresh_children(widget);
        ui_widget_update_status(widget);
        ui_widget_request_reflow(parent);
        if (ui_widget_has_observer(parent, TYPE_CHILD_LIST)) {
                record = ui_mutation_record_create(widget, TYPE_CHILD_LIST);
                list_append(&record->added_widgets, widget);
                ui_widget_add_mutation_record(parent, record);
                ui_mutation_record_destroy(record);
        }
        return 0;
}

int ui_widget_unwrap(ui_widget_t *w)
{
        size_t len;
        ui_widget_t *child;
        ui_event_t ev = { 0 };
        ui_mutation_record_t *record;
        list_t *children;
        list_node_t *target, *node, *prev;

        if (!w->parent) {
                return -1;
        }
        if (ui_widget_has_observer(w, TYPE_CHILD_LIST)) {
                record = ui_mutation_record_create(w, TYPE_CHILD_LIST);
                for (list_each(node, &w->children)) {
                        list_append(&record->removed_widgets, node->data);
                }
                ui_widget_add_mutation_record(w, record);
                ui_mutation_record_destroy(record);
        }
        if (ui_widget_has_observer(w->parent, TYPE_CHILD_LIST)) {
                record = ui_mutation_record_create(w->parent, TYPE_CHILD_LIST);
                for (list_each(node, &w->children)) {
                        list_append(&record->added_widgets, node->data);
                }
                ui_widget_add_mutation_record(w->parent, record);
                ui_mutation_record_destroy(record);
        }
        children = &w->parent->children;
        len = w->children.length;
        if (len > 0) {
                node = list_get_node(&w->children, 0);
                ui_widget_remove_status(node->data, "first-child");
                node = list_get_last_node(&w->children);
                ui_widget_remove_status(node->data, "last-child");
        }
        node = &w->node;
        target = node->prev;
        node = w->children.tail.prev;
        ev.cancel_bubble = true;
        while (len > 0) {
                assert(node != NULL);
                assert(node->data != NULL);
                prev = node->prev;
                child = node->data;
                ev.type = UI_EVENT_UNLINK;
                ui_widget_emit_event(child, ev, NULL);
                list_unlink(&w->children, node);
                list_link(children, target, node);
                child->parent = w->parent;
                ev.type = UI_EVENT_LINK;
                ui_widget_emit_event(child, ev, NULL);
                ui_widget_request_refresh_children(child);
                node = prev;
                --len;
        }
        if (w->index == 0) {
                ui_widget_add_status(target->next->data, "first-child");
        }
        if (w->index == children->length - 1) {
                node = list_get_last_node(children);
                ui_widget_add_status(node->data, "last-child");
        }
        ui_widget_remove(w);
        return 0;
}

int ui_widget_unlink(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_event_t ev = { 0 };
        ui_mutation_record_t *record;
        list_node_t *node;

        if (!w->parent) {
                return -1;
        }
        node = &w->node;
        if (w->index == w->parent->children.length - 1) {
                ui_widget_remove_status(w, "last-child");
                child = ui_widget_prev(w);
                if (child) {
                        ui_widget_add_status(child, "last-child");
                }
        }
        if (w->index == 0) {
                ui_widget_remove_status(w, "first-child");
                child = ui_widget_next(w);
                if (child) {
                        ui_widget_add_status(child, "first-child");
                }
        }
        /** 修改它后面的部件的 index 值 */
        node = node->next;
        while (node) {
                child = node->data;
                child->index -= 1;
                node = node->next;
        }
        node = &w->node;
        ev.cancel_bubble = true;
        ev.type = UI_EVENT_UNLINK;
        ui_widget_emit_event(w, ev, NULL);
        list_unlink(&w->parent->children, node);
        list_unlink(&w->parent->stacking_context, &w->node_show);
        ui_widget_request_reflow(w->parent);
        if (ui_widget_has_observer(w->parent, TYPE_CHILD_LIST)) {
                record = ui_mutation_record_create(w->parent, TYPE_CHILD_LIST);
                list_append(&record->removed_widgets, w->parent);
                ui_widget_add_mutation_record(w->parent, record);
                ui_mutation_record_destroy(record);
        }
        w->parent = NULL;
        return 0;
}

void ui_widget_empty(ui_widget_t *w)
{
        ui_widget_t *child;
        list_node_t *node;
        ui_event_t ev = { 0 };
        ui_mutation_record_t *record;

        ui_event_init(&ev, "unlink");
        if (ui_widget_has_observer(w, TYPE_CHILD_LIST)) {
                record = ui_mutation_record_create(w->parent, TYPE_CHILD_LIST);
                for (list_each(node, &w->children)) {
                        list_append(&record->removed_widgets, node->data);
                }
                ui_widget_add_mutation_record(w, record);
                ui_mutation_record_destroy(record);
        }
        while ((node = list_get_first_node(&w->children)) != NULL) {
                child = node->data;
                list_unlink(&w->children, node);
                ui_widget_emit_event(child, ev, NULL);
                ui_trash_add(child);
        }
        list_destroy_without_node(&w->stacking_context, NULL);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
        ui_widget_request_refresh_style(w);
}

void ui_widget_remove(ui_widget_t *w)
{
        ui_widget_t *child;
        list_node_t *node;

        assert(w->state != UI_WIDGET_STATE_DELETED);
        if (!w->parent) {
                ui_trash_add(w);
                return;
        }
        /* Update the index of the siblings behind it */
        node = w->node.next;
        while (node) {
                child = node->data;
                child->index -= 1;
                node = node->next;
        }
        if (w->computed_style.type_bits.position != CSS_POSITION_ABSOLUTE) {
                ui_widget_request_reflow(w->parent);
        }
        ui_widget_mark_dirty_rect(w->parent, &w->canvas_box,
                                  UI_BOX_TYPE_CONTENT_BOX);
        ui_widget_unlink(w);
        ui_trash_add(w);
}

ui_widget_t *ui_widget_prev(ui_widget_t *w)
{
        list_node_t *node = &w->node;
        if (node->prev && node != w->parent->children.head.next) {
                return node->prev->data;
        }
        return NULL;
}

ui_widget_t *ui_widget_next(ui_widget_t *w)
{
        list_node_t *node = &w->node;
        if (node->next) {
                return node->next->data;
        }
        return NULL;
}

ui_widget_t *ui_widget_get_child(ui_widget_t *w, size_t index)
{
        list_node_t *node = list_get_node(&w->children, index);
        if (node) {
                return node->data;
        }
        return NULL;
}

size_t ui_widget_each(ui_widget_t *w, ui_widget_callback_t callback, void *arg)
{
        size_t count = 0;

        ui_widget_t *next;
        ui_widget_t *child = list_get(&w->children, 0);

        while (child && child != w) {
                callback(child, arg);
                ++count;
                next = list_get(&child->children, 0);
                while (!next && child != w) {
                        next = ui_widget_next(child);
                        child = child->parent;
                }
                child = next;
        }
        return count;
}

ui_widget_t *ui_widget_at(ui_widget_t *widget, int ix, int iy)
{
        float x, y;
        bool is_hit;
        list_node_t *node;
        ui_widget_t *target = widget, *c = NULL;

        if (!widget) {
                return NULL;
        }
        x = 1.0f * ix;
        y = 1.0f * iy;
        do {
                is_hit = false;
                for (list_each(node, &target->stacking_context)) {
                        c = node->data;
                        if (!ui_widget_is_visible(c)) {
                                continue;
                        }
                        if (ui_rect_has_point(&c->border_box, x, y)) {
                                target = c;
                                x -= c->padding_box.x;
                                y -= c->padding_box.y;
                                is_hit = true;
                                break;
                        }
                }
        } while (is_hit);
        return target == widget ? NULL : target;
}

#include <locale.h>
#include <stdio.h>

static void _ui_print_tree(ui_widget_t *w, int depth, const wchar_t *prefix)
{
        ui_widget_t *child;
        list_node_t *node;
        css_selector_node_t *snode;
        wchar_t str[16], child_prefix[512];
        size_t len;

        len = wcslen(prefix);
        wcscpy(child_prefix, prefix);
        for (list_each(node, &w->children)) {
                if (node == w->children.tail.prev) {
                        wcscpy(str, L"\u2514");
                        wcscpy(child_prefix + len, L"   ");
                } else {
                        wcscpy(str, L"\u251C");
                        wcscpy(child_prefix + len, L"\u2502  ");
                }
                wcscat(str, L"\u2500\u2500");
                child = node->data;
                if (child->children.length == 0) {
                        wcscat(str, L"\u2500\u2500");
                } else {
                        wcscat(str, L"\u252C\u2500");
                }
                snode = ui_widget_create_selector_node(child);
                logger_error(
                    "%ls%ls %s, xy:(%g,%g), size:(%g,%g), "
                    "visible: %s, display: %d, padding: (%g,%g,%g,%g), margin: "
                    "(%g,%g,%g,%g)\n",
                    prefix, str, snode->fullname, child->border_box.x,
                    child->border_box.y, child->border_box.width,
                    child->border_box.height,
                    ui_widget_is_visible(child) ? "true" : "false",
                    child->computed_style.type_bits.display,
                    child->computed_style.padding_top,
                    child->computed_style.padding_right,
                    child->computed_style.padding_bottom,
                    child->computed_style.padding_left,
                    child->computed_style.margin_top,
                    child->computed_style.margin_right,
                    child->computed_style.margin_bottom,
                    child->computed_style.margin_left);
                css_selector_node_destroy(snode);
                _ui_print_tree(child, depth + 1, child_prefix);
        }
}

void ui_print_tree(ui_widget_t *w)
{
        css_selector_node_t *node;

        w = w ? w : ui_root();
        node = ui_widget_create_selector_node(w);
        logger_error("%s, xy:(%g,%g), size:(%g,%g), visible: %s\n",
                     node->fullname, w->border_box.x, w->border_box.y,
                     w->border_box.width, w->border_box.height,
                     ui_widget_is_visible(w) ? "true" : "false");
        css_selector_node_destroy(node);
        _ui_print_tree(w, 0, L"  ");
}
