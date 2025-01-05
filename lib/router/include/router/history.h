/*
 * lib/router/include/router/history.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_HISTORY_H
#define LIB_ROUTER_INCLUDE_ROUTER_HISTORY_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC router_history_t *router_history_create(void);

LIBROUTER_PUBLIC void router_history_destroy(router_history_t *history);

LIBROUTER_PUBLIC router_watcher_t *router_history_watch(
    router_history_t *history, router_callback_t callback, void *data);

LIBROUTER_PUBLIC void router_history_unwatch(router_history_t *history,
                                                router_watcher_t *watcher);

LIBROUTER_PUBLIC void router_history_push(router_history_t *history,
                                             router_route_t *route);

LIBROUTER_PUBLIC void router_history_replace(router_history_t *history,
                                                router_route_t *route);

LIBROUTER_PUBLIC void router_history_go(router_history_t *history,
                                           int delta);

LIBROUTER_PUBLIC size_t
router_history_get_index(const router_history_t *history);

LIBROUTER_PUBLIC size_t
router_history_get_length(const router_history_t *history);

LIBROUTER_END_DECLS

#endif
