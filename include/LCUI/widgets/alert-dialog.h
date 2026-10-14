/*
 * include/LCUI/widgets/alert-dialog.h: -- AlertDialog widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_ALERT_DIALOG_H
#define LCUI_INCLUDE_WIDGETS_ALERT_DIALOG_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

/* Open/close state reuses the ui_dialog_* APIs with the alert-dialog
 * widget passed as the dialog argument. */
LCUI_API ui_widget_t *ui_create_alert_dialog(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_trigger(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_overlay(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_content(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_header(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_media(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_title(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_description(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_footer(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_cancel(void);
LCUI_API ui_widget_t *ui_create_alert_dialog_action(void);
LCUI_API void ui_alert_dialog_content_set_size(ui_widget_t *content,
                                               const char *size);
LCUI_API void ui_alert_dialog_set_size(ui_widget_t *alert, const char *size);
LCUI_API void ui_alert_dialog_set_media(ui_widget_t *alert, ui_widget_t *media);
LCUI_API void ui_register_alert_dialog(void);

LCUI_END_HEADER

#endif
