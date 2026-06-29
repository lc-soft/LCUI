/*
 * include/LCUI/widgets/radio_group.h: -- RadioGroup and RadioGroupItem widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_RADIO_GROUP_H
#define LCUI_INCLUDE_WIDGETS_RADIO_GROUP_H

#include <LCUI/common.h>
#include <string.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_create_radio_group(void);

LCUI_API ui_widget_t *ui_create_radio_group_item(void);

LCUI_API void ui_register_radio_group(void);

/**
 * Sync item checked states with the group's current value.
 *
 * Walks all radio-group-item descendants of w and sets checked=1 on the
 * item whose "value" attribute equals w's "value", others to checked=0.
 *
 * Called automatically by ui_radio_group_set_value(). App code may also
 * call it directly after appending new items at runtime to refresh the
 * visual selection without changing the group's value.
 */
LCUI_API void ui_radio_group_update(ui_widget_t *w);

INLINE const char *ui_radio_group_get_value(ui_widget_t *w)
{
        return ui_widget_get_attr(w, "value");
}

INLINE int ui_radio_group_set_value(ui_widget_t *w, const char *value)
{
        if (ui_widget_set_attr(w, "value", value) < 0) {
                return -1;
        }
        ui_radio_group_update(w);
        return 0;
}

INLINE int ui_radio_group_item_get_checked(ui_widget_t *w)
{
        const char *v = ui_widget_get_attr(w, "checked");

        return v && strcmp(v, "true") == 0;
}

INLINE int ui_radio_group_item_set_checked(ui_widget_t *w, int checked)
{
        return ui_widget_set_attr(w, "checked", checked ? "true" : "false");
}

LCUI_END_HEADER

#endif
