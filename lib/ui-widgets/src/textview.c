﻿/*
 * textview.c -- TextView widget for display text.
 *
 * Copyright (c) 2018-2020, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <LCUI/pandagl.h>
#include <LCUI/css.h>
#include <LCUI/text/textlayer.h>
#include "./internal.h"
#include "../include/textview.h"

typedef struct ui_textview_task_t {
	wchar_t *content;
	LCUI_BOOL update_content;
} ui_textview_task_t;

typedef struct ui_textview_t_ {
	float available_width;
	wchar_t *content;
	LCUI_BOOL trimming;
	ui_widget_t *widget;
	LCUI_TextLayer layer;
	ui_font_style_t style;
	ui_textview_task_t task;
	list_node_t node;
} ui_textview_t;

static struct ui_textview_module_t {
	int css_key_word_break;
	list_t list;
	ui_widget_prototype_t *prototype;
} ui_textview;

static LCUI_BOOL parse_boolean(const char *str)
{
	if (strcmp(str, "on") == 0 && strcmp(str, "true") == 0 &&
	    strcmp(str, "yes") == 0 && strcmp(str, "1") == 0) {
		return TRUE;
	}
	return FALSE;
}

static int css_parse_word_break_property(css_style_parser_t *ctx, const char *value)
{
	char *str = strdup2(value);
	css_unit_value_t *s = &ctx->style->sheet[ui_textview.css_key_word_break];

	if (s->is_valid && s->string) {
		free(s->string);
	}
	s->unit = CSS_UNIT_STRING;
	s->is_valid = TRUE;
	s->string = str;
	return 0;
}

static LCUI_WordBreakMode compute_word_break_mode(css_style_decl_t *style)
{
	css_unit_value_t *s = &style->sheet[ui_textview.css_key_word_break];
	if (s->is_valid && s->unit == CSS_UNIT_STRING && s->string) {
		if (strcmp(s->string, "break-all") == 0) {
			return LCUI_WORD_BREAK_BREAK_ALL;
		}
	}
	return LCUI_WORD_BREAK_NORMAL;
}

static void ui_textview_on_parse_attr(ui_widget_t *w, const char *name,
				 const char *value)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);

	if (strcmp(name, "trimming") == 0) {
		if (parse_boolean(value) != txt->trimming) {
			txt->trimming = !txt->trimming;
			ui_textview_set_text_w(w, txt->content);
		}
		return;
	}
	if (strcmp(name, "multiline") == 0) {
		LCUI_BOOL enable = parse_boolean(value);
		if (enable != txt->layer->enable_mulitiline) {
			ui_textview_set_multiline(w, enable);
		}
	}
}

static void ui_textview_on_parse_text(ui_widget_t *w, const char *text)
{
	ui_textview_set_text(w, text);
}

static void ui_textview_on_update(ui_widget_t *w)
{
	float scale = ui_get_scale();

	ui_rect_t rect;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	list_t rects;
	list_node_t *node;

	list_create(&rects);
	TextLayer_Update(txt->layer, &rects);
	TextLayer_ClearInvalidRect(txt->layer);
	for (list_each(node, &rects)) {
		ui_convert_rect(node->data, &rect, 1.0f / scale);
		ui_widget_mark_dirty_rect(w, &rect, CSS_KEYWORD_CONTENT_BOX);
	}
	pd_rects_clear(&rects);
	ui_widget_add_task(w, UI_TASK_REFLOW);
}

static void ui_textview_on_update_style(ui_widget_t *w)
{
	ui_font_style_t style;
	LCUI_TextStyleRec text_style;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	ui_font_style_init(&style);
	ui_font_style_compute(&style, w->style);
	if (ui_font_style_is_equal(&style, &txt->style)) {
		ui_font_style_destroy(&style);
		return;
	}
	convert_font_style_to_text_style(&style, &text_style);
	switch (style.text_align) {
	case CSS_KEYWORD_CENTER:
		TextLayer_SetTextAlign(txt->layer, LCUI_TEXT_CENTER);
		break;
	case CSS_KEYWORD_RIGHT:
		TextLayer_SetTextAlign(txt->layer, LCUI_TEXT_RIGHT);
		break;
	case CSS_KEYWORD_LEFT:
	default:
		TextLayer_SetTextAlign(txt->layer, LCUI_TEXT_LEFT);
		break;
	}
	TextLayer_SetLineHeight(txt->layer, style.line_height);
	TextLayer_SetAutoWrap(txt->layer,
			      style.white_space != CSS_KEYWORD_NOWRAP);
	TextLayer_SetWordBreak(txt->layer, compute_word_break_mode(w->style));
	TextLayer_SetTextStyle(txt->layer, &text_style);
	if (style.content) {
		ui_textview_set_text_w(w, style.content);
	} else if (txt->style.content) {
		ui_textview_set_text_w(w, NULL);
	}
	ui_font_style_destroy(&txt->style);
	TextStyle_Destroy(&text_style);
	txt->style = style;
	ui_textview_on_update(w);
}

static void ui_textview_on_init(ui_widget_t *w)
{
	ui_textview_t *txt;

	txt = ui_widget_add_data(w, ui_textview.prototype, sizeof(ui_textview_t));
	txt->widget = w;
	txt->available_width = 0;
	txt->task.update_content = FALSE;
	txt->task.content = NULL;
	txt->content = NULL;
	txt->trimming = TRUE;
	txt->layer = TextLayer_New();
	TextLayer_SetAutoWrap(txt->layer, TRUE);
	TextLayer_SetMultiline(txt->layer, TRUE);
	TextLayer_EnableStyleTag(txt->layer, TRUE);
	ui_font_style_init(&txt->style);
	txt->node.data = txt;
	txt->node.prev = txt->node.next = NULL;
	list_append_node(&ui_textview.list, &txt->node);
}

static void ui_textview_on_destroy(ui_widget_t *w)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	list_unlink(&ui_textview.list, &txt->node);
	ui_font_style_destroy(&txt->style);
	TextLayer_Destroy(txt->layer);
	free(txt->content);
	if (txt->task.content) {
		free(txt->task.content);
		txt->task.content = NULL;
	}
}

static void ui_textview_on_auto_size(ui_widget_t *w, float *width, float *height,
				ui_layout_rule_t rule)
{
	int max_width, max_height;
	float scale = ui_get_scale();

	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	list_t rects;

	if (w->parent &&
	    w->parent->computed_style.width_sizing == UI_SIZING_RULE_FIXED) {
		txt->available_width = w->parent->box.content.width;
		max_width = (int)(scale * txt->available_width - padding_x(w) -
				  border_x(w));
	} else {
		txt->available_width = 0;
		max_width = 0;
	}
	switch (rule) {
	case UI_LAYOUT_RULE_FIXED_WIDTH:
		max_width = (int)(scale * w->box.content.width);
		max_height = 0;
		break;
	case UI_LAYOUT_RULE_FIXED_HEIGHT:
		max_height = (int)(scale * w->box.content.height);
		break;
	case UI_LAYOUT_RULE_FIXED:
		max_width = (int)(scale * w->box.content.width);
		max_height = (int)(scale * w->box.content.height);
		break;
	default:
		max_height = 0;
		break;
	}
	list_create(&rects);
	TextLayer_SetFixedSize(txt->layer, 0, 0);
	TextLayer_SetMaxSize(txt->layer, max_width, max_height);
	TextLayer_Update(txt->layer, &rects);
	TextLayer_ClearInvalidRect(txt->layer);
	*width = TextLayer_GetWidth(txt->layer) / scale;
	*height = TextLayer_GetHeight(txt->layer) / scale;
	pd_rects_clear(&rects);
}

static void ui_textview_on_resize(ui_widget_t *w, float width, float height)
{
	float scale = ui_get_scale();
	int fixed_width = (int)(width * scale);
	int fixed_height = (int)(height * scale);

	ui_rect_t rect;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	list_t rects;
	list_node_t *node;

	list_create(&rects);
	TextLayer_SetFixedSize(txt->layer, fixed_width, fixed_height);
	TextLayer_SetMaxSize(txt->layer, fixed_width, fixed_height);
	TextLayer_Update(txt->layer, &rects);
	TextLayer_ClearInvalidRect(txt->layer);
	for (list_each(node, &rects)) {
		ui_convert_rect(node->data, &rect, 1.0f / scale);
		ui_widget_mark_dirty_rect(w, &rect, CSS_KEYWORD_CONTENT_BOX);
	}
	pd_rects_clear(&rects);
}

static void ui_textview_on_paint(ui_widget_t *w, pd_paint_context_t *paint,
			     ui_widget_actual_style_t *style)
{
	pd_pos_t pos;
	pd_canvas_t canvas;
	pd_rect_t content_rect, rect;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	content_rect.width = style->content_box.width;
	content_rect.height = style->content_box.height;
	content_rect.x = style->content_box.x - style->canvas_box.x;
	content_rect.y = style->content_box.y - style->canvas_box.y;
	if (!pd_rect_overlap(&content_rect, &paint->rect, &rect)) {
		return;
	}
	pos.x = content_rect.x - rect.x;
	pos.y = content_rect.y - rect.y;
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	pd_canvas_quote(&canvas, &paint->canvas, &rect);
	rect = paint->rect;
	rect.x -= content_rect.x;
	rect.y -= content_rect.y;
	TextLayer_RenderTo(txt->layer, rect, pos, &canvas);
}

int ui_textview_set_text_w(ui_widget_t *w, const wchar_t *text)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;
	wchar_t *newtext = wcsdup2(text);

	if (!newtext) {
		return -ENOMEM;
	}
	if (txt->content) {
		free(txt->content);
	}
	txt->content = wcsdup2(text);
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
	if (txt->task.content) {
		free(txt->task.content);
	}
	txt->task.update_content = TRUE;
	txt->task.content = newtext;
	ui_widget_add_task(w, UI_TASK_USER);
	return 0;
}

int ui_textview_set_text(ui_widget_t *w, const char *utf8_text)
{
	int ret;
	wchar_t *wstr;
	size_t len = strlen(utf8_text) + 1;

	wstr = malloc(sizeof(wchar_t) * len);
	decode_utf8(wstr, utf8_text, len);
	ret = ui_textview_set_text_w(w, wstr);
	if (wstr) {
		free(wstr);
	}
	return ret;
}

void ui_textview_set_multiline(ui_widget_t *w, LCUI_BOOL enable)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);;

	TextLayer_SetMultiline(txt->layer, enable);
	ui_widget_add_task(w, UI_TASK_USER);
}

static void textview_on_font_face_load(ui_widget_t *w, ui_event_t *e, void *arg)
{
	ui_textview_t *txt;
	list_node_t *node;

	for (list_each(node, &ui_textview.list)) {
		txt = node->data;
		if (txt->widget->state != UI_WIDGET_STATE_DELETED) {
			ui_widget_refresh_style(txt->widget);
		}
	}
}

static void ui_textview_on_run_rask(ui_widget_t *w, int task)
{
	ui_textview_t *txt;

	txt = ui_widget_get_data(w, ui_textview.prototype);;
	if (txt->task.update_content) {
		TextLayer_SetTextW(txt->layer, txt->task.content, NULL);
		ui_textview_on_update(w);
		free(txt->task.content);
		txt->task.content = NULL;
		txt->task.update_content = FALSE;
	}
	if (task != UI_TASK_RESIZE ||
	    w->computed_style.width_sizing != UI_SIZING_RULE_FIT_CONTENT) {
		return;
	}
	if (w->parent && w->parent->box.content.width != txt->available_width) {
		txt->available_width = w->parent->box.content.width;
		ui_widget_add_task(w, UI_TASK_REFLOW);
	}
}

void ui_register_textview(void)
{
	ui_textview.css_key_word_break = css_register_property_name("word-break");
	ui_textview.prototype = ui_create_widget_prototype("textview", NULL);
	ui_textview.prototype->init = ui_textview_on_init;
	ui_textview.prototype->paint = ui_textview_on_paint;
	ui_textview.prototype->destroy = ui_textview_on_destroy;
	ui_textview.prototype->autosize = ui_textview_on_auto_size;
	ui_textview.prototype->resize = ui_textview_on_resize;
	ui_textview.prototype->update = ui_textview_on_update_style;
	ui_textview.prototype->settext = ui_textview_on_parse_text;
	ui_textview.prototype->setattr = ui_textview_on_parse_attr;
	ui_textview.prototype->runtask = ui_textview_on_run_rask;
	list_create(&ui_textview.list);
	ui_on_event("font_face_load", textview_on_font_face_load, NULL, NULL);
	css_register_property_parser(ui_textview.css_key_word_break, "word-break",
				     css_parse_word_break_property);
}

void ui_unregister_textview(void)
{
	list_destroy_without_node(&ui_textview.list, NULL);
	ui_off_event("font_face_load", textview_on_font_face_load);
}
