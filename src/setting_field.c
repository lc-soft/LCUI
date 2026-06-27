/*
 * src/setting_field.c: -- schema-driven (de)serialization.
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>

#include "setting_field.h"

bool setting_fields_deserialize(ini_doc_t *doc, const char *section,
                                const setting_field_def_t *fields,
                                size_t field_count, void *out, bool *dirty)
{
        size_t i;
        unsigned char *base = (unsigned char *)out;

        for (i = 0; i < field_count; ++i) {
                const setting_field_def_t *f = &fields[i];

                if (f->type == SETTING_FIELD_TYPE_INT) {
                        int value;

                        if (!ini_doc_get_int(doc, section, f->name, &value)) {
                                int def = f->int_default;

                                memcpy(base + f->offset, &def, sizeof(def));
                                *dirty = true;
                                continue;
                        }
                        if (value < f->int_min || value > f->int_max) {
                                int def = f->int_default;

                                memcpy(base + f->offset, &def, sizeof(def));
                                *dirty = true;
                                continue;
                        }
                        memcpy(base + f->offset, &value, sizeof(value));
                } else if (f->type == SETTING_FIELD_TYPE_BOOL) {
                        bool value;

                        if (!ini_doc_get_bool(doc, section, f->name, &value)) {
                                memcpy(base + f->offset, &f->bool_default,
                                       sizeof(f->bool_default));
                                *dirty = true;
                                continue;
                        }
                        memcpy(base + f->offset, &value, sizeof(value));
                }
        }
        return true;
}

bool setting_fields_serialize(ini_doc_t *doc, const char *section,
                              const setting_field_def_t *fields,
                              size_t field_count, const void *src)
{
        size_t i;
        const unsigned char *base = (const unsigned char *)src;

        for (i = 0; i < field_count; ++i) {
                const setting_field_def_t *f = &fields[i];

                if (f->type == SETTING_FIELD_TYPE_INT) {
                        int value;

                        memcpy(&value, base + f->offset, sizeof(value));
                        if (!ini_doc_set_int(doc, section, f->name, value)) {
                                return false;
                        }
                } else if (f->type == SETTING_FIELD_TYPE_BOOL) {
                        bool value;

                        memcpy(&value, base + f->offset, sizeof(value));
                        if (!ini_doc_set_bool(doc, section, f->name, value)) {
                                return false;
                        }
                }
        }
        return true;
}
