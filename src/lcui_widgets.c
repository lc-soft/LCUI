/*
 * src/lcui_widgets.c
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI/widgets.h>
#include <ui_router.h>

void lcui_widgets_init(void)
{
        ui_register_text();
        ui_register_canvas();
        ui_register_anchor();
        ui_register_button();
        ui_register_progress();
        ui_register_scrollarea();
        ui_register_scrollbar();
        ui_register_textcaret();
        ui_register_textinput();
        ui_register_router_link();
        ui_register_router_view();
        ui_register_label();
        ui_register_checkbox();
}

void lcui_widgets_destroy(void)
{
        ui_unregister_text();
        ui_unregister_anchor();
}
