/*
 * textedit.c -- textedit widget, used to allow user edit text.
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <stdlib.h>
#include <string.h>
#include <LCUI/thread.h>
#include <LCUI/platform.h>
#include <pandagl.h>
#include <css.h>
#include "./internal.h"
#include "../include/textedit.h"
#include "../include/textcaret.h"

#define TEXTBLOCK_SIZE 512
#define DEFAULT_WIDTH 176.0f
#define PLACEHOLDER_COLOR RGB(140, 140, 140)

enum task_type_t { TASK_SET_TEXT, TASK_UPDATE, TASK_TOTAL };

typedef struct ui_textedit_t {
	ui_text_style_t style;          /**< 字体样式 */
	pd_text_t* layer_source;        /**< 实际文本层 */
	pd_text_t* layer_mask;          /**< 屏蔽后的文本层 */
	pd_text_t* layer_placeholder;   /**< 占位符的文本层 */
	pd_text_t* layer;               /**< 当前使用的文本层 */
	ui_widget_t* scrollbars[2];     /**< 两个滚动条 */
	ui_widget_t* caret;             /**< 文本插入符 */
	LCUI_BOOL is_read_only;         /**< 是否只读 */
	LCUI_BOOL is_multiline_mode;    /**< 是否为多行模式 */
	LCUI_BOOL is_placeholder_shown; /**< 是否已经显示占位符 */
	wchar_t* allow_input_char;      /**< 允许输入的字符 */
	wchar_t password_char;          /**< 屏蔽符的副本 */
	size_t text_block_size;         /**< 块大小 */
	list_t text_blocks;             /**< 文本块缓冲区 */
	list_t text_tags;               /**< 当前处理的标签列表 */
	LCUI_BOOL tasks[TASK_TOTAL];    /**< 待处理的任务 */
	thread_mutex_t mutex;           /**< 互斥锁 */
} ui_textedit_t;

typedef enum {
	TEXTBLOCK_BEGIN,
	TEXTBLOCK_BODY,
	TEXTBLOCK_END
} textblock_type_t;

typedef enum {
	TEXTBLOCK_ACTION_INSERT,
	TEXTBLOCK_ACTION_APPEND
} textblock_action_t;

typedef enum {
	TEXTBLOCK_OWNER_SOURCE,
	TEXTBLOCK_OWNER_PLACEHOLDER
} textblock_owner_t;

/** 文本块数据结构 */
typedef struct textblock_t {
	textblock_type_t type;     /**< 文本块类型 */
	textblock_owner_t owner;   /**< 所属的文本层 */
	textblock_action_t action; /**< 指定该文本块的添加方式 */
	wchar_t* text;             /**< 文本块(段) */
	size_t length;             /**< 文本块的长度 */
} textblock_t;

static ui_widget_prototype_t* ui_textedit_proto;

static const char *ui_textedit_css = css_string(

textedit {
	min-width: 124px;
	min-height: 14px;
	background-color: #fff;
	border: 1px solid #eee;
	padding: 5px 10px;
	display: inline-block;
}
textedit:focus {
	border: 1px solid #2196F3;
	box-shadow: 0 0 6px rgba(33,150,243,0.4);
}

textedit:disabled {
	opacity: 0.45;
}

);

static void fillchar(wchar_t* str, wchar_t ch)
{
	if (str) {
		wchar_t* p;
		for (p = str; *p; ++p) {
			*p = ch;
		}
	}
}

