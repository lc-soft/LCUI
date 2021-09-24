﻿/*
 * css_library.c -- CSS library operation module.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/types.h>
#include <LCUI/util.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>

/* clang-format off */

#define MAX_NAME_LEN	256
#define LEN(A)		sizeof(A) / sizeof(*A)

enum SelectorRank {
	GENERAL_RANK = 0,
	TYPE_RANK = 1,
	CLASS_RANK = 10,
	PCLASS_RANK = 10,
	ID_RANK = 100
};

enum SelectorFinderLevel {
	LEVEL_NONE,
	LEVEL_TYPE,
	LEVEL_ID,
	LEVEL_CLASS,
	LEVEL_CLASS_2,
	LEVEL_STATUS,
	LEVEL_STATUS_2,
	LEVEL_TOTAL_NUM
};

/* 样式表查找器的上下文数据结构 */
typedef struct NamesFinderRec_ {
	int level;			/**< 当前选择器层级 */
	int class_i;			/**< 当前处理到第几个类名 */
	int status_i;			/**< 当前处理到第几个状态名（伪类名） */
	int name_i;			/**< 选择器名称从第几个字符开始 */
	char name[MAX_NAME_LEN];	/**< 选择器名称缓存 */
	LCUI_SelectorNode node;		/**< 针对的选择器结点 */
} NamesFinderRec, *NamesFinder;

/** 样式链接记录组 */
typedef struct StyleLinkGroupRec_ {
	dict_t *links;             /**< 样式链接表 */
	char *name;              /**< 选择器名称 */
	LCUI_SelectorNode snode; /**< 选择器结点 */
} StyleLinkGroupRec, *StyleLinkGroup;

/** 样式结点记录 */
typedef struct StyleNodeRec_ {
	int rank;		/**< 权值，决定优先级 */
	int batch_num;		/**< 批次号 */
	char *space;		/**< 所属的空间 */
	char *selector;		/**< 选择器 */
	LCUI_StyleList list;	/**< 样式表 */
	list_node_t node;	/**< 在链表中的结点 */
} StyleNodeRec, *StyleNode;

/** 样式链接记录 */
typedef struct StyleLinkRec_ {
	char *selector;		/**< 选择器 */
	StyleLinkGroup group;	/**< 所属组 */
	list_t styles;	/**< 作用于当前选择器的样式 */
	dict_t *parents;		/**< 父级节点 */
} StyleLinkRec, *StyleLink;

static struct {
	LCUI_BOOL active;
	list_t groups;		/**< 样式组列表 */
	dict_t *cache;			/**< 样式表缓存，以选择器的 hash 值索引 */
	dict_t *names;			/**< 样式属性名称表，以值的名称索引 */
	dict_t *value_keys;		/**< 样式属性值表，以值的名称索引 */
	dict_t *value_names;		/**< 样式属性值名称表，以值索引 */
	dict_type_t names_dict;		/**< 样式属性名称表的类型 */
	dict_type_t value_keys_dict;	/**< 样式属性值表的类型 */
	dict_type_t value_names_dict;	/**< 样式属性值名称表的类型 */
	dict_type_t style_link_dict;	/**< 样式链接表的类型 */
	dict_type_t style_group_dict;	/**< 样式组的类型 */
	dict_type_t cache_dict;		/**< 样式表缓存的类型 */
	strpool_t *strpool;		/**< 字符串池 */
	int count;			/**< 当前记录的属性数量 */
} library;

/** 样式字符串值与标识码 */
typedef struct KeyNameGroupRec_ {
	int key;
	char *name;
} KeyNameGroupRec, *KeyNameGroup;

/* clang-format off */

static KeyNameGroupRec style_name_map[] = {
	{ key_visibility, "visibility" },
	{ key_width, "width" },
	{ key_height, "height" },
	{ key_min_width, "min-width" },
	{ key_min_height, "min-height" },
	{ key_max_width, "max-width" },
	{ key_max_height, "max-height" },
	{ key_display, "display" },
	{ key_z_index, "z-index" },
	{ key_top, "top" },
	{ key_right, "right" },
	{ key_left, "left" },
	{ key_bottom, "bottom" },
	{ key_position, "position" },
	{ key_opacity, "opacity" },
	{ key_vertical_align, "vertical-align" },
	{ key_background_color, "background-color" },
	{ key_background_position, "background-position" },
	{ key_background_size, "background-size" },
	{ key_background_image, "background-image" },
	{ key_padding_left, "padding-left" },
	{ key_padding_right, "padding-right" },
	{ key_padding_top, "padding-top" },
	{ key_padding_bottom, "padding-bottom" },
	{ key_margin_left, "margin-left" },
	{ key_margin_right, "margin-right" },
	{ key_margin_top, "margin-top" },
	{ key_margin_bottom, "margin-bottom" },
	{ key_border_top_color, "border-top-color" },
	{ key_border_right_color, "border-right-color" },
	{ key_border_bottom_color, "border-bottom-color" },
	{ key_border_left_color, "border-left-color" },
	{ key_border_top_width, "border-top-width" },
	{ key_border_right_width, "border-right-width" },
	{ key_border_bottom_width, "border-bottom-width" },
	{ key_border_left_width, "border-left-width" },
	{ key_border_top_width, "border-top-width" },
	{ key_border_right_width, "border-right-width" },
	{ key_border_bottom_width, "border-bottom-width" },
	{ key_border_left_width, "border-left-width" },
	{ key_border_top_style, "border-top-style" },
	{ key_border_right_style, "border-right-style" },
	{ key_border_bottom_style, "border-bottom-style" },
	{ key_border_left_style, "border-left-style" },
	{ key_border_top_left_radius, "border-top-left-radius" },
	{ key_border_top_right_radius, "border-top-right-radius" },
	{ key_border_bottom_left_radius, "border-bottom-left-radius" },
	{ key_border_bottom_right_radius, "border-bottom-right-radius" },
	{ key_box_shadow_x, "box-shadow-x" },
	{ key_box_shadow_y, "box-shadow-y" },
	{ key_box_shadow_blur, "box-shadow-blur" },
	{ key_box_shadow_spread, "box-shadow-spread" },
	{ key_box_shadow_color, "box-shadow-color" },
	{ key_pointer_events, "pointer-events" },
	{ key_focusable, "focusable" },
	{ key_box_sizing, "box-sizing" },
	{ key_flex_basis, "flex-basis" },
	{ key_flex_direction, "flex-direction" },
	{ key_flex_grow, "flex-grow" },
	{ key_flex_shrink, "flex-shrink" },
	{ key_flex_wrap, "flex-wrap" },
	{ key_justify_content, "justify-content" },
	{ key_align_content, "align-content" },
	{ key_align_items, "align-items" }
};

