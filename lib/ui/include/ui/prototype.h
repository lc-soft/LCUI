/*
 * lib/ui/include/ui/prototype.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_PROTOTYPE_H
#define LIB_UI_INCLUDE_UI_PROTOTYPE_H

#include "common.h"
#include "types.h"

#define UI_WDIGET_ADD_DATA(WIDGET, PROTO_NAME) \
        ui_widget_add_data(WIDGET, PROTO_NAME##_proto, sizeof(PROTO_NAME##_t))

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC ui_widget_prototype_t *ui_create_widget_prototype(
    const char *name, const char *parent_name);
LIBUI_PUBLIC ui_widget_prototype_t *ui_get_widget_prototype(const char *name);
LIBUI_PUBLIC bool ui_check_widget_type(ui_widget_t *w, const char *type);
LIBUI_PUBLIC bool ui_check_widget_prototype(ui_widget_t *w,
                                            const ui_widget_prototype_t *proto);
LIBUI_PUBLIC void *ui_widget_get_data(ui_widget_t *widget,
                                      ui_widget_prototype_t *proto);
LIBUI_PUBLIC void *ui_widget_add_data(ui_widget_t *widget,
                                      ui_widget_prototype_t *proto,
                                      size_t data_size);

LIBUI_END_DECLS

#endif
