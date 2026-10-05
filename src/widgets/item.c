/*
 * src/widgets/item.c: -- Item widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI/widgets/item.h>
#include <ui/prototype.h>

static ui_widget_prototype_t *ui_item_prototype;
static ui_widget_prototype_t *ui_item_media_prototype;
static ui_widget_prototype_t *ui_item_content_prototype;
static ui_widget_prototype_t *ui_item_title_prototype;
static ui_widget_prototype_t *ui_item_description_prototype;
static ui_widget_prototype_t *ui_item_actions_prototype;

static const char *ui_item_variant_names[] = {
        "default",
        "outline",
        "muted",
};

static const char *ui_item_media_variant_names[] = {
        "default",
        "icon",
        "image",
};

static const char *ui_item_size_names[] = {
        "default",
        "sm",
        "xs",
};

static void ui_item_sync_status(ui_widget_t *item, const char *const *names,
                                size_t count, const char *value)
{
        size_t index;

        for (index = 1; index < count; ++index) {
                ui_widget_remove_status(item, names[index]);
        }
        for (index = 1; index < count; ++index) {
                if (value && strcmp(value, names[index]) == 0) {
                        ui_widget_add_status(item, names[index]);
                        return;
                }
        }
}

static void ui_item_on_set_attr(ui_widget_t *item, const char *name,
                                const char *value)
{
        if (strcmp(name, "variant") == 0) {
                ui_item_sync_status(item, ui_item_variant_names,
                                    sizeof(ui_item_variant_names) /
                                        sizeof(ui_item_variant_names[0]),
                                    value);
        } else if (strcmp(name, "size") == 0) {
                ui_item_sync_status(
                    item, ui_item_size_names,
                    sizeof(ui_item_size_names) / sizeof(ui_item_size_names[0]),
                    value);
        } else if (strcmp(name, "interactive") == 0) {
                if (value &&
                    (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                     strcmp(value, "on") == 0 || strcmp(value, "1") == 0)) {
                        ui_widget_add_status(item, "interactive");
                } else {
                        ui_widget_remove_status(item, "interactive");
                }
        }
}

static void ui_item_media_on_set_attr(ui_widget_t *media, const char *name,
                                      const char *value)
{
        if (strcmp(name, "variant") == 0) {
                ui_item_sync_status(media, ui_item_media_variant_names,
                                    sizeof(ui_item_media_variant_names) /
                                        sizeof(ui_item_media_variant_names[0]),
                                    value);
        }
}

ui_widget_t *ui_create_item(void)
{
        return ui_create_widget_with_prototype(ui_item_prototype);
}
ui_widget_t *ui_create_item_media(void)
{
        return ui_create_widget_with_prototype(ui_item_media_prototype);
}
ui_widget_t *ui_create_item_content(void)
{
        return ui_create_widget_with_prototype(ui_item_content_prototype);
}
ui_widget_t *ui_create_item_title(void)
{
        return ui_create_widget_with_prototype(ui_item_title_prototype);
}
ui_widget_t *ui_create_item_description(void)
{
        return ui_create_widget_with_prototype(ui_item_description_prototype);
}
ui_widget_t *ui_create_item_actions(void)
{
        return ui_create_widget_with_prototype(ui_item_actions_prototype);
}

void ui_item_set_variant(ui_widget_t *item, ui_item_variant_t variant)
{
        if (variant < UI_ITEM_VARIANT_DEFAULT ||
            variant > UI_ITEM_VARIANT_MUTED) {
                variant = UI_ITEM_VARIANT_DEFAULT;
        }
        ui_widget_set_attr(item, "variant", ui_item_variant_names[variant]);
}

void ui_item_set_size(ui_widget_t *item, ui_item_size_t size)
{
        if (size < UI_ITEM_SIZE_DEFAULT || size > UI_ITEM_SIZE_XS) {
                size = UI_ITEM_SIZE_DEFAULT;
        }
        ui_widget_set_attr(item, "size", ui_item_size_names[size]);
}

void ui_item_set_interactive(ui_widget_t *item, bool interactive)
{
        ui_widget_set_attr(item, "interactive", interactive ? "true" : "false");
}

bool ui_item_is_interactive(ui_widget_t *item)
{
        return ui_widget_has_status(item, "interactive");
}

void ui_item_media_set_variant(ui_widget_t *media,
                               ui_item_media_variant_t variant)
{
        if (variant < UI_ITEM_MEDIA_VARIANT_DEFAULT ||
            variant > UI_ITEM_MEDIA_VARIANT_IMAGE) {
                variant = UI_ITEM_MEDIA_VARIANT_DEFAULT;
        }
        ui_widget_set_attr(media, "variant",
                           ui_item_media_variant_names[variant]);
}

void ui_register_item(void)
{
        ui_item_prototype = ui_create_widget_prototype("item", NULL);
        ui_item_media_prototype =
            ui_create_widget_prototype("item-media", NULL);
        ui_item_content_prototype =
            ui_create_widget_prototype("item-content", NULL);
        ui_item_title_prototype =
            ui_create_widget_prototype("item-title", "text");
        ui_item_description_prototype =
            ui_create_widget_prototype("item-description", "text");
        ui_item_actions_prototype =
            ui_create_widget_prototype("item-actions", NULL);
        ui_item_prototype->setattr = ui_item_on_set_attr;
        ui_item_media_prototype->setattr = ui_item_media_on_set_attr;
}
