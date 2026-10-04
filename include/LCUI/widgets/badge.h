/*
 * include/LCUI/widgets/badge.h: -- LCUI's Badge widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_BADGE_H
#define LCUI_INCLUDE_WIDGETS_BADGE_H

#include <LCUI/common.h>
#include <css/types.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_badge_variant {
        UI_BADGE_VARIANT_DEFAULT,
        UI_BADGE_VARIANT_SECONDARY,
        UI_BADGE_VARIANT_DESTRUCTIVE,
        UI_BADGE_VARIANT_OUTLINE,
        UI_BADGE_VARIANT_GHOST,
} ui_badge_variant_t;

LCUI_API ui_widget_t *ui_create_badge(void);
LCUI_API void ui_badge_set_variant(ui_widget_t *badge,
                                   ui_badge_variant_t variant);
LCUI_API ui_badge_variant_t ui_badge_get_variant(ui_widget_t *badge);
LCUI_API void ui_badge_set_outline(ui_widget_t *badge, bool outline);
LCUI_API bool ui_badge_is_outline(ui_widget_t *badge);
LCUI_API void ui_badge_set_color(ui_widget_t *badge, const char *color);
LCUI_API void ui_badge_set_background_color(ui_widget_t *badge,
                                            css_color_value_t color);
LCUI_API void ui_badge_set_border_color(ui_widget_t *badge,
                                        css_color_value_t color);
LCUI_API void ui_register_badge(void);

LCUI_END_HEADER

#endif
