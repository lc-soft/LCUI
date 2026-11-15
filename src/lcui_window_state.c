/*
 * src/lcui_window_state.c: -- window state settings.
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "lcui_settings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/settings/window_state.h>
#include "setting_field.h"

typedef struct wstate_entry {
        char *id;
        lcui_window_state_t state;
} wstate_entry_t;

static struct wstate_module {
        list_t entries;
} window_state;

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
        entry->id = y_strdup(id);
        if (!entry->id) {
                free(entry);
                return false;
        }
        entry->state = *state;
        list_append(&window_state.entries, entry);
        return true;
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
        copy = y_strdup(section);
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

void lcui_window_state_init(void)
{
        list_create(&window_state.entries);
        lcui_settings_on_deserialize(wstate_deserialize, NULL);
        lcui_settings_on_serialize(wstate_serialize, NULL);
}

void lcui_window_state_destroy(void)
{
        lcui_settings_off_deserialize(wstate_deserialize, NULL);
        lcui_settings_off_serialize(wstate_serialize, NULL);
        list_destroy(&window_state.entries, wstate_entry_destroy);
}
