/*
 * src/widgets/select.c: -- Select widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <yutil.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/icon.h>
#include <LCUI/widgets/scrollarea.h>
#include <LCUI/widgets/scrollbar.h>
#include <LCUI/widgets/select.h>
#include <LCUI/widgets/text.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>
#include <css.h>

typedef struct ui_select_registry_entry {
        char *key;
        ui_select_t *select;
        list_node_t node;
} ui_select_registry_entry_t;

struct ui_select {
        ui_widget_t *owner;
        ui_widget_t *trigger;
        ui_widget_t *value_widget;
        ui_widget_t *content;
        ui_widget_t *list;
        ui_widget_t *portal;
        ui_widget_t *anchor;
        char *placeholder;
        char *value;
        bool open;
        bool loose;
        bool align_item;
        bool settling;
        bool side_top;
        int settle_stage;
        int settle_pass;
        list_node_t node;
};

static ui_widget_prototype_t *ui_select_prototype;
static ui_widget_prototype_t *ui_select_trigger_prototype;
static ui_widget_prototype_t *ui_select_value_prototype;
static ui_widget_prototype_t *ui_select_arrow_prototype;
static ui_widget_prototype_t *ui_select_content_prototype;
static ui_widget_prototype_t *ui_select_group_prototype;
static ui_widget_prototype_t *ui_select_label_prototype;
static ui_widget_prototype_t *ui_select_separator_prototype;
static ui_widget_prototype_t *ui_select_item_prototype;
static list_t ui_select_registry;

static void ui_select_on_destroy_event(ui_widget_t *w, ui_event_t *e,
                                       void *unused);

/* Structural rules only (display, positioning, flex, sizing). visual
 * refinements live in src/styles/style-nova.css so the shadcn-compatible
 * look stays in one place. */
static const char *ui_select_css = "\
select {\
  display: inline-block;\
}\
select-trigger {\
  display: inline-block;\
}\
select-trigger button {\
  display: flex;\
  align-items: center;\
  box-sizing: border-box;\
  min-width: 176px;\
  width: 100%;\
}\
select-value {\
  display: inline-block;\
  text-align: left;\
  flex: 1;\
}\
select-arrow {\
  display: inline-block;\
}\
select-content {\
  display: none;\
  position: absolute;\
  box-sizing: border-box;\
  min-width: 144px;\
}\
select-group {\
  display: block;\
}\
select-label {\
  display: block;\
}\
select-separator {\
  display: block;\
}\
select-item {\
  display: block;\
  position: relative;\
}\
";

static void ui_select_on_portal_destroy(ui_widget_t *w, ui_event_t *e,
                                        void *unused)
{
        ui_select_t *data = e->data;

        data->portal = NULL;
}

static void ui_select_on_portal_outside(ui_widget_t *w, ui_event_t *e,
                                        void *unused)
{
        ui_select_t *data = e->data;

        data->open = false;
}

static void ui_select_data_destroy(ui_select_t *data)
{
        if (data->portal) {
                ui_widget_off(data->portal, "destroy",
                              ui_select_on_portal_destroy, data);
                ui_widget_off(data->portal, "portal-outside",
                              ui_select_on_portal_outside, data);
                data->portal = NULL;
        }
        free(data->placeholder);
        free(data->value);
}

static void ui_select_destroy_registry_entry(void *arg)
{
        ui_select_registry_entry_t *entry = arg;

        ui_select_data_destroy(entry->select);
        free(entry->select);
        free(entry->key);
        free(entry);
}

ui_select_t *ui_select_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_select_prototype);
}

static bool ui_select_is_item(ui_widget_t *w)
{
        return w && (w->proto == ui_select_item_prototype ||
                     ui_widget_get_attr(w, "data-select-item") != NULL);
}

static ui_widget_t *ui_select_find_owner(ui_widget_t *w)
{
        while (w) {
                if (w->proto == ui_select_prototype) {
                        return w;
                }
                w = w->parent;
        }
        return NULL;
}

