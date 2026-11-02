/*
 * src/widgets/breadcrumb.c: -- Breadcrumb widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <LCUI/widgets/breadcrumb.h>
#include <LCUI/widgets/dropdown_menu.h>
#include <LCUI/widgets/icon.h>
#include <LCUI/widgets/text.h>
#include <css.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>

typedef struct ui_breadcrumb {
        ui_widget_t *owner;
        ui_widget_t *list;
        ui_widget_t *ellipsis_item;
        ui_widget_t *ellipsis_menu;
        ui_widget_t *ellipsis_content;
        int collapse_from;
        int collapse_start;
        int collapse_end;
        size_t collapse_count;
        char *separator;
        ui_widget_t **items;
        float *widths;
        size_t width_count;
        size_t item_count;
        float ellipsis_width;
        float measured_width;
        bool dirty;
        bool measuring;
} ui_breadcrumb_t;

static ui_widget_prototype_t *ui_breadcrumb_prototype;
static ui_widget_prototype_t *ui_breadcrumb_list_prototype;
static ui_widget_prototype_t *ui_breadcrumb_item_prototype;
static ui_widget_prototype_t *ui_breadcrumb_link_prototype;
static ui_widget_prototype_t *ui_breadcrumb_page_prototype;
static ui_widget_prototype_t *ui_breadcrumb_separator_prototype;
static ui_widget_prototype_t *ui_breadcrumb_ellipsis_prototype;

static void ui_breadcrumb_on_ellipsis_click(ui_widget_t *w, ui_event_t *e,
                                            void *arg);

static const char *ui_breadcrumb_css = "\
breadcrumb {\
  display: block;\
  min-width: 0;\
}\
breadcrumb-list {\
  display: flex;\
  flex-direction: row;\
  align-items: center;\
  gap: 6px;\
  white-space: nowrap;\
  min-width: 0;\
}\
breadcrumb-item {\
  display: inline-flex;\
  align-items: center;\
  gap: 6px;\
  flex-shrink: 0;\
  position: relative;\
}\
breadcrumb-item:collapsed {\
  display: none;\
}\
breadcrumb-separator {\
  display: inline-flex;\
  align-items: center;\
}\
breadcrumb-ellipsis {\
  display: inline-flex;\
  align-items: center;\
  justify-content: center;\
  width: 24px;\
  height: 24px;\
}\
";

static ui_breadcrumb_t *ui_breadcrumb_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_breadcrumb_prototype);
}

static ui_widget_t *ui_breadcrumb_find_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (strcmp(child->type, type) == 0) {
                        return child;
                }
                found = ui_breadcrumb_find_type(child, type);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_breadcrumb_ensure_list(ui_widget_t *breadcrumb)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        ui_widget_t *list;

        if (data->list) {
                return data->list;
        }
        list = ui_breadcrumb_find_type(breadcrumb, "breadcrumb-list");
        if (!list) {
                list = ui_create_breadcrumb_list();
                ui_widget_append(breadcrumb, list);
        }
        data->list = list;
        return list;
}

static size_t ui_breadcrumb_count_items(ui_breadcrumb_t *data)
{
        ui_widget_t *child;
        size_t i;
        size_t count = 0;

        if (!data->list) {
                return 0;
        }
        for (i = 0; (child = ui_widget_get_child(data->list, i)); ++i) {
                if (child == data->ellipsis_item ||
                    strcmp(child->type, "breadcrumb-item") != 0) {
                        continue;
                }
                count++;
        }
        return count;
}

static ui_widget_t *ui_breadcrumb_get_last_item(ui_breadcrumb_t *data)
{
        ui_widget_t *child;
        ui_widget_t *last = NULL;
        size_t i;

        if (!data->list) {
                return NULL;
        }
        for (i = 0; (child = ui_widget_get_child(data->list, i)); ++i) {
                if (child == data->ellipsis_item) {
                        continue;
                }
                last = child;
        }
        return last;
}

static const char *ui_breadcrumb_item_get_key(ui_widget_t *item)
{
        const char *key = ui_widget_get_attr(item, "data-key");

        if (!key) {
                key = ui_widget_get_attr(item, "data-path");
        }
        return key;
}

static void ui_breadcrumb_item_set_key(ui_widget_t *item, const char *key)
{
        if (key) {
                ui_widget_set_attr(item, "data-key", key);
        }
}

static ui_widget_t *ui_breadcrumb_create_separator(ui_breadcrumb_t *data)
{
        ui_widget_t *separator = ui_create_breadcrumb_separator();
        ui_widget_t *icon = ui_widget_get_child(separator, 0);

        if (icon && data->separator) {
                ui_icon_set_name(icon, data->separator);
        }
        return separator;
}

static void ui_breadcrumb_on_link_click(ui_widget_t *w, ui_event_t *e,
                                        void *arg)
{
        ui_widget_t *breadcrumb = e->data;
        ui_widget_t *item = w->parent;
        const char *key;
        ui_event_t ev;

        if (!breadcrumb || !item || item->disabled) {
                return;
        }
        key = ui_breadcrumb_item_get_key(item);
        if (!key) {
                return;
        }
        ui_event_init(&ev, "change");
        ui_widget_emit_event(breadcrumb, ev, (void *)key);
}

static void ui_breadcrumb_prepare_ellipsis(ui_breadcrumb_t *data)
{
        ui_widget_t *item;
        ui_widget_t *ellipsis;
        ui_widget_t *menu;
        ui_widget_t *content;

        if (!data->ellipsis_item) {
                item = ui_create_breadcrumb_item();
                ellipsis = ui_create_breadcrumb_ellipsis();
                menu = ui_create_dropdown_menu();
                content = ui_create_dropdown_menu_content();
                ui_widget_set_attr(ellipsis, "data-dropdown-menu-trigger",
                                   "true");
                ui_widget_set_attr(content, "side", "bottom");
                ui_widget_set_attr(content, "align", "start");
                ui_widget_on(ellipsis, "click", ui_breadcrumb_on_ellipsis_click,
                             data->owner);
                ui_widget_append(menu, ellipsis);
                ui_widget_append(menu, content);
                ui_widget_append(item, menu);
                ui_widget_append(item, ui_breadcrumb_create_separator(data));
                ui_widget_add_status(item, "collapsed");
                data->ellipsis_item = item;
                data->ellipsis_menu = menu;
                data->ellipsis_content = content;
        }
        /* ui_widget_append() unlinks an existing child and appends it, so this
         * also moves the ellipsis after the previous last item. */
        ui_widget_append(data->list, data->ellipsis_item);
}

