/*
 * src/widgets/alert.c: -- Alert widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <LCUI/widgets/alert.h>
#include <LCUI/widgets/icon.h>
#include <LCUI/widgets/text.h>
#include <ui/prototype.h>

static ui_widget_prototype_t *ui_alert_prototype;
static ui_widget_prototype_t *ui_alert_content_prototype;
static ui_widget_prototype_t *ui_alert_title_prototype;
static ui_widget_prototype_t *ui_alert_description_prototype;
static ui_widget_prototype_t *ui_alert_action_prototype;

static const char *ui_alert_variant_names[] = {
        "default", "info", "success", "warning", "destructive",
};

static ui_widget_t *ui_alert_find_child(ui_widget_t *alert,
                                        ui_widget_prototype_t *prototype)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(alert, index)); ++index) {
                if (child->proto == prototype) {
                        return child;
                }
        }
        return NULL;
}

static ui_widget_t *ui_alert_get_text_child(ui_widget_t *alert,
                                            ui_widget_prototype_t *prototype)
{
        ui_widget_t *content =
            ui_alert_find_child(alert, ui_alert_content_prototype);
        ui_widget_t *child;

        if (!content) {
                content =
                    ui_create_widget_with_prototype(ui_alert_content_prototype);
                ui_widget_append(alert, content);
        }
        child = ui_alert_find_child(content, prototype);

        if (!child) {
                child = ui_create_widget_with_prototype(prototype);
                ui_widget_append(content, child);
        }
        return child;
}

ui_widget_t *ui_create_alert(void)
{
        return ui_create_widget_with_prototype(ui_alert_prototype);
}

ui_widget_t *ui_create_alert_content(void)
{
        return ui_create_widget_with_prototype(ui_alert_content_prototype);
}

ui_widget_t *ui_create_alert_title(void)
{
        return ui_create_widget_with_prototype(ui_alert_title_prototype);
}

ui_widget_t *ui_create_alert_description(void)
{
        return ui_create_widget_with_prototype(ui_alert_description_prototype);
}

ui_widget_t *ui_create_alert_action(void)
{
        return ui_create_widget_with_prototype(ui_alert_action_prototype);
}

void ui_alert_set_variant(ui_widget_t *alert, ui_alert_variant_t variant)
{
        ui_alert_variant_t current = UI_ALERT_VARIANT_DEFAULT;
        const char *name = ui_widget_get_attr(alert, "variant");

        if (variant < UI_ALERT_VARIANT_DEFAULT ||
            variant > UI_ALERT_VARIANT_DESTRUCTIVE) {
                variant = UI_ALERT_VARIANT_DEFAULT;
        }
        for (current = UI_ALERT_VARIANT_INFO;
             current <= UI_ALERT_VARIANT_DESTRUCTIVE; ++current) {
                if (name &&
                    strcmp(name, ui_alert_variant_names[current]) == 0) {
                        ui_widget_remove_status(
                            alert, ui_alert_variant_names[current]);
                }
        }
        if (variant != UI_ALERT_VARIANT_DEFAULT) {
                ui_widget_add_status(alert, ui_alert_variant_names[variant]);
        }
        ui_widget_set_attr(alert, "variant", ui_alert_variant_names[variant]);
}

static ui_widget_t *ui_alert_find_icon(ui_widget_t *alert)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(alert, index)); ++index) {
                if (ui_check_widget_type(child, "icon")) {
                        return child;
                }
        }
        return NULL;
}

void ui_alert_set_icon(ui_widget_t *alert, const char *icon_name)
{
        ui_widget_t *icon = ui_alert_find_icon(alert);

        if (!icon) {
                icon = ui_create_icon();
                ui_widget_prepend(alert, icon);
        }
        ui_icon_set_name(icon, icon_name);
}

void ui_alert_set_title(ui_widget_t *alert, const char *text)
{
        ui_text_set_content(
            ui_alert_get_text_child(alert, ui_alert_title_prototype), text);
}

void ui_alert_set_description(ui_widget_t *alert, const char *text)
{
        ui_text_set_content(
            ui_alert_get_text_child(alert, ui_alert_description_prototype),
            text);
}

void ui_register_alert(void)
{
        ui_alert_prototype = ui_create_widget_prototype("alert", NULL);
        ui_alert_content_prototype =
            ui_create_widget_prototype("alert-content", NULL);
        ui_alert_title_prototype =
            ui_create_widget_prototype("alert-title", "text");
        ui_alert_description_prototype =
            ui_create_widget_prototype("alert-description", "text");
        ui_alert_action_prototype =
            ui_create_widget_prototype("alert-action", NULL);
}