static ui_widget_t *ui_select_find_descendant(ui_widget_t *w,
                                              ui_widget_prototype_t *proto)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (child->proto == proto) {
                        return child;
                }
                found = ui_select_find_descendant(child, proto);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_select_find_selected_item(ui_widget_t *content,
                                                 const char *value)
{
        ui_widget_t *child;
        ui_widget_t *found;
        const char *item_value;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (ui_select_is_item(child)) {
                        item_value = ui_widget_get_attr(child, "value");
                        if (item_value && strcmp(item_value, value) == 0) {
                                return child;
                        }
                }
                found = ui_select_find_selected_item(child, value);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_select_find_value_node(ui_widget_t *trigger)
{
        return ui_select_find_descendant(trigger, ui_select_value_prototype);
}

static ui_widget_t *ui_select_find_first_item(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_select_is_item(child)) {
                        return child;
                }
                found = ui_select_find_first_item(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

/* The item to align with the trigger: the selected one, or the first item
 * when nothing is selected. */
static ui_widget_t *ui_select_find_highlight_item(ui_select_t *data,
                                                  ui_widget_t *content)
{
        ui_widget_t *item;

        item = data->value ? ui_select_find_selected_item(content, data->value)
                           : NULL;
        return item ? item : ui_select_find_first_item(content);
}

static ui_widget_t *ui_select_find_align_item(ui_select_t *data,
                                              ui_widget_t *content)
{
        if (!data->align_item) {
                return NULL;
        }
        return ui_select_find_highlight_item(data, content);
}

static void ui_select_clear_highlight_each(ui_widget_t *w, void *arg)
{
        if (ui_select_is_item(w)) {
                ui_widget_remove_status(w, "highlighted");
        }
}

static void ui_select_set_highlight(ui_widget_t *content, ui_widget_t *item)
{
        ui_widget_each(content, ui_select_clear_highlight_each, NULL);
        if (item) {
                ui_widget_add_status(item, "highlighted");
        }
}

static ui_widget_t *ui_select_content_find_type(ui_widget_t *content,
                                                const char *type)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (ui_check_widget_type(child, type)) {
                        return child;
                }
        }
        return NULL;
}

/* Move any option or group appended directly to <select-content> into the
 * internal <scrollarea-content> list, so the declarative model
 * (SelectContent > SelectGroup / SelectItem) keeps working even though the
 * options actually live inside the scrollable list. */
static void ui_select_content_absorb(ui_widget_t *content, ui_widget_t *list)
{
        ui_widget_t *child;
        size_t i = 0;

        while ((child = ui_widget_get_child(content, i)) != NULL) {
                if (child == list ||
                    ui_check_widget_type(child, "scrollarea-content") ||
                    ui_check_widget_type(child, "scrollbar")) {
                        ++i;
                        continue;
                }
                ui_widget_append(list, child);
        }
}

static ui_widget_t *ui_select_content_ensure_list(ui_widget_t *content)
{
        ui_widget_t *list;

        list = ui_select_content_find_type(content, "scrollarea-content");
        if (!list) {
                list = ui_create_scrollarea_content();
                ui_widget_append(content, list);
        }
        if (!ui_select_content_find_type(content, "scrollbar")) {
                ui_widget_append(content, ui_create_scrollbar());
        }
        ui_select_content_absorb(content, list);
        return list;
}

static void ui_select_content_prepare(ui_widget_t *content)
{
        ui_select_content_ensure_list(content);
        ui_scrollarea_set_scroll_top(content, 0);
}

/* Available space between the anchor and the root edge, used for both the
 * flip side and the popup max-height. */
static float ui_select_available_height(ui_widget_t *anchor, bool above)
{
        ui_widget_t *root = ui_root();
        float x, y;
        const float margin = 8.0f;

        ui_widget_get_offset(anchor, root, &x, &y);
        if (above) {
                return y - margin;
        }
        return root->border_box.height - (y + anchor->border_box.height) -
               margin;
}

