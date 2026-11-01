/*
 * src/widgets/accordion.c: -- Accordion widgets
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
#include <ptk.h>
#include <LCUI/widgets/accordion.h>
#include <LCUI/widgets/icon.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>

typedef struct ui_accordion {
        bool multiple;
        bool initialized;
        bool syncing;
} ui_accordion_t;

typedef struct ui_accordion_item {
        bool open;
        bool syncing;
} ui_accordion_item_t;

static ui_widget_prototype_t *ui_accordion_prototype;
static ui_widget_prototype_t *ui_accordion_item_prototype;
static ui_widget_prototype_t *ui_accordion_trigger_prototype;
static ui_widget_prototype_t *ui_accordion_content_prototype;

static void ui_accordion_set_item_state(ui_widget_t *item, bool open,
                                        bool notify);

static const char *ui_accordion_css = "\
accordion {\
  display: block;\
  width: 100%;\
}\
accordion-item {\
  display: block;\
  position: relative;\
}\
accordion-trigger {\
  display: flex;\
  align-items: center;\
  justify-content: space-between;\
  width: 100%;\
  cursor: pointer;\
}\
accordion-content {\
  display: block;\
}\
accordion-item:closed accordion-content {\
  display: none;\
}\
";

static ui_accordion_t *ui_accordion_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_accordion_prototype);
}

static ui_accordion_item_t *ui_accordion_item_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_accordion_item_prototype);
}

static bool ui_accordion_value_is_true(const char *value)
{
        return value &&
               (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
}

static bool ui_accordion_is_root(ui_widget_t *w)
{
        return w && ui_check_widget_prototype(w, ui_accordion_prototype);
}

static bool ui_accordion_is_item(ui_widget_t *w)
{
        return w && ui_check_widget_prototype(w, ui_accordion_item_prototype);
}

static ui_widget_t *ui_accordion_find_root(ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (ui_accordion_is_root(w)) {
                        return w;
                }
        }
        return NULL;
}

static ui_widget_t *ui_accordion_find_item(ui_widget_t *target,
                                           ui_widget_t *root)
{
        for (; target && target != root; target = target->parent) {
                if (ui_accordion_is_item(target)) {
                        return target;
                }
        }
        return NULL;
}

static bool ui_accordion_is_trigger(ui_widget_t *w)
{
        if (!w) {
                return false;
        }
        if (ui_check_widget_prototype(w, ui_accordion_trigger_prototype)) {
                return true;
        }
        return ui_accordion_value_is_true(
            ui_widget_get_attr(w, "data-accordion-trigger"));
}

static ui_widget_t *ui_accordion_trigger_find_icon(ui_widget_t *w)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_widget_has_class(child, "accordion-trigger-icon")) {
                        return child;
                }
        }
        return NULL;
}

static ui_widget_t *ui_accordion_find_trigger_widget(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child,
                                              ui_accordion_trigger_prototype)) {
                        return child;
                }
                if (ui_accordion_is_root(child) ||
                    ui_accordion_is_item(child)) {
                        continue;
                }
                found = ui_accordion_find_trigger_widget(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *ui_accordion_trigger_last_child(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *last = NULL;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                last = child;
        }
        return last;
}

static void ui_accordion_trigger_sync_chevron(ui_widget_t *w)
{
        const char *chevron = ui_widget_get_attr(w, "chevron");
        bool want = !chevron || ui_accordion_value_is_true(chevron);
        ui_widget_t *item = ui_accordion_find_item(w, NULL);
        ui_widget_t *icon = ui_accordion_trigger_find_icon(w);

        if (want && !icon) {
                icon = ui_create_fluent_icon("chevron-down");
                ui_widget_add_class(icon, "accordion-trigger-icon");
                ui_icon_set_size(icon, 16);
                ui_widget_append(w, icon);
        } else if (!want && icon) {
                ui_widget_destroy(icon);
                return;
        }
        if (icon) {
                if (ui_accordion_trigger_last_child(w) != icon) {
                        ui_widget_append(w, icon);
                }
                ui_icon_set_name(icon, item && ui_accordion_item_get_open(item)
                                           ? "chevron-up"
                                           : "chevron-down");
        }
}

static void ui_accordion_trigger_on_init(ui_widget_t *w)
{
        w->tab_index = 0;
        ui_accordion_trigger_sync_chevron(w);
}

static void ui_accordion_trigger_on_set_attr(ui_widget_t *w, const char *name,
                                             const char *value)
{
        if (strcmp(name, "chevron") == 0) {
                ui_accordion_trigger_sync_chevron(w);
        }
}

static void ui_accordion_trigger_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_widget_t *icon;
        ui_widget_t *last;

        if (task != UI_TASK_BEFORE_UPDATE) {
                return;
        }
        icon = ui_accordion_trigger_find_icon(w);
        if (!icon) {
                return;
        }
        last = ui_accordion_trigger_last_child(w);
        if (last != icon) {
                ui_widget_append(w, icon);
        }
}

static void ui_accordion_sync_item_chevron(ui_widget_t *item)
{
        ui_widget_t *trigger = ui_accordion_find_trigger_widget(item);

        if (trigger) {
                ui_accordion_trigger_sync_chevron(trigger);
        }
}

static ui_widget_t *ui_accordion_find_trigger(ui_widget_t *target,
                                              ui_widget_t *item)
{
        for (; target && target != item; target = target->parent) {
                if (ui_accordion_is_trigger(target)) {
                        return target;
                }
        }
        return NULL;
}

static bool ui_accordion_toggle_from_target(ui_widget_t *root,
                                            ui_widget_t *target)
{
        ui_widget_t *item;
        ui_widget_t *trigger;

        if (!target || ui_accordion_find_root(target) != root) {
                return false;
        }
        item = ui_accordion_find_item(target, root);
        if (!item || item->disabled) {
                return false;
        }
        trigger = ui_accordion_find_trigger(target, item);
        if (!trigger) {
                return false;
        }
        ui_accordion_item_set_open(item, !ui_accordion_item_get_open(item));
        return true;
}

static void ui_accordion_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        if (ui_accordion_toggle_from_target(w, e->target)) {
                e->cancel_bubble = true;
        }
}

static void ui_accordion_on_keydown(ui_widget_t *w, ui_event_t *e, void *arg)
{
        if (e->key.code != KEY_ENTER && e->key.code != KEY_SPACE) {
                return;
        }
        if (ui_accordion_toggle_from_target(w, e->target)) {
                e->cancel_bubble = true;
        }
}

static const char *ui_accordion_item_value(ui_widget_t *item)
{
        const char *value = ui_widget_get_attr(item, "value");

        return value ? value : "";
}

static bool ui_accordion_segment_matches(const char *start, size_t length,
                                         const char *value)
{
        size_t value_len = strlen(value);

        return value_len > 0 && length == value_len &&
               strncmp(start, value, length) == 0;
}

static bool ui_accordion_list_has(const char *list, const char *value)
{
        if (!value || !value[0]) {
                return false;
        }
        while (list && *list) {
                const char *start = list;
                const char *end = strchr(list, ',');
                size_t len = end ? (size_t)(end - list) : strlen(list);

                while (len && (*start == ' ' || *start == '\t')) {
                        start++;
                        len--;
                }
                while (len &&
                       (start[len - 1] == ' ' || start[len - 1] == '\t')) {
                        len--;
                }
                if (len && ui_accordion_segment_matches(start, len, value)) {
                        return true;
                }
                if (!end) {
                        break;
                }
                list = end + 1;
        }
        return false;
}

struct ui_accordion_has_value_ctx {
        ui_widget_t *root;
        const char *value;
        size_t length;
        bool found;
};

static void ui_accordion_has_value(ui_widget_t *w, void *arg)
{
        struct ui_accordion_has_value_ctx *ctx = arg;

        if (ctx->found || !ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root) {
                return;
        }
        if (ui_accordion_segment_matches(ctx->value, ctx->length,
                                         ui_accordion_item_value(w))) {
                ctx->found = true;
        }
}

/* Single mode opens the first listed value that has a matching item;
 * entries without an item are skipped rather than aborting the list. */