static void ui_textedit_update_caret(ui_widget_t* widget)
{
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);

	int row = edit->layer->insert_y;
	int offset_x, offset_y;
	float height, width;
	float scale = ui_metrics.scale;
	float x, y, caret_x = 0, caret_y = 0;

	if (!edit->is_placeholder_shown) {
		pd_pos_t pos;
		if (pd_text_get_insert_pixel_position(edit->layer, &pos) != 0) {
			return;
		}
		caret_x = pos.x / scale;
		caret_y = pos.y / scale;
	}
	offset_x = y_iround(edit->layer->offset_x / scale);
	offset_y = y_iround(edit->layer->offset_y / scale);
	x = caret_x + offset_x;
	y = caret_y + offset_y;
	width = edit->layer->width / scale;
	height = pd_text_get_line_height(edit->layer, row) / scale;
	ui_widget_set_style_unit_value(edit->caret, css_key_height, height,
				       CSS_UNIT_PX);
	/* Keep the caret in the visible area */
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if (x + edit->caret->border_box.width > widget->content_box.width) {
		x = widget->content_box.width - edit->caret->border_box.width;
	}
	if (y + edit->caret->border_box.height > widget->content_box.height) {
		y = widget->content_box.height - edit->caret->border_box.height;
	}
	/* Keep current line text in the visible area */
	if (width < widget->content_box.width) {
		x = caret_x;
	} else if (edit->caret->border_box.width + offset_x + width <
		   widget->content_box.width) {
		x = caret_x + widget->content_box.width -
		    (edit->layer->width / scale);
	}
	offset_x = y_iround((x - caret_x) * scale);
	offset_y = y_iround((y - caret_y) * scale);
	if (pd_text_set_offset(edit->layer, offset_x, offset_y)) {
		edit->tasks[TASK_UPDATE] = TRUE;
		ui_widget_add_task(widget, UI_TASK_USER);
	}
	x += widget->computed_style.padding_left;
	y += widget->computed_style.padding_top;
	ui_widget_move(edit->caret, x, y);
	ui_textcaret_refresh(edit->caret);
	if (edit->password_char) {
		pd_text_set_insert_position(edit->layer_source,
					    edit->layer->insert_y,
					    edit->layer->insert_x);
	}
}

void ui_textedit_move_caret(ui_widget_t* widget, int row, int col)
{
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);
	if (edit->is_placeholder_shown) {
		row = col = 0;
	}
	pd_text_set_insert_position(edit->layer, row, col);
	ui_textedit_update_caret(widget);
}

static void textblock_destroy(textblock_t* blk)
{
	free(blk->text);
	blk->text = NULL;
	free(blk);
}

static int ui_textedit_add_textblock(ui_widget_t* widget, const wchar_t* wtext,
				     textblock_action_t action,
				     textblock_owner_t owner)
{
	const wchar_t* p;
	ui_textedit_t* edit;
	textblock_t* block;
	size_t i, j, len, tag_len, size;

	if (!wtext) {
		return -1;
	}
	len = wcslen(wtext);
	edit = ui_widget_get_data(widget, ui_textedit_proto);
	for (i = 0; i < len; ++i) {
		block = malloc(sizeof(textblock_t));
		if (!block) {
			return -ENOMEM;
		}
		block->owner = owner;
		block->action = action;
		size = edit->text_block_size;
		if (i == 0) {
			block->type = TEXTBLOCK_BEGIN;
		} else if (len - i > edit->text_block_size) {
			block->type = TEXTBLOCK_BODY;
		} else {
			size = len - i;
			block->type = TEXTBLOCK_END;
		}
		block->text = malloc(sizeof(wchar_t) * size);
		if (!block->text) {
			return -ENOMEM;
		}
		/* 如果未启用样式标签功能 */
		if (!edit->layer->style_tag_enabled) {
			for (j = 0; i < len && j < size - 1; ++j, ++i) {
				block->text[j] = wtext[i];
			}
			--i;
			block->text[j] = 0;
			block->length = j;
			list_append(&edit->text_blocks, block);
			continue;
		}
		for (j = 0; i < len && j < size - 1; ++j, ++i) {
			wchar_t* text;
			block->text[j] = wtext[i];
			/* 检测是否有样式标签 */
			p = pd_scan_style_open_tag(wtext + i, NULL, 0, NULL);
			if (!p) {
				p = pd_scan_style_close_tag(wtext + i, NULL);
				if (!p) {
					continue;
				}
			}
			/* 计算标签的长度 */
			tag_len = p - wtext - i;
			/* 若当前块大小能够容纳这个标签 */
			if (j + tag_len <= size - 1) {
				continue;
			}
			/* 重新计算该文本块的大小，并重新分配内存空间 */
			size = j + tag_len + 1;
			text = realloc(block->text, sizeof(wchar_t) * size);
			if (!text) {
				return -ENOMEM;
			}
			block->text = text;
		}
		--i;
		block->text[j] = 0;
		block->length = j;
		/* 添加文本块至缓冲区 */
		list_append(&edit->text_blocks, block);
	}
	edit->tasks[TASK_SET_TEXT] = TRUE;
	ui_widget_add_task(widget, UI_TASK_USER);
	return 0;
}

