/*
 * include/LCUI/widgets/button.h: -- LCUI‘s Button widget
 *
 * Copyright (c) 2018-2026, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_BUTTON_H
#define LCUI_INCLUDE_WIDGETS_BUTTON_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

typedef enum ui_button_variant {
        UI_BUTTON_VARIANT_OUTLINE,
        UI_BUTTON_VARIANT_PRIMARY,
        UI_BUTTON_VARIANT_SECONDARY,
        UI_BUTTON_VARIANT_GHOST,
        UI_BUTTON_VARIANT_DESTRUCTIVE,
        UI_BUTTON_VARIANT_LINK,
} ui_button_variant_t;

typedef enum ui_button_size {
        UI_BUTTON_SIZE_DEFAULT,
        UI_BUTTON_SIZE_XS,
        UI_BUTTON_SIZE_SM,
        UI_BUTTON_SIZE_LG,
        UI_BUTTON_SIZE_ICON,
        UI_BUTTON_SIZE_ICON_XS,
        UI_BUTTON_SIZE_ICON_SM,
        UI_BUTTON_SIZE_ICON_LG,
} ui_button_size_t;

LCUI_API ui_widget_t *ui_create_button(void);

LCUI_API void ui_button_set_variant(ui_widget_t *button,
                                    ui_button_variant_t variant);

LCUI_API ui_button_variant_t ui_button_get_variant(ui_widget_t *button);

LCUI_API void ui_button_set_size(ui_widget_t *button, ui_button_size_t size);

LCUI_API ui_button_size_t ui_button_get_size(ui_widget_t *button);

LCUI_API void ui_button_set_text_w(ui_widget_t *w, const wchar_t *wstr);

LCUI_API void ui_button_set_text(ui_widget_t *w, const char *str);

LCUI_API void ui_register_button(void);

LCUI_END_HEADER

#endif
