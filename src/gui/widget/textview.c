/*
 * textview.c -- TextView widget for display text.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/css_fontstyle.h>
#include <LCUI/gui/widget/textview.h>

#define GetData(W) Widget_GetData(W, self.prototype)
#define ComputeActual LCUIMetrics_ComputeActual

enum TaskType {
	TASK_SET_TEXT,
	TASK_SET_TEXT_ALIGN,
	TASK_SET_TEXT_STYLE,
	TASK_SET_WORD_BREAK,
	TASK_SET_MULITILINE,
	TASK_SET_AUTOWRAP,
	TASK_UPDATE_SIZE,
	TASK_UPDATE,
	TASK_TOTAL
};

typedef struct LCUI_TextViewRec_ {
	wchar_t *content;
	LCUI_BOOL trimming;
	LCUI_Widget widget;
	LCUI_TextLayer layer;
	LCUI_CSSFontStyleRec style;
	LinkedListNode node;
	struct {
		LCUI_BOOL is_valid;
		union {
			wchar_t *text;
			LCUI_BOOL enable;
			LCUI_WordBreakMode mode;
			LCUI_TextStyleRec style;
			int align;
		};
	} tasks[TASK_TOTAL];
} LCUI_TextViewRec, *LCUI_TextView;

static struct LCUI_TextViewModule {
	int key_word_break;
	LinkedList list;
	LCUI_WidgetPrototype prototype;
} self;

static LCUI_BOOL ParseBoolean(const char *str)
{
	if (strcmp(str, "on") == 0 && strcmp(str, "true") == 0 &&
	    strcmp(str, "yes") == 0 && strcmp(str, "1") == 0) {
		return TRUE;
	}
	return FALSE;
}

static int OnParseWordBreak(LCUI_CSSParserStyleContext ctx, const char *value)
{
	char *str = strdup2(value);
	LCUI_Style s = &ctx->sheet->sheet[self.key_word_break];
	if (s->is_valid && s->string) {
		free(s->string);
	}
	s->type = LCUI_STYPE_STRING;
	s->is_valid = TRUE;
	s->string = str;
	return 0;
}

static LCUI_WordBreakMode ComputeWordBreakMode(LCUI_StyleSheet sheet)
{
	LCUI_Style s = &sheet->sheet[self.key_word_break];
	if (s->is_valid && s->type == LCUI_STYPE_STRING && s->string) {
		if (strcmp(s->string, "break-all") == 0) {
			return LCUI_WORD_BREAK_BREAK_ALL;
		}
	}
	return LCUI_WORD_BREAK_NORMAL;
}

static void TextView_OnParseAttr(LCUI_Widget w, const char *name,
				 const char *value)
{
	LCUI_TextView txt = GetData(w);
	if (strcmp(name, "trimming") == 0) {
		if (ParseBoolean(value) != txt->trimming) {
			txt->trimming = !txt->trimming;
			TextView_SetTextW(w, txt->content);
		}
		return;
	}
	if (strcmp(name, "multiline") == 0) {
		LCUI_BOOL enable = ParseBoolean(value);
		if (enable != txt->layer->enable_mulitiline) {
			TextView_SetMulitiline(w, enable);
		}
	}
}

static void TextView_OnParseText(LCUI_Widget w, const char *text)
{
	TextView_SetText(w, text);
}

static void TextView_SetTaskForLineHeight(LCUI_Widget w, int height)
{
	LCUI_TextView txt = GetData(w);
	TextLayer_SetLineHeight(txt->layer, height);
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
}

static void TextView_SetTaskForTextStyle(LCUI_Widget w, LCUI_TextStyle style)
{
	LCUI_TextView txt = GetData(w);
	TextStyle_Copy(&txt->tasks[TASK_SET_TEXT_STYLE].style, style);
	txt->tasks[TASK_SET_TEXT_STYLE].is_valid = TRUE;
}

static void TextView_SetTaskForTextAlign(LCUI_Widget w, int align)
{
	LCUI_TextView txt = GetData(w);
	txt->tasks[TASK_SET_TEXT_ALIGN].align = align;
	txt->tasks[TASK_SET_TEXT_ALIGN].is_valid = TRUE;
}

static void TextView_SetTaskForAutoWrap(LCUI_Widget w, LCUI_BOOL enable)
{
	LCUI_TextView txt = GetData(w);
	txt->tasks[TASK_SET_AUTOWRAP].enable = enable;
	txt->tasks[TASK_SET_AUTOWRAP].is_valid = TRUE;
}

static void TextView_SetTaskForWordBreak(LCUI_Widget w, LCUI_WordBreakMode mode)
{
	LCUI_TextView txt = GetData(w);
	txt->tasks[TASK_SET_WORD_BREAK].mode = mode;
	txt->tasks[TASK_SET_WORD_BREAK].is_valid = TRUE;
}

static void TextView_UpdateStyle(LCUI_Widget w)
{
	LCUI_TextStyleRec ts;
	LCUI_TextView txt = GetData(w);
	LCUI_CSSFontStyle fs = &txt->style;

	CSSFontStyle_Compute(fs, w->style);
	CSSFontStyle_GetTextStyle(fs, &ts);
	/* 设置任务，统一处理属性变更 */
	TextView_SetTaskForTextAlign(w, fs->text_align);
	TextView_SetTaskForLineHeight(w, fs->line_height);
	TextView_SetTaskForAutoWrap(w, fs->white_space != SV_NOWRAP);
	TextView_SetTaskForWordBreak(w, ComputeWordBreakMode(w->style));
	TextView_SetTaskForTextStyle(w, &ts);
	if (fs->content) {
		if (!txt->content || wcscmp(fs->content, txt->content) != 0) {
			TextView_SetTextW(w, fs->content);
		}
	}
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask(w, LCUI_WTASK_USER);
	TextStyle_Destroy(&ts);
}