static ui_select_registry_entry_t *ui_select_find_registry(const char *key)
{
        list_node_t *node;

        if (!key) {
                return NULL;
        }
        for (list_each(node, &ui_select_registry)) {
                ui_select_registry_entry_t *entry = node->data;

                if (strcmp(entry->key, key) == 0) {
                        return entry;
                }
        }
        return NULL;
}

static ui_select_t *ui_select_get_loose(const char *key)
{
        ui_select_registry_entry_t *entry;

        if (!key) {
                return NULL;
        }
        entry = ui_select_find_registry(key);
        if (entry) {
                return entry->select;
        }
        entry = calloc(1, sizeof(*entry));
        entry->key = y_strdup(key);
        entry->select = calloc(1, sizeof(*entry->select));
        entry->select->loose = true;
        entry->select->align_item = true;
        entry->select->owner = ui_root();
        entry->node.data = entry;
        list_append_node(&ui_select_registry, &entry->node);
        return entry->select;
}

static ui_select_t *ui_select_resolve(ui_widget_t *w, const char *attr)
{
        ui_widget_t *owner = ui_select_find_owner(w);
        const char *key;

        if (owner) {
                return ui_select_get_data(owner);
        }
        key = ui_widget_get_attr(w, attr);
        return ui_select_get_loose(key);
}

static void ui_select_set_item_checked(ui_widget_t *item, bool checked)
{
        ui_widget_set_attr(item, "checked", checked ? "true" : "false");
        if (checked) {
                ui_widget_add_status(item, "checked");
        } else {
                ui_widget_remove_status(item, "checked");
        }
}

static void ui_select_sync_item_each(ui_widget_t *w, void *arg)
{
        ui_select_t *data = arg;
        const char *item_value;

        if (!ui_select_is_item(w)) {
                return;
        }
        item_value = ui_widget_get_attr(w, "value");
        ui_select_set_item_checked(w, data->value && item_value &&
                                          strcmp(data->value, item_value) == 0);
}

static void ui_select_set_text(ui_widget_t *w, const char *text)
{
        size_t len;
        wchar_t *buffer;

        if (!w) {
                return;
        }
        text = text ? text : "";
        len = decode_utf8(NULL, text, 0);
        buffer = calloc(len + 1, sizeof(*buffer));
        decode_utf8(buffer, text, len + 1);
        ui_text_set_content_w(w, buffer);
        free(buffer);
}

static const char *ui_select_item_label(ui_widget_t *item, char *buffer,
                                        size_t size)
{
        const char *title;
        const char *value;
        wchar_t text[256];

        if (item->proto == ui_select_item_prototype &&
            ui_text_get_content_w(item, text, sizeof(text) / sizeof(*text))) {
                encode_utf8(buffer, text, size);
                return buffer;
        }
        title = ui_widget_get_attr(item, "title");
        if (title) {
                return title;
        }
        value = ui_widget_get_attr(item, "value");
        return value ? value : "";
}

static void ui_select_set_value_text(ui_widget_t *value, const char *text)
{
        if (!value) {
                return;
        }
        if (value->proto == ui_select_value_prototype) {
                ui_select_set_text(value, text);
        } else {
                ui_widget_set_text(value, text ? text : "");
        }
}

