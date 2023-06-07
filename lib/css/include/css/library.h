/*
 * css_library.h -- CSS library operation module.
 *
 * Copyright (c) 2022, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
