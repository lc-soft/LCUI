/*
 * src/lcui_settings.h: -- internal definitions for settings infrastructure.
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_SRC_SETTINGS_H
#define LCUI_SRC_SETTINGS_H

#include <stdbool.h>
#include <yutil.h>
#include <LCUI/settings.h>
#include "ini_doc.h"

typedef void (*lcui_settings_deserialize_fn)(ini_doc_t *doc, bool *dirty,
                                             void *data);
typedef void (*lcui_settings_serialize_fn)(ini_doc_t *doc, void *data);

struct lcui_settings_module {
        bool loaded;
        bool dirty;
        bool deserialize_ready;
        bool serialize_ready;
        char *path;
        ini_doc_t *doc;
        list_t deserialize_callbacks;
        list_t serialize_callbacks;
};

extern struct lcui_settings_module lcui_settings;

bool lcui_settings_on_deserialize(lcui_settings_deserialize_fn fn, void *data);
int lcui_settings_off_deserialize(lcui_settings_deserialize_fn fn, void *data);
bool lcui_settings_on_serialize(lcui_settings_serialize_fn fn, void *data);
int lcui_settings_off_serialize(lcui_settings_serialize_fn fn, void *data);
ini_doc_t *lcui_settings_get_doc(void);
void lcui_settings_mark_dirty(void);

#endif
