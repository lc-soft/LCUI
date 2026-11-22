/*
 * tests/integration/test_theme_accent.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI.h>
#include <css/style_value.h>
#include <ctest-custom.h>

static ui_widget_t *create_primary_button(void)
{
        ui_widget_t *button = ui_create_widget("button");

        ui_widget_add_class(button, "button-primary");
        ui_widget_append(ui_root(), button);
        return button;
}

static void test_blue_accent(void)
{
        ui_widget_t *button = create_primary_button();

        ui_theme_set_accent("blue");
        ui_update();
        ctest_equal_bool("blue primary background",
                         button->computed_style.background_color ==
                             css_color(255, 0x25, 0x63, 0xeb),
                         true);
        ctest_equal_bool(
            "blue primary uses light text",
            button->computed_style.color == css_color(255, 0xfa, 0xfa, 0xfa),
            true);

        ui_widget_add_class(ui_root(), "dark");
        ui_update();
        ctest_equal_bool("blue primary dark background",
                         button->computed_style.background_color ==
                             css_color(255, 0x60, 0xa5, 0xfa),
                         true);

        ui_widget_remove_class(ui_root(), "dark");
        ui_theme_set_accent(NULL);
        ui_update();
        ctest_equal_bool("default primary background",
                         button->computed_style.background_color ==
                             css_color(255, 0x17, 0x17, 0x17),
                         true);

        ui_widget_destroy(button);
        ui_clear_trash();
}

static void test_custom_accent(void)
{
        ui_widget_t *button = create_primary_button();

        ui_theme_set_accent_color("#2563eb");
        ui_update();
        ctest_equal_bool("custom primary background",
                         button->computed_style.background_color ==
                             css_color(255, 0x25, 0x63, 0xeb),
                         true);

        ui_widget_add_class(ui_root(), "dark");
        ui_update();
        ctest_equal_bool("custom primary dark background",
                         button->computed_style.background_color ==
                             css_color(255, 0x51, 0x82, 0xef),
                         true);

        ui_widget_remove_class(ui_root(), "dark");
        ui_theme_set_accent(NULL);
        ui_widget_destroy(button);
        ui_clear_trash();
}

static void test_custom_badge_color(void)
{
        ui_widget_t *badge = ui_create_badge();

        ui_badge_set_color(badge, "blue");
        ui_widget_append(ui_root(), badge);
        ui_theme_set_accent("green");
        ui_update();
        ctest_equal_bool(
            "custom badge color should override the accent",
            badge->computed_style.color == css_color(255, 0x1d, 0x4e, 0xd8),
            true);

        ui_widget_add_class(ui_root(), "dark");
        ui_update();
        ctest_equal_bool(
            "custom badge color should override the dark accent",
            badge->computed_style.color == css_color(255, 0x79, 0xc0, 0xff),
            true);
        ui_widget_remove_class(ui_root(), "dark");

        ui_theme_set_accent(NULL);
        ui_widget_destroy(badge);
        ui_clear_trash();
}

static void test_swatch_dark(void)
{
        ui_widget_t *swatch = ui_create_widget(NULL);

        ui_widget_add_class(swatch, "accent-swatch accent-swatch-neutral");
        ui_widget_append(ui_root(), swatch);
        ui_update();
        ctest_equal_bool("neutral swatch should be dark in light mode",
                         swatch->computed_style.background_color ==
                             css_color(255, 0x17, 0x17, 0x17),
                         true);

        ui_widget_add_class(ui_root(), "dark");
        ui_update();
        ctest_equal_bool("neutral swatch should be light in dark mode",
                         swatch->computed_style.background_color ==
                             css_color(255, 0xf5, 0xf5, 0xf5),
                         true);

        ui_widget_remove_class(ui_root(), "dark");
        ui_widget_destroy(swatch);
        ui_clear_trash();
}

static void test_accent_switch(void)
{
        ui_widget_t *button = create_primary_button();

        ui_theme_set_accent("blue");
        ui_update();
        ui_theme_set_accent("yellow");
        ui_update();
        ctest_equal_bool("switching accents should apply the new accent",
                         button->computed_style.background_color ==
                             css_color(255, 0xca, 0x8a, 0x04),
                         true);
        ctest_equal_bool("switching accents should update the name",
                         strcmp(ui_theme_get_accent(), "yellow") == 0, true);
        ctest_equal_bool("switching accents should drop the old class",
                         ui_widget_has_class(ui_root(), "accent-blue"), false);

        ui_theme_set_accent(NULL);
        ui_widget_destroy(button);
        ui_clear_trash();
}

static void test_focus_ring(void)
{
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_add_status(input, "focus");
        ui_widget_append(ui_root(), input);
        ui_theme_set_accent("green");
        ui_update();
        ctest_equal_bool("focus ring should use the accent",
                         input->computed_style.box_shadow_color ==
                             css_color(51, 0x16, 0xa3, 0x4a),
                         true);
        ctest_equal_bool("focus border should use the accent",
                         input->computed_style.border_top_color ==
                             css_color(255, 0x16, 0xa3, 0x4a),
                         true);

        ui_theme_set_accent_color("#2563eb");
        ui_update();
        ctest_equal_bool("custom focus ring should use the accent",
                         input->computed_style.box_shadow_color ==
                             css_color(51, 0x25, 0x63, 0xeb),
                         true);

        ui_theme_set_accent(NULL);
        ui_widget_destroy(input);
        ui_clear_trash();
}

static void test_grouped_input_focus(void)
{
        ui_widget_t *group = ui_create_widget("input-group");
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_add_class(input, "input-group-input");
        ui_widget_append(group, input);
        ui_widget_append(ui_root(), group);
        ui_widget_add_status(input, "focus");
        ui_theme_set_accent("green");
        ui_update();
        ctest_equal_bool(
            "grouped input should not draw its own ring",
            input->computed_style.type_bits.box_shadow == CSS_BOX_SHADOW_NONE,
            true);
        ctest_equal_float("grouped input should not draw its own border",
                          input->computed_style.border_top_width, 0.0f);

        ui_theme_set_accent(NULL);
        ui_widget_destroy(group);
        ui_clear_trash();
}

static void test_fill_primary(void)
{
        ui_widget_t *fill = ui_create_widget(NULL);

        ui_widget_add_class(fill, "fill-primary");
        ui_widget_append(ui_root(), fill);
        ui_theme_set_accent("green");
        ui_update();
        ctest_equal_bool("fill-primary should follow the accent",
                         fill->computed_style.background_color ==
                             css_color(255, 0x16, 0xa3, 0x4a),
                         true);

        ui_theme_set_accent(NULL);
        ui_widget_destroy(fill);
        ui_clear_trash();
}

void test_theme_accent(void)
{
        lcui_init();

        ctest_describe("accent blue", test_blue_accent);
        ctest_describe("accent custom", test_custom_accent);
        ctest_describe("accent badge color", test_custom_badge_color);
        ctest_describe("accent swatch", test_swatch_dark);
        ctest_describe("accent switch", test_accent_switch);
        ctest_describe("accent focus ring", test_focus_ring);
        ctest_describe("accent grouped input", test_grouped_input_focus);
        ctest_describe("accent fill", test_fill_primary);

        lcui_destroy();
}
