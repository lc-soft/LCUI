/*
 * include/LCUI/widgets/dialog.h: -- Dialog widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_DIALOG_H
#define LCUI_INCLUDE_WIDGETS_DIALOG_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef struct ui_dialog ui_dialog_t;

LCUI_API ui_widget_t *ui_create_dialog(void);
LCUI_API ui_dialog_t *ui_dialog_get_data(ui_widget_t *dialog);
LCUI_API void ui_dialog_build_children(ui_widget_t *dialog);
LCUI_API void ui_dialog_set_open(ui_widget_t *dialog, int open);
LCUI_API void ui_dialog_open(ui_widget_t *dialog);
LCUI_API void ui_dialog_close(ui_widget_t *dialog);
LCUI_API int ui_dialog_is_open(ui_widget_t *dialog);
LCUI_API void ui_dialog_show_close_button(ui_widget_t *dialog, bool show);
LCUI_API bool ui_dialog_shows_close_button(ui_widget_t *dialog);
LCUI_API void ui_dialog_set_trigger_text(ui_widget_t *dialog, const char *text);
LCUI_API void ui_dialog_set_title(ui_widget_t *dialog, const char *text);
LCUI_API void ui_dialog_set_description(ui_widget_t *dialog, const char *text);
LCUI_API void ui_dialog_set_cancel_text(ui_widget_t *dialog, const char *text);
LCUI_API void ui_dialog_set_action_text(ui_widget_t *dialog, const char *text);
LCUI_API void ui_dialog_append_body(ui_widget_t *dialog, ui_widget_t *widget);
LCUI_API ui_widget_t *ui_dialog_get_footer_button(ui_widget_t *dialog,
                                                  bool is_action);
LCUI_API ui_widget_t *ui_dialog_get_content(ui_widget_t *dialog);
LCUI_API ui_widget_t *ui_create_dialog_trigger(void);
LCUI_API ui_widget_t *ui_create_dialog_overlay(void);
LCUI_API ui_widget_t *ui_create_dialog_content(void);
LCUI_API ui_widget_t *ui_create_dialog_header(void);
LCUI_API ui_widget_t *ui_create_dialog_title(void);
LCUI_API ui_widget_t *ui_create_dialog_description(void);
LCUI_API ui_widget_t *ui_create_dialog_footer(void);
LCUI_API ui_widget_t *ui_create_dialog_close(void);
LCUI_API void ui_register_dialog(void);
LCUI_API void ui_unregister_dialog(void);

/* Bind a widget that declares `data-dialog-trigger` to its matching loose
 * dialog. Useful for custom trigger widgets that link before the page root is
 * mounted, when the dialog's attach-time trigger scan cannot see them yet. */
LCUI_API void ui_dialog_bind_data_trigger(ui_widget_t *trigger);

LCUI_END_HEADER

#endif
