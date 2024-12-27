/*
 * lib/router/include/router/location.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_LOCATION_H
#define LIB_ROUTER_INCLUDE_ROUTER_LOCATION_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC router_location_t *router_location_create(const char *name,
                                                              const char *path);

LIBROUTER_PUBLIC void router_location_destroy(router_location_t *location);

LIBROUTER_PUBLIC void router_location_set_name(router_location_t *location,
                                                  const char *name);

LIBROUTER_PUBLIC router_location_t *router_location_duplicate(
    const router_location_t *location);

LIBROUTER_PUBLIC router_location_t *router_location_normalize(
    const router_location_t *raw, const router_route_t *current, bool append);

LIBROUTER_PUBLIC int router_location_set_param(router_location_t *location,
                                                  const char *key,
                                                  const char *value);

LIBROUTER_PUBLIC const char *router_location_get_param(
    const router_location_t *location, const char *key);

LIBROUTER_PUBLIC int router_location_set_query(router_location_t *location,
                                                  const char *key,
                                                  const char *value);

LIBROUTER_PUBLIC const char *router_location_get_query(
    const router_location_t *location, const char *key);

LIBROUTER_PUBLIC const char *router_location_get_path(
    const router_location_t *location);

LIBROUTER_PUBLIC char *router_location_stringify(
    const router_location_t *location);

LIBROUTER_END_DECLS

#endif
