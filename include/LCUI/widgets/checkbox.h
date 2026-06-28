/*
 * include/LCUI/widgets/checkbox.h: -- LCUI's Checkbox widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_CHECKBOX_H
#define LCUI_INCLUDE_WIDGETS_CHECKBOX_H

#include <LCUI/common.h>
#include <string.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_create_checkbox(void);

LCUI_API void ui_register_checkbox(void);

static inline int ui_checkbox_set_checked(ui_widget_t *w, int checked)
{
        return ui_widget_set_attr(w, "checked", checked ? "true" : "false");
}

static inline int ui_checkbox_get_checked(ui_widget_t *w)
{
        const char *v = ui_widget_get_attr(w, "checked");

        return v && strcmp(v, "true") == 0;
}

static inline int ui_checkbox_set_indeterminate(ui_widget_t *w,
                                                int indeterminate)
{
        return ui_widget_set_attr(w, "indeterminate",
                                  indeterminate ? "true" : "false");
}

static inline int ui_checkbox_get_indeterminate(ui_widget_t *w)
{
        const char *v = ui_widget_get_attr(w, "indeterminate");

        return v && strcmp(v, "true") == 0;
}

LCUI_END_HEADER

#endif
