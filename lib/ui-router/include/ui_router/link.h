/*
 * lib/ui-router/include/ui_router/link.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_LINK_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_LINK_H

#include <ui.h>
#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC void ui_router_link_set_location(
    ui_widget_t *w, router_location_t *location);

LIBUI_ROUTER_PUBLIC void ui_router_link_set_exact(ui_widget_t *w, bool exact);

LIBUI_ROUTER_PUBLIC void ui_register_router_link(void);

LIBUI_ROUTER_END_DECLS

#endif
