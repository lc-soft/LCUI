/*
 * tests/previews/preview_dialog.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "preview_window.h"
#include <ptk/main.h>

/* Visual diagnostics for dialog / alert-dialog: check overlay dimming,
 * content centering, Esc / overlay-click dismissal differences, default
 * close button, and focus return. */
int main(int argc, char **argv)
{
        ui_widget_t *box;
        ui_widget_t *dialog;
        ui_widget_t *alert;
        ui_widget_t *trigger;
        ui_widget_t *button;

        (void)argc;
        (void)argv;
        lcui_init();

        box = ui_create_widget(NULL);
        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "row");
        ui_widget_set_style_string(box, "gap", "12px");
        ui_widget_set_style_string(box, "padding", "24px");

        dialog = ui_create_dialog();
        ui_dialog_build_children(dialog);
        trigger = ui_widget_get_child(dialog, 0);
        button = ui_widget_get_child(trigger, 0);
        ui_button_set_text(button, "Open Dialog");

        alert = ui_create_alert_dialog();
        ui_dialog_build_children(alert);
        trigger = ui_widget_get_child(alert, 0);
        button = ui_widget_get_child(trigger, 0);
        ui_button_set_text(button, "Show Alert Dialog");

        ui_widget_append(box, dialog);
        ui_widget_append(box, alert);
        ui_widget_append(ui_root(), box);
        preview_show_default_window();
        return lcui_main();
}