static bool ui_accordion_first_list_value(ui_widget_t *root, const char *list,
                                          const char **value, size_t *length)
{
        while (list && *list) {
                const char *start = list;
                const char *end = strchr(list, ',');
                size_t len = end ? (size_t)(end - list) : strlen(list);
                struct ui_accordion_has_value_ctx ctx = { .root = root };

                while (len && (*start == ' ' || *start == '\t')) {
                        start++;
                        len--;
                }
                while (len &&
                       (start[len - 1] == ' ' || start[len - 1] == '\t')) {
                        len--;
                }
                if (len) {
                        ctx.value = start;
                        ctx.length = len;
                        ui_widget_each(root, ui_accordion_has_value, &ctx);
                        if (ctx.found) {
                                *value = start;
                                *length = len;
                                return true;
                        }
                }
                if (!end) {
                        break;
                }
                list = end + 1;
        }
        return false;
}

struct ui_accordion_value_buf {
        ui_widget_t *root;
        char *data;
        size_t length;
        size_t capacity;
};

static bool ui_accordion_buf_reserve(struct ui_accordion_value_buf *buf,
                                     size_t extra)
{
        size_t need = buf->length + extra + 1;
        size_t capacity = buf->capacity ? buf->capacity : 64;
        char *data;

        if (need <= buf->capacity) {
                return true;
        }
        while (capacity < need) {
                capacity *= 2;
        }
        data = realloc(buf->data, capacity);
        if (!data) {
                return false;
        }
        buf->data = data;
        buf->capacity = capacity;
        return true;
}

