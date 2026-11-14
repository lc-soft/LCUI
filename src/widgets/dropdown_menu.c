/*
 * src/widgets/dropdown_menu.c: -- DropdownMenu widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <LCUI/widgets/dropdown_menu.h>
#include <LCUI/widgets/icon.h>
#include <LCUI/widgets/portal.h>
#include <LCUI/widgets/text.h>
#include <yutil.h>
#include <css.h>
#include <ptk.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>

typedef enum ui_dropdown_menu_side {
        UI_DROPDOWN_MENU_SIDE_TOP = 0,
        UI_DROPDOWN_MENU_SIDE_RIGHT,
        UI_DROPDOWN_MENU_SIDE_BOTTOM,
        UI_DROPDOWN_MENU_SIDE_LEFT,
} ui_dropdown_menu_side_t;

typedef enum ui_dropdown_menu_align {
        UI_DROPDOWN_MENU_ALIGN_START = 0,
        UI_DROPDOWN_MENU_ALIGN_CENTER,
        UI_DROPDOWN_MENU_ALIGN_END,
} ui_dropdown_menu_align_t;

typedef struct ui_dropdown_menu {
        ui_widget_t *owner;
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *portal;
        ui_widget_t *parent_menu;
        ui_widget_t *highlighted;
        ui_widget_t *last_focus;
        bool open;
        bool bound;
        bool pending_open;
        bool submenu;
        bool reveal_pending;
        bool focus_pending;
        bool match_trigger_width;
        int settle_stage;
        wchar_t typeahead[32];
        size_t typeahead_len;
        int typeahead_start;
        int typeahead_match;
        int64_t typeahead_time;
} ui_dropdown_menu_t;

typedef struct ui_dropdown_menu_content {
        ui_widget_t *menu;
        ui_dropdown_menu_side_t side;
        ui_dropdown_menu_align_t align;
        int side_offset;
        int align_offset;
} ui_dropdown_menu_content_t;

typedef struct ui_dropdown_menu_items {
        ui_widget_t **items;
        size_t count;
        size_t capacity;
} ui_dropdown_menu_items_t;

static ui_widget_prototype_t *ui_dropdown_menu_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_trigger_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_content_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_group_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_label_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_item_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_checkbox_item_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_radio_group_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_radio_item_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_separator_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_shortcut_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_sub_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_sub_trigger_prototype;
static ui_widget_prototype_t *ui_dropdown_menu_sub_content_prototype;

static ui_dropdown_menu_t *ui_dropdown_active_menu;
static ui_dropdown_menu_t *ui_dropdown_key_menu;

static void ui_dropdown_menu_open_data(ui_dropdown_menu_t *data);
static void ui_dropdown_menu_close_data(ui_dropdown_menu_t *data);
static bool ui_dropdown_menu_value_is_true(const char *value);

static const char *ui_dropdown_menu_side_names[] = {
        "top",
        "right",
        "bottom",
        "left",
};

static const char *ui_dropdown_menu_align_names[] = {
        "start",
        "center",
        "end",
};

#define UI_DROPDOWN_MENU_ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

static ui_dropdown_menu_t *ui_dropdown_menu_get_data(ui_widget_t *w)
{
        if (!w) {
                return NULL;
        }
        if (ui_check_widget_prototype(w, ui_dropdown_menu_prototype)) {
                return ui_widget_get_data(w, ui_dropdown_menu_prototype);
        }
        if (ui_check_widget_prototype(w, ui_dropdown_menu_sub_prototype)) {
                return ui_widget_get_data(w, ui_dropdown_menu_sub_prototype);
        }
        return NULL;
}

static ui_dropdown_menu_content_t *ui_dropdown_menu_content_get_data(
    ui_widget_t *w)
{
        if (!w) {
                return NULL;
        }
        if (ui_check_widget_prototype(w, ui_dropdown_menu_content_prototype)) {
                return ui_widget_get_data(w,
                                          ui_dropdown_menu_content_prototype);
        }
        if (ui_check_widget_prototype(w,
                                      ui_dropdown_menu_sub_content_prototype)) {
                return ui_widget_get_data(
                    w, ui_dropdown_menu_sub_content_prototype);
        }
        return NULL;
}

static bool ui_dropdown_menu_is_content(ui_widget_t *w)
{
        return w && (ui_check_widget_prototype(
                         w, ui_dropdown_menu_content_prototype) ||
                     ui_check_widget_prototype(
                         w, ui_dropdown_menu_sub_content_prototype));
}

static bool ui_dropdown_menu_is_item(ui_widget_t *w)
{
        return w &&
               (ui_check_widget_prototype(w, ui_dropdown_menu_item_prototype) ||
                ui_check_widget_prototype(
                    w, ui_dropdown_menu_checkbox_item_prototype) ||
                ui_check_widget_prototype(
                    w, ui_dropdown_menu_radio_item_prototype));
}

static bool ui_dropdown_menu_is_sub_trigger(ui_widget_t *w)
{
        return w && ui_check_widget_prototype(
                        w, ui_dropdown_menu_sub_trigger_prototype);
}

static bool ui_dropdown_menu_is_interactive(ui_widget_t *w)
{
        return ui_dropdown_menu_is_item(w) ||
               ui_dropdown_menu_is_sub_trigger(w);
}

static ui_widget_t *ui_dropdown_menu_find_content(ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (ui_dropdown_menu_is_content(w)) {
                        return w;
                }
        }
        return NULL;
}

static ui_widget_t *ui_dropdown_menu_find_descendant(
    ui_widget_t *w, ui_widget_prototype_t *prototype)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child, prototype)) {
                        return child;
                }
                found = ui_dropdown_menu_find_descendant(child, prototype);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_dropdown_menu_find_trigger_by_attr(ui_widget_t *w,
                                                          const char *name)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_dropdown_menu_value_is_true(
                        ui_widget_get_attr(child, name))) {
                        return child;
                }
                found = ui_dropdown_menu_find_trigger_by_attr(child, name);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_dropdown_menu_root_widget(ui_widget_t *menu)
{
        ui_dropdown_menu_t *data = ui_dropdown_menu_get_data(menu);

        while (data && data->parent_menu) {
                menu = data->parent_menu;
                data = ui_dropdown_menu_get_data(menu);
        }
        return menu;
}

static bool ui_dropdown_menu_is_disabled(ui_widget_t *w)
{
        return w->disabled || ui_widget_has_status(w, "disabled");
}

static bool ui_dropdown_menu_value_is_true(const char *value)
{
        return value &&
               (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
}

static void ui_dropdown_menu_emit(ui_widget_t *w, const char *name, void *arg)
{
        ui_event_t event;

        if (!w) {
                return;
        }
        ui_event_init(&event, name);
        ui_widget_emit_event(w, event, arg);
}

static void ui_dropdown_menu_sync_open_attr(ui_dropdown_menu_t *data)
{
        if (!data->owner) {
                return;
        }
        ui_widget_set_attr(data->owner, "open", data->open ? "true" : "false");
        if (data->open) {
                ui_widget_remove_status(data->owner, "closed");
                ui_widget_add_status(data->owner, "open");
        } else {
                ui_widget_remove_status(data->owner, "open");
                ui_widget_add_status(data->owner, "closed");
        }
}

static ui_dropdown_menu_side_t ui_dropdown_menu_side_from_name(const char *name)
{
        size_t i;

        if (name) {
                for (i = 0; i < UI_DROPDOWN_MENU_ARRAY_LEN(
                                    ui_dropdown_menu_side_names);
                     ++i) {
                        if (strcmp(name, ui_dropdown_menu_side_names[i]) == 0) {
                                return (ui_dropdown_menu_side_t)i;
                        }
                }
        }
        return UI_DROPDOWN_MENU_SIDE_BOTTOM;
}

static ui_dropdown_menu_align_t ui_dropdown_menu_align_from_name(
    const char *name)
{
        size_t i;

        if (name) {
                for (i = 0; i < UI_DROPDOWN_MENU_ARRAY_LEN(
                                    ui_dropdown_menu_align_names);
                     ++i) {
                        if (strcmp(name, ui_dropdown_menu_align_names[i]) ==
                            0) {
                                return (ui_dropdown_menu_align_t)i;
                        }
                }
        }
        return UI_DROPDOWN_MENU_ALIGN_START;
}

static void ui_dropdown_menu_apply_placement(ui_dropdown_menu_t *data)
{
        ui_dropdown_menu_content_t *content;

        if (!data->portal || !data->content) {
                return;
        }
        content = ui_dropdown_menu_content_get_data(data->content);
        if (!content) {
                return;
        }
        ui_portal_set_side(data->portal,
                           ui_dropdown_menu_side_names[content->side]);
        ui_portal_set_align(data->portal,
                            ui_dropdown_menu_align_names[content->align]);
        ui_portal_set_side_offset(data->portal, content->side_offset);
        ui_portal_set_align_offset(data->portal, content->align_offset);
}

static void ui_dropdown_menu_collect_item(ui_widget_t *w, void *arg)
{
        ui_dropdown_menu_items_t *items = arg;
        ui_widget_t **next;

        if (!ui_dropdown_menu_is_interactive(w) || w->disabled ||
            ui_widget_has_status(w, "disabled")) {
                return;
        }
        if (items->count >= items->capacity) {
                size_t capacity = items->capacity ? items->capacity * 2 : 8;

                next = realloc(items->items, capacity * sizeof(ui_widget_t *));
                if (!next) {
                        return;
                }
                items->items = next;
                items->capacity = capacity;
        }
        items->items[items->count++] = w;
}

static void ui_dropdown_menu_free_items(ui_dropdown_menu_items_t *items)
{
        free(items->items);
        items->items = NULL;
        items->count = 0;
        items->capacity = 0;
}

static ui_dropdown_menu_items_t ui_dropdown_menu_get_items(
    ui_dropdown_menu_t *data)
{
        ui_dropdown_menu_items_t items = { 0 };

        if (data && data->content) {
                ui_widget_each(data->content, ui_dropdown_menu_collect_item,
                               &items);
        }
        return items;
}

static void ui_dropdown_menu_highlight(ui_dropdown_menu_t *data,
                                       ui_widget_t *item)
{
        if (!data) {
                return;
        }
        if (data->highlighted == item) {
                if (item) {
                        ui_set_focus(item);
                }
                return;
        }
        if (data->highlighted) {
                ui_widget_remove_status(data->highlighted, "highlighted");
        }
        data->highlighted = item;
        if (item) {
                ui_widget_add_status(item, "highlighted");
                ui_set_focus(item);
        }
}

static void ui_dropdown_menu_highlight_first(ui_dropdown_menu_t *data)
{
        ui_dropdown_menu_items_t items = ui_dropdown_menu_get_items(data);

        if (items.count > 0) {
                ui_dropdown_menu_highlight(data, items.items[0]);
        }
        ui_dropdown_menu_free_items(&items);
}

/* Move keyboard focus into the menu without adding the highlighted status.
 * The first item only looks selected after the user presses an arrow key. */
