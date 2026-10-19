/*
 * tests/integration/test_item.c: -- Item widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>

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

void test_item(void)
{
        lcui_init();
        ctest_describe("item interactive", test_interactive);
        ctest_describe("item parts", test_parts);
        ctest_describe("item variants", test_variants);
        ctest_describe("item size", test_size);
        lcui_destroy();
}
