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
        ctest_equal_float("lg button should keep text-sm",
                          button->computed_style.font_size, 14.0f);
        ctest_equal_float("lg button should use a 20px line box",
                          button->computed_style.line_height, 20.0f);

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
        ui_widget_t *media = ui_create_item_media();
        ui_widget_t *title = ui_create_item_title();
        ui_widget_t *description = ui_create_item_description();

        ui_text_set_content(title, "Title");
        ui_text_set_content(description, "Description");
        ui_widget_append(item, media);
        ui_widget_append(item, title);
        ui_widget_append(item, description);
        ui_root_append(item);
        ui_update();

        ctest_equal_int("item should center cross-axis items",
                        item->computed_style.type_bits.align_items,
                        CSS_ALIGN_ITEMS_CENTER);
        ctest_equal_int("item media should align to flex-start",
                        media->computed_style.type_bits.align_self,
                        CSS_ALIGN_SELF_FLEX_START);
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

static ui_widget_t *find_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(w, index)); ++index) {
                if (ui_check_widget_type(child, type)) {
                        return child;
                }
                found = find_type(child, type);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static void check_toast_theme(void)
{
        ui_toast_options_t options = { 0 };
        ui_widget_t *toast;
        ui_widget_t *content;
        ui_widget_t *title;
        ui_widget_t *description;

        options.title = "Event created";
        options.description = "Sunday, December 3 at 9:00 AM";
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        ui_update();
        content = find_type(toast, "toast-content");
        title = find_type(toast, "toast-title");
        description = find_type(toast, "toast-description");

        ctest_equal_int("toast should use a solid border",
                        toast->computed_style.type_bits.border_top_style,
                        CSS_BORDER_STYLE_SOLID);
        ctest_equal_float("toast should use a 16px corner radius",
                          toast->computed_style.border_top_left_radius, 16.0f);
        ctest_equal_float("toast content should use 16px padding",
                          content->computed_style.padding_top, 16.0f);
        ctest_equal_float("toast title should use text-sm",
                          title->computed_style.font_size, 14.0f);
        ctest_equal_int("toast title should use medium weight",
                        title->computed_style.type_bits.font_weight,
                        CSS_FONT_WEIGHT_500);
        ctest_equal_float("toast description should use text-sm",
                          description->computed_style.font_size, 14.0f);

        ui_toast_close(toast);
        ui_update();
        ui_clear_trash();
}

static void check_card_theme(void)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *footer = ui_create_card_footer();

        ui_widget_add_class(content, "clip-safe");
        ui_text_set_content(title, "Card title");
        ui_text_set_content(description, "Card description");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(card, footer);
        ui_root_append(card);
        ui_update();

        ctest_equal_float("card title should use text-base",
                          title->computed_style.font_size, 16.0f);
        ctest_equal_int("card title should use medium weight",
                        title->computed_style.type_bits.font_weight,
                        CSS_FONT_WEIGHT_500);
        ctest_equal_float("card description should use text-sm",
                          description->computed_style.font_size, 14.0f);
        ctest_equal_float(
            "clip-safe card content should use 16px inline padding",
            content->computed_style.padding_left, 16.0f);
        ctest_equal_float(
            "clip-safe card content should reserve 4px top padding",
            content->computed_style.padding_top, 4.0f);
        ctest_equal_float("card footer should use 16px padding",
                          footer->computed_style.padding_top, 16.0f);

        ui_card_set_size(card, UI_CARD_SIZE_SM);
        ui_update();
        ctest_equal_float("small card title should use text-sm",
                          title->computed_style.font_size, 14.0f);
        ctest_equal_float(
            "small clip-safe card content should use 12px inline padding",
            content->computed_style.padding_left, 12.0f);
        ctest_equal_float(
            "small clip-safe card content should reserve 4px top padding",
            content->computed_style.padding_top, 4.0f);

        ui_widget_destroy(card);
}