static void ui_dropdown_menu_focus_first(ui_dropdown_menu_t *data)
{
        ui_dropdown_menu_items_t items = ui_dropdown_menu_get_items(data);

        if (items.count > 0) {
                ui_set_focus(items.items[0]);
        }
        ui_dropdown_menu_free_items(&items);
}

static void ui_dropdown_menu_reset_typeahead(ui_dropdown_menu_t *data)
{
        data->typeahead[0] = 0;
        data->typeahead_len = 0;
        data->typeahead_start = -1;
        data->typeahead_match = -1;
        data->typeahead_time = 0;
}

static void ui_dropdown_menu_collect_label(ui_widget_t *w, void *arg)
{
        wchar_t text[64];
        wchar_t *buf = arg;
        size_t len = wcslen(buf);
        size_t size = 128;

        if (ui_check_widget_prototype(w, ui_dropdown_menu_shortcut_prototype)) {
                return;
        }
        if (!ui_check_widget_type(w, "text") ||
            !ui_text_get_content_w(w, text, 64)) {
                return;
        }
        if (len + wcslen(text) + 1 < size) {
                wcsncat(buf, text, size - len - 1);
        }
}

static bool ui_dropdown_menu_label_starts_with(const wchar_t *text,
                                               const wchar_t *prefix)
{
        for (; *prefix; ++text, ++prefix) {
                if (!*text || towlower(*text) != towlower(*prefix)) {
                        return false;
                }
        }
        return true;
}

