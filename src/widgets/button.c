/*
 * src/widgets/button.c: -- Button widget
 *
 * Copyright (c) 2018-2026, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <ui.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/text.h>

#define UI_BUTTON_ARRAY_LEN(ARR) (sizeof(ARR) / sizeof((ARR)[0]))

typedef struct ui_button {
        /* Sentinel -1 means the variant/size has not been set yet. The CSS
         * `button` rule already provides the default outline look, so we only
         * maintain status classes the moment a value is applied. */
        int variant;
        int size;
} ui_button_t;

static ui_widget_prototype_t *ui_button_prototype = NULL;

static const char *ui_button_variant_names[] = {
        "outline", "primary", "secondary", "ghost", "destructive", "link",
};

static const char *ui_button_size_names[] = {
        "default", "xs", "sm", "lg", "icon", "icon-xs", "icon-sm", "icon-lg",
};

static ui_button_t *ui_button_get_data(ui_widget_t *button)
{
        return ui_widget_get_data(button, ui_button_prototype);
}

static int ui_button_variant_from_value(const char *value)
{
        int i;

        if (value) {
                for (i = 0;
                     i < (int)UI_BUTTON_ARRAY_LEN(ui_button_variant_names);
                     ++i) {
                        if (strcmp(value, ui_button_variant_names[i]) == 0) {
                                return i;
                        }
                }
        }
        return UI_BUTTON_VARIANT_OUTLINE;
}

static int ui_button_size_from_value(const char *value)
{
        int i;

        if (value) {
                for (i = 0; i < (int)UI_BUTTON_ARRAY_LEN(ui_button_size_names);
                     ++i) {
                        if (strcmp(value, ui_button_size_names[i]) == 0) {
                                return i;
                        }
                }
        }
        return UI_BUTTON_SIZE_DEFAULT;
}

/* Track the previous value via the private member so we remove exactly the
 * old status class instead of scanning the whole names table. */
static void ui_button_sync_variant(ui_widget_t *button, int new_variant)
{
        ui_button_t *data = ui_button_get_data(button);

        if (data->variant == new_variant) {
                return;
        }
        if (data->variant >= 0) {
                ui_widget_remove_status(button,
                                        ui_button_variant_names[data->variant]);
        }
        data->variant = new_variant;
        ui_widget_add_status(button, ui_button_variant_names[new_variant]);
}

static void ui_button_sync_size(ui_widget_t *button, int new_size)
{
        ui_button_t *data = ui_button_get_data(button);

        if (data->size == new_size) {
                return;
        }
        if (data->size > UI_BUTTON_SIZE_DEFAULT) {
                ui_widget_remove_status(button,
                                        ui_button_size_names[data->size]);
        }
        data->size = new_size;
        if (new_size > UI_BUTTON_SIZE_DEFAULT) {
                ui_widget_add_status(button, ui_button_size_names[new_size]);
        }
}

static void ui_button_on_init(ui_widget_t *w)
{
        ui_button_t *data;

        ui_button_prototype->proto->init(w);
        w->tab_index = 0;
        data = ui_widget_add_data(w, ui_button_prototype, sizeof(ui_button_t));
        data->variant = -1;
        data->size = -1;
}

static void ui_button_on_set_attr(ui_widget_t *button, const char *name,
                                  const char *value)
{
        if (strcmp(name, "variant") == 0) {
                ui_button_sync_variant(button,
                                       ui_button_variant_from_value(value));
        } else if (strcmp(name, "size") == 0) {
                ui_button_sync_size(button, ui_button_size_from_value(value));
        } else {
                ui_button_prototype->proto->setattr(button, name, value);
        }
}

ui_widget_t *ui_create_button(void)
{
        return ui_create_widget_with_prototype(ui_button_prototype);
}

void ui_button_set_variant(ui_widget_t *button, ui_button_variant_t variant)
{
        if (variant < UI_BUTTON_VARIANT_OUTLINE ||
            variant > UI_BUTTON_VARIANT_LINK) {
                variant = UI_BUTTON_VARIANT_OUTLINE;
        }
        ui_widget_set_attr(button, "variant", ui_button_variant_names[variant]);
}

ui_button_variant_t ui_button_get_variant(ui_widget_t *button)
{
        ui_button_t *data = ui_button_get_data(button);

        if (!data || data->variant < 0) {
                return UI_BUTTON_VARIANT_OUTLINE;
        }
        return (ui_button_variant_t)data->variant;
}

void ui_button_set_size(ui_widget_t *button, ui_button_size_t size)
{
        if (size < UI_BUTTON_SIZE_DEFAULT || size > UI_BUTTON_SIZE_ICON_LG) {
                size = UI_BUTTON_SIZE_DEFAULT;
        }
        ui_widget_set_attr(button, "size", ui_button_size_names[size]);
}

ui_button_size_t ui_button_get_size(ui_widget_t *button)
{
        ui_button_t *data = ui_button_get_data(button);

        if (!data || data->size < 0) {
                return UI_BUTTON_SIZE_DEFAULT;
        }
        return (ui_button_size_t)data->size;
}

void ui_button_set_text_w(ui_widget_t *w, const wchar_t *wstr)
{
        ui_text_set_content_w(w, wstr);
}

void ui_button_set_text(ui_widget_t *w, const char *str)
{
        ui_text_set_content(w, str);
}

void ui_register_button(void)
{
        ui_button_prototype = ui_create_widget_prototype("button", "text");
        ui_button_prototype->init = ui_button_on_init;
        ui_button_prototype->setattr = ui_button_on_set_attr;
}
