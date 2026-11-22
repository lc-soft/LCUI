/*
 * src/widgets/textinput.c: -- textinput widget, used to allow user edit
 *
 * Copyright (c) 2018-2026, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <ptk.h>
#include <pandagl.h>
#include <css.h>
#include <LCUI/widgets/textinput.h>
#include <LCUI/widgets/textcaret.h>
#include "textstyle.h"
#include "textedit.h"

#define DEFAULT_WIDTH 176.0f
#define PLACEHOLDER_COLOR pd_rgb(140, 140, 140)

enum task_type_t { TASK_UPDATE, TASK_TOTAL };

typedef struct ui_textinput {
        ui_text_style_t style;           /**< 字体样式 */
        pd_color_t selection_color;      /**< 选区背景色 */
        pd_color_t selection_text_color; /**< 选区文字颜色 */
        ui_text_edit_t *editor;          /**< 文本编辑状态（canonical text） */
        pd_text_t *layer_source;         /**< 实际文本层 */
        pd_text_t *layer_mask;           /**< 屏蔽后的文本层 */
        pd_text_t *layer_placeholder;    /**< 占位符的文本层 */
        pd_text_t *layer;                /**< 当前使用的文本层 */
        float layer_scale;               /**< 文本层排版时使用的实际缩放比例 */
        ui_widget_t *caret;              /**< 文本插入符 */
        ui_widget_t *selection_style;    /**< 承载选区样式（0x0 隐藏元素） */
        bool is_read_only;               /**< 是否只读 */
        bool is_multiline_mode;          /**< 是否为多行模式 */
        bool placeholder_visible;        /**< 是否已经显示占位符 */
        wchar_t *allow_input_char;       /**< 允许输入的字符 */
        wchar_t password_char;           /**< 屏蔽符的副本 */
        bool change_pending;             /**< 是否有待发送的 change 事件 */
        bool tasks[TASK_TOTAL];          /**< 待处理的任务 */
        thread_mutex_t mutex;            /**< 互斥锁 */
} ui_textinput_t;

static ui_widget_prototype_t *ui_textinput_proto;

static const char *ui_textinput_css = "\
textinput {\
	min-width: 124px;\
	min-height: 14px;\
	cursor: text;\
	background-color: #fff;\
	border: 1px solid #eee;\
	padding: 5px 10px;\
	display: inline-block;\
}\
textinput:focus {\
	border: 1px solid #2196F3;\
	box-shadow: 0 0 6px rgba(33,150,243,0.4);\
}\
textinput:disabled {\
	opacity: 0.45;\
}";

static void ui_textinput_update_caret(ui_widget_t *widget)
{
        ui_textinput_t *edit = ui_widget_get_data(widget, ui_textinput_proto);

        int row = edit->layer->caret.y;
        int offset_x, offset_y;
        float height, width;
        float scale = edit->layer_scale;
        float x, y, caret_x = 0, caret_y = 0;

        /* A transient zero-sized layout (e.g. a column collapsing during a
         * focus/reflow pass) must not scroll the text layer; otherwise the
         * caret update can move the whole text out of view. */
        if (widget->content_box.width <= 0 || widget->content_box.height <= 0) {
                return;
        }

        if (!edit->placeholder_visible) {
                pd_pos_t pos;
                if (pd_text_get_caret_pixel(edit->layer, &pos) != 0) {
                        return;
                }
                caret_x = pos.x / scale;
                caret_y = pos.y / scale;
        }
        offset_x = (int)round(edit->layer->offset_x / scale);
        offset_y = (int)round(edit->layer->offset_y / scale);
        x = caret_x + offset_x;
        y = caret_y + offset_y;
        width = edit->layer->width / scale;
        height = pd_text_get_line_height(edit->layer, row) / scale;
        ui_widget_set_style_unit_value(edit->caret, css_prop_height, height,
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
        offset_x = (int)round((x - caret_x) * scale);
        offset_y = (int)round((y - caret_y) * scale);
        if (pd_text_set_offset(edit->layer, offset_x, offset_y)) {
                edit->tasks[TASK_UPDATE] = true;
                ui_widget_request_update(widget);
        }
        x += widget->computed_style.padding_left;
        y += widget->computed_style.padding_top;
        ui_widget_move(edit->caret, x, y);
        ui_textcaret_refresh(edit->caret);
        if (edit->password_char) {
                pd_text_set_caret(edit->layer_mask,
                                  ui_text_edit_get_caret_row(edit->editor),
                                  ui_text_edit_get_caret_col(edit->editor));
        }
}

static void ui_textinput_update_caret_by_pointer(ui_widget_t *w, ui_event_t *e,
                                                 bool drag)
{
        float scale, offset_x, offset_y, x, y;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        /* 鼠标坐标是逻辑像素，文本层按 layer_scale 排版为实际像素 */
        scale = edit->layer_scale;
        ui_widget_get_offset(w, NULL, &offset_x, &offset_y);
        x = (e->mouse.x - offset_x - w->computed_style.padding_left) * scale;
        y = (e->mouse.y - offset_y - w->computed_style.padding_top) * scale;
        if (edit->password_char) {
                /* 密码显示层是统一宽度的 mask，命中检测必须与显示同源 */
                pd_text_set_caret_pixel(edit->layer_mask, (int)x, (int)y);
                ui_text_edit_move_caret_extend(edit->editor,
                                               edit->layer_mask->caret.y,
                                               edit->layer_mask->caret.x, drag);
        } else if (drag) {
                ui_text_edit_drag(edit->editor,
                                  x - edit->layer_source->offset_x,
                                  y - edit->layer_source->offset_y);
        } else {
                ui_text_edit_click(edit->editor,
                                   x - edit->layer_source->offset_x,
                                   y - edit->layer_source->offset_y);
        }
        if (edit->password_char) {
                pd_text_set_caret(edit->layer_mask,
                                  ui_text_edit_get_caret_row(edit->editor),
                                  ui_text_edit_get_caret_col(edit->editor));
        }
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_CONTENT_BOX);
        ui_textinput_update_caret(w);
}

