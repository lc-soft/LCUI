/*
 * tests/integration/test_alert.c: -- Alert widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void test_text_setters_reuse_children(void)
{
        ui_widget_t *alert = ui_create_alert();
        ui_widget_t *content = ui_create_alert_content();
        ui_widget_t *title = ui_create_alert_title();

        ui_widget_append(content, title);
        ui_widget_append(alert, content);
        ui_alert_set_title(alert, "Saved");
        ui_alert_set_description(alert, "Your changes are available.");

        ctest_equal_bool("should retain the explicit content",
                         ui_widget_get_child(alert, 0) == content, true);
        ctest_equal_bool("should retain the explicit title",
                         ui_widget_get_child(content, 0) == title, true);
        ctest_equal_bool("should add only the missing description",
                         ui_widget_get_child(content, 1) != NULL, true);
        ctest_equal_bool("should create an alert description",
                         ui_check_widget_type(ui_widget_get_child(content, 1),
                                              "alert-description"),
                         true);

        ui_widget_destroy(alert);
}

static void test_variant(void)
{
        ui_widget_t *alert = ui_create_alert();

        ui_alert_set_variant(alert, UI_ALERT_VARIANT_SUCCESS);
        ctest_equal_bool("should add success status",
                         ui_widget_has_status(alert, "success"), true);

        ui_alert_set_variant(alert, UI_ALERT_VARIANT_WARNING);
        ctest_equal_bool("should remove previous status",
                         ui_widget_has_status(alert, "success"), false);
        ctest_equal_bool("should add warning status",
                         ui_widget_has_status(alert, "warning"), true);

        ui_widget_destroy(alert);
}

static void test_icon_setter(void)
{
        ui_widget_t *alert = ui_create_alert();
        ui_widget_t *content = ui_create_alert_content();
        ui_widget_t *icon;

        ui_widget_append(content, ui_create_alert_title());
        ui_widget_append(alert, content);

        ui_alert_set_icon(alert, "info");
        icon = ui_widget_get_child(alert, 0);
        ctest_equal_bool("should prepend an icon as the first child",
                         ui_check_widget_type(icon, "icon"), true);

        ui_alert_set_icon(alert, "warning");
        ctest_equal_bool("should reuse the icon instead of adding another",
                         ui_widget_get_child(alert, 1) == content, true);
        ctest_equal_bool(
            "should keep the icon as the first child",
            ui_check_widget_type(ui_widget_get_child(alert, 0), "icon"), true);

        ui_widget_destroy(alert);
}

void test_alert(void)
{
        lcui_init();
        ctest_describe("alert text setters", test_text_setters_reuse_children);
        ctest_describe("alert variant", test_variant);
        ctest_describe("alert icon", test_icon_setter);
        lcui_destroy();
}
