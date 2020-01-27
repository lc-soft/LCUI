/*
 * widget_style.c -- widget style library module for LCUI.
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
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/css_fontstyle.h>
#include "widget_util.h"

#define ARRAY_LEN(ARR) sizeof(ARR) / sizeof(ARR[0])

typedef struct LCUI_TaskCacheStatus {
	int start, end;
	LCUI_WidgetTaskType task;
	LCUI_BOOL is_valid;
} LCUI_TaskStatus;

/* clang-format off */

/** 部件的缺省样式 */
const char *global_css = CodeToString(

* {
	width: auto;
	height:	auto;
	background-color: transparent;
	border: 0px solid transparent;
	display: block;
	position: static;
	padding: 0;
	margin: 0;
}

root {
	box-sizing: border-box;
}

);

/* clang-format on */

INLINE int ComputeStyleOption(LCUI_Widget w, int key, int default_value)
{
	if (!w->style->sheet[key].is_valid ||
	    w->style->sheet[key].type != LCUI_STYPE_STYLE) {
		return default_value;
	}
	return w->style->sheet[key].style;
}

void Widget_ComputePaddingStyle(LCUI_Widget w)
{
	if (!Widget_HasAutoStyle(w, key_padding_top)) {
		w->padding.top = Widget_ComputeYMetric(w, key_padding_top);
	}
	if (!Widget_HasAutoStyle(w, key_padding_right)) {
		w->padding.right = Widget_ComputeXMetric(w, key_padding_right);
	}
	if (!Widget_HasAutoStyle(w, key_padding_bottom)) {
		w->padding.bottom =
		    Widget_ComputeYMetric(w, key_padding_bottom);
	}
	if (!Widget_HasAutoStyle(w, key_padding_left)) {
		w->padding.left = Widget_ComputeXMetric(w, key_padding_left);
	}
}

void Widget_ComputeMarginStyle(LCUI_Widget w)
{
	if (!Widget_HasAutoStyle(w, key_margin_top)) {
		w->margin.top = Widget_ComputeYMetric(w, key_margin_top);
	}
	if (!Widget_HasAutoStyle(w, key_margin_right)) {
		w->margin.right = Widget_ComputeXMetric(w, key_margin_right);
	}
	if (!Widget_HasAutoStyle(w, key_margin_bottom)) {
		w->margin.bottom = Widget_ComputeYMetric(w, key_margin_bottom);
	}
	if (!Widget_HasAutoStyle(w, key_margin_left)) {
		w->margin.left = Widget_ComputeXMetric(w, key_margin_left);
	}
}

void Widget_ComputeProperties(LCUI_Widget w)
{
	LCUI_Style s;
	LCUI_WidgetStyle *style = &w->computed_style;

	s = &w->style->sheet[key_focusable];
	style->pointer_events =
	    ComputeStyleOption(w, key_pointer_events, SV_INHERIT);
	if (s->is_valid && s->type == LCUI_STYPE_BOOL && s->val_bool == 0) {
		style->focusable = FALSE;
	} else {
		style->focusable = TRUE;
	}
}

void Widget_ComputeWidthStyle(LCUI_Widget w)
{
	LCUI_WidgetStyle *style;
	const float border_spacing_x = w->padding.left + w->padding.right +
				       w->computed_style.border.left.width +
				       w->computed_style.border.right.width;

	style = &w->computed_style;
	style->max_width = -1;
	style->min_width = -1;
	if (Widget_CheckStyleValid(w, key_max_width)) {
		style->max_width = Widget_ComputeXMetric(w, key_max_width);
	}
	if (Widget_CheckStyleValid(w, key_min_width)) {
		style->min_width = Widget_ComputeXMetric(w, key_min_width);
	}
	if (w->computed_style.box_sizing != SV_BORDER_BOX) {
		if (style->max_width != -1) {
			style->max_width += border_spacing_x;
		}
		if (style->min_width != -1) {
			style->min_width += border_spacing_x;
		}
	}
	switch (Widget_GetWidthSizingRule(w)) {
	case LCUI_SIZING_RULE_FIXED:
		w->width = Widget_ComputeXMetric(w, key_width);
		if (w->computed_style.box_sizing == SV_CONTENT_BOX) {
			w->width += border_spacing_x;
		}
		break;
	case LCUI_SIZING_RULE_FILL:
		w->width = w->parent->box.content.width;
		w->width -= w->margin.left + w->margin.right;
		break;
	default:
		break;
	}
	w->width = Widget_GetLimitedWidth(w, w->width);
}

