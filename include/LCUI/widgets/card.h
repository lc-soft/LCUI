/*
 * include/LCUI/widgets/card.h: -- LCUI's Card widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_CARD_H
#define LCUI_INCLUDE_WIDGETS_CARD_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_card_size {
        UI_CARD_SIZE_DEFAULT,
        UI_CARD_SIZE_SM,
} ui_card_size_t;

LCUI_API ui_widget_t *ui_create_card(void);
LCUI_API ui_widget_t *ui_create_card_header(void);
LCUI_API ui_widget_t *ui_create_card_title(void);
LCUI_API ui_widget_t *ui_create_card_description(void);
LCUI_API ui_widget_t *ui_create_card_action(void);
LCUI_API ui_widget_t *ui_create_card_content(void);
LCUI_API ui_widget_t *ui_create_card_footer(void);

LCUI_API void ui_card_set_size(ui_widget_t *card, ui_card_size_t size);
LCUI_API ui_card_size_t ui_card_get_size(ui_widget_t *card);
LCUI_API void ui_register_card(void);

LCUI_END_HEADER

#endif
