/*
 * include/LCUI/settings/base.h
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_SETTINGS_BASE_H
#define LCUI_INCLUDE_LCUI_SETTINGS_BASE_H

#include <LCUI/common.h>
#include <stdbool.h>

LCUI_BEGIN_HEADER

LCUI_API bool lcui_settings_is_loaded(void);
LCUI_API bool lcui_settings_load(void);
LCUI_API bool lcui_settings_flush(void);
LCUI_API void lcui_settings_unload(void);

/* Read / write a custom string setting under [section].  `section`
 * and `key` must be ASCII.  Returns true on read if the key exists
 * (the caller owns the returned string).  Returns true on write if
 * the value was stored (the setting is not persisted until the next
 * call to lcui_settings_flush()). */
LCUI_API bool lcui_settings_get_string(const char *section, const char *key,
                                       char **out);
LCUI_API bool lcui_settings_set_string(const char *section, const char *key,
                                       const char *value);

LCUI_END_HEADER

#endif