void Widget_ComputeHeightStyle(LCUI_Widget w)
{
	LCUI_WidgetStyle *style;
	const float border_spacing_y = w->padding.top + w->padding.bottom +
				       w->computed_style.border.top.width +
				       w->computed_style.border.bottom.width;

	style = &w->computed_style;
	style->max_height = -1;
	style->min_height = -1;
	if (Widget_CheckStyleValid(w, key_max_height)) {
		style->max_height = Widget_ComputeYMetric(w, key_max_height);
	}
	if (Widget_CheckStyleValid(w, key_min_height)) {
		style->min_height = Widget_ComputeYMetric(w, key_min_height);
	}
	if (w->computed_style.box_sizing != SV_BORDER_BOX) {
		if (style->max_height != -1) {
			style->max_height += border_spacing_y;
		}
		if (style->min_height != -1) {
			style->min_height += border_spacing_y;
		}
	}
	switch (Widget_GetHeightSizingRule(w)) {
	case LCUI_SIZING_RULE_FIXED:
		w->height = Widget_ComputeYMetric(w, key_height);
		if (w->computed_style.box_sizing == SV_CONTENT_BOX) {
			w->height += border_spacing_y;
		}
		break;
	default:
		break;
	}
	w->height = Widget_GetLimitedHeight(w, w->height);
}

void Widget_ComputeSizeStyle(LCUI_Widget w)
{
	w->computed_style.box_sizing =
	    ComputeStyleOption(w, key_box_sizing, SV_CONTENT_BOX);
	Widget_ComputeWidthStyle(w);
	Widget_ComputeHeightStyle(w);
}

void Widget_ComputeFlexBasisStyle(LCUI_Widget w)
{
	LCUI_FlexBoxLayoutStyle *flex = &w->computed_style.flex;

	if (w->parent &&
	    w->parent->computed_style.flex.direction == SV_COLUMN) {
		if (Widget_HasAutoStyle(w, key_flex_basis)) {
			if (!Widget_HasStaticHeight(w)) {
				flex->basis =
				    ToBorderBoxWidth(w, w->min_content_height);
				return;
			}
			flex->basis = Widget_ComputeYMetric(w, key_height);
			return;
		}
		flex->basis = Widget_ComputeYMetric(w, key_flex_basis);
		return;
	}
	if (Widget_HasAutoStyle(w, key_flex_basis)) {
		if (Widget_HasStaticWidth(w)) {
			flex->basis = Widget_ComputeXMetric(w, key_width);
			return;
		}
		flex->basis = ToBorderBoxWidth(w, w->min_content_width);
		return;
	}
	flex->basis = Widget_ComputeXMetric(w, key_flex_basis);
}

void Widget_ComputeVisibilityStyle(LCUI_Widget w)
{
	LCUI_Style s = &w->style->sheet[key_visibility];

	if (w->computed_style.display == SV_NONE) {
		w->computed_style.visible = FALSE;
	} else if (s->is_valid && s->type == LCUI_STYPE_STRING &&
		   strcmp(s->val_string, "hidden") == 0) {
		w->computed_style.visible = FALSE;
	} else {
		w->computed_style.visible = TRUE;
	}
}

void Widget_ComputeDisplayStyle(LCUI_Widget w)
{
	LCUI_Style s = &w->style->sheet[key_display];
	LCUI_WidgetStyle *style = &w->computed_style;

	if (s->is_valid && s->type == LCUI_STYPE_STYLE) {
		style->display = s->style;
		if (style->display == SV_NONE) {
			w->computed_style.visible = FALSE;
		}
	} else {
		style->display = SV_BLOCK;
	}
	Widget_ComputeVisibilityStyle(w);
}

