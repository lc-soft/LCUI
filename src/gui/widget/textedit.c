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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/css_fontstyle.h>
#include <LCUI/gui/widget/textedit.h>
#include <LCUI/gui/widget/textcaret.h>
#include <LCUI/ime.h>

#define TEXT_BLOCK_SIZE 512
#define DEFAULT_WIDTH 176.0f
#define PLACEHOLDER_COLOR RGB(140, 140, 140)
#define GetData(W) Widget_GetData(W, self.prototype)
#define AddData(W) Widget_AddData(W, self.prototype, sizeof(LCUI_TextEditRec))
#define TextBlocks_Clear(blocks) LinkedList_Clear(blocks, TextBlock_OnDestroy)

enum TaskType {
	TASK_SET_TEXT,
	TASK_UPDATE,
	TASK_UPDATE_MASK,
	TASK_UPDATE_CARET,
	TASK_TOTAL
};

typedef struct LCUI_TextEditRec_ {
	LCUI_CSSFontStyleRec style;       /**< 字体样式 */
	LCUI_TextLayer layer_source;      /**< 实际文本层 */
	LCUI_TextLayer layer_mask;        /**< 屏蔽后的文本层 */
	LCUI_TextLayer layer_placeholder; /**< 占位符的文本层 */
	LCUI_TextLayer layer;             /**< 当前使用的文本层 */
	LCUI_ObjectWatcher value_watcher;
	LCUI_Widget scrollbars[2];      /**< 两个滚动条 */
	LCUI_Widget caret;              /**< 文本插入符 */
	LCUI_BOOL is_read_only;         /**< 是否只读 */
	LCUI_BOOL is_multiline_mode;    /**< 是否为多行模式 */
	LCUI_BOOL is_placeholder_shown; /**< 是否已经显示占位符 */
	wchar_t *allow_input_char;      /**< 允许输入的字符 */
	wchar_t password_char;          /**< 屏蔽符的副本 */
	size_t text_block_size;         /**< 块大小 */
	LinkedList text_blocks;         /**< 文本块缓冲区 */
	LinkedList text_tags;           /**< 当前处理的标签列表 */
	LCUI_BOOL tasks[TASK_TOTAL];    /**< 待处理的任务 */
	LCUI_Mutex mutex;               /**< 互斥锁 */
} LCUI_TextEditRec, *LCUI_TextEdit;

typedef enum {
	TEXT_BLOCK_BEGIN,
	TEXT_BLOCK_BODY,
	TEXT_BLOCK_END
} TextBlockType;

typedef enum {
	TEXT_BLOCK_ACTION_INSERT,
	TEXT_BLOCK_ACTION_APPEND
} TextBlockAction;

typedef enum {
	TEXT_BLOCK_OWNER_SOURCE,
	TEXT_BLOCK_OWNER_PLACEHOLDER
} TextBlockOwner;

/** 文本块数据结构 */
typedef struct LCUI_TextBlockRec_ {
	TextBlockType type;     /**< 文本块类型 */
	TextBlockOwner owner;   /**< 所属的文本层 */
	TextBlockAction action; /**< 指定该文本块的添加方式 */
	wchar_t *text;          /**< 文本块(段) */
	size_t length;          /**< 文本块的长度 */
} LCUI_TextBlockRec, *LCUI_TextBlock;

static struct LCUI_TextEditModule {
	LCUI_WidgetPrototype prototype;
} self;

