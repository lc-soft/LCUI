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
#include <ui/css.h>
#include <ui_router.h>

#include "styles/style-nova.css.h"

void lcui_widgets_init(void)
{
        ui_register_portal();
        ui_register_text();
        ui_register_canvas();
        ui_register_spinner();
        ui_register_shimmer();
        ui_register_anchor();
        ui_register_button();
        ui_register_icon();
        ui_register_scrollarea();
        ui_register_scrollbar();
        ui_register_select();
        ui_register_progress();
        ui_register_textcaret();
        ui_register_textinput();
        ui_register_router_link();
        ui_register_router_view();
        ui_register_label();
        ui_register_checkbox();
        ui_register_radio_group();
        ui_register_field();
        ui_register_field_set();
        ui_register_field_legend();
        ui_register_field_group();
        ui_register_field_label();
        ui_register_field_description();
        ui_register_field_separator();
        ui_register_field_content();
        ui_register_field_title();
        ui_register_badge();
        ui_register_card();
        ui_register_alert();
        ui_register_item();
        ui_register_attachment();
        ui_register_empty();
        ui_register_dialog();
        ui_register_alert_dialog();
        ui_register_tooltip();
        ui_register_popover();
        ui_register_toast();

        /* The theme layer loads last so its rules win over each widget's own
         * default CSS at equal specificity, letting style-nova.css refine the
         * look without editing the widget sources. */
        ui_load_css_string_style_nova_chunks();
}

void lcui_widgets_destroy(void)
{
        ui_unregister_text();
        ui_unregister_anchor();
        ui_unregister_select();
        ui_unregister_dialog();
        ui_unregister_tooltip();
        ui_unregister_popover();
}