void Widget_ComputeOpacityStyle(LCUI_Widget w)
{
	float opacity = 1.0;
	LCUI_Style s = &w->style->sheet[key_opacity];

	if (s->is_valid) {
		switch (s->type) {
		case LCUI_STYPE_INT:
			opacity = 1.0f * s->val_int;
			break;
		case LCUI_STYPE_SCALE:
			opacity = s->val_scale;
			break;
		default:
			opacity = 1.0f;
			break;
		}
		if (opacity > 1.0) {
			opacity = 1.0;
		} else if (opacity < 0.0) {
			opacity = 0.0;
		}
	}
	w->computed_style.opacity = opacity;
}

void Widget_ComputeZIndexStyle(LCUI_Widget w)
{
	LCUI_Style s = &w->style->sheet[key_z_index];

	if (s->is_valid && s->type == LCUI_STYPE_INT) {
		w->computed_style.z_index = s->val_int;
	} else {
		w->computed_style.z_index = 0;
	}
}

void Widget_ComputePositionStyle(LCUI_Widget w)
{
	int position = ComputeStyleOption(w, key_position, SV_STATIC);
	int valign = ComputeStyleOption(w, key_vertical_align, SV_TOP);

	w->computed_style.vertical_align = valign;
	w->computed_style.left = Widget_ComputeXMetric(w, key_left);
	w->computed_style.right = Widget_ComputeXMetric(w, key_right);
	w->computed_style.top = Widget_ComputeYMetric(w, key_top);
	w->computed_style.bottom = Widget_ComputeYMetric(w, key_bottom);
	w->computed_style.position = position;
	Widget_ComputeZIndexStyle(w);
}

void Widget_ComputeFlexBoxStyle(LCUI_Widget w)
{
	LCUI_Style s = w->style->sheet;
	LCUI_FlexBoxLayoutStyle *flex = &w->computed_style.flex;

	if (!Widget_IsFlexLayoutStyleWorks(w)) {
		return;
	}

	/* Reset to default value */

	flex->grow = 0;
	flex->shrink = 1;
	flex->wrap = SV_NOWRAP;
	flex->direction = SV_ROW;
	flex->justify_content = SV_NORMAL;
	flex->align_content = SV_NORMAL;
	flex->align_items = SV_NORMAL;

	/* Compute style */

	if (s[key_flex_grow].is_valid &&
	    s[key_flex_grow].type == LCUI_STYPE_INT) {
		flex->grow = s[key_flex_grow].val_int;
	}
	if (s[key_flex_shrink].is_valid &&
	    s[key_flex_shrink].type == LCUI_STYPE_INT) {
		flex->shrink = s[key_flex_shrink].val_int;
	}
	if (s[key_flex_wrap].is_valid &&
	    s[key_flex_wrap].type == LCUI_STYPE_STYLE) {
		flex->wrap = s[key_flex_wrap].val_style;
	}
	if (s[key_flex_direction].is_valid &&
	    s[key_flex_direction].type == LCUI_STYPE_STYLE) {
		flex->direction = s[key_flex_direction].val_style;
	}
	if (s[key_justify_content].is_valid &&
	    s[key_justify_content].type == LCUI_STYPE_STYLE) {
		flex->justify_content = s[key_justify_content].val_style;
	}
	if (s[key_align_content].is_valid &&
	    s[key_align_content].type == LCUI_STYPE_STYLE) {
		flex->align_content = s[key_align_content].val_style;
	}
	if (s[key_align_items].is_valid &&
	    s[key_align_items].type == LCUI_STYPE_STYLE) {
		flex->align_items = s[key_align_items].val_style;
	}
	Widget_ComputeFlexBasisStyle(w);
}

LCUI_SelectorNode Widget_GetSelectorNode(LCUI_Widget w)
{
	int i;
	ASSIGN(sn, LCUI_SelectorNode);
	ZEROSET(sn, LCUI_SelectorNode);

	if (w->id) {
		sn->id = strdup2(w->id);
	}
	if (w->type) {
		sn->type = strdup2(w->type);
	}
	for (i = 0; w->classes && w->classes[i]; ++i) {
		sortedstrlist_add(&sn->classes, w->classes[i]);
	}
	for (i = 0; w->status && w->status[i]; ++i) {
		sortedstrlist_add(&sn->status, w->status[i]);
	}
	SelectorNode_Update(sn);
	return sn;
}

