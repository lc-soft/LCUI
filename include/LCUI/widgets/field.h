/*
 * include/LCUI/widgets/field.h: -- Field widget family
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_FIELD_H
#define LCUI_INCLUDE_WIDGETS_FIELD_H

#include <LCUI/common.h>
#include <string.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_create_field(void);
LCUI_API void ui_register_field(void);

LCUI_API ui_widget_t *ui_create_field_set(void);
LCUI_API void ui_register_field_set(void);

LCUI_API ui_widget_t *ui_create_field_legend(void);
LCUI_API void ui_register_field_legend(void);

LCUI_API ui_widget_t *ui_create_field_group(void);
LCUI_API void ui_register_field_group(void);

LCUI_API ui_widget_t *ui_create_field_label(void);
LCUI_API void ui_register_field_label(void);

LCUI_API ui_widget_t *ui_create_field_description(void);
LCUI_API void ui_register_field_description(void);

LCUI_API ui_widget_t *ui_create_field_separator(void);
LCUI_API void ui_register_field_separator(void);

LCUI_API ui_widget_t *ui_create_field_content(void);
LCUI_API void ui_register_field_content(void);

LCUI_API ui_widget_t *ui_create_field_title(void);
LCUI_API void ui_register_field_title(void);

/**
 * Get the orientation attribute of a Field.
 * Returns "vertical" (default) or "horizontal".
 */
INLINE const char *ui_field_get_orientation(ui_widget_t *w)
{
        const char *v = ui_widget_get_attr(w, "orientation");
        return v ? v : "vertical";
}

/**
 * Set the orientation attribute of a Field.
 * Accepts "vertical" or "horizontal". Invalid values (NULL or any other
 * string) are rejected and the orientation attribute remains unchanged.
 * Returns 0 on success, -1 on rejection.
 */
INLINE int ui_field_set_orientation(ui_widget_t *w, const char *orientation)
{
        if (!orientation) {
                return -1;
        }
        if (strcmp(orientation, "vertical") != 0 &&
            strcmp(orientation, "horizontal") != 0) {
                return -1;
        }
        return ui_widget_set_attr(w, "orientation", orientation);
}

LCUI_END_HEADER

#endif
