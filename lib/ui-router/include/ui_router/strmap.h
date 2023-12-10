/*
 * lib/ui-router/include/ui_router/strmap.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_STRMAP_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_STRMAP_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC strmap_t *strmap_create(void);

LIBUI_ROUTER_PUBLIC void strmap_destroy(strmap_t *dict);

LIBUI_ROUTER_PUBLIC void strmap_delete(strmap_t *dict, const char *key);

LIBUI_ROUTER_PUBLIC int strmap_set(strmap_t *dict, const char *key,
                                   const char *value);

LIBUI_ROUTER_PUBLIC const char *strmap_get(strmap_t *dict, const char *key);

LIBUI_ROUTER_PUBLIC size_t strmap_extend(strmap_t *target, strmap_t *other);

LIBUI_ROUTER_PUBLIC strmap_t *strmap_duplicate(strmap_t *target);

LIBUI_ROUTER_PUBLIC bool strmap_includes(strmap_t *a, strmap_t *b);

LIBUI_ROUTER_PUBLIC bool strmap_equal(strmap_t *a, strmap_t *b);

LIBUI_ROUTER_PUBLIC strmap_iterator_t *strmap_get_iterator(strmap_t *map);

LIBUI_ROUTER_PUBLIC void strmap_destroy_iterator(strmap_iterator_t *iter);

LIBUI_ROUTER_PUBLIC strmap_item_t *strmap_next(strmap_iterator_t *iter);

LIBUI_ROUTER_END_DECLS

#endif
