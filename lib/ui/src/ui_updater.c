/*
 * lib/ui/src/ui_updater.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

// #define UI_DEBUG_ENABLED
#include <string.h>
#include <time.h>
#include <css.h>
#include <ui/base.h>
#include <ui/style.h>
#include <ui/rect.h>
#include <ui/metrics.h>
#include <ui/mutation_observer.h>
#include <ui/hash.h>
#include "ui_debug.h"
#include "ui_diff.h"
#include "ui_updater.h"
#include "ui_mutation_observer.h"
#include "ui_widget_style.h"
#include "ui_widget_box.h"
#include "ui_widget_observer.h"
#include "ui_widget.h"

static struct ui_updater_t {
        /**
         * dict_t<hash, css_style_decl_t*>
         */
        dict_t* style_cache;
        ui_metrics_t metrics;
        bool refresh_all;
} ui_updater;

static uint64_t ui_style_dict_hash(const void* key)
{
        return (*(unsigned int*)key);
}

static int ui_style_dict_string_key_compare(void* privdata, const void* key1,
                                            const void* key2)
{
        return *(unsigned int*)key1 == *(unsigned int*)key2;
}

static void ui_style_dict_string_key_free(void* privdata, void* key)
{
        free(key);
}

static void* ui_style_dict_string_key_dup(void* privdata, const void* key)
{
        unsigned int* newkey = malloc(sizeof(unsigned int));
        *newkey = *(unsigned int*)key;
        return newkey;
}

static void ui_style_dict_val_free(void* privdata, void* val)
{
        css_style_decl_destroy(val);
}

void ui_widget_request_refresh_children(ui_widget_t* widget)
{
        ui_widget_t* child;
        list_node_t* node;

        widget->update.should_update_children = true;
        for (list_each(node, &widget->children)) {
                child = node->data;
                child->update.should_refresh_style = true;
                ui_widget_request_refresh_children(child);
        }
}

void ui_widget_request_update(ui_widget_t* w)
{
        w->update.should_update_self = true;
        for (w = w->parent; w; w = w->parent) {
                w->update.should_update_children = true;
        }
}

void ui_widget_set_rules(ui_widget_t* w, const ui_widget_rules_t* rules)
{
        ui_widget_use_extra_data(w);
        if (rules) {
                w->extra->rules = *rules;
        } else {
                memset(&w->extra->rules, 0, sizeof(ui_widget_rules_t));
        }
        w->extra->update_progress = 0;
        w->extra->default_max_update_count = 2048;
}

void ui_widget_update_stacking_context(ui_widget_t* w)
{
        ui_widget_t *child, *target;
        css_computed_style_t *s, *ts;
        list_node_t *node, *target_node;
        list_t* list;

        list = &w->stacking_context;
        list_destroy_without_node(list, NULL);
        for (list_each(node, &w->children)) {
                child = node->data;
                s = &child->computed_style;
                if (child->state < UI_WIDGET_STATE_READY) {
                        continue;
                }
                for (list_each(target_node, list)) {
                        target = target_node->data;
                        ts = &target->computed_style;
                        if (s->z_index == ts->z_index) {
                                if (s->type_bits.position ==
                                    ts->type_bits.position) {
                                        if (child->index < target->index) {
                                                continue;
                                        }
                                } else if (s->type_bits.position <
                                           ts->type_bits.position) {
                                        continue;
                                }
                        } else if (s->z_index < ts->z_index) {
                                continue;
                        }
                        list_link(list, target_node->prev, &child->node_show);
                        break;
                }
                if (!target_node) {
                        list_append_node(list, &child->node_show);
                }
        }
}

static void ui_widget_match_style(ui_widget_t* w)
{
        css_selector_t* selector;
        css_style_decl_t* style;

        if (w->hash && w->update.should_refresh_style) {
                ui_widget_generate_self_hash(w);
        }
        if (w->hash) {
                style = dict_fetch_value(ui_updater.style_cache, &w->hash);
                if (!style) {
                        selector = ui_widget_create_selector(w);
                        style = css_select_style_with_cache(selector);
                        dict_add(ui_updater.style_cache, &w->hash, style);
                        css_selector_destroy(selector);
                }
                w->matched_style = style;
        } else {
                selector = ui_widget_create_selector(w);
                w->matched_style = css_select_style_with_cache(selector);
                css_selector_destroy(selector);
        }
}