/** 更新文本框内的字体位图 */
static void TextEdit_ProcTextBlock(ui_widget_t* widget, textblock_t* txtblk)
{
	list_t* tags;
	ui_textedit_t* edit;
	pd_text_t* layer;

	edit = ui_widget_get_data(widget, ui_textedit_proto);
	switch (txtblk->owner) {
	case TEXTBLOCK_OWNER_SOURCE:
		layer = edit->layer_source;
		tags = &edit->text_tags;
		break;
	case TEXTBLOCK_OWNER_PLACEHOLDER:
		layer = edit->layer_placeholder;
		tags = NULL;
		break;
	default:
		return;
	}
	switch (txtblk->action) {
	case TEXTBLOCK_ACTION_APPEND:
		/* 将此文本块追加至文本末尾 */
		pd_text_append(layer, txtblk->text, tags);
		break;
	case TEXTBLOCK_ACTION_INSERT:
		/* 将此文本块插入至文本插入符所在处 */
		pd_text_insert(layer, txtblk->text, tags);
	default:
		break;
	}
	if (edit->password_char && txtblk->owner == TEXTBLOCK_OWNER_SOURCE) {
		wchar_t* text = malloc(sizeof(wchar_t) * txtblk->length + 1);
		wcsncpy(text, txtblk->text, txtblk->length + 1);
		fillchar(text, edit->password_char);
		layer = edit->layer_mask;
		if (txtblk->action == TEXTBLOCK_ACTION_INSERT) {
			pd_text_insert(layer, text, NULL);
		} else {
			pd_text_append(layer, text, NULL);
		}
		free(text);
	}
}

/** 更新文本框的文本图层 */
static void TextEdit_UpdateTextLayer(ui_widget_t* w)
{
	float scale;
	list_t rects;
	ui_rect_t rect;
	ui_textedit_t* edit;
	pd_text_style_t style;
	list_node_t* node;

	list_create(&rects);
	scale = ui_metrics.scale;
	edit = ui_widget_get_data(w, ui_textedit_proto);
	pd_text_style_copy(&style, &edit->layer_source->default_style);
	if (edit->password_char) {
		pd_text_set_style(edit->layer_mask, &style);
	}
	style.has_fore_color = TRUE;
	style.fore_color = PLACEHOLDER_COLOR;
	pd_text_set_style(edit->layer_placeholder, &style);
	pd_text_style_destroy(&style);
	pd_text_update(edit->layer, &rects);
	for (list_each(node, &rects)) {
		ui_convert_rect(node->data, &rect, 1.0f / scale);
		ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
	}
	pd_rects_clear(&rects);
}

static void ui_textedit_on_task(ui_widget_t* widget, int task)
{
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);

	if (edit->tasks[TASK_SET_TEXT]) {
		list_t blocks;
		list_node_t* node;
		ui_event_t ev;

		list_create(&blocks);
		thread_mutex_lock(&edit->mutex);
		list_concat(&blocks, &edit->text_blocks);
		thread_mutex_unlock(&edit->mutex);
		for (list_each(node, &blocks)) {
			TextEdit_ProcTextBlock(widget, node->data);
		}
		list_destroy(&blocks,
			     (list_item_destructor_t)textblock_destroy);
		ui_event_init(&ev, "change");
		ui_widget_emit_event(widget, ev, NULL);
		edit->tasks[TASK_SET_TEXT] = FALSE;
		edit->tasks[TASK_UPDATE] = TRUE;
	}
	if (edit->tasks[TASK_UPDATE]) {
		LCUI_BOOL is_shown;
		is_shown = edit->layer_source->length == 0;
		if (is_shown) {
			edit->layer = edit->layer_placeholder;
		} else if (edit->password_char) {
			edit->layer = edit->layer_mask;
		} else {
			edit->layer = edit->layer_source;
		}
		TextEdit_UpdateTextLayer(widget);
		if (edit->is_placeholder_shown != is_shown) {
			ui_widget_mark_dirty_rect(widget, NULL,
						  UI_BOX_TYPE_PADDING_BOX);
		}
		edit->is_placeholder_shown = is_shown;
		edit->tasks[TASK_UPDATE] = FALSE;
		ui_textedit_update_caret(widget);
	}
}

