/*
 * lib/router/include/router/strmap.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_STRMAP_H
#define LIB_ROUTER_INCLUDE_ROUTER_STRMAP_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC strmap_t *strmap_create(void);

LIBROUTER_PUBLIC void strmap_destroy(strmap_t *dict);

LIBROUTER_PUBLIC void strmap_delete(strmap_t *dict, const char *key);

LIBROUTER_PUBLIC int strmap_set(strmap_t *dict, const char *key,
                                   const char *value);

LIBROUTER_PUBLIC const char *strmap_get(strmap_t *dict, const char *key);

LIBROUTER_PUBLIC size_t strmap_extend(strmap_t *target, strmap_t *other);

LIBROUTER_PUBLIC strmap_t *strmap_duplicate(strmap_t *target);

LIBROUTER_PUBLIC bool strmap_includes(strmap_t *a, strmap_t *b);

LIBROUTER_PUBLIC bool strmap_equal(strmap_t *a, strmap_t *b);

LIBROUTER_PUBLIC strmap_iterator_t *strmap_get_iterator(strmap_t *map);

LIBROUTER_PUBLIC void strmap_destroy_iterator(strmap_iterator_t *iter);

LIBROUTER_PUBLIC strmap_item_t *strmap_next(strmap_iterator_t *iter);

LIBROUTER_END_DECLS

#endif