static const char *textedit_css = CodeToString(

textedit {
	min-width: 124px;
	min-height: 14px;
	background-color: #fff;
	border: 1px solid #eee;
	padding: 5px 10px;
	focusable: true;
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

static void fillchar(wchar_t *str, wchar_t ch)
{
	if (str) {
		wchar_t *p;
		for (p = str; *p; ++p) {
			*p = ch;
		}
	}
}

static void TextEdit_UpdateCaret(LCUI_Widget widget)
{
	LCUI_TextEdit edit = GetData(widget);

	int row = edit->layer->insert_y;
	int offset_x, offset_y;
	float height, width;
	float scale = LCUIMetrics_GetScale();
	float x, y, caret_x = 0, caret_y = 0;

	if (!edit->is_placeholder_shown) {
		LCUI_Pos pos;
		if (TextLayer_GetCaretPixelPos(edit->layer, &pos) != 0) {
			return;
		}
		caret_x = pos.x / scale;
		caret_y = pos.y / scale;
	}
	offset_x = iround(edit->layer->offset_x / scale);
	offset_y = iround(edit->layer->offset_y / scale);
	x = caret_x + offset_x;
	y = caret_y + offset_y;
	width = edit->layer->width / scale;
	height = TextLayer_GetRowHeight(edit->layer, row) / scale;
	Widget_SetStyle(edit->caret, key_height, height, px);
	/* Keep the caret in the visible area */
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if (x + edit->caret->width > widget->box.content.width) {
		x = widget->box.content.width - edit->caret->width;
	}
	if (y + edit->caret->height > widget->box.content.height) {
		y = widget->box.content.height - edit->caret->height;
	}
	/* Keep current line text in the visible area */
	if (width < widget->box.content.width) {
		x = caret_x;
	} else if (edit->caret->width + offset_x + width <
		   widget->box.content.width) {
		x = caret_x + widget->box.content.width -
		    (edit->layer->width / scale);
	}
	offset_x = iround((x - caret_x) * scale);
	offset_y = iround((y - caret_y) * scale);
	if (TextLayer_SetOffset(edit->layer, offset_x, offset_y)) {
		edit->tasks[TASK_UPDATE] = TRUE;
		Widget_AddTask(widget, LCUI_WTASK_USER);
	}
	x += widget->padding.left;
	y += widget->padding.top;
	Widget_Move(edit->caret, x, y);
	TextCaret_Refresh(edit->caret);
	if (edit->password_char) {
		TextLayer_SetCaretPos(edit->layer_source, edit->layer->insert_y,
				      edit->layer->insert_x);
	}
}

void TextEdit_MoveCaret(LCUI_Widget widget, int row, int col)
{
	LCUI_TextEdit edit = Widget_GetData(widget, self.prototype);
	if (edit->is_placeholder_shown) {
		row = col = 0;
	}
	TextLayer_SetCaretPos(edit->layer, row, col);
	TextEdit_UpdateCaret(widget);
}

static void TextBlock_OnDestroy(void *arg)
{
	LCUI_TextBlock blk = arg;
	free(blk->text);
	blk->text = NULL;
	free(blk);
}

static int TextEdit_AddTextBlock(LCUI_Widget widget, const wchar_t *wtext,
				 TextBlockAction action, TextBlockOwner owner)
{
	const wchar_t *p;
	LCUI_TextEdit edit;
	LCUI_TextBlock block;
	size_t i, j, len, tag_len, size;

	if (!wtext) {
		return -1;
	}
	len = wcslen(wtext);
	edit = Widget_GetData(widget, self.prototype);
	for (i = 0; i < len; ++i) {
		block = NEW(LCUI_TextBlockRec, 1);
		if (!block) {
			return -ENOMEM;
		}
		block->owner = owner;
		block->action = action;
		size = edit->text_block_size;
		if (i == 0) {
			block->type = TEXT_BLOCK_BEGIN;
		} else if (len - i > edit->text_block_size) {
			block->type = TEXT_BLOCK_BODY;
		} else {
			size = len - i;
			block->type = TEXT_BLOCK_END;
		}
		block->text = NEW(wchar_t, size);
		if (!block->text) {
			return -ENOMEM;
		}
		/* 如果未启用样式标签功能 */
		if (!edit->layer->enable_style_tag) {
			for (j = 0; i < len && j < size - 1; ++j, ++i) {
				block->text[j] = wtext[i];
			}
			--i;
			block->text[j] = 0;
			block->length = j;
			LinkedList_Append(&edit->text_blocks, block);
			continue;
		}
		for (j = 0; i < len && j < size - 1; ++j, ++i) {
			wchar_t *text;
			block->text[j] = wtext[i];
			/* 检测是否有样式标签 */
			p = ScanStyleTag(wtext + i, NULL, 0, NULL);
			if (!p) {
				p = ScanStyleEndingTag(wtext + i, NULL);
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
		LinkedList_Append(&edit->text_blocks, block);
	}
	edit->tasks[TASK_SET_TEXT] = TRUE;
	Widget_AddTask(widget, LCUI_WTASK_USER);
	return 0;
}

/** 更新文本框内的字体位图 */
static void TextEdit_ProcTextBlock(LCUI_Widget widget, LCUI_TextBlock txtblk)
{
	LinkedList *tags;
	LCUI_TextEdit edit;
	LCUI_TextLayer layer;

	edit = Widget_GetData(widget, self.prototype);
	switch (txtblk->owner) {
	case TEXT_BLOCK_OWNER_SOURCE:
		layer = edit->layer_source;
		tags = &edit->text_tags;
		break;
	case TEXT_BLOCK_OWNER_PLACEHOLDER:
		layer = edit->layer_placeholder;
		tags = NULL;
		break;
	default:
		return;
	}
	switch (txtblk->action) {
	case TEXT_BLOCK_ACTION_APPEND:
		/* 将此文本块追加至文本末尾 */
		TextLayer_AppendTextW(layer, txtblk->text, tags);
		break;
	case TEXT_BLOCK_ACTION_INSERT:
		/* 将此文本块插入至文本插入符所在处 */
		TextLayer_InsertTextW(layer, txtblk->text, tags);
	default:
		break;
	}
	if (edit->password_char && txtblk->owner == TEXT_BLOCK_OWNER_SOURCE) {
		wchar_t *text = NEW(wchar_t, txtblk->length + 1);
		wcsncpy(text, txtblk->text, txtblk->length + 1);
		fillchar(text, edit->password_char);
		layer = edit->layer_mask;
		if (txtblk->action == TEXT_BLOCK_ACTION_INSERT) {
			TextLayer_InsertTextW(layer, text, NULL);
		} else {
			TextLayer_AppendTextW(layer, text, NULL);
		}
		free(text);
	}
}

/** 更新文本框的文本图层 */
static void TextEdit_UpdateTextLayer(LCUI_Widget w)
{
	float scale;
	LinkedList rects;
	LCUI_RectF rect;
	LCUI_TextEdit edit;
	LCUI_TextStyleRec style;
	LinkedListNode *node;

	LinkedList_Init(&rects);
	scale = LCUIMetrics_GetScale();
	edit = Widget_GetData(w, self.prototype);
	TextStyle_Copy(&style, &edit->layer_source->text_default_style);
	if (edit->password_char) {
		TextLayer_SetTextStyle(edit->layer_mask, &style);
	}
	style.has_fore_color = TRUE;
	style.fore_color = PLACEHOLDER_COLOR;
	TextLayer_SetTextStyle(edit->layer_placeholder, &style);
	TextStyle_Destroy(&style);
	TextLayer_Update(edit->layer, &rects);
	for (LinkedList_Each(node, &rects)) {
		LCUIRect_ToRectF(node->data, &rect, 1.0f / scale);
		Widget_InvalidateArea(w, &rect, SV_CONTENT_BOX);
	}
	TextLayer_ClearInvalidRect(edit->layer);
	RectList_Clear(&rects);
}

static void TextEdit_OnTask(LCUI_Widget widget)
{
	size_t i, len;
	wchar_t text[256];
	LCUI_TextEdit edit = Widget_GetData(widget, self.prototype);

	while (edit->tasks[TASK_UPDATE_MASK]) {
		edit->tasks[TASK_UPDATE] = TRUE;
		edit->tasks[TASK_UPDATE_MASK] = FALSE;
		TextLayer_ClearText(edit->layer_mask);
		if (!edit->password_char) {
			edit->layer = edit->layer_source;
			break;
		}
		edit->layer = edit->layer_mask;
		len = TextEdit_GetTextLength(widget);
		for (i = 0; i < len; i += 255) {
			TextEdit_GetTextW(widget, i, 255, text);
			fillchar(text, edit->password_char);
			TextLayer_AppendTextW(edit->layer, text, NULL);
		}
	}
	if (edit->tasks[TASK_SET_TEXT]) {
		LinkedList blocks;
		LinkedListNode *node;
		LCUI_WidgetEventRec ev;

		LinkedList_Init(&blocks);
		LCUIMutex_Lock(&edit->mutex);
		LinkedList_Concat(&blocks, &edit->text_blocks);
		LCUIMutex_Unlock(&edit->mutex);
		for (LinkedList_Each(node, &blocks)) {
			TextEdit_ProcTextBlock(widget, node->data);
		}
		TextBlocks_Clear(&blocks);
		LCUI_InitWidgetEvent(&ev, "change");
		Widget_TriggerEvent(widget, &ev, NULL);
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
			Widget_InvalidateArea(widget, NULL, SV_PADDING_BOX);
		}
		edit->is_placeholder_shown = is_shown;
		edit->tasks[TASK_UPDATE_CARET] = TRUE;
		edit->tasks[TASK_UPDATE] = FALSE;
	}
	if (edit->tasks[TASK_UPDATE_CARET]) {
		edit->tasks[TASK_UPDATE_CARET] = FALSE;
		TextEdit_UpdateCaret(widget);
	}
}

static void TextEdit_AutoSize(LCUI_Widget widget, float *width, float *height)
{
	int i, n, h;
	float scale = LCUIMetrics_GetScale();
	LCUI_TextEdit edit = GetData(widget);

	if (edit->is_multiline_mode) {
		n = max(TextLayer_GetRowTotal(edit->layer), 3);
		for (h = 0, i = 0; i < n; ++i) {
			h += TextLayer_GetRowHeight(edit->layer, i);
		}
	} else {
		h = TextLayer_GetHeight(edit->layer);
	}
	if (*height <= 0) {
		*height = h / scale;
	}
	if (*width <= 0) {
		*width = DEFAULT_WIDTH;
	}
}

void TextEdit_EnableStyleTag(LCUI_Widget widget, LCUI_BOOL enable)
{
	LCUI_TextEdit edit = Widget_GetData(widget, self.prototype);
	TextLayer_EnableStyleTag(edit->layer, enable);
}

/* FIXME: improve multiline editing mode of the textedit widget
 * The multipline editing mode of the textedit widget have not been fully
 * tested, it may have many problems.
 */

void TextEdit_EnableMultiline(LCUI_Widget w, LCUI_BOOL enable)
{
	if (enable) {
		Widget_SetFontStyle(w, key_white_space, SV_AUTO, style);
	} else {
		Widget_SetFontStyle(w, key_white_space, SV_NOWRAP, style);
	}
}

void TextEdit_ClearText(LCUI_Widget widget)
{
	LCUI_TextEdit edit;
	LCUI_TextBlock block;
	LinkedListNode *node, *prev;

	edit = Widget_GetData(widget, self.prototype);
	LCUIMutex_Lock(&edit->mutex);
	for (LinkedList_Each(node, &edit->text_blocks)) {
		block = node->data;
		prev = node->prev;
		if (block->owner == TEXT_BLOCK_OWNER_SOURCE) {
			TextBlock_OnDestroy(block);
			LinkedList_DeleteNode(&edit->text_blocks, node);
			node = prev;
		}
	}
	TextLayer_ClearText(edit->layer_source);
	StyleTags_Clear(&edit->text_tags);
	edit->tasks[TASK_UPDATE] = TRUE;
	Widget_AddTask(widget, LCUI_WTASK_USER);
	LCUIMutex_Unlock(&edit->mutex);
	Widget_InvalidateArea(widget, NULL, SV_PADDING_BOX);
}

size_t TextEdit_GetTextW(LCUI_Widget w, size_t start, size_t max_len,
			 wchar_t *buf)
{
	LCUI_TextEdit edit = GetData(w);
	return TextLayer_GetTextW(edit->layer_source, start, max_len, buf);
}

size_t TextEdit_GetTextLength(LCUI_Widget w)
{
	LCUI_TextEdit edit = GetData(w);
	return edit->layer_source->length;
}

LCUI_Object TextEdit_GetProperty(LCUI_Widget w, const char *name)
{
	size_t len = TextEdit_GetTextLength(w);
	size_t size = (len + 1) * sizeof(wchar_t);
	wchar_t *wcs = malloc(size);
	LCUI_Object prop;

	if (strcmp(name, "value") != 0) {
		return NULL;
	}
	TextEdit_GetTextW(w, 0, len, wcs);
	prop = WString_New(wcs);
	free(wcs);
	return prop;
}

int TextEdit_SetTextW(LCUI_Widget w, const wchar_t *wstr)
{
	TextEdit_ClearText(w);
	return TextEdit_AddTextBlock(w, wstr, TEXT_BLOCK_ACTION_APPEND,
				     TEXT_BLOCK_OWNER_SOURCE);
}

int TextEdit_SetText(LCUI_Widget widget, const char *utf8_str)
{
	int ret;
	size_t len = strlen(utf8_str) + 1;
	wchar_t *wstr = malloc(len * sizeof(wchar_t));

	if (!wstr) {
		return -ENOMEM;
	}
	len = LCUI_DecodeString(wstr, utf8_str, len, ENCODING_UTF8);
	wstr[len] = 0;
	ret = TextEdit_SetTextW(widget, wstr);
	free(wstr);
	return ret;
}

void TextEdit_SetPasswordChar(LCUI_Widget w, wchar_t ch)
{
	LCUI_TextEdit edit = GetData(w);
	edit->password_char = ch;
	edit->tasks[TASK_UPDATE_MASK] = TRUE;
	Widget_AddTask(w, LCUI_WTASK_USER);
}

int TextEdit_AppendTextW(LCUI_Widget w, const wchar_t *wstr)
{
	return TextEdit_AddTextBlock(w, wstr, TEXT_BLOCK_ACTION_APPEND,
				     TEXT_BLOCK_OWNER_SOURCE);
}

int TextEdit_InsertTextW(LCUI_Widget w, const wchar_t *wstr)
{
	return TextEdit_AddTextBlock(w, wstr, TEXT_BLOCK_ACTION_INSERT,
				     TEXT_BLOCK_OWNER_SOURCE);
}

int TextEdit_SetPlaceHolderW(LCUI_Widget w, const wchar_t *wstr)
{
	LCUI_TextEdit edit = GetData(w);
	LCUIMutex_Lock(&edit->mutex);
	TextLayer_ClearText(edit->layer_placeholder);
	LCUIMutex_Unlock(&edit->mutex);
	if (edit->is_placeholder_shown) {
		Widget_InvalidateArea(w, NULL, SV_PADDING_BOX);
	}
	return TextEdit_AddTextBlock(w, wstr, TEXT_BLOCK_ACTION_INSERT,
				     TEXT_BLOCK_OWNER_PLACEHOLDER);
}

int TextEdit_SetPlaceHolder(LCUI_Widget w, const char *str)
{
	int ret;
	size_t len = strlen(str) + 1;
	wchar_t *wstr = malloc(len * sizeof(wchar_t));

	if (!wstr) {
		return -ENOMEM;
	}
	len = LCUI_DecodeString(wstr, str, len, ENCODING_UTF8);
	wstr[len] = 0;
	ret = TextEdit_SetPlaceHolderW(w, wstr);
	free(wstr);
	return ret;
}

void TextEdit_SetCaretBlink(LCUI_Widget w, LCUI_BOOL visible, int time)
{
	LCUI_TextEdit edit = GetData(w);
	TextCaret_SetVisible(edit->caret, visible);
	TextCaret_SetBlinkTime(edit->caret, time);
}

static void TextEdit_OnParseText(LCUI_Widget w, const char *text)
{
	TextEdit_SetText(w, text);
}

static void TextEdit_OnFocus(LCUI_Widget widget, LCUI_WidgetEvent e, void *arg)
{
	LCUI_TextEdit edit;

	edit = Widget_GetData(widget, self.prototype);
	TextCaret_SetVisible(edit->caret, TRUE);
	edit->tasks[TASK_UPDATE_CARET] = TRUE;
	Widget_AddTask(widget, LCUI_WTASK_USER);
}

static void TextEdit_OnBlur(LCUI_Widget widget, LCUI_WidgetEvent e, void *arg)
{
	LCUI_TextEdit edit;

	edit = Widget_GetData(widget, self.prototype);
	TextCaret_SetVisible(edit->caret, FALSE);
	/* In single-line editing mode, we should reset the caret position to
	 * the head, otherwise it will mistakenly think that only the last part
	 * is entered after inputting long text. */
	if (!edit->is_multiline_mode) {
		TextEdit_MoveCaret(widget, 0, 0);
	}
}

static void TextEdit_TextBackspace(LCUI_Widget widget, int n_ch)
{
	LCUI_TextEdit edit;
	LCUI_WidgetEventRec ev;

	edit = Widget_GetData(widget, self.prototype);
	LCUIMutex_Lock(&edit->mutex);
	TextLayer_TextBackspace(edit->layer_source, n_ch);
	if (edit->password_char) {
		TextLayer_TextBackspace(edit->layer_mask, n_ch);
	}
	TextCaret_Refresh(edit->caret);
	edit->tasks[TASK_UPDATE] = TRUE;
	Widget_AddTask(widget, LCUI_WTASK_USER);
	LCUIMutex_Unlock(&edit->mutex);
	LCUI_InitWidgetEvent(&ev, "change");
	Widget_TriggerEvent(widget, &ev, NULL);
}

static void TextEdit_TextDelete(LCUI_Widget widget, int n_ch)
{
	LCUI_TextEdit edit;
	LCUI_WidgetEventRec ev;

	edit = Widget_GetData(widget, self.prototype);
	LCUIMutex_Lock(&edit->mutex);
	TextLayer_TextDelete(edit->layer_source, n_ch);
	if (edit->password_char) {
		TextLayer_TextDelete(edit->layer_mask, n_ch);
	}
	TextCaret_Refresh(edit->caret);
	edit->tasks[TASK_UPDATE] = TRUE;
	Widget_AddTask(widget, LCUI_WTASK_USER);
	LCUIMutex_Unlock(&edit->mutex);
	LCUI_InitWidgetEvent(&ev, "change");
	Widget_TriggerEvent(widget, &ev, NULL);
}

/** 处理按键事件 */
static void TextEdit_OnKeyDown(LCUI_Widget widget, LCUI_WidgetEvent e,
			       void *arg)
{
	int cols, rows;
	int cur_col, cur_row;
	LCUI_TextEdit edit = Widget_GetData(widget, self.prototype);

	cur_row = edit->layer->insert_y;
	cur_col = edit->layer->insert_x;
	rows = TextLayer_GetRowTotal(edit->layer);
	cols = TextLayer_GetRowTextLength(edit->layer, cur_row);
	e->cancel_bubble = TRUE;
	switch (e->key.code) {
	case LCUI_KEY_HOME:
		cur_col = 0;
		break;
	case LCUI_KEY_END:
		cur_col = cols;
		break;
	case LCUI_KEY_LEFT:
		if (cur_col > 0) {
			--cur_col;
		} else if (cur_row > 0) {
			--cur_row;
			cur_col =
			    TextLayer_GetRowTextLength(edit->layer, cur_row);
		}
		break;
	case LCUI_KEY_RIGHT:
		if (cur_col < cols) {
			++cur_col;
		} else if (cur_row < rows - 1) {
			++cur_row;
			cur_col = 0;
		}
		break;
	case LCUI_KEY_UP:
		if (cur_row > 0) {
			--cur_row;
		}
		break;
	case LCUI_KEY_DOWN:
		if (cur_row < rows - 1) {
			++cur_row;
		}
		break;
	case LCUI_KEY_BACKSPACE:
		TextEdit_TextBackspace(widget, 1);
		return;
	case LCUI_KEY_DELETE:
		TextEdit_TextDelete(widget, 1);
		return;
	default:
		break;
	}
	TextEdit_MoveCaret(widget, cur_row, cur_col);
}

/** 处理输入法对文本框输入的内容 */
static void TextEdit_OnTextInput(LCUI_Widget widget, LCUI_WidgetEvent e,
				 void *arg)
{
	uint_t i, j, k;
	wchar_t ch, *text, excludes[8] = L"\b\r\t\x1b";
	LCUI_TextEdit edit = Widget_GetData(widget, self.prototype);

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
	TextEdit_InsertTextW(widget, text);
	free(text);
}

static void TextEdit_OnResize(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	int iw, ih;
	float scale, width = 0, height = 0;
	float max_width = 0, max_height = 0;

	LCUI_RectF rect;
	LinkedList rects;
	LinkedListNode *node;
	LCUI_TextEdit edit = GetData(w);

	if (!w->style->sheet[key_width].is_valid ||
	    w->style->sheet[key_width].type == LCUI_STYPE_AUTO) {
		max_width = Widget_ComputeMaxContentWidth(w);
	} else {
		max_width = width = w->box.content.width;
	}
	if (w->style->sheet[key_height].is_valid &&
	    w->style->sheet[key_height].type != LCUI_STYPE_AUTO) {
		max_height = height = w->box.content.width;
	}
	LinkedList_Init(&rects);
	iw = iround(width);
	ih = iround(height);
	TextLayer_SetFixedSize(edit->layer_mask, iw, ih);
	TextLayer_SetFixedSize(edit->layer_source, iw, ih);
	TextLayer_SetFixedSize(edit->layer_placeholder, iw, ih);
	iw = iround(max_width);
	ih = iround(max_height);
	TextLayer_SetMaxSize(edit->layer_mask, iw, ih);
	TextLayer_SetMaxSize(edit->layer_source, iw, ih);
	TextLayer_SetMaxSize(edit->layer_placeholder, iw, ih);
	TextLayer_Update(edit->layer, &rects);
	scale = LCUIMetrics_GetScale();
	for (LinkedList_Each(node, &rects)) {
		LCUIRect_ToRectF(node->data, &rect, 1.0f / scale);
		Widget_InvalidateArea(w, node->data, SV_CONTENT_BOX);
	}
	RectList_Clear(&rects);
	TextLayer_ClearInvalidRect(edit->layer);
}

static void TextEdit_OnMouseMove(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	int x, y;
	float scale, offset_x, offset_y;
	LCUI_TextEdit edit = GetData(w);

	if (edit->is_placeholder_shown) {
		TextEdit_UpdateCaret(w);
		return;
	}
	scale = LCUIMetrics_GetScale();
	Widget_GetOffset(w, NULL, &offset_x, &offset_y);
	x = iround((e->motion.x - offset_x - w->padding.left) * scale);
	y = iround((e->motion.y - offset_y - w->padding.top) * scale);
	TextLayer_SetCaretPosByPixelPos(edit->layer, x, y);
	TextEdit_UpdateCaret(w);
}

static void TextEdit_OnMouseUp(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	Widget_ReleaseMouseCapture(w);
	Widget_UnbindEvent(w, "mousemove", TextEdit_OnMouseMove);
}

static void TextEdit_OnMouseDown(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	int x, y;
	float offset_x, offset_y;
	float scale = LCUIMetrics_GetScale();
	LCUI_TextEdit edit = GetData(w);

	Widget_GetOffset(w, NULL, &offset_x, &offset_y);
	x = iround((e->motion.x - offset_x - w->padding.left) * scale);
	y = iround((e->motion.y - offset_y - w->padding.top) * scale);
	TextLayer_SetCaretPosByPixelPos(edit->layer, x, y);
	TextEdit_UpdateCaret(w);
	Widget_SetMouseCapture(w);
	Widget_BindEvent(w, "mousemove", TextEdit_OnMouseMove, NULL, NULL);
}

static void TextEdit_OnReady(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	TextEdit_UpdateCaret(w);
}

static void TextEdit_SetAttr(LCUI_Widget w, const char *name, const char *val)
{
	if (strcmp(name, "placeholder") == 0) {
		TextEdit_SetPlaceHolder(w, val);
	}
}

static void TextEdit_OnInit(LCUI_Widget w)
{
	LCUI_TextEdit edit = AddData(w);

	edit->is_read_only = FALSE;
	edit->password_char = 0;
	edit->allow_input_char = NULL;
	edit->is_placeholder_shown = FALSE;
	edit->is_multiline_mode = FALSE;
	edit->layer_mask = TextLayer_New();
	edit->layer_source = TextLayer_New();
	edit->layer_placeholder = TextLayer_New();
	edit->layer = edit->layer_source;
	edit->value_watcher = NULL;
	edit->text_block_size = TEXT_BLOCK_SIZE;
	edit->caret = LCUIWidget_New("textcaret");
	w->computed_style.focusable = TRUE;
	memset(edit->tasks, 0, sizeof(edit->tasks));
	LinkedList_Init(&edit->text_blocks);
	StyleTags_Init(&edit->text_tags);
	TextEdit_EnableMultiline(w, FALSE);
	TextLayer_SetAutoWrap(edit->layer, TRUE);
	TextLayer_SetAutoWrap(edit->layer_mask, TRUE);
	TextLayer_EnableStyleTag(edit->layer, FALSE);
	Widget_BindEvent(w, "textinput", TextEdit_OnTextInput, NULL, NULL);
	Widget_BindEvent(w, "mousedown", TextEdit_OnMouseDown, NULL, NULL);
	Widget_BindEvent(w, "mouseup", TextEdit_OnMouseUp, NULL, NULL);
	Widget_BindEvent(w, "keydown", TextEdit_OnKeyDown, NULL, NULL);
	Widget_BindEvent(w, "resize", TextEdit_OnResize, NULL, NULL);
	Widget_BindEvent(w, "focus", TextEdit_OnFocus, NULL, NULL);
	Widget_BindEvent(w, "blur", TextEdit_OnBlur, NULL, NULL);
	Widget_BindEvent(w, "ready", TextEdit_OnReady, NULL, NULL);
	Widget_Append(w, edit->caret);
	Widget_Hide(edit->caret);
	LCUIMutex_Init(&edit->mutex);
	CSSFontStyle_Init(&edit->style);
}

static void TextEdit_OnDestroy(LCUI_Widget widget)
{
	LCUI_TextEdit edit = GetData(widget);

	edit->layer = NULL;
	TextLayer_Destroy(edit->layer_source);
	TextLayer_Destroy(edit->layer_placeholder);
	TextLayer_Destroy(edit->layer_mask);
	CSSFontStyle_Destroy(&edit->style);
	TextBlocks_Clear(&edit->text_blocks);
	if (edit->value_watcher) {
		ObjectWatcher_Delete(edit->value_watcher);
		edit->value_watcher = NULL;
	}
}

static void TextEdit_OnPaint(LCUI_Widget w, LCUI_PaintContext paint,
			     LCUI_WidgetActualStyle style)
{
	LCUI_Pos pos;
	LCUI_Graph canvas;
	LCUI_Rect content_rect, rect;
	LCUI_TextEdit edit = GetData(w);

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
	TextLayer_RenderTo(edit->layer, rect, pos, &canvas);
}

static void TextEdit_OnUpdateStyle(LCUI_Widget w)
{
	int i;

	LCUI_TextEdit edit = GetData(w);
	LCUI_TextStyleRec text_style;
	LCUI_CSSFontStyleRec style;
	LCUI_TextLayer layers[3] = { edit->layer_mask, edit->layer_placeholder,
				      edit->layer_source };

	CSSFontStyle_Init(&style);
	CSSFontStyle_Compute(&style, w->style);
	if (CSSFontStyle_IsEquals(&style, &edit->style)) {
		CSSFontStyle_Destroy(&style);
		return;
	}
	CSSFontStyle_GetTextStyle(&style, &text_style);
	for (i = 0; i < 3; ++i) {
		TextLayer_SetTextAlign(layers[i], style.text_align);
		TextLayer_SetLineHeight(layers[i], style.line_height);
		TextLayer_SetAutoWrap(layers[i],
				      style.white_space != SV_NOWRAP);
		TextLayer_SetTextStyle(layers[i], &text_style);
	}
	CSSFontStyle_Destroy(&edit->style);
	TextStyle_Destroy(&text_style);
	edit->style = style;
	edit->tasks[TASK_UPDATE] = TRUE;
	Widget_AddTask(w, LCUI_WTASK_USER);
}

static void TextEdit_OnValueChanged(LCUI_Object value, void *arg)
{
	LCUI_Widget w = arg;

	if (value->type == LCUI_StringObject) {
		TextEdit_SetText(w, value->value.string);
	} else if (value->type == LCUI_WStringObject) {
		TextEdit_SetTextW(w, value->value.wstring);
	}
}

void TextEdit_BindProperty(LCUI_Widget w, const char *name, LCUI_Object value)
{
	LCUI_TextEdit edit = Widget_GetData(w, self.prototype);

	if (strcmp(name, "value") == 0) {
		if (edit->value_watcher) {
			ObjectWatcher_Delete(edit->value_watcher);
			edit->value_watcher = NULL;
		}
		if (value) {
			edit->value_watcher =
			    Object_Watch(value, TextEdit_OnValueChanged, w);
			TextEdit_OnValueChanged(value, w);
		} else {
			TextEdit_ClearText(w);
		}
	}
}

void LCUIWidget_AddTextEdit(void)
{
	self.prototype = LCUIWidget_NewPrototype("textedit", NULL);
	self.prototype->init = TextEdit_OnInit;
	self.prototype->paint = TextEdit_OnPaint;
	self.prototype->destroy = TextEdit_OnDestroy;
	self.prototype->settext = TextEdit_OnParseText;
	self.prototype->setattr = TextEdit_SetAttr;
	self.prototype->bindprop = TextEdit_BindProperty;
	self.prototype->autosize = TextEdit_AutoSize;
	self.prototype->runtask = TextEdit_OnTask;
	self.prototype->update = TextEdit_OnUpdateStyle;
	LCUI_LoadCSSString(textedit_css, __FILE__);
}