static void ui_select_sync_values(ui_select_t *data)
{
        char label[1024];
        ui_widget_t *selected = NULL;
        ui_widget_t *value;

        if (data->trigger) {
                value = ui_select_find_value_node(data->trigger);
                value = value ? value : data->value_widget;
                ui_select_set_value_text(
                    value, data->value ? data->value : data->placeholder);
        }
        if (data->content) {
                data->list = ui_select_content_ensure_list(data->content);
                ui_widget_each(data->content, ui_select_sync_item_each, data);
                if (data->value) {
                        selected = ui_select_find_selected_item(data->content,
                                                                data->value);
                        if (selected) {
                                ui_select_set_value_text(
                                    data->trigger
                                        ? (ui_select_find_value_node(
                                               data->trigger)
                                               ? ui_select_find_value_node(
                                                     data->trigger)
                                               : data->value_widget)
                                        : data->value_widget,
                                    ui_select_item_label(selected, label,
                                                         sizeof(label)));
                        }
                }
        }
}

static void ui_select_content_reveal(ui_select_t *data, ui_widget_t *content)
{
        ui_widget_set_style_keyword_value(content, css_prop_visibility,
                                          CSS_KEYWORD_VISIBLE);
        data->settling = false;
}

/* Settle the popup before revealing it: stage 0 picks the flip side and
 * max-height, stage 1 applies the alignment scroll and panel offset, stage 2
 * reveals once the layout is stable. Keeping the content invisible during
 * these passes avoids a visible re-position jitter. */
static void ui_select_on_content_layout(ui_widget_t *w, ui_event_t *e,
                                        void *arg)
{
        ui_select_t *data = e->data;
        ui_widget_t *root = ui_root();
        ui_widget_t *anchor = data->anchor;
        ui_widget_t *item;
        float below, above, natural, chrome, limit, panel_h, panel_top, scroll;
        float ax, ay, ix, iy, item_h, anchor_h, root_h, desired_top;
        bool clamped;
        const float margin = 8.0f;

        if (!data->settling || !data->open || !anchor || !data->portal) {
                return;
        }
        data->settle_pass++;
        if (data->settle_pass > 5) {
                ui_select_content_reveal(data, w);
                return;
        }
        if (data->settle_stage == 0) {
                chrome = w->border_box.height - w->content_box.height;
                natural = ui_scrollarea_get_scroll_height(w) + chrome;
                root_h = root->border_box.height;
                below = ui_select_available_height(anchor, false);
                above = ui_select_available_height(anchor, true);
                item = ui_select_find_align_item(data, w);
                if (natural > below && above > below) {
                        /* Near the bottom edge: flip above and use that
                         * side's available height instead of aligning. */
                        item = NULL;
                        data->side_top = true;
                        limit = above;
                } else if (item) {
                        /* Align mode: overlap the trigger, using the whole
                         * viewport minus margins as the height cap. */
                        data->side_top = false;
                        limit = root_h - 2 * margin;
                } else {
                        data->side_top = false;
                        limit = below;
                }
                if (limit < 0) {
                        limit = 0;
                }
                ui_widget_set_style_unit_value(w, css_prop_max_height, limit,
                                               CSS_UNIT_PX);
                ui_portal_set_side(data->portal,
                                   data->side_top ? "top" : "bottom");
                ui_select_set_highlight(w,
                                        ui_select_find_highlight_item(data, w));
                data->settle_stage = 1;
                ui_portal_open(data->portal);
                ui_widget_request_update(w);
                return;
        }
        if (data->settle_stage == 1) {
                if (data->align_item && !data->side_top) {
                        item = ui_select_find_align_item(data, w);
                        if (item) {
                                ui_widget_get_offset(item, w, &ix, &iy);
                                item_h = item->border_box.height;
                                ui_widget_get_offset(anchor, root, &ax, &ay);
                                anchor_h = anchor->border_box.height;
                                root_h = root->border_box.height;
                                panel_h = w->border_box.height;
                                /* Panel top that puts the selected item over
                                 * the trigger. */
                                desired_top =
                                    ay + anchor_h / 2 - iy - item_h / 2;
                                clamped = false;
                                panel_top = desired_top;
                                if (panel_top < margin) {
                                        panel_top = margin;
                                        clamped = true;
                                }
                                if (panel_top + panel_h + margin > root_h) {
                                        panel_top = root_h - panel_h - margin;
                                        clamped = true;
                                }
                                if (panel_top < margin) {
                                        panel_top = margin;
                                        clamped = true;
                                }
                                if (clamped) {
                                        /* Could not overlap as asked; scroll
                                         * the item into the viewport instead.
                                         */
                                        scroll = iy - (w->content_box.height -
                                                       item_h) /
                                                          2;
                                        ui_scrollarea_set_scroll_top(w, scroll);
                                }
                                ui_portal_set_side_offset(
                                    data->portal,
                                    (int)(panel_top - ay - anchor_h));
                        }
                }
                data->settle_stage = 2;
                ui_portal_open(data->portal);
                ui_widget_request_update(w);
                return;
        }
        ui_select_content_reveal(data, w);
}