static void ui_breadcrumb_on_menu_item_click(ui_widget_t *w, ui_event_t *e,
                                             void *arg)
{
        ui_widget_t *breadcrumb = e->data;
        ui_widget_t *target = e->target ? e->target : w;
        ui_breadcrumb_t *data;
        const char *key;
        ui_event_t ev;

        while (target && strcmp(target->type, "dropdown-menu-item") != 0) {
                target = target->parent;
        }
        key = target ? ui_widget_get_attr(target, "data-key") : NULL;
        if (!breadcrumb || !key) {
                return;
        }
        data = ui_breadcrumb_get_data(breadcrumb);
        if (data && data->ellipsis_menu) {
                ui_dropdown_menu_set_open(data->ellipsis_menu, false);
        }
        ui_event_init(&ev, "change");
        ui_widget_emit_event(breadcrumb, ev, (void *)key);
}

static void ui_breadcrumb_rebuild_menu(ui_widget_t *breadcrumb,
                                       ui_breadcrumb_t *data, int start,
                                       int end)
{
        ui_widget_t *content = data->ellipsis_content;
        ui_widget_t *menu_item;
        ui_widget_t *text;
        ui_widget_t *source;
        const char *key;
        const char *label;
        wchar_t wide[512];
        int i;

        if (!content) {
                return;
        }
        ui_widget_empty(content);
        if (start < 0 || end < start) {
                return;
        }
        for (i = start; i <= end; ++i) {
                key = ui_breadcrumb_item_get_key(data->items[i]);
                label = ui_widget_get_attr(data->items[i], "data-label");
                source = ui_breadcrumb_find_type(data->items[i], "text");
                wide[0] = 0;
                if (source) {
                        ui_text_get_content_w(source, wide, 512);
                }
                if ((!label || !label[0]) && !wide[0] && !key) {
                        continue;
                }
                menu_item = ui_create_dropdown_menu_item();
                if (key) {
                        ui_widget_set_attr(menu_item, "data-key", key);
                } else {
                        ui_widget_set_disabled(menu_item, true);
                }
                text = ui_create_widget("text");
                if (label && label[0]) {
                        ui_text_set_content(text, label);
                } else if (wide[0]) {
                        ui_text_set_content_w(text, wide);
                } else {
                        ui_text_set_content(text, key);
                }
                ui_widget_append(menu_item, text);
                ui_widget_on(menu_item, "click",
                             ui_breadcrumb_on_menu_item_click, breadcrumb);
                ui_widget_append(content, menu_item);
        }
}

