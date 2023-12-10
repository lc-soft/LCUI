/*
 * lib/ui-router/include/ui_router/history.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_HISTORY_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_HISTORY_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC router_history_t *router_history_create(void);

LIBUI_ROUTER_PUBLIC void router_history_destroy(router_history_t *history);

LIBUI_ROUTER_PUBLIC router_watcher_t *router_history_watch(
    router_history_t *history, router_callback_t callback, void *data);

LIBUI_ROUTER_PUBLIC void router_history_unwatch(router_history_t *history,
                                                router_watcher_t *watcher);

LIBUI_ROUTER_PUBLIC void router_history_push(router_history_t *history,
                                             router_route_t *route);

LIBUI_ROUTER_PUBLIC void router_history_replace(router_history_t *history,
                                                router_route_t *route);

LIBUI_ROUTER_PUBLIC void router_history_go(router_history_t *history,
                                           int delta);

LIBUI_ROUTER_PUBLIC size_t
router_history_get_index(const router_history_t *history);

LIBUI_ROUTER_PUBLIC size_t
router_history_get_length(const router_history_t *history);

LIBUI_ROUTER_END_DECLS

#endif