static void ui_select_open_data(ui_select_t *data, ui_widget_t *trigger)
{
        ui_widget_t *anchor;
        float anchor_width;
        float below;

        if (!data->portal) {
                data->portal = ui_create_portal();
                ui_widget_on(data->portal, "destroy",
                             ui_select_on_portal_destroy, data);
                ui_widget_on(data->portal, "portal-outside",
                             ui_select_on_portal_outside, data);
                ui_widget_append(data->owner, data->portal);
                data->content =
                    data->content ? data->content : ui_create_select_content();
                ui_portal_set_content(data->portal, data->content);
        }
        anchor = trigger ? trigger : ui_widget_get_child(data->trigger, 0);
        anchor = anchor ? anchor : data->trigger;
        data->anchor = anchor;
        data->list = ui_select_content_ensure_list(data->content);
        ui_select_content_prepare(data->content);
        ui_portal_set_anchor(data->portal, anchor);
        ui_portal_set_side(data->portal, "bottom");
        ui_portal_set_align(data->portal, "start");
        ui_portal_set_side_offset(data->portal, 0);
        ui_widget_set_style_keyword_value(data->content, css_prop_visibility,
                                          CSS_KEYWORD_HIDDEN);
        data->open = true;
        data->settling = true;
        data->settle_stage = 0;
        data->settle_pass = 0;
        data->side_top = false;
        anchor_width = anchor->border_box.width;
        ui_widget_set_style_unit_value(data->content, css_prop_width,
                                       anchor_width, CSS_UNIT_PX);
        below = ui_select_available_height(anchor, false);
        ui_widget_set_style_unit_value(data->content, css_prop_max_height,
                                       below < 0 ? 0 : below, CSS_UNIT_PX);
        ui_portal_open(data->portal);
        ui_widget_request_update(data->content);
}

static void ui_select_close_data(ui_select_t *data)
{
        data->open = false;
        data->settling = false;
        data->settle_stage = 0;
        if (data->content) {
                ui_widget_each(data->content, ui_select_clear_highlight_each,
                               NULL);
        }
        if (data->portal) {
                ui_portal_close(data->portal);
        }
}

static void ui_select_on_click(ui_widget_t *w, ui_event_t *e, void *unused)
{
        ui_select_t *data = e->data;
        ui_widget_t *target = e->target;
        ui_widget_t *item = target;
        const char *value;

        while (item && item != data->content) {
                if (ui_select_is_item(item)) {
                        break;
                }
                item = item->parent;
        }
        if (item && item != data->content) {
                value = ui_widget_get_attr(item, "value");
                if (!value || item->disabled) {
                        return;
                }
                free(data->value);
                data->value = y_strdup(value);
                if (data->owner) {
                        ui_widget_set_attr(data->owner, "value", value);
                }
                ui_select_sync_values(data);
                ui_select_close_data(data);
        }
}

static void ui_select_on_trigger_click(ui_widget_t *w, ui_event_t *e,
                                       void *unused)
{
        ui_select_t *data = e->data;

        if (data->owner && data->owner->disabled) {
                return;
        }
        if (data->open) {
                ui_select_close_data(data);
        } else {
                data->trigger = data->trigger ? data->trigger : w;
                ui_select_open_data(data, w);
        }
}

