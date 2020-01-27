/*
 * css_library.h -- CSS library operation module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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

#ifndef LCUI_CSS_LIBRARY_H
#define LCUI_CSS_LIBRARY_H

#include <LCUI/util/linkedlist.h>

LCUI_BEGIN_HEADER

/* clang-format off */

#define MAX_SELECTOR_LEN	1024
#define MAX_SELECTOR_DEPTH	32

 /** 样式属性名 */
enum LCUI_StyleKeyName {
	// position start
	key_left,
	key_right,
	key_top,
	key_bottom,
	key_position,
	// position end

	// display start
	key_visibility,
	key_display,
	// display end

	key_z_index,
	key_opacity,
	key_box_sizing,
	key_width,
	key_height,
	key_min_width,
	key_min_height,
	key_max_width,
	key_max_height,

	// margin start
	key_margin_top,
	key_margin_right,
	key_margin_bottom,
	key_margin_left,
	// margin end

	// padding start
	key_padding_top,
	key_padding_right,
	key_padding_bottom,
	key_padding_left,
	// padding end

	key_vertical_align,

	// border start
	key_border_top_width,
	key_border_top_style,
	key_border_top_color,
	key_border_right_width,
	key_border_right_style,
	key_border_right_color,
	key_border_bottom_width,
	key_border_bottom_style,
	key_border_bottom_color,
	key_border_left_width,
	key_border_left_style,
	key_border_left_color,
	key_border_top_left_radius,
	key_border_top_right_radius,
	key_border_bottom_left_radius,
	key_border_bottom_right_radius,
	// border end

	// background start
	key_background_color,
	key_background_image,
	key_background_size,
	key_background_size_width,
	key_background_size_height,
	key_background_repeat,
	key_background_repeat_x,
	key_background_repeat_y,
	key_background_position,
	key_background_position_x,
	key_background_position_y,
	key_background_origin,
	// background end

	// box shadow start
	key_box_shadow_x,
	key_box_shadow_y,
	key_box_shadow_spread,
	key_box_shadow_blur,
	key_box_shadow_color,
	// box shadow end
	
	// flex style start
	key_flex_basis,
	key_flex_grow,
	key_flex_shrink,
	key_flex_direction,
	key_flex_wrap,
	key_justify_content,
	key_align_content,
	key_align_items,
	// flex style end

	key_pointer_events,
	key_focusable,
	STYLE_KEY_TOTAL
};

#define key_flex_style_start	key_flex_basis
#define key_flex_style_end	key_align_content
#define key_position_start	key_left
#define key_position_end	key_position
#define key_margin_start	key_margin_top
#define key_margin_end		key_margin_left
#define key_padding_start	key_padding_top
#define key_padding_end		key_padding_left
#define key_border_start	key_border_top_width
#define key_border_end		key_border_bottom_right_radius
#define key_background_start	key_background_color
#define key_background_end	key_background_origin
#define key_box_shadow_start	key_box_shadow_x
#define key_box_shadow_end	key_box_shadow_color

typedef struct LCUI_StyleSheetRec_ {
	LCUI_Style sheet;
	int length;
} LCUI_StyleSheetRec, *LCUI_StyleSheet;

typedef const LCUI_StyleSheetRec *LCUI_CachedStyleSheet;

typedef LinkedList LCUI_StyleListRec;
typedef LinkedList* LCUI_StyleList;

typedef struct LCUI_StyleListNodeRec_ {
	int key;
	LCUI_StyleRec style;
	LinkedListNode node;
} LCUI_StyleListNodeRec, *LCUI_StyleListNode;

/** 选择器结点结构 */
typedef struct LCUI_SelectorNodeRec_ {
	char *id;			/**< ID */
	char *type;			/**< 类型名称 */
	char **classes;			/**< 样式类列表 */
	char **status;			/**< 状态列表 */
	char *fullname;			/**< 全名，由 id、type、classes、status 组合而成 */
	int rank;			/**< 权值 */
} LCUI_SelectorNodeRec, *LCUI_SelectorNode;

/** 选择器结构 */
typedef struct LCUI_SelectorRec_ {
	int rank;			/**< 权值，决定优先级 */
	int batch_num;			/**< 批次号 */
	int length;			/**< 选择器结点长度 */
	unsigned hash;			/**< 哈希值 */
	LCUI_SelectorNode *nodes;	/**< 选择器结点列表 */
} LCUI_SelectorRec, *LCUI_Selector;

/* clang-format on */

