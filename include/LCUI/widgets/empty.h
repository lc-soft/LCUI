/*
 * include/LCUI/widgets/empty.h: -- LCUI's Empty widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LCUI_INCLUDE_WIDGETS_EMPTY_H
#define LCUI_INCLUDE_WIDGETS_EMPTY_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_empty_media_variant {
        UI_EMPTY_MEDIA_VARIANT_DEFAULT,
        UI_EMPTY_MEDIA_VARIANT_ICON,
} ui_empty_media_variant_t;

LCUI_API ui_widget_t *ui_create_empty(void);
LCUI_API ui_widget_t *ui_create_empty_header(void);
LCUI_API ui_widget_t *ui_create_empty_media(void);
LCUI_API ui_widget_t *ui_create_empty_title(void);
LCUI_API ui_widget_t *ui_create_empty_description(void);
LCUI_API ui_widget_t *ui_create_empty_content(void);
LCUI_API ui_widget_t *ui_create_empty_action(void);
LCUI_API void ui_empty_media_set_variant(ui_widget_t *media,
                                         ui_empty_media_variant_t variant);
LCUI_API void ui_empty_set_media(ui_widget_t *empty, const char *text);
LCUI_API void ui_empty_set_title(ui_widget_t *empty, const char *text);
LCUI_API void ui_empty_set_description(ui_widget_t *empty, const char *text);
LCUI_API ui_widget_t *ui_empty_set_action(ui_widget_t *empty,
                                          ui_widget_t *action);
LCUI_API void ui_register_empty(void);

LCUI_END_HEADER

#endif