static void ui_select_bind_trigger(ui_widget_t *trigger)
{
        ui_select_t *data = ui_select_resolve(trigger, "data-select-trigger");
        ui_widget_t *button;
        const char *align_attr;

        if (!data) {
                return;
        }
        data->trigger = trigger;
        align_attr = ui_widget_get_attr(trigger, "align-item-with-trigger");
        if (align_attr) {
                data->align_item = strcmp(align_attr, "false") != 0;
        }
        button = trigger->proto == ui_select_trigger_prototype
                     ? ui_widget_get_child(trigger, 0)
                     : trigger;
        ui_widget_off(button, "click", ui_select_on_trigger_click, data);
        ui_widget_on(button, "click", ui_select_on_trigger_click, data);
}

static void ui_select_bind_value(ui_widget_t *value)
{
        ui_select_t *data = ui_select_resolve(value, "data-select-value");

        if (!data) {
                return;
        }
        data->value_widget = value;
        if (!data->placeholder) {
                const char *placeholder =
                    ui_widget_get_attr(value, "data-select-placeholder");
                data->placeholder = placeholder ? y_strdup(placeholder) : NULL;
        }
        ui_select_sync_values(data);
}

static void ui_select_bind_plain_widgets(ui_widget_t *w)
{
        ui_widget_t *child;
        size_t i;

        if (ui_widget_get_attr(w, "data-select-trigger")) {
                ui_select_bind_trigger(w);
        }
        if (ui_widget_get_attr(w, "data-select-value")) {
                ui_select_bind_value(w);
        }
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                ui_select_bind_plain_widgets(child);
        }
}

static void ui_select_bind_content(ui_widget_t *content)
{
        ui_select_t *data = ui_select_resolve(content, "data-select-target");

        if (!data) {
                return;
        }
        data->content = content;
        data->list = ui_select_content_ensure_list(content);
        ui_select_bind_plain_widgets(ui_root());
        ui_widget_off(content, "click", ui_select_on_click, data);
        ui_widget_on(content, "click", ui_select_on_click, data);
        ui_widget_off(content, "afterlayout", ui_select_on_content_layout,
                      data);
        ui_widget_on(content, "afterlayout", ui_select_on_content_layout, data);
}

static void ui_select_on_trigger_link(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_select_bind_trigger(w);
}

static void ui_select_on_content_link(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_select_bind_content(w);
}

static void ui_select_on_value_link(ui_widget_t *w, ui_event_t *e, void *unused)
{
        ui_select_bind_value(w);
}

static void ui_select_on_set_attr(ui_widget_t *w, const char *name,
                                  const char *value)
{
        ui_select_t *data = ui_select_get_data(w);

        if (strcmp(name, "align-item-with-trigger") == 0) {
                data->align_item = !(value && strcmp(value, "false") == 0);
        } else if (strcmp(name, "value") == 0) {
                free(data->value);
                data->value = value ? y_strdup(value) : NULL;
                ui_select_sync_values(data);
        } else if (strcmp(name, "placeholder") == 0) {
                free(data->placeholder);
                data->placeholder = value ? y_strdup(value) : NULL;
                ui_select_sync_values(data);
        }
}

