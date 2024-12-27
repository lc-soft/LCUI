/*
 * include/LCUI/widgets/router_link.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCOIDE_ROUTER_LINK_H
#define LCUI_INCOIDE_ROUTER_LINK_H

#include <ui/types.h>
#include <router/types.h>
#include <LCUI/common.h>

LCUI_BEGIN_HEADER

LCUI_API void ui_router_link_set_location(
    ui_widget_t *w, router_location_t *location);

LCUI_API void ui_router_link_set_exact(ui_widget_t *w, bool exact);

LCUI_API void ui_register_router_link(void);

LCUI_END_HEADER

#endif