static void ui_breadcrumb_on_ellipsis_click(ui_widget_t *w, ui_event_t *e,
                                            void *arg)
{
        ui_widget_t *breadcrumb = e->data;
        ui_breadcrumb_t *data =
            breadcrumb ? ui_breadcrumb_get_data(breadcrumb) : NULL;

        if (!data || data->collapse_start < 0) {
                return;
        }
        ui_breadcrumb_rebuild_menu(breadcrumb, data, data->collapse_start,
                                   data->collapse_end);
}

static bool ui_breadcrumb_normalize(ui_widget_t *w, ui_breadcrumb_t *data)
{
        ui_widget_t *child;
        ui_widget_t *last;
        size_t i = 0;
        bool moved = false;

        while ((child = ui_widget_get_child(w, i))) {
                if (strcmp(child->type, "breadcrumb-item") != 0) {
                        i++;
                        continue;
                }
                ui_widget_append(data->list, child);
                moved = true;
                i = 0;
        }
        if (!data->ellipsis_item && ui_breadcrumb_count_items(data) >= 2) {
                last = ui_breadcrumb_get_last_item(data);
                ui_breadcrumb_prepare_ellipsis(data);
                if (last) {
                        ui_widget_append(data->list, last);
                }
                moved = true;
        }
        return moved;
}

static void ui_breadcrumb_bind_links(ui_widget_t *w, ui_breadcrumb_t *data)
{
        ui_widget_t *child;
        ui_widget_t *link;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(data->list, i)); ++i) {
                if (child == data->ellipsis_item) {
                        continue;
                }
                link = ui_breadcrumb_find_type(child, "breadcrumb-link");
                if (link &&
                    !ui_widget_get_attr(link, "data-breadcrumb-bound")) {
                        ui_widget_set_attr(link, "data-breadcrumb-bound",
                                           "true");
                        ui_widget_on(link, "click", ui_breadcrumb_on_link_click,
                                     w);
                }
        }
}

static void ui_breadcrumb_sync_separators(ui_breadcrumb_t *data)
{
        ui_widget_t *item;
        ui_widget_t *separator;
        ui_widget_t *icon;
        char class_name[128];
        size_t i;

        if (!data->list || !data->separator) {
                return;
        }
        snprintf(class_name, sizeof(class_name), "fui-icon-%s",
                 data->separator);
        for (i = 0; (item = ui_widget_get_child(data->list, i)); ++i) {
                separator =
                    ui_breadcrumb_find_type(item, "breadcrumb-separator");
                if (!separator) {
                        continue;
                }
                icon = ui_widget_get_child(separator, 0);
                if (icon && !ui_widget_has_class(icon, class_name)) {
                        ui_icon_set_name(icon, data->separator);
                }
        }
}

static void ui_breadcrumb_demote_page(ui_widget_t *breadcrumb,
                                      ui_widget_t *item)
{
        ui_widget_t *page = ui_breadcrumb_find_type(item, "breadcrumb-page");
        ui_widget_t *link;
        wchar_t text[512];

        if (!page) {
                return;
        }
        text[0] = 0;
        ui_text_get_content_w(page, text, 512);
        ui_widget_destroy(page);
        link = ui_create_breadcrumb_link();
        ui_text_set_content_w(link, text);
        ui_widget_set_attr(link, "data-breadcrumb-bound", "true");
        ui_widget_on(link, "click", ui_breadcrumb_on_link_click, breadcrumb);
        ui_widget_prepend(item, link);
}