static size_t ui_widget_update_visible_children(ui_widget_t* w)
{
        size_t total = 0, count;
        bool found = false;
        ui_rect_t rect, visible_rect;
        ui_widget_t *child, *parent;
        list_node_t *node, *next;

        rect = w->padding_box;
        if (rect.width < 1 &&
            w->computed_style.type_bits.width == CSS_WIDTH_AUTO) {
                rect.width = w->parent->padding_box.width;
        }
        if (rect.height < 1 &&
            w->computed_style.type_bits.height == CSS_HEIGHT_AUTO) {
                rect.height = w->parent->padding_box.height;
        }
        for (child = w, parent = w->parent; parent;
             child = parent, parent = parent->parent) {
                if (child == w) {
                        continue;
                }
                rect.x += child->padding_box.x;
                rect.y += child->padding_box.y;
                ui_rect_correct(&rect, parent->padding_box.width,
                                parent->padding_box.height);
        }
        visible_rect = rect;
        rect = w->padding_box;
        ui_widget_get_offset(w, NULL, &rect.x, &rect.y);
        if (!ui_rect_overlap(&visible_rect, &rect, &visible_rect)) {
                return 0;
        }
        visible_rect.x -= w->padding_box.x;
        visible_rect.y -= w->padding_box.y;
        for (node = w->children.head.next; node; node = next) {
                child = node->data;
                next = node->next;
                if (!ui_rect_overlap(&visible_rect, &child->border_box,
                                     &rect)) {
                        if (found) {
                                break;
                        }
                        continue;
                }
                found = true;
                count = ui_widget_update(child);
                if (ui_widget_has_update(child)) {
                        w->update.should_update_children = true;
                }
                total += count;
                node = next;
        }
        return total;
}

static size_t ui_widget_update_children(ui_widget_t* w)
{
        clock_t msec = 0;
        ui_widget_t* child;
        ui_widget_rules_t* rules;
        list_node_t *node, *next;
        size_t total = 0, update_count = 0, count;

        node = w->children.head.next;
        rules = w->extra ? &w->extra->rules : NULL;
        if (rules) {
                msec = clock();
                if (rules->only_on_visible) {
                        if (!ui_widget_in_viewport(w)) {
                                DEBUG_MSG("%s %s: is not visible\n", w->type,
                                          w->id);
                                return 0;
                        }
                }
                DEBUG_MSG("%s %s: is visible\n", w->type, w->id);
                if (rules->first_update_visible_children) {
                        total += ui_widget_update_visible_children(w);
                        DEBUG_MSG("first update visible children "
                                  "count: %zu\n",
                                  total);
                }
                if (!w->update.should_update_children) {
                        return 0;
                }
        }
        while (node) {
                child = node->data;
                next = node->next;
                count = ui_widget_update(child);
                if (ui_widget_has_update(child)) {
                        w->update.should_update_children = true;
                }
                total += count;
                node = next;
                if (!rules || rules->max_update_children_count == 0) {
                        continue;
                }
                if (count > 0) {
                        w->extra->update_progress =
                            y_max(child->index, w->extra->update_progress);
                        if (w->extra->update_progress >
                            w->stacking_context.length) {
                                w->extra->update_progress = child->index;
                        }
                        update_count += 1;
                }
                if (rules->max_update_children_count > 0) {
                        if (update_count >=
                            (size_t)rules->max_update_children_count) {
                                w->update.should_update_children = true;
                                break;
                        }
                }
                if (update_count < w->extra->default_max_update_count) {
                        continue;
                }
                w->update.should_update_children = true;
                msec = (clock() - msec);
                if (msec < 1) {
                        w->extra->default_max_update_count += 128;
                        continue;
                }
                w->extra->default_max_update_count =
                    update_count * CLOCKS_PER_SEC / 120 / msec;
                if (w->extra->default_max_update_count < 1) {
                        w->extra->default_max_update_count = 32;
                }
                break;
        }
        if (rules) {
                if (!w->update.should_update_children) {
                        w->extra->update_progress = w->stacking_context.length;
                }
                if (rules->on_update_progress) {
                        rules->on_update_progress(w, w->extra->update_progress);
                }
        }
        return total;
}