static void check_attachment_theme(void)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *description = ui_create_attachment_description();

        ui_text_set_content(title, "workspace.png");
        ui_text_set_content(description, "PNG - 820 KB");
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        ui_root_append(attachment);
        ui_update();

        ctest_equal_int("attachment should use a solid border",
                        attachment->computed_style.type_bits.border_top_style,
                        CSS_BORDER_STYLE_SOLID);
        ctest_equal_float("attachment should use a 12px radius",
                          attachment->computed_style.border_top_left_radius,
                          12.0f);
        ctest_equal_float("attachment title should use text-sm",
                          title->computed_style.font_size, 14.0f);
        ctest_equal_int("attachment title should use medium weight",
                        title->computed_style.type_bits.font_weight,
                        CSS_FONT_WEIGHT_500);
        ctest_equal_float("attachment description should use text-xs",
                          description->computed_style.font_size, 12.0f);
        ctest_equal_float("attachment media should be 40px wide",
                          media->content_box.width, 40.0f);

        ui_attachment_set_size(attachment, UI_ATTACHMENT_SIZE_SM);
        ui_update();
        ctest_equal_float("small attachment title should use text-xs",
                          title->computed_style.font_size, 12.0f);
        ctest_equal_float("small attachment media should be 32px wide",
                          media->content_box.width, 32.0f);

        ui_attachment_set_size(attachment, UI_ATTACHMENT_SIZE_DEFAULT);
        ui_attachment_set_orientation(attachment,
                                      UI_ATTACHMENT_ORIENTATION_VERTICAL);
        ui_update();
        ctest_equal_float("vertical attachment title should use 11px",
                          title->computed_style.font_size, 11.0f);

        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_ERROR);
        ui_update();
        ctest_equal_bool(
            "error attachment should keep a solid border",
            attachment->computed_style.type_bits.border_top_style ==
                CSS_BORDER_STYLE_SOLID,
            true);

        ui_widget_destroy(attachment);
}

static void check_button_class_variant(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *proxy = ui_create_widget(NULL);

        ui_widget_add_class(proxy, "button");
        ui_widget_append(ui_root(), button);
        ui_widget_append(ui_root(), proxy);
        ui_update();

        ctest_equal_float("class .button should match the button height",
                          proxy->computed_style.height,
                          button->computed_style.height);
        ctest_equal_float("class .button should match the button line height",
                          proxy->computed_style.line_height,
                          button->computed_style.line_height);
        ctest_equal_float("class .button should match the button padding",
                          proxy->computed_style.padding_top,
                          button->computed_style.padding_top);
        ctest_equal_float("class .button should center text with padding",
                          proxy->computed_style.padding_top, 5.0f);

        ui_widget_destroy(button);
        ui_widget_destroy(proxy);
        ui_clear_trash();
}

static void check_button_class_variants(void)
{
        ui_widget_t *primary = ui_create_button();
        ui_widget_t *secondary = ui_create_button();
        ui_widget_t *xs = ui_create_button();
        ui_widget_t *lg = ui_create_button();
        ui_widget_t *primary_proxy = ui_create_widget(NULL);
        ui_widget_t *secondary_proxy = ui_create_widget(NULL);
        ui_widget_t *xs_proxy = ui_create_widget(NULL);
        ui_widget_t *lg_proxy = ui_create_widget(NULL);

        ui_button_set_variant(primary, UI_BUTTON_VARIANT_PRIMARY);
        ui_button_set_variant(secondary, UI_BUTTON_VARIANT_SECONDARY);
        ui_button_set_size(xs, UI_BUTTON_SIZE_XS);
        ui_button_set_size(lg, UI_BUTTON_SIZE_LG);
        ui_widget_add_class(primary_proxy, "button button-primary");
        ui_widget_add_class(secondary_proxy, "button button-secondary");
        ui_widget_add_class(xs_proxy, "button button-xs");
        ui_widget_add_class(lg_proxy, "button button-lg");
        ui_widget_append(ui_root(), primary);
        ui_widget_append(ui_root(), secondary);
        ui_widget_append(ui_root(), xs);
        ui_widget_append(ui_root(), lg);
        ui_widget_append(ui_root(), primary_proxy);
        ui_widget_append(ui_root(), secondary_proxy);
        ui_widget_append(ui_root(), xs_proxy);
        ui_widget_append(ui_root(), lg_proxy);
        ui_update();

        ctest_equal_bool("class .button-primary should match primary colors",
                         primary_proxy->computed_style.background_color ==
                             primary->computed_style.background_color,
                         true);
        ctest_equal_bool(
            "class .button-secondary should match secondary colors",
            secondary_proxy->computed_style.background_color ==
                secondary->computed_style.background_color,
            true);
        ctest_equal_float("class .button-xs should match xs height",
                          xs_proxy->computed_style.height,
                          xs->computed_style.height);
        ctest_equal_float("class .button-lg should match lg height",
                          lg_proxy->computed_style.height,
                          lg->computed_style.height);

        ui_widget_destroy(primary);
        ui_widget_destroy(secondary);
        ui_widget_destroy(xs);
        ui_widget_destroy(lg);
        ui_widget_destroy(primary_proxy);
        ui_widget_destroy(secondary_proxy);
        ui_widget_destroy(xs_proxy);
        ui_widget_destroy(lg_proxy);
        ui_clear_trash();
}

