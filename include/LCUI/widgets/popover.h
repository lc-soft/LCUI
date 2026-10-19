/*
 * include/LCUI/widgets/popover.h: -- LCUI's Popover widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_POPOVER_H
#define LCUI_INCLUDE_WIDGETS_POPOVER_H

#include <LCUI/common.h>
#include <LCUI/widgets/placement.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef ui_side_t ui_popover_side_t;
typedef ui_align_t ui_popover_align_t;

LCUI_API ui_widget_t *ui_create_popover(void);
LCUI_API ui_widget_t *ui_create_popover_trigger(void);
LCUI_API ui_widget_t *ui_create_popover_content(void);
LCUI_API ui_widget_t *ui_create_popover_header(void);
LCUI_API ui_widget_t *ui_create_popover_title(void);
LCUI_API ui_widget_t *ui_create_popover_description(void);

LCUI_API void ui_popover_build_children(ui_widget_t *popover);
LCUI_API void ui_popover_set_trigger_text(ui_widget_t *popover,
                                          const char *text);
LCUI_API void ui_popover_set_title(ui_widget_t *popover, const char *text);
LCUI_API void ui_popover_set_description(ui_widget_t *popover,
                                         const char *text);
LCUI_API ui_widget_t *ui_popover_get_content(ui_widget_t *popover);

LCUI_API void ui_popover_content_set_side(ui_widget_t *content,
                                          ui_popover_side_t side);
LCUI_API void ui_popover_content_set_align(ui_widget_t *content,
                                           ui_popover_align_t align);
LCUI_API void ui_popover_content_set_side_offset(ui_widget_t *content,
                                                 int offset);
LCUI_API void ui_popover_content_set_align_offset(ui_widget_t *content,
                                                  int offset);

LCUI_API void ui_popover_set_open(ui_widget_t *popover, bool open);
LCUI_API void ui_popover_open(ui_widget_t *popover);
LCUI_API void ui_popover_close(ui_widget_t *popover);
LCUI_API bool ui_popover_is_open(ui_widget_t *popover);

LCUI_API void ui_register_popover(void);
LCUI_API void ui_unregister_popover(void);

LCUI_END_HEADER

#endif
