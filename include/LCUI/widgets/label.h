/*
 * include/LCUI/widgets/label.h: -- LCUI's Label widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_LABEL_H
#define LCUI_INCLUDE_WIDGETS_LABEL_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_create_label(void);

LCUI_API void ui_register_label(void);

static inline int ui_label_set_for(ui_widget_t *w, const char *id)
{
        return ui_widget_set_attr(w, "for", id);
}

static inline const char *ui_label_get_for(ui_widget_t *w)
{
        return ui_widget_get_attr(w, "for");
}

LCUI_END_HEADER

#endif
