/*
 * include/LCUI/widgets/breadcrumb.h: -- LCUI's Breadcrumb widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_BREADCRUMB_H
#define LCUI_INCLUDE_WIDGETS_BREADCRUMB_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef const char *(*ui_breadcrumb_label_provider_t)(const char *segment,
                                                      const char *path,
                                                      void *arg);

LCUI_API ui_widget_t *ui_create_breadcrumb(void);
LCUI_API ui_widget_t *ui_create_breadcrumb_list(void);
LCUI_API ui_widget_t *ui_create_breadcrumb_item(void);
LCUI_API ui_widget_t *ui_create_breadcrumb_link(void);
LCUI_API ui_widget_t *ui_create_breadcrumb_page(void);
LCUI_API ui_widget_t *ui_create_breadcrumb_separator(void);
LCUI_API ui_widget_t *ui_create_breadcrumb_ellipsis(void);

LCUI_API void ui_breadcrumb_append_item(ui_widget_t *breadcrumb,
                                        const char *key, const char *label);
LCUI_API void ui_breadcrumb_append(ui_widget_t *breadcrumb,
                                   ui_widget_t *content);
LCUI_API void ui_breadcrumb_build_from_path(
    ui_widget_t *breadcrumb, const char *path,
    ui_breadcrumb_label_provider_t provider, void *arg);
LCUI_API const char *ui_breadcrumb_get_key(ui_widget_t *breadcrumb);
LCUI_API void ui_breadcrumb_set_collapse_from(ui_widget_t *breadcrumb,
                                              int index);
LCUI_API int ui_breadcrumb_get_collapse_from(ui_widget_t *breadcrumb);
LCUI_API void ui_breadcrumb_set_separator(ui_widget_t *breadcrumb,
                                          const char *icon_name);
LCUI_API void ui_register_breadcrumb(void);
LCUI_API void ui_unregister_breadcrumb(void);

LCUI_END_HEADER

#endif
