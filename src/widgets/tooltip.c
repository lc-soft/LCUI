/*
 * src/widgets/tooltip.c: -- Tooltip widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/widgets/tooltip.h>
#include <LCUI/widgets/portal.h>
#include <LCUI/widgets/text.h>
#include <css.h>
#include <ptk.h>
#include <yutil.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>

typedef struct ui_tooltip {
        ui_widget_t *owner;
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *portal;
        ui_widget_t *arrow;
        int delay;
        ui_widget_t *text;
        ui_widget_t *inner;
        int show_timer;
        bool open;
        bool reveal_pending;
        int settle_stage;
        list_node_t live_node;
        list_node_t attr_node;
} ui_tooltip_t;

typedef struct ui_tooltip_content {
        ui_tooltip_side_t side;
        ui_tooltip_align_t align;
        int side_offset;
        int align_offset;
} ui_tooltip_content_t;

typedef struct ui_tooltip_registry_entry {
        char *key;
        ui_tooltip_t *data;
        list_node_t node;
} ui_tooltip_registry_entry_t;

static ui_widget_prototype_t *ui_tooltip_prototype;
static ui_widget_prototype_t *ui_tooltip_trigger_prototype;
static ui_widget_prototype_t *ui_tooltip_content_prototype;
static list_t ui_tooltip_registry;
static list_t ui_tooltip_live;
static list_t ui_tooltip_attr_list;
static ui_tooltip_t *ui_tooltip_active;

static const char *ui_tooltip_side_names[] = {
        "top",
        "right",
        "bottom",
        "left",
};

static const char *ui_tooltip_align_names[] = {
        "center",
        "start",
        "end",
};

#define UI_TOOLTIP_ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

static void ui_tooltip_on_content_layout(ui_widget_t *w, ui_event_t *e,
                                         void *arg);
static void ui_tooltip_show_data(ui_tooltip_t *data);
static void ui_tooltip_hide_data(ui_tooltip_t *data);

static const char *ui_tooltip_css = "\
tooltip {\
  display: inline-block;\
}\
tooltip-trigger {\
  display: inline-block;\
}\
tooltip-content {\
  position: absolute;\
}\
";

static ui_tooltip_t *ui_tooltip_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_tooltip_prototype);
}

static ui_tooltip_content_t *ui_tooltip_content_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_tooltip_content_prototype);
}

static ui_tooltip_side_t ui_tooltip_side_normalize(ui_tooltip_side_t side)
{
        if (side < UI_SIDE_TOP || side > UI_SIDE_LEFT) {
                return UI_SIDE_TOP;
        }
        return side;
}

static ui_tooltip_align_t ui_tooltip_align_normalize(ui_tooltip_align_t align)
{
        if (align < UI_ALIGN_CENTER || align > UI_ALIGN_END) {
                return UI_ALIGN_CENTER;
        }
        return align;
}

static ui_tooltip_side_t ui_tooltip_side_from_name(const char *name)
{
        size_t i;

        if (name) {
                for (i = 0; i < UI_TOOLTIP_ARRAY_LEN(ui_tooltip_side_names);
                     ++i) {
                        if (strcmp(name, ui_tooltip_side_names[i]) == 0) {
                                return (ui_tooltip_side_t)i;
                        }
                }
        }
        return UI_SIDE_TOP;
}

static ui_tooltip_align_t ui_tooltip_align_from_name(const char *name)
{
        size_t i;

        if (name) {
                for (i = 0; i < UI_TOOLTIP_ARRAY_LEN(ui_tooltip_align_names);
                     ++i) {
                        if (strcmp(name, ui_tooltip_align_names[i]) == 0) {
                                return (ui_tooltip_align_t)i;
                        }
                }
        }
        return UI_ALIGN_CENTER;
}

static ui_widget_t *ui_tooltip_find_child(ui_widget_t *w,
                                          ui_widget_prototype_t *proto)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child, proto)) {
                        return child;
                }
                found = ui_tooltip_find_child(child, proto);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

/* Keep the user content in an inner bubble so the arrow can sit in the
 * content's transparent padding instead of on top of the bubble. */
