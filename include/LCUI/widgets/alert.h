/*
 * include/LCUI/widgets/alert.h: -- LCUI's Alert widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LCUI_INCLUDE_WIDGETS_ALERT_H
#define LCUI_INCLUDE_WIDGETS_ALERT_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_alert_variant {
        UI_ALERT_VARIANT_DEFAULT,
        UI_ALERT_VARIANT_INFO,
        UI_ALERT_VARIANT_SUCCESS,
        UI_ALERT_VARIANT_WARNING,
        UI_ALERT_VARIANT_DESTRUCTIVE,
} ui_alert_variant_t;

LCUI_API ui_widget_t *ui_create_alert(void);
LCUI_API ui_widget_t *ui_create_alert_content(void);
LCUI_API ui_widget_t *ui_create_alert_title(void);
LCUI_API ui_widget_t *ui_create_alert_description(void);
LCUI_API ui_widget_t *ui_create_alert_action(void);
LCUI_API void ui_alert_set_variant(ui_widget_t *alert,
                                   ui_alert_variant_t variant);
LCUI_API void ui_alert_set_icon(ui_widget_t *alert, const char *icon_name);
LCUI_API void ui_alert_set_title(ui_widget_t *alert, const char *text);
LCUI_API void ui_alert_set_description(ui_widget_t *alert, const char *text);
LCUI_API void ui_register_alert(void);

LCUI_END_HEADER

#endif
