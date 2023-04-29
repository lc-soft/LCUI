/*
 * scrollbar.c -- LCUI's scrollbar widget
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

#ifndef LIB_UI_WIDGETS_INCLUDE_SCROLLBAR_H
#define LIB_UI_WIDGETS_INCLUDE_SCROLLBAR_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

typedef enum ui_scrollbar_direction_t {
	UI_SCROLLBAR_HORIZONTAL,
	UI_SCROLLBAR_VERTICAL
} ui_scrollbar_direction_t;

LIBUI_WIDGETS_PUBLIC void ui_scrollbar_bind_container(ui_widget_t* w,
					  ui_widget_t* container);

LIBUI_WIDGETS_PUBLIC void ui_scrollbar_bind_target(ui_widget_t* w, ui_widget_t* target);

/** 获取滚动条的位置 */
LIBUI_WIDGETS_PUBLIC float ui_scrollbar_get_position(ui_widget_t* w);

/** 将与滚动条绑定的内容滚动至指定位置 */
LIBUI_WIDGETS_PUBLIC float ui_scrollbar_set_position(ui_widget_t* w, float pos);

/** 设置滚动条的方向 */
LIBUI_WIDGETS_PUBLIC void ui_scrollbar_set_direction(ui_widget_t* w,
					 ui_scrollbar_direction_t direction);

LIBUI_WIDGETS_PUBLIC void ui_register_scrollbar(void);

LIBUI_WIDGETS_END_DECLS

#endif