static bool ui_accordion_buf_has(struct ui_accordion_value_buf *buf,
                                 const char *value)
{
        size_t start = 0;
        size_t value_len = strlen(value);

        if (!buf->data) {
                return false;
        }
        while (start <= buf->length) {
                const char *segment = buf->data + start;
                const char *comma = strchr(segment, ',');
                size_t len =
                    comma ? (size_t)(comma - segment) : strlen(segment);

                if (len == value_len && strncmp(segment, value, len) == 0) {
                        return true;
                }
                if (!comma) {
                        break;
                }
                start += len + 1;
        }
        return false;
}

static bool ui_accordion_buf_append(struct ui_accordion_value_buf *buf,
                                    const char *value)
{
        size_t value_len = strlen(value);

        if (!value_len) {
                return true;
        }
        if (buf->length) {
                if (!ui_accordion_buf_reserve(buf, 1)) {
                        return false;
                }
                buf->data[buf->length++] = ',';
        }
        if (!ui_accordion_buf_reserve(buf, value_len)) {
                return false;
        }
        memcpy(buf->data + buf->length, value, value_len);
        buf->length += value_len;
        buf->data[buf->length] = '\0';
        return true;
}

static void ui_accordion_collect_value(ui_widget_t *w, void *arg)
{
        struct ui_accordion_value_buf *buf = arg;
        const char *value;

        if (!ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != buf->root) {
                return;
        }
        if (!ui_accordion_item_get_open(w)) {
                return;
        }
        value = ui_accordion_item_value(w);
        if (value[0] && !ui_accordion_buf_has(buf, value)) {
                ui_accordion_buf_append(buf, value);
        }
}

static void ui_accordion_sync_value(ui_widget_t *root)
{
        ui_accordion_t *data = ui_accordion_get_data(root);
        struct ui_accordion_value_buf buf = { .root = root };

        if (!data || data->syncing) {
                return;
        }
        ui_widget_each(root, ui_accordion_collect_value, &buf);
        data->syncing = true;
        ui_widget_set_attr(root, "value", buf.data ? buf.data : "");
        data->syncing = false;
        free(buf.data);
}

struct ui_accordion_apply_ctx {
        ui_widget_t *root;
        const char *values;
        const char *single_value;
        size_t single_length;
        bool multiple;
        bool opened;
};

