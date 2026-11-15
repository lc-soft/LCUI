/*
 * src/widgets/popover.c: -- Popover widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/popover.h>
#include <LCUI/widgets/portal.h>
#include <LCUI/widgets/text.h>
#include <css.h>
#include <ptk.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>

typedef struct ui_popover {
        ui_widget_t *owner;
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *portal;
        ui_widget_t *header;
        ui_widget_t *title;
        ui_widget_t *description;
        ui_widget_t *trigger_button;
        ui_widget_t *last_focus;
        bool bound;
        bool open;
        bool pending_open;
        bool reveal_pending;
        bool focus_pending;
        int settle_stage;
} ui_popover_t;

typedef struct ui_popover_content {
        ui_popover_side_t side;
        ui_popover_align_t align;
        int side_offset;
        int align_offset;
} ui_popover_content_t;

static const char *ui_popover_side_names[] = {
        "top",
        "right",
        "bottom",
        "left",
};

static const char *ui_popover_align_names[] = {
        "center",
        "start",
        "end",
};

#define UI_POPOVER_ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

static ui_widget_prototype_t *ui_popover_prototype;
static ui_widget_prototype_t *ui_popover_trigger_prototype;
static ui_widget_prototype_t *ui_popover_content_prototype;
static ui_widget_prototype_t *ui_popover_header_prototype;
static ui_widget_prototype_t *ui_popover_title_prototype;
static ui_widget_prototype_t *ui_popover_description_prototype;
static ui_popover_t *ui_popover_active;

static void ui_popover_open_data(ui_popover_t *data);
static void ui_popover_close_data(ui_popover_t *data);
static void ui_popover_on_content_layout(ui_widget_t *w, ui_event_t *e,
                                         void *arg);
static void ui_popover_on_portal_outside(ui_widget_t *w, ui_event_t *e,
                                         void *arg);
static void ui_popover_on_portal_destroy(ui_widget_t *w, ui_event_t *e,
                                         void *arg);
static void ui_popover_on_trigger_click(ui_widget_t *w, ui_event_t *e,
                                        void *arg);
static void ui_popover_on_content_destroy(ui_widget_t *w, ui_event_t *e,
                                          void *arg);
static void ui_popover_on_trigger_destroy(ui_widget_t *w, ui_event_t *e,
                                          void *arg);

static const char *ui_popover_css = "\
popover {\
  display: inline-block;\
}\
popover-trigger {\
  display: inline-block;\
}\
popover-content {\
  position: absolute;\
  pointer-events: auto;\
}\
popover-header {\
  display: flex;\
  flex-direction: column;\
  gap: 6px;\
}\
";

static ui_popover_t *ui_popover_get_data(ui_widget_t *w)
{
        ui_popover_t *data = ui_widget_get_data(w, ui_popover_prototype);

        if (data) {
                data->owner = w;
        }
        return data;
}

static ui_popover_content_t *ui_popover_content_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_popover_content_prototype);
}

static ui_popover_side_t ui_popover_side_normalize(ui_popover_side_t side)
{
        if (side < UI_SIDE_TOP || side > UI_SIDE_LEFT) {
                return UI_SIDE_BOTTOM;
        }
        return side;
}

static ui_popover_align_t ui_popover_align_normalize(ui_popover_align_t align)
{
        if (align < UI_ALIGN_CENTER || align > UI_ALIGN_END) {
                return UI_ALIGN_CENTER;
        }
        return align;
}

static ui_popover_side_t ui_popover_side_from_name(const char *name)
{
        size_t i;

        if (name) {
                for (i = 0; i < UI_POPOVER_ARRAY_LEN(ui_popover_side_names);
                     ++i) {
                        if (strcmp(name, ui_popover_side_names[i]) == 0) {
                                return (ui_popover_side_t)i;
                        }
                }
        }
        return UI_SIDE_BOTTOM;
}

static ui_popover_align_t ui_popover_align_from_name(const char *name)
{
        size_t i;

        if (name) {
                for (i = 0; i < UI_POPOVER_ARRAY_LEN(ui_popover_align_names);
                     ++i) {
                        if (strcmp(name, ui_popover_align_names[i]) == 0) {
                                return (ui_popover_align_t)i;
                        }
                }
        }
        return UI_ALIGN_CENTER;
}

static ui_widget_t *ui_popover_find_by_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, type)) {
                        return child;
                }
                found = ui_popover_find_by_type(child, type);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_popover_find_by_prototype(ui_widget_t *w,
                                                 ui_widget_prototype_t *proto)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child, proto)) {
                        return child;
                }
                found = ui_popover_find_by_prototype(child, proto);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static void ui_popover_ensure_trigger(ui_popover_t *data)
{
        ui_widget_t *trigger = data->trigger;
        ui_widget_t *button;

        if (!trigger) {
                trigger = ui_popover_find_by_prototype(
                    data->owner, ui_popover_trigger_prototype);
        }
        if (!trigger) {
                trigger = ui_create_popover_trigger();
                button = ui_create_button();
                ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
                ui_button_set_text(button, "Open popover");
                ui_widget_append(trigger, button);
                ui_widget_prepend(data->owner, trigger);
                data->trigger_button = button;
        }
        data->trigger = trigger;
        if (!data->trigger_button) {
                data->trigger_button =
                    ui_popover_find_by_type(trigger, "button");
        }
}

static void ui_popover_ensure_content(ui_popover_t *data)
{
        ui_widget_t *content = data->content;

        if (!content) {
                content = ui_popover_find_by_prototype(
                    data->owner, ui_popover_content_prototype);
        }
        if (!content) {
                content = ui_create_popover_content();
                ui_widget_append(data->owner, content);
        }
        data->content = content;
}

static void ui_popover_ensure_header(ui_popover_t *data)
{
        ui_widget_t *header = data->header;
        ui_widget_t *title;
        ui_widget_t *description;

        if (!header) {
                header = ui_popover_find_by_prototype(
                    data->content, ui_popover_header_prototype);
        }
        if (!header) {
                header = ui_create_popover_header();
                ui_widget_prepend(data->content, header);
        }
        data->header = header;

        title = data->title ? data->title
                            : ui_popover_find_by_prototype(
                                  header, ui_popover_title_prototype);
        if (!title) {
                title = ui_create_popover_title();
                ui_text_set_content(title, "Untitled popover");
                ui_widget_append(header, title);
        }
        data->title = title;

        description = data->description
                          ? data->description
                          : ui_popover_find_by_prototype(
                                header, ui_popover_description_prototype);
        if (!description) {
                description = ui_create_popover_description();
                ui_text_set_content(description, "Add supporting text here.");
                ui_widget_append(header, description);
        }
        data->description = description;
}

void ui_popover_build_children(ui_widget_t *popover)
{
        ui_popover_t *data = ui_popover_get_data(popover);

        data->owner = popover;
        ui_popover_ensure_trigger(data);
        ui_popover_ensure_content(data);
        ui_popover_ensure_header(data);
}

void ui_popover_set_trigger_text(ui_widget_t *popover, const char *text)
{
        ui_popover_t *data = ui_popover_get_data(popover);

        if (!data->trigger) {
                data->trigger = ui_popover_find_by_prototype(
                    popover, ui_popover_trigger_prototype);
        }
        if (!data->trigger_button) {
                data->trigger_button =
                    ui_popover_find_by_type(data->trigger, "button");
        }
        if (data->trigger_button) {
                ui_button_set_text(data->trigger_button, text);
        }
}

void ui_popover_set_title(ui_widget_t *popover, const char *text)
{
        ui_popover_t *data = ui_popover_get_data(popover);

        if (!data->title && data->content) {
                data->title = ui_popover_find_by_prototype(
                    data->content, ui_popover_title_prototype);
        }
        if (data->title) {
                ui_text_set_content(data->title, text);
        }
}

void ui_popover_set_description(ui_widget_t *popover, const char *text)
{
        ui_popover_t *data = ui_popover_get_data(popover);

        if (!data->description && data->content) {
                data->description = ui_popover_find_by_prototype(
                    data->content, ui_popover_description_prototype);
        }
        if (data->description) {
                ui_text_set_content(data->description, text);
        }
}

ui_widget_t *ui_popover_get_content(ui_widget_t *popover)
{
        ui_popover_build_children(popover);
        return ui_popover_get_data(popover)->content;
}

static void ui_popover_content_on_init(ui_widget_t *w)
{
        ui_popover_content_t *data = ui_widget_add_data(
            w, ui_popover_content_prototype, sizeof(ui_popover_content_t));

        data->side = UI_SIDE_BOTTOM;
        data->align = UI_ALIGN_CENTER;
        data->side_offset = 4;
        data->align_offset = 0;
}

static void ui_popover_content_on_set_attr(ui_widget_t *w, const char *name,
                                           const char *value)
{
        ui_popover_content_t *data = ui_popover_content_get_data(w);

        if (!data) {
                return;
        }
        if (strcmp(name, "side") == 0) {
                data->side = ui_popover_side_from_name(value);
        } else if (strcmp(name, "align") == 0) {
                data->align = ui_popover_align_from_name(value);
        } else if (strcmp(name, "side-offset") == 0) {
                data->side_offset = value ? atoi(value) : 4;
        } else if (strcmp(name, "align-offset") == 0) {
                data->align_offset = value ? atoi(value) : 0;
        }
}

void ui_popover_content_set_side(ui_widget_t *content, ui_popover_side_t side)
{
        side = ui_popover_side_normalize(side);
        ui_widget_set_attr(content, "side", ui_popover_side_names[side]);
}

void ui_popover_content_set_align(ui_widget_t *content,
                                  ui_popover_align_t align)
{
        align = ui_popover_align_normalize(align);
        ui_widget_set_attr(content, "align", ui_popover_align_names[align]);
}

void ui_popover_content_set_side_offset(ui_widget_t *content, int offset)
{
        char value[32];

        snprintf(value, sizeof(value), "%d", offset);
        ui_widget_set_attr(content, "side-offset", value);
}

void ui_popover_content_set_align_offset(ui_widget_t *content, int offset)
{
        char value[32];

        snprintf(value, sizeof(value), "%d", offset);
        ui_widget_set_attr(content, "align-offset", value);
}

static void ui_popover_apply_placement(ui_popover_t *data)
{
        ui_popover_content_t *content =
            data->content ? ui_popover_content_get_data(data->content) : NULL;
        ui_popover_side_t side = content ? content->side : UI_SIDE_BOTTOM;
        ui_popover_align_t align = content ? content->align : UI_ALIGN_CENTER;
        int side_offset = content ? content->side_offset : 4;
        int align_offset = content ? content->align_offset : 0;

        if (!data->portal) {
                return;
        }
        ui_portal_set_side(data->portal, ui_popover_side_names[side]);
        ui_portal_set_align(data->portal, ui_popover_align_names[align]);
        ui_portal_set_side_offset(data->portal, side_offset);
        ui_portal_set_align_offset(data->portal, align_offset);
}

static void ui_popover_emit(ui_popover_t *data, const char *name)
{
        ui_event_t event;

        if (!data->owner) {
                return;
        }
        ui_event_init(&event, name);
        ui_widget_emit_event(data->owner, event, NULL);
}

static void ui_popover_sync_open_attr(ui_popover_t *data)
{
        if (data->owner) {
                ui_widget_set_attr(data->owner, "open",
                                   data->open ? "true" : "false");
        }
}

static bool ui_popover_is_inside(ui_widget_t *ancestor, ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (w == ancestor) {
                        return true;
                }
        }
        return false;
}

static void ui_popover_on_last_focus_destroy(ui_widget_t *w, ui_event_t *e,
                                             void *arg)
{
        ui_popover_t *data = e->data;

        if (data->last_focus == w) {
                data->last_focus = NULL;
        }
}

static void ui_popover_focus_first_each(ui_widget_t *w, void *arg)
{
        ui_widget_t **target = arg;

        if (*target) {
                return;
        }
        if (w->tab_index >= 0 && !w->disabled &&
            w->computed_style.type_bits.pointer_events !=
                CSS_POINTER_EVENTS_NONE) {
                *target = w;
        }
}

static void ui_popover_focus_first(ui_popover_t *data)
{
        ui_widget_t *target = NULL;

        if (!data->content) {
                return;
        }
        ui_widget_each(data->content, ui_popover_focus_first_each, &target);
        if (target) {
                ui_set_focus(target);
        }
}

static void ui_popover_close_data(ui_popover_t *data)
{
        if (!data->open) {
                data->pending_open = false;
                return;
        }
        ui_widget_t *focus = ui_get_focus_in(data->owner);

        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        if (ui_popover_active == data) {
                ui_popover_active = NULL;
        }
        if (focus && data->content &&
            ui_popover_is_inside(data->content, focus)) {
                if (data->last_focus &&
                    data->last_focus->state != UI_WIDGET_STATE_DELETED) {
                        ui_set_focus(data->last_focus);
                }
        }
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_popover_on_last_focus_destroy, data);
                data->last_focus = NULL;
        }
        if (data->portal) {
                ui_portal_close(data->portal);
        }
        ui_popover_sync_open_attr(data);
        ui_popover_emit(data, "close");
}

static void ui_popover_on_content_layout(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        ui_popover_t *data = e->data;

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
                if (data->focus_pending) {
                        data->focus_pending = false;
                        ui_popover_focus_first(data);
                }
        }
}

static void ui_popover_on_portal_outside(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        ui_popover_close_data(e->data);
}

static void ui_popover_on_portal_destroy(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        ui_popover_t *data = e->data;

        data->portal = NULL;
        data->open = false;
        data->focus_pending = false;
        if (ui_popover_active == data) {
                ui_popover_active = NULL;
        }
}

static void ui_popover_on_trigger_click(ui_widget_t *w, ui_event_t *e,
                                        void *arg)
{
        ui_popover_t *data = e->data;

        if (data->open) {
                ui_popover_close_data(data);
        } else {
                ui_popover_open_data(data);
        }
}

static void ui_popover_clear_last_focus(ui_popover_t *data)
{
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_popover_on_last_focus_destroy, data);
                data->last_focus = NULL;
        }
}

static void ui_popover_on_content_destroy(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        ui_popover_t *data = e->data;
        bool was_open = data->open;

        data->content = NULL;
        data->header = NULL;
        data->title = NULL;
        data->description = NULL;
        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        if (ui_popover_active == data) {
                ui_popover_active = NULL;
        }
        ui_popover_clear_last_focus(data);
        if (was_open && data->owner &&
            data->owner->state != UI_WIDGET_STATE_DELETED) {
                ui_popover_sync_open_attr(data);
                ui_popover_emit(data, "close");
        }
}

static void ui_popover_on_trigger_destroy(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        ui_popover_t *data = e->data;
        bool was_open = data->open;

        data->trigger = NULL;
        data->trigger_button = NULL;
        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        if (ui_popover_active == data) {
                ui_popover_active = NULL;
        }
        ui_popover_clear_last_focus(data);
        if (data->owner && data->owner->state != UI_WIDGET_STATE_DELETED) {
                if (data->portal) {
                        ui_portal_close(data->portal);
                }
                if (was_open) {
                        ui_popover_sync_open_attr(data);
                        ui_popover_emit(data, "close");
                }
        }
}

static bool ui_popover_bind_structure(ui_popover_t *data)
{
        if (data->bound) {
                return data->portal != NULL;
        }
        if (!data->owner) {
                return false;
        }
        if (!data->trigger) {
                data->trigger = ui_popover_find_by_prototype(
                    data->owner, ui_popover_trigger_prototype);
        }
        if (!data->content) {
                data->content = ui_popover_find_by_prototype(
                    data->owner, ui_popover_content_prototype);
        }
        if (!data->trigger || !data->content) {
                return false;
        }
        data->portal = ui_create_portal();
        ui_portal_set_passthrough(data->portal, true);
        ui_portal_set_outside_close(data->portal, true);
        ui_widget_append(data->owner, data->portal);
        ui_portal_set_content(data->portal, data->content);
        ui_portal_set_anchor(data->portal, data->trigger);
        ui_portal_set_follow_anchor(data->portal, true);
        ui_widget_on(data->content, "afterlayout", ui_popover_on_content_layout,
                     data);
        ui_widget_on(data->content, "destroy", ui_popover_on_content_destroy,
                     data);
        ui_widget_on(data->portal, "portal-outside",
                     ui_popover_on_portal_outside, data);
        ui_widget_on(data->portal, "destroy", ui_popover_on_portal_destroy,
                     data);
        ui_widget_on(data->trigger, "click", ui_popover_on_trigger_click, data);
        ui_widget_on(data->trigger, "destroy", ui_popover_on_trigger_destroy,
                     data);
        data->bound = true;
        if (data->pending_open) {
                data->pending_open = false;
                ui_popover_open_data(data);
        }
        return true;
}

static void ui_popover_open_data(ui_popover_t *data)
{
        if (data->open) {
                return;
        }
        if (!data->bound && !ui_popover_bind_structure(data)) {
                data->pending_open = true;
                return;
        }
        if (!data->portal || !data->content) {
                data->pending_open = true;
                return;
        }
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_popover_on_last_focus_destroy, data);
                data->last_focus = NULL;
        }
        data->last_focus = ui_get_focus_in(data->owner);
        if (data->last_focus) {
                ui_widget_on(data->last_focus, "destroy",
                             ui_popover_on_last_focus_destroy, data);
        }
        data->focus_pending = true;
        if (ui_popover_active && ui_popover_active != data) {
                ui_popover_close_data(ui_popover_active);
        }
        data->open = true;
        data->pending_open = false;
        ui_popover_active = data;
        if (data->portal) {
                ui_portal_set_anchor(data->portal, data->trigger);
        }
        ui_popover_apply_placement(data);
        /* The content was reparented into portal-root before its subtree was
         * laid out; refresh descendants so text styles are computed. */
        ui_widget_refresh_children_style(data->content);
        ui_widget_set_style_keyword_value(data->content, css_prop_visibility,
                                          CSS_KEYWORD_HIDDEN);
        data->reveal_pending = true;
        data->settle_stage = 0;
        ui_portal_open(data->portal);
        ui_popover_sync_open_attr(data);
        ui_popover_emit(data, "open");
}

