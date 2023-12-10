/*
 * lib/ui-router/include/ui_router/route.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_ROUTE_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_ROUTE_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC router_route_t *router_route_create(
    const router_route_record_t *record, const router_location_t *location);

LIBUI_ROUTER_PUBLIC void router_route_destroy(router_route_t *route);

LIBUI_ROUTER_PUBLIC const router_route_record_t *
router_route_get_matched_record(const router_route_t *route, size_t index);

LIBUI_ROUTER_PUBLIC const char *router_route_get_full_path(
    const router_route_t *route);

LIBUI_ROUTER_PUBLIC const char *router_route_get_path(
    const router_route_t *route);

LIBUI_ROUTER_PUBLIC const char *router_route_get_hash(
    const router_route_t *route);

LIBUI_ROUTER_PUBLIC const char *router_route_get_param(
    const router_route_t *route, const char *key);

LIBUI_ROUTER_PUBLIC const char *router_route_get_query(
    const router_route_t *route, const char *key);

LIBUI_ROUTER_END_DECLS

#endif