static void ui_widget_update_size(ui_widget_t* w)
{
        css_unit_t unit;
        css_numeric_value_t limit;
        css_computed_style_t* src = &w->specified_style;
        css_computed_style_t* dest = &w->computed_style;

        CSS_COPY_LENGTH(dest, src, width);
        CSS_COPY_LENGTH(dest, src, height);
        ui_widget_compute_style(w);
        ui_widget_update_box_size(w);
        ui_widget_reflow(w);
        w->max_content_width = w->content_box.width;
        w->max_content_height = w->content_box.height;
        if (css_computed_max_width(dest, &limit, &unit) == CSS_MAX_WIDTH_SET &&
            unit == CSS_UNIT_PX) {
                w->max_content_width = limit;
        } else if (css_computed_min_width(dest, &limit, &unit) ==
                       CSS_MIN_WIDTH_SET &&
                   unit == CSS_UNIT_PX && w->max_content_width < limit) {
                w->max_content_width = limit;
        }
        if (css_computed_max_height(dest, &limit, &unit) ==
                CSS_MAX_HEIGHT_SET &&
            unit == CSS_UNIT_PX) {
                w->max_content_width = limit;
        } else if (css_computed_min_height(dest, &limit, &unit) ==
                       CSS_MIN_HEIGHT_SET &&
                   unit == CSS_UNIT_PX && w->max_content_height < limit) {
                w->max_content_height = limit;
        }
}

size_t ui_widget_update(ui_widget_t* w)
{
        size_t count = 0;
        ui_style_diff_t style_diff = { 0 };

        if (ui_updater.refresh_all) {
                w->update.should_update_children = true;
                w->update.should_refresh_style = true;
                w->update.should_reflow = true;
        }
        if (ui_widget_has_update(w)) {
                if (w->proto && w->proto->update) {
                        w->proto->update(w, UI_TASK_BEFORE_UPDATE);
                }
                if (w->update.should_refresh_style) {
                        ui_widget_match_style(w);
                        w->update.should_update_style = true;
                        if (w->proto && w->proto->update) {
                                w->proto->update(w, UI_TASK_REFRESH_STYLE);
                        }
                }
                if (w->update.should_update_style) {
                        ui_style_diff_init(&style_diff, w);
                        if (!ui_updater.refresh_all) {
                                ui_style_diff_begin(&style_diff, w);
                        }
                        ui_widget_update_style(w);
                        ui_style_diff_end(&style_diff, w);
                        if (w->proto && w->proto->update) {
                                w->proto->update(w, UI_TASK_UPDATE_STYLE);
                        }
                }
                ui_widget_add_state(w, UI_WIDGET_STATE_UPDATED);
                if (w->proto && w->proto->update) {
                        w->proto->update(w, UI_TASK_AFTER_UPDATE);
                }
        }
        if (w->update.should_update_children) {
                count += ui_widget_update_children(w);
        }
        if (w->update.should_reflow) {
                if (w->parent && ui_widget_in_layout_flow(w)) {
                        float width = w->border_box.width;
                        float height = w->border_box.height;
                        ui_widget_update_size(w);
                        // 只需要判断宽高的变化，因为 style diff
                        // 已经判断了位置变化
                        if (width != w->border_box.width ||
                            height != w->border_box.height) {
                                ui_widget_request_reflow(w->parent);
                        }
                } else {
                        ui_widget_update_size(w);
                        // TODO: 更新 box size？
                        ui_widget_update_box_position(w);
                }
        }
        ui_widget_update_stacking_context(w);
        return count;
}

