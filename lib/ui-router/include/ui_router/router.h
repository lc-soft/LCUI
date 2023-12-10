/*
 * lib/ui-router/include/ui_router/router.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_ROUTER_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_ROUTER_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC router_t *router_create(const char *name);

LIBUI_ROUTER_PUBLIC void router_destroy(router_t *router);

LIBUI_ROUTER_PUBLIC router_route_record_t *router_add_route_record(
    router_t *router, const router_config_t *config,
    const router_route_record_t *parent);

LIBUI_ROUTER_PUBLIC router_route_t *router_match(
    router_t *router, const router_location_t *raw_location,
    const router_route_t *current_route);

LIBUI_ROUTER_PUBLIC router_route_record_t *router_get_matched_route_record(
    router_t *router, size_t index);

LIBUI_ROUTER_PUBLIC router_history_t *router_get_history(router_t *router);

LIBUI_ROUTER_PUBLIC router_watcher_t *router_watch(router_t *router,
                                                   router_callback_t callback,
                                                   void *data);

LIBUI_ROUTER_PUBLIC void router_unwatch(router_t *router,
                                        router_watcher_t *watcher);

LIBUI_ROUTER_PUBLIC router_resolved_t *router_resolve(
    router_t *router, router_location_t *location, bool append);

LIBUI_ROUTER_PUBLIC router_location_t *router_resolved_get_location(
    router_resolved_t *resolved);

LIBUI_ROUTER_PUBLIC router_route_t *router_resolved_get_route(
    router_resolved_t *resolved);

LIBUI_ROUTER_PUBLIC void router_resolved_destroy(router_resolved_t *resolved);

LIBUI_ROUTER_PUBLIC const router_route_t *router_get_current_route(
    router_t *router);

LIBUI_ROUTER_PUBLIC void router_push(router_t *router,
                                     router_location_t *location);

LIBUI_ROUTER_PUBLIC void router_replace(router_t *router,
                                        router_location_t *location);

LIBUI_ROUTER_PUBLIC void router_go(router_t *router, int delta);

LIBUI_ROUTER_PUBLIC void router_back(router_t *router);

LIBUI_ROUTER_PUBLIC void router_forward(router_t *router);

LIBUI_ROUTER_PUBLIC router_t *router_get_by_name(const char *name);

LIBUI_ROUTER_PUBLIC const char *router_get_version(void);

LIBUI_ROUTER_END_DECLS

#endif