static void check_button_variant_borders_in_dark(void)
{
        ui_widget_t *link = ui_create_widget("button");
        ui_widget_t *primary = ui_create_widget("button");
        ui_widget_t *ghost = ui_create_widget("button");

        ui_widget_add_class(link, "button-link");
        ui_widget_add_class(primary, "button-primary");
        ui_widget_add_class(ghost, "button-ghost");
        ui_widget_append(ui_root(), link);
        ui_widget_append(ui_root(), primary);
        ui_widget_append(ui_root(), ghost);
        ui_widget_add_class(ui_root(), "dark");
        ui_update();

        ctest_equal_int("dark link button border should stay transparent",
                        (link->computed_style.border_top_color >> 24) & 0xff,
                        0);
        ctest_equal_int("dark primary button border should stay transparent",
                        (primary->computed_style.border_top_color >> 24) & 0xff,
                        0);
        ctest_equal_int("dark ghost button border should stay transparent",
                        (ghost->computed_style.border_top_color >> 24) & 0xff,
                        0);

        ui_widget_remove_class(ui_root(), "dark");
        ui_widget_destroy(link);
        ui_widget_destroy(primary);
        ui_widget_destroy(ghost);
        ui_clear_trash();
}

static void check_link_button_has_no_background_in_dark(void)
{
        ui_widget_t *link = ui_create_widget("button");

        ui_widget_add_class(link, "button-link");
        ui_widget_append(ui_root(), link);
        ui_widget_add_class(ui_root(), "dark");

        ui_widget_add_status(link, "hover");
        ui_update();
        ctest_equal_int(
            "dark link button hover should keep a transparent background",
            (link->computed_style.background_color >> 24) & 0xff, 0);

        ui_widget_remove_status(link, "hover");
        ui_widget_add_status(link, "active");
        ui_update();
        ctest_equal_int(
            "dark link button active should keep a transparent background",
            (link->computed_style.background_color >> 24) & 0xff, 0);

        ui_widget_remove_class(ui_root(), "dark");
        ui_widget_destroy(link);
        ui_clear_trash();
}

void test_style_nova(void)
{
        lcui_init();

        ctest_describe("style-nova: label", check_label_typography);
        ctest_describe("style-nova: button typography",
                       check_button_typography);
        ctest_describe("style-nova: button class variant",
                       check_button_class_variant);
        ctest_describe("style-nova: button class variants",
                       check_button_class_variants);
        ctest_describe("style-nova: button icon typography",
                       check_button_icon_typography);
        ctest_describe("style-nova: button interaction",
                       check_button_interaction_style);
        ctest_describe("style-nova: button variant borders in dark",
                       check_button_variant_borders_in_dark);
        ctest_describe("style-nova: link button background in dark",
                       check_link_button_has_no_background_in_dark);
        ctest_describe("style-nova: badge", check_badge_typography);
        ctest_describe("style-nova: item", check_item_typography);
        ctest_describe("style-nova: tooltip typography",
                       check_tooltip_typography);
        ctest_describe("style-nova: tooltip arrow",
                       check_tooltip_arrow_geometry);
        ctest_describe("style-nova: tooltip padding",
                       check_tooltip_arrow_padding);
        ctest_describe("style-nova: toast", check_toast_theme);
        ctest_describe("style-nova: card", check_card_theme);
        ctest_describe("style-nova: attachment", check_attachment_theme);

        lcui_destroy();
}