void ui_textinput_move_caret(ui_widget_t *widget, int row, int col)
{
        ui_textinput_t *edit = ui_widget_get_data(widget, ui_textinput_proto);

        if (edit->placeholder_visible) {
                row = col = 0;
        }
        ui_text_edit_move_caret(edit->editor, row, col);
        if (edit->password_char) {
                pd_text_set_caret(edit->layer_mask,
                                  ui_text_edit_get_caret_row(edit->editor),
                                  ui_text_edit_get_caret_col(edit->editor));
        }
        ui_textinput_update_caret(widget);
        ui_widget_mark_dirty_rect(widget, NULL, UI_BOX_TYPE_CONTENT_BOX);
}

static void ui_textinput_on_resize(ui_widget_t *w, float width, float height)
{
        int i;
        float scale = ui_get_actual_scale();
        list_t rects;
        list_node_t *node;
        ui_rect_t rect;
        pd_text_t *layers[3];
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        layers[0] = edit->layer_source;
        layers[1] = edit->layer_mask;
        layers[2] = edit->layer_placeholder;
        edit->layer_scale = scale;
        for (i = 0; i < 3; ++i) {
                list_create(&rects);
                pd_text_set_fixed_size(layers[i], (int)(width * scale),
                                       (int)(height * scale));
                pd_text_set_max_size(layers[i], (int)(width * scale),
                                     (int)(height * scale));
                pd_text_update(layers[i], &rects);
                for (list_each(node, &rects)) {
                        ui_rect_from_pd_rect(&rect, node->data, scale);
                        ui_widget_mark_dirty_rect(w, &rect,
                                                  UI_BOX_TYPE_CONTENT_BOX);
                }
                pd_rects_clear(&rects);
        }
}

static void ui_textinput_on_autosize(ui_widget_t *w, ui_sizehint_t *hint)
{
        int i;
        int max_width = 0, max_height = 0;
        float scale = ui_get_actual_scale();
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        max_width = ui_compute(DEFAULT_WIDTH);
        if (edit->is_multiline_mode) {
                for (max_height = 0, i = 0; i < 6; ++i) {
                        max_height +=
                            pd_text_get_line_height(edit->layer_source, i);
                }
        } else {
                max_height = pd_text_get_height(edit->layer_source);
        }
        hint->max_height = max_height / scale;
        hint->max_width = max_width / scale;
        hint->min_width = 0;
        hint->min_height = 0;
}

void ui_textinput_enable_style_tag(ui_widget_t *widget, bool enable)
{
        ui_textinput_t *edit = ui_widget_get_data(widget, ui_textinput_proto);

        /* 编辑模式使用 canonical 字符索引；样式标签会被 pd_text 消费，
         * 造成索引错位，因此仅允许在只读展示时启用。 */
        if (enable && !edit->is_read_only) {
                return;
        }
        pd_text_set_style_tag(edit->layer_source, enable);
}

/* FIXME: improve multiline editing mode of the textinput widget
 * The multipline editing mode of the textinput widget have not been fully
 * tested, it may have many problems.
 */

void ui_textinput_enable_multiline(ui_widget_t *w, bool enable)
{
        int i;
        pd_text_t *layers[3];
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        edit->is_multiline_mode = enable;
        layers[0] = edit->layer_source;
        layers[1] = edit->layer_mask;
        layers[2] = edit->layer_placeholder;
        for (i = 0; i < 3; ++i) {
                pd_text_set_multiline(layers[i], enable);
        }
        if (enable) {
                ui_widget_set_style_keyword_value(w, css_prop_white_space,
                                                  CSS_KEYWORD_AUTO);
        } else {
                ui_widget_set_style_keyword_value(w, css_prop_white_space,
                                                  CSS_KEYWORD_NOWRAP);
        }
}

static void ui_textinput_request_change(ui_widget_t *w)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        edit->change_pending = true;
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
}

void ui_textinput_set_read_only(ui_widget_t *w, bool read_only)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        edit->is_read_only = read_only;
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
}

void ui_textinput_set_selection_color(ui_widget_t *w, pd_color_t color)
{
        char value[64];
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        edit->selection_color = color;
        snprintf(value, sizeof(value), "rgba(%u,%u,%u,%.3f)", color.r, color.g,
                 color.b, color.a / 255.0f);
        ui_widget_set_style_string(edit->selection_style, "background-color",
                                   value);
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_CONTENT_BOX);
}

void ui_textinput_set_selection_text_color(ui_widget_t *w, pd_color_t color)
{
        char value[64];
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        edit->selection_text_color = color;
        snprintf(value, sizeof(value), "rgba(%u,%u,%u,%.3f)", color.r, color.g,
                 color.b, color.a / 255.0f);
        ui_widget_set_style_string(edit->selection_style, "color", value);
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_CONTENT_BOX);
}

