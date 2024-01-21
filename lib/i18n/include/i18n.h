/*
 * lib/i18n/include/i18n.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBI18N_INCLUDE_I18N_H
#define LIBI18N_INCLUDE_I18N_H

/* clang-format off */

#define LIBI18N_VERSION "0.1.0-a"
#define LIBI18N_VERSION_MAJOR 0
#define LIBI18N_VERSION_MINOR 1
#define LIBI18N_VERSION_ALTER 0

#define LIBI18N_STATIC_BUILD 1
#define LIBI18N_HAS_YAML 1

/* clang-format on */

#ifndef LIBI18N_PUBLIC
#if defined(_MSC_VER) && !defined(LIBI18N_STATIC_BUILD)
#ifdef LIBI18N_DLL_EXPORT
#define LIBI18N_PUBLIC __declspec(dllexport)
#else
#define LIBI18N_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBI18N_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBI18N_PUBLIC extern
#endif
#endif

#define I18N_MAX_KEY_LEN 256

#include <wchar.h>
#include <stdbool.h>
#include <yutil.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LIBI18N_HAS_YAML

dict_t *i18n_load_yaml_file(const char *path);

#endif

LIBI18N_PUBLIC const wchar_t *i18n_translate(const wchar_t *text);

LIBI18N_PUBLIC size_t i18n_get_languages(wchar_t ***languages);

LIBI18N_PUBLIC void i18n_clear(void);

LIBI18N_PUBLIC bool i18n_load_language(const wchar_t *lang_code,
                                       const char *filename);

LIBI18N_PUBLIC bool i18n_change_language(const wchar_t *lang_code);

LIBI18N_PUBLIC size_t i18n_detect_language(wchar_t *lang, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif
