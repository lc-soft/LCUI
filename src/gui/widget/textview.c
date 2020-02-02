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
#include "../widget_util.h"

#define GetData(W) Widget_GetData(W, self.prototype)
#define ComputeActual LCUIMetrics_ComputeActual

typedef struct LCUI_TextViewRec_ {
	wchar_t *content;
	LCUI_BOOL trimming;
	LCUI_Widget widget;
	LCUI_TextLayer layer;
	LCUI_CSSFontStyleRec style;
	LinkedListNode node;
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

static void TextView_OnRunTask(LCUI_Widget w, int task)
{
	if (task == LCUI_WTASK_RESIZE &&
	    Widget_GetWidthSizingRule(w) == LCUI_SIZING_RULE_FIT_CONTENT) {
		Widget_AddTask(w, LCUI_WTASK_REFLOW);
	}
}

static void TextView_UpdateStyle(LCUI_Widget w)
{
	LCUI_CSSFontStyleRec style;
	LCUI_TextStyleRec text_style;
	LCUI_TextView txt = GetData(w);

	CSSFontStyle_Init(&style);
	CSSFontStyle_Compute(&style, w->style);
	if (CSSFontStyle_IsEquals(&style, &txt->style)) {
		CSSFontStyle_Destroy(&style);
		return;
	}
	CSSFontStyle_GetTextStyle(&style, &text_style);
	TextLayer_SetTextAlign(txt->layer, style.text_align);
	TextLayer_SetLineHeight(txt->layer, style.line_height);
	TextLayer_SetAutoWrap(txt->layer, style.white_space != SV_NOWRAP);
	TextLayer_SetWordBreak(txt->layer, ComputeWordBreakMode(w->style));
	TextLayer_SetTextStyle(txt->layer, &text_style);
	if (style.content) {
		TextView_SetTextW(w, style.content);
	} else if (txt->style.content) {
		TextView_SetTextW(w, NULL);
	}
	CSSFontStyle_Destroy(&txt->style);
	TextStyle_Destroy(&text_style);
	txt->style = style;
	Widget_AddTask(w, LCUI_WTASK_USER);
}

static void TextView_OnInit(LCUI_Widget w)
{
	LCUI_TextView txt;

	txt = Widget_AddData(w, self.prototype, sizeof(LCUI_TextViewRec));
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
	CSSFontStyle_Init(&txt->style);
	txt->node.data = txt;
	txt->node.prev = txt->node.next = NULL;
	LinkedList_AppendNode(&self.list, &txt->node);
}

static void TextView_OnDestroy(LCUI_Widget w)
{
	LCUI_TextView txt = GetData(w);

	LinkedList_Unlink(&self.list, &txt->node);
	CSSFontStyle_Destroy(&txt->style);
	TextLayer_Destroy(txt->layer);
	free(txt->content);
}

static void TextView_AutoSize(LCUI_Widget w, float *content_width,
			      float *content_height)
{
	int max_width = 0, max_height = 0;
	int fixed_width = 0, fixed_height = 0;
	int text_width, text_height;
	float scale = LCUIMetrics_GetScale();

	LCUI_RectF rect;
	LCUI_TextView txt = GetData(w);

	LinkedList rects;
	LinkedListNode *node;

	if (w->parent) {
		if (Widget_GetWidthSizingRule(w->parent) !=
		    LCUI_SIZING_RULE_FIT_CONTENT) {
			max_width =
			    (int)(scale * (w->parent->box.content.width -
					   PaddingX(w) - BorderX(w)));
		}
		if (Widget_GetHeightSizingRule(w->parent) !=
		    LCUI_SIZING_RULE_FIT_CONTENT) {
			max_height =
			    (int)(scale * (w->parent->box.content.height -
					   PaddingY(w) - BorderY(w)));
		}
	}
	if (Widget_GetWidthSizingRule(w) != LCUI_SIZING_RULE_FIT_CONTENT) {
		fixed_width = (int)(scale * w->box.content.width);
		max_width = fixed_width;
	}
	if (Widget_GetHeightSizingRule(w) != LCUI_SIZING_RULE_FIT_CONTENT) {
		fixed_height = (int)(scale * w->box.content.height);
		max_height = fixed_height;
	}
	LinkedList_Init(&rects);
	TextLayer_SetFixedSize(txt->layer, fixed_width, fixed_height);
	TextLayer_SetMaxSize(txt->layer, max_width, max_height);
	TextLayer_Update(txt->layer, &rects);
	TextLayer_ClearInvalidRect(txt->layer);
	text_width = TextLayer_GetWidth(txt->layer);
	text_height = TextLayer_GetHeight(txt->layer);
	if (fixed_width == 0 && *content_width > text_width) {
		fixed_width = *content_width;
		if (fixed_height == 0 && *content_height > text_height) {
			fixed_height = *content_height;
		}
		TextLayer_SetFixedSize(txt->layer, fixed_width, fixed_height);
		TextLayer_Update(txt->layer, &rects);
		TextLayer_ClearInvalidRect(txt->layer);
	} else if (fixed_height == 0 && *content_height > text_height) {
		fixed_height = *content_height;
		TextLayer_SetFixedSize(txt->layer, fixed_width, fixed_height);
		TextLayer_Update(txt->layer, &rects);
		TextLayer_ClearInvalidRect(txt->layer);
	}
	*content_width = TextLayer_GetWidth(txt->layer) / scale;
	*content_height = TextLayer_GetHeight(txt->layer) / scale;
	for (LinkedList_Each(node, &rects)) {
		LCUIRect_ToRectF(node->data, &rect, 1.0f / scale);
		Widget_InvalidateArea(w, &rect, SV_CONTENT_BOX);
	}
	RectList_Clear(&rects);
}

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
	TextLayer_SetTextW(txt->layer, newtext, NULL);
	Widget_AddTask(w, LCUI_WTASK_REFLOW);
	free(newtext);
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

	TextLayer_SetMultiline(txt->layer, enable);
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
	self.prototype->destroy = TextView_OnDestroy;
	self.prototype->autosize = TextView_AutoSize;
	self.prototype->update = TextView_UpdateStyle;
	self.prototype->settext = TextView_OnParseText;
	self.prototype->setattr = TextView_OnParseAttr;
	self.prototype->runtask = TextView_OnRunTask;
	LCUI_AddCSSPropertyParser(&parser);
	LinkedList_Init(&self.list);
}

void LCUIWidget_FreeTextView(void)
{
	LinkedList_ClearData(&self.list, NULL);
}