static wchar_t *ui_textinput_filter_newlines(const wchar_t *text, size_t *len)
{
        size_t i, j, n = wcslen(text);
        wchar_t *out = malloc(sizeof(wchar_t) * (n + 1));

        if (!out) {
                *len = 0;
                return NULL;
        }
        for (i = 0, j = 0; i < n; ++i) {
                if (text[i] != L'\r' && text[i] != L'\n') {
                        out[j++] = text[i];
                }
        }
        out[j] = 0;
        *len = j;
        return out;
}

void ui_textinput_clear_text(ui_widget_t *widget)
{
        ui_textinput_t *edit;

        edit = ui_widget_get_data(widget, ui_textinput_proto);
        thread_mutex_lock(&edit->mutex);
        ui_text_edit_set_text(edit->editor, L"", 0);
        thread_mutex_unlock(&edit->mutex);
        ui_textinput_request_change(widget);
        ui_widget_mark_dirty_rect(widget, NULL, UI_BOX_TYPE_PADDING_BOX);
}

size_t ui_textinput_get_text_w(ui_widget_t *w, size_t start, size_t max_len,
                               wchar_t *buf)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);
        return ui_text_edit_get_text(edit->editor, start, max_len, buf);
}

size_t ui_textinput_get_text_length(ui_widget_t *w)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);
        return ui_text_edit_get_length(edit->editor);
}

int ui_textinput_set_text_w(ui_widget_t *w, const wchar_t *wstr)
{
        size_t len;
        wchar_t *filtered = NULL;
        const wchar_t *value = wstr;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (!wstr) {
                return -1;
        }
        len = wcslen(wstr);
        if (!edit->is_multiline_mode) {
                filtered = ui_textinput_filter_newlines(wstr, &len);
                if (!filtered) {
                        return -ENOMEM;
                }
                value = filtered;
        }
        thread_mutex_lock(&edit->mutex);
        ui_text_edit_set_text(edit->editor, value, len);
        thread_mutex_unlock(&edit->mutex);
        free(filtered);
        ui_textinput_request_change(w);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_PADDING_BOX);
        return 0;
}

int ui_textinput_set_text(ui_widget_t *widget, const char *utf8_str)
{
        int ret;
        size_t len = strlen(utf8_str) + 1;
        wchar_t *wstr = malloc(len * sizeof(wchar_t));

        if (!wstr) {
                return -ENOMEM;
        }
        len = decode_utf8(wstr, utf8_str, len);
        ret = ui_textinput_set_text_w(widget, wstr);
        free(wstr);
        return ret;
}

void ui_textinput_set_password_char(ui_widget_t *w, wchar_t ch)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        edit->password_char = ch;
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
}

int ui_textinput_append_text_w(ui_widget_t *w, const wchar_t *wstr)
{
        size_t len;
        wchar_t *filtered = NULL;
        const wchar_t *value = wstr;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (!wstr) {
                return -1;
        }
        len = wcslen(wstr);
        if (!edit->is_multiline_mode) {
                filtered = ui_textinput_filter_newlines(wstr, &len);
                if (!filtered) {
                        return -ENOMEM;
                }
                value = filtered;
        }
        thread_mutex_lock(&edit->mutex);
        ui_text_edit_append_text(edit->editor, value, len);
        thread_mutex_unlock(&edit->mutex);
        free(filtered);
        ui_textinput_request_change(w);
        return 0;
}

int ui_textinput_insert_text_w(ui_widget_t *w, const wchar_t *wstr)
{
        size_t len;
        wchar_t *filtered = NULL;
        const wchar_t *value = wstr;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (!wstr) {
                return -1;
        }
        len = wcslen(wstr);
        if (!edit->is_multiline_mode) {
                filtered = ui_textinput_filter_newlines(wstr, &len);
                if (!filtered) {
                        return -ENOMEM;
                }
                value = filtered;
        }
        thread_mutex_lock(&edit->mutex);
        ui_text_edit_text_input(edit->editor, value, len);
        thread_mutex_unlock(&edit->mutex);
        free(filtered);
        ui_textinput_request_change(w);
        return 0;
}

int ui_textinput_set_placeholder_w(ui_widget_t *w, const wchar_t *wstr)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (!wstr) {
                return -1;
        }
        thread_mutex_lock(&edit->mutex);
        pd_text_clear(edit->layer_placeholder);
        pd_text_append(edit->layer_placeholder, wstr, NULL);
        thread_mutex_unlock(&edit->mutex);
        if (edit->placeholder_visible) {
                ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_PADDING_BOX);
        }
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(w);
        return 0;
}

int ui_textinput_set_placeholder(ui_widget_t *w, const char *str)
{
        int ret;
        size_t len = strlen(str) + 1;
        wchar_t *wstr = malloc(len * sizeof(wchar_t));

        if (!wstr) {
                return -ENOMEM;
        }
        len = decode_utf8(wstr, str, len);
        ret = ui_textinput_set_placeholder_w(w, wstr);
        free(wstr);
        return ret;
}

void ui_textinput_set_caret_blink(ui_widget_t *w, bool visible, int time)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);
        ui_textcaret_set_visible(edit->caret, visible);
        ui_textcaret_set_blink_time(edit->caret, time);
}

static void ui_textinput_on_parse_text(ui_widget_t *w, const char *text)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        ui_textinput_set_text(w, text);
        /* Initial parsed content is not a user edit. */
        edit->change_pending = false;
}

