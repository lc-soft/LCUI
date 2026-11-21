/*
 * lib/ui/include/ui/theme.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_THEME_H
#define LIB_UI_INCLUDE_UI_THEME_H

#include <pandagl.h>
#include "common.h"

LIBUI_BEGIN_DECLS

typedef enum ui_theme_token {
        UI_THEME_TOKEN_PRIMARY,
        UI_THEME_TOKEN_PRIMARY_FOREGROUND,
        UI_THEME_TOKEN_ACCENT,
        UI_THEME_TOKEN_ACCENT_FOREGROUND,
        UI_THEME_TOKEN_SIDEBAR_PRIMARY,
        UI_THEME_TOKEN_SIDEBAR_PRIMARY_FOREGROUND,
        UI_THEME_TOKEN_SIDEBAR_ACCENT,
        UI_THEME_TOKEN_SIDEBAR_ACCENT_FOREGROUND,
} ui_theme_token_t;

typedef struct ui_theme_accent {
        const char *name;
        pd_color_t light;
        pd_color_t dark;
} ui_theme_accent_t;

typedef struct ui_theme_rule {
        const char *selector;
        const char *property;
        ui_theme_token_t token;
        int dark;
        int alpha;
} ui_theme_rule_t;

/**
 * Set the accent to a preset name.
 *
 * Passing NULL or "default" removes the accent class and restores the
 * style-nova colors.
 */
LIBUI_PUBLIC int ui_theme_set_accent(const char *name);

/**
 * Set the accent from a custom #RRGGBB color.
 *
 * The light theme uses the color as is; the dark theme mixes it with white
 * by 20% so it stays visible on dark surfaces.
 */
LIBUI_PUBLIC int ui_theme_set_accent_color(const char *hex);

/** Get the current accent name, or the custom hex string. */
LIBUI_PUBLIC const char *ui_theme_get_accent(void);

LIBUI_END_DECLS
#endif
