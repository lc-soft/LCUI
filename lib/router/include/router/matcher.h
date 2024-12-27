/*
 * lib/router/include/router/matcher.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_MATCHER_H
#define LIB_ROUTER_INCLUDE_ROUTER_MATCHER_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC router_matcher_t *router_matcher_create(void);

LIBROUTER_PUBLIC void router_matcher_destroy(router_matcher_t *matcher);

LIBROUTER_PUBLIC router_route_t *router_matcher_match(
    router_matcher_t *matcher, const router_location_t *raw_location,
    const router_route_t *current_route);

LIBROUTER_PUBLIC router_route_record_t *router_matcher_add_route_record(
    router_matcher_t *matcher, const router_config_t *config,
    const router_route_record_t *parent);

LIBROUTER_END_DECLS

#endif