/** 样式字符串与标识码的映射表 */
static KeyNameGroupRec style_value_map[] = {
	{ SV_NONE, "none" },
	{ SV_AUTO, "auto" },
	{ SV_INHERIT, "inherit" },
	{ SV_INITIAL, "initial" },
	{ SV_CONTAIN, "contain" },
	{ SV_COVER, "cover" },
	{ SV_LEFT, "left" },
	{ SV_CENTER, "center" },
	{ SV_RIGHT, "right" },
	{ SV_TOP, "top" },
	{ SV_TOP_LEFT, "top left" },
	{ SV_TOP_CENTER, "top center" },
	{ SV_TOP_RIGHT, "top right" },
	{ SV_MIDDLE, "middle" },
	{ SV_CENTER_LEFT, "center left" },
	{ SV_CENTER_CENTER, "center center" },
	{ SV_CENTER_RIGHT, "center right" },
	{ SV_BOTTOM, "bottom" },
	{ SV_BOTTOM_LEFT, "bottom left" },
	{ SV_BOTTOM_CENTER, "bottom center" },
	{ SV_BOTTOM_RIGHT, "bottom right" },
	{ SV_SOLID, "solid" },
	{ SV_DOTTED, "dotted" },
	{ SV_DOUBLE, "double" },
	{ SV_DASHED, "dashed" },
	{ SV_CONTENT_BOX, "content-box" },
	{ SV_PADDING_BOX, "padding-box" },
	{ SV_BORDER_BOX, "border-box" },
	{ SV_GRAPH_BOX, "graph-box" },
	{ SV_STATIC, "static" },
	{ SV_RELATIVE, "relative" },
	{ SV_ABSOLUTE, "absolute" },
	{ SV_BLOCK, "block" },
	{ SV_INLINE_BLOCK, "inline-block" },
	{ SV_FLEX, "flex" },
	{ SV_NORMAL, "normal" },
	{ SV_FLEX_START, "flex-start" },
	{ SV_FLEX_END, "flex-end" },
	{ SV_STRETCH, "stretch" },
	{ SV_SPACE_BETWEEN, "space-between" },
	{ SV_SPACE_AROUND, "space-around" },
	{ SV_SPACE_EVENLY, "space-evenly" },
	{ SV_NOWRAP, "nowrap" },
	{ SV_WRAP, "wrap" },
	{ SV_ROW, "row" },
	{ SV_COLUMN, "column" }
};

static int LCUI_DirectAddStyleName(int key, const char *name)
{
	if(!library.names->type->val_dup){
		return -2;
	}
	return dict_add(library.names, &key, name);
}

int LCUI_SetStyleName(int key, const char *name)
{
	char *newname;
	dict_entry_t *entry;
	entry = dict_find(library.names, &key);
	if (entry) {
		newname = strdup2(name);
		free(entry->v.val);
		entry->v.val = newname;
		return 0;
	}
	return -1;
}

int LCUI_AddCSSPropertyName(const char *name)
{
	int key;
	key = library.count++;
	if (LCUI_DirectAddStyleName(key, name) != 0) {
		--library.count;
		return -1;
	}
	return key;
}

const char *LCUI_GetStyleName(int key)
{
	return dict_fetch_value(library.names, &key);
}

static KeyNameGroup CreateKeyNameGroup(int key, const char *name)
{
	KeyNameGroup group;
	group = malloc(sizeof(KeyNameGroupRec));
	group->name = strdup2(name);
	group->key = key;
	return group;
}

static void DestroyKeyNameGroup(void *data)
{
	KeyNameGroup group = data;
	free(group->name);
	free(group);
}

static void KeyNameGroupDestructor(void *privdata, void *data)
{
	DestroyKeyNameGroup(data);
}

int LCUI_AddStyleValue(int key, const char *name)
{
	KeyNameGroup group = CreateKeyNameGroup(key, name);
	if (dict_add(library.value_keys, group->name, group)) {
		DestroyKeyNameGroup(group);
		return -1;
	}
	if (dict_add(library.value_names, &group->key, group)) {
		DestroyKeyNameGroup(group);
		return -2;
	}
	return 0;
}

int LCUI_GetStyleValue(const char *str)
{
	KeyNameGroup group;
	group = dict_fetch_value(library.value_keys, str);
	if (group) {
		return group->key;
	}
	return -1;
}

const char *LCUI_GetStyleValueName(int val)
{
	KeyNameGroup group;
	group = dict_fetch_value(library.value_names, &val);
	if (group) {
		return group->name;
	}
	return NULL;
}

int LCUI_GetStyleTotal(void)
{
	return library.count;
}