static void ui_popover_on_set_attr(ui_widget_t *w, const char *name,
                                   const char *value)
{
        ui_popover_t *data = ui_popover_get_data(w);
        bool open;

        if (!data || strcmp(name, "open") != 0) {
                return;
        }
        open = value && strcmp(value, "false") != 0 && strcmp(value, "0") != 0;
        if (open == data->open) {
                if (!open) {
                        data->pending_open = false;
                }
                return;
        }
        if (open) {
                ui_popover_open_data(data);
        } else {
                ui_popover_close_data(data);
        }
}

static void ui_popover_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_popover_t *data = ui_popover_get_data(w);

        if (task != UI_TASK_BEFORE_UPDATE) {
                return;
        }
        data->owner = w;
        if (!data->bound) {
                ui_popover_bind_structure(data);
        }
}

static void ui_popover_on_destroy(ui_widget_t *w)
{
        ui_popover_t *data = ui_popover_get_data(w);

        if (ui_popover_active == data) {
                ui_popover_active = NULL;
        }
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_popover_on_last_focus_destroy, data);
                data->last_focus = NULL;
        }
        data->owner = NULL;
        data->portal = NULL;
        data->content = NULL;
        data->open = false;
        data->focus_pending = false;
}

static void ui_popover_module_on_keydown(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        if (e->key.code != KEY_ESCAPE) {
                return;
        }
        if (ui_popover_active && ui_popover_active->open) {
                ui_popover_close_data(ui_popover_active);
        }
}