static bool ui_dropdown_menu_typeahead_allows_rapid(
    ui_dropdown_menu_items_t *items)
{
        wchar_t label[128];
        size_t i;

        for (i = 0; i < items->count; ++i) {
                label[0] = 0;
                ui_widget_each(items->items[i], ui_dropdown_menu_collect_label,
                               label);
                if (label[0] && label[1] &&
                    towlower(label[0]) == towlower(label[1])) {
                        return false;
                }
        }
        return true;
}

static int ui_dropdown_menu_typeahead_find(ui_dropdown_menu_items_t *items,
                                           const wchar_t *search, int start)
{
        wchar_t label[128];
        size_t i;
        size_t offset;
        size_t count = items->count;
        size_t index;

        if (count == 0) {
                return -1;
        }
        index = start < 0 ? 0 : (size_t)start % count;
        for (offset = 0; offset < count; ++offset) {
                i = (index + offset) % count;
                label[0] = 0;
                ui_widget_each(items->items[i], ui_dropdown_menu_collect_label,
                               label);
                if (label[0] &&
                    ui_dropdown_menu_label_starts_with(label, search)) {
                        return (int)i;
                }
        }
        return -1;
}

static int ui_dropdown_menu_typeahead_current(ui_dropdown_menu_t *data,
                                              ui_dropdown_menu_items_t *items)
{
        size_t i;

        if (!data->highlighted) {
                return -1;
        }
        for (i = 0; i < items->count; ++i) {
                if (items->items[i] == data->highlighted) {
                        return (int)i;
                }
        }
        return -1;
}

static void ui_dropdown_menu_typeahead(ui_dropdown_menu_t *data, wchar_t ch)
{
        ui_dropdown_menu_items_t items = ui_dropdown_menu_get_items(data);
        int64_t now = y_gettime();
        int index;
        bool new_session;

        if (items.count == 0) {
                ui_dropdown_menu_reset_typeahead(data);
                ui_dropdown_menu_free_items(&items);
                return;
        }
        if (data->typeahead_len > 0 && now - data->typeahead_time > 750) {
                data->typeahead_len = 0;
                data->typeahead_start = data->typeahead_match;
        }
        new_session = data->typeahead_len == 0;
        if (new_session) {
                data->typeahead_start =
                    ui_dropdown_menu_typeahead_current(data, &items);
        }
        if (data->typeahead_len == 1 && data->typeahead[0] == ch &&
            ui_dropdown_menu_typeahead_allows_rapid(&items)) {
                data->typeahead_len = 0;
                data->typeahead_start = data->typeahead_match;
        }
        if (data->typeahead_len + 1 <
            sizeof(data->typeahead) / sizeof(data->typeahead[0])) {
                data->typeahead[data->typeahead_len++] = ch;
                data->typeahead[data->typeahead_len] = 0;
        }
        index = ui_dropdown_menu_typeahead_find(&items, data->typeahead,
                                                data->typeahead_start + 1);
        if (index >= 0) {
                ui_dropdown_menu_highlight(data, items.items[index]);
                data->typeahead_match = index;
        } else {
                data->typeahead_len = 0;
        }
        data->typeahead_time = now;
        ui_dropdown_menu_free_items(&items);
}

static bool ui_dropdown_menu_is_typeahead_key(int code)
{
        return (code >= 'A' && code <= 'Z') || (code >= 'a' && code <= 'z') ||
               (code >= '0' && code <= '9');
}

static void ui_dropdown_menu_close_subs(ui_widget_t *content,
                                        ui_widget_t *except)
{
        ui_widget_t *child;
        size_t i;

        if (!content) {
                return;
        }
        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                ui_dropdown_menu_t *data;

                if (ui_check_widget_prototype(child,
                                              ui_dropdown_menu_sub_prototype) &&
                    child != except) {
                        data = ui_dropdown_menu_get_data(child);
                        if (data && data->open) {
                                ui_dropdown_menu_close_data(data);
                        }
                }
                ui_dropdown_menu_close_subs(child, except);
        }
}

static void ui_dropdown_menu_on_content_layout(ui_widget_t *w, ui_event_t *e,
                                               void *arg)
{
        ui_dropdown_menu_t *data = e->data;

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
                        ui_dropdown_menu_focus_first(data);
                }
        }
}

static bool ui_dropdown_menu_is_inside(ui_widget_t *ancestor, ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (w == ancestor) {
                        return true;
                }
        }
        return false;
}

static void ui_dropdown_menu_on_last_focus_destroy(ui_widget_t *w,
                                                   ui_event_t *e, void *arg)
{
        ui_dropdown_menu_t *data = e->data;

        if (data->last_focus == w) {
                data->last_focus = NULL;
        }
}

static void ui_dropdown_menu_clear_last_focus(ui_dropdown_menu_t *data)
{
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_dropdown_menu_on_last_focus_destroy, data);
                data->last_focus = NULL;
        }
}

static void ui_dropdown_menu_close_data(ui_dropdown_menu_t *data)
{
        ui_widget_t *focus;
        bool was_open;

        if (!data) {
                return;
        }
        was_open = data->open;
        ui_dropdown_menu_reset_typeahead(data);
        if (!data->open) {
                data->pending_open = false;
                return;
        }
        ui_dropdown_menu_close_subs(data->content, NULL);
        focus = ui_get_focus();
        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        ui_dropdown_menu_highlight(data, NULL);
        if (ui_dropdown_active_menu == data) {
                ui_dropdown_active_menu = NULL;
        }
        if (ui_dropdown_key_menu == data) {
                ui_dropdown_key_menu =
                    data->parent_menu
                        ? ui_dropdown_menu_get_data(data->parent_menu)
                        : NULL;
        }
        if (focus && data->content &&
            ui_dropdown_menu_is_inside(data->content, focus) &&
            data->last_focus &&
            data->last_focus->state != UI_WIDGET_STATE_DELETED) {
                ui_set_focus(data->last_focus);
        }
        ui_dropdown_menu_clear_last_focus(data);
        if (data->portal) {
                ui_portal_close(data->portal);
        }
        ui_dropdown_menu_sync_open_attr(data);
        if (was_open) {
                ui_dropdown_menu_emit(data->owner, "close", NULL);
        }
}

