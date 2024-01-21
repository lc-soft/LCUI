/*
 * lib/ui-router/include/ui_router/utils.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_UTILS_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_UTILS_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC char *router_path_fill_params(const char *path,
                                                  strmap_t *params);

LIBUI_ROUTER_PUBLIC const char *router_path_parse_key(const char *path,
                                                      char key[256],
                                                      size_t *key_len);

LIBUI_ROUTER_PUBLIC size_t router_path_parse_keys(const char *path,
                                                  router_linkedlist_t *keys);

LIBUI_ROUTER_PUBLIC char *router_path_resolve(const char *relative,
                                              const char *base, bool append);

LIBUI_ROUTER_PUBLIC strmap_t *router_parse_query(const char *query_str);

LIBUI_ROUTER_PUBLIC int router_string_compare(const char *a, const char *b);

LIBUI_ROUTER_PUBLIC int router_path_compare(const char *a, const char *b);

LIBUI_ROUTER_PUBLIC bool router_path_starts_with(const char *path,
                                                 const char *subpath);

LIBUI_ROUTER_PUBLIC bool router_is_same_route(const router_route_t *a,
                                              const router_route_t *b);

LIBUI_ROUTER_PUBLIC bool router_is_included_route(const router_route_t *current,
                                                  const router_route_t *target);

LIBUI_ROUTER_END_DECLS

#endif
