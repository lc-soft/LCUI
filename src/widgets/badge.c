/*
 * src/widgets/badge.c: -- Badge widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <LCUI/widgets/badge.h>
#include <css.h>
#include <ui/css.h>
#include <ui/prototype.h>
#include <ui/style.h>

#define UI_BADGE_ARRAY_LEN(ARR) (sizeof(ARR) / sizeof((ARR)[0]))

/* Track the previously applied variant/color so we remove exactly the old
 * status class or color class instead of scanning the whole table. A value of
 * -1 means it has not been applied yet. The base `badge` CSS rule already
 * provides the default variant look, so the default variant keeps no status. */
typedef struct ui_badge {
        int variant;
        int color;
} ui_badge_t;

static ui_widget_prototype_t *ui_badge_prototype;

static const char *ui_badge_variant_names[] = {
        "default", "secondary", "destructive", "outline", "ghost",
};

static const char *ui_badge_color_names[] = {
        "blue", "green", "sky", "purple", "red",
};

static ui_badge_t *ui_badge_get_data(ui_widget_t *badge)
{
        return ui_widget_get_data(badge, ui_badge_prototype);
}

static int ui_badge_variant_from_value(const char *value)
{
        int i;

        if (value) {
                for (i = 0; i < (int)UI_BADGE_ARRAY_LEN(ui_badge_variant_names);
                     ++i) {
                        if (strcmp(value, ui_badge_variant_names[i]) == 0) {
                                return i;
                        }
                }
        }
        return UI_BADGE_VARIANT_DEFAULT;
}

static int ui_badge_color_from_value(const char *value)
{
        int i;

        if (value) {
                for (i = 0; i < (int)UI_BADGE_ARRAY_LEN(ui_badge_color_names);
                     ++i) {
                        if (strcmp(value, ui_badge_color_names[i]) == 0) {
                                return i;
                        }
                }
        }
        return -1;
}

static void ui_badge_set_border_colors(ui_widget_t *badge,
                                       css_color_value_t color)
{
        ui_widget_set_style_color_value(badge, css_prop_border_top_color,
                                        color);
        ui_widget_set_style_color_value(badge, css_prop_border_right_color,
                                        color);
        ui_widget_set_style_color_value(badge, css_prop_border_bottom_color,
                                        color);
        ui_widget_set_style_color_value(badge, css_prop_border_left_color,
                                        color);
}

static void ui_badge_sync_variant(ui_widget_t *badge, int new_variant)
{
        ui_badge_t *data = ui_badge_get_data(badge);

        if (data->variant == new_variant) {
                return;
        }
        if (data->variant > UI_BADGE_VARIANT_DEFAULT) {
                ui_widget_remove_status(badge,
                                        ui_badge_variant_names[data->variant]);
        }
        data->variant = new_variant;
        if (new_variant > UI_BADGE_VARIANT_DEFAULT) {
                ui_widget_add_status(badge,
                                     ui_badge_variant_names[new_variant]);
        }
}

static void ui_badge_sync_color(ui_widget_t *badge, int new_color)
{
        ui_badge_t *data = ui_badge_get_data(badge);
        char class_name[32];

        if (data->color == new_color) {
                return;
        }
        if (data->color >= 0) {
                snprintf(class_name, sizeof(class_name), "badge-color-%s",
                         ui_badge_color_names[data->color]);
                ui_widget_remove_class(badge, class_name);
        }
        data->color = new_color;
        if (new_color >= 0) {
                snprintf(class_name, sizeof(class_name), "badge-color-%s",
                         ui_badge_color_names[new_color]);
                ui_widget_add_class(badge, class_name);
        }
}

static void ui_badge_on_init(ui_widget_t *badge)
{
        ui_badge_t *data;

        ui_badge_prototype->proto->init(badge);
        data =
            ui_widget_add_data(badge, ui_badge_prototype, sizeof(ui_badge_t));
        data->variant = UI_BADGE_VARIANT_DEFAULT;
        data->color = -1;
}

static void ui_badge_on_set_attr(ui_widget_t *badge, const char *name,
                                 const char *value)
{
        if (strcmp(name, "variant") == 0) {
                ui_badge_sync_variant(badge,
                                      ui_badge_variant_from_value(value));
        } else if (strcmp(name, "color") == 0) {
                ui_badge_sync_color(badge, ui_badge_color_from_value(value));
        } else {
                ui_badge_prototype->proto->setattr(badge, name, value);
        }
}

ui_widget_t *ui_create_badge(void)
{
        return ui_create_widget_with_prototype(ui_badge_prototype);
}

void ui_badge_set_variant(ui_widget_t *badge, ui_badge_variant_t variant)
{
        if (variant < UI_BADGE_VARIANT_DEFAULT ||
            variant > UI_BADGE_VARIANT_GHOST) {
                variant = UI_BADGE_VARIANT_DEFAULT;
        }
        ui_widget_set_attr(badge, "variant", ui_badge_variant_names[variant]);
}

ui_badge_variant_t ui_badge_get_variant(ui_widget_t *badge)
{
        ui_badge_t *data = ui_badge_get_data(badge);

        if (!data || data->variant < 0) {
                return UI_BADGE_VARIANT_DEFAULT;
        }
        return (ui_badge_variant_t)data->variant;
}

void ui_badge_set_outline(ui_widget_t *badge, bool outline)
{
        if (outline) {
                ui_badge_set_variant(badge, UI_BADGE_VARIANT_OUTLINE);
        } else if (ui_badge_is_outline(badge)) {
                ui_badge_set_variant(badge, UI_BADGE_VARIANT_DEFAULT);
        }
}

bool ui_badge_is_outline(ui_widget_t *badge)
{
        return ui_badge_get_variant(badge) == UI_BADGE_VARIANT_OUTLINE;
}

void ui_badge_set_color(ui_widget_t *badge, const char *color)
{
        ui_widget_set_attr(badge, "color", color ? color : "");
}

void ui_badge_set_background_color(ui_widget_t *badge, css_color_value_t color)
{
        ui_widget_set_style_color_value(badge, css_prop_background_color,
                                        color);
}

void ui_badge_set_border_color(ui_widget_t *badge, css_color_value_t color)
{
        ui_badge_set_border_colors(badge, color);
}

void ui_register_badge(void)
{
        ui_badge_prototype = ui_create_widget_prototype("badge", "text");
        ui_badge_prototype->init = ui_badge_on_init;
        ui_badge_prototype->setattr = ui_badge_on_set_attr;
}