void ui_refresh_style(void)
{
        ui_updater.refresh_all = true;
}

static void ui_process_mutations(ui_widget_t* w)
{
        ui_mutation_record_t* record;
        ui_mutation_record_type_t type = UI_MUTATION_RECORD_TYPE_PROPERTIES;

        if (w->parent) {
                if (w->parent->rendering.dirty_rect_type !=
                        UI_DIRTY_RECT_TYPE_FULL &&
                    (!ui_rect_is_equal(&w->border_box,
                                       &w->update.border_box_backup) ||
                     !ui_rect_is_equal(&w->canvas_box,
                                       &w->update.canvas_box_backup))) {
                        w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
                        ui_widget_expose_dirty_rect(w);
                        ui_widget_mark_dirty_rect(w->parent,
                                                  &w->update.canvas_box_backup,
                                                  UI_BOX_TYPE_PADDING_BOX);
                }
        } else if (w->rendering.dirty_rect_type != UI_DIRTY_RECT_TYPE_FULL &&
                   (!ui_rect_is_equal(&w->border_box,
                                      &w->update.border_box_backup) ||
                    !ui_rect_is_equal(&w->canvas_box,
                                      &w->update.canvas_box_backup))) {
                w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
                ui_widget_expose_dirty_rect(w);
        }
        if (w->update.should_update_children || w->update.should_reflow) {
                ui_widget_each(w, (ui_widget_callback_t)ui_process_mutations,
                               NULL);
        }
        w->update.should_update_self = false;
        w->update.should_update_style = false;
        w->update.should_refresh_style = false;
        w->update.should_update_children = false;
        w->update.should_reflow = false;
        ui_widget_add_state(w, UI_WIDGET_STATE_LAYOUTED);
        if (ui_widget_has_observer(w, type)) {
                if (w->update.border_box_backup.x != w->border_box.x) {
                        record = ui_mutation_record_create(w, type);
                        record->property_name = strdup2("x");
                        ui_widget_add_mutation_record(w, record);
                        ui_mutation_record_destroy(record);
                }
                if (w->update.border_box_backup.y != w->border_box.y) {
                        record = ui_mutation_record_create(w, type);
                        record->property_name = strdup2("y");
                        ui_widget_add_mutation_record(w, record);
                        ui_mutation_record_destroy(record);
                }
                if (w->update.border_box_backup.width != w->border_box.width) {
                        record = ui_mutation_record_create(w, type);
                        record->property_name = strdup2("width");
                        ui_widget_add_mutation_record(w, record);
                        ui_mutation_record_destroy(record);
                }
                if (w->update.border_box_backup.height !=
                    w->border_box.height) {
                        record = ui_mutation_record_create(w, type);
                        record->property_name = strdup2("height");
                        ui_widget_add_mutation_record(w, record);
                        ui_mutation_record_destroy(record);
                }
        }
        w->update.canvas_box_backup = w->canvas_box;
        w->update.border_box_backup = w->border_box;
}

size_t ui_update(void)
{
        size_t count;
        ui_widget_t* root;

        root = ui_root();
        if (memcmp(&ui_metrics, &ui_updater.metrics, sizeof(ui_metrics_t))) {
                ui_updater.refresh_all = true;
                root->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_FULL;
        }
        count = ui_widget_update(root);
        ui_updater.metrics = ui_metrics;
        ui_updater.refresh_all = false;
        ui_process_mutations(root);
        ui_process_mutation_observers();
        return count;
}

void ui_init_updater(void)
{
        static dict_type_t type;

        type.val_dup = NULL;
        type.key_dup = ui_style_dict_string_key_dup;
        type.key_compare = ui_style_dict_string_key_compare;
        type.hash_function = ui_style_dict_hash;
        type.key_destructor = ui_style_dict_string_key_free;
        type.val_destructor = ui_style_dict_val_free;
        ui_updater.refresh_all = true;
        ui_updater.style_cache = dict_create(&type, NULL);
}

void ui_destroy_updater(void)
{
        dict_destroy(ui_updater.style_cache);
        ui_updater.style_cache = NULL;
}
