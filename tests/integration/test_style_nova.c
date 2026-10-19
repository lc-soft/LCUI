/*
 * tests/integration/test_style_nova.c: -- style-nova theme contract tests
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

static void check_label_typography(void)
{
        ui_widget_t *label = ui_create_label();

        ui_widget_append(ui_root(), label);
        ui_update();

        ctest_equal_int("label should use the medium font weight",
                        label->computed_style.type_bits.font_weight,
                        CSS_FONT_WEIGHT_500);

        ui_widget_destroy(label);
}

static void check_button_typography(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_text(button, "Style");
        ui_widget_append(ui_root(), button);
        ui_update();

        ctest_equal_float("default button should use text-sm",
                          button->computed_style.font_size, 14.0f);
        ctest_equal_int("default button should use medium font weight",
                        button->computed_style.type_bits.font_weight,
                        CSS_FONT_WEIGHT_500);
        ctest_equal_int("default button should use a solid border",
                        button->computed_style.type_bits.border_top_style,
                        CSS_BORDER_STYLE_SOLID);

        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ui_update();
        ctest_equal_float("xs button should use text-xs",
                          button->computed_style.font_size, 12.0f);
        ctest_equal_float("xs button should use a 16px line box",
                          button->computed_style.line_height, 16.0f);

        ui_button_set_size(button, UI_BUTTON_SIZE_DEFAULT);
        ui_update();
        ctest_equal_float("default button should use a 20px line box",
                          button->computed_style.line_height, 20.0f);

        ui_button_set_size(button, UI_BUTTON_SIZE_LG);
        ui_update();
        ctest_equal_float("lg button should use text-base",
                          button->computed_style.font_size, 16.0f);
        ctest_equal_float("lg button should use a 24px line box",
                          button->computed_style.line_height, 24.0f);

        ui_widget_destroy(button);
}

static void check_button_icon_typography(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, "arrow-up-right");
        ui_widget_append(button, icon);
        ui_widget_append(ui_root(), button);
        ui_update();

        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_XS);
        ui_update();
        ctest_equal_float("icon-xs button should use a 12px icon",
                          icon->computed_style.font_size, 12.0f);

        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_SM);
        ui_update();
        ctest_equal_float("icon-sm button should use a 14px icon",
                          icon->computed_style.font_size, 14.0f);

        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_update();
        ctest_equal_float("icon button should use a 16px icon",
                          icon->computed_style.font_size, 16.0f);

        ui_widget_destroy(button);
}

static void check_button_interaction_style(void)
{
        ui_widget_t *button = ui_create_button();

        ui_widget_append(ui_root(), button);
        ui_widget_add_status(button, "active");
        ui_update();

        ctest_equal_int("active button should use relative positioning",
                        button->computed_style.type_bits.position,
                        CSS_POSITION_RELATIVE);
        ctest_equal_float("active button should shift down by one pixel",
                          button->computed_style.top, 1.0f);

        ui_widget_remove_status(button, "active");
        ui_widget_set_disabled(button, true);
        ui_widget_add_status(button, "hover");
        ui_update();
        ctest_equal_float("disabled button should use 50% opacity",
                          button->computed_style.opacity, 0.5f);

        ui_widget_destroy(button);
}

static void check_badge_typography(void)
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

        ctest_equal_float("badge should use text-xs",
                          badge->computed_style.font_size, 12.0f);
        ctest_equal_int("badge should use medium font weight",
                        badge->computed_style.type_bits.font_weight,
                        CSS_FONT_WEIGHT_500);
        ctest_equal_float("badge should use a 20px height",
                          badge->border_box.height, 20.0f);
        ctest_equal_float("badge icon should use a 12px icon",
                          icon->computed_style.font_size, 12.0f);
        ctest_equal_float("badge text should use text-xs",
                          text->computed_style.font_size, 12.0f);

        ui_widget_destroy(badge);
}

static void check_item_typography(void)
{
        ui_widget_t *item = ui_create_item();
        ui_widget_t *title = ui_create_item_title();
        ui_widget_t *description = ui_create_item_description();

        ui_text_set_content(title, "Title");
        ui_text_set_content(description, "Description");
        ui_widget_append(item, title);
        ui_widget_append(item, description);
        ui_root_append(item);
        ui_update();

        ctest_equal_float("default item title should use text-sm",
                          title->computed_style.font_size, 14.0f);
        ctest_equal_float("default item description should use text-sm",
                          description->computed_style.font_size, 14.0f);

        ui_item_set_size(item, UI_ITEM_SIZE_SM);
        ui_update();
        ctest_equal_float("sm item title should stay text-sm",
                          title->computed_style.font_size, 14.0f);
        ctest_equal_float("sm item description should stay text-sm",
                          description->computed_style.font_size, 14.0f);

        ui_item_set_size(item, UI_ITEM_SIZE_XS);
        ui_update();
        ctest_equal_float("xs item title should stay text-sm",
                          title->computed_style.font_size, 14.0f);
        ctest_equal_float("xs item description should use text-xs",
                          description->computed_style.font_size, 12.0f);

        ui_widget_destroy(item);
}

static void check_tooltip_typography(void)
{
        ui_widget_t *inner = ui_create_widget(NULL);

        ui_widget_add_class(inner, "tooltip-inner");
        ui_root_append(inner);
        ui_update();

        ctest_equal_float("tooltip inner should use text-xs",
                          inner->computed_style.font_size, 12.0f);
        ctest_equal_uint("tooltip inner should use white text",
                         inner->computed_style.color,
                         css_color(255, 255, 255, 255));

        ui_widget_destroy(inner);
}

static void check_tooltip_arrow_geometry(void)
{
        ui_widget_t *top = ui_create_widget(NULL);
        ui_widget_t *bottom = ui_create_widget(NULL);
        ui_widget_t *left = ui_create_widget(NULL);
        ui_widget_t *right = ui_create_widget(NULL);

        ui_widget_add_class(top, "tooltip-arrow");
        ui_widget_add_class(top, "tooltip-side-top");
        ui_widget_add_class(bottom, "tooltip-arrow");
        ui_widget_add_class(bottom, "tooltip-side-bottom");
        ui_widget_add_class(left, "tooltip-arrow");
        ui_widget_add_class(left, "tooltip-side-left");
        ui_widget_add_class(right, "tooltip-arrow");
        ui_widget_add_class(right, "tooltip-side-right");
        ui_root_append(top);
        ui_root_append(bottom);
        ui_root_append(left);
        ui_root_append(right);
        ui_update();

        ctest_equal_float("top arrow should use a 5px top border",
                          top->computed_style.border_top_width, 5.0f);
        ctest_equal_float("top arrow should use 4px side borders",
                          top->computed_style.border_left_width, 4.0f);
        ctest_equal_bool(
            "top arrow should be painted",
            css_color_alpha(top->computed_style.border_top_color) > 0, true);
        ctest_equal_int("top arrow should keep transparent side borders",
                        css_color_alpha(top->computed_style.border_left_color),
                        0);
        ctest_equal_float("bottom arrow should use a 5px bottom border",
                          bottom->computed_style.border_bottom_width, 5.0f);
        ctest_equal_float("left arrow should use a 5px left border",
                          left->computed_style.border_left_width, 5.0f);
        ctest_equal_float("right arrow should use a 5px right border",
                          right->computed_style.border_right_width, 5.0f);

        ui_widget_destroy(top);
        ui_widget_destroy(bottom);
        ui_widget_destroy(left);
        ui_widget_destroy(right);
}

static void check_tooltip_arrow_padding(void)
{
        ui_widget_t *top = ui_create_tooltip_content();
        ui_widget_t *bottom = ui_create_tooltip_content();
        ui_widget_t *left = ui_create_tooltip_content();
        ui_widget_t *right = ui_create_tooltip_content();

        ui_widget_add_class(top, "tooltip-side-top");
        ui_widget_add_class(bottom, "tooltip-side-bottom");
        ui_widget_add_class(left, "tooltip-side-left");
        ui_widget_add_class(right, "tooltip-side-right");
        ui_root_append(top);
        ui_root_append(bottom);
        ui_root_append(left);
        ui_root_append(right);
        ui_update();

        ctest_equal_float("top tooltip should reserve 5px bottom space",
                          top->computed_style.padding_bottom, 5.0f);
        ctest_equal_float("bottom tooltip should reserve 5px top space",
                          bottom->computed_style.padding_top, 5.0f);
        ctest_equal_float("left tooltip should reserve 5px right space",
                          left->computed_style.padding_right, 5.0f);
        ctest_equal_float("right tooltip should reserve 5px left space",
                          right->computed_style.padding_left, 5.0f);

        ui_widget_destroy(top);
        ui_widget_destroy(bottom);
        ui_widget_destroy(left);
        ui_widget_destroy(right);
}

void test_style_nova(void)
{
        lcui_init();

        ctest_describe("style-nova: label", check_label_typography);
        ctest_describe("style-nova: button typography",
                       check_button_typography);
        ctest_describe("style-nova: button icon typography",
                       check_button_icon_typography);
        ctest_describe("style-nova: button interaction",
                       check_button_interaction_style);
        ctest_describe("style-nova: badge", check_badge_typography);
        ctest_describe("style-nova: item", check_item_typography);
        ctest_describe("style-nova: tooltip typography",
                       check_tooltip_typography);
        ctest_describe("style-nova: tooltip arrow",
                       check_tooltip_arrow_geometry);
        ctest_describe("style-nova: tooltip padding",
                       check_tooltip_arrow_padding);

        lcui_destroy();
}