void ui_popover_set_open(ui_widget_t *popover, bool open)
{
        ui_widget_set_attr(popover, "open", open ? "true" : "false");
}

void ui_popover_open(ui_widget_t *popover)
{
        ui_popover_set_open(popover, true);
}

void ui_popover_close(ui_widget_t *popover)
{
        ui_popover_set_open(popover, false);
}

bool ui_popover_is_open(ui_widget_t *popover)
{
        return ui_popover_get_data(popover)->open;
}

static void ui_popover_on_init(ui_widget_t *w)
{
        ui_popover_t *data =
            ui_widget_add_data(w, ui_popover_prototype, sizeof(ui_popover_t));

        memset(data, 0, sizeof(ui_popover_t));
}

ui_widget_t *ui_create_popover(void)
{
        return ui_create_widget_with_prototype(ui_popover_prototype);
}

ui_widget_t *ui_create_popover_trigger(void)
{
        return ui_create_widget_with_prototype(ui_popover_trigger_prototype);
}

ui_widget_t *ui_create_popover_content(void)
{
        return ui_create_widget_with_prototype(ui_popover_content_prototype);
}

ui_widget_t *ui_create_popover_header(void)
{
        return ui_create_widget_with_prototype(ui_popover_header_prototype);
}

ui_widget_t *ui_create_popover_title(void)
{
        return ui_create_widget_with_prototype(ui_popover_title_prototype);
}

