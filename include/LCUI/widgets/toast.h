/*
 * include/LCUI/widgets/toast.h: -- LCUI's Toast widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_TOAST_H
#define LCUI_INCLUDE_WIDGETS_TOAST_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_toast_type {
        UI_TOAST_TYPE_DEFAULT,
        UI_TOAST_TYPE_SUCCESS,
        UI_TOAST_TYPE_INFO,
        UI_TOAST_TYPE_WARNING,
        UI_TOAST_TYPE_ERROR,
        UI_TOAST_TYPE_LOADING,
} ui_toast_type_t;

typedef void (*ui_toast_action_cb)(ui_widget_t *toast, void *arg);

#define UI_TOAST_DURATION_DEFAULT 0
#define UI_TOAST_DURATION_PERSISTENT (-1)

typedef struct ui_toast_options {
        const char *title;
        const char *description;
        const char *action_text;
        ui_toast_action_cb on_action;
        void *action_arg;
        ui_toast_type_t type;
        int duration;
} ui_toast_options_t;

/* Create a toast root with its default internal structure. The caller is
 * responsible for appending it to a toast viewport. */
LCUI_API ui_widget_t *ui_create_toast(void);

/* Show a toast in the singleton viewport, creating the viewport under ui_root()
 * on first use. Returns the toast root. */
LCUI_API ui_widget_t *ui_toast_show(const ui_toast_options_t *options);

LCUI_API ui_widget_t *ui_toast_get_viewport(void);
LCUI_API ui_widget_t *ui_toast_get_content(ui_widget_t *toast);

/* Return the action row. The row is initially hidden and becomes visible when
 * this function is called, so callers can compose arbitrary action buttons. */
LCUI_API ui_widget_t *ui_toast_get_actions(ui_widget_t *toast);

LCUI_API void ui_toast_set_title(ui_widget_t *toast, const char *text);
LCUI_API void ui_toast_set_description(ui_widget_t *toast, const char *text);
LCUI_API void ui_toast_set_type(ui_widget_t *toast, ui_toast_type_t type);
LCUI_API ui_toast_type_t ui_toast_get_type(ui_widget_t *toast);
LCUI_API void ui_toast_set_action(ui_widget_t *toast, const char *text,
                                  ui_toast_action_cb handler, void *arg);
LCUI_API void ui_toast_set_duration(ui_widget_t *toast, int duration);
LCUI_API void ui_toast_close(ui_widget_t *toast);

LCUI_API void ui_register_toast(void);

LCUI_END_HEADER

#endif