void ui_breadcrumb_append_item(ui_widget_t *breadcrumb, const char *key,
                               const char *label)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        ui_widget_t *list = ui_breadcrumb_ensure_list(breadcrumb);
        ui_widget_t *last;
        ui_widget_t *item;
        ui_widget_t *page;
        size_t count = 0;
        const char *text = label ? label : key;

        while (ui_widget_get_child(list, count)) {
                count++;
        }
        if (count > 0) {
                last = ui_widget_get_child(list, count - 1);
                ui_breadcrumb_demote_page(breadcrumb, last);
                if (!ui_breadcrumb_find_type(last, "breadcrumb-separator")) {
                        ui_widget_append(last,
                                         ui_breadcrumb_create_separator(data));
                }
        }
        item = ui_create_breadcrumb_item();
        ui_breadcrumb_item_set_key(item, key);
        if (text) {
                ui_widget_set_attr(item, "data-label", text);
        }
        page = ui_create_breadcrumb_page();
        if (text) {
                ui_text_set_content(page, text);
        }
        ui_widget_append(item, page);
        ui_breadcrumb_prepare_ellipsis(data);
        ui_widget_append(list, item);
        data->dirty = true;
}

void ui_breadcrumb_append(ui_widget_t *breadcrumb, ui_widget_t *content)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        ui_widget_t *list = ui_breadcrumb_ensure_list(breadcrumb);
        ui_widget_t *last;
        ui_widget_t *item;
        const char *key;
        const char *label;
        size_t count = 0;

        if (!content) {
                return;
        }
        while (ui_widget_get_child(list, count)) {
                count++;
        }
        if (count > 0) {
                last = ui_widget_get_child(list, count - 1);
                ui_breadcrumb_demote_page(breadcrumb, last);
                if (!ui_breadcrumb_find_type(last, "breadcrumb-separator")) {
                        ui_widget_append(last,
                                         ui_breadcrumb_create_separator(data));
                }
        }
        if (strcmp(content->type, "breadcrumb-item") == 0) {
                item = content;
        } else {
                item = ui_create_breadcrumb_item();
                ui_widget_append(item, content);
        }
        key = ui_widget_get_attr(content, "data-key");
        if (key) {
                ui_widget_set_attr(item, "data-key", key);
        }
        label = ui_widget_get_attr(content, "data-label");
        if (label) {
                ui_widget_set_attr(item, "data-label", label);
        }
        ui_breadcrumb_prepare_ellipsis(data);
        ui_widget_append(list, item);
        data->dirty = true;
}

const char *ui_breadcrumb_get_key(ui_widget_t *breadcrumb)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        ui_widget_t *list = data ? data->list : NULL;
        ui_widget_t *last;
        const char *key;
        size_t count = 0;

        if (!list) {
                return "";
        }
        while (ui_widget_get_child(list, count)) {
                count++;
        }
        if (count == 0) {
                return "";
        }
        last = ui_widget_get_child(list, count - 1);
        key = ui_breadcrumb_item_get_key(last);
        return key ? key : "";
}

static bool ui_breadcrumb_has_explicit_width(ui_widget_t *w)
{
        css_numeric_value_t value;
        css_unit_t unit;

        return css_computed_width(&w->specified_style, &value, &unit) ==
               CSS_WIDTH_SET;
}

static float ui_breadcrumb_available_width(ui_widget_t *w)
{
        if (!ui_breadcrumb_has_explicit_width(w) && w->parent) {
                return w->parent->content_box.width;
        }
        return w->content_box.width;
}

static void ui_breadcrumb_start_measure(ui_widget_t *w)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(w);
        ui_widget_t *child;
        ui_widget_t *ellipsis;
        size_t i;
        float available;

        if (!data || !data->list) {
                return;
        }
        available = ui_breadcrumb_available_width(w);
        if (available <= 0.0f) {
                return;
        }
        for (i = 0; (child = ui_widget_get_child(data->list, i)); ++i) {
                ui_widget_remove_status(child, "collapsed");
        }
        ellipsis = data->ellipsis_item;
        if (ellipsis) {
                ui_widget_remove_status(ellipsis, "collapsed");
        }
        data->measured_width = available;
        data->dirty = false;
        data->measuring = true;
        ui_widget_request_reflow(w);
}