static void ui_dropdown_menu_on_portal_destroy(ui_widget_t *w, ui_event_t *e,
                                               void *arg)
{
        ui_dropdown_menu_t *data = e->data;

        data->portal = NULL;
        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        if (ui_dropdown_active_menu == data) {
                ui_dropdown_active_menu = NULL;
        }
        if (ui_dropdown_key_menu == data) {
                ui_dropdown_key_menu = NULL;
        }
}

static void ui_dropdown_menu_on_content_destroy(ui_widget_t *w, ui_event_t *e,
                                                void *arg)
{
        ui_dropdown_menu_t *data = e->data;
        bool was_open = data->open;

        data->content = NULL;
        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        ui_dropdown_menu_highlight(data, NULL);
        if (ui_dropdown_active_menu == data) {
                ui_dropdown_active_menu = NULL;
        }
        if (ui_dropdown_key_menu == data) {
                ui_dropdown_key_menu = NULL;
        }
        if (data->portal) {
                ui_portal_close(data->portal);
        }
        if (was_open && data->owner &&
            data->owner->state != UI_WIDGET_STATE_DELETED) {
                ui_dropdown_menu_sync_open_attr(data);
                ui_dropdown_menu_emit(data->owner, "close", NULL);
        }
}

static void ui_dropdown_menu_on_trigger_destroy(ui_widget_t *w, ui_event_t *e,
                                                void *arg)
{
        ui_dropdown_menu_t *data = e->data;
        bool was_open = data->open;

        data->trigger = NULL;
        data->open = false;
        data->pending_open = false;
        data->reveal_pending = false;
        data->focus_pending = false;
        data->settle_stage = 0;
        if (ui_dropdown_active_menu == data) {
                ui_dropdown_active_menu = NULL;
        }
        if (ui_dropdown_key_menu == data) {
                ui_dropdown_key_menu = NULL;
        }
        if (data->portal) {
                ui_portal_close(data->portal);
        }
        if (was_open && data->owner &&
            data->owner->state != UI_WIDGET_STATE_DELETED) {
                ui_dropdown_menu_sync_open_attr(data);
                ui_dropdown_menu_emit(data->owner, "close", NULL);
        }
}

static ui_widget_t *ui_dropdown_menu_item_from_target(ui_widget_t *target,
                                                      ui_widget_t *content)
{
        for (; target && target != content; target = target->parent) {
                if (ui_dropdown_menu_is_interactive(target)) {
                        return target;
                }
        }
        return NULL;
}

static ui_widget_t *ui_dropdown_menu_radio_group_from_item(ui_widget_t *item,
                                                           ui_widget_t *content)
{
        for (; item && item != content; item = item->parent) {
                if (ui_check_widget_prototype(
                        item, ui_dropdown_menu_radio_group_prototype)) {
                        return item;
                }
        }
        return NULL;
}

static void ui_dropdown_menu_sync_radio_group(ui_widget_t *group)
{
        const char *value = ui_widget_get_attr(group, "value");
        ui_widget_t *item;
        const char *item_value;
        size_t i;

        for (i = 0; (item = ui_widget_get_child(group, i)); ++i) {
                ui_dropdown_menu_sync_radio_group(item);
                if (!ui_check_widget_prototype(
                        item, ui_dropdown_menu_radio_item_prototype)) {
                        continue;
                }
                item_value = ui_widget_get_attr(item, "value");
                if (value && item_value && strcmp(value, item_value) == 0) {
                        ui_widget_add_status(item, "checked");
                } else {
                        ui_widget_remove_status(item, "checked");
                }
        }
}

static void ui_dropdown_menu_activate(ui_widget_t *item)
{
        ui_widget_t *content;
        ui_widget_t *root;
        ui_dropdown_menu_t *data;
        ui_dropdown_menu_content_t *content_data;

        if (!item || ui_dropdown_menu_is_disabled(item)) {
                return;
        }
        if (ui_dropdown_menu_is_sub_trigger(item)) {
                ui_dropdown_menu_t *sub =
                    ui_dropdown_menu_get_data(item->parent);

                if (sub) {
                        if (sub->open) {
                                ui_dropdown_menu_close_data(sub);
                        } else {
                                ui_dropdown_menu_open_data(sub);
                        }
                }
                return;
        }
        content = ui_dropdown_menu_find_content(item);
        content_data = ui_dropdown_menu_content_get_data(content);
        data =
            content_data ? ui_dropdown_menu_get_data(content_data->menu) : NULL;
        if (!data) {
                return;
        }
        root = ui_dropdown_menu_root_widget(data->owner);
        if (ui_check_widget_prototype(
                item, ui_dropdown_menu_checkbox_item_prototype)) {
                bool checked = ui_dropdown_menu_value_is_true(
                    ui_widget_get_attr(item, "checked"));

                ui_widget_set_attr(item, "checked", checked ? "false" : "true");
                ui_dropdown_menu_emit(root, "checked-change", item);
                return;
        }
        if (ui_check_widget_prototype(item,
                                      ui_dropdown_menu_radio_item_prototype)) {
                ui_widget_t *group =
                    ui_dropdown_menu_radio_group_from_item(item, content);
                const char *value = ui_widget_get_attr(item, "value");

                if (group && value) {
                        ui_widget_set_attr(group, "value", value);
                }
                ui_dropdown_menu_emit(root, "value-change", item);
                return;
        }
        ui_dropdown_menu_close_data(ui_dropdown_active_menu);
        ui_dropdown_menu_emit(root, "select", item);
}

static void ui_dropdown_menu_on_content_click(ui_widget_t *w, ui_event_t *e,
                                              void *arg)
{
        ui_widget_t *target;

        target = ui_dropdown_menu_item_from_target(e->target, w);
        if (!target || ui_dropdown_menu_is_disabled(target)) {
                return;
        }
        ui_dropdown_menu_activate(target);
}

