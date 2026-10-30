/*
 * include/LCUI/widgets/router.h: -- Router widgets
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_ROUTER_H
#define LCUI_INCLUDE_WIDGETS_ROUTER_H

#include <stdbool.h>
#include <LCUI/common.h>
#include <ui/types.h>
#include <router/types.h>

LCUI_BEGIN_HEADER

LCUI_API void ui_router_link_set_location(ui_widget_t *w,
                                          router_location_t *location);

LCUI_API void ui_router_link_set_exact(ui_widget_t *w, bool exact);

LCUI_API void ui_register_router_link(void);

LCUI_API ui_widget_t *ui_router_view_get_matched_widget(ui_widget_t *w);

LCUI_API void ui_register_router_view(void);

LCUI_END_HEADER

#endif
