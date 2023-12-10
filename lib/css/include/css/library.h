/*
 * lib/css/include/css/library.h: -- CSS library operation module.
 *
 * Copyright (c) 2022-2023-2023-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_LIBRARY_H
#define LIBCSS_INCLUDE_CSS_LIBRARY_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC int css_add_style_decl(css_selector_t *selector,
				const css_style_decl_t *in_ss,
				const char *space);

/**
 * 从指定组中查找样式表
 * @param[in] group 组号
 * @param[in] name 选择器结点名称，若为 NULL，则根据选择器结点生成名称
 * @param[in] s 选择器
 * @param[out] list 找到的样式表列表
 */
LIBCSS_PUBLIC int css_query_selector_from_group(int group, const char *name,
					   const css_selector_t *s,
					   list_t *list);

LIBCSS_INLINE int css_query_selector(const css_selector_t *s, list_t *list)
{
	return css_query_selector_from_group(0, NULL, s, list);
}

LIBCSS_PUBLIC void css_each_style_rule(void (*callback)(css_style_rule_t *, const char *,
					     void *),
			    void *data);

LIBCSS_PUBLIC css_style_decl_t *css_select_style(const css_selector_t *s);

LIBCSS_PUBLIC css_style_decl_t *css_select_style_with_cache(const css_selector_t *s);

LIBCSS_PUBLIC void css_init_library(void);

LIBCSS_PUBLIC void css_destroy_library(void);

LIBCSS_PUBLIC size_t css_print_style_rules(void);

LIBCSS_PUBLIC size_t css_style_rules_to_string(char *str, size_t max_len);

LIBCSS_END_DECLS

#endif