static void ui_accordion_apply_value(ui_widget_t *w, void *arg)
{
        struct ui_accordion_apply_ctx *ctx = arg;
        const char *value;
        size_t length;
        bool open;

        if (!ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root) {
                return;
        }
        if (ctx->multiple) {
                open = ui_accordion_list_has(ctx->values,
                                             ui_accordion_item_value(w));
        } else if (ctx->single_value) {
                value = ui_accordion_item_value(w);
                length = strlen(value);
                open = length == ctx->single_length &&
                       strncmp(value, ctx->single_value, length) == 0;
                if (open && ctx->opened) {
                        open = false;
                }
                if (open) {
                        ctx->opened = true;
                }
        } else {
                open = false;
        }
        ui_accordion_set_item_state(w, open, false);
}

struct ui_accordion_close_ctx {
        ui_widget_t *root;
        ui_widget_t *except;
};

static void ui_accordion_close_other(ui_widget_t *w, void *arg)
{
        struct ui_accordion_close_ctx *ctx = arg;

        if (w == ctx->except || !ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root) {
                return;
        }
        if (ui_accordion_item_get_open(w)) {
                ui_accordion_set_item_state(w, false, false);
        }
}

static void ui_accordion_set_item_state(ui_widget_t *item, bool open,
                                        bool notify)
{
        ui_accordion_item_t *item_data = ui_accordion_item_get_data(item);
        ui_widget_t *root = ui_accordion_find_root(item);
        ui_accordion_t *data = root ? ui_accordion_get_data(root) : NULL;
        ui_event_t ev;
        const char *value;
        char *payload;
        size_t length;

        if (!item_data || item_data->open == open) {
                return;
        }
        item_data->open = open;
        if (open) {
                ui_widget_remove_status(item, "closed");
                ui_widget_add_status(item, "open");
        } else {
                ui_widget_remove_status(item, "open");
                ui_widget_add_status(item, "closed");
        }
        ui_accordion_sync_item_chevron(item);
        item_data->syncing = true;
        ui_widget_set_attr(item, "open", open ? "true" : "false");
        item_data->syncing = false;
        if (!root || !data) {
                return;
        }
        if (!data->syncing) {
                if (open && !data->multiple) {
                        struct ui_accordion_close_ctx ctx = {
                                .root = root,
                                .except = item,
                        };

                        data->syncing = true;
                        ui_widget_each(root, ui_accordion_close_other, &ctx);
                        data->syncing = false;
                }
                if (data->initialized) {
                        ui_accordion_sync_value(root);
                }
        }
        if (notify && data->initialized) {
                value = ui_accordion_item_value(item);
                length = strlen(value);
                payload = malloc(length + 1);
                if (payload) {
                        memcpy(payload, value, length + 1);
                }
                ui_event_init(&ev, open ? "open" : "close");
                ui_widget_emit_event(item, ev, NULL);
                ui_event_init(&ev, "change");
                ui_widget_emit_event(root, ev,
                                     payload ? (void *)payload : (void *)value);
                free(payload);
        }
}

static void ui_accordion_apply_values(ui_widget_t *root, const char *values)
{
        ui_accordion_t *data = ui_accordion_get_data(root);
        struct ui_accordion_apply_ctx ctx = {
                .root = root,
                .values = values ? values : "",
                .multiple = data ? data->multiple : false,
        };

        if (!data || data->syncing) {
                return;
        }
        if (!ctx.multiple) {
                ui_accordion_first_list_value(
                    root, ctx.values, &ctx.single_value, &ctx.single_length);
        }
        data->syncing = true;
        ui_widget_each(root, ui_accordion_apply_value, &ctx);
        data->syncing = false;
        ui_accordion_sync_value(root);
}

struct ui_accordion_last_ctx {
        ui_widget_t *root;
        ui_widget_t *last;
};

static void ui_accordion_clear_last(ui_widget_t *w, void *arg)
{
        struct ui_accordion_last_ctx *ctx = arg;

        if (!ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root) {
                return;
        }
        ui_widget_remove_status(w, "last");
}