static void TextView_UpdateLayerSize(LCUI_Widget w)
{
	LCUI_TextView txt;
	int width = 0, height = 0;
	float max_width = 0, max_height = 0;

	txt = GetData(w);
	if (Widget_HasFitContentWidth(w) || Widget_HasParentDependentWidth(w)) {
		max_width = Widget_ComputeMaxContentWidth(w);
	} else {
		max_width = w->box.content.width;
	}
	if (!Widget_HasAutoStyle(w, key_height)) {
		max_height = w->box.content.height;
	}
	/* 将当前部件宽高作为文本层的固定宽高 */
	width = ComputeActual(w->box.content.width, LCUI_STYPE_PX);
	height = ComputeActual(w->box.content.height, LCUI_STYPE_PX);
	TextLayer_SetFixedSize(txt->layer, width, height);
	width = ComputeActual(max_width, LCUI_STYPE_PX);
	height = ComputeActual(max_height, LCUI_STYPE_PX);
	TextLayer_SetMaxSize(txt->layer, width, height);
}

static void TextView_OnResize(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	float scale;
	LCUI_RectF rect;
	LCUI_TextView txt;
	LinkedList rects;
	LinkedListNode *node;

	txt = GetData(w);
	LinkedList_Init(&rects);
	scale = LCUIMetrics_GetScale();
	TextView_UpdateLayerSize(w);
	TextLayer_Update(txt->layer, &rects);
	for (LinkedList_Each(node, &rects)) {
		LCUIRect_ToRectF(node->data, &rect, 1.0f / scale);
		Widget_InvalidateArea(w, &rect, SV_CONTENT_BOX);
	}
	RectList_Clear(&rects);
	TextLayer_ClearInvalidRect(txt->layer);
}

/** 初始化 TextView 部件数据 */
static void TextView_OnInit(LCUI_Widget w)
{
	int i;
	LCUI_TextView txt;
	txt = Widget_AddData(w, self.prototype, sizeof(LCUI_TextViewRec));
	for (i = 0; i < TASK_TOTAL; ++i) {
		txt->tasks[i].is_valid = FALSE;
	}
	txt->widget = w;
	txt->content = NULL;
	/* 默认清除首尾空白符 */
	txt->trimming = TRUE;
	/* 初始化文本图层 */
	txt->layer = TextLayer_New();
	/* 启用多行文本显示 */
	TextLayer_SetAutoWrap(txt->layer, TRUE);
	TextLayer_SetMultiline(txt->layer, TRUE);
	/* 启用样式标签的支持 */
	TextLayer_EnableStyleTag(txt->layer, TRUE);
	Widget_BindEvent(w, "resize", TextView_OnResize, NULL, NULL);
	CSSFontStyle_Init(&txt->style);
	txt->node.data = txt;
	txt->node.prev = txt->node.next = NULL;
	LinkedList_AppendNode(&self.list, &txt->node);
}

static void TextView_ClearTasks(LCUI_Widget w)
{
	int i = TASK_SET_TEXT;
	LCUI_TextView txt = GetData(w);
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		free(txt->tasks[i].text);
		txt->tasks[i].text = NULL;
	}
}

/** 释放 TextView 部件占用的资源 */
static void TextView_OnDestroy(LCUI_Widget w)
{
	LCUI_TextView txt = GetData(w);

	LinkedList_Unlink(&self.list, &txt->node);
	CSSFontStyle_Destroy(&txt->style);
	TextLayer_Destroy(txt->layer);
	TextView_ClearTasks(w);
	free(txt->content);
}

