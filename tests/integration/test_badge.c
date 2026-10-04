/*
 * tests/integration/test_badge.c: -- Badge widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void test_variant(void)
{
        ui_widget_t *badge = ui_create_badge();

        ctest_equal_int("should default to default variant",
                        ui_badge_get_variant(badge), UI_BADGE_VARIANT_DEFAULT);

        ui_badge_set_variant(badge, UI_BADGE_VARIANT_SECONDARY);
        ctest_equal_int("should store secondary variant",
                        ui_badge_get_variant(badge),
                        UI_BADGE_VARIANT_SECONDARY);
        ctest_equal_bool("should add secondary status",
                         ui_widget_has_status(badge, "secondary"), true);

        ui_badge_set_variant(badge, UI_BADGE_VARIANT_GHOST);
        ctest_equal_bool("should remove previous variant status",
                         ui_widget_has_status(badge, "secondary"), false);
        ctest_equal_bool("should add ghost status",
                         ui_widget_has_status(badge, "ghost"), true);

        ui_widget_destroy(badge);
}

static void test_outline(void)
{
        ui_widget_t *badge = ui_create_badge();

        ui_badge_set_outline(badge, true);
        ctest_equal_bool("should enable outline variant",
                         ui_badge_is_outline(badge), true);

        ui_badge_set_outline(badge, false);
        ctest_equal_bool("should restore default variant",
                         ui_badge_is_outline(badge), false);

        ui_widget_destroy(badge);
}

static void test_color(void)
{
        ui_widget_t *badge = ui_create_badge();

        ui_badge_set_color(badge, "blue");
        ctest_equal_bool("should add the named color class",
                         ui_widget_has_class(badge, "badge-color-blue"), true);

        ui_badge_set_color(badge, "green");
        ctest_equal_bool("should remove the previous color class",
                         ui_widget_has_class(badge, "badge-color-blue"), false);
        ctest_equal_bool("should add the replacement color class",
                         ui_widget_has_class(badge, "badge-color-green"), true);

        ui_badge_set_background_color(badge, css_color(255, 20, 90, 180));
        ui_badge_set_border_color(badge, css_color(255, 10, 50, 100));

        ui_widget_destroy(badge);
}

static void test_text_content(void)
{
        ui_widget_t *badge = ui_create_badge();

        ui_text_set_content(badge, "New");
        ctest_equal_bool("should inherit the text prototype",
                         ui_check_widget_type(badge, "text"), true);

        ui_widget_destroy(badge);
}

static void test_child_typography(void)
{
        ui_widget_t *badge = ui_create_badge();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *text = ui_create_widget("text");

        ui_icon_set_name(icon, "checkmark");
        ui_text_set_content(text, "Verified");
        ui_widget_append(badge, icon);
        ui_widget_append(badge, text);
        ui_widget_append(ui_root(), badge);
        ui_update();

        ctest_equal_float("badge text should inherit the badge font size",
                          text->computed_style.font_size,
                          badge->computed_style.font_size);
        ctest_equal_float("default badge font size should be 12px",
                          text->computed_style.font_size, 12.f);

        ui_widget_destroy(badge);
}

static void test_icon_does_not_stretch_badge(void)
{
        ui_widget_t *badge = ui_create_badge();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *text = ui_create_widget("text");

        ui_icon_set_name(icon, "checkmark");
        ui_text_set_content(text, "Verified");
        ui_widget_append(badge, icon);
        ui_widget_append(badge, text);
        ui_widget_append(ui_root(), badge);
        ui_update();

        ctest_equal_float("badge icon should be constrained to 12px",
                          icon->computed_style.font_size, 12.f);
        ctest_equal_float("badge height should stay at 20px with an icon",
                          badge->border_box.height, 20.f);

        ui_widget_destroy(badge);
}

void test_badge(void)
{
        lcui_init();

        ctest_describe("badge variant", test_variant);
        ctest_describe("badge outline", test_outline);
        ctest_describe("badge color", test_color);
        ctest_describe("badge text content", test_text_content);
        ctest_describe("badge child typography", test_child_typography);
        ctest_describe("badge icon height", test_icon_does_not_stretch_badge);

        lcui_destroy();
}