static void ui_accordion_find_last(ui_widget_t *w, void *arg)
{
        struct ui_accordion_last_ctx *ctx = arg;

        if (!ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root) {
                return;
        }
        ctx->last = w;
}

static void ui_accordion_sync_last(ui_widget_t *root)
{
        struct ui_accordion_last_ctx ctx = { .root = root };

        ui_widget_each(root, ui_accordion_clear_last, &ctx);
        ui_widget_each(root, ui_accordion_find_last, &ctx);
        if (ctx.last) {
                ui_widget_add_status(ctx.last, "last");
        }
}

static void ui_accordion_sync_chevron_each(ui_widget_t *w, void *arg)
{
        if (ui_accordion_is_item(w) && ui_accordion_find_root(w) == arg) {
                ui_accordion_sync_item_chevron(w);
        }
}

struct ui_accordion_keep_ctx {
        ui_widget_t *root;
        bool kept;
};

static void ui_accordion_keep_first(ui_widget_t *w, void *arg)
{
        struct ui_accordion_keep_ctx *ctx = arg;

        if (!ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root ||
            !ui_accordion_item_get_open(w)) {
                return;
        }
        if (!ctx->kept) {
                ctx->kept = true;
                return;
        }
        ui_accordion_set_item_state(w, false, false);
}

static void ui_accordion_on_afterlayout(ui_widget_t *w, ui_event_t *e,
                                        void *arg)
{
        ui_accordion_t *data = ui_accordion_get_data(w);
        const char *value;
        const char *default_value;

        if (!data) {
                return;
        }
        if (!data->initialized) {
                data->initialized = true;
                value = ui_widget_get_attr(w, "value");
                default_value = ui_widget_get_attr(w, "default-value");
                if (value || default_value) {
                        ui_accordion_apply_values(
                            w, value ? value : default_value);
                } else {
                        ui_accordion_sync_value(w);
                }
        }
        ui_accordion_sync_last(w);
        ui_widget_each(w, ui_accordion_sync_chevron_each, w);
}