static void ui_textedit_on_resize(ui_widget_t* w, float width, float height)
{
	float scale = ui_metrics.scale;

	list_t rects;
	list_node_t* node;

	ui_rect_t rect;
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);

	list_create(&rects);
	pd_text_set_fixed_size(edit->layer, (int)(width * scale),
			       (int)(width * scale));
	pd_text_set_max_size(edit->layer, (int)(height * scale),
			     (int)(height * scale));
	pd_text_update(edit->layer, &rects);
	for (list_each(node, &rects)) {
		ui_convert_rect(node->data, &rect, 1.0f / scale);
		ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
	}
	pd_rects_clear(&rects);
}

static void ui_textedit_on_auto_size(ui_widget_t* w, float* width,
				     float* height, ui_layout_rule_t rule)
{
	int i, n;
	int max_width = 0, max_height = 0;
	float scale = ui_metrics.scale;

	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);

	switch (rule) {
	case UI_LAYOUT_RULE_FIXED_WIDTH:
		max_width = ui_compute(w->content_box.width);
		if (edit->is_multiline_mode) {
			n = y_max(pd_text_get_lines_length(edit->layer), 6);
			for (max_height = 0, i = 0; i < n; ++i) {
				max_height +=
				    pd_text_get_line_height(edit->layer, i);
			}
		} else {
			max_height = pd_text_get_height(edit->layer);
		}
		break;
	case UI_LAYOUT_RULE_FIXED_HEIGHT:
		max_width = ui_compute(DEFAULT_WIDTH);
		max_height = ui_compute(w->content_box.height);
		break;
	case UI_LAYOUT_RULE_FIXED:
		max_width = ui_compute(w->content_box.width);
		max_height = ui_compute(w->content_box.height);
		break;
	default:
		max_width = ui_compute(DEFAULT_WIDTH);
		if (edit->is_multiline_mode) {
			n = y_max(pd_text_get_lines_length(edit->layer), 6);
			for (max_height = 0, i = 0; i < n; ++i) {
				max_height +=
				    pd_text_get_line_height(edit->layer, i);
			}
		} else {
			max_height = pd_text_get_height(edit->layer);
		}
		break;
	}
	*height = max_height / scale;
	*width = max_width / scale;
}

void ui_textedit_enable_style_tag(ui_widget_t* widget, LCUI_BOOL enable)
{
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);
	pd_text_set_style_tag(edit->layer, enable);
}

/* FIXME: improve multiline editing mode of the textedit widget
 * The multipline editing mode of the textedit widget have not been fully
 * tested, it may have many problems.
 */

void ui_textedit_enable_multiline(ui_widget_t* w, LCUI_BOOL enable)
{
	if (enable) {
		ui_widget_set_style_keyword_value(w, css_key_white_space,
						  CSS_KEYWORD_AUTO);
	} else {
		ui_widget_set_style_keyword_value(w, css_key_white_space,
						  CSS_KEYWORD_NOWRAP);
	}
}

void ui_textedit_clear_text(ui_widget_t* widget)
{
	ui_textedit_t* edit;
	textblock_t* block;
	list_node_t *node, *prev;

	edit = ui_widget_get_data(widget, ui_textedit_proto);
	thread_mutex_lock(&edit->mutex);
	for (list_each(node, &edit->text_blocks)) {
		block = node->data;
		prev = node->prev;
		if (block->owner == TEXTBLOCK_OWNER_SOURCE) {
			textblock_destroy(block);
			list_delete_node(&edit->text_blocks, node);
			node = prev;
		}
	}
	pd_text_empty(edit->layer_source);
	pd_style_tags_clear(&edit->text_tags);
	edit->tasks[TASK_UPDATE] = TRUE;
	ui_widget_add_task(widget, UI_TASK_USER);
	thread_mutex_unlock(&edit->mutex);
	ui_widget_mark_dirty_rect(widget, NULL, UI_BOX_TYPE_PADDING_BOX);
}

size_t ui_textedit_get_text_w(ui_widget_t* w, size_t start, size_t max_len,
			      wchar_t* buf)
{
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);
	return pd_text_dump(edit->layer_source, start, max_len, buf);
}

size_t ui_textedit_get_text_length(ui_widget_t* w)
{
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);
	return edit->layer_source->length;
}

