/*
 * lib/router/include/router/utils.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_UTILS_H
#define LIB_ROUTER_INCLUDE_ROUTER_UTILS_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC char *router_path_fill_params(const char *path,
                                                  strmap_t *params);

LIBROUTER_PUBLIC const char *router_path_parse_key(const char *path,
                                                      char key[256],
                                                      size_t *key_len);

LIBROUTER_PUBLIC size_t router_path_parse_keys(const char *path,
                                                  router_linkedlist_t *keys);

LIBROUTER_PUBLIC char *router_path_resolve(const char *relative,
                                              const char *base, bool append);

LIBROUTER_PUBLIC strmap_t *router_parse_query(const char *query_str);

LIBROUTER_PUBLIC int router_string_compare(const char *a, const char *b);

LIBROUTER_PUBLIC int router_path_compare(const char *a, const char *b);

LIBROUTER_PUBLIC bool router_path_starts_with(const char *path,
                                                 const char *subpath);

LIBROUTER_PUBLIC bool router_is_same_route(const router_route_t *a,
                                              const router_route_t *b);

LIBROUTER_PUBLIC bool router_is_included_route(const router_route_t *current,
                                                  const router_route_t *target);

LIBROUTER_END_DECLS

#endif
