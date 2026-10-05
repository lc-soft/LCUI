/*
 * include/LCUI/widgets/item.h: -- LCUI's Item widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LCUI_INCLUDE_WIDGETS_ITEM_H
#define LCUI_INCLUDE_WIDGETS_ITEM_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_item_variant {
        UI_ITEM_VARIANT_DEFAULT,
        UI_ITEM_VARIANT_OUTLINE,
        UI_ITEM_VARIANT_MUTED,
} ui_item_variant_t;

typedef enum ui_item_media_variant {
        UI_ITEM_MEDIA_VARIANT_DEFAULT,
        UI_ITEM_MEDIA_VARIANT_ICON,
        UI_ITEM_MEDIA_VARIANT_IMAGE,
} ui_item_media_variant_t;

typedef enum ui_item_size {
        UI_ITEM_SIZE_DEFAULT,
        UI_ITEM_SIZE_SM,
        UI_ITEM_SIZE_XS,
} ui_item_size_t;

LCUI_API ui_widget_t *ui_create_item(void);
LCUI_API ui_widget_t *ui_create_item_media(void);
LCUI_API ui_widget_t *ui_create_item_content(void);
LCUI_API ui_widget_t *ui_create_item_title(void);
LCUI_API ui_widget_t *ui_create_item_description(void);
LCUI_API ui_widget_t *ui_create_item_actions(void);
LCUI_API void ui_item_set_variant(ui_widget_t *item, ui_item_variant_t variant);
LCUI_API void ui_item_set_size(ui_widget_t *item, ui_item_size_t size);
LCUI_API void ui_item_set_interactive(ui_widget_t *item, bool interactive);
LCUI_API bool ui_item_is_interactive(ui_widget_t *item);
LCUI_API void ui_item_media_set_variant(ui_widget_t *media,
                                        ui_item_media_variant_t variant);
LCUI_API void ui_register_item(void);

LCUI_END_HEADER

#endif
