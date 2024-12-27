/*
 * include/LCUI/widgets/textinput.h: -- textinput widget, used to allow user edit text.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_TEXTINPUT_H
#define LCUI_INCLUDE_WIDGETS_TEXTINPUT_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

/** Enable style tag parser  */
LCUI_API void ui_textinput_enable_style_tag(ui_widget_t* widget, bool enable);

LCUI_API void ui_textinput_enable_multiline(ui_widget_t* widget, bool enable);

LCUI_API void ui_textinput_move_caret(ui_widget_t* widget, int row, int col);

/** 清空文本内容 */
LCUI_API void ui_textinput_clear_text(ui_widget_t* widget);

/** 获取文本内容 */
LCUI_API size_t ui_textinput_get_text_w(ui_widget_t* w, size_t start,
				      size_t max_len, wchar_t *buf);

/** 获取文本长度 */
LCUI_API size_t ui_textinput_get_text_length(ui_widget_t* w);

/** 设置文本编辑框内的光标，指定是否闪烁、闪烁时间间隔 */
LCUI_API void ui_textinput_set_caret_blink(ui_widget_t* w, bool visible, int time);

    /** 为文本框设置文本（宽字符版） */
LCUI_API int ui_textinput_set_text_w(ui_widget_t* widget, const wchar_t *wstr);

LCUI_API int ui_textinput_set_text(ui_widget_t* widget, const char *utf8_str);

/** 为文本框追加文本（宽字符版） */
LCUI_API int ui_textinput_append_text_w(ui_widget_t* widget, const wchar_t *wstr);

/** 为文本框插入文本（宽字符版） */
LCUI_API int ui_textinput_insert_text_w(ui_widget_t* widget, const wchar_t *wstr);

/** 设置占位符，当文本编辑框内容为空时显示占位符 */
LCUI_API int ui_textinput_set_placeholder_w(ui_widget_t* w, const wchar_t *wstr);

LCUI_API int ui_textinput_set_placeholder(ui_widget_t* w, const char *str);

/** 设置密码屏蔽符 */
LCUI_API void ui_textinput_set_password_char(ui_widget_t* w, wchar_t ch);

LCUI_API void ui_register_textinput(void);

LCUI_END_HEADER

#endif
