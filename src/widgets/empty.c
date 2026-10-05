/*
 * src/widgets/empty.c: -- Empty widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <LCUI/widgets/empty.h>
#include <LCUI/widgets/text.h>
#include <ui/prototype.h>

static ui_widget_prototype_t *ui_empty_prototype;
static ui_widget_prototype_t *ui_empty_header_prototype;
static ui_widget_prototype_t *ui_empty_media_prototype;
static ui_widget_prototype_t *ui_empty_title_prototype;
static ui_widget_prototype_t *ui_empty_description_prototype;
static ui_widget_prototype_t *ui_empty_content_prototype;
static ui_widget_prototype_t *ui_empty_action_prototype;

static const char *ui_empty_media_variant_names[] = {
        "default",
        "icon",
};

static void ui_empty_media_on_set_attr(ui_widget_t *media, const char *name,
                                       const char *value)
{
        if (strcmp(name, "variant") != 0) {
                return;
        }
        ui_widget_remove_status(media, "icon");
        if (value && strcmp(value, "icon") == 0) {
                ui_widget_add_status(media, "icon");
        }
}

static ui_widget_t *ui_empty_find_child(ui_widget_t *empty,
                                        ui_widget_prototype_t *prototype)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(empty, index)); ++index) {
                if (child->proto == prototype) {
                        return child;
                }
        }
        return NULL;
}

static ui_widget_t *ui_empty_get_child(ui_widget_t *empty,
                                       ui_widget_prototype_t *prototype)
{
        ui_widget_t *child = ui_empty_find_child(empty, prototype);

        if (!child) {
                child = ui_create_widget_with_prototype(prototype);
                ui_widget_append(empty, child);
        }
        return child;
}

static ui_widget_t *ui_empty_get_header_child(ui_widget_t *empty,
                                              ui_widget_prototype_t *prototype)
{
        ui_widget_t *child = ui_empty_find_child(empty, prototype);
        ui_widget_t *header;

        if (child) {
                return child;
        }
        if (ui_empty_find_child(empty, ui_empty_media_prototype) ||
            ui_empty_find_child(empty, ui_empty_title_prototype) ||
            ui_empty_find_child(empty, ui_empty_description_prototype)) {
                return ui_empty_get_child(empty, prototype);
        }
        header = ui_empty_get_child(empty, ui_empty_header_prototype);
        return ui_empty_get_child(header, prototype);
}

static ui_widget_t *ui_empty_get_content_child(ui_widget_t *empty,
                                               ui_widget_prototype_t *prototype)
{
        ui_widget_t *child = ui_empty_find_child(empty, prototype);
        ui_widget_t *content;

        if (child) {
                return child;
        }
        content = ui_empty_get_child(empty, ui_empty_content_prototype);
        return ui_empty_get_child(content, prototype);
}

ui_widget_t *ui_create_empty(void)
{
        return ui_create_widget_with_prototype(ui_empty_prototype);
}

ui_widget_t *ui_create_empty_header(void)
{
        return ui_create_widget_with_prototype(ui_empty_header_prototype);
}

ui_widget_t *ui_create_empty_media(void)
{
        return ui_create_widget_with_prototype(ui_empty_media_prototype);
}
ui_widget_t *ui_create_empty_title(void)
{
        return ui_create_widget_with_prototype(ui_empty_title_prototype);
}
ui_widget_t *ui_create_empty_description(void)
{
        return ui_create_widget_with_prototype(ui_empty_description_prototype);
}

ui_widget_t *ui_create_empty_content(void)
{
        return ui_create_widget_with_prototype(ui_empty_content_prototype);
}

ui_widget_t *ui_create_empty_action(void)
{
        return ui_create_widget_with_prototype(ui_empty_action_prototype);
}

void ui_empty_media_set_variant(ui_widget_t *media,
                                ui_empty_media_variant_t variant)
{
        if (variant < UI_EMPTY_MEDIA_VARIANT_DEFAULT ||
            variant > UI_EMPTY_MEDIA_VARIANT_ICON) {
                variant = UI_EMPTY_MEDIA_VARIANT_DEFAULT;
        }
        ui_widget_set_attr(media, "variant",
                           ui_empty_media_variant_names[variant]);
}

void ui_empty_set_media(ui_widget_t *empty, const char *text)
{
        ui_text_set_content(
            ui_empty_get_header_child(empty, ui_empty_media_prototype), text);
}

void ui_empty_set_title(ui_widget_t *empty, const char *text)
{
        ui_text_set_content(
            ui_empty_get_header_child(empty, ui_empty_title_prototype), text);
}

void ui_empty_set_description(ui_widget_t *empty, const char *text)
{
        ui_text_set_content(
            ui_empty_get_header_child(empty, ui_empty_description_prototype),
            text);
}

ui_widget_t *ui_empty_set_action(ui_widget_t *empty, ui_widget_t *action)
{
        ui_widget_t *slot =
            ui_empty_get_content_child(empty, ui_empty_action_prototype);

        ui_widget_empty(slot);
        if (action) {
                ui_widget_append(slot, action);
        }
        return slot;
}

void ui_register_empty(void)
{
        ui_empty_prototype = ui_create_widget_prototype("empty", NULL);
        ui_empty_header_prototype =
            ui_create_widget_prototype("empty-header", NULL);
        ui_empty_media_prototype =
            ui_create_widget_prototype("empty-media", "text");
        ui_empty_title_prototype =
            ui_create_widget_prototype("empty-title", "text");
        ui_empty_description_prototype =
            ui_create_widget_prototype("empty-description", "text");
        ui_empty_content_prototype =
            ui_create_widget_prototype("empty-content", NULL);
        ui_empty_action_prototype =
            ui_create_widget_prototype("empty-action", NULL);
        ui_empty_media_prototype->setattr = ui_empty_media_on_set_attr;
}