static void ui_dropdown_menu_on_content_mouseover(ui_widget_t *w, ui_event_t *e,
                                                  void *arg)
{
        ui_widget_t *target;
        ui_dropdown_menu_t *data;

        target = ui_dropdown_menu_item_from_target(e->target, w);
        if (ui_dropdown_menu_is_sub_trigger(target)) {
                data = ui_dropdown_menu_get_data(target->parent);
                if (data && !data->open &&
                    !ui_dropdown_menu_is_disabled(target)) {
                        ui_dropdown_menu_open_data(data);
                }
                return;
        }
        if (ui_dropdown_menu_is_item(target)) {
                ui_dropdown_menu_close_subs(w, NULL);
        }
}

static void ui_dropdown_menu_bind_structure(ui_dropdown_menu_t *data);

static void ui_dropdown_menu_on_trigger_click(ui_widget_t *w, ui_event_t *e,
                                              void *arg)
{
        ui_dropdown_menu_t *data = e->data;

        if (data->open) {
                ui_dropdown_menu_close_data(data);
        } else {
                ui_dropdown_menu_open_data(data);
        }
}

static void ui_dropdown_menu_on_portal_outside(ui_widget_t *w, ui_event_t *e,
                                               void *arg)
{
        ui_dropdown_menu_close_data(e->data);
}

static void ui_dropdown_menu_bind_structure(ui_dropdown_menu_t *data)
{
        ui_widget_t *owner = data->owner;
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *parent_content;
        ui_dropdown_menu_content_t *content_data;

        if (data->bound || !owner || owner->state == UI_WIDGET_STATE_DELETED) {
                return;
        }
        if (!data->trigger) {
                data->trigger = ui_dropdown_menu_find_descendant(
                    owner, data->submenu
                               ? ui_dropdown_menu_sub_trigger_prototype
                               : ui_dropdown_menu_trigger_prototype);
        }
        if (!data->trigger) {
                data->trigger = ui_dropdown_menu_find_trigger_by_attr(
                    owner, data->submenu ? "data-dropdown-menu-sub-trigger"
                                         : "data-dropdown-menu-trigger");
        }
        if (!data->content) {
                data->content = ui_dropdown_menu_find_descendant(
                    owner, data->submenu
                               ? ui_dropdown_menu_sub_content_prototype
                               : ui_dropdown_menu_content_prototype);
        }
        trigger = data->trigger;
        content = data->content;
        if (!trigger || !content) {
                return;
        }
        if (data->submenu) {
                parent_content = ui_dropdown_menu_find_content(owner->parent);
                if (parent_content) {
                        ui_dropdown_menu_content_t *parent_data =
                            ui_dropdown_menu_content_get_data(parent_content);

                        if (parent_data) {
                                data->parent_menu = parent_data->menu;
                        }
                }
        }
        data->portal = ui_create_dropdown_menu_portal();
        ui_widget_append(owner, data->portal);
        ui_portal_set_content(data->portal, content);
        ui_portal_set_anchor(data->portal, trigger);
        ui_portal_set_follow_anchor(data->portal, true);
        content_data = ui_dropdown_menu_content_get_data(content);
        if (content_data) {
                content_data->menu = owner;
        }
        ui_widget_on(content, "afterlayout", ui_dropdown_menu_on_content_layout,
                     data);
        ui_widget_on(content, "destroy", ui_dropdown_menu_on_content_destroy,
                     data);
        ui_widget_on(content, "click", ui_dropdown_menu_on_content_click, data);
        ui_widget_on(content, "mouseover",
                     ui_dropdown_menu_on_content_mouseover, data);
        ui_widget_on(data->portal, "portal-outside",
                     ui_dropdown_menu_on_portal_outside, data);
        ui_widget_on(data->portal, "destroy",
                     ui_dropdown_menu_on_portal_destroy, data);
        if (!data->submenu) {
                ui_widget_on(trigger, "click",
                             ui_dropdown_menu_on_trigger_click, data);
        }
        ui_widget_on(trigger, "destroy", ui_dropdown_menu_on_trigger_destroy,
                     data);
        data->bound = true;
        if (data->pending_open) {
                data->pending_open = false;
                ui_dropdown_menu_open_data(data);
        }
}

static void ui_dropdown_menu_open_data(ui_dropdown_menu_t *data)
{
        if (!data || data->open) {
                return;
        }
        ui_dropdown_menu_reset_typeahead(data);
        if (!data->bound) {
                ui_dropdown_menu_bind_structure(data);
        }
        if (!data->portal || !data->content) {
                data->pending_open = true;
                return;
        }
        if (!data->submenu && ui_dropdown_active_menu &&
            ui_dropdown_active_menu != data) {
                ui_dropdown_menu_close_data(ui_dropdown_active_menu);
        }
        if (data->submenu && data->parent_menu) {
                ui_dropdown_menu_t *parent =
                    ui_dropdown_menu_get_data(data->parent_menu);

                if (parent) {
                        ui_dropdown_menu_close_subs(parent->content,
                                                    data->owner);
                }
        }
        ui_dropdown_menu_clear_last_focus(data);
        data->last_focus = ui_get_focus();
        if (data->last_focus) {
                ui_widget_on(data->last_focus, "destroy",
                             ui_dropdown_menu_on_last_focus_destroy, data);
        }
        data->open = true;
        data->pending_open = false;
        data->focus_pending = true;
        if (!data->submenu) {
                ui_dropdown_active_menu = data;
        }
        ui_dropdown_key_menu = data;
        ui_portal_set_anchor(data->portal, data->trigger);
        if (data->match_trigger_width) {
                ui_widget_set_style_unit_value(data->content, css_prop_width,
                                               data->trigger->border_box.width,
                                               CSS_UNIT_PX);
        }
        ui_dropdown_menu_apply_placement(data);
        ui_widget_refresh_children_style(data->content);
        ui_widget_set_style_keyword_value(data->content, css_prop_visibility,
                                          CSS_KEYWORD_HIDDEN);
        data->reveal_pending = true;
        data->settle_stage = 0;
        ui_dropdown_menu_sync_open_attr(data);
        ui_portal_open(data->portal);
        ui_dropdown_menu_emit(data->owner, "open", NULL);
}