static void ui_tooltip_ensure_inner(ui_tooltip_t *data)
{
        ui_widget_t *child;
        ui_widget_t *inner = data->inner;
        size_t i;

        if (inner) {
                return;
        }
        inner = ui_create_widget(NULL);
        ui_widget_append(data->content, inner);
        data->inner = inner;
        i = 0;
        while ((child = ui_widget_get_child(data->content, i))) {
                if (child == inner) {
                        ++i;
                        continue;
                }
                ui_widget_append(inner, child);
        }
}

static void ui_tooltip_ensure_arrow(ui_tooltip_t *data)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(data->content, i)); ++i) {
                if (ui_widget_has_class(child, "tooltip-arrow")) {
                        data->arrow = child;
                        return;
                }
        }
        data->arrow = ui_create_widget(NULL);
        ui_widget_add_class(data->arrow, "tooltip-arrow");
        ui_widget_append(data->content, data->arrow);
}

static bool ui_tooltip_is_descendant(ui_widget_t *ancestor, ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (w == ancestor) {
                        return true;
                }
        }
        return false;
}

static void ui_tooltip_cancel_show(ui_tooltip_t *data)
{
        if (data->show_timer) {
                ptk_clear_timer(data->show_timer);
                data->show_timer = 0;
        }
}

static void ui_tooltip_on_show_timeout(void *arg)
{
        ui_tooltip_t *data = arg;

        data->show_timer = 0;
        ui_tooltip_show_data(data);
}

static void ui_tooltip_schedule_show(ui_tooltip_t *data)
{
        if (!data->trigger || data->open || data->show_timer) {
                return;
        }
        if (data->delay <= 0) {
                ui_tooltip_show_data(data);
                return;
        }
        data->show_timer =
            ptk_set_timeout(data->delay, ui_tooltip_on_show_timeout, data);
}

static ui_widget_t *ui_tooltip_find_owner(ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (w->proto == ui_tooltip_prototype) {
                        return w;
                }
        }
        return NULL;
}

static ui_widget_t *ui_tooltip_find_attr_ancestor(ui_widget_t *w,
                                                  const char *name)
{
        for (; w; w = w->parent) {
                if (ui_widget_get_attr(w, name)) {
                        return w;
                }
        }
        return NULL;
}

static ui_tooltip_registry_entry_t *ui_tooltip_find_registry(const char *key)
{
        list_node_t *node;

        if (!key) {
                return NULL;
        }
        for (list_each(node, &ui_tooltip_registry)) {
                ui_tooltip_registry_entry_t *entry = node->data;

                if (strcmp(entry->key, key) == 0) {
                        return entry;
                }
        }
        return NULL;
}

static ui_tooltip_t *ui_tooltip_get_loose(const char *key)
{
        ui_tooltip_registry_entry_t *entry;

        entry = ui_tooltip_find_registry(key);
        if (entry) {
                return entry->data;
        }
        entry = calloc(1, sizeof(ui_tooltip_registry_entry_t));
        entry->key = y_strdup(key);
        entry->data = calloc(1, sizeof(ui_tooltip_t));
        entry->data->owner = ui_root();
        entry->node.data = entry;
        list_append_node(&ui_tooltip_registry, &entry->node);
        return entry->data;
}