int ui_textedit_set_text_w(ui_widget_t* w, const wchar_t* wstr)
{
	ui_textedit_clear_text(w);
	return ui_textedit_add_textblock(w, wstr, TEXTBLOCK_ACTION_APPEND,
					 TEXTBLOCK_OWNER_SOURCE);
}

int ui_textedit_set_text(ui_widget_t* widget, const char* utf8_str)
{
	int ret;
	size_t len = strlen(utf8_str) + 1;
	wchar_t* wstr = malloc(len * sizeof(wchar_t));

	if (!wstr) {
		return -ENOMEM;
	}
	len = decode_utf8(wstr, utf8_str, len);
	wstr[len] = 0;
	ret = ui_textedit_set_text_w(widget, wstr);
	free(wstr);
	return ret;
}

void ui_textedit_set_passworld_char(ui_widget_t* w, wchar_t ch)
{
	size_t i, len;
	wchar_t text[256];
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);

	edit->password_char = ch;
	edit->tasks[TASK_UPDATE] = TRUE;
	ui_widget_add_task(w, UI_TASK_USER);
	pd_text_empty(edit->layer_mask);
	if (!edit->password_char) {
		edit->layer = edit->layer_source;
		return;
	}
	edit->layer = edit->layer_mask;
	len = ui_textedit_get_text_length(w);
	for (i = 0; i < len; i += 255) {
		ui_textedit_get_text_w(w, i, 255, text);
		fillchar(text, edit->password_char);
		pd_text_append(edit->layer, text, NULL);
	}
}

int ui_textedit_append_text_w(ui_widget_t* w, const wchar_t* wstr)
{
	return ui_textedit_add_textblock(w, wstr, TEXTBLOCK_ACTION_APPEND,
					 TEXTBLOCK_OWNER_SOURCE);
}

int ui_textedit_insert_text_w(ui_widget_t* w, const wchar_t* wstr)
{
	return ui_textedit_add_textblock(w, wstr, TEXTBLOCK_ACTION_INSERT,
					 TEXTBLOCK_OWNER_SOURCE);
}

int ui_textedit_set_placeholder_w(ui_widget_t* w, const wchar_t* wstr)
{
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);
	thread_mutex_lock(&edit->mutex);
	pd_text_empty(edit->layer_placeholder);
	thread_mutex_unlock(&edit->mutex);
	if (edit->is_placeholder_shown) {
		ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_PADDING_BOX);
	}
	return ui_textedit_add_textblock(w, wstr, TEXTBLOCK_ACTION_INSERT,
					 TEXTBLOCK_OWNER_PLACEHOLDER);
}

int ui_textedit_set_placeholder(ui_widget_t* w, const char* str)
{
	int ret;
	size_t len = strlen(str) + 1;
	wchar_t* wstr = malloc(len * sizeof(wchar_t));

	if (!wstr) {
		return -ENOMEM;
	}
	len = decode_utf8(wstr, str, len);
	wstr[len] = 0;
	ret = ui_textedit_set_placeholder_w(w, wstr);
	free(wstr);
	return ret;
}

void ui_textedit_set_caret_blink(ui_widget_t* w, LCUI_BOOL visible, int time)
{
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);
	ui_textcaret_set_visible(edit->caret, visible);
	ui_textcaret_set_blink_time(edit->caret, time);
}

static void ui_textedit_on_parse_text(ui_widget_t* w, const char* text)
{
	ui_textedit_set_text(w, text);
}

static void ui_textedit_on_focus(ui_widget_t* widget, ui_event_t* e, void* arg)
{
	ui_textedit_t* edit;

	edit = ui_widget_get_data(widget, ui_textedit_proto);
	ui_textcaret_set_visible(edit->caret, TRUE);
	ui_textedit_update_caret(widget);
}

static void ui_textedit_on_blur(ui_widget_t* widget, ui_event_t* e, void* arg)
{
	ui_textedit_t* edit;

	edit = ui_widget_get_data(widget, ui_textedit_proto);
	ui_textcaret_set_visible(edit->caret, FALSE);
	/* In single-line editing mode, we should reset the caret position to
	 * the head, otherwise it will mistakenly think that only the last part
	 * is entered after inputting long text. */
	if (!edit->is_multiline_mode) {
		ui_textedit_move_caret(widget, 0, 0);
	}
}

