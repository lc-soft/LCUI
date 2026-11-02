/*
 * tests/integration/test_item.c: -- Item widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <stdio.h>

static void test_interactive(void)
{
        ui_widget_t *item = ui_create_item();

        ctest_equal_bool("should not be interactive by default",
                         ui_item_is_interactive(item), false);

        ui_item_set_interactive(item, true);
        ctest_equal_bool("should add interactive status",
                         ui_item_is_interactive(item), true);

        ui_item_set_interactive(item, false);
        ctest_equal_bool("should remove interactive status",
                         ui_item_is_interactive(item), false);

        ui_widget_destroy(item);
}

static void test_parts(void)
{
        ui_widget_t *item = ui_create_item();

        ui_widget_append(item, ui_create_item_media());
        ui_widget_append(item, ui_create_item_content());
        ui_widget_append(item, ui_create_item_actions());

        ctest_equal_bool(
            "should accept media",
            ui_check_widget_type(ui_widget_get_child(item, 0), "item-media"),
            true);
        ctest_equal_bool(
            "should accept content",
            ui_check_widget_type(ui_widget_get_child(item, 1), "item-content"),
            true);
        ctest_equal_bool(
            "should accept actions",
            ui_check_widget_type(ui_widget_get_child(item, 2), "item-actions"),
            true);

        ui_widget_set_disabled(item, true);
        ctest_equal_bool("should support disabled state", item->disabled, true);

        ui_widget_destroy(item);
}

static void test_variants(void)
{
        ui_widget_t *item = ui_create_item();
        ui_widget_t *media = ui_create_item_media();

        ui_item_set_variant(item, UI_ITEM_VARIANT_OUTLINE);
        ctest_equal_bool("should add the outline variant status",
                         ui_widget_has_status(item, "outline"), true);
        ui_item_set_variant(item, UI_ITEM_VARIANT_MUTED);
        ctest_equal_bool("should remove the previous item variant status",
                         ui_widget_has_status(item, "outline"), false);
        ctest_equal_bool("should add the muted variant status",
                         ui_widget_has_status(item, "muted"), true);

        ui_item_media_set_variant(media, UI_ITEM_MEDIA_VARIANT_ICON);
        ctest_equal_bool("should add the icon media variant status",
                         ui_widget_has_status(media, "icon"), true);
        ui_item_media_set_variant(media, UI_ITEM_MEDIA_VARIANT_IMAGE);
        ctest_equal_bool("should replace the media variant status",
                         ui_widget_has_status(media, "icon"), false);
        ctest_equal_bool("should add the image media variant status",
                         ui_widget_has_status(media, "image"), true);

        ui_widget_destroy(item);
        ui_widget_destroy(media);
}

static void test_size(void)
{
        ui_widget_t *item = ui_create_item();

        ui_item_set_size(item, UI_ITEM_SIZE_SM);
        ctest_equal_bool("should add the sm size status",
                         ui_widget_has_status(item, "sm"), true);
        ui_item_set_size(item, UI_ITEM_SIZE_XS);
        ctest_equal_bool("should remove the previous size status",
                         ui_widget_has_status(item, "sm"), false);
        ctest_equal_bool("should add the xs size status",
                         ui_widget_has_status(item, "xs"), true);
        ui_item_set_size(item, UI_ITEM_SIZE_DEFAULT);
        ctest_equal_bool("should clear size status for default",
                         ui_widget_has_status(item, "xs"), false);

        ui_widget_destroy(item);
}

static void test_geometry(void)
{
        ui_widget_t *item = ui_create_item();
        ui_widget_t *media = ui_create_item_media();
        ui_widget_t *content = ui_create_item_content();
        ui_widget_t *title = ui_create_item_title();
        ui_widget_t *description = ui_create_item_description();
        ui_widget_t *actions = ui_create_item_actions();
        ui_widget_t *action = ui_create_widget(NULL);
        float media_x, action_right, content_right;

        ui_text_set_content(title, "Project");
        ui_text_set_content(description,
                            "A short description that stays on one line.");
        ui_widget_set_style_string(item, "width", "400px");
        ui_widget_set_style_string(media, "width", "24px");
        ui_widget_set_style_string(media, "height", "24px");
        ui_widget_set_style_string(action, "width", "30px");
        ui_widget_set_style_string(action, "height", "20px");

        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(actions, action);
        ui_widget_append(item, media);
        ui_widget_append(item, content);
        ui_widget_append(item, actions);
        ui_widget_append(ui_root(), item);
        ui_update();

        ui_widget_get_offset(media, item, &media_x, NULL);
        action_right = 0;
        ui_widget_get_offset(actions, item, &action_right, NULL);
        action_right += actions->border_box.width;
        content_right = 0;
        ui_widget_get_offset(content, item, &content_right, NULL);
        content_right += content->border_box.width;

        ctest_equal_bool("item media should start after the item padding",
                         media_x >= 10.f && media_x <= 14.f, true);
        ctest_equal_bool("item content should take the remaining width",
                         content->border_box.width > 250.f, true);
        ctest_equal_bool("item actions should stick to the trailing edge",
                         item->border_box.width - action_right >= 10.f &&
                             item->border_box.width - action_right <= 16.f,
                         true);
        ctest_equal_bool("item content should not overlap the actions",
                         content_right <= action_right - 30.f + 1.f, true);

        ui_widget_destroy(item);
}

void test_item(void)
{
        lcui_init();
        ctest_describe("item interactive", test_interactive);
        ctest_describe("item parts", test_parts);
        ctest_describe("item variants", test_variants);
        ctest_describe("item size", test_size);
        ctest_describe("item geometry", test_geometry);
        lcui_destroy();
}
