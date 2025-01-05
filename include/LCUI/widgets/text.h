/*
 * include/LCUI/widgets/text.h: -- TextView widget for display
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_TEXT_H
#define LCUI_INCLUDE_WIDGETS_TEXT_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

LCUI_API int ui_text_set_content_w(ui_widget_t *w,
                                               const wchar_t *text);

LCUI_API size_t ui_text_get_content_w(ui_widget_t *w, wchar_t *buf,
                                                  size_t size);

LCUI_API int ui_text_set_content(ui_widget_t *w,
                                             const char *utf8_text);

LCUI_API void ui_text_set_multiline(ui_widget_t *w, bool enable);

LCUI_API void ui_register_text(void);

LCUI_API void ui_unregister_text(void);

LCUI_END_HEADER

#endif