LCUI_BOOL SelectorNode_Match(LCUI_SelectorNode sn1, LCUI_SelectorNode sn2)
{
	int i, j;
	if (sn2->id) {
		if (!sn1->id || strcmp(sn1->id, sn2->id) != 0) {
			return FALSE;
		}
	}
	if (sn2->type && strcmp(sn2->type, "*") != 0) {
		if (!sn1->type || strcmp(sn1->type, sn2->type) != 0) {
			return FALSE;
		}
	}
	if (sn2->classes) {
		if (!sn1->classes) {
			return FALSE;
		}
		for (i = 0; sn2->classes[i]; ++i) {
			for (j = 0; sn1->classes[j]; ++j) {
				if (strcmp(sn2->classes[i], sn1->classes[i]) ==
				    0) {
					j = -1;
					break;
				}
			}
			if (j != -1) {
				return FALSE;
			}
		}
	}
	if (sn2->status) {
		if (!sn1->status) {
			return FALSE;
		}
		for (i = 0; sn2->status[i]; ++i) {
			for (j = 0; sn1->status[j]; ++j) {
				if (strcmp(sn2->status[i], sn1->status[i]) ==
				    0) {
					j = -1;
					break;
				}
			}
			if (j != -1) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

static void SelectorNode_Copy(LCUI_SelectorNode dst, LCUI_SelectorNode src)
{
	int i;
	dst->id = src->id ? strdup2(src->id) : NULL;
	dst->type = src->type ? strdup2(src->type) : NULL;
	dst->fullname = src->fullname ? strdup2(src->fullname) : NULL;
	if (src->classes) {
		for (i = 0; src->classes[i]; ++i) {
			strlist_sorted_add(&dst->classes, src->classes[i]);
		}
	}
	if (src->status) {
		for (i = 0; src->status[i]; ++i) {
			strlist_sorted_add(&dst->status, src->status[i]);
		}
	}
}

void SelectorNode_Delete(LCUI_SelectorNode node)
{
	if (node->type) {
		free(node->type);
		node->type = NULL;
	}
	if (node->id) {
		free(node->id);
		node->id = NULL;
	}
	if (node->classes) {
		strlist_free(node->classes);
		node->classes = NULL;
	}
	if (node->status) {
		strlist_free(node->status);
		node->status = NULL;
	}
	if (node->fullname) {
		free(node->fullname);
		node->fullname = NULL;
	}
	free(node);
}

void Selector_Delete(LCUI_Selector s)
{
	int i;
	for (i = 0; i < MAX_SELECTOR_DEPTH; ++i) {
		if (!s->nodes[i]) {
			break;
		}
		SelectorNode_Delete(s->nodes[i]);
		s->nodes[i] = NULL;
	}
	s->rank = 0;
	s->length = 0;
	s->batch_num = 0;
	free(s->nodes);
	free(s);
}

LCUI_StyleList StyleList(void)
{
	LCUI_StyleList list;

	list = malloc(sizeof(LCUI_StyleListRec));
	list_create(list);
	return list;
}

void DestroyStyle(LCUI_Style s)
{
	switch (s->type) {
	case LCUI_STYPE_STRING:
		if (s->is_valid && s->string) {
			free(s->string);
		}
		s->string = NULL;
		break;
	case LCUI_STYPE_WSTRING:
		if (s->is_valid && s->string) {
			free(s->wstring);
		}
		s->wstring = NULL;
		break;
	default:
		break;
	}
	s->is_valid = FALSE;
}

void MergeStyle(LCUI_Style dst, LCUI_Style src)
{
	switch (src->type) {
	case LCUI_STYPE_STRING:
		dst->string = strdup2(src->string);
		break;
	case LCUI_STYPE_WSTRING:
		dst->wstring = wcsdup2(src->wstring);
		break;
	default:
		*dst = *src;
		break;
	}
	dst->is_valid = TRUE;
	dst->type = src->type;
}

static void DeleteStyleListNode(LCUI_StyleListNode node)
{
	DestroyStyle(&node->style);
	free(node);
}

void StyleList_Delete(LCUI_StyleList list)
{
	list_destroy_without_node(list, (FuncPtr)DeleteStyleListNode);
	free(list);
}

LCUI_StyleSheet StyleSheet(void)
{
	LCUI_StyleSheet ss;

	ss = NEW(LCUI_StyleSheetRec, 1);
	if (!ss) {
		return ss;
	}
	ss->length = LCUI_GetStyleTotal();
	ss->sheet = NEW(LCUI_StyleRec, ss->length + 1);
	return ss;
}

void StyleSheet_Clear(LCUI_StyleSheet ss)
{
	int i;

	for (i = 0; i < ss->length; ++i) {
		DestroyStyle(&ss->sheet[i]);
	}
}

void StyleSheet_Delete(LCUI_StyleSheet ss)
{
	StyleSheet_Clear(ss);
	free(ss->sheet);
	free(ss);
}

LCUI_StyleListNode StyleList_GetNode(LCUI_StyleList list, int key)
{
	list_node_t *node;
	LCUI_StyleListNode snode;

	for (list_each(node, list)) {
		snode = node->data;
		if (snode->key == key) {
			return snode;
		}
	}
	return NULL;
}

int StyleList_RemoveNode(LCUI_StyleList list, int key)
{
	list_node_t *node;
	LCUI_StyleListNode snode;

	for (list_each(node, list)) {
		snode = node->data;
		if (snode->key == key) {
			list_unlink(list, node);
			DestroyStyle(&snode->style);
			free(snode);
			return 0;
		}
	}
	return -1;
}

LCUI_StyleListNode StyleList_AddNode(LCUI_StyleList list, int key)
{
	LCUI_StyleListNode node;

	node = malloc(sizeof(LCUI_StyleListNodeRec));
	node->key = key;
	node->style.is_valid = FALSE;
	node->style.type = LCUI_STYPE_NONE;
	node->node.data = node;
	list_append_node(list, &node->node);
	return node;
}

static unsigned StyleList_Merge(LCUI_StyleList list, const LCUI_StyleSheetRec *sheet)
{
	int i, count;
	LCUI_StyleListNode node;

	for (count = 0, i = 0; i < sheet->length; ++i) {
		if (!sheet->sheet[i].is_valid) {
			continue;
		}
		node = StyleList_AddNode(list, i);
		MergeStyle(&node->style, &sheet->sheet[i]);
		count += 1;
	}
	return count;
}

int StyleSheet_Merge(LCUI_StyleSheet dest, const LCUI_StyleSheetRec *src)
{
	int i;
	size_t size;
	LCUI_Style s;

	if (src->length > dest->length) {
		size = sizeof(LCUI_StyleRec) * src->length;
		s = realloc(dest->sheet, size);
		if (!s) {
			return -1;
		}
		for (i = dest->length; i < src->length; ++i) {
			s[i].is_valid = FALSE;
		}
		dest->sheet = s;
		dest->length = src->length;
	}
	for (i = 0; i < src->length; ++i) {
		if (src->sheet[i].is_valid && !dest->sheet[i].is_valid) {
			MergeStyle(&dest->sheet[i], &src->sheet[i]);
		}
	}
	return 0;
}

int StyleSheet_MergeList(LCUI_StyleSheet ss, LCUI_StyleList list)
{
	LCUI_Style s;
	LCUI_StyleListNode snode;
	list_node_t *node;
	size_t size;
	int i = 0, count = 0;

	for (list_each(node, list)) {
		snode = node->data;
		if (snode->key > ss->length) {
			size = sizeof(LCUI_StyleRec) * (snode->key + 1);
			s = realloc(ss->sheet, size);
			if (!s) {
				return -1;
			}
			for (i = ss->length; i <= snode->key; ++i) {
				s[i].is_valid = FALSE;
			}
			ss->sheet = s;
			ss->length = snode->key + 1;
		}
		if (!ss->sheet[snode->key].is_valid && snode->style.is_valid) {
			MergeStyle(&ss->sheet[snode->key], &snode->style);
			++count;
		}
	}
	return (int)count;
}

int StyleSheet_Replace(LCUI_StyleSheet dest, const LCUI_StyleSheetRec *src)
{
	int i;
	LCUI_Style s;
	size_t count, size;

	if (src->length > dest->length) {
		size = sizeof(LCUI_StyleRec) * src->length;
		s = realloc(dest->sheet, size);
		if (!s) {
			return -1;
		}
		for (i = dest->length; i < src->length; ++i) {
			s[i].is_valid = FALSE;
		}
		dest->sheet = s;
		dest->length = src->length;
	}
	for (count = 0, i = 0; i < src->length; ++i) {
		if (!src->sheet[i].is_valid) {
			continue;
		}
		DestroyStyle(&dest->sheet[i]);
		MergeStyle(&dest->sheet[i], &src->sheet[i]);
		++count;
	}
	return (int)count;
}

/** 初始化样式表查找器 */
static void NamesFinder_Init(NamesFinder sfinder, LCUI_SelectorNode snode)
{
	sfinder->level = 0;
	sfinder->class_i = 0;
	sfinder->name_i = 0;
	sfinder->status_i = 0;
	sfinder->name[0] = 0;
	sfinder->node = snode;
}

/** 销毁样式表查找器 */
static void NamesFinder_Destroy(NamesFinder sfinder)
{
	sfinder->name_i = 0;
	sfinder->name[0] = 0;
	sfinder->class_i = 0;
	sfinder->status_i = 0;
	sfinder->node = NULL;
	sfinder->level = LEVEL_NONE;
}

/* 生成选择器全名列表 */
static int NamesFinder_Find(NamesFinder sfinder, list_t *list)
{
	size_t len, old_len;
	int i, old_level, count = 0;
	char *fullname = sfinder->name + sfinder->name_i;

	old_len = len = strlen(fullname);
	old_level = sfinder->level;
	switch (sfinder->level) {
	case LEVEL_TYPE:
		/* 按类型选择器生成选择器全名 */
		if (!sfinder->node->type) {
			return 0;
		}
		strcpy(fullname, sfinder->node->type);
		list_append(list, strdup2(fullname));
		break;
	case LEVEL_ID:
		/* 按ID选择器生成选择器全名 */
		if (!sfinder->node->id) {
			return 0;
		}
		fullname[len++] = '#';
		fullname[len] = 0;
		strcpy(fullname + len, sfinder->node->id);
		list_append(list, strdup2(fullname));
		break;
	case LEVEL_CLASS:
		if (!sfinder->node->classes) {
			return 0;
		}
		/* 按类选择器生成选择器全名
		 * 假设当前选择器全名为：textview#main-btn-text，且有 .a .b .c
		 * 这三个类，那么下面的处理将会拆分成以下三个选择器：
		 * textview#test-text.a
		 * textview#test-text.b
		 * textview#test-text.a
		 */
		fullname[len++] = '.';
		for (i = 0; sfinder->node->classes[i]; ++i) {
			sfinder->level += 1;
			sfinder->class_i = i;
			strcpy(fullname + len, sfinder->node->classes[i]);
			list_append(list, strdup2(fullname));
			/* 将当前选择器名与其它层级的选择器名组合 */
			while (sfinder->level < LEVEL_TOTAL_NUM) {
				count += NamesFinder_Find(sfinder, list);
				sfinder->level += 1;
			}
			sfinder->level = LEVEL_CLASS;
		}
		sfinder->level = LEVEL_CLASS;
		fullname[old_len] = 0;
		sfinder->class_i = 0;
		return count;
	case LEVEL_CLASS_2:
		if (!sfinder->node->classes) {
			return 0;
		}
		/* 按类选择器生成选择器全名，结果类似于这样：
		 * textview#test-text.a.b
		 * textview#test-text.a.c
		 * textview#test-text.b.c
		 * textview#test-text.a.b.c
		 */
		fullname[len++] = '.';
		for (i = 0; sfinder->node->classes[i]; ++i) {
			if (i <= sfinder->class_i) {
				continue;
			}
			strcpy(fullname + len, sfinder->node->classes[i]);
			list_append(list, strdup2(fullname));
			sfinder->class_i = i;
			count += NamesFinder_Find(sfinder, list);
			sfinder->class_i = 0;
			sfinder->level = LEVEL_STATUS;
			/**
			 * 递归拼接伪类名，例如：
			 * textview#main-btn-text:active
			 */
			count += NamesFinder_Find(sfinder, list);
			sfinder->level = LEVEL_CLASS_2;
		}
		fullname[old_len] = 0;
		sfinder->level = LEVEL_CLASS_2;
		return count;
	case LEVEL_STATUS:
		if (!sfinder->node->status) {
			return 0;
		}
		fullname[len++] = ':';
		sfinder->level = LEVEL_STATUS_2;
		/**
		 * 按伪类选择器生成选择器全名
		 * 假设当前选择器全名为：textview#main-btn-text:hover:focus:active
		 * 那么下面的循环会将它拆分为以下几个选择器：
		 * textview#main-btn-text:active
		 * textview#main-btn-text:active:focus
		 * textview#main-btn-text:active:focus:hover
		 * textview#main-btn-text:active:hover
		 * textview#main-btn-text:focus
		 * textview#main-btn-text:focus:hover
		 * textview#main-btn-text:hover
		 */
		for (i = 0; sfinder->node->status[i]; ++i) {
			sfinder->status_i = i;
			strcpy(fullname + len, sfinder->node->status[i]);
			list_append(list, strdup2(fullname));
			/**
			 * 递归调用，以一层层拼接出像下面这样的选择器：
			 * textview#main-btn-text:active:focus:hover
			 */
			count += NamesFinder_Find(sfinder, list);
		}
		sfinder->level = LEVEL_STATUS;
		fullname[old_len] = 0;
		sfinder->status_i = 0;
		return count;
	case LEVEL_STATUS_2:
		if (!sfinder->node->status) {
			return 0;
		}
		/** 按伪类选择器生成选择器全名 */
		for (i = 0; sfinder->node->status[i]; ++i) {
			if (i <= sfinder->status_i) {
				continue;
			}
			fullname[len] = ':';
			strcpy(fullname + len + 1, sfinder->node->status[i]);
			list_append(list, strdup2(fullname));
			sfinder->status_i = i;
			count += NamesFinder_Find(sfinder, list);
			sfinder->status_i = 0;
		}
		fullname[old_len] = 0;
		return count;
	default:
		break;
	}
	for (i = sfinder->level + 1; i < LEVEL_TOTAL_NUM; ++i) {
		if (i == LEVEL_STATUS_2 || i == LEVEL_CLASS_2) {
			continue;
		}
		sfinder->level = i;
		count += NamesFinder_Find(sfinder, list);
	}
	fullname[old_len] = 0;
	sfinder->level = old_level;
	return count;
}

static int SelectorNode_Save(LCUI_SelectorNode node, const char *name, int len,
			     char type)
{
	char *str;
	if (len < 1) {
		return 0;
	}
	switch (type) {
	case 0:
		if (node->type) {
			break;
		}
		len += 1;
		str = malloc(sizeof(char) * len);
		strncpy(str, name, len);
		node->type = str;
		return TYPE_RANK;
	case ':':
		if (strlist_sorted_add(&node->status, name) == 0) {
			return PCLASS_RANK;
		}
		break;
	case '.':
		if (strlist_sorted_add(&node->classes, name) == 0) {
			return CLASS_RANK;
		}
		break;
	case '#':
		if (node->id) {
			break;
		}
		len += 1;
		str = malloc(sizeof(char) * len);
		strncpy(str, name, len);
		node->id = str;
		return ID_RANK;
	default:
		break;
	}
	return 0;
}

int SelectorNode_GetNames(LCUI_SelectorNode sn, list_t *names)
{
	int count;
	NamesFinderRec sfinder;
	NamesFinder_Init(&sfinder, sn);
	count = NamesFinder_Find(&sfinder, names);
	NamesFinder_Destroy(&sfinder);
	return count;
}

int SelectorNode_Update(LCUI_SelectorNode node)
{
	size_t i, len = 0;
	char *fullname;

	node->rank = 0;
	if (node->id) {
		len += strlen(node->id) + 1;
		node->rank += ID_RANK;
	}
	if (node->type) {
		len += strlen(node->type) + 1;
		node->rank += TYPE_RANK;
	}
	if (node->classes) {
		for (i = 0; node->classes[i]; ++i) {
			len += strlen(node->classes[i]) + 1;
			node->rank += CLASS_RANK;
		}
	}
	if (node->status) {
		for (i = 0; node->status[i]; ++i) {
			len += strlen(node->status[i]) + 1;
			node->rank += PCLASS_RANK;
		}
	}
	if (len > 0) {
		fullname = malloc(sizeof(char) * (len + 1));
		if (!fullname) {
			return -ENOMEM;
		}
		fullname[0] = 0;
		if (node->type) {
			strcat(fullname, node->type);
		}
		if (node->id) {
			strcat(fullname, "#");
			strcat(fullname, node->id);
		}
		if (node->classes) {
			for (i = 0; node->classes[i]; ++i) {
				strcat(fullname, ".");
				strcat(fullname, node->classes[i]);
			}
			len += 1;
		}
		if (node->status) {
			for (i = 0; node->status[i]; ++i) {
				strcat(fullname, ":");
				strcat(fullname, node->status[i]);
			}
			len += 1;
		}
	} else {
		fullname = NULL;
	}
	if (node->fullname) {
		free(node->fullname);
	}
	node->fullname = fullname;
	return 0;
}

void Selector_Update(LCUI_Selector s)
{
	int i;
	const unsigned char *p;
	unsigned int hash = 5381;
	for (i = 0; i < s->length; ++i) {
		p = (unsigned char *)s->nodes[i]->fullname;
		while (*p) {
			hash = ((hash << 5) + hash) + (*p++);
		}
	}
	s->hash = hash;
}

int Selector_AppendNode(LCUI_Selector selector, LCUI_SelectorNode node)
{
	const unsigned char *p;

	if (selector->length >= MAX_SELECTOR_DEPTH) {
		logger_warning("[css] warning: the number of nodes in the "
			       "selector has exceeded the %d limit\n",
			       MAX_SELECTOR_DEPTH);
		return -1;
	}
	selector->nodes[selector->length++] = node;
	selector->nodes[selector->length] = NULL;
	p = (unsigned char *)node->fullname;
	while (*p) {
		selector->hash = ((selector->hash << 5) + selector->hash) + (*p++);
	}
	return 0;
}

LCUI_Selector Selector(const char *selector)
{
	const char *p;
	int ni, si, rank;
	static int batch_num = 0;
	char type = 0, name[MAX_NAME_LEN];
	LCUI_BOOL is_saving = FALSE;
	LCUI_SelectorNode node = NULL;
	LCUI_Selector s = NEW(LCUI_SelectorRec, 1);

	s->batch_num = ++batch_num;
	s->nodes = NEW(LCUI_SelectorNode, MAX_SELECTOR_DEPTH);
	if (!selector) {
		s->length = 0;
		s->nodes[0] = NULL;
		return s;
	}
	for (ni = 0, si = 0, p = selector; *p; ++p) {
		if (!node && is_saving) {
			node = NEW(LCUI_SelectorNodeRec, 1);
			if (si >= MAX_SELECTOR_DEPTH) {
				logger_warning(
				    "%s: selector node list is too long.\n",
				    selector);
				return NULL;
			}
			s->nodes[si] = node;
		}
		switch (*p) {
		case ':':
		case '.':
		case '#':
			if (!is_saving) {
				is_saving = TRUE;
				type = *p;
				continue;
			}
			/* 保存上个结点 */
			rank = SelectorNode_Save(node, name, ni, type);
			is_saving = TRUE;
			type = *p;
			if (rank > 0) {
				s->rank += rank;
				ni = 0;
				continue;
			}
			logger_error("%s: invalid selector node at %ld.\n",
				     selector, p - selector - ni);
			SelectorNode_Delete(node);
			node = NULL;
			ni = 0;
			continue;
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			if (!is_saving) {
				ni = 0;
				node = NULL;
				continue;
			}
			is_saving = FALSE;
			rank = SelectorNode_Save(node, name, ni, type);
			if (rank > 0) {
				SelectorNode_Update(node);
				s->rank += rank;
				node = NULL;
				ni = 0;
				si++;
				continue;
			}
			logger_error("%s: invalid selector node at %ld.\n",
				     selector, p - selector - ni);
			SelectorNode_Delete(node);
			node = NULL;
			ni = 0;
			continue;
		default:
			break;
		}
		if (*p == '-' || *p == '_' || *p == '*' ||
		    (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') ||
		    (*p >= '0' && *p <= '9')) {
			if (!is_saving) {
				type = 0;
				is_saving = TRUE;
			}
			name[ni++] = *p;
			name[ni] = 0;
			continue;
		}
		logger_warning("%s: unknown char 0x%02x at %ld.\n",
			       selector, *p, p - selector);
		return NULL;
	}
	if (is_saving) {
		if (!node) {
			node = NEW(LCUI_SelectorNodeRec, 1);
			if (si >= MAX_SELECTOR_DEPTH) {
				logger_warning(
				    "%s: selector node list is too long.\n",
				    selector);
				return NULL;
			}
			s->nodes[si] = node;
		}
		rank = SelectorNode_Save(s->nodes[si], name, ni, type);
		if (rank > 0) {
			SelectorNode_Update(s->nodes[si]);
			s->rank += rank;
			si++;
		} else {
			SelectorNode_Delete(s->nodes[si]);
		}
	}
	s->nodes[si] = NULL;
	s->length = si;
	Selector_Update(s);
	return s;
}

LCUI_Selector Selector_Copy(LCUI_Selector selector)
{
	int i;
	LCUI_Selector s;

	s = Selector(NULL);
	for (i = 0; i < selector->length; ++i) {
		s->nodes[i] = NEW(LCUI_SelectorNodeRec, 1);
		SelectorNode_Copy(s->nodes[i], selector->nodes[i]);
	}
	s->nodes[selector->length] = NULL;
	s->length = selector->length;
	s->hash = selector->hash;
	s->rank  = selector->rank;
	s->batch_num = selector->batch_num;
	return s;
}

static void DeleteStyleNode(StyleNode node)
{
	if (node->space) {
		strpool_free_str(node->space);
		node->space = NULL;
	}
	if (node->selector) {
		free(node->selector);
		node->selector = NULL;
	}
	StyleList_Delete(node->list);
	node->list = NULL;
	free(node);
}

static StyleLink CreateStyleLink(void)
{
	StyleLink link = NEW(StyleLinkRec, 1);
	static dict_type_t t;

	dict_init_string_copy_key_type(&t);
	link->group = NULL;
	list_create(&link->styles);
	link->parents = dict_create(&t, NULL);
	return link;
}

static void DeleteStyleLink(StyleLink link)
{
	dict_destroy(link->parents);
	list_destroy_without_node(&link->styles, (FuncPtr)DeleteStyleNode);
	free(link->selector);
	link->selector = NULL;
	link->parents = NULL;
	link->group = NULL;
	free(link);
}

static StyleLinkGroup CreateStyleLinkGroup(LCUI_SelectorNode snode)
{
	StyleLinkGroup group = NEW(StyleLinkGroupRec, 1);
	group->snode = NEW(LCUI_SelectorNodeRec, 1);
	SelectorNode_Copy(group->snode, snode);
	group->name = group->snode->fullname;
	group->links = dict_create(&library.style_link_dict, NULL);
	return group;
}

static void DeleteStyleLinkGroup(StyleLinkGroup group)
{
	dict_type_t *dtype;
	dtype = group->links->priv_data;
	SelectorNode_Delete(group->snode);
	dict_destroy(group->links);
	free(dtype);
	free(group);
}

static void StyleLinkGroupDestructor(void *privdata, void *data)
{
	DeleteStyleLinkGroup(data);
}

static void InitStyleGroupDict(void)
{
	dict_type_t *dt = &library.style_group_dict;

	dict_init_string_copy_key_type(dt);
	dt->val_destructor = StyleLinkGroupDestructor;
}

static dict_t *CreateStyleGroup(void)
{
	return dict_create(&library.style_group_dict, NULL);
}

static void DeleteStyleGroup(dict_t *dict)
{
	dict_destroy(dict);
}

/** 根据选择器，选中匹配的样式表 */
static LCUI_StyleList LCUI_SelectStyleList(LCUI_Selector selector,
					   const char *space)
{
	int i, right;
	StyleLink link;
	StyleNode snode;
	StyleLinkGroup slg;
	LCUI_SelectorNode sn;
	dict_t *group, *parents;
	char buf[MAX_SELECTOR_LEN];
	char fullname[MAX_SELECTOR_LEN];

	link = NULL;
	parents = NULL;
	for (i = 0, right = selector->length - 1; right >= 0; --right, ++i) {
		group = list_get(&library.groups, i);
		if (!group) {
			group = CreateStyleGroup();
			list_append(&library.groups, group);
		}
		sn = selector->nodes[right];
		slg = dict_fetch_value(group, sn->fullname);
		if (!slg) {
			slg = CreateStyleLinkGroup(sn);
			dict_add(group, sn->fullname, slg);
		}
		if (i == 0) {
			strcpy(fullname, "*");
		} else {
			strcpy(fullname, buf);
		}
		link = dict_fetch_value(slg->links, fullname);
		if (!link) {
			link = CreateStyleLink();
			link->group = slg;
			link->selector = strdup2(fullname);
			dict_add(slg->links, fullname, link);
		}
		if (i == 0) {
			strcpy(buf, sn->fullname);
			strcpy(fullname, buf);
		} else {
			strcpy(fullname, buf);
			sprintf(buf, "%s %s", sn->fullname, fullname);
		}
		/* 如果有上一级的父链接记录，则将当前链接添加进去 */
		if (parents) {
			if (!dict_fetch_value(parents, sn->fullname)) {
				dict_add(parents, sn->fullname, link);
			}
		}
		parents = link->parents;
	}
	if (!link) {
		return NULL;
	}
	snode = NEW(StyleNodeRec, 1);
	if (space) {
		snode->space = strpool_alloc_str(library.strpool, space);
		strcpy(snode->space, space);
	} else {
		snode->space = NULL;
	}
	snode->node.data = snode;
	snode->list = StyleList();
	snode->rank = selector->rank;
	snode->selector = strdup2(fullname);
	snode->batch_num = selector->batch_num;
	list_append_node(&link->styles, &snode->node);
	return snode->list;
}

int LCUI_PutStyleSheet(LCUI_Selector selector, LCUI_StyleSheet in_ss,
		       const char *space)
{
	LCUI_StyleList list;
	dict_empty(library.cache, NULL);
	list = LCUI_SelectStyleList(selector, space);
	if (list) {
		StyleList_Merge(list, in_ss);
	}
	return 0;
}

static size_t StyleLink_GetStyleSheets(StyleLink link, list_t *outlist)
{
	size_t i;
	LCUI_BOOL found;
	StyleNode snode, out_snode;
	list_node_t *node, *out_node;

	if (!outlist) {
		return link->styles.length;
	}
	for (list_each(node, &link->styles)) {
		i = 0;
		found = FALSE;
		snode = node->data;
		for (list_each(out_node, outlist)) {
			out_snode = out_node->data;
			if (snode->rank > out_snode->rank) {
				found = TRUE;
				break;
			}
			if (snode->rank != out_snode->rank) {
				i += 1;
				continue;
			}
			if (snode->batch_num > out_snode->batch_num) {
				found = TRUE;
				break;
			}
			i += 1;
		}
		if (found) {
			list_insert(outlist, i, snode);
		} else {
			list_append(outlist, snode);
		}
	}
	return link->styles.length;
}

static size_t LCUI_FindStyleSheetFromLink(StyleLink link, LCUI_Selector s,
					  int i, list_t *list)
{
	size_t count = 0;
	StyleLink parent;
	list_t names;
	list_node_t *node;
	LCUI_SelectorNode sn;

	list_create(&names);
	count += StyleLink_GetStyleSheets(link, list);
	while (--i >= 0) {
		sn = s->nodes[i];
		SelectorNode_GetNames(sn, &names);
		for (list_each(node, &names)) {
			parent = dict_fetch_value(link->parents, node->data);
			if (!parent) {
				continue;
			}
			count +=
			    LCUI_FindStyleSheetFromLink(parent, s, i, list);
		}
		list_destroy(&names, free);
	}
	return count;
}

int LCUI_FindStyleSheetFromGroup(int group, const char *name, LCUI_Selector s,
				 list_t *list)
{
	int i;
	size_t count;
	dict_t *groups;
	StyleLinkGroup slg;
	list_node_t *node;
	list_t names;

	groups = list_get(&library.groups, group);
	if (!groups || s->length < 1) {
		return 0;
	}
	count = 0;
	i = s->length - 1;
	list_create(&names);
	if (name) {
		list_append(&names, strdup2(name));
	} else {
		SelectorNode_GetNames(s->nodes[i], &names);
		list_append(&names, strdup2("*"));
	}
	for (list_each(node, &names)) {
		dict_entry_t *entry;
		dict_iterator_t *iter;
		char *name = node->data;
		slg = dict_fetch_value(groups, name);
		if (!slg) {
			continue;
		}
		iter = dict_get_iterator(slg->links);
		while ((entry = dict_next(iter))) {
			StyleLink link = dict_get_val(entry);
			count += LCUI_FindStyleSheetFromLink(link, s, i, list);
		}
		dict_destroy_iterator(iter);
	}
	list_destroy(&names, free);
	return (int)count;
}

static void PrintStyleName(int key)
{
	const char *name;

	name = LCUI_GetStyleName(key);
	if (name) {
		logger_debug("\t%s", name);
	} else {
		logger_debug("\t<unknown style %d>", key);
	}
	logger_debug("%s: ", key > STYLE_KEY_TOTAL ? " (+)" : "");
}

static void PrintStyleValue(LCUI_Style s)
{
	switch (s->type) {
	case LCUI_STYPE_AUTO:
		logger_debug("auto");
		break;
	case LCUI_STYPE_BOOL:
		logger_debug("%s", s->val_bool ? "true" : "false");
		break;
	case LCUI_STYPE_COLOR: {
		LCUI_Color *clr = &s->val_color;
		if (clr->alpha < 255) {
			logger_debug("rgba(%d,%d,%d,%g)", clr->r, clr->g, clr->b,
				clr->a / 255.0);
		} else {
			logger_debug("#%02x%02x%02x", clr->r, clr->g, clr->b);
		}
		break;
	}
	case LCUI_STYPE_PX:
		logger_debug("%gpx", s->val_px);
		break;
	case LCUI_STYPE_DIP:
		logger_debug("%gdip", s->val_dip);
		break;
	case LCUI_STYPE_SP:
		logger_debug("%gsp", s->val_sp);
		break;
	case LCUI_STYPE_STRING:
		logger_debug("%s", s->val_string);
		break;
	case LCUI_STYPE_WSTRING:
		logger_debug("%S", s->val_wstring);
		break;
	case LCUI_STYPE_SCALE:
		logger_debug("%g%%", s->val_scale * 100);
		break;
	case LCUI_STYPE_STYLE:
		logger_debug("%s", LCUI_GetStyleValueName(s->val_style));
		break;
	case LCUI_STYPE_INT:
		logger_debug("%d", s->val_int);
		break;
	default:
		logger_debug("%g", s->value);
		break;
	}
	logger_debug(";\n");
}

void LCUI_PrintStyleList(LCUI_StyleList list)
{
	list_node_t *node;
	LCUI_StyleListNode snode;

	for (list_each(node, list)) {
		snode = node->data;
		if (snode->style.is_valid) {
			PrintStyleName(snode->key);
			PrintStyleValue(&snode->style);
		}
	}
}

void LCUI_PrintStyleSheet(LCUI_StyleSheet ss)
{
	int key;
	LCUI_Style s;

	for (key = 0; key < ss->length; ++key) {
		s = &ss->sheet[key];
		if (s->is_valid) {
			PrintStyleName(key);
			PrintStyleValue(s);
		}
	}
}

void LCUI_PrintSelector(LCUI_Selector selector)
{
	char path[MAX_SELECTOR_LEN];
	LCUI_SelectorNode *sn;

	path[0] = 0;
	for (sn = selector->nodes; *sn; ++sn) {
		strcat(path, (*sn)->fullname);
		strcat(path, " ");
	}
	logger_debug("path: %s (rank = %d, batch_num = %d)\n", path,
		     selector->rank, selector->batch_num);
}

static void LCUI_PrintStyleLink(StyleLink link, const char *selector)
{
	dict_entry_t *entry;
	dict_iterator_t *iter;
	list_node_t *node;
	char fullname[MAX_SELECTOR_LEN];

	if (selector) {
		sprintf(fullname, "%s %s", link->group->name, selector);
	} else {
		strcpy(fullname, link->group->name);
	}
	for (list_each(node, &link->styles)) {
		StyleNode snode = node->data;
		logger_debug("\n[%s]", snode->space ? snode->space : "<none>");
		logger_debug("[rank: %d]\n%s {\n", snode->rank, fullname);
		LCUI_PrintStyleList(snode->list);
		logger_debug("}\n");
	}
	iter = dict_get_iterator(link->parents);
	while ((entry = dict_next(iter))) {
		StyleLink parent = dict_get_val(entry);
		LCUI_PrintStyleLink(parent, fullname);
	}
	dict_destroy_iterator(iter);
}

void LCUI_PrintCSSLibrary(void)
{
	dict_t *group;
	StyleLink link;
	StyleLinkGroup slg;
	dict_iterator_t *iter;
	dict_entry_t *entry;

	link = NULL;
	logger_debug("style library begin\n");
	group = list_get(&library.groups, 0);
	iter = dict_get_iterator(group);
	while ((entry = dict_next(iter))) {
		dict_entry_t *entry_slg;
		dict_iterator_t *iter_slg;

		slg = dict_get_val(entry);
		iter_slg = dict_get_iterator(slg->links);
		while ((entry_slg = dict_next(iter_slg))) {
			link = dict_get_val(entry_slg);
			LCUI_PrintStyleLink(link, NULL);
		}
		dict_destroy_iterator(iter_slg);
	}
	dict_destroy_iterator(iter);
	logger_debug("style library end\n");
}

LCUI_CachedStyleSheet LCUI_GetCachedStyleSheet(LCUI_Selector s)
{
	list_t list;
	list_node_t *node;
	LCUI_StyleSheet ss;

	list_create(&list);
	ss = dict_fetch_value(library.cache, &s->hash);
	if (ss) {
		return ss;
	}
	ss = StyleSheet();
	LCUI_FindStyleSheet(s, &list);
	for (list_each(node, &list)) {
		StyleNode sn = node->data;
		StyleSheet_MergeList(ss, sn->list);
	}
	list_destroy(&list, NULL);
	dict_add(library.cache, &s->hash, ss);
	return ss;
}

void LCUI_GetStyleSheet(LCUI_Selector s, LCUI_StyleSheet out_ss)
{
	const LCUI_StyleSheetRec *ss;

	ss = LCUI_GetCachedStyleSheet(s);
	StyleSheet_Clear(out_ss);
	StyleSheet_Replace(out_ss, ss);
}

void LCUI_PrintStyleSheetsBySelector(LCUI_Selector s)
{
	list_t list;
	list_node_t *node;
	LCUI_StyleSheet ss;
	list_create(&list);
	ss = StyleSheet();
	LCUI_FindStyleSheet(s, &list);
	logger_debug("selector(%u) stylesheets begin\n", s->hash);
	for (list_each(node, &list)) {
		StyleNode sn = node->data;
		logger_debug("\n[%s]", sn->space ? sn->space : "<none>");
		logger_debug("[rank: %d]\n%s {\n", sn->rank, sn->selector);
		LCUI_PrintStyleList(sn->list);
		logger_debug("}\n");
		StyleSheet_MergeList(ss, sn->list);
	}
	list_destroy(&list, NULL);
	logger_debug("[selector(%u) final stylesheet] {\n", s->hash);
	LCUI_PrintStyleSheet(ss);
	logger_debug("}\n");
	StyleSheet_Delete(ss);
	logger_debug("selector(%u) stylesheets end\n", s->hash);
}

static void StyleSheetCacheDestructor(void *privdata, void *val)
{
	StyleSheet_Delete(val);
}

static void *DupStyleName(void *privdata, const void *val)
{
	return strdup2(val);
}

static void StyleNameDestructor(void *privdata, void *val)
{
	free(val);
}

static unsigned int IntKeyDict_HashFunction(const void *key)
{
	return (*(unsigned int *)key);
}

static int IntKeyDict_KeyCompare(void *privdata, const void *key1,
				 const void *key2)
{
	return *(unsigned int *)key1 == *(unsigned int *)key2;
}

static void IntKeyDict_KeyDestructor(void *privdata, void *key)
{
	free(key);
}

static void *IntKeyDict_KeyDup(void *privdata, const void *key)
{
	unsigned int *newkey = malloc(sizeof(unsigned int));
	*newkey = *(unsigned int *)key;
	return newkey;
}

static void InitStylesheetCache(void)
{
	dict_type_t *dt = &library.cache_dict;

	dt->val_dup = NULL;
	dt->key_dup = IntKeyDict_KeyDup;
	dt->key_compare = IntKeyDict_KeyCompare;
	dt->hash_function = IntKeyDict_HashFunction;
	dt->key_destructor = IntKeyDict_KeyDestructor;
	dt->val_destructor = StyleSheetCacheDestructor;
	dt->key_destructor = IntKeyDict_KeyDestructor;
	library.cache = dict_create(dt, NULL);
}

static void DestroyStylesheetCache(void)
{
	dict_destroy(library.cache);
	library.cache = NULL;
}

static void StyleLinkDestructor(void *privdata, void *data)
{
	DeleteStyleLink(data);
}

static void InitStyleLinkDict(void)
{
	dict_init_string_copy_key_type(&library.style_link_dict);
	library.style_link_dict.val_destructor = StyleLinkDestructor;
}

static void InitStyleNameLibrary(void)
{
	dict_type_t *dt = &library.names_dict;

	dt->val_dup = DupStyleName;
	dt->key_dup = IntKeyDict_KeyDup;
	dt->key_compare = IntKeyDict_KeyCompare;
	dt->val_destructor = StyleNameDestructor;
	dt->hash_function = IntKeyDict_HashFunction;
	dt->key_destructor = IntKeyDict_KeyDestructor;
	library.names = dict_create(dt, NULL);
}

static void DestroyStyleNameLibrary(void)
{
	dict_destroy(library.names);
	library.names = NULL;
}

static void InitStyleValueLibrary(void)
{
	dict_type_t *keys_dt = &library.value_keys_dict;
	dict_type_t *names_dt = &library.value_names_dict;

	memset(names_dt, 0, sizeof(dict_type_t));
	names_dt->key_compare = IntKeyDict_KeyCompare;
	names_dt->hash_function = IntKeyDict_HashFunction;
	dict_init_string_key_type(keys_dt);
	keys_dt->val_destructor = KeyNameGroupDestructor;
	/* value_keys 表用于存放 key 和 name 数据 */
	library.value_keys = dict_create(keys_dt, NULL);
	/* value_names 表仅引用 value_keys 里的数据  */
	library.value_names = dict_create(names_dt, NULL);
}

static void DestroyStyleValueLibrary(void)
{
	dict_destroy(library.value_names);
	dict_destroy(library.value_keys);
	library.value_keys = NULL;
	library.value_names = NULL;
}

void LCUI_InitCSSLibrary(void)
{
	KeyNameGroup skn, skn_end;

	library.strpool = strpool_create();
	InitStyleLinkDict();
	InitStyleGroupDict();
	InitStylesheetCache();
	InitStyleNameLibrary();
	InitStyleValueLibrary();
	list_create(&library.groups);
	skn_end = style_name_map + LEN(style_name_map);
	for (skn = style_name_map; skn < skn_end; ++skn) {
		LCUI_DirectAddStyleName(skn->key, skn->name);
	}
	skn_end = style_value_map + LEN(style_value_map);
	for (skn = style_value_map; skn < skn_end; ++skn) {
		LCUI_AddStyleValue(skn->key, skn->name);
	}
	library.count = STYLE_KEY_TOTAL;
	library.active = TRUE;
}

void LCUI_FreeCSSLibrary(void)
{
	library.active = FALSE;
	DestroyStylesheetCache();
	DestroyStyleNameLibrary();
	DestroyStyleValueLibrary();
	list_destroy(&library.groups, (FuncPtr)DeleteStyleGroup);
	strpool_destroy(library.strpool);
}
