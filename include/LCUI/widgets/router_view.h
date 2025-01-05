/*
 * include/LCUI/widgets/router_view.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCOIDE_ROUTER_VIEW_H
#define LCUI_INCOIDE_ROUTER_VIEW_H

#include <ui/types.h>
#include <LCUI/common.h>

LCUI_BEGIN_HEADER

LCUI_API ui_widget_t *ui_router_view_get_matched_widget(
    ui_widget_t *w);

LCUI_API void ui_register_router_view(void);

LCUI_END_HEADER

#endif
