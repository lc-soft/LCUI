/*
 * src/ini_doc.h -- generic INI document.
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io>
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_SRC_INI_DOC_H
#define LCUI_SRC_INI_DOC_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ini_doc ini_doc_t;

typedef bool (*ini_doc_section_fn)(const char *suffix, void *ctx);

ini_doc_t *ini_doc_create(void);
ini_doc_t *ini_doc_load(const char *path);
bool ini_doc_save(const ini_doc_t *doc, const char *path);
void ini_doc_destroy(ini_doc_t *doc);

bool ini_doc_get_int(const ini_doc_t *doc, const char *section, const char *key,
                     int *out);
bool ini_doc_get_bool(const ini_doc_t *doc, const char *section,
                      const char *key, bool *out);
bool ini_doc_get_string(const ini_doc_t *doc, const char *section,
                        const char *key, char **out);

bool ini_doc_set_int(ini_doc_t *doc, const char *section, const char *key,
                     int value);
bool ini_doc_set_bool(ini_doc_t *doc, const char *section, const char *key,
                      bool value);
bool ini_doc_set_string(ini_doc_t *doc, const char *section, const char *key,
                        const char *value);

bool ini_doc_has_section(const ini_doc_t *doc, const char *section);
bool ini_doc_has_key(const ini_doc_t *doc, const char *section,
                     const char *key);
bool ini_doc_remove_section(ini_doc_t *doc, const char *section);
void ini_doc_foreach_section(const ini_doc_t *doc, const char *prefix,
                             ini_doc_section_fn fn, void *ctx);

#endif