LCUI_Selector Widget_GetSelector(LCUI_Widget w)
{
	int i = 0;
	LinkedList list;
	LCUI_Selector s;
	LCUI_Widget parent;
	LinkedListNode *node;

	s = Selector(NULL);
	LinkedList_Init(&list);
	for (parent = w; parent; parent = parent->parent) {
		if (parent->id || parent->type || parent->classes ||
		    parent->status) {
			LinkedList_Append(&list, parent);
		}
	}
	if (list.length >= MAX_SELECTOR_DEPTH) {
		LinkedList_Clear(&list, NULL);
		Selector_Delete(s);
		return NULL;
	}
	for (LinkedList_EachReverse(node, &list)) {
		parent = node->data;
		s->nodes[i] = Widget_GetSelectorNode(parent);
		s->rank += s->nodes[i]->rank;
		i += 1;
	}
	LinkedList_Clear(&list, NULL);
	s->nodes[i] = NULL;
	s->length = i;
	Selector_Update(s);
	return s;
}

size_t Widget_GetChildrenStyleChanges(LCUI_Widget w, int type, const char *name)
{
	LCUI_Selector s;
	LinkedList snames;
	LinkedListNode *node;

	size_t i, n, len;
	size_t count = 0;
	char ch, *str, **names = NULL;

	switch (type) {
	case 0:
		ch = '.';
		break;
	case 1:
		ch = ':';
		break;
	default:
		return 0;
	}
	LinkedList_Init(&snames);
	s = Widget_GetSelector(w);
	n = strsplit(name, " ", &names);
	/* 为分割出来的字符串加上前缀 */
	for (i = 0; i < n; ++i) {
		len = strlen(names[i]) + 2;
		str = malloc(len * sizeof(char));
		strncpy(str + 1, names[i], len - 1);
		str[0] = ch;
		free(names[i]);
		names[i] = str;
	}
	SelectorNode_GetNames(s->nodes[s->length - 1], &snames);
	for (LinkedList_Each(node, &snames)) {
		char *sname = node->data;
		/* 过滤掉不包含 name 中存在的名称 */
		for (i = 0; i < n; ++i) {
			char *p = strstr(sname, names[i]);
			if (p) {
				p += strlen(names[i]);
				switch (*p) {
				case 0:
				case ':':
				case '.':
					break;
				default:
					continue;
				}
				break;
			}
		}
		if (i < n) {
			count +=
			    LCUI_FindStyleSheetFromGroup(1, sname, s, NULL);
		}
	}
	Selector_Delete(s);
	LinkedList_Clear(&snames, free);
	for (i = 0; names[i]; ++i) {
		free(names[i]);
	}
	free(names);
	return count;
}

void Widget_PrintStyleSheets(LCUI_Widget w)
{
	LCUI_Selector s = Widget_GetSelector(w);
	LCUI_PrintStyleSheetsBySelector(s);
	Selector_Delete(s);
}

void Widget_UpdateStyle(LCUI_Widget w, LCUI_BOOL is_refresh_all)
{
	if (is_refresh_all) {
		Widget_AddTask(w, LCUI_WTASK_REFRESH_STYLE);
	} else {
		Widget_AddTask(w, LCUI_WTASK_UPDATE_STYLE);
	}
}

void Widget_UpdateChildrenStyle(LCUI_Widget w, LCUI_BOOL is_refresh_all)
{
	LinkedListNode *node;
	w->task.for_children = TRUE;
	for (LinkedList_Each(node, &w->children)) {
		Widget_UpdateStyle(node->data, is_refresh_all);
		Widget_UpdateChildrenStyle(node->data, is_refresh_all);
	}
}

