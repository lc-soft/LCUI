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
#include <LCUI/widgets/select.h>
#include <LCUI/widgets/text.h>
#include <ui/events.h>
#include <ui/prototype.h>
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
        ui_widget_t *portal;
        char *placeholder;
        char *value;
        bool open;
        bool loose;
        list_node_t node;
};

static ui_widget_prototype_t *ui_select_prototype;
static ui_widget_prototype_t *ui_select_trigger_prototype;
static ui_widget_prototype_t *ui_select_value_prototype;
static ui_widget_prototype_t *ui_select_arrow_prototype;
static ui_widget_prototype_t *ui_select_content_prototype;
static ui_widget_prototype_t *ui_select_item_prototype;
static list_t ui_select_registry;

static void ui_select_on_destroy_event(ui_widget_t *w, ui_event_t *e,
                                       void *unused);

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
  min-width: 176px;\
}\
select-value {\
  display: inline-block;\
  flex: 1;\
}\
select-arrow {\
  display: inline-block;\
  margin-left: 8px;\
}\
select-content {\
  display: none;\
  position: absolute;\
  box-sizing: border-box;\
  min-width: 120px;\
  padding: 4px 0;\
  background-color: #fff;\
  border: 1px solid #d9d9d9;\
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);\
}\
select-item {\
  display: block;\
  padding: 6px 12px;\
}\
select-item:hover {\
  background-color: #f5f5f5;\
}\
select-item:active {\
  background-color: #e8e8e8;\
}\
select-item:checked {\
  background-color: #e8f0fe;\
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

static void ui_select_open_data(ui_select_t *data, ui_widget_t *trigger)
{
        ui_widget_t *anchor;
        float anchor_width;

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
        ui_portal_set_anchor(data->portal, anchor ? anchor : data->trigger);
        ui_portal_set_side(data->portal, "bottom");
        ui_portal_set_align(data->portal, "start");
        data->open = true;
        ui_portal_open(data->portal);
        anchor_width =
            anchor ? anchor->border_box.width : data->trigger->border_box.width;
        ui_widget_set_style_unit_value(data->content, css_prop_width,
                                       anchor_width, CSS_UNIT_PX);
        ui_widget_request_update(data->portal);
}

static void ui_select_close_data(ui_select_t *data)
{
        data->open = false;
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

        if (!data) {
                return;
        }
        data->trigger = trigger;
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
        ui_select_bind_plain_widgets(ui_root());
        ui_widget_off(content, "click", ui_select_on_click, data);
        ui_widget_on(content, "click", ui_select_on_click, data);
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

static void ui_select_on_init(ui_widget_t *w)
{
        ui_select_t *data =
            ui_widget_add_data(w, ui_select_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        data->owner = w;
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
        ui_text_set_content(w, "\xE2\x96\xBC");
}

static void ui_select_content_on_init(ui_widget_t *w)
{
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
        ui_select_bind_trigger(trigger);
        ui_select_bind_content(content);
        ui_select_sync_values(data);
}

int ui_select_append_item(ui_widget_t *select, ui_widget_t *item)
{
        ui_select_t *data = ui_select_get_data(select);

        ui_select_build_children(select);
        return ui_widget_append(data->content, item);
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
        while (ui_widget_get_child(data->content, 0)) {
                ui_widget_remove(ui_widget_get_child(data->content, 0));
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
        ui_select_trigger_prototype =
            ui_create_widget_prototype("select-trigger", NULL);
        ui_select_trigger_prototype->init = ui_select_trigger_on_init;
        ui_select_value_prototype =
            ui_create_widget_prototype("select-value", "text");
        ui_select_value_prototype->init = ui_select_value_on_init;
        ui_select_arrow_prototype =
            ui_create_widget_prototype("select-arrow", "text");
        ui_select_arrow_prototype->init = ui_select_arrow_on_init;
        ui_select_content_prototype =
            ui_create_widget_prototype("select-content", NULL);
        ui_select_content_prototype->init = ui_select_content_on_init;
        ui_select_item_prototype =
            ui_create_widget_prototype("select-item", "text");
        ui_load_css_string(ui_select_css, __FILE__);
}

void ui_unregister_select(void)
{
        list_destroy_without_node(&ui_select_registry,
                                  ui_select_destroy_registry_entry);
}
