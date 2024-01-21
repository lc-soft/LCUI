/*
 * lib/ui-widgets/include/ui_widgets/text.h: -- TextView widget for display text.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_TEXTVIEW_H
#define LIB_UI_WIDGETS_INCLUDE_TEXTVIEW_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

LIBUI_WIDGETS_PUBLIC int ui_text_set_content_w(ui_widget_t *w,
                                                const wchar_t *text);

LIBUI_WIDGETS_PUBLIC int ui_text_set_content(ui_widget_t *w,
                                              const char *utf8_text);

LIBUI_WIDGETS_PUBLIC void ui_text_set_multiline(ui_widget_t *w,
                                                    bool enable);

LIBUI_WIDGETS_PUBLIC void ui_register_text(void);

LIBUI_WIDGETS_PUBLIC void ui_unregister_text(void);

LIBUI_WIDGETS_END_DECLS

#endif
