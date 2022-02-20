/*
 * textedit.c -- textedit widget, used to allow user edit text.
 *
 * Copyright (c) 2018-22, Liu chao <lc-soft@live.cn> All rights reserved.
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

#ifndef LIB_UI_WIDGETS_INCLUDE_TEXTEDIT_H
#define LIB_UI_WIDGETS_INCLUDE_TEXTEDIT_H

#include <LCUI/def.h>
#include <LCUI/ui.h>

LCUI_BEGIN_HEADER

/** Enable style tag parser  */
LCUI_API void ui_textedit_enable_style_tag(ui_widget_t* widget, LCUI_BOOL enable);

LCUI_API void ui_textedit_enable_multiline(ui_widget_t* widget, LCUI_BOOL enable);

LCUI_API void ui_textedit_move_caret(ui_widget_t* widget, int row, int col);

/** 清空文本内容 */
LCUI_API void ui_textedit_clear_text(ui_widget_t* widget);

/** 获取文本内容 */
LCUI_API size_t ui_textedit_get_text_w(ui_widget_t* w, size_t start,
				      size_t max_len, wchar_t *buf);

/** 获取文本长度 */
LCUI_API size_t ui_textedit_get_text_length(ui_widget_t* w);

/** 设置文本编辑框内的光标，指定是否闪烁、闪烁时间间隔 */
LCUI_API void ui_textedit_set_caret_blink(ui_widget_t* w, LCUI_BOOL visible, int time);

    /** 为文本框设置文本（宽字符版） */
LCUI_API int ui_textedit_set_text_w(ui_widget_t* widget, const wchar_t *wstr);

LCUI_API int ui_textedit_set_text(ui_widget_t* widget, const char *utf8_str);

/** 为文本框追加文本（宽字符版） */
LCUI_API int ui_textedit_append_text_w(ui_widget_t* widget, const wchar_t *wstr);

/** 为文本框插入文本（宽字符版） */
LCUI_API int ui_textedit_insert_text_w(ui_widget_t* widget, const wchar_t *wstr);

/** 设置占位符，当文本编辑框内容为空时显示占位符 */
LCUI_API int ui_textedit_set_placeholder_w(ui_widget_t* w, const wchar_t *wstr);

LCUI_API int ui_textedit_set_placeholder(ui_widget_t* w, const char *str);

/** 设置密码屏蔽符 */
LCUI_API void ui_textedit_set_passworld_char(ui_widget_t* w, wchar_t ch);

LCUI_API void ui_register_textedit(void);

LCUI_END_HEADER

#endif