static void ui_dropdown_menu_on_set_attr(ui_widget_t *w, const char *name,
                                         const char *value)
{
        ui_dropdown_menu_t *data = ui_dropdown_menu_get_data(w);
        bool open;

        if (!data) {
                return;
        }
        if (strcmp(name, "open") == 0) {
                open = ui_dropdown_menu_value_is_true(value);
                if (open == data->open) {
                        if (!open) {
                                data->pending_open = false;
                        }
                        return;
                }
                if (open) {
                        ui_dropdown_menu_open_data(data);
                } else {
                        ui_dropdown_menu_close_data(data);
                }
        } else if (strcmp(name, "default-open") == 0) {
                if (!ui_widget_get_attr(w, "open")) {
                        ui_dropdown_menu_open_data(data);
                }
        } else if (strcmp(name, "match-trigger-width") == 0) {
                data->match_trigger_width =
                    ui_dropdown_menu_value_is_true(value);
        }
}

static void ui_dropdown_menu_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_dropdown_menu_t *data = ui_dropdown_menu_get_data(w);

        if (task != UI_TASK_BEFORE_UPDATE || !data) {
                return;
        }
        data->owner = w;
        if (!data->bound) {
                ui_dropdown_menu_bind_structure(data);
        }
}

static void ui_dropdown_menu_on_destroy(ui_widget_t *w)
{
        ui_dropdown_menu_t *data = ui_dropdown_menu_get_data(w);

        if (!data) {
                return;
        }
        if (ui_dropdown_active_menu == data) {
                ui_dropdown_active_menu = NULL;
        }
        if (ui_dropdown_key_menu == data) {
                ui_dropdown_key_menu = NULL;
        }
        ui_dropdown_menu_clear_last_focus(data);
        data->owner = NULL;
        data->trigger = NULL;
        data->content = NULL;
        data->portal = NULL;
        data->open = false;
}

static void ui_dropdown_menu_on_init(ui_widget_t *w)
{
        ui_dropdown_menu_t *data;
        ui_widget_prototype_t *prototype;
        bool submenu;

        submenu = ui_check_widget_prototype(w, ui_dropdown_menu_sub_prototype);
        prototype = submenu ? ui_dropdown_menu_sub_prototype
                            : ui_dropdown_menu_prototype;
        data = ui_widget_add_data(w, prototype, sizeof(ui_dropdown_menu_t));
        memset(data, 0, sizeof(ui_dropdown_menu_t));
        data->owner = w;
        data->submenu = submenu;
        ui_dropdown_menu_reset_typeahead(data);
        ui_widget_add_status(w, "closed");
}

static void ui_dropdown_menu_content_on_init(ui_widget_t *w)
{
        bool submenu = ui_check_widget_prototype(
            w, ui_dropdown_menu_sub_content_prototype);
        ui_dropdown_menu_content_t *data =
            ui_widget_add_data(w,
                               submenu ? ui_dropdown_menu_sub_content_prototype
                                       : ui_dropdown_menu_content_prototype,
                               sizeof(ui_dropdown_menu_content_t));

        data->menu = NULL;
        data->side = submenu ? UI_DROPDOWN_MENU_SIDE_RIGHT
                             : UI_DROPDOWN_MENU_SIDE_BOTTOM;
        data->align = UI_DROPDOWN_MENU_ALIGN_START;
        data->side_offset = submenu ? 4 : 4;
        data->align_offset = 0;
}

static void ui_dropdown_menu_content_on_set_attr(ui_widget_t *w,
                                                 const char *name,
                                                 const char *value)
{
        ui_dropdown_menu_content_t *data = ui_dropdown_menu_content_get_data(w);

        if (!data) {
                return;
        }
        if (strcmp(name, "side") == 0) {
                data->side = ui_dropdown_menu_side_from_name(value);
        } else if (strcmp(name, "align") == 0) {
                data->align = ui_dropdown_menu_align_from_name(value);
        } else if (strcmp(name, "side-offset") == 0) {
                data->side_offset = value ? atoi(value) : 0;
        } else if (strcmp(name, "align-offset") == 0) {
                data->align_offset = value ? atoi(value) : 0;
        }
}

static void ui_dropdown_menu_item_on_init(ui_widget_t *w)
{
        w->tab_index = 0;
}

static void ui_dropdown_menu_item_on_set_attr(ui_widget_t *w, const char *name,
                                              const char *value)
{
        if (strcmp(name, "checked") == 0) {
                if (ui_dropdown_menu_value_is_true(value)) {
                        ui_widget_add_status(w, "checked");
                } else {
                        ui_widget_remove_status(w, "checked");
                }
        } else if (strcmp(name, "variant") == 0) {
                if (value && strcmp(value, "destructive") == 0) {
                        ui_widget_add_status(w, "destructive");
                } else {
                        ui_widget_remove_status(w, "destructive");
                }
        } else if (strcmp(name, "disabled") == 0) {
                ui_widget_set_disabled(w,
                                       ui_dropdown_menu_value_is_true(value));
        }
}

static void ui_dropdown_menu_radio_group_on_set_attr(ui_widget_t *w,
                                                     const char *name,
                                                     const char *value)
{
        if (strcmp(name, "value") == 0) {
                ui_dropdown_menu_sync_radio_group(w);
        }
}

static void ui_dropdown_menu_radio_group_on_update(ui_widget_t *w,
                                                   ui_task_type_t task)
{
        if (task == UI_TASK_BEFORE_UPDATE) {
                ui_dropdown_menu_sync_radio_group(w);
        }
}

static ui_widget_t *ui_dropdown_menu_find_indicator(ui_widget_t *w)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_widget_has_class(child, "dropdown-menu-indicator")) {
                        return child;
                }
        }
        return NULL;
}

static void ui_dropdown_menu_ensure_indicator(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *indicator;
        ui_widget_t *last = NULL;
        size_t i;

        indicator = ui_dropdown_menu_find_indicator(w);
        if (!indicator) {
                indicator = ui_create_fluent_icon("checkmark");
                ui_widget_add_class(indicator, "dropdown-menu-indicator");
                ui_widget_append(w, indicator);
                return;
        }
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                last = child;
        }
        if (last != indicator) {
                ui_widget_append(w, indicator);
        }
}

