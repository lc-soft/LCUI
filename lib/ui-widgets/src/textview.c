/*
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
#include <pandagl.h>
#include <css.h>
#include <ui_widgets/textview.h>
#include "textstyle.h"

typedef struct ui_textview_task_t {
	wchar_t *content;
	bool update_content;
} ui_textview_task_t;

typedef struct ui_textview_t_ {
	wchar_t *content;
	bool trimming;
	ui_widget_t *widget;
	pd_text_t *layer;
	ui_text_style_t style;
	ui_textview_task_t task;
	list_node_t node;
} ui_textview_t;

static struct ui_textview_module_t {
	list_t list;
	ui_widget_prototype_t *prototype;
} ui_textview;

static bool parse_boolean(const char *str)
{
	if (strcmp(str, "on") == 0 && strcmp(str, "true") == 0 &&
	    strcmp(str, "yes") == 0 && strcmp(str, "1") == 0) {
		return TRUE;
	}
	return FALSE;
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
		bool enable = parse_boolean(value);
		if (enable != txt->layer->mulitiline_enabled) {
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
	float scale = ui_metrics.scale;

	ui_rect_t rect;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);

	list_t rects;
	list_node_t *node;

	list_create(&rects);
	pd_text_update(txt->layer, &rects);
	for (list_each(node, &rects)) {
		ui_convert_rect(node->data, &rect, 1.0f / scale);
		ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
	}
	pd_rects_clear(&rects);
	ui_widget_add_task(w, UI_TASK_REFLOW);
}

static void ui_textview_on_update_style(ui_widget_t *w)
{
	ui_text_style_t style;
	pd_text_style_t text_style;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);

	ui_text_style_init(&style);
	ui_compute_text_style(&style, &w->computed_style);
	if (ui_text_style_is_equal(&style, &txt->style)) {
		ui_text_style_destroy(&style);
		return;
	}
	convert_font_style_to_text_style(&style, &text_style);
	pd_text_set_align(txt->layer, style.text_align);
	pd_text_set_line_height(txt->layer, style.line_height);
	pd_text_set_autowrap(txt->layer,
			     style.white_space != CSS_WHITE_SPACE_NOWRAP);
	pd_text_set_word_break(txt->layer, style.word_break == CSS_WORD_BREAK_BREAK_ALL
				   ? PD_WORD_BREAK_BREAK_ALL
				   : PD_WORD_BREAK_NORMAL);
	pd_text_set_style(txt->layer, &text_style);
	if (style.content) {
		ui_textview_set_text_w(w, style.content);
	} else if (txt->style.content) {
		ui_textview_set_text_w(w, NULL);
	}
	ui_text_style_destroy(&txt->style);
	pd_text_style_destroy(&text_style);
	txt->style = style;
	ui_textview_on_update(w);
}

static void ui_textview_on_init(ui_widget_t *w)
{
	ui_textview_t *txt;

	txt =
	    ui_widget_add_data(w, ui_textview.prototype, sizeof(ui_textview_t));
	txt->widget = w;
	txt->task.update_content = FALSE;
	txt->task.content = NULL;
	txt->content = NULL;
	txt->trimming = TRUE;
	txt->layer = pd_text_create();
	pd_text_set_autowrap(txt->layer, TRUE);
	pd_text_set_multiline(txt->layer, TRUE);
	pd_text_set_style_tag(txt->layer, TRUE);
	ui_text_style_init(&txt->style);
	txt->node.data = txt;
	txt->node.prev = txt->node.next = NULL;
	list_append_node(&ui_textview.list, &txt->node);
}

static void ui_textview_on_destroy(ui_widget_t *w)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);

	list_unlink(&ui_textview.list, &txt->node);
	ui_text_style_destroy(&txt->style);
	pd_text_destroy(txt->layer);
	free(txt->content);
	if (txt->task.content) {
		free(txt->task.content);
		txt->task.content = NULL;
	}
}

static void ui_textview_on_auto_size(ui_widget_t *w, float *width,
				     float *height, ui_layout_rule_t rule)
{
	float max_width, max_height;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);
	list_t rects;

	if (w->parent &&
	    IS_CSS_FIXED_LENGTH(&w->parent->computed_style, width)) {
		max_width = w->parent->content_box.width -
			    (w->border_box.width - w->content_box.width);
	} else {
		max_width = 0;
	}
	switch (rule) {
	case UI_LAYOUT_RULE_FIXED_WIDTH:
		max_width = w->content_box.width;
		max_height = 0;
		break;
	case UI_LAYOUT_RULE_FIXED_HEIGHT:
		max_height = w->content_box.height;
		break;
	case UI_LAYOUT_RULE_FIXED:
		max_width = w->content_box.width;
		max_height = w->content_box.height;
		break;
	default:
		max_height = 0;
		break;
	}
	list_create(&rects);
	pd_text_set_fixed_size(txt->layer, 0, 0);
	pd_text_set_max_size(txt->layer, ui_compute(max_width),
			     ui_compute(max_height));
	pd_text_update(txt->layer, &rects);
	*width = pd_text_get_width(txt->layer) / ui_metrics.scale;
	*height = pd_text_get_height(txt->layer) / ui_metrics.scale;
	pd_rects_clear(&rects);
}

static void ui_textview_on_resize(ui_widget_t *w, float width, float height)
{
	float scale = ui_metrics.scale;
	int fixed_width = (int)(width * scale);
	int fixed_height = (int)(height * scale);

	ui_rect_t rect;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);
	;

	list_t rects;
	list_node_t *node;

	list_create(&rects);
	pd_text_set_fixed_size(txt->layer, fixed_width, fixed_height);
	pd_text_set_max_size(txt->layer, fixed_width, fixed_height);
	pd_text_update(txt->layer, &rects);
	for (list_each(node, &rects)) {
		ui_convert_rect(node->data, &rect, 1.0f / scale);
		ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
	}
	pd_rects_clear(&rects);
}

static void ui_textview_on_paint(ui_widget_t *w, pd_context_t *paint,
				 ui_widget_actual_style_t *style)
{
	pd_pos_t pos;
	pd_canvas_t canvas;
	pd_rect_t content_rect, rect;
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);
	;

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
	pd_text_render_to(txt->layer, rect, pos, &canvas);
}

int ui_textview_set_text_w(ui_widget_t *w, const wchar_t *text)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);
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

void ui_textview_set_multiline(ui_widget_t *w, bool enable)
{
	ui_textview_t *txt = ui_widget_get_data(w, ui_textview.prototype);

	pd_text_set_multiline(txt->layer, enable);
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

	txt = ui_widget_get_data(w, ui_textview.prototype);
	if (txt->task.update_content) {
		pd_text_write(txt->layer, txt->task.content, NULL);
		ui_textview_on_update(w);
		free(txt->task.content);
		txt->task.content = NULL;
		txt->task.update_content = FALSE;
		ui_widget_add_task(w->parent, UI_TASK_REFLOW);
	}
}

void ui_register_textview(void)
{
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
}

void ui_unregister_textview(void)
{
	list_destroy_without_node(&ui_textview.list, NULL);
	ui_off_event("font_face_load", textview_on_font_face_load, NULL);
}