static bool ui_breadcrumb_fits(ui_breadcrumb_t *data, float container,
                               int prefix_len, int suffix_start)
{
        float fit = data->ellipsis_width;
        int visible = 1;
        int i;

        for (i = 0; i < prefix_len; ++i) {
                fit += data->widths[i];
                visible++;
        }
        for (i = suffix_start; i < (int)data->item_count; ++i) {
                fit += data->widths[i];
                visible++;
        }
        fit += 6.0f * (visible - 1);
        return fit <= container;
}

static void ui_breadcrumb_measure(ui_widget_t *w)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(w);
        ui_widget_t *child;
        float container = ui_breadcrumb_available_width(w);
        float total = 0.0f;
        int start;
        int end;
        size_t count;
        size_t i;
        size_t capacity = 0;

        if (!data || !data->list) {
                return;
        }
        if (container <= 0.0f) {
                data->measuring = false;
                return;
        }
        while (ui_widget_get_child(data->list, capacity)) {
                capacity++;
        }
        if (capacity > data->width_count) {
                data->items =
                    realloc(data->items, sizeof(*data->items) * capacity);
                data->widths =
                    realloc(data->widths, sizeof(*data->widths) * capacity);
                data->width_count = capacity;
        }
        data->item_count = 0;
        data->ellipsis_width = 0.0f;
        for (i = 0; (child = ui_widget_get_child(data->list, i)); ++i) {
                if (child == data->ellipsis_item) {
                        data->ellipsis_width = child->border_box.width;
                        continue;
                }
                if (strcmp(child->type, "breadcrumb-item") != 0) {
                        continue;
                }
                data->items[data->item_count] = child;
                data->widths[data->item_count] = child->border_box.width;
                data->item_count++;
        }
        data->measuring = false;
        count = data->item_count;
        if (count < 2) {
                if (data->ellipsis_item) {
                        ui_widget_add_status(data->ellipsis_item, "collapsed");
                }
                data->collapse_start = -1;
                data->collapse_end = -1;
                data->collapse_count = count;
                ui_widget_request_reflow(w);
                return;
        }
        for (i = 0; i < count; ++i) {
                total += data->widths[i];
        }
        total += 6.0f * (count - 1);
        /* Publish the full intrinsic width to the layout engine. The first
         * pass can run before child items have been measured, which would
         * otherwise cache a min-content width and keep the breadcrumb
         * collapsed after the parent gains its real width. */
        if (!ui_breadcrumb_has_explicit_width(w) &&
            fabsf(w->unconstrained_content_width - total) > 0.5f) {
                w->unconstrained_content_width = total;
                if (w->parent) {
                        ui_widget_request_reflow(w->parent);
                }
        }
        if (total <= container) {
                for (i = 0; i < count; ++i) {
                        ui_widget_remove_status(data->items[i], "collapsed");
                }
                if (data->ellipsis_item) {
                        ui_widget_add_status(data->ellipsis_item, "collapsed");
                }
                data->collapse_start = -1;
                data->collapse_end = -1;
                data->collapse_count = count;
                ui_widget_request_reflow(w);
                return;
        }
        /* Collapse as few items as possible: keep every trailing item that
         * still fits, and only move the hidden range leftward when nothing
         * else fits. `collapse-from` is the leftmost index that may be hidden
         * without also dropping the leading context. */
        start = data->collapse_from;
        if (start > (int)count - 2) {
                start = (int)count - 2;
        }
        if (start < 0) {
                start = 0;
        }
        for (end = start; end <= (int)count - 2; ++end) {
                if (ui_breadcrumb_fits(data, container, start, end + 1)) {
                        break;
                }
        }
        if (end > (int)count - 2) {
                end = (int)count - 2;
                for (start = data->collapse_from - 1; start >= 0; --start) {
                        if (ui_breadcrumb_fits(data, container, start,
                                               (int)count - 1)) {
                                break;
                        }
                }
                if (start < 0) {
                        start = 0;
                }
        }
        for (i = 0; i < count; ++i) {
                if ((int)i >= start && (int)i <= end) {
                        ui_widget_add_status(data->items[i], "collapsed");
                } else {
                        ui_widget_remove_status(data->items[i], "collapsed");
                }
        }
        if (data->ellipsis_item) {
                ui_widget_remove_status(data->ellipsis_item, "collapsed");
        }
        data->collapse_start = start;
        data->collapse_end = end;
        data->collapse_count = count;
        ui_widget_request_reflow(w);
}

