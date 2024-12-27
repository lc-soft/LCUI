/*
 * lib/router/include/router/route_record.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_ROUTE_RECORD_H
#define LIB_ROUTER_INCLUDE_ROUTER_ROUTE_RECORD_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC router_route_record_t *router_route_record_create(void);

LIBROUTER_PUBLIC void router_route_record_destroy(
    router_route_record_t *record);

LIBROUTER_PUBLIC void router_route_record_set_path(
    router_route_record_t *record, const char *path);

LIBROUTER_PUBLIC const char *router_route_record_get_component(
    const router_route_record_t *record, const char *key);

LIBROUTER_END_DECLS

#endif