static void TextView_AutoSize(LCUI_Widget w, float *width, float *height)
{
	float max_width;
	int fixed_w, fixed_h;
	LCUI_TextView txt = GetData(w);
	float scale = LCUIMetrics_GetScale();

	TextView_UpdateLayerSize(w);
	fixed_w = txt->layer->fixed_width;
	fixed_h = txt->layer->fixed_height;
	if (Widget_HasFitContentWidth(w) || !Widget_HasStaticWidthParent(w)) {
		/* 解除固定宽高设置，以计算最大宽高 */
		TextLayer_SetFixedSize(txt->layer, (int)(*width * scale), 0);
		if (Widget_HasParentDependentWidth(w)) {
			max_width = scale * Widget_ComputeMaxContentWidth(w);
			TextLayer_SetMaxSize(txt->layer, (int)max_width, 0);
		}
		TextLayer_Update(txt->layer, NULL);
		if (*width <= 0) {
			*width = TextLayer_GetWidth(txt->layer) / scale;
		}
		if (*height <= 0) {
			*height = TextLayer_GetHeight(txt->layer) / scale;
		}
		/* 还原固定宽高设置 */
		TextLayer_SetFixedSize(txt->layer, fixed_w, fixed_h);
		TextLayer_Update(txt->layer, NULL);
		return;
	}
	if (*width <= 0) {
		*width = TextLayer_GetWidth(txt->layer) / scale;
		if (*width <= 0) {
			return;
		}
	}
	TextLayer_SetFixedSize(txt->layer, (int)(*width * scale), 0);
	TextLayer_Update(txt->layer, NULL);
	*height = TextLayer_GetHeight(txt->layer) / scale;
}

static void TextView_OnRefresh(LCUI_Widget w)
{
	LCUI_TextView txt = GetData(w);
	txt->tasks[TASK_UPDATE_SIZE].is_valid = TRUE;
	Widget_AddTask(w, LCUI_WTASK_USER);
}

/** 私有的任务处理接口 */
static void TextView_OnTask(LCUI_Widget w)
{
	int i;
	float scale;
	LCUI_RectF rect;
	LinkedList rects;
	LinkedListNode *node;
	LCUI_TextView txt = GetData(w);

	LinkedList_Init(&rects);
	i = TASK_SET_TEXT;
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetTextW(txt->layer, txt->tasks[i].text, NULL);
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
		txt->tasks[TASK_UPDATE_SIZE].is_valid = TRUE;
		free(txt->tasks[i].text);
		txt->tasks[i].text = NULL;
	}
	i = TASK_SET_AUTOWRAP;
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetAutoWrap(txt->layer, txt->tasks[i].enable);
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_SET_WORD_BREAK;
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetWordBreak(txt->layer, txt->tasks[i].mode);
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_SET_TEXT_STYLE;
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetTextStyle(txt->layer, &txt->tasks[i].style);
		TextStyle_Destroy(&txt->tasks[i].style);
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_SET_TEXT_ALIGN;
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetTextAlign(txt->layer, txt->tasks[i].align);
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_SET_MULITILINE;
	if (txt->tasks[i].is_valid) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetMultiline(txt->layer, txt->tasks[i].enable);
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_UPDATE_SIZE;
	if (txt->tasks[i].is_valid) {
		TextView_UpdateLayerSize(w);
		if (Widget_HasFitContentWidth(w) ||
		    Widget_HasAutoStyle(w, key_width) ||
		    Widget_HasAutoStyle(w, key_height)) {
			Widget_AddTask(w, LCUI_WTASK_RESIZE);
		}
		txt->tasks[i].is_valid = FALSE;
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_UPDATE;
	if (!txt->tasks[i].is_valid) {
		return;
	}
	txt->tasks[i].is_valid = FALSE;
	LinkedList_Init(&rects);
	scale = LCUIMetrics_GetScale();
	TextLayer_Update(txt->layer, &rects);
	for (LinkedList_Each(node, &rects)) {
		LCUIRect_ToRectF(node->data, &rect, 1.0f / scale);
		Widget_InvalidateArea(w, &rect, SV_CONTENT_BOX);
	}
	RectList_Clear(&rects);
	TextLayer_ClearInvalidRect(txt->layer);
}

