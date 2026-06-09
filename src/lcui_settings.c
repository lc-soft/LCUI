/*
 * src/lcui_settings.c -- global settings infrastructure.
 *
 * Copyright (c) 2020, James Duong <duong.james@gmail.com>
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io>
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yutil.h>
#ifndef _WIN32
#include <strings.h>
#endif
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <LCUI/app.h>
#include "lcui_settings.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

typedef struct lcui_settings_callback_entry {
        void *fn;
        void *data;
} lcui_settings_callback_entry_t;

struct lcui_settings_module lcui_settings;

static void lcui_settings_callback_entry_destroy(void *data)
{
        free(data);
}

static bool lcui_settings_ensure_deserialize_list(void)
{
        if (lcui_settings.deserialize_ready) {
                return true;
        }
        list_create(&lcui_settings.deserialize_callbacks);
        lcui_settings.deserialize_ready = true;
        return true;
}

static bool lcui_settings_ensure_serialize_list(void)
{
        if (lcui_settings.serialize_ready) {
                return true;
        }
        list_create(&lcui_settings.serialize_callbacks);
        lcui_settings.serialize_ready = true;
        return true;
}

static void lcui_settings_run_deserializers(void)
{
        list_node_t *node;
        bool dirty = false;

        if (!lcui_settings.deserialize_ready || !lcui_settings.doc) {
                return;
        }
        for (list_each(node, &lcui_settings.deserialize_callbacks)) {
                lcui_settings_callback_entry_t *entry = node->data;
                ((lcui_settings_deserialize_fn)entry->fn)(lcui_settings.doc,
                                                          &dirty, entry->data);
        }
        if (dirty) {
                lcui_settings.dirty = true;
        }
}

static void lcui_settings_run_serializers(void)
{
        list_node_t *node;

        if (!lcui_settings.serialize_ready || !lcui_settings.doc) {
                return;
        }
        for (list_each(node, &lcui_settings.serialize_callbacks)) {
                lcui_settings_callback_entry_t *entry = node->data;
                ((lcui_settings_serialize_fn)entry->fn)(lcui_settings.doc,
                                                        entry->data);
        }
}

static int lcui_mkdir(const char *path)
{
#ifdef _WIN32
        return _mkdir(path);
#else
        return mkdir(path, 0700);
#endif
}

static bool lcui_mkdir_recursive(const char *path)
{
        size_t i, len;
        char ch;
        char *buffer;

        len = strlen(path);
        buffer = strdup2(path);
        if (!buffer) {
                return false;
        }
        for (i = 1; i <= len; ++i) {
                if (buffer[i] != '/' && buffer[i] != '\\' && buffer[i] != 0) {
                        continue;
                }
                ch = buffer[i];
                if (ch != 0) {
                        buffer[i] = 0;
                }
                if (buffer[0] == 0) {
                        if (ch != 0) {
                                buffer[i] = ch;
                        }
                        continue;
                }
#ifdef _WIN32
                if (isalpha((unsigned char)buffer[0]) && buffer[1] == ':' &&
                    buffer[2] == 0) {
                        if (ch != 0) {
                                buffer[i] = ch;
                        }
                        continue;
                }
#endif
                if (lcui_mkdir(buffer) != 0 && errno != EEXIST) {
                        free(buffer);
                        return false;
                }
                if (ch != 0) {
                        buffer[i] = ch;
                }
        }
        free(buffer);
        return true;
}

static char *lcui_join_path(const char *left, const char *right)
{
        size_t left_len, right_len;
        char *path;

        left_len = strlen(left);
        right_len = strlen(right);
        path = malloc(sizeof(char) * (left_len + right_len + 2));
        if (!path) {
                return NULL;
        }
        strcpy(path, left);
        if (left_len > 0 && path[left_len - 1] != '/' &&
            path[left_len - 1] != '\\') {
                path[left_len] = '/';
                path[left_len + 1] = 0;
        }
        strcat(path, right);
        return path;
}

static char *lcui_settings_get_config_root(void)
{
#ifdef _WIN32
        const char *base = getenv("APPDATA");
        if (base && base[0] != 0) {
                return strdup2(base);
        }
        base = getenv("USERPROFILE");
        if (base && base[0] != 0) {
                return lcui_join_path(base, "AppData/Roaming");
        }
#elif defined(__APPLE__)
        const char *base = getenv("HOME");
        if (base && base[0] != 0) {
                return lcui_join_path(base, "Library/Application Support");
        }
#else
        const char *base = getenv("XDG_CONFIG_HOME");
        if (base && base[0] != 0) {
                return strdup2(base);
        }
        base = getenv("HOME");
        if (base && base[0] != 0) {
                return lcui_join_path(base, ".config");
        }
#endif
        return NULL;
}

static bool lcui_settings_resolve_path(char **out_path)
{
        char *config_root, *app_dir, *settings_path;
        const char *app_id = lcui_get_app_id();

        if (!app_id || app_id[0] == 0) {
                return false;
        }
        config_root = lcui_settings_get_config_root();
        if (!config_root) {
                return false;
        }
        app_dir = lcui_join_path(config_root, app_id);
        free(config_root);
        if (!app_dir) {
                return false;
        }
        if (!lcui_mkdir_recursive(app_dir)) {
                free(app_dir);
                return false;
        }
        settings_path = lcui_join_path(app_dir, "settings.ini");
        free(app_dir);
        if (!settings_path) {
                return false;
        }
        *out_path = settings_path;
        return true;
}

bool lcui_settings_on_deserialize(lcui_settings_deserialize_fn fn, void *data)
{
        lcui_settings_callback_entry_t *entry;
        list_node_t *node;

        if (!lcui_settings_ensure_deserialize_list()) {
                return false;
        }
        for (list_each(node, &lcui_settings.deserialize_callbacks)) {
                entry = node->data;
                if (entry->fn == (void *)fn) {
                        return true;
                }
        }
        entry = malloc(sizeof(*entry));
        if (!entry) {
                return false;
        }
        entry->fn = (void *)fn;
        entry->data = data;
        list_append(&lcui_settings.deserialize_callbacks, entry);
        return true;
}

int lcui_settings_off_deserialize(lcui_settings_deserialize_fn fn, void *data)
{
        int count = 0;
        list_node_t *node, *next;

        if (!lcui_settings.deserialize_ready) {
                return 0;
        }
        for (node = lcui_settings.deserialize_callbacks.head.next; node;
             node = next) {
                next = node->next;
                {
                        lcui_settings_callback_entry_t *entry = node->data;
                        if (entry->fn == (void *)fn) {
                                list_delete_node(
                                    &lcui_settings.deserialize_callbacks, node);
                                lcui_settings_callback_entry_destroy(entry);
                                ++count;
                        }
                }
        }
        return count;
}

bool lcui_settings_on_serialize(lcui_settings_serialize_fn fn, void *data)
{
        lcui_settings_callback_entry_t *entry;
        list_node_t *node;

        if (!lcui_settings_ensure_serialize_list()) {
                return false;
        }
        for (list_each(node, &lcui_settings.serialize_callbacks)) {
                entry = node->data;
                if (entry->fn == (void *)fn) {
                        return true;
                }
        }
        entry = malloc(sizeof(*entry));
        if (!entry) {
                return false;
        }
        entry->fn = (void *)fn;
        entry->data = data;
        list_append(&lcui_settings.serialize_callbacks, entry);
        return true;
}

int lcui_settings_off_serialize(lcui_settings_serialize_fn fn, void *data)
{
        int count = 0;
        list_node_t *node, *next;

        if (!lcui_settings.serialize_ready) {
                return 0;
        }
        for (node = lcui_settings.serialize_callbacks.head.next; node;
             node = next) {
                next = node->next;
                {
                        lcui_settings_callback_entry_t *entry = node->data;
                        if (entry->fn == (void *)fn) {
                                list_delete_node(
                                    &lcui_settings.serialize_callbacks, node);
                                lcui_settings_callback_entry_destroy(entry);
                                ++count;
                        }
                }
        }
        return count;
}

ini_doc_t *lcui_settings_get_doc(void)
{
        return lcui_settings.doc;
}

void lcui_settings_mark_dirty(void)
{
        lcui_settings.dirty = true;
}

bool lcui_settings_is_loaded(void)
{
        return lcui_settings.loaded;
}

bool lcui_settings_load(void)
{
        bool has_version = false;
        char *path;
        ini_doc_t *doc;
        int version = 0;

        if (lcui_settings.loaded) {
                return true;
        }
        if (!lcui_settings_resolve_path(&path)) {
                return false;
        }
        doc = ini_doc_load(path);
        if (!doc) {
                free(path);
                return false;
        }
        if (ini_doc_get_int(doc, "meta", "version", &version)) {
                has_version = (version == 1);
        }
        if (!has_version) {
                ini_doc_set_int(doc, "meta", "version", 1);
                lcui_settings.dirty = true;
        }
        lcui_settings.doc = doc;
        lcui_settings.path = path;
        lcui_settings.loaded = true;
        lcui_settings_run_deserializers();
        if (!has_version) {
                lcui_settings.dirty = true;
        }
        return true;
}

bool lcui_settings_flush(void)
{
        if (!lcui_settings.loaded || !lcui_settings.path) {
                return false;
        }
        if (!lcui_settings.dirty) {
                return true;
        }
        if (!lcui_settings.doc) {
                return false;
        }
        lcui_settings_run_serializers();
        if (!ini_doc_save(lcui_settings.doc, lcui_settings.path)) {
                return false;
        }
        lcui_settings.dirty = false;
        return true;
}

void lcui_settings_unload(void)
{
        if (lcui_settings.doc) {
                ini_doc_destroy(lcui_settings.doc);
                lcui_settings.doc = NULL;
        }
        free(lcui_settings.path);
        lcui_settings.path = NULL;
        lcui_settings.loaded = false;
        lcui_settings.dirty = false;
}