#define CheckStyleType(S, K, T) \
	(S->sheet[K].is_valid && S->sheet[K].type == LCUI_STYPE_##T)

#define SetStyle(S, NAME, VAL, TYPE)             \
	S->sheet[NAME].is_valid = TRUE,          \
	S->sheet[NAME].type = LCUI_STYPE_##TYPE, \
	S->sheet[NAME].val_##TYPE = VAL

#define UnsetStyle(S, NAME)              \
	S->sheet[NAME].is_valid = FALSE, \
	S->sheet[NAME].type = LCUI_STYPE_NONE, S->sheet[NAME].val_int = 0

#define LCUI_FindStyleSheet(S, L) LCUI_FindStyleSheetFromGroup(0, NULL, S, L)

#define StyleSheet_GetStyle(S, K) &((S)->sheet[K])

LCUI_API void DestroyStyle(LCUI_Style s);

LCUI_API void MergeStyle(LCUI_Style dst, LCUI_Style src);

LCUI_API LCUI_StyleList StyleList(void);

LCUI_API LCUI_StyleListNode StyleList_GetNode(LCUI_StyleList list, int key);

LCUI_API int StyleList_RemoveNode(LCUI_StyleList list, int key);

LCUI_API LCUI_StyleListNode StyleList_AddNode(LCUI_StyleList list, int key);

LCUI_API void StyleList_Delete(LCUI_StyleList list);

LCUI_API LCUI_StyleSheet StyleSheet(void);

LCUI_API void StyleSheet_Clear(LCUI_StyleSheet ss);

LCUI_API void StyleSheet_Delete(LCUI_StyleSheet ss);

LCUI_API int StyleSheet_Merge(LCUI_StyleSheet dest,
			      const LCUI_StyleSheetRec *src);

LCUI_API int StyleSheet_MergeList(LCUI_StyleSheet ss, LCUI_StyleList list);

LCUI_API int StyleSheet_Replace(LCUI_StyleSheet dest,
				const LCUI_StyleSheetRec *src);

LCUI_API LCUI_Selector Selector(const char *selector);

LCUI_API LCUI_Selector Selector_Copy(LCUI_Selector selector);

LCUI_API int Selector_AppendNode(LCUI_Selector selector,
				 LCUI_SelectorNode node);

LCUI_API void Selector_Update(LCUI_Selector s);

LCUI_API void Selector_Delete(LCUI_Selector s);

LCUI_API int SelectorNode_GetNames(LCUI_SelectorNode sn, LinkedList *names);

LCUI_API int SelectorNode_Update(LCUI_SelectorNode node);

LCUI_API void SelectorNode_Delete(LCUI_SelectorNode node);

/**
 * 匹配选择器节点
 * 左边的选择器必须包含右边的选择器的所有属性。
 */
LCUI_API LCUI_BOOL SelectorNode_Match(LCUI_SelectorNode sn1,
				      LCUI_SelectorNode sn2);

LCUI_API int LCUI_PutStyleSheet(LCUI_Selector selector, LCUI_StyleSheet in_ss,
				const char *space);

/**
 * 从指定组中查找样式表
 * @param[in] group 组号
 * @param[in] name 选择器结点名称，若为 NULL，则根据选择器结点生成名称
 * @param[in] s 选择器
 * @param[out] list 找到的样式表列表
 */
LCUI_API int LCUI_FindStyleSheetFromGroup(int group, const char *name,
					  LCUI_Selector s, LinkedList *list);

LCUI_API LCUI_CachedStyleSheet LCUI_GetCachedStyleSheet(LCUI_Selector s);

LCUI_API void LCUI_GetStyleSheet(LCUI_Selector s, LCUI_StyleSheet out_ss);

LCUI_API void LCUI_PrintStyleSheetsBySelector(LCUI_Selector s);

LCUI_API int LCUI_SetStyleName(int key, const char *name);

LCUI_API int LCUI_AddCSSPropertyName(const char *name);

LCUI_API int LCUI_AddStyleValue(int key, const char *name);

LCUI_API int LCUI_GetStyleValue(const char *str);

LCUI_API const char *LCUI_GetStyleValueName(int val);

LCUI_API const char *LCUI_GetStyleName(int key);

LCUI_API int LCUI_GetStyleTotal(void);

LCUI_API void LCUI_PrintStyleSheet(LCUI_StyleSheet ss);

LCUI_API void LCUI_PrintSelector(LCUI_Selector selector);

LCUI_API void LCUI_PrintCSSLibrary(void);

LCUI_API void LCUI_InitCSSLibrary(void);

LCUI_API void LCUI_FreeCSSLibrary(void);

LCUI_END_HEADER

#endif