static void ui_textedit_on_press_backspace_key(ui_widget_t* widget, int n_ch)
{
	ui_textedit_t* edit;
	ui_event_t ev;

	edit = ui_widget_get_data(widget, ui_textedit_proto);
	thread_mutex_lock(&edit->mutex);
	pd_text_backspace(edit->layer_source, n_ch);
	if (edit->password_char) {
		pd_text_backspace(edit->layer_mask, n_ch);
	}
	ui_textcaret_refresh(edit->caret);
	edit->tasks[TASK_UPDATE] = TRUE;
	ui_widget_add_task(widget, UI_TASK_USER);
	thread_mutex_unlock(&edit->mutex);
	ui_event_init(&ev, "change");
	ui_widget_emit_event(widget, ev, NULL);
}

static void ui_textedit_on_press_delete_key(ui_widget_t* widget, int n_ch)
{
	ui_textedit_t* edit;
	ui_event_t ev;

	edit = ui_widget_get_data(widget, ui_textedit_proto);
	thread_mutex_lock(&edit->mutex);
	pd_text_delete(edit->layer_source, n_ch);
	if (edit->password_char) {
		pd_text_delete(edit->layer_mask, n_ch);
	}
	ui_textcaret_refresh(edit->caret);
	edit->tasks[TASK_UPDATE] = TRUE;
	ui_widget_add_task(widget, UI_TASK_USER);
	thread_mutex_unlock(&edit->mutex);
	ui_event_init(&ev, "change");
	ui_widget_emit_event(widget, ev, NULL);
}

static void ui_textedit_on_paste(ui_widget_t* w, ui_event_t* e, void* arg)
{
	clipboard_t* clipboard = arg;
	if (clipboard) {
		ui_textedit_insert_text_w(w, clipboard->text);
	}
}

static void ui_textedit_on_clipboard_ready(void* arg, ui_widget_t* widget)
{
	clipboard_t* clipboard = arg;
	ui_event_t e = { 0 };

	ui_event_init(&e, "paste");
	ui_widget_emit_event(widget, e, clipboard);
}

/** 处理按键事件 */
static void ui_textedit_on_keydown(ui_widget_t* widget, ui_event_t* e,
				   void* arg)
{
	int cols, rows;
	int cur_col, cur_row;
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);

	cur_row = edit->layer->insert_y;
	cur_col = edit->layer->insert_x;
	rows = pd_text_get_lines_length(edit->layer);
	cols = pd_text_get_line_length(edit->layer, cur_row);
	e->cancel_bubble = TRUE;
	switch (e->key.code) {
	case KEY_HOME:
		cur_col = 0;
		break;
	case KEY_END:
		cur_col = cols;
		break;
	case KEY_LEFT:
		if (cur_col > 0) {
			--cur_col;
		} else if (cur_row > 0) {
			--cur_row;
			cur_col = pd_text_get_line_length(edit->layer, cur_row);
		}
		break;
	case KEY_RIGHT:
		if (cur_col < cols) {
			++cur_col;
		} else if (cur_row < rows - 1) {
			++cur_row;
			cur_col = 0;
		}
		break;
	case KEY_UP:
		if (cur_row > 0) {
			--cur_row;
		}
		break;
	case KEY_DOWN:
		if (cur_row < rows - 1) {
			++cur_row;
		}
		break;
	case KEY_BACKSPACE:
		ui_textedit_on_press_backspace_key(widget, 1);
		return;
	case KEY_DELETE:
		ui_textedit_on_press_delete_key(widget, 1);
		return;
	default:
		break;
	}
	ui_textedit_move_caret(widget, cur_row, cur_col);

	char key = e->key.code;
	// CTRL+V
	if (key == KEY_V && e->key.ctrl_key) {
		clipboard_request_text(
		    (clipboard_callback_t)ui_textedit_on_clipboard_ready,
		    widget);
	}
	// CTRL+C
	if (key == KEY_C && e->key.ctrl_key) {
		// @WhoAteDaCake
		// Currently copies internal widget text
		// once selection is implemented, it would copy that instead
		size_t len = ui_textedit_get_text_length(widget);
		wchar_t* wcs = malloc((len + 1) * sizeof(wchar_t));
		ui_textedit_get_text_w(widget, 0, len, wcs);
		clipboard_set_text(wcs, len);
		free(wcs);
	}
}