static void OnSetStyle(int key, LCUI_Style style, void *arg)
{
	LCUI_Widget w = arg;
	LCUI_Style s = Widget_GetStyle(w, key);

	if (style->is_valid) {
		DestroyStyle(s);
		*s = *style;
		Widget_AddTaskByStyle(w, key);
	} else {
		Widget_UnsetStyle(w, key);
	}
}

void Widget_SetStyleString(LCUI_Widget w, const char *name, const char *value)
{
	LCUI_CSSParserStyleContextRec ctx = { 0 };

	ctx.style_handler = OnSetStyle;
	ctx.style_handler_arg = w;
	ctx.parser = LCUI_GetCSSPropertyParser(name);
	ctx.parser->parse(&ctx, value);
}

void Widget_AddTaskByStyle(LCUI_Widget w, int key)
{
	size_t i;
	LCUI_TaskStatus task_status[] = {
		{ key_visibility, key_visibility, LCUI_WTASK_VISIBLE, TRUE },
		{ key_display, key_display, LCUI_WTASK_DISPLAY, TRUE },
		{ key_flex_style_start, key_flex_style_end, LCUI_WTASK_FLEX,
		  TRUE },
		{ key_opacity, key_opacity, LCUI_WTASK_OPACITY, TRUE },
		{ key_z_index, key_z_index, LCUI_WTASK_ZINDEX, TRUE },
		{ key_width, key_height, LCUI_WTASK_RESIZE, TRUE },
		{ key_min_width, key_max_height, LCUI_WTASK_RESIZE, TRUE },
		{ key_padding_start, key_padding_end, LCUI_WTASK_RESIZE, TRUE },
		{ key_box_sizing, key_box_sizing, LCUI_WTASK_RESIZE, TRUE },
		{ key_margin_start, key_margin_end, LCUI_WTASK_MARGIN, TRUE },
		{ key_position_start, key_position_end, LCUI_WTASK_POSITION,
		  TRUE },
		{ key_vertical_align, key_vertical_align, LCUI_WTASK_POSITION,
		  TRUE },
		{ key_border_start, key_border_end, LCUI_WTASK_BORDER, TRUE },
		{ key_background_start, key_background_end,
		  LCUI_WTASK_BACKGROUND, TRUE },
		{ key_box_shadow_start, key_box_shadow_end, LCUI_WTASK_SHADOW,
		  TRUE },
		{ key_pointer_events, key_focusable, LCUI_WTASK_PROPS, TRUE }
	};

	for (i = 0; i < ARRAY_LEN(task_status); ++i) {
		if (key >= task_status[i].start && key <= task_status[i].end) {
			if (!task_status[i].is_valid) {
				break;
			}
			task_status[i].is_valid = FALSE;
			Widget_AddTask(w, task_status[i].task);
		}
	}
}

void Widget_ExecUpdateStyle(LCUI_Widget w, LCUI_BOOL is_update_all)
{
	StyleSheet_Clear(w->style);
	if (is_update_all) {
		/* 刷新该部件的相关数据 */
		if (w->proto && w->proto->refresh) {
			w->proto->refresh(w);
		}
	}
	if (w->custom_style) {
		StyleSheet_MergeList(w->style, w->custom_style);
	}
	StyleSheet_Merge(w->style, w->inherited_style);
	if (w->proto && w->proto->update &&
	    w->style->length > STYLE_KEY_TOTAL) {
		/* 扩展部分的样式交给该部件自己处理 */
		w->proto->update(w);
	}
}

void Widget_DestroyStyleSheets(LCUI_Widget w)
{
	w->inherited_style = NULL;
	if (w->custom_style) {
		StyleList_Delete(w->custom_style);
	}
	StyleSheet_Delete(w->style);
}

void LCUIWidget_InitStyle(void)
{
	LCUI_InitCSSLibrary();
	LCUI_InitCSSParser();
	LCUI_InitCSSFontStyle();
	LCUI_LoadCSSString(global_css, __FILE__);
}

void LCUIWidget_FreeStyle(void)
{
	LCUI_FreeCSSFontStyle();
	LCUI_FreeCSSLibrary();
	LCUI_FreeCSSParser();
}