static void ui_textinput_on_focus(ui_widget_t *widget, ui_event_t *e, void *arg)
{
        ui_textinput_t *edit;

        edit = ui_widget_get_data(widget, ui_textinput_proto);
        ui_textcaret_set_visible(edit->caret, true);
        ui_textinput_update_caret(widget);
        ui_widget_mark_dirty_rect(widget, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

static void ui_textinput_on_blur(ui_widget_t *widget, ui_event_t *e, void *arg)
{
        ui_textinput_t *edit;

        edit = ui_widget_get_data(widget, ui_textinput_proto);
        ui_textcaret_set_visible(edit->caret, false);
        ui_text_edit_clear_selection(edit->editor);
        edit->tasks[TASK_UPDATE] = true;
        ui_widget_request_update(widget);
        ui_widget_mark_dirty_rect(widget, NULL, UI_BOX_TYPE_GRAPH_BOX);
        /* In single-line editing mode, we should reset the caret position to
         * the head, otherwise it will mistakenly think that only the last part
         * is entered after inputting long text. */
        if (!edit->is_multiline_mode) {
                ui_textinput_move_caret(widget, 0, 0);
        }
}

static void ui_textinput_on_paste(ui_widget_t *w, ui_event_t *e, void *arg)
{
        size_t i, j;
        wchar_t *text;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);
        ptk_clipboard_t *clipboard = arg;

        if (!clipboard || !clipboard->text || edit->is_read_only) {
                return;
        }
        if (edit->is_multiline_mode) {
                ui_textinput_insert_text_w(w, clipboard->text);
                return;
        }
        /* 单行模式需要丢弃粘贴文本中的换行符 */
        text = malloc(sizeof(wchar_t) * (wcslen(clipboard->text) + 1));
        if (!text) {
                return;
        }
        for (i = 0, j = 0; clipboard->text[i]; ++i) {
                if (clipboard->text[i] != L'\r' &&
                    clipboard->text[i] != L'\n') {
                        text[j++] = clipboard->text[i];
                }
        }
        text[j] = 0;
        ui_textinput_insert_text_w(w, text);
        free(text);
}

static void ui_textinput_on_clipboard_ready(void *arg, ui_widget_t *widget)
{
        ptk_clipboard_t *clipboard = arg;
        ui_event_t e = { 0 };

        ui_event_init(&e, "paste");
        ui_widget_emit_event(widget, e, clipboard);
}

static int ui_textinput_map_edit_key(ui_event_t *e)
{
        int key;

        switch (e->key.code) {
        case KEY_LEFT:
                key = e->key.ctrl_key ? UI_TEXT_EDIT_KEY_WORDLEFT
                                      : UI_TEXT_EDIT_KEY_LEFT;
                break;
        case KEY_RIGHT:
                key = e->key.ctrl_key ? UI_TEXT_EDIT_KEY_WORDRIGHT
                                      : UI_TEXT_EDIT_KEY_RIGHT;
                break;
        case KEY_UP:
                key = UI_TEXT_EDIT_KEY_UP;
                break;
        case KEY_DOWN:
                key = UI_TEXT_EDIT_KEY_DOWN;
                break;
        case KEY_HOME:
                key = e->key.ctrl_key ? UI_TEXT_EDIT_KEY_TEXTSTART
                                      : UI_TEXT_EDIT_KEY_LINESTART;
                break;
        case KEY_END:
                key = e->key.ctrl_key ? UI_TEXT_EDIT_KEY_TEXTEND
                                      : UI_TEXT_EDIT_KEY_LINEEND;
                break;
        case KEY_PAGEUP:
                key = UI_TEXT_EDIT_KEY_PGUP;
                break;
        case KEY_PAGEDOWN:
                key = UI_TEXT_EDIT_KEY_PGDOWN;
                break;
        case KEY_BACKSPACE:
                key = UI_TEXT_EDIT_KEY_BACKSPACE;
                break;
        case KEY_DELETE:
                key = UI_TEXT_EDIT_KEY_DELETE;
                break;
        default:
                return 0;
        }
        if (e->key.shift_key) {
                key |= UI_TEXT_EDIT_KEY_SHIFT;
        }
        return key;
}

/** 处理按键事件 */
static void ui_textinput_on_keydown(ui_widget_t *widget, ui_event_t *e,
                                    void *arg)
{
        int key;
        ui_textinput_t *edit = ui_widget_get_data(widget, ui_textinput_proto);

        e->cancel_bubble = true;
        key = ui_textinput_map_edit_key(e);
        if (key) {
                int base = key & ~UI_TEXT_EDIT_KEY_SHIFT;
                size_t length_before, length_after;

                if (base == UI_TEXT_EDIT_KEY_UP ||
                    base == UI_TEXT_EDIT_KEY_DOWN) {
                        thread_mutex_lock(&edit->mutex);
                        ui_text_edit_move_vertical(
                            edit->editor, base == UI_TEXT_EDIT_KEY_UP ? -1 : 1,
                            (key & UI_TEXT_EDIT_KEY_SHIFT) != 0);
                        thread_mutex_unlock(&edit->mutex);
                        edit->tasks[TASK_UPDATE] = true;
                        ui_widget_request_update(widget);
                        ui_widget_mark_dirty_rect(widget, NULL,
                                                  UI_BOX_TYPE_CONTENT_BOX);
                        return;
                }
                if (edit->is_read_only && (base == UI_TEXT_EDIT_KEY_BACKSPACE ||
                                           base == UI_TEXT_EDIT_KEY_DELETE ||
                                           base == UI_TEXT_EDIT_KEY_UNDO ||
                                           base == UI_TEXT_EDIT_KEY_REDO)) {
                        return;
                }
                thread_mutex_lock(&edit->mutex);
                length_before = ui_text_edit_get_length(edit->editor);
                ui_text_edit_key(edit->editor, key);
                length_after = ui_text_edit_get_length(edit->editor);
                thread_mutex_unlock(&edit->mutex);
                edit->tasks[TASK_UPDATE] = true;
                ui_widget_request_update(widget);
                ui_widget_mark_dirty_rect(widget, NULL,
                                          UI_BOX_TYPE_CONTENT_BOX);
                if (((base == UI_TEXT_EDIT_KEY_BACKSPACE) ||
                     (base == UI_TEXT_EDIT_KEY_DELETE)) &&
                    length_after != length_before) {
                        ui_textinput_request_change(widget);
                }
                return;
        }
        if (e->key.code == KEY_ENTER) {
                if (edit->is_multiline_mode && !edit->is_read_only) {
                        ui_textinput_insert_text_w(widget, L"\n");
                }
                return;
        }

        int key_code = e->key.code;
        if (edit->is_read_only && e->key.ctrl_key &&
            (key_code == KEY_V || key_code == KEY_X || key_code == KEY_Z ||
             key_code == KEY_Y)) {
                return;
        }
        // CTRL+V
        if (key_code == KEY_V && e->key.ctrl_key) {
                ptk_clipboard_request_text(
                    (ptk_clipboard_callback_t)ui_textinput_on_clipboard_ready,
                    widget);
                return;
        }
        // CTRL+A
        if (key_code == KEY_A && e->key.ctrl_key) {
                thread_mutex_lock(&edit->mutex);
                ui_text_edit_select_all(edit->editor);
                thread_mutex_unlock(&edit->mutex);
                edit->tasks[TASK_UPDATE] = true;
                ui_widget_request_update(widget);
                ui_widget_mark_dirty_rect(widget, NULL,
                                          UI_BOX_TYPE_CONTENT_BOX);
                return;
        }
        // CTRL+Z / CTRL+Y (CTRL+SHIFT+Z redoes)
        if (key_code == KEY_Z && e->key.ctrl_key) {
                thread_mutex_lock(&edit->mutex);
                ui_text_edit_key(edit->editor, e->key.shift_key
                                                   ? UI_TEXT_EDIT_KEY_REDO
                                                   : UI_TEXT_EDIT_KEY_UNDO);
                thread_mutex_unlock(&edit->mutex);
                edit->tasks[TASK_UPDATE] = true;
                ui_widget_request_update(widget);
                ui_widget_mark_dirty_rect(widget, NULL,
                                          UI_BOX_TYPE_CONTENT_BOX);
                ui_textinput_request_change(widget);
                return;
        }
        if (key_code == KEY_Y && e->key.ctrl_key) {
                thread_mutex_lock(&edit->mutex);
                ui_text_edit_key(edit->editor, UI_TEXT_EDIT_KEY_REDO);
                thread_mutex_unlock(&edit->mutex);
                edit->tasks[TASK_UPDATE] = true;
                ui_widget_request_update(widget);
                ui_widget_mark_dirty_rect(widget, NULL,
                                          UI_BOX_TYPE_CONTENT_BOX);
                ui_textinput_request_change(widget);
                return;
        }
        // CTRL+C / CTRL+X
        if ((key_code == KEY_C || key_code == KEY_X) && e->key.ctrl_key) {
                size_t n, len;
                bool changed = false;
                wchar_t *wcs;

                if (edit->password_char) {
                        return;
                }
                thread_mutex_lock(&edit->mutex);
                len = ui_text_edit_get_length(edit->editor);
                wcs = malloc((len + 1) * sizeof(wchar_t));
                if (!wcs) {
                        thread_mutex_unlock(&edit->mutex);
                        return;
                }
                if (key_code == KEY_X) {
                        if (!ui_text_edit_has_selection(edit->editor)) {
                                free(wcs);
                                thread_mutex_unlock(&edit->mutex);
                                return;
                        }
                        n = ui_text_edit_cut(edit->editor, wcs, len);
                        changed = true;
                } else if (ui_text_edit_has_selection(edit->editor)) {
                        n = ui_text_edit_copy(edit->editor, wcs, len);
                } else {
                        n = ui_text_edit_get_text(edit->editor, 0, len, wcs);
                }
                thread_mutex_unlock(&edit->mutex);
                if (n > 0) {
                        ptk_clipboard_set_text(wcs, n);
                }
                free(wcs);
                if (changed) {
                        ui_textinput_request_change(widget);
                }
                return;
        }
}
/** 处理输入法对文本框输入的内容 */
static void ui_textinput_on_textinput(ui_widget_t *widget, ui_event_t *e,
                                      void *arg)
{
        unsigned i, j, k;
        wchar_t ch, *text, excludes[8] = L"\b\r\t\x1b";
        ui_textinput_t *edit = ui_widget_get_data(widget, ui_textinput_proto);

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
                for (k = 0; edit->allow_input_char[k]; ++k) {
                        if (edit->allow_input_char[k] == ch) {
                                break;
                        }
                }
                if (edit->allow_input_char[k]) {
                        text[j++] = ch;
                }
        }
        text[j] = 0;
        ui_textinput_insert_text_w(widget, text);
        free(text);
}