/** 处理输入法对文本框输入的内容 */
static void ui_textedit_on_textinput(ui_widget_t* widget, ui_event_t* e,
				     void* arg)
{
	unsigned i, j, k;
	wchar_t ch, *text, excludes[8] = L"\b\r\t\x1b";
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);

	/* 如果不是多行文本编辑模式则删除换行符 */
	if (!edit->is_multiline_mode) {
		wcscat(excludes, L"\n");
	}
	/* 如果文本框是只读的 */
	if (edit->is_read_only) {
		return;
	}
	text = malloc(sizeof(wchar_t) * (e->text.length + 1));
	if (!text) {
		return;
	}
	for (i = 0, j = 0; i < e->text.length; ++i) {
		ch = e->text.text[i];
		for (k = 0; excludes[k]; ++k) {
			if (ch == excludes[k]) {
				break;
			}
		}
		if (excludes[k]) {
			continue;
		}
		if (!edit->allow_input_char) {
			text[j++] = ch;
			continue;
		}
		/* 判断当前字符是否为限制范围内的字符 */
		for (j = 0; edit->allow_input_char[j]; ++j) {
			if (edit->allow_input_char[j] == ch) {
				break;
			}
		}
		/* 如果已提前结束循环，则表明当前字符是允许的 */
		if (edit->allow_input_char[j]) {
			text[j++] = e->text.text[i];
			continue;
		}
		text[j] = 0;
	}
	text[j] = 0;
	ui_textedit_insert_text_w(widget, text);
	free(text);
}

static void ui_textedit_on_mousemove(ui_widget_t* w, ui_event_t* e, void* arg)
{
	float offset_x, offset_y;
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);

	if (edit->is_placeholder_shown) {
		ui_textedit_update_caret(w);
		return;
	}
	ui_widget_get_offset(w, NULL, &offset_x, &offset_y);
	pd_text_set_insert_pixel_position(
	    edit->layer,
	    ui_compute(e->mouse.x - offset_x - w->computed_style.padding_left),
	    ui_compute(e->mouse.y - offset_y - w->computed_style.padding_top));
	ui_textedit_update_caret(w);
}

static void ui_textedit_on_mouseup(ui_widget_t* w, ui_event_t* e, void* arg)
{
	ui_widget_release_mouse_capture(w);
	ui_widget_off(w, "mousemove", ui_textedit_on_mousemove, w);
}

static void ui_textedit_on_mousedown(ui_widget_t* w, ui_event_t* e, void* arg)
{
	float offset_x, offset_y;
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);

	ui_widget_get_offset(w, NULL, &offset_x, &offset_y);
	pd_text_set_insert_pixel_position(
	    edit->layer,
	    ui_compute(e->mouse.x - offset_x - w->computed_style.padding_left),
	    ui_compute(e->mouse.y - offset_y - w->computed_style.padding_top));
	ui_textedit_update_caret(w);
	ui_widget_set_mouse_capture(w);
	ui_widget_on(w, "mousemove", ui_textedit_on_mousemove, NULL, NULL);
}

static void ui_textedit_on_ready(ui_widget_t* w, ui_event_t* e, void* arg)
{
	ui_textedit_update_caret(w);
}

static void ui_textedit_on_set_attr(ui_widget_t* w, const char* name,
				    const char* val)
{
	if (strcmp(name, "placeholder") == 0) {
		ui_textedit_set_placeholder(w, val);
	}
}

