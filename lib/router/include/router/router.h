/*
 * lib/router/include/router/router.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_ROUTER_H
#define LIB_ROUTER_INCLUDE_ROUTER_ROUTER_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC router_t *router_create(const char *name);

LIBROUTER_PUBLIC void router_destroy(router_t *router);

LIBROUTER_PUBLIC router_route_record_t *router_add_route_record(
    router_t *router, const router_config_t *config,
    const router_route_record_t *parent);

LIBROUTER_PUBLIC router_route_t *router_match(
    router_t *router, const router_location_t *raw_location,
    const router_route_t *current_route);

LIBROUTER_PUBLIC router_route_record_t *router_get_matched_route_record(
    router_t *router, size_t index);

LIBROUTER_PUBLIC router_history_t *router_get_history(router_t *router);

LIBROUTER_PUBLIC router_watcher_t *router_watch(router_t *router,
                                                   router_callback_t callback,
                                                   void *data);

LIBROUTER_PUBLIC void router_unwatch(router_t *router,
                                        router_watcher_t *watcher);

LIBROUTER_PUBLIC router_resolved_t *router_resolve(
    router_t *router, router_location_t *location, bool append);

LIBROUTER_PUBLIC router_location_t *router_resolved_get_location(
    router_resolved_t *resolved);

LIBROUTER_PUBLIC router_route_t *router_resolved_get_route(
    router_resolved_t *resolved);

LIBROUTER_PUBLIC void router_resolved_destroy(router_resolved_t *resolved);

LIBROUTER_PUBLIC const router_route_t *router_get_current_route(
    router_t *router);

LIBROUTER_PUBLIC void router_push(router_t *router,
                                     router_location_t *location);

LIBROUTER_PUBLIC void router_replace(router_t *router,
                                        router_location_t *location);

LIBROUTER_PUBLIC void router_go(router_t *router, int delta);

LIBROUTER_PUBLIC void router_back(router_t *router);

LIBROUTER_PUBLIC void router_forward(router_t *router);

LIBROUTER_PUBLIC router_t *router_get_by_name(const char *name);

LIBROUTER_PUBLIC const char *router_get_version(void);

LIBROUTER_END_DECLS

#endif