static void ui_breadcrumb_on_afterlayout(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(w);

        if (!data || !data->list) {
                return;
        }
        if (ui_breadcrumb_normalize(w, data)) {
                data->dirty = true;
        }
        ui_breadcrumb_sync_separators(data);
        ui_breadcrumb_bind_links(w, data);
        if (w->content_box.width <= 0.0f ||
            ui_breadcrumb_available_width(w) <= 0.0f) {
                return;
        }
        if (data->measuring) {
                ui_breadcrumb_measure(w);
                return;
        }
        if (!data->dirty &&
            ui_breadcrumb_available_width(w) == data->measured_width) {
                return;
        }
        ui_breadcrumb_start_measure(w);
}

int ui_breadcrumb_get_collapse_from(ui_widget_t *breadcrumb)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);

        return data ? data->collapse_from : 1;
}

void ui_breadcrumb_set_collapse_from(ui_widget_t *breadcrumb, int index)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        size_t count;

        if (!data) {
                return;
        }
        count = ui_breadcrumb_count_items(data);
        if (count >= 2) {
                if (index < 0) {
                        index = 0;
                } else if (index > (int)count - 2) {
                        index = (int)count - 2;
                }
        }
        data->collapse_from = index;
        data->dirty = true;
        ui_widget_request_reflow(breadcrumb);
}

void ui_breadcrumb_build_from_path(ui_widget_t *breadcrumb, const char *path,
                                   ui_breadcrumb_label_provider_t provider,
                                   void *arg)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        ui_widget_t *list = ui_breadcrumb_ensure_list(breadcrumb);
        char *copy;
        char *segment;
        char node_path[1024];
        const char *label;
        size_t len;

        ui_widget_empty(list);
        data->ellipsis_item = NULL;
        data->ellipsis_menu = NULL;
        data->ellipsis_content = NULL;
        data->measuring = false;
        data->item_count = 0;
        data->dirty = true;
        if (!path || !path[0]) {
                return;
        }
        if (strcmp(path, "/") == 0) {
                ui_breadcrumb_append_item(breadcrumb, "/", "/");
                return;
        }
        copy = y_strdup(path);
        node_path[0] = 0;
        segment = strtok(copy, "/");
        while (segment) {
                len = strlen(node_path);
                if (len + strlen(segment) + 2 < sizeof(node_path)) {
                        node_path[len] = '/';
                        strcpy(node_path + len + 1, segment);
                }
                label = provider ? provider(segment, node_path, arg) : segment;
                if (!label || !label[0]) {
                        label = segment;
                }
                ui_breadcrumb_append_item(breadcrumb, segment, label);
                segment = strtok(NULL, "/");
        }
        free(copy);
}

void ui_breadcrumb_set_separator(ui_widget_t *breadcrumb, const char *icon_name)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(breadcrumb);
        ui_widget_t *item;
        ui_widget_t *separator;
        ui_widget_t *icon;
        size_t i;

        if (!data || !icon_name) {
                return;
        }
        if (!data->list) {
                ui_breadcrumb_ensure_list(breadcrumb);
        }
        free(data->separator);
        data->separator = y_strdup(icon_name);
        for (i = 0; (item = ui_widget_get_child(data->list, i)); ++i) {
                separator =
                    ui_breadcrumb_find_type(item, "breadcrumb-separator");
                if (!separator) {
                        continue;
                }
                icon = ui_widget_get_child(separator, 0);
                if (icon) {
                        ui_icon_set_name(icon, icon_name);
                }
        }
}

static void ui_breadcrumb_on_setattr(ui_widget_t *w, const char *name,
                                     const char *value)
{
        if (strcmp(name, "separator") == 0) {
                ui_breadcrumb_set_separator(w, value);
        } else if (strcmp(name, "path") == 0) {
                ui_breadcrumb_build_from_path(w, value, NULL, NULL);
        }
}