static void ui_textedit_on_init(ui_widget_t* w)
{
	ui_textedit_t* edit;

	edit = ui_widget_add_data(w, ui_textedit_proto, sizeof(ui_textedit_t));
	edit->is_read_only = FALSE;
	edit->password_char = 0;
	edit->allow_input_char = NULL;
	edit->is_placeholder_shown = FALSE;
	edit->is_multiline_mode = FALSE;
	edit->layer_mask = pd_text_create();
	edit->layer_source = pd_text_create();
	edit->layer_placeholder = pd_text_create();
	edit->layer = edit->layer_source;
	edit->text_block_size = TEXTBLOCK_SIZE;
	edit->caret = ui_create_widget("textcaret");
	w->tab_index = 0;
	memset(edit->tasks, 0, sizeof(edit->tasks));
	list_create(&edit->text_blocks);
	list_create(&edit->text_tags);
	ui_textedit_enable_multiline(w, FALSE);
	pd_text_set_autowrap(edit->layer, TRUE);
	pd_text_set_autowrap(edit->layer_mask, TRUE);
	pd_text_set_style_tag(edit->layer, FALSE);
	ui_widget_on(w, "textinput", ui_textedit_on_textinput, NULL, NULL);
	ui_widget_on(w, "mousedown", ui_textedit_on_mousedown, NULL, NULL);
	ui_widget_on(w, "mouseup", ui_textedit_on_mouseup, NULL, NULL);
	ui_widget_on(w, "keydown", ui_textedit_on_keydown, NULL, NULL);
	ui_widget_on(w, "focus", ui_textedit_on_focus, NULL, NULL);
	ui_widget_on(w, "blur", ui_textedit_on_blur, NULL, NULL);
	ui_widget_on(w, "ready", ui_textedit_on_ready, NULL, NULL);
	ui_widget_on(w, "paste", ui_textedit_on_paste, NULL, NULL);
	ui_widget_append(w, edit->caret);
	ui_widget_hide(edit->caret);
	thread_mutex_init(&edit->mutex);
	ui_text_style_init(&edit->style);
}

static void ui_textedit_on_destroy(ui_widget_t* widget)
{
	ui_textedit_t* edit = ui_widget_get_data(widget, ui_textedit_proto);

	edit->layer = NULL;
	pd_text_destroy(edit->layer_source);
	pd_text_destroy(edit->layer_placeholder);
	pd_text_destroy(edit->layer_mask);
	ui_text_style_destroy(&edit->style);
	thread_mutex_destroy(&edit->mutex);
	list_destroy(&edit->text_blocks,
		     (list_item_destructor_t)textblock_destroy);
}

static void ui_textedit_on_paint(ui_widget_t* w, pd_context_t* paint,
				 ui_widget_actual_style_t* style)
{
	pd_pos_t pos;
	pd_canvas_t canvas;
	pd_rect_t content_rect, rect;
	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);

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
	pd_text_render_to(edit->layer, rect, pos, &canvas);
}

static void ui_textedit_on_update_style(ui_widget_t* w)
{
	int i;

	ui_textedit_t* edit = ui_widget_get_data(w, ui_textedit_proto);
	pd_text_style_t text_style;
	ui_text_style_t style;
	pd_text_t* layers[3] = { edit->layer_mask, edit->layer_placeholder,
				 edit->layer_source };

	ui_text_style_init(&style);
	ui_compute_text_style(&style, &w->computed_style);
	if (ui_text_style_is_equal(&style, &edit->style)) {
		ui_text_style_destroy(&style);
		return;
	}
	convert_font_style_to_text_style(&style, &text_style);
	for (i = 0; i < 3; ++i) {
		pd_text_set_align(layers[i], style.text_align);
		pd_text_set_line_height(layers[i], style.line_height);
		pd_text_set_autowrap(
		    layers[i], style.white_space != CSS_WHITE_SPACE_NOWRAP);
		pd_text_set_style(layers[i], &text_style);
	}
	ui_text_style_destroy(&edit->style);
	pd_text_style_destroy(&text_style);
	edit->style = style;
	edit->tasks[TASK_UPDATE] = TRUE;
	ui_widget_add_task(w, UI_TASK_USER);
}

void ui_register_textedit(void)
{
	ui_textedit_proto = ui_create_widget_prototype("textedit", NULL);
	ui_textedit_proto->init = ui_textedit_on_init;
	ui_textedit_proto->paint = ui_textedit_on_paint;
	ui_textedit_proto->destroy = ui_textedit_on_destroy;
	ui_textedit_proto->settext = ui_textedit_on_parse_text;
	ui_textedit_proto->setattr = ui_textedit_on_set_attr;
	ui_textedit_proto->autosize = ui_textedit_on_auto_size;
	ui_textedit_proto->resize = ui_textedit_on_resize;
	ui_textedit_proto->runtask = ui_textedit_on_task;
	ui_textedit_proto->update = ui_textedit_on_update_style;
	ui_load_css_string(ui_textedit_css, __FILE__);
}
