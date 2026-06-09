/*
 * src/lcui_window_state.c -- window state settings.
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io>
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "lcui_settings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui_server.h>
#include <LCUI/settings/window_state.h>
#include "setting_field.h"

typedef struct wstate_binding {
        ui_widget_t *widget;
        ptk_window_t *window;
        char *id;
        lcui_window_state_t state;
        bool has_state;
} wstate_binding_t;

typedef struct wstate_entry {
        char *id;
        lcui_window_state_t state;
} wstate_entry_t;

static struct wstate_module {
        bool listeners_active;
        list_t entries;
        list_t bindings;
} window_state;

static void wstate_on_widget_destroy(ui_widget_t *widget, ui_event_t *e,
                                     void *arg);
static void wstate_binding_remove(wstate_binding_t *b);

static wstate_binding_t *wstate_find_binding_by_widget(ui_widget_t *widget)
{
        list_node_t *node;

        for (list_each(node, &window_state.bindings)) {
                wstate_binding_t *b = node->data;
                if (b->widget == widget) {
                        return b;
                }
        }
        return NULL;
}

static wstate_binding_t *wstate_find_binding_by_window(ptk_window_t *window)
{
        list_node_t *node;

        for (list_each(node, &window_state.bindings)) {
                wstate_binding_t *b = node->data;
                if (b->window == window) {
                        return b;
                }
        }
        return NULL;
}

static wstate_binding_t *wstate_find_binding_by_id(const char *id)
{
        list_node_t *node;

        for (list_each(node, &window_state.bindings)) {
                wstate_binding_t *b = node->data;
                if (b->id && strcmp(b->id, id) == 0) {
                        return b;
                }
        }
        return NULL;
}

static bool wstate_id_is_valid(const char *id)
{
        const unsigned char *p;

        if (id[0] == 0) {
                return false;
        }
        for (p = (const unsigned char *)id; *p; ++p) {
                if (isalnum(*p) || *p == '.' || *p == '_' || *p == '-') {
                        continue;
                }
                return false;
        }
        return true;
}

static bool wstate_validate(const lcui_window_state_t *state)
{
        if (state->width < LCUI_WINDOW_STATE_SIZE_MIN ||
            state->width > LCUI_WINDOW_STATE_SIZE_MAX) {
                return false;
        }
        if (state->height < LCUI_WINDOW_STATE_SIZE_MIN ||
            state->height > LCUI_WINDOW_STATE_SIZE_MAX) {
                return false;
        }
        return true;
}

static bool wstate_equals(const lcui_window_state_t *a,
                          const lcui_window_state_t *b)
{
        return a->x == b->x && a->y == b->y && a->width == b->width &&
               a->height == b->height && a->maximized == b->maximized;
}

static void wstate_entry_destroy(void *data)
{
        wstate_entry_t *entry = data;

        free(entry->id);
        free(entry);
}

static void wstate_binding_destroy(void *data)
{
        wstate_binding_t *binding = data;

        free(binding->id);
        free(binding);
}

static wstate_entry_t *wstate_find_entry(const char *id)
{
        list_node_t *node;

        for (list_each(node, &window_state.entries)) {
                wstate_entry_t *entry = node->data;
                if (strcmp(entry->id, id) == 0) {
                        return entry;
                }
        }
        return NULL;
}

static bool wstate_set_entry(const char *id, const lcui_window_state_t *state)
{
        wstate_entry_t *entry;

        entry = wstate_find_entry(id);
        if (entry) {
                entry->state = *state;
                return true;
        }
        entry = malloc(sizeof(*entry));
        if (!entry) {
                return false;
        }
        entry->id = strdup2(id);
        if (!entry->id) {
                free(entry);
                return false;
        }
        entry->state = *state;
        list_append(&window_state.entries, entry);
        return true;
}

static bool wstate_query(ptk_window_t *window, lcui_window_state_t *out)
{
        ptk_window_get_position(window, &out->x, &out->y);
        out->width = ptk_window_get_width(window);
        out->height = ptk_window_get_height(window);
        out->maximized = ptk_window_is_maximized(window);
        return wstate_validate(out);
}

static bool wstate_apply(ptk_window_t *window, const lcui_window_state_t *state)
{
        if (!wstate_validate(state)) {
                return false;
        }
        ptk_window_set_position(window, state->x, state->y);
        ptk_window_set_size(window, state->width, state->height);
        ptk_window_set_maximized(window, state->maximized);
        return true;
}

static void wstate_binding_sync(wstate_binding_t *binding)
{
        if (!binding->window) {
                return;
        }
        if (!wstate_query(binding->window, &binding->state)) {
                return;
        }
        binding->has_state = true;
        if (wstate_set_entry(binding->id, &binding->state)) {
                lcui_settings_mark_dirty();
        }
        lcui_settings_flush();
}

static void wstate_binding_remove(wstate_binding_t *b)
{
        list_node_t *node;

        for (list_each(node, &window_state.bindings)) {
                if (node->data == b) {
                        list_delete_node(&window_state.bindings, node);
                        break;
                }
        }
        if (b->widget) {
                ui_widget_off(b->widget, "destroy", wstate_on_widget_destroy,
                              NULL);
        }
        wstate_binding_destroy(b);
}

static void wstate_on_widget_destroy(ui_widget_t *widget, ui_event_t *e,
                                     void *arg)
{
        wstate_binding_t *b;

        (void)e;
        (void)arg;
        b = wstate_find_binding_by_widget(widget);
        if (b) {
                wstate_binding_remove(b);
        }
}

static void wstate_on_window_create(ptk_event_t *e, void *arg)
{
        wstate_binding_t *b;
        ui_widget_t *widget;

        (void)arg;
        if (!e->window) {
                return;
        }
        widget = ui_server_get_widget(e->window);
        b = wstate_find_binding_by_widget(widget);
        if (!b) {
                return;
        }
        b->window = e->window;
        if (b->has_state) {
                wstate_apply(e->window, &b->state);
        }
}

static void wstate_on_window_close(ptk_event_t *e, void *arg)
{
        wstate_binding_t *b;

        (void)arg;
        if (!e->window) {
                return;
        }
        b = wstate_find_binding_by_window(e->window);
        if (!b) {
                return;
        }
        wstate_binding_sync(b);
        b->window = NULL;
}

static void wstate_attach_listeners(void)
{
        if (window_state.listeners_active) {
                return;
        }
        ptk_on_event(PTK_EVENT_CREATE, wstate_on_window_create, NULL);
        ptk_on_event(PTK_EVENT_CLOSE, wstate_on_window_close, NULL);
        window_state.listeners_active = true;
}

static void wstate_detach_listeners(void)
{
        if (!window_state.listeners_active) {
                return;
        }
        ptk_off_event(PTK_EVENT_CREATE, wstate_on_window_create);
        ptk_off_event(PTK_EVENT_CLOSE, wstate_on_window_close);
        window_state.listeners_active = false;
}

static void wstate_serialize_window_sections(const char *id, void *ctx)
{
        char section[256];
        ini_doc_t *doc = (ini_doc_t *)ctx;
        wstate_entry_t *entry = wstate_find_entry(id);

        if (!entry) {
                return;
        }
        snprintf(section, sizeof(section), "window.%s", id);
        ini_doc_set_int(doc, section, "x", entry->state.x);
        ini_doc_set_int(doc, section, "y", entry->state.y);
        ini_doc_set_int(doc, section, "width", entry->state.width);
        ini_doc_set_int(doc, section, "height", entry->state.height);
        ini_doc_set_bool(doc, section, "maximized", entry->state.maximized);
}

typedef struct wstate_deserialize_ctx {
        bool *dirty;
        char **invalid_sections;
        size_t invalid_count;
        size_t invalid_capacity;
} wstate_deserialize_ctx_t;

static bool wstate_add_invalid(wstate_deserialize_ctx_t *c, const char *section)
{
        char **new_arr;
        char *copy;

        if (c->invalid_count >= c->invalid_capacity) {
                size_t new_cap =
                    c->invalid_capacity ? c->invalid_capacity * 2 : 4;

                new_arr =
                    realloc(c->invalid_sections, new_cap * sizeof(char *));
                if (!new_arr) {
                        return false;
                }
                c->invalid_sections = new_arr;
                c->invalid_capacity = new_cap;
        }
        copy = strdup2(section);
        if (!copy) {
                return false;
        }
        c->invalid_sections[c->invalid_count++] = copy;
        return true;
}

static bool wstate_on_section(const char *suffix, void *ctx)
{
        wstate_deserialize_ctx_t *c = (wstate_deserialize_ctx_t *)ctx;
        lcui_window_state_t state = { 0 };
        ini_doc_t *doc = lcui_settings_get_doc();
        char section[256];

        if (!wstate_id_is_valid(suffix)) {
                *c->dirty = true;
                snprintf(section, sizeof(section), "window.%s", suffix);
                wstate_add_invalid(c, section);
                return true;
        }
        snprintf(section, sizeof(section), "window.%s", suffix);
        if (!ini_doc_get_int(doc, section, "x", &state.x) ||
            !ini_doc_get_int(doc, section, "y", &state.y) ||
            !ini_doc_get_int(doc, section, "width", &state.width) ||
            !ini_doc_get_int(doc, section, "height", &state.height) ||
            !ini_doc_get_bool(doc, section, "maximized", &state.maximized)) {
                *c->dirty = true;
                wstate_add_invalid(c, section);
                return true;
        }
        if (!wstate_validate(&state)) {
                *c->dirty = true;
                wstate_add_invalid(c, section);
                return true;
        }
        if (!wstate_set_entry(suffix, &state)) {
                return true;
        }
        return true;
}

static void wstate_deserialize(ini_doc_t *doc, bool *dirty, void *data)
{
        size_t i;
        wstate_deserialize_ctx_t ctx = { dirty, NULL, 0, 0 };

        (void)data;
        list_destroy(&window_state.entries, wstate_entry_destroy);
        list_create(&window_state.entries);
        ini_doc_foreach_section(doc, "window.", wstate_on_section, &ctx);
        for (i = 0; i < ctx.invalid_count; ++i) {
                ini_doc_remove_section(doc, ctx.invalid_sections[i]);
                free(ctx.invalid_sections[i]);
        }
        free(ctx.invalid_sections);
}

static void wstate_serialize(ini_doc_t *doc, void *data)
{
        list_node_t *node;

        (void)data;
        for (list_each(node, &window_state.entries)) {
                wstate_entry_t *entry = node->data;

                wstate_serialize_window_sections(entry->id, doc);
        }
}

bool lcui_get_window_state(const char *id, lcui_window_state_t *out)
{
        wstate_entry_t *entry;

        if (!lcui_settings_is_loaded() || !wstate_id_is_valid(id)) {
                return false;
        }
        entry = wstate_find_entry(id);
        if (!entry) {
                return false;
        }
        *out = entry->state;
        return true;
}

bool lcui_set_window_state(const char *id, const lcui_window_state_t *in)
{
        wstate_entry_t *entry;

        if (!lcui_settings_is_loaded() || !wstate_id_is_valid(id) ||
            !wstate_validate(in)) {
                return false;
        }
        entry = wstate_find_entry(id);
        if (entry && wstate_equals(&entry->state, in)) {
                return true;
        }
        if (!wstate_set_entry(id, in)) {
                return false;
        }
        lcui_settings_mark_dirty();
        return true;
}

bool lcui_bind_window_state(ui_widget_t *widget, const char *id)
{
        wstate_binding_t *binding;
        ptk_window_t *window;
        lcui_window_state_t state;

        if (!lcui_settings_is_loaded() || !wstate_id_is_valid(id)) {
                return false;
        }
        binding = wstate_find_binding_by_widget(widget);
        if (binding) {
                return strcmp(binding->id, id) == 0;
        }
        if (wstate_find_binding_by_id(id)) {
                return false;
        }
        binding = calloc(1, sizeof(*binding));
        if (!binding) {
                return false;
        }
        binding->id = strdup2(id);
        if (!binding->id) {
                free(binding);
                return false;
        }
        binding->widget = widget;
        list_append(&window_state.bindings, binding);
        ui_widget_on(widget, "destroy", wstate_on_widget_destroy, NULL);
        wstate_attach_listeners();
        if (lcui_get_window_state(id, &state)) {
                binding->state = state;
                binding->has_state = true;
                ui_widget_resize(widget, (float)state.width,
                                 (float)state.height);
        }
        window = ui_server_get_window(widget);
        if (window) {
                binding->window = window;
                if (binding->has_state) {
                        wstate_apply(window, &binding->state);
                }
        }
        return true;
}

void lcui_window_state_init(void)
{
        list_create(&window_state.entries);
        list_create(&window_state.bindings);
        lcui_settings_on_deserialize(wstate_deserialize, NULL);
        lcui_settings_on_serialize(wstate_serialize, NULL);
}

void lcui_window_state_destroy(void)
{
        wstate_detach_listeners();
        lcui_settings_off_deserialize(wstate_deserialize, NULL);
        lcui_settings_off_serialize(wstate_serialize, NULL);
        list_destroy(&window_state.bindings, wstate_binding_destroy);
        list_destroy(&window_state.entries, wstate_entry_destroy);
        window_state.listeners_active = false;
}