static void ui_breadcrumb_on_init(ui_widget_t *w)
{
        ui_breadcrumb_t *data =
            ui_widget_add_data(w, ui_breadcrumb_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        data->owner = w;
        data->collapse_from = 1;
        data->collapse_start = -1;
        data->collapse_end = -1;
        data->separator = y_strdup("chevron-right");
        data->list = ui_create_breadcrumb_list();
        ui_widget_append(w, data->list);
        ui_widget_on(w, "afterlayout", ui_breadcrumb_on_afterlayout, NULL);
}

static void ui_breadcrumb_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(w);

        if (task != UI_TASK_BEFORE_UPDATE || !data || !data->list) {
                return;
        }
        /* Move items appended directly to the breadcrumb into the list
         * before children are updated, so their intrinsic widths are
         * available to the first layout pass. */
        if (ui_breadcrumb_normalize(w, data)) {
                data->dirty = true;
        }
}

static void ui_breadcrumb_on_destroy(ui_widget_t *w)
{
        ui_breadcrumb_t *data = ui_breadcrumb_get_data(w);

        if (!data) {
                return;
        }
        free(data->separator);
        free(data->items);
        free(data->widths);
        data->separator = NULL;
        data->items = NULL;
        data->widths = NULL;
}

static void ui_breadcrumb_separator_on_init(ui_widget_t *w)
{
        ui_widget_t *icon = ui_create_fluent_icon("chevron-right");

        ui_icon_set_size(icon, 16);
        ui_widget_append(w, icon);
}

static void ui_breadcrumb_ellipsis_on_init(ui_widget_t *w)
{
        ui_widget_t *icon = ui_create_fluent_icon("more-horizontal");

        ui_icon_set_size(icon, 16);
        ui_widget_append(w, icon);
}

ui_widget_t *ui_create_breadcrumb(void)
{
        return ui_create_widget_with_prototype(ui_breadcrumb_prototype);
}

ui_widget_t *ui_create_breadcrumb_list(void)
{
        return ui_create_widget_with_prototype(ui_breadcrumb_list_prototype);
}

ui_widget_t *ui_create_breadcrumb_item(void)
{
        return ui_create_widget_with_prototype(ui_breadcrumb_item_prototype);
}

ui_widget_t *ui_create_breadcrumb_link(void)
{
        return ui_create_widget_with_prototype(ui_breadcrumb_link_prototype);
}

ui_widget_t *ui_create_breadcrumb_page(void)
{
        return ui_create_widget_with_prototype(ui_breadcrumb_page_prototype);
}

ui_widget_t *ui_create_breadcrumb_separator(void)
{
        return ui_create_widget_with_prototype(
            ui_breadcrumb_separator_prototype);
}

ui_widget_t *ui_create_breadcrumb_ellipsis(void)
{
        return ui_create_widget_with_prototype(
            ui_breadcrumb_ellipsis_prototype);
}

void ui_register_breadcrumb(void)
{
        ui_breadcrumb_prototype =
            ui_create_widget_prototype("breadcrumb", NULL);
        ui_breadcrumb_list_prototype =
            ui_create_widget_prototype("breadcrumb-list", NULL);
        ui_breadcrumb_item_prototype =
            ui_create_widget_prototype("breadcrumb-item", NULL);
        ui_breadcrumb_link_prototype =
            ui_create_widget_prototype("breadcrumb-link", "text");
        ui_breadcrumb_page_prototype =
            ui_create_widget_prototype("breadcrumb-page", "text");
        ui_breadcrumb_separator_prototype =
            ui_create_widget_prototype("breadcrumb-separator", NULL);
        ui_breadcrumb_ellipsis_prototype =
            ui_create_widget_prototype("breadcrumb-ellipsis", NULL);

        ui_breadcrumb_prototype->init = ui_breadcrumb_on_init;
        ui_breadcrumb_prototype->destroy = ui_breadcrumb_on_destroy;
        ui_breadcrumb_prototype->setattr = ui_breadcrumb_on_setattr;
        ui_breadcrumb_prototype->update = ui_breadcrumb_on_update;
        ui_breadcrumb_separator_prototype->init =
            ui_breadcrumb_separator_on_init;
        ui_breadcrumb_ellipsis_prototype->init = ui_breadcrumb_ellipsis_on_init;

        ui_load_css_string(ui_breadcrumb_css, __FILE__);
}

void ui_unregister_breadcrumb(void)
{
}
