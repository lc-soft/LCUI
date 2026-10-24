/*
 * tests/integration/test_card.c: -- Card widget tests
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

static ui_widget_t *find_child_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(w, index)); ++index) {
                if (ui_check_widget_type(child, type)) {
                        return child;
                }
        }
        return NULL;
}

static void test_structure(void)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *action = ui_create_card_action();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *footer = ui_create_card_footer();

        ui_text_set_content(title, "Card title");
        ui_text_set_content(description, "Card description");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(header, action);
        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(card, footer);

        ctest_equal_bool("card should have a header",
                         find_child_type(card, "card-header") == header, true);
        ctest_equal_bool("header should have a title",
                         find_child_type(header, "card-title") == title, true);
        ctest_equal_bool(
            "header should have a description",
            find_child_type(header, "card-description") == description, true);
        ctest_equal_bool("header should have an action",
                         find_child_type(header, "card-action") == action,
                         true);
        ctest_equal_bool("card should have content",
                         find_child_type(card, "card-content") == content,
                         true);
        ctest_equal_bool("card should have a footer",
                         find_child_type(card, "card-footer") == footer, true);

        ui_widget_destroy(card);
}

static void test_size(void)
{
        ui_widget_t *card = ui_create_card();

        ctest_equal_int("card should default to the default size",
                        ui_card_get_size(card), UI_CARD_SIZE_DEFAULT);
        ui_card_set_size(card, UI_CARD_SIZE_SM);
        ctest_equal_int("card should store the small size",
                        ui_card_get_size(card), UI_CARD_SIZE_SM);
        ctest_equal_bool("small card should add the sm status",
                         ui_widget_has_status(card, "sm"), true);
        ui_card_set_size(card, (ui_card_size_t)99);
        ctest_equal_int("invalid size should fall back to default",
                        ui_card_get_size(card), UI_CARD_SIZE_DEFAULT);
        ctest_equal_bool("default card should remove the sm status",
                         ui_widget_has_status(card, "sm"), false);

        ui_widget_destroy(card);
}

static void test_layout(void)
{
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *description = ui_create_card_description();
        ui_widget_t *action = ui_create_card_action();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *footer = ui_create_card_footer();
        float title_x, title_y, action_x, action_y;
        float description_x, description_y;
        float footer_width;

        ui_text_set_content(title, "Card title");
        ui_text_set_content(description, "Card description");
        ui_widget_resize(ui_root(), 400, 600);
        ui_widget_set_style_string(card, "width", "320px");
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(header, action);
        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(card, footer);
        ui_widget_append(ui_root(), card);
        ui_update();

        ui_widget_get_offset(title, ui_root(), &title_x, &title_y);
        ui_widget_get_offset(description, ui_root(), &description_x,
                             &description_y);
        ui_widget_get_offset(action, ui_root(), &action_x, &action_y);
        footer_width = footer->border_box.width;

        ctest_equal_bool("header should mark the action slot",
                         ui_widget_has_class(header, "has-card-action"), true);
        ctest_equal_bool("action should stay on the title row",
                         action_y == title_y, true);
        ctest_equal_bool("action should be aligned to the right",
                         action_x > title_x, true);
        ctest_equal_bool("description should be below the title",
                         description_y > title_y, true);
        ctest_equal_float("description should align with the title",
                          description_x, title_x);
        ctest_equal_float("footer should span the card content width",
                          footer_width, card->content_box.width);

        ui_widget_destroy(card);
}

void test_card(void)
{
        lcui_init();

        ctest_describe("card structure", test_structure);
        ctest_describe("card size", test_size);
        ctest_describe("card layout", test_layout);

        lcui_destroy();
}
