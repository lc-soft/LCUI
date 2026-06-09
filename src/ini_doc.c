/*
 * src/ini_doc.c -- generic INI document.
 *
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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yutil.h>
#ifndef _WIN32
#include <strings.h>
#endif

#include "ini_doc.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

typedef struct ini_entry {
        char *key;
        char *value;
} ini_entry_t;

typedef struct ini_section {
        char *name;
        ini_entry_t *entries;
        size_t entry_count;
        size_t entry_capacity;
} ini_section_t;

struct ini_doc {
        ini_section_t *sections;
        size_t section_count;
        size_t section_capacity;
};

static ini_section_t *ini_doc_find_section(ini_doc_t *doc, const char *name,
                                           bool create)
{
        size_t i;
        ini_section_t *section;

        for (i = 0; i < doc->section_count; ++i) {
                if (strcasecmp(doc->sections[i].name, name) == 0) {
                        return &doc->sections[i];
                }
        }
        if (!create) {
                return NULL;
        }
        if (doc->section_count >= doc->section_capacity) {
                size_t cap =
                    doc->section_capacity ? doc->section_capacity * 2 : 8;
                ini_section_t *s;

                s = realloc(doc->sections, cap * sizeof(ini_section_t));
                if (!s) {
                        return NULL;
                }
                doc->sections = s;
                doc->section_capacity = cap;
        }
        section = &doc->sections[doc->section_count];
        memset(section, 0, sizeof(*section));
        section->name = strdup2(name);
        if (!section->name) {
                return NULL;
        }
        doc->section_count++;
        return section;
}

static const ini_section_t *ini_doc_find_section_const(const ini_doc_t *doc,
                                                       const char *name)
{
        size_t i;

        for (i = 0; i < doc->section_count; ++i) {
                if (strcasecmp(doc->sections[i].name, name) == 0) {
                        return &doc->sections[i];
                }
        }
        return NULL;
}

static ini_entry_t *ini_section_find_entry(ini_section_t *section,
                                           const char *key)
{
        size_t i;

        for (i = 0; i < section->entry_count; ++i) {
                if (strcasecmp(section->entries[i].key, key) == 0) {
                        return &section->entries[i];
                }
        }
        return NULL;
}

static const ini_entry_t *ini_section_find_entry_const(
    const ini_section_t *section, const char *key)
{
        size_t i;

        for (i = 0; i < section->entry_count; ++i) {
                if (strcasecmp(section->entries[i].key, key) == 0) {
                        return &section->entries[i];
                }
        }
        return NULL;
}

static ini_entry_t *ini_section_ensure_entry(ini_section_t *section,
                                             const char *key)
{
        ini_entry_t *entry = ini_section_find_entry(section, key);

        if (entry) {
                return entry;
        }
        if (section->entry_count >= section->entry_capacity) {
                size_t cap =
                    section->entry_capacity ? section->entry_capacity * 2 : 4;
                ini_entry_t *e;

                e = realloc(section->entries, cap * sizeof(ini_entry_t));
                if (!e) {
                        return NULL;
                }
                section->entries = e;
                section->entry_capacity = cap;
        }
        entry = &section->entries[section->entry_count];
        memset(entry, 0, sizeof(*entry));
        entry->key = strdup2(key);
        if (!entry->key) {
                return NULL;
        }
        section->entry_count++;
        return entry;
}

static bool ini_entry_set_value(ini_entry_t *entry, const char *value)
{
        char *copy = strdup2(value);

        if (!copy) {
                return false;
        }
        free(entry->value);
        entry->value = copy;
        return true;
}

static char *ini_trim(char *str)
{
        size_t len;

        while (*str && isspace((unsigned char)*str)) {
                ++str;
        }
        len = strlen(str);
        while (len > 0 && isspace((unsigned char)str[len - 1])) {
                str[--len] = 0;
        }
        return str;
}

static void ini_section_destroy(ini_section_t *section)
{
        size_t i;

        for (i = 0; i < section->entry_count; ++i) {
                free(section->entries[i].key);
                free(section->entries[i].value);
        }
        free(section->entries);
        free(section->name);
        memset(section, 0, sizeof(*section));
}

ini_doc_t *ini_doc_create(void)
{
        ini_doc_t *doc = calloc(1, sizeof(ini_doc_t));

        return doc;
}

ini_doc_t *ini_doc_load(const char *path)
{
        char line[1024];
        char section[256] = { 0 };
        FILE *fp;
        ini_doc_t *doc;

        fp = fopen(path, "r");
        if (!fp) {
                if (errno == ENOENT) {
                        return ini_doc_create();
                }
                return NULL;
        }
        doc = ini_doc_create();
        if (!doc) {
                fclose(fp);
                return NULL;
        }
        while (fgets(line, sizeof(line), fp)) {
                char *data, *value, *end;

                data = ini_trim(line);
                if (data[0] == 0 || data[0] == ';' || data[0] == '#') {
                        continue;
                }
                if (data[0] == '[') {
                        end = strchr(data, ']');
                        if (!end) {
                                continue;
                        }
                        *end = 0;
                        snprintf(section, sizeof(section), "%s", data + 1);
                        continue;
                }
                value = strchr(data, '=');
                if (!value || section[0] == 0) {
                        continue;
                }
                *value++ = 0;
                data = ini_trim(data);
                value = ini_trim(value);
                if (!ini_doc_set_string(doc, section, data, value)) {
                        ini_doc_destroy(doc);
                        fclose(fp);
                        return NULL;
                }
        }
        fclose(fp);
        return doc;
}

bool ini_doc_save(const ini_doc_t *doc, const char *path)
{
        size_t i, j, path_len;
        char *backup_path = NULL;
        char *tmp_path;
        FILE *fp;
        int ret;
        bool first_section = true;

        path_len = strlen(path);
        tmp_path = malloc(sizeof(char) * (path_len + 5));
        if (!tmp_path) {
                return false;
        }
        strcpy(tmp_path, path);
        strcat(tmp_path, ".tmp");
        fp = fopen(tmp_path, "w");
        if (!fp) {
                free(tmp_path);
                return false;
        }
        for (i = 0; i < doc->section_count; ++i) {
                const ini_section_t *s = &doc->sections[i];

                if (!first_section) {
                        fprintf(fp, "\n");
                }
                fprintf(fp, "[%s]\n", s->name);
                for (j = 0; j < s->entry_count; ++j) {
                        fprintf(fp, "%s=%s\n", s->entries[j].key,
                                s->entries[j].value);
                }
                first_section = false;
        }
        fclose(fp);
#ifdef _WIN32
        backup_path = malloc(sizeof(char) * (path_len + 5));
        if (!backup_path) {
                remove(tmp_path);
                free(tmp_path);
                return false;
        }
        snprintf(backup_path, path_len + 5, "%s.bak", path);
        remove(backup_path);
        ret = rename(path, backup_path);
        if (ret != 0 && errno != ENOENT) {
                remove(tmp_path);
                free(backup_path);
                free(tmp_path);
                return false;
        }
        ret = rename(tmp_path, path);
        if (ret == 0) {
                remove(backup_path);
        } else if (rename(backup_path, path) != 0) {
                remove(backup_path);
        }
#else
        ret = rename(tmp_path, path);
#endif
        free(backup_path);
        free(tmp_path);
        return ret == 0;
}

void ini_doc_destroy(ini_doc_t *doc)
{
        size_t i;

        for (i = 0; i < doc->section_count; ++i) {
                ini_section_destroy(&doc->sections[i]);
        }
        free(doc->sections);
        free(doc);
}

bool ini_doc_get_int(const ini_doc_t *doc, const char *section, const char *key,
                     int *out)
{
        long val;
        char *endptr;
        const ini_section_t *s;
        const ini_entry_t *e;

        s = ini_doc_find_section_const(doc, section);
        if (!s) {
                return false;
        }
        e = ini_section_find_entry_const(s, key);
        if (!e || !e->value) {
                return false;
        }
        errno = 0;
        val = strtol(e->value, &endptr, 10);
        if (errno != 0 || endptr == e->value || *endptr != 0 || val > INT_MAX ||
            val < INT_MIN) {
                return false;
        }
        *out = (int)val;
        return true;
}

bool ini_doc_get_bool(const ini_doc_t *doc, const char *section,
                      const char *key, bool *out)
{
        const ini_section_t *s;
        const ini_entry_t *e;

        s = ini_doc_find_section_const(doc, section);
        if (!s) {
                return false;
        }
        e = ini_section_find_entry_const(s, key);
        if (!e || !e->value) {
                return false;
        }
        if (strcmp(e->value, "1") == 0 || strcasecmp(e->value, "true") == 0) {
                *out = true;
                return true;
        }
        if (strcmp(e->value, "0") == 0 || strcasecmp(e->value, "false") == 0) {
                *out = false;
                return true;
        }
        return false;
}

bool ini_doc_get_string(const ini_doc_t *doc, const char *section,
                        const char *key, char **out)
{
        const ini_section_t *s;
        const ini_entry_t *e;

        s = ini_doc_find_section_const(doc, section);
        if (!s) {
                return false;
        }
        e = ini_section_find_entry_const(s, key);
        if (!e || !e->value) {
                return false;
        }
        *out = strdup2(e->value);
        return *out != NULL;
}

bool ini_doc_set_int(ini_doc_t *doc, const char *section, const char *key,
                     int value)
{
        char buf[32];

        snprintf(buf, sizeof(buf), "%d", value);
        return ini_doc_set_string(doc, section, key, buf);
}

bool ini_doc_set_bool(ini_doc_t *doc, const char *section, const char *key,
                      bool value)
{
        return ini_doc_set_string(doc, section, key, value ? "1" : "0");
}

bool ini_doc_set_string(ini_doc_t *doc, const char *section, const char *key,
                        const char *value)
{
        ini_section_t *s;
        ini_entry_t *e;

        s = ini_doc_find_section(doc, section, true);
        if (!s) {
                return false;
        }
        e = ini_section_ensure_entry(s, key);
        if (!e) {
                return false;
        }
        return ini_entry_set_value(e, value);
}

bool ini_doc_has_section(const ini_doc_t *doc, const char *section)
{
        return ini_doc_find_section_const(doc, section) != NULL;
}

bool ini_doc_remove_section(ini_doc_t *doc, const char *section)
{
        size_t i;
        ini_section_t *s;

        for (i = 0; i < doc->section_count; ++i) {
                if (strcasecmp(doc->sections[i].name, section) == 0) {
                        s = &doc->sections[i];
                        ini_section_destroy(s);
                        if (i + 1 < doc->section_count) {
                                memmove(&doc->sections[i],
                                        &doc->sections[i + 1],
                                        (doc->section_count - i - 1) *
                                            sizeof(ini_section_t));
                        }
                        doc->section_count--;
                        return true;
                }
        }
        return false;
}

bool ini_doc_has_key(const ini_doc_t *doc, const char *section, const char *key)
{
        const ini_section_t *s;

        s = ini_doc_find_section_const(doc, section);
        if (!s) {
                return false;
        }
        return ini_section_find_entry_const(s, key) != NULL;
}

void ini_doc_foreach_section(const ini_doc_t *doc, const char *prefix,
                             ini_doc_section_fn fn, void *ctx)
{
        size_t i, prefix_len;

        prefix_len = strlen(prefix);
        for (i = 0; i < doc->section_count; ++i) {
                const char *name = doc->sections[i].name;

                if (strncasecmp(name, prefix, prefix_len) == 0) {
                        if (!fn(name + prefix_len, ctx)) {
                                return;
                        }
                }
        }
}