static void ui_textinput_on_mousemove(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (edit->placeholder_visible) {
                ui_textinput_update_caret(w);
                return;
        }
        ui_textinput_update_caret_by_pointer(w, e, true);
}

static void ui_textinput_on_mouseup(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_release_mouse_capture(w);
        ui_widget_off(w, "mousemove", ui_textinput_on_mousemove, w);
}

static void ui_textinput_on_mousedown(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_textinput_update_caret_by_pointer(w, e, false);
        ui_widget_set_mouse_capture(w);
        ui_widget_on(w, "mousemove", ui_textinput_on_mousemove, w);
}

static void ui_textinput_on_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_textinput_update_caret(w);
}

static bool ui_textinput_attr_to_bool(const char *value)
{
        return value &&
               (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
}

static void ui_textinput_on_set_attr(ui_widget_t *w, const char *name,
                                     const char *val)
{
        if (strcmp(name, "placeholder") == 0) {
                ui_textinput_set_placeholder(w, val);
        } else if (strcmp(name, "type") == 0) {
                if (val && strcmp(val, "password") == 0) {
                        ui_textinput_set_password_char(w, L'*');
                } else {
                        ui_textinput_set_password_char(w, 0);
                }
        } else if (strcmp(name, "multiline") == 0) {
                ui_textinput_enable_multiline(w,
                                              ui_textinput_attr_to_bool(val));
        } else if (strcmp(name, "readonly") == 0) {
                ui_textinput_set_read_only(w, ui_textinput_attr_to_bool(val));
        } else if (strcmp(name, "selection-color") == 0 && val) {
                css_style_value_t parsed;

                if (css_parse_color_value(&parsed, val) &&
                    parsed.type == CSS_COLOR_VALUE) {
                        css_color_value_t cv = parsed.color_value;
                        ui_textinput_set_selection_color(
                            w, pd_argb((uint8_t)(cv >> 24), (uint8_t)(cv >> 16),
                                       (uint8_t)(cv >> 8), (uint8_t)cv));
                }
        } else if (strcmp(name, "selection-text-color") == 0 && val) {
                css_style_value_t parsed;

                if (css_parse_color_value(&parsed, val) &&
                    parsed.type == CSS_COLOR_VALUE) {
                        css_color_value_t cv = parsed.color_value;
                        ui_textinput_set_selection_text_color(
                            w, pd_argb((uint8_t)(cv >> 24), (uint8_t)(cv >> 16),
                                       (uint8_t)(cv >> 8), (uint8_t)cv));
                }
        }
}

static void ui_textinput_on_init(ui_widget_t *w)
{
        ui_textinput_t *edit;

        edit =
            ui_widget_add_data(w, ui_textinput_proto, sizeof(ui_textinput_t));
        edit->is_read_only = false;
        edit->password_char = 0;
        edit->allow_input_char = NULL;
        edit->placeholder_visible = false;
        edit->is_multiline_mode = false;
        edit->change_pending = false;
        edit->selection_color = pd_rgb(23, 23, 23);
        edit->selection_text_color = pd_rgb(255, 255, 255);
        edit->layer_mask = pd_text_create();
        edit->layer_source = pd_text_create();
        edit->layer_placeholder = pd_text_create();
        edit->layer = edit->layer_source;
        edit->layer_scale = ui_get_actual_scale();
        edit->editor = ui_text_edit_create();
        ui_text_edit_set_layer(edit->editor, edit->layer_source);
        edit->caret = ui_create_widget("textcaret");
        edit->selection_style = ui_create_widget(NULL);
        ui_widget_add_class(edit->selection_style, "selection");
        ui_widget_set_style_string(edit->selection_style, "position",
                                   "absolute");
        ui_widget_set_style_string(edit->selection_style, "left", "0px");
        ui_widget_set_style_string(edit->selection_style, "top", "0px");
        ui_widget_set_style_string(edit->selection_style, "width", "0px");
        ui_widget_set_style_string(edit->selection_style, "height", "0px");
        ui_widget_set_style_string(edit->selection_style, "pointer-events",
                                   "none");
        w->tab_index = 0;
        memset(edit->tasks, 0, sizeof(edit->tasks));
        ui_textinput_enable_multiline(w, false);
        pd_text_set_autowrap(edit->layer, true);
        pd_text_set_autowrap(edit->layer_mask, true);
        pd_text_set_style_tag(edit->layer, false);
        ui_widget_on(w, "textinput", ui_textinput_on_textinput, NULL);
        ui_widget_on(w, "mousedown", ui_textinput_on_mousedown, NULL);
        ui_widget_on(w, "mouseup", ui_textinput_on_mouseup, NULL);
        ui_widget_on(w, "keydown", ui_textinput_on_keydown, NULL);
        ui_widget_on(w, "focus", ui_textinput_on_focus, NULL);
        ui_widget_on(w, "blur", ui_textinput_on_blur, NULL);
        ui_widget_on(w, "ready", ui_textinput_on_ready, NULL);
        ui_widget_on(w, "paste", ui_textinput_on_paste, NULL);
        ui_widget_append(w, edit->caret);
        ui_widget_append(w, edit->selection_style);
        ui_widget_hide(edit->caret);
        thread_mutex_init(&edit->mutex);
        ui_text_style_init(&edit->style);
}

static void ui_textinput_on_destroy(ui_widget_t *widget)
{
        ui_textinput_t *edit = ui_widget_get_data(widget, ui_textinput_proto);

        edit->layer = NULL;
        ui_text_edit_destroy(edit->editor);
        pd_text_destroy(edit->layer_source);
        pd_text_destroy(edit->layer_placeholder);
        pd_text_destroy(edit->layer_mask);
        ui_text_style_destroy(&edit->style);
        thread_mutex_destroy(&edit->mutex);
}

static pd_color_t ui_textinput_get_selection_color(ui_textinput_t *edit)
{
        css_color_value_t c =
            edit->selection_style->computed_style.background_color;

        if ((c >> 24) & 0xff) {
                return pd_argb((uint8_t)(c >> 24), (uint8_t)(c >> 16),
                               (uint8_t)(c >> 8), (uint8_t)c);
        }
        return edit->selection_color;
}

static void ui_textinput_paint_selection(ui_widget_t *w, pd_canvas_t *canvas,
                                         pd_pos_t layer_pos, pd_rect_t area)
{
        int i, line_start, line_len, col_start, col_end;
        size_t sel_start, sel_end;
        pd_rect_t r, clipped;
        pd_color_t color;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (!ui_text_edit_has_selection(edit->editor)) {
                return;
        }
        color = ui_textinput_get_selection_color(edit);
        ui_text_edit_selection(edit->editor, &sel_start, &sel_end);
        if (sel_start == sel_end) {
                return;
        }
        for (i = 0; i < pd_text_get_lines_length(edit->layer); ++i) {
                line_start = pd_text_get_line_char_start(edit->layer, i);
                line_len = pd_text_get_line_length(edit->layer, i);
                if ((int)sel_end <= line_start) {
                        break;
                }
                if ((int)sel_start > line_start + line_len) {
                        continue;
                }
                col_start = (int)sel_start > line_start
                                ? (int)sel_start - line_start
                                : 0;
                col_end = (int)sel_end < line_start + line_len
                              ? (int)sel_end - line_start
                              : line_len;
                if (col_start >= col_end) {
                        continue;
                }
                if (pd_text_get_line_rect(edit->layer, i, col_start,
                                          col_end - 1, &r) != 0) {
                        continue;
                }
                /* 与 pd_text_render_to 使用相同的 canvas / layer_pos / area，
                 * 避免选区随 widget 的 paint 偏移而错位 */
                if (!pd_rect_overlap(&r, &area, &clipped)) {
                        continue;
                }
                clipped.x += layer_pos.x;
                clipped.y += layer_pos.y;
                pd_canvas_fill_rect(canvas, color, clipped);
        }
}

static pd_color_t ui_textinput_get_selection_text_color(ui_widget_t *w,
                                                        ui_textinput_t *edit)
{
        css_color_value_t c = edit->selection_style->computed_style.color;

        if (c == w->computed_style.color) {
                return edit->selection_text_color;
        }
        return pd_argb((uint8_t)(c >> 24), (uint8_t)(c >> 16),
                       (uint8_t)(c >> 8), (uint8_t)c);
}

static void ui_textinput_paint_selected_text(ui_widget_t *w,
                                             pd_canvas_t *canvas,
                                             pd_pos_t layer_pos, pd_rect_t area)
{
        int i, line_start, line_len, col_start, col_end;
        size_t sel_start, sel_end;
        pd_rect_t r, clipped, clip_area;
        pd_pos_t sel_pos;
        pd_canvas_t sel_canvas;
        pd_color_t old_color, text_color;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        if (!ui_text_edit_has_selection(edit->editor)) {
                return;
        }
        ui_text_edit_selection(edit->editor, &sel_start, &sel_end);
        text_color = ui_textinput_get_selection_text_color(w, edit);
        if (sel_start == sel_end || text_color.a == 0) {
                return;
        }
        for (i = 0; i < pd_text_get_lines_length(edit->layer); ++i) {
                line_start = pd_text_get_line_char_start(edit->layer, i);
                line_len = pd_text_get_line_length(edit->layer, i);
                if ((int)sel_end <= line_start) {
                        break;
                }
                if ((int)sel_start > line_start + line_len) {
                        continue;
                }
                col_start = (int)sel_start > line_start
                                ? (int)sel_start - line_start
                                : 0;
                col_end = (int)sel_end < line_start + line_len
                              ? (int)sel_end - line_start
                              : line_len;
                if (col_start >= col_end) {
                        continue;
                }
                if (pd_text_get_line_rect(edit->layer, i, col_start,
                                          col_end - 1, &r) != 0) {
                        continue;
                }
                if (!pd_rect_overlap(&r, &area, &clipped)) {
                        continue;
                }
                clip_area = clipped;
                clip_area.x += layer_pos.x;
                clip_area.y += layer_pos.y;
                pd_canvas_init(&sel_canvas);
                if (pd_canvas_quote(&sel_canvas, canvas, &clip_area) != 0) {
                        continue;
                }
                sel_pos.x = layer_pos.x - clip_area.x;
                sel_pos.y = layer_pos.y - clip_area.y;
                old_color = edit->layer->default_style.fore_color;
                edit->layer->default_style.fore_color = text_color;
                pd_text_render_to(edit->layer, r, sel_pos, &sel_canvas);
                edit->layer->default_style.fore_color = old_color;
        }
}

static void ui_textinput_on_paint(ui_widget_t *w, pd_context_t *paint,
                                  ui_widget_actual_style_t *style)
{
        pd_pos_t pos;
        pd_canvas_t canvas;
        pd_rect_t content_rect, rect;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

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
        ui_textinput_paint_selection(w, &canvas, pos, rect);
        ui_textinput_paint_selected_text(w, &canvas, pos, rect);
}

static void ui_textinput_update_mask(ui_widget_t *w)
{
        size_t i, len;
        wchar_t *text;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        pd_text_clear(edit->layer_mask);
        if (!edit->password_char) {
                return;
        }
        len = ui_text_edit_get_length(edit->editor);
        text = malloc(sizeof(wchar_t) * (len + 1));
        if (!text) {
                return;
        }
        ui_text_edit_get_text(edit->editor, 0, len, text);
        for (i = 0; i < len; ++i) {
                if (text[i] != L'\n') {
                        text[i] = edit->password_char;
                }
        }
        pd_text_append(edit->layer_mask, text, NULL);
        free(text);
        pd_text_set_caret(edit->layer_mask, edit->layer_source->caret.y,
                          edit->layer_source->caret.x);
        pd_text_set_offset(edit->layer_mask, edit->layer_source->offset_x,
                           edit->layer_source->offset_y);
}

/** 更新文本框的文本图层 */
static void textinput_update_textlayer(ui_widget_t *w)
{
        float scale;
        list_t rects;
        ui_rect_t rect;
        ui_textinput_t *edit;
        pd_text_style_t style;
        list_node_t *node;

        list_create(&rects);
        scale = ui_get_actual_scale();
        edit = ui_widget_get_data(w, ui_textinput_proto);
        edit->layer_scale = scale;
        pd_text_style_copy(&style, &edit->layer_source->default_style);
        if (edit->password_char) {
                pd_text_set_style(edit->layer_mask, &style);
                ui_textinput_update_mask(w);
        }
        style.has_fore_color = true;
        style.fore_color = PLACEHOLDER_COLOR;
        pd_text_set_style(edit->layer_placeholder, &style);
        pd_text_style_destroy(&style);
        pd_text_update(edit->layer, &rects);
        for (list_each(node, &rects)) {
                ui_rect_from_pd_rect(&rect, node->data, scale);
                ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
        }
        pd_rects_clear(&rects);
}

static void ui_textinput_update(ui_widget_t *w)
{
        ui_event_t ev;
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);
        bool placeholder_visible = ui_text_edit_get_length(edit->editor) == 0;

        if (placeholder_visible) {
                edit->layer = edit->layer_placeholder;
        } else if (edit->password_char) {
                edit->layer = edit->layer_mask;
        } else {
                edit->layer = edit->layer_source;
        }
        textinput_update_textlayer(w);
        {
                int line_h = pd_text_get_line_height(edit->layer, 0);
                int rows = 1;

                if (line_h > 0) {
                        rows =
                            (int)((w->content_box.height * edit->layer_scale) /
                                  line_h);
                }
                ui_text_edit_set_page_rows(edit->editor, rows);
        }
        if (edit->placeholder_visible != placeholder_visible) {
                ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_PADDING_BOX);
        }
        edit->placeholder_visible = placeholder_visible;
        ui_textinput_update_caret(w);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_CONTENT_BOX);
        /* A change handler may edit the widget again (validation/formatting).
         * Emit the follow-up change in the same update, with a small guard to
         * avoid an infinite handler loop. */
        {
                int guard = 0;

                while (edit->change_pending && guard++ < 8) {
                        edit->change_pending = false;
                        ui_event_init(&ev, "change");
                        ui_widget_emit_event(w, ev, NULL);
                }
        }
}

