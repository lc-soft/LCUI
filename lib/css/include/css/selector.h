/*
 * lib/css/include/css/selector.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_SELECTOR_H
#define LIBCSS_INCLUDE_CSS_SELECTOR_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC css_selector_t *css_selector_create(const char *selector);

LIBCSS_PUBLIC css_selector_t *css_selector_duplicate(css_selector_t *selector);

LIBCSS_PUBLIC int css_selector_append(css_selector_t *selector,
				 css_selector_node_t *node);

LIBCSS_PUBLIC void css_selector_update(css_selector_t *s);

LIBCSS_PUBLIC void css_selector_destroy(css_selector_t *s);

LIBCSS_PUBLIC int css_selector_node_get_name_list(css_selector_node_t *sn,
					     list_t *names);

LIBCSS_PUBLIC int css_selector_node_update(css_selector_node_t *node);

LIBCSS_PUBLIC css_selector_node_t * css_selector_node_duplicate(const css_selector_node_t *src);

LIBCSS_PUBLIC void css_selector_node_destroy(css_selector_node_t *node);

/**
 * 匹配选择器节点
 * 左边的选择器必须包含右边的选择器的所有属性。
 */
LIBCSS_PUBLIC libcss_bool_t css_selector_node_match(css_selector_node_t *sn1,
					   css_selector_node_t *sn2);

LIBCSS_END_DECLS

#endif