static ui_widget_t *ui_tooltip_find_target(ui_widget_t *w, const char *key)
{
        ui_widget_t *child;
        ui_widget_t *found;
        const char *value;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                value = ui_widget_get_attr(child, "data-tooltip-target");
                if (ui_check_widget_type(child, "tooltip-content") && value &&
                    strcmp(value, key) == 0) {
                        return child;
                }
                found = ui_tooltip_find_target(child, key);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static void ui_tooltip_on_loose_destroy(ui_widget_t *w, ui_event_t *e,
                                        void *arg)
{
        ui_tooltip_t *data = e->data;

        if (data->trigger == w) {
                data->trigger = NULL;
                ui_tooltip_hide_data(data);
        }
}

static ui_tooltip_t *ui_tooltip_activate_loose(ui_widget_t *trigger)
{
        ui_tooltip_t *data;
        const char *key;
        ui_widget_t *content;

        key = ui_widget_get_attr(trigger, "data-tooltip-trigger");
        if (!key || !key[0]) {
                return NULL;
        }
        data = ui_tooltip_get_loose(key);
        if (!data->portal) {
                content = ui_tooltip_find_target(ui_root(), key);
                if (!content) {
                        return NULL;
                }
                data->content = content;
                data->trigger = trigger;
                data->portal = ui_create_portal();
                ui_portal_set_passthrough(data->portal, true);
                ui_widget_append(ui_root(), data->portal);
                ui_portal_set_content(data->portal, content);
                ui_portal_set_anchor(data->portal, trigger);
                ui_tooltip_ensure_inner(data);
                ui_tooltip_ensure_arrow(data);
                ui_widget_on(content, "afterlayout",
                             ui_tooltip_on_content_layout, data);
                ui_widget_on(trigger, "destroy", ui_tooltip_on_loose_destroy,
                             data);
        } else if (data->trigger != trigger) {
                data->trigger = trigger;
                ui_portal_set_anchor(data->portal, trigger);
                ui_widget_on(trigger, "destroy", ui_tooltip_on_loose_destroy,
                             data);
        }
        if (ui_tooltip_active && ui_tooltip_active != data) {
                ui_tooltip_hide_data(ui_tooltip_active);
        }
        if (!data->open) {
                ui_tooltip_show_data(data);
        }
        ui_tooltip_active = data;
        return data;
}

static int ui_tooltip_attr_int(ui_widget_t *w, const char *name, int fallback)
{
        const char *value = ui_widget_get_attr(w, name);

        return value ? atoi(value) : fallback;
}

static ui_tooltip_t *ui_tooltip_find_attr_controller(ui_widget_t *target)
{
        list_node_t *node;

        for (list_each(node, &ui_tooltip_attr_list)) {
                ui_tooltip_t *data = node->data;

                if (data->trigger == target) {
                        return data;
                }
        }
        return NULL;
}

static void ui_tooltip_attr_free(ui_tooltip_t *data)
{
        ui_tooltip_cancel_show(data);
        if (ui_tooltip_active == data) {
                ui_tooltip_active = NULL;
        }
        if (data->portal) {
                ui_widget_destroy(data->portal);
                data->portal = NULL;
        }
        list_unlink(&ui_tooltip_attr_list, &data->attr_node);
        free(data);
}

static void ui_tooltip_attr_on_target_destroy(ui_widget_t *w, ui_event_t *e,
                                              void *arg)
{
        ui_tooltip_attr_free(e->data);
}

static void ui_tooltip_attr_release(ui_tooltip_t *data)
{
        if (data->trigger) {
                ui_widget_off(data->trigger, "destroy",
                              ui_tooltip_attr_on_target_destroy, data);
        }
        ui_tooltip_attr_free(data);
}

static void ui_tooltip_attr_apply(ui_tooltip_t *data, ui_widget_t *target,
                                  const char *text)
{
        ui_tooltip_content_set_side(
            data->content, ui_tooltip_side_from_name(ui_widget_get_attr(
                               target, "data-tooltip-side")));
        ui_tooltip_content_set_align(
            data->content, ui_tooltip_align_from_name(ui_widget_get_attr(
                               target, "data-tooltip-align")));
        ui_tooltip_content_set_side_offset(
            data->content,
            ui_tooltip_attr_int(target, "data-tooltip-side-offset", 4));
        ui_tooltip_content_set_align_offset(
            data->content,
            ui_tooltip_attr_int(target, "data-tooltip-align-offset", 0));
        data->delay = ui_tooltip_attr_int(target, "data-tooltip-delay", 0);
        if (data->delay < 0) {
                data->delay = 0;
        }
        ui_text_set_content(data->text, text);
}

static void ui_tooltip_attr_activate(ui_widget_t *target, const char *text)
{
        ui_tooltip_t *data;
        ui_widget_t *content;

        data = ui_tooltip_find_attr_controller(target);
        if (!data) {
                data = calloc(1, sizeof(ui_tooltip_t));
                content = ui_create_tooltip_content();
                data->trigger = target;
                data->content = content;
                data->text = ui_create_widget("text");
                ui_widget_append(content, data->text);
                data->portal = ui_create_portal();
                ui_portal_set_passthrough(data->portal, true);
                ui_widget_append(ui_root(), data->portal);
                ui_portal_set_content(data->portal, content);
                ui_portal_set_anchor(data->portal, target);
                ui_tooltip_ensure_inner(data);
                ui_tooltip_ensure_arrow(data);
                ui_widget_on(content, "afterlayout",
                             ui_tooltip_on_content_layout, data);
                ui_widget_on(target, "destroy",
                             ui_tooltip_attr_on_target_destroy, data);
                data->attr_node.data = data;
                list_append_node(&ui_tooltip_attr_list, &data->attr_node);
        }
        if (ui_tooltip_active && ui_tooltip_active != data) {
                ui_tooltip_hide_data(ui_tooltip_active);
        }
        ui_tooltip_attr_apply(data, target, text);
        ui_portal_set_anchor(data->portal, target);
        if (!data->open) {
                ui_tooltip_schedule_show(data);
        }
        ui_tooltip_active = data;
}

static void ui_tooltip_attr_destroy(void *arg)
{
        ui_tooltip_t *data = arg;

        ui_tooltip_cancel_show(data);
        if (data->portal) {
                ui_widget_destroy(data->portal);
        }
        free(data);
}

static void ui_tooltip_attr_refresh(ui_widget_t *target)
{
        ui_tooltip_t *data = ui_tooltip_find_attr_controller(target);
        const char *text;

        if (!data) {
                return;
        }
        text = ui_widget_get_attr(target, "data-tooltip");
        if (!text || !text[0]) {
                ui_tooltip_attr_release(data);
                return;
        }
        ui_tooltip_attr_apply(data, target, text);
        if (data->open) {
                ui_tooltip_show_data(data);
        }
}

void ui_tooltip_options_init(ui_tooltip_options_t *options)
{
        if (!options) {
                return;
        }
        memset(options, 0, sizeof(*options));
        options->side = UI_SIDE_TOP;
        options->align = UI_ALIGN_CENTER;
        options->side_offset = 4;
        options->align_offset = 0;
}

static void ui_tooltip_set_attr_int(ui_widget_t *target, const char *name,
                                    int value)
{
        char text[32];

        snprintf(text, sizeof(text), "%d", value);
        ui_widget_set_attr(target, name, text);
}

void ui_tooltip_attach(ui_widget_t *target, const char *text)
{
        if (!target) {
                return;
        }
        ui_widget_set_attr(target, "data-tooltip",
                           text && text[0] ? text : NULL);
        ui_tooltip_attr_refresh(target);
}

void ui_tooltip_attach_ex(ui_widget_t *target,
                          const ui_tooltip_options_t *options)
{
        ui_tooltip_side_t side;
        ui_tooltip_align_t align;
        int delay;

        if (!target || !options) {
                return;
        }
        if (!options->content || !options->content[0]) {
                ui_widget_set_attr(target, "data-tooltip", NULL);
                ui_tooltip_attr_refresh(target);
                return;
        }
        side = ui_tooltip_side_normalize(options->side);
        align = ui_tooltip_align_normalize(options->align);
        delay = options->delay < 0 ? 0 : options->delay;
        ui_widget_set_attr(target, "data-tooltip", options->content);
        ui_widget_set_attr(target, "data-tooltip-side",
                           ui_tooltip_side_names[side]);
        ui_widget_set_attr(target, "data-tooltip-align",
                           ui_tooltip_align_names[align]);
        ui_tooltip_set_attr_int(target, "data-tooltip-delay", delay);
        ui_tooltip_set_attr_int(target, "data-tooltip-side-offset",
                                options->side_offset);
        ui_tooltip_set_attr_int(target, "data-tooltip-align-offset",
                                options->align_offset);
        ui_tooltip_attr_refresh(target);
}

static bool ui_tooltip_pointer_inside(ui_widget_t *trigger, float x, float y)
{
        float tx, ty;

        if (!trigger) {
                return false;
        }
        ui_widget_get_offset(trigger, ui_root(), &tx, &ty);
        return x >= tx && x < tx + trigger->border_box.width && y >= ty &&
               y < ty + trigger->border_box.height;
}

static bool ui_tooltip_live_is_alive(ui_tooltip_t *data)
{
        return data->owner && data->owner->state != UI_WIDGET_STATE_DELETED &&
               data->trigger && data->trigger->state != UI_WIDGET_STATE_DELETED;
}

static void ui_tooltip_hide_other_structural_tooltips(ui_tooltip_t *keep)
{
        list_node_t *node;

        for (list_each(node, &ui_tooltip_live)) {
                ui_tooltip_t *data = node->data;

                if (data != keep && data->open &&
                    ui_tooltip_live_is_alive(data)) {
                        ui_tooltip_cancel_show(data);
                        ui_tooltip_hide_data(data);
                }
        }
}

static void ui_tooltip_module_on_mousemove(ui_widget_t *w, ui_event_t *e,
                                           void *arg)
{
        ui_widget_t *trigger =
            ui_tooltip_find_attr_ancestor(e->target, "data-tooltip-trigger");
        list_node_t *node;

        if (trigger) {
                ui_tooltip_hide_other_structural_tooltips(NULL);
                ui_tooltip_activate_loose(trigger);
                return;
        }
        {
                ui_widget_t *attr_target =
                    ui_tooltip_find_attr_ancestor(e->target, "data-tooltip");
                const char *text =
                    attr_target
                        ? ui_widget_get_attr(attr_target, "data-tooltip")
                        : NULL;

                if (attr_target && !ui_tooltip_find_owner(attr_target) &&
                    text && text[0]) {
                        ui_tooltip_hide_other_structural_tooltips(NULL);
                        ui_tooltip_attr_activate(attr_target, text);
                        return;
                }
        }
        for (list_each(node, &ui_tooltip_live)) {
                ui_tooltip_t *data = node->data;

                if (!ui_tooltip_live_is_alive(data)) {
                        continue;
                }
                if (ui_tooltip_pointer_inside(data->trigger, e->mouse.x,
                                              e->mouse.y)) {
                        if (!data->open) {
                                ui_tooltip_schedule_show(data);
                        }
                } else if (data->open || data->show_timer) {
                        ui_tooltip_cancel_show(data);
                        ui_tooltip_hide_data(data);
                }
        }
        if (ui_tooltip_active && ui_tooltip_active->open &&
            !ui_tooltip_pointer_inside(ui_tooltip_active->trigger, e->mouse.x,
                                       e->mouse.y)) {
                ui_tooltip_hide_data(ui_tooltip_active);
                ui_tooltip_active = NULL;
        }
}

static void ui_tooltip_module_on_focus(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *focus = ui_get_focus_in(w);
        ui_widget_t *trigger =
            ui_tooltip_find_attr_ancestor(focus, "data-tooltip-trigger");
        list_node_t *node;

        if (trigger) {
                ui_tooltip_activate_loose(trigger);
        } else {
                ui_widget_t *attr_target =
                    ui_tooltip_find_attr_ancestor(focus, "data-tooltip");
                const char *text =
                    attr_target
                        ? ui_widget_get_attr(attr_target, "data-tooltip")
                        : NULL;

                if (attr_target && !ui_tooltip_find_owner(attr_target) &&
                    text && text[0]) {
                        ui_tooltip_hide_other_structural_tooltips(NULL);
                        ui_tooltip_attr_activate(attr_target, text);
                } else if (ui_tooltip_active &&
                           !ui_tooltip_is_descendant(ui_tooltip_active->trigger,
                                                     focus)) {
                        ui_tooltip_hide_data(ui_tooltip_active);
                        ui_tooltip_active = NULL;
                }
        }
        for (list_each(node, &ui_tooltip_live)) {
                ui_tooltip_t *data = node->data;

                if (!ui_tooltip_live_is_alive(data)) {
                        continue;
                }
                if (ui_tooltip_is_descendant(data->trigger, focus)) {
                        if (!data->open) {
                                ui_tooltip_schedule_show(data);
                        }
                } else if (data->open || data->show_timer) {
                        ui_tooltip_cancel_show(data);
                        ui_tooltip_hide_data(data);
                }
        }
}

static void ui_tooltip_module_on_blur(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_tooltip_module_on_focus(w, e, arg);
}

static void ui_tooltip_module_on_keydown(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        list_node_t *node;

        if (e->key.code != KEY_ESCAPE) {
                return;
        }
        for (list_each(node, &ui_tooltip_live)) {
                ui_tooltip_t *data = node->data;

                if (data->open && ui_tooltip_live_is_alive(data)) {
                        ui_tooltip_cancel_show(data);
                        ui_tooltip_hide_data(data);
                }
        }
        if (ui_tooltip_active && ui_tooltip_active->open) {
                ui_tooltip_hide_data(ui_tooltip_active);
                ui_tooltip_active = NULL;
        }
}

static void ui_tooltip_bind(ui_widget_t *w)
{
        ui_tooltip_t *data = ui_tooltip_get_data(w);
        ui_widget_t *trigger;
        ui_widget_t *content;

        if (data->portal) {
                return;
        }
        trigger = ui_tooltip_find_child(w, ui_tooltip_trigger_prototype);
        content = ui_tooltip_find_child(w, ui_tooltip_content_prototype);
        if (!trigger || !content) {
                return;
        }
        data->owner = w;
        data->trigger = trigger;
        data->content = content;
        data->portal = ui_create_portal();
        ui_portal_set_passthrough(data->portal, true);
        ui_widget_append(w, data->portal);
        ui_portal_set_content(data->portal, content);
        ui_portal_set_anchor(data->portal, trigger);
        ui_tooltip_ensure_inner(data);
        ui_tooltip_ensure_arrow(data);
        ui_widget_on(content, "afterlayout", ui_tooltip_on_content_layout,
                     data);
        data->live_node.data = data;
        list_append_node(&ui_tooltip_live, &data->live_node);
}

static void ui_tooltip_on_update(ui_widget_t *w, ui_task_type_t task)
{
        if (task != UI_TASK_BEFORE_UPDATE) {
                return;
        }
        ui_tooltip_bind(w);
}

static void ui_tooltip_on_init(ui_widget_t *w)
{
        ui_tooltip_t *data =
            ui_widget_add_data(w, ui_tooltip_prototype, sizeof(ui_tooltip_t));

        memset(data, 0, sizeof(ui_tooltip_t));
        data->delay = 0;
        data->open = false;
}

static void ui_tooltip_on_destroy(ui_widget_t *w)
{
        ui_tooltip_t *data = ui_tooltip_get_data(w);

        /* Children (including the portal and trigger) are destroyed before
         * this callback, so only local state may be touched here. */
        ui_tooltip_cancel_show(data);
        if (data->portal) {
                list_unlink(&ui_tooltip_live, &data->live_node);
                data->portal = NULL;
        }
}

static void ui_tooltip_content_on_init(ui_widget_t *w)
{
        ui_tooltip_content_t *data;

        data = ui_widget_add_data(w, ui_tooltip_content_prototype,
                                  sizeof(ui_tooltip_content_t));
        data->side = UI_SIDE_TOP;
        data->align = UI_ALIGN_CENTER;
        data->side_offset = 4;
        data->align_offset = 0;
}

static void ui_tooltip_content_on_set_attr(ui_widget_t *w, const char *name,
                                           const char *value)
{
        ui_tooltip_content_t *data = ui_tooltip_content_get_data(w);

        if (!data) {
                return;
        }
        if (strcmp(name, "side") == 0) {
                data->side = ui_tooltip_side_from_name(value);
        } else if (strcmp(name, "align") == 0) {
                data->align = ui_tooltip_align_from_name(value);
        } else if (strcmp(name, "side-offset") == 0) {
                data->side_offset = value ? atoi(value) : 4;
        } else if (strcmp(name, "align-offset") == 0) {
                data->align_offset = value ? atoi(value) : 0;
        }
}

static void ui_tooltip_on_set_attr(ui_widget_t *w, const char *name,
                                   const char *value)
{
        ui_tooltip_t *data = ui_tooltip_get_data(w);

        if (!data || strcmp(name, "delay") != 0) {
                return;
        }
        data->delay = value ? atoi(value) : 0;
        if (data->delay < 0) {
                data->delay = 0;
        }
}

void ui_tooltip_content_set_side(ui_widget_t *w, ui_tooltip_side_t side)
{
        side = ui_tooltip_side_normalize(side);
        ui_widget_set_attr(w, "side", ui_tooltip_side_names[side]);
}

void ui_tooltip_content_set_align(ui_widget_t *w, ui_tooltip_align_t align)
{
        align = ui_tooltip_align_normalize(align);
        ui_widget_set_attr(w, "align", ui_tooltip_align_names[align]);
}

void ui_tooltip_content_set_side_offset(ui_widget_t *w, int offset)
{
        char value[32];

        snprintf(value, sizeof(value), "%d", offset);
        ui_widget_set_attr(w, "side-offset", value);
}

void ui_tooltip_content_set_align_offset(ui_widget_t *w, int offset)
{
        char value[32];

        snprintf(value, sizeof(value), "%d", offset);
        ui_widget_set_attr(w, "align-offset", value);
}

void ui_tooltip_set_delay(ui_widget_t *w, int delay_ms)
{
        char value[32];

        delay_ms = delay_ms < 0 ? 0 : delay_ms;
        snprintf(value, sizeof(value), "%d", delay_ms);
        ui_widget_set_attr(w, "delay", value);
}

int ui_tooltip_get_delay(ui_widget_t *w)
{
        ui_tooltip_t *data = ui_tooltip_get_data(w);

        return data->delay;
}

static void ui_tooltip_apply_placement(ui_tooltip_t *data)
{
        ui_tooltip_content_t *content =
            ui_tooltip_content_get_data(data->content);
        ui_tooltip_side_t side = content ? content->side : UI_SIDE_TOP;
        ui_tooltip_align_t align = content ? content->align : UI_ALIGN_CENTER;
        int side_offset = content ? content->side_offset : 4;
        int align_offset = content ? content->align_offset : 0;
        const char *side_name = ui_tooltip_side_names[side];
        char class_name[32];
        size_t i;

        ui_portal_set_side(data->portal, side_name);
        ui_portal_set_align(data->portal, ui_tooltip_align_names[align]);
        ui_portal_set_side_offset(data->portal, side_offset);
        ui_portal_set_align_offset(data->portal, align_offset);
        for (i = 0; i < UI_TOOLTIP_ARRAY_LEN(ui_tooltip_side_names); ++i) {
                snprintf(class_name, sizeof(class_name), "tooltip-side-%s",
                         ui_tooltip_side_names[i]);
                ui_widget_remove_class(data->content, class_name);
                if (data->arrow) {
                        ui_widget_remove_class(data->arrow, class_name);
                }
        }
        snprintf(class_name, sizeof(class_name), "tooltip-side-%s", side_name);
        ui_widget_add_class(data->content, class_name);
        if (data->arrow) {
                ui_widget_add_class(data->arrow, class_name);
        }
        if (data->inner) {
                ui_widget_add_class(data->inner, "tooltip-inner");
                ui_widget_refresh_children_style(data->inner);
        }
}

static void ui_tooltip_on_content_layout(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        ui_tooltip_t *data = e->data;

        if (!data->open) {
                return;
        }
        if (data->settle_stage == 0) {
                data->settle_stage = 1;
                ui_portal_open(data->portal);
                ui_widget_request_reflow(data->content);
                return;
        }
        if (data->settle_stage == 1 && data->reveal_pending) {
                data->reveal_pending = false;
                data->settle_stage = 2;
                ui_widget_set_style_keyword_value(
                    data->content, css_prop_visibility, CSS_KEYWORD_VISIBLE);
                data->content->computed_style.type_bits.visibility =
                    CSS_VISIBILITY_VISIBLE;
        }
}

static void ui_tooltip_show_data(ui_tooltip_t *data)
{
        if (!data->portal) {
                return;
        }
        ui_tooltip_apply_placement(data);
        if (data->content) {
                ui_widget_set_style_keyword_value(
                    data->content, css_prop_visibility, CSS_KEYWORD_HIDDEN);
                data->content->computed_style.type_bits.visibility =
                    CSS_VISIBILITY_HIDDEN;
        }
        data->open = true;
        data->reveal_pending = true;
        data->settle_stage = 0;
        ui_portal_open(data->portal);
}

static void ui_tooltip_hide_data(ui_tooltip_t *data)
{
        if (data->portal) {
                ui_portal_close(data->portal);
        }
        data->open = false;
        data->reveal_pending = false;
        data->settle_stage = 0;
}

void ui_tooltip_show(ui_widget_t *w)
{
        ui_tooltip_show_data(ui_tooltip_get_data(w));
}

void ui_tooltip_hide(ui_widget_t *w)
{
        ui_tooltip_hide_data(ui_tooltip_get_data(w));
}

bool ui_tooltip_is_open(ui_widget_t *w)
{
        ui_tooltip_t *data = ui_tooltip_get_data(w);

        return data->open;
}

ui_widget_t *ui_create_tooltip(void)
{
        return ui_create_widget_with_prototype(ui_tooltip_prototype);
}

ui_widget_t *ui_create_tooltip_trigger(void)
{
        return ui_create_widget_with_prototype(ui_tooltip_trigger_prototype);
}

ui_widget_t *ui_create_tooltip_content(void)
{
        return ui_create_widget_with_prototype(ui_tooltip_content_prototype);
}

static void ui_tooltip_destroy_registry_entry(void *arg)
{
        ui_tooltip_registry_entry_t *entry = arg;

        if (entry->data->portal) {
                ui_widget_destroy(entry->data->portal);
        }
        free(entry->data);
        free(entry->key);
        free(entry);
}

void ui_register_tooltip(void)
{
        list_create(&ui_tooltip_registry);
        list_create(&ui_tooltip_live);
        list_create(&ui_tooltip_attr_list);
        ui_tooltip_prototype = ui_create_widget_prototype("tooltip", NULL);
        ui_tooltip_trigger_prototype =
            ui_create_widget_prototype("tooltip-trigger", NULL);
        ui_tooltip_content_prototype =
            ui_create_widget_prototype("tooltip-content", NULL);
        ui_tooltip_prototype->init = ui_tooltip_on_init;
        ui_tooltip_prototype->setattr = ui_tooltip_on_set_attr;
        ui_tooltip_prototype->update = ui_tooltip_on_update;
        ui_tooltip_prototype->destroy = ui_tooltip_on_destroy;
        ui_tooltip_content_prototype->init = ui_tooltip_content_on_init;
        ui_tooltip_content_prototype->setattr = ui_tooltip_content_on_set_attr;
        ui_load_css_string(ui_tooltip_css, __FILE__);
        ui_widget_on(ui_root(), "mousemove", ui_tooltip_module_on_mousemove,
                     NULL);
        ui_widget_on(ui_root(), "focus", ui_tooltip_module_on_focus, NULL);
        ui_widget_on(ui_root(), "blur", ui_tooltip_module_on_blur, NULL);
        ui_widget_on(ui_root(), "keydown", ui_tooltip_module_on_keydown, NULL);
}

void ui_unregister_tooltip(void)
{
        ui_widget_off(ui_root(), "mousemove", ui_tooltip_module_on_mousemove,
                      NULL);
        ui_widget_off(ui_root(), "focus", ui_tooltip_module_on_focus, NULL);
        ui_widget_off(ui_root(), "blur", ui_tooltip_module_on_blur, NULL);
        ui_widget_off(ui_root(), "keydown", ui_tooltip_module_on_keydown, NULL);
        list_destroy_without_node(&ui_tooltip_registry,
                                  ui_tooltip_destroy_registry_entry);
        list_destroy_without_node(&ui_tooltip_attr_list,
                                  ui_tooltip_attr_destroy);
        ui_tooltip_active = NULL;
}