static void ui_textinput_on_update_style(ui_widget_t *w)
{
        int i;

        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);
        pd_text_style_t text_style;
        ui_text_style_t style;
        pd_text_t *layers[3] = { edit->layer_mask, edit->layer_placeholder,
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
        edit->tasks[TASK_UPDATE] = true;
}

static void ui_textinput_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_textinput_t *edit = ui_widget_get_data(w, ui_textinput_proto);

        switch (task) {
        case UI_TASK_UPDATE_STYLE:
                ui_textinput_on_update_style(w);
                break;
        case UI_TASK_AFTER_UPDATE:
                if (edit->tasks[TASK_UPDATE]) {
                        edit->tasks[TASK_UPDATE] = false;
                        ui_textinput_update(w);
                }
        default:
                break;
        }
}

void ui_register_textinput(void)
{
        ui_textinput_proto = ui_create_widget_prototype("textinput", NULL);
        ui_textinput_proto->init = ui_textinput_on_init;
        ui_textinput_proto->paint = ui_textinput_on_paint;
        ui_textinput_proto->destroy = ui_textinput_on_destroy;
        ui_textinput_proto->settext = ui_textinput_on_parse_text;
        ui_textinput_proto->setattr = ui_textinput_on_set_attr;
        ui_textinput_proto->sizehint = ui_textinput_on_autosize;
        ui_textinput_proto->has_sizehint = true;
        ui_textinput_proto->sizehint_exact = true;
        ui_textinput_proto->resize = ui_textinput_on_resize;
        ui_textinput_proto->update = ui_textinput_on_update;
        ui_load_css_string(ui_textinput_css, __FILE__);
}
