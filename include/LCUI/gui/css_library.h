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

#ifndef LIBCSS_INCLUDE_APP_H
#define LIBCSS_INCLUDE_APP_H

#include <LCUI/header.h>
#include <LCUI/util.h>

LCUI_BEGIN_HEADER

/* clang-format off */

#define CSS_SELECTOR_MAX_LEN	1024
#define CSS_SELECTOR_MAX_DEPTH	32

 /** 样式属性名 */
typedef enum css_property_key_t {
	// position start
	css_key_left,
	css_key_right,
	css_key_top,
	css_key_bottom,
	css_key_position,
	// position end

	// display start
	css_key_visibility,
	css_key_display,
	// display end

	css_key_z_index,
	css_key_opacity,
	css_key_box_sizing,
	css_key_width,
	css_key_height,
	css_key_min_width,
	css_key_min_height,
	css_key_max_width,
	css_key_max_height,

	// margin start
	css_key_margin_top,
	css_key_margin_right,
	css_key_margin_bottom,
	css_key_margin_left,
	// margin end

	// padding start
	css_key_padding_top,
	css_key_padding_right,
	css_key_padding_bottom,
	css_key_padding_left,
	// padding end

	css_key_vertical_align,

	// border start
	css_key_border_top_width,
	css_key_border_top_style,
	css_key_border_top_color,
	css_key_border_right_width,
	css_key_border_right_style,
	css_key_border_right_color,
	css_key_border_bottom_width,
	css_key_border_bottom_style,
	css_key_border_bottom_color,
	css_key_border_left_width,
	css_key_border_left_style,
	css_key_border_left_color,
	css_key_border_top_left_radius,
	css_key_border_top_right_radius,
	css_key_border_bottom_left_radius,
	css_key_border_bottom_right_radius,
	// border end

	// background start
	css_key_background_color,
	css_key_background_image,
	css_key_background_size,
	css_key_background_size_width,
	css_key_background_size_height,
	css_key_background_repeat,
	css_key_background_repeat_x,
	css_key_background_repeat_y,
	css_key_background_position,
	css_key_background_position_x,
	css_key_background_position_y,
	css_key_background_origin,
	// background end

	// box shadow start
	css_key_box_shadow_x,
	css_key_box_shadow_y,
	css_key_box_shadow_spread,
	css_key_box_shadow_blur,
	css_key_box_shadow_color,
	// box shadow end

	// flex style start
	css_key_flex_basis,
	css_key_flex_grow,
	css_key_flex_shrink,
	css_key_flex_direction,
	css_key_flex_wrap,
	css_key_justify_content,
	css_key_align_content,
	css_key_align_items,
	// flex style end

	css_key_pointer_events,
	css_key_focusable,
	STYLE_KEY_TOTAL
} css_property_key_t;

#define css_key_flex_style_start	css_key_flex_basis
#define css_key_flex_style_end		css_key_align_content
#define css_key_position_start		css_key_left
#define css_key_position_end		css_key_position
#define css_key_margin_start		css_key_margin_top
#define css_key_margin_end		css_key_margin_left
#define css_key_padding_start		css_key_padding_top
#define css_key_padding_end		css_key_padding_left
#define css_key_border_start		css_key_border_top_width
#define css_key_border_end		css_key_border_bottom_right_radius
#define css_key_background_start	css_key_background_color
#define css_key_background_end		css_key_background_origin
#define css_key_box_shadow_start	css_key_box_shadow_x
#define css_key_box_shadow_end		css_key_box_shadow_color

struct css_style_declaration_t {
	css_unit_value_t *sheet;
	size_t length;
};

typedef struct css_style_declaration_t css_style_declaration_t;
typedef list_t css_style_properties_t;

typedef css_style_properties_t css_style_props_t;
typedef css_style_declaration_t css_style_decl_t;

typedef struct css_style_property_t {
	css_property_key_t key;
	css_unit_value_t style;
	list_node_t node;
} css_style_property_t;

/** 选择器结点结构 */
typedef struct css_selector_node_t {
	char *id;			/**< ID */
	char *type;			/**< 类型名称 */
	char **classes;			/**< 样式类列表 */
	char **status;			/**< 状态列表 */
	char *fullname;			/**< 全名，由 id、type、classes、status 组合而成 */
	int rank;			/**< 权值 */
} css_selector_node_t;

/** 选择器结构 */
typedef struct css_selector_t {
	int rank;			/**< 权值，决定优先级 */
	int batch_num;			/**< 批次号 */
	int length;			/**< 选择器结点长度 */
	unsigned hash;			/**< 哈希值 */
	css_selector_node_t **nodes;	/**< 选择器结点列表 */
} css_selector_t;

/* clang-format on */

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
	return style->sheet[key].is_valid && style->sheet[key].type == unit;
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