static void ui_dropdown_menu_item_on_update(ui_widget_t *w, ui_task_type_t task)
{
        if (task != UI_TASK_BEFORE_UPDATE) {
                return;
        }
        if (ui_widget_has_status(w, "disabled")) {
                ui_widget_set_disabled(w, true);
        }
        if (ui_check_widget_prototype(
                w, ui_dropdown_menu_checkbox_item_prototype) ||
            ui_check_widget_prototype(w,
                                      ui_dropdown_menu_radio_item_prototype)) {
                ui_dropdown_menu_ensure_indicator(w);
        }
}

static void ui_dropdown_menu_on_keydown(ui_widget_t *w, ui_event_t *e,
                                        void *arg)
{
        ui_dropdown_menu_t *data = ui_dropdown_key_menu;
        ui_dropdown_menu_items_t items;
        ui_widget_t *target = NULL;
        size_t i;
        bool found = false;

        if (!data || !data->open || !data->content) {
                return;
        }
        if (e->key.code == KEY_ESCAPE) {
                if (data->submenu) {
                        ui_widget_t *parent = data->parent_menu;

                        ui_dropdown_menu_close_data(data);
                        if (parent) {
                                ui_dropdown_menu_t *parent_data =
                                    ui_dropdown_menu_get_data(parent);
                                if (parent_data) {
                                        ui_dropdown_menu_highlight(
                                            parent_data,
                                            parent_data->highlighted);
                                }
                        }
                } else {
                        ui_dropdown_menu_close_data(data);
                }
                e->cancel_bubble = true;
                return;
        }
        if (e->key.code == KEY_LEFT) {
                if (data->submenu && data->parent_menu) {
                        ui_dropdown_menu_t *parent =
                            ui_dropdown_menu_get_data(data->parent_menu);
                        ui_widget_t *trigger = data->trigger;

                        ui_dropdown_menu_close_data(data);
                        if (parent) {
                                ui_dropdown_menu_highlight(parent, trigger);
                        }
                }
                e->cancel_bubble = true;
                return;
        }
        if (e->key.code == KEY_RIGHT) {
                ui_widget_t *sub_item = data->highlighted;

                if (!sub_item && data->content &&
                    ui_dropdown_menu_is_inside(data->content, ui_get_focus())) {
                        sub_item = ui_get_focus();
                }
                if (sub_item && ui_dropdown_menu_is_sub_trigger(sub_item) &&
                    sub_item->parent) {
                        ui_dropdown_menu_t *sub =
                            ui_dropdown_menu_get_data(sub_item->parent);

                        if (sub) {
                                ui_dropdown_menu_open_data(sub);
                                if (sub->content) {
                                        ui_dropdown_menu_highlight_first(sub);
                                }
                        }
                }
                e->cancel_bubble = true;
                return;
        }
        if (e->key.code == KEY_ENTER || e->key.code == KEY_SPACE) {
                if (e->key.code == KEY_SPACE && data->typeahead_len > 0) {
                        ui_dropdown_menu_typeahead(data, L' ');
                } else if (data->highlighted) {
                        ui_dropdown_menu_activate(data->highlighted);
                }
                e->cancel_bubble = true;
                return;
        }
        if (!e->key.ctrl_key && !e->key.alt_key && !e->key.meta_key &&
            ui_dropdown_menu_is_typeahead_key(e->key.code)) {
                ui_dropdown_menu_typeahead(
                    data, (wchar_t)towlower((wint_t)e->key.code));
                e->cancel_bubble = true;
                return;
        }
        if (e->key.code != KEY_DOWN && e->key.code != KEY_UP &&
            e->key.code != KEY_HOME && e->key.code != KEY_END) {
                return;
        }
        items = ui_dropdown_menu_get_items(data);
        if (items.count == 0) {
                ui_dropdown_menu_free_items(&items);
                return;
        }
        if (e->key.code == KEY_HOME) {
                target = items.items[0];
        } else if (e->key.code == KEY_END) {
                target = items.items[items.count - 1];
        } else {
                for (i = 0; i < items.count; ++i) {
                        if (items.items[i] == data->highlighted) {
                                found = true;
                                break;
                        }
                }
                if (e->key.code == KEY_DOWN) {
                        if (found) {
                                target = items.items[(i + 1) % items.count];
                        } else {
                                target = items.items[0];
                        }
                } else if (found) {
                        target =
                            items.items[(i + items.count - 1) % items.count];
                } else {
                        target = items.items[items.count - 1];
                }
        }
        ui_dropdown_menu_highlight(data, target);
        ui_dropdown_menu_free_items(&items);
        e->cancel_bubble = true;
}

void ui_dropdown_menu_set_open(ui_widget_t *menu, bool open)
{
        ui_widget_set_attr(menu, "open", open ? "true" : "false");
}

bool ui_dropdown_menu_is_open(ui_widget_t *menu)
{
        ui_dropdown_menu_t *data = ui_dropdown_menu_get_data(menu);

        return data ? data->open : false;
}

void ui_dropdown_menu_item_set_variant(ui_widget_t *item,
                                       ui_dropdown_menu_item_variant_t variant)
{
        ui_widget_set_attr(item, "variant",
                           variant == UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE
                               ? "destructive"
                               : "default");
}

ui_widget_t *ui_create_dropdown_menu(void)
{
        return ui_create_widget_with_prototype(ui_dropdown_menu_prototype);
}

ui_widget_t *ui_create_dropdown_menu_trigger(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_trigger_prototype);
}

ui_widget_t *ui_create_dropdown_menu_content(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_content_prototype);
}

ui_widget_t *ui_create_dropdown_menu_portal(void)
{
        ui_widget_t *portal = ui_create_portal();

        ui_portal_set_passthrough(portal, true);
        ui_portal_set_outside_close(portal, true);
        return portal;
}

ui_widget_t *ui_create_dropdown_menu_group(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_group_prototype);
}

ui_widget_t *ui_create_dropdown_menu_label(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_label_prototype);
}