/** 绘制 TextView 部件 */
static void TextView_OnPaint(LCUI_Widget w, LCUI_PaintContext paint,
			     LCUI_WidgetActualStyle style)
{
	LCUI_Pos pos;
	LCUI_Graph canvas;
	LCUI_Rect content_rect, rect;
	LCUI_TextView txt = GetData(w);
	content_rect.width = style->content_box.width;
	content_rect.height = style->content_box.height;
	content_rect.x = style->content_box.x - style->canvas_box.x;
	content_rect.y = style->content_box.y - style->canvas_box.y;
	if (!LCUIRect_GetOverlayRect(&content_rect, &paint->rect, &rect)) {
		return;
	}
	pos.x = content_rect.x - rect.x;
	pos.y = content_rect.y - rect.y;
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	Graph_Quote(&canvas, &paint->canvas, &rect);
	rect = paint->rect;
	rect.x -= content_rect.x;
	rect.y -= content_rect.y;
	TextLayer_RenderTo(txt->layer, rect, pos, &canvas);
}

int TextView_SetTextW(LCUI_Widget w, const wchar_t *text)
{
	LCUI_TextView txt = GetData(w);
	wchar_t *newtext = malloc(wcssize(text));
	if (!newtext) {
		return -ENOMEM;
	}
	if (txt->content) {
		free(txt->content);
	}
	txt->content = malloc(wcssize(text));
	if (!txt->content) {
		free(newtext);
		return -ENOMEM;
	}
	do {
		if (!text) {
			newtext[0] = 0;
			txt->content[0] = 0;
			break;
		}
		wcscpy(txt->content, text);
		if (!txt->trimming) {
			wcscpy(newtext, text);
			break;
		}
		wcstrim(newtext, text, NULL);
	} while (0);
	if (txt->tasks[TASK_SET_TEXT].is_valid &&
	    txt->tasks[TASK_SET_TEXT].text) {
		free(txt->tasks[TASK_SET_TEXT].text);
	}
	txt->tasks[TASK_SET_TEXT].is_valid = TRUE;
	txt->tasks[TASK_SET_TEXT].text = newtext;
	Widget_AddTask(w, LCUI_WTASK_USER);
	return 0;
}

int TextView_SetText(LCUI_Widget w, const char *utf8_text)
{
	int ret;
	wchar_t *wstr;
	size_t len = strlen(utf8_text) + 1;
	wstr = malloc(sizeof(wchar_t) * len);
	LCUI_DecodeString(wstr, utf8_text, len, ENCODING_UTF8);
	ret = TextView_SetTextW(w, wstr);
	if (wstr) {
		free(wstr);
	}
	return ret;
}

void TextView_SetLineHeight(LCUI_Widget w, int height)
{
	Widget_SetFontStyle(w, key_line_height, (float)height, px);
}

void TextView_SetTextAlign(LCUI_Widget w, int align)
{
	Widget_SetFontStyle(w, key_text_align, align, style);
}

void TextView_SetColor(LCUI_Widget w, LCUI_Color color)
{
	Widget_SetFontStyle(w, key_color, color, color);
}

void TextView_SetAutoWrap(LCUI_Widget w, LCUI_BOOL enable)
{
	if (enable) {
		Widget_SetFontStyle(w, key_white_space, SV_AUTO, style);
	} else {
		Widget_SetFontStyle(w, key_white_space, SV_NOWRAP, style);
	}
}

void TextView_SetMulitiline(LCUI_Widget w, LCUI_BOOL enable)
{
	LCUI_TextView txt = GetData(w);
	txt->tasks[TASK_SET_MULITILINE].enable = enable;
	txt->tasks[TASK_SET_MULITILINE].is_valid = TRUE;
	Widget_AddTask(w, LCUI_WTASK_USER);
}

size_t LCUIWidget_RefreshTextView(void)
{
	size_t count = 0;
	LCUI_TextView txt;
	LinkedListNode *node;

	for (LinkedList_Each(node, &self.list)) {
		txt = node->data;
		if (txt->widget->state != LCUI_WSTATE_DELETED) {
			Widget_UpdateStyle(txt->widget, TRUE);
		}
		count += 1;
	}
	return count;
}

void LCUIWidget_AddTextView(void)
{
	LCUI_CSSPropertyParserRec parser = { 0, "word-break",
					     OnParseWordBreak };
	self.key_word_break = LCUI_AddCSSPropertyName("word-break");
	self.prototype = LCUIWidget_NewPrototype("textview", NULL);
	self.prototype->init = TextView_OnInit;
	self.prototype->paint = TextView_OnPaint;
	self.prototype->refresh = TextView_OnRefresh;
	self.prototype->destroy = TextView_OnDestroy;
	self.prototype->autosize = TextView_AutoSize;
	self.prototype->update = TextView_UpdateStyle;
	self.prototype->settext = TextView_OnParseText;
	self.prototype->setattr = TextView_OnParseAttr;
	self.prototype->runtask = TextView_OnTask;
	LCUI_AddCSSPropertyParser(&parser);
	LinkedList_Init(&self.list);
}

void LCUIWidget_FreeTextView(void)
{
	LinkedList_ClearData(&self.list, NULL);
}
