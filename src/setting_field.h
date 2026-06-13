/*
 * src/setting_field.h: -- schema-driven field definitions for settings.
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.

 * *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI,
 * distributed under the MIT License found in the
 * LICENSE.TXT file in the
 * root directory of this source tree.
 */

#ifndef LCUI_SRC_SETTING_FIELD_H
#define LCUI_SRC_SETTING_FIELD_H

#include <stdbool.h>
#include <stddef.h>
#include "ini_doc.h"

typedef enum {
        SETTING_FIELD_TYPE_INT,
        SETTING_FIELD_TYPE_BOOL
} setting_field_type_t;

typedef struct setting_field_def {
        const char *name;
        setting_field_type_t type;
        size_t offset;
        int int_default;
        int int_min;
        int int_max;
        bool bool_default;
} setting_field_def_t;

#define DEFINE_SETTING_INT_FIELD(struct_type, name, def, min, max) \
        { #name,                                                   \
          SETTING_FIELD_TYPE_INT,                                  \
          offsetof(struct_type, name),                             \
          (def),                                                   \
          (min),                                                   \
          (max),                                                   \
          false }

#define DEFINE_SETTING_BOOL_FIELD(struct_type, name, def) \
        {                                                 \
                #name,                                    \
                SETTING_FIELD_TYPE_BOOL,                  \
                offsetof(struct_type, name),              \
                0,                                        \
                0,                                        \
                0,                                        \
                (def)                                     \
        }

bool setting_fields_deserialize(ini_doc_t *doc, const char *section,
                                const setting_field_def_t *fields,
                                size_t field_count, void *out, bool *dirty);

bool setting_fields_serialize(ini_doc_t *doc, const char *section,
                              const setting_field_def_t *fields,
                              size_t field_count, const void *src);

#endif