static void ui_accordion_on_init(ui_widget_t *w)
{
        ui_accordion_t *data =
            ui_widget_add_data(w, ui_accordion_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        ui_widget_on(w, "afterlayout", ui_accordion_on_afterlayout, NULL);
        ui_widget_on(w, "click", ui_accordion_on_click, NULL);
        ui_widget_on(w, "keydown", ui_accordion_on_keydown, NULL);
}

static void ui_accordion_item_on_init(ui_widget_t *w)
{
        ui_accordion_item_t *data =
            ui_widget_add_data(w, ui_accordion_item_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        ui_widget_add_status(w, "closed");
}

static void ui_accordion_on_set_attr(ui_widget_t *w, const char *name,
                                     const char *value)
{
        ui_accordion_t *data = ui_accordion_get_data(w);

        if (!data) {
                return;
        }
        if (strcmp(name, "multiple") == 0) {
                data->multiple = ui_accordion_value_is_true(value);
                if (!data->multiple && data->initialized) {
                        struct ui_accordion_keep_ctx ctx = { .root = w };

                        data->syncing = true;
                        ui_widget_each(w, ui_accordion_keep_first, &ctx);
                        data->syncing = false;
                        ui_accordion_sync_value(w);
                }
        } else if (strcmp(name, "value") == 0) {
                if (!data->syncing && data->initialized) {
                        ui_accordion_apply_values(w, value);
                }
        }
}

static void ui_accordion_item_on_set_attr(ui_widget_t *w, const char *name,
                                          const char *value)
{
        ui_accordion_item_t *data = ui_accordion_item_get_data(w);
        ui_widget_t *root;
        ui_accordion_t *root_data;

        if (!data || data->syncing || strcmp(name, "open") != 0) {
                return;
        }
        root = ui_accordion_find_root(w);
        root_data = root ? ui_accordion_get_data(root) : NULL;
        ui_accordion_set_item_state(w, ui_accordion_value_is_true(value),
                                    root_data && root_data->initialized);
}

ui_widget_t *ui_create_accordion(void)
{
        return ui_create_widget_with_prototype(ui_accordion_prototype);
}

ui_widget_t *ui_create_accordion_item(void)
{
        return ui_create_widget_with_prototype(ui_accordion_item_prototype);
}

ui_widget_t *ui_create_accordion_trigger(void)
{
        return ui_create_widget_with_prototype(ui_accordion_trigger_prototype);
}

ui_widget_t *ui_create_accordion_content(void)
{
        return ui_create_widget_with_prototype(ui_accordion_content_prototype);
}

void ui_accordion_set_multiple(ui_widget_t *accordion, bool multiple)
{
        ui_widget_set_attr(accordion, "multiple", multiple ? "true" : "false");
}

bool ui_accordion_get_multiple(ui_widget_t *accordion)
{
        ui_accordion_t *data = ui_accordion_get_data(accordion);

        return data ? data->multiple : false;
}

void ui_accordion_set_value(ui_widget_t *accordion, const char *values)
{
        ui_widget_set_attr(accordion, "value", values ? values : "");
}

const char *ui_accordion_get_value(ui_widget_t *accordion)
{
        const char *value = ui_widget_get_attr(accordion, "value");

        return value ? value : "";
}

struct ui_accordion_find_value_ctx {
        ui_widget_t *root;
        const char *value;
        bool found;
};

static void ui_accordion_find_value(ui_widget_t *w, void *arg)
{
        struct ui_accordion_find_value_ctx *ctx = arg;

        if (ctx->found || !ui_accordion_is_item(w) ||
            ui_accordion_find_root(w) != ctx->root) {
                return;
        }
        if (strcmp(ui_accordion_item_value(w), ctx->value) == 0 &&
            ui_accordion_item_get_open(w)) {
                ctx->found = true;
        }
}

bool ui_accordion_is_open(ui_widget_t *accordion, const char *value)
{
        struct ui_accordion_find_value_ctx ctx = {
                .root = accordion,
                .value = value ? value : "",
        };

        if (!value || !value[0]) {
                return false;
        }
        ui_widget_each(accordion, ui_accordion_find_value, &ctx);
        return ctx.found;
}

void ui_accordion_item_set_value(ui_widget_t *item, const char *value)
{
        ui_widget_set_attr(item, "value", value ? value : "");
        if (ui_accordion_item_get_open(item)) {
                ui_widget_t *root = ui_accordion_find_root(item);
                ui_accordion_t *data =
                    root ? ui_accordion_get_data(root) : NULL;

                if (data && data->initialized) {
                        ui_accordion_sync_value(root);
                }
        }
}

void ui_accordion_item_set_open(ui_widget_t *item, bool open)
{
        ui_widget_t *root = ui_accordion_find_root(item);
        ui_accordion_t *data = root ? ui_accordion_get_data(root) : NULL;

        ui_accordion_set_item_state(item, open, data && data->initialized);
}

bool ui_accordion_item_get_open(ui_widget_t *item)
{
        ui_accordion_item_t *data = ui_accordion_item_get_data(item);

        return data ? data->open : false;
}

void ui_register_accordion(void)
{
        ui_accordion_prototype = ui_create_widget_prototype("accordion", NULL);
        ui_accordion_item_prototype =
            ui_create_widget_prototype("accordion-item", NULL);
        ui_accordion_trigger_prototype =
            ui_create_widget_prototype("accordion-trigger", NULL);
        ui_accordion_content_prototype =
            ui_create_widget_prototype("accordion-content", NULL);

        ui_accordion_prototype->init = ui_accordion_on_init;
        ui_accordion_prototype->setattr = ui_accordion_on_set_attr;
        ui_accordion_item_prototype->init = ui_accordion_item_on_init;
        ui_accordion_item_prototype->setattr = ui_accordion_item_on_set_attr;
        ui_accordion_trigger_prototype->init = ui_accordion_trigger_on_init;
        ui_accordion_trigger_prototype->setattr =
            ui_accordion_trigger_on_set_attr;
        ui_accordion_trigger_prototype->update = ui_accordion_trigger_on_update;

        ui_load_css_string(ui_accordion_css, __FILE__);
}
