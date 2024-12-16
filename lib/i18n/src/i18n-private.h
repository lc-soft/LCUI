/*
 * lib/i18n/src/i18n-private.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <i18n.h>
#include <yutil.h>
#include "i18n.h"

typedef enum { NONE, STRING, DICT } dict_value_type_t;

typedef struct dict_string_value {
        wchar_t *data;
        size_t length;
} dict_string_value_t;

typedef struct dict_value dict_value_t;

struct dict_value {
        dict_value_type_t type;
        union {
                dict_string_value_t string;
                dict_t *dict;
        };
        wchar_t *key;
        dict_t *parent_dict;
        dict_value_t *parent_value;
};

/** dict_t<wchar_t*, dict_value_t*> */
dict_t *i18n_dict_create(void);

bool i18n_dict_add_dict(dict_t *dict, const wchar_t *key, dict_t *child_dict);

dict_t *i18n_load_yaml_file(const char *path);