static void ui_select_on_init(ui_widget_t *w)
{
        ui_select_t *data =
            ui_widget_add_data(w, ui_select_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        data->owner = w;
        data->align_item = true;
        ui_widget_on(w, "click", ui_select_on_click, data);
        ui_widget_on(w, "destroy", ui_select_on_destroy_event, data);
}

static void ui_select_on_destroy(ui_widget_t *w)
{
        ui_select_t *data = ui_select_get_data(w);
        ui_select_data_destroy(data);
}

static void ui_select_on_destroy_event(ui_widget_t *w, ui_event_t *e,
                                       void *unused)
{
        ui_select_t *data = e->data;

        data->portal = NULL;
}

static void ui_select_trigger_on_init(ui_widget_t *w)
{
        ui_widget_t *button = ui_create_widget("button");
        ui_widget_t *value = ui_create_select_value();
        ui_widget_t *arrow = ui_create_select_arrow();

        ui_widget_append(button, value);
        ui_widget_append(button, arrow);
        ui_widget_append(w, button);
        ui_widget_on(w, "link", ui_select_on_trigger_link, NULL);
        ui_select_bind_trigger(w);
}

static void ui_select_value_on_init(ui_widget_t *w)
{
        w->proto->proto->init(w);
        ui_widget_on(w, "link", ui_select_on_value_link, NULL);
}

static void ui_select_arrow_on_init(ui_widget_t *w)
{
        w->proto->proto->init(w);
        ui_icon_set_name(w, "chevron-down");
}

static void ui_select_item_on_init(ui_widget_t *w)
{
        ui_widget_t *indicator;

        w->proto->proto->init(w);
        indicator = ui_create_icon();
        ui_icon_set_name(indicator, "checkmark");
        ui_widget_add_class(indicator, "select-item-indicator");
        ui_widget_append(w, indicator);
}

static void ui_select_content_on_init(ui_widget_t *w)
{
        w->proto->proto->init(w);
        ui_select_content_ensure_list(w);
        ui_widget_on(w, "link", ui_select_on_content_link, NULL);
        ui_select_bind_content(w);
}

void ui_select_set_value(ui_widget_t *select, const char *value)
{
        ui_select_t *data = ui_select_get_data(select);

        free(data->value);
        data->value = value ? y_strdup(value) : NULL;
        ui_widget_set_attr(select, "value", value);
        ui_select_sync_values(data);
}

const char *ui_select_get_value(ui_widget_t *select)
{
        return ui_widget_get_attr(select, "value");
}

void ui_select_set_placeholder(ui_widget_t *select, const char *placeholder)
{
        ui_select_t *data = ui_select_get_data(select);

        free(data->placeholder);
        data->placeholder = placeholder ? y_strdup(placeholder) : NULL;
        ui_select_sync_values(data);
}

const char *ui_select_get_placeholder(ui_widget_t *select)
{
        return ui_select_get_data(select)->placeholder;
}

void ui_select_set_align_item_with_trigger(ui_widget_t *select, bool enabled)
{
        ui_select_t *data = ui_select_get_data(select);

        data->align_item = enabled;
        ui_widget_set_attr(select, "align-item-with-trigger",
                           enabled ? "true" : "false");
}

bool ui_select_get_align_item_with_trigger(ui_widget_t *select)
{
        return ui_select_get_data(select)->align_item;
}

void ui_select_open(ui_widget_t *select)
{
        ui_select_t *data = ui_select_get_data(select);

        ui_select_open_data(data, ui_widget_get_child(data->trigger, 0));
}

void ui_select_close(ui_widget_t *select)
{
        ui_select_close_data(ui_select_get_data(select));
}

void ui_select_build_children(ui_widget_t *select)
{
        ui_select_t *data = ui_select_get_data(select);
        ui_widget_t *trigger;
        ui_widget_t *content;

        trigger =
            ui_select_find_descendant(select, ui_select_trigger_prototype);
        if (!trigger) {
                trigger = ui_create_select_trigger();
                ui_widget_append(select, trigger);
        }
        data->trigger = trigger;
        content = data->content;
        if (!content) {
                content = ui_select_find_descendant(
                    select, ui_select_content_prototype);
        }
        if (!content) {
                content = ui_create_select_content();
                ui_widget_append(select, content);
        } else if (!content->parent) {
                ui_widget_append(select, content);
        }
        data->content = content;
        data->list = ui_select_content_ensure_list(content);
        ui_select_bind_trigger(trigger);
        ui_select_bind_content(content);
        ui_select_sync_values(data);
}

int ui_select_append_item(ui_widget_t *select, ui_widget_t *item)
{
        ui_select_t *data = ui_select_get_data(select);

        ui_select_build_children(select);
        data->list = ui_select_content_ensure_list(data->content);
        return ui_widget_append(data->list, item);
}

ui_widget_t *ui_select_add_item(ui_widget_t *select, const char *label,
                                const char *value)
{
        ui_widget_t *item = ui_create_select_item(value);

        ui_text_set_content(item, label);
        ui_select_append_item(select, item);
        return item;
}

void ui_select_clear_items(ui_widget_t *select)
{
        ui_select_t *data = ui_select_get_data(select);

        ui_select_build_children(select);
        data->list = ui_select_content_ensure_list(data->content);
        while (ui_widget_get_child(data->list, 0)) {
                ui_widget_remove(ui_widget_get_child(data->list, 0));
        }
}

ui_widget_t *ui_create_select(void)
{
        return ui_create_widget_with_prototype(ui_select_prototype);
}

ui_widget_t *ui_create_select_trigger(void)
{
        return ui_create_widget_with_prototype(ui_select_trigger_prototype);
}

ui_widget_t *ui_create_select_value(void)
{
        return ui_create_widget_with_prototype(ui_select_value_prototype);
}

ui_widget_t *ui_create_select_arrow(void)
{
        return ui_create_widget_with_prototype(ui_select_arrow_prototype);
}

ui_widget_t *ui_create_select_content(void)
{
        return ui_create_widget_with_prototype(ui_select_content_prototype);
}

ui_widget_t *ui_create_select_group(void)
{
        return ui_create_widget_with_prototype(ui_select_group_prototype);
}

ui_widget_t *ui_create_select_label(void)
{
        return ui_create_widget_with_prototype(ui_select_label_prototype);
}

ui_widget_t *ui_create_select_separator(void)
{
        return ui_create_widget_with_prototype(ui_select_separator_prototype);
}

ui_widget_t *ui_create_select_item(const char *value)
{
        ui_widget_t *item =
            ui_create_widget_with_prototype(ui_select_item_prototype);

        if (value) {
                ui_widget_set_attr(item, "value", value);
        }
        return item;
}

void ui_register_select(void)
{
        list_create(&ui_select_registry);
        ui_select_prototype = ui_create_widget_prototype("select", NULL);
        ui_select_prototype->init = ui_select_on_init;
        ui_select_prototype->destroy = ui_select_on_destroy;
        ui_select_prototype->setattr = ui_select_on_set_attr;
        ui_select_trigger_prototype =
            ui_create_widget_prototype("select-trigger", NULL);
        ui_select_trigger_prototype->init = ui_select_trigger_on_init;
        ui_select_value_prototype =
            ui_create_widget_prototype("select-value", "text");
        ui_select_value_prototype->init = ui_select_value_on_init;
        ui_select_arrow_prototype =
            ui_create_widget_prototype("select-arrow", "icon");
        ui_select_arrow_prototype->init = ui_select_arrow_on_init;
        ui_select_content_prototype =
            ui_create_widget_prototype("select-content", "scrollarea");
        ui_select_content_prototype->init = ui_select_content_on_init;
        ui_select_group_prototype =
            ui_create_widget_prototype("select-group", NULL);
        ui_select_label_prototype =
            ui_create_widget_prototype("select-label", "text");
        ui_select_separator_prototype =
            ui_create_widget_prototype("select-separator", NULL);
        ui_select_item_prototype =
            ui_create_widget_prototype("select-item", "text");
        ui_select_item_prototype->init = ui_select_item_on_init;
        ui_load_css_string(ui_select_css, __FILE__);
}

void ui_unregister_select(void)
{
        list_destroy_without_node(&ui_select_registry,
                                  ui_select_destroy_registry_entry);
}