ui_widget_t *ui_create_popover_description(void)
{
        return ui_create_widget_with_prototype(
            ui_popover_description_prototype);
}

void ui_register_popover(void)
{
        ui_popover_prototype = ui_create_widget_prototype("popover", NULL);
        ui_popover_trigger_prototype =
            ui_create_widget_prototype("popover-trigger", NULL);
        ui_popover_content_prototype =
            ui_create_widget_prototype("popover-content", NULL);
        ui_popover_header_prototype =
            ui_create_widget_prototype("popover-header", NULL);
        ui_popover_title_prototype =
            ui_create_widget_prototype("popover-title", "text");
        ui_popover_description_prototype =
            ui_create_widget_prototype("popover-description", "text");
        ui_popover_prototype->init = ui_popover_on_init;
        ui_popover_prototype->setattr = ui_popover_on_set_attr;
        ui_popover_prototype->update = ui_popover_on_update;
        ui_popover_prototype->destroy = ui_popover_on_destroy;
        ui_popover_content_prototype->init = ui_popover_content_on_init;
        ui_popover_content_prototype->setattr = ui_popover_content_on_set_attr;
        ui_load_css_string(ui_popover_css, __FILE__);
        ui_widget_on(ui_root(), "keydown", ui_popover_module_on_keydown, NULL);
}

void ui_unregister_popover(void)
{
        ui_widget_off(ui_root(), "keydown", ui_popover_module_on_keydown, NULL);
}
