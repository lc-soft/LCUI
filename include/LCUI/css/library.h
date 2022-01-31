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

#include <LCUI/header.h>
#include "def.h"

LCUI_BEGIN_HEADER

/* clang-format off */

#define css_check_style_prop(S, K, T) \
	css_style_declaration_check_property(S, K, CSS_UNIT_##T)

#define css_set_style_prop(S, KEY, VAL, UNIT)         \
	do {                                          \
		S->sheet[KEY].is_valid = TRUE;        \
		S->sheet[KEY].UNIT = CSS_UNIT_##UNIT; \
		S->sheet[KEY].val_##UNIT = VAL;       \
	} while (0);

#define css_unset_style_prop(S, KEY)                \
	do {                                        \
		S->sheet[KEY].is_valid = FALSE;     \
		S->sheet[KEY].type = CSS_UNIT_NONE; \
		S->sheet[KEY].val_int = 0;          \
	} while (0);

LCUI_API void css_unit_value_destroy(css_unit_value_t *s);

LCUI_API void css_unit_value_merge(css_unit_value_t *dst,
				   css_unit_value_t *src);

LCUI_API css_style_props_t *css_style_properties_create(void);

LCUI_API css_style_property_t *css_style_properties_find(
    css_style_props_t *list, int key);

LCUI_API int css_style_properties_remove(css_style_props_t *list, int key);

LCUI_API css_style_property_t *css_style_properties_add(css_style_props_t *list,
							int key);

LCUI_API void css_style_properties_destroy(css_style_props_t *list);

LCUI_API css_style_decl_t *css_style_declaration_create(void);

INLINE LCUI_BOOL css_style_declaration_check_property(
    css_style_decl_t *style, css_property_key_t key, css_unit_t unit)
{
	return style->sheet[key].is_valid && style->sheet[key].unit == unit;
}

LCUI_API void css_style_declaration_clear(css_style_decl_t *ss);

LCUI_API void css_style_declaration_destroy(css_style_decl_t *ss);

LCUI_API int css_style_declaration_merge(css_style_decl_t *dest,
					 const css_style_decl_t *src);

LCUI_API int css_style_declaration_merge_properties(css_style_decl_t *ss,
						    css_style_props_t *list);

LCUI_API int css_style_declaration_replace(css_style_decl_t *dest,
					   const css_style_decl_t *src);

LCUI_API css_selector_t *css_selector_create(const char *selector);

LCUI_API css_selector_t *css_selector_duplicate(css_selector_t *selector);

LCUI_API int css_selector_append(css_selector_t *selector,
				 css_selector_node_t *node);

LCUI_API void css_selector_update(css_selector_t *s);

LCUI_API void css_selector_destroy(css_selector_t *s);

LCUI_API int css_selector_node_get_name_list(css_selector_node_t *sn,
					     list_t *names);

LCUI_API int css_selector_node_update(css_selector_node_t *node);

LCUI_API void css_selector_node_destroy(css_selector_node_t *node);

/**
 * 匹配选择器节点
 * 左边的选择器必须包含右边的选择器的所有属性。
 */
LCUI_API LCUI_BOOL css_selector_node_match(css_selector_node_t *sn1,
					   css_selector_node_t *sn2);

LCUI_API int css_add_style_sheet(css_selector_t *selector,
				css_style_decl_t *in_ss, const char *space);

/**
 * 从指定组中查找样式表
 * @param[in] group 组号
 * @param[in] name 选择器结点名称，若为 NULL，则根据选择器结点生成名称
 * @param[in] s 选择器
 * @param[out] list 找到的样式表列表
 */
LCUI_API int css_query_selector_from_group(int group, const char *name,
					     css_selector_t *s, list_t *list);

INLINE int css_query_selector(css_selector_t *s, list_t *list)
{
	return css_query_selector_from_group(0, NULL, s, list);
}

LCUI_API const css_style_decl_t *css_get_computed_style_with_cache(css_selector_t *s);

LCUI_API void css_get_computed_style(css_selector_t *s, css_style_decl_t *out_ss);

LCUI_API void css_print_style_rules_by_selector(css_selector_t *s);

LCUI_API int css_register_property_name(const char *name);

LCUI_API int css_register_keyword(int key, const char *name);

LCUI_API int css_get_keyword_key(const char *str);

LCUI_API const char *css_get_keyword_name(int val);

LCUI_API const char *css_get_property_name(int key);

LCUI_API int css_get_property_count(void);

LCUI_API void css_style_declartation_print(css_style_decl_t *ss);

LCUI_API void css_selector_print(css_selector_t *selector);

LCUI_API void css_print_all(void);

LCUI_API void css_init(void);

LCUI_API void css_destroy(void);

LCUI_END_HEADER

#endif
