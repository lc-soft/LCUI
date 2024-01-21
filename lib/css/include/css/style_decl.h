/*
 * lib/css/include/css/style_decl.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_STYLE_DECL_H
#define LIBCSS_INCLUDE_CSS_STYLE_DECL_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC css_style_decl_t *css_style_decl_create(void);

LIBCSS_PUBLIC void css_style_decl_destroy(css_style_decl_t *list);

LIBCSS_PUBLIC css_prop_t *css_style_decl_alloc(css_style_decl_t *list, int key);

LIBCSS_PUBLIC void css_style_decl_add(css_style_decl_t *list, int key,
				 const css_style_value_t *value);

LIBCSS_PUBLIC void css_style_decl_set(css_style_decl_t *list, int key,
				 const css_style_value_t *value);

LIBCSS_PUBLIC int css_style_decl_remove(css_style_decl_t *list, int key);

LIBCSS_PUBLIC void css_style_decl_merge(css_style_decl_t *dst,
				   const css_style_decl_t *src);

LIBCSS_PUBLIC css_prop_t *css_style_decl_find(css_style_decl_t *list, int key);

LIBCSS_PUBLIC size_t css_print_style_decl(const css_style_decl_t *s);

LIBCSS_PUBLIC size_t css_style_decl_to_string(const css_style_decl_t *list,
					 char *str, size_t max_len);
LIBCSS_END_DECLS

#endif