ui_widget_t *ui_create_dropdown_menu_item(void)
{
        return ui_create_widget_with_prototype(ui_dropdown_menu_item_prototype);
}

ui_widget_t *ui_create_dropdown_menu_checkbox_item(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_checkbox_item_prototype);
}

ui_widget_t *ui_create_dropdown_menu_radio_group(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_radio_group_prototype);
}

ui_widget_t *ui_create_dropdown_menu_radio_item(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_radio_item_prototype);
}

ui_widget_t *ui_create_dropdown_menu_separator(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_separator_prototype);
}

ui_widget_t *ui_create_dropdown_menu_shortcut(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_shortcut_prototype);
}

ui_widget_t *ui_create_dropdown_menu_sub(void)
{
        return ui_create_widget_with_prototype(ui_dropdown_menu_sub_prototype);
}

ui_widget_t *ui_create_dropdown_menu_sub_trigger(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_sub_trigger_prototype);
}

ui_widget_t *ui_create_dropdown_menu_sub_content(void)
{
        return ui_create_widget_with_prototype(
            ui_dropdown_menu_sub_content_prototype);
}

void ui_register_dropdown_menu(void)
{
        ui_dropdown_menu_prototype =
            ui_create_widget_prototype("dropdown-menu", NULL);
        ui_dropdown_menu_trigger_prototype =
            ui_create_widget_prototype("dropdown-menu-trigger", NULL);
        ui_dropdown_menu_content_prototype =
            ui_create_widget_prototype("dropdown-menu-content", NULL);
        ui_dropdown_menu_group_prototype =
            ui_create_widget_prototype("dropdown-menu-group", NULL);
        ui_dropdown_menu_label_prototype =
            ui_create_widget_prototype("dropdown-menu-label", "text");
        ui_dropdown_menu_item_prototype =
            ui_create_widget_prototype("dropdown-menu-item", NULL);
        ui_dropdown_menu_checkbox_item_prototype =
            ui_create_widget_prototype("dropdown-menu-checkbox-item", NULL);
        ui_dropdown_menu_radio_group_prototype =
            ui_create_widget_prototype("dropdown-menu-radio-group", NULL);
        ui_dropdown_menu_radio_item_prototype =
            ui_create_widget_prototype("dropdown-menu-radio-item", NULL);
        ui_dropdown_menu_separator_prototype =
            ui_create_widget_prototype("dropdown-menu-separator", NULL);
        ui_dropdown_menu_shortcut_prototype =
            ui_create_widget_prototype("dropdown-menu-shortcut", "text");
        ui_dropdown_menu_sub_prototype =
            ui_create_widget_prototype("dropdown-menu-sub", NULL);
        ui_dropdown_menu_sub_trigger_prototype =
            ui_create_widget_prototype("dropdown-menu-sub-trigger", NULL);
        ui_dropdown_menu_sub_content_prototype =
            ui_create_widget_prototype("dropdown-menu-sub-content", NULL);

        ui_dropdown_menu_prototype->init = ui_dropdown_menu_on_init;
        ui_dropdown_menu_prototype->setattr = ui_dropdown_menu_on_set_attr;
        ui_dropdown_menu_prototype->update = ui_dropdown_menu_on_update;
        ui_dropdown_menu_prototype->destroy = ui_dropdown_menu_on_destroy;
        ui_dropdown_menu_sub_prototype->init = ui_dropdown_menu_on_init;
        ui_dropdown_menu_sub_prototype->setattr = ui_dropdown_menu_on_set_attr;
        ui_dropdown_menu_sub_prototype->update = ui_dropdown_menu_on_update;
        ui_dropdown_menu_sub_prototype->destroy = ui_dropdown_menu_on_destroy;
        ui_dropdown_menu_content_prototype->init =
            ui_dropdown_menu_content_on_init;
        ui_dropdown_menu_content_prototype->setattr =
            ui_dropdown_menu_content_on_set_attr;
        ui_dropdown_menu_sub_content_prototype->init =
            ui_dropdown_menu_content_on_init;
        ui_dropdown_menu_sub_content_prototype->setattr =
            ui_dropdown_menu_content_on_set_attr;
        ui_dropdown_menu_item_prototype->init = ui_dropdown_menu_item_on_init;
        ui_dropdown_menu_item_prototype->setattr =
            ui_dropdown_menu_item_on_set_attr;
        ui_dropdown_menu_item_prototype->update =
            ui_dropdown_menu_item_on_update;
        ui_dropdown_menu_checkbox_item_prototype->init =
            ui_dropdown_menu_item_on_init;
        ui_dropdown_menu_checkbox_item_prototype->setattr =
            ui_dropdown_menu_item_on_set_attr;
        ui_dropdown_menu_checkbox_item_prototype->update =
            ui_dropdown_menu_item_on_update;
        ui_dropdown_menu_radio_item_prototype->init =
            ui_dropdown_menu_item_on_init;
        ui_dropdown_menu_radio_item_prototype->setattr =
            ui_dropdown_menu_item_on_set_attr;
        ui_dropdown_menu_radio_item_prototype->update =
            ui_dropdown_menu_item_on_update;
        ui_dropdown_menu_sub_trigger_prototype->init =
            ui_dropdown_menu_item_on_init;
        ui_dropdown_menu_sub_trigger_prototype->setattr =
            ui_dropdown_menu_item_on_set_attr;
        ui_dropdown_menu_sub_trigger_prototype->update =
            ui_dropdown_menu_item_on_update;
        ui_dropdown_menu_radio_group_prototype->setattr =
            ui_dropdown_menu_radio_group_on_set_attr;
        ui_dropdown_menu_radio_group_prototype->update =
            ui_dropdown_menu_radio_group_on_update;
        ui_widget_on(ui_root(), "keydown", ui_dropdown_menu_on_keydown, NULL);
}

void ui_unregister_dropdown_menu(void)
{
        if (ui_root()) {
                ui_widget_off(ui_root(), "keydown", ui_dropdown_menu_on_keydown,
                              NULL);
        }
        ui_dropdown_active_menu = NULL;
        ui_dropdown_key_menu = NULL;
}
