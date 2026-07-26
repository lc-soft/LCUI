/*
 * tests/integration/test_flex_layout_percentage_wrap.c
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#ifndef LCUI_FLEX_LAYOUT_PERCENTAGE_WRAP_PREVIEW
#include <ctest-custom.h>
#else
#define ctest_describe(name, fn) fn()
#define ctest_equal_bool(name, actual, expected) \
        ((void)(actual), (void)(expected))
#define ctest_equal_float(name, actual, expected) \
        ((void)(actual), (void)(expected))
#endif

static void load_scene(void)
{
        ui_widget_t *wrapper;

        wrapper = ui_load_xml_file("test_flex_layout_percentage_wrap.xml");
        ui_widget_append(ui_root(), wrapper);
        ui_widget_unwrap(wrapper);
        ui_update();
}

static void test_percentage_wrap_choice_cards(void)
{
        ui_widget_t *group;
        ui_widget_t *card1, *card2;
        ui_widget_t *content1, *desc1;
        float group_width, card1_width, card2_width;

        group = ui_get_widget("choice-card-group");
        card1 = ui_get_widget("choice-card-1");
        card2 = ui_get_widget("choice-card-2");
        content1 = ui_get_widget("choice-card-content-1");
        desc1 = ui_get_widget("choice-card-desc-1");
        group_width = group->border_box.width;
        card1_width = card1->border_box.width;
        card2_width = card2->border_box.width;

        ctest_equal_bool(
            "should contain description intrinsic width",
            content1->content_box.width >= desc1->max_content_width, true);
        ui_widget_request_refresh_style(card1);
        ui_update();
        ctest_equal_float("choice-card-group width stable after style refresh",
                          group->border_box.width, group_width);
        ctest_equal_float("choice-card width stable after style refresh",
                          card1->border_box.width, card1_width);
        ctest_equal_float("second choice-card width stable after style refresh",
                          card2->border_box.width, card2_width);
}

static void test_four_thirty_percent_items(void)
{
        ui_widget_t *item1 = ui_get_widget("percent-30-item-1");
        ui_widget_t *item2 = ui_get_widget("percent-30-item-2");
        ui_widget_t *item3 = ui_get_widget("percent-30-item-3");
        ui_widget_t *item4 = ui_get_widget("percent-30-item-4");

        ctest_equal_float("should keep first two items on one line",
                          item2->border_box.y, item1->border_box.y);
        ctest_equal_float("should keep first three items on one line",
                          item3->border_box.y, item1->border_box.y);
        ctest_equal_bool("should wrap fourth item to next line",
                         item4->border_box.y > item1->border_box.y, true);
}

static void test_mixed_percentage_and_fixed_width(void)
{
        ui_widget_t *group = ui_get_widget("mixed-size-group");
        ui_widget_t *percent = ui_get_widget("mixed-percent-item");
        ui_widget_t *fixed = ui_get_widget("mixed-fixed-item");

        ctest_equal_float("should keep mixed items on one line",
                          percent->border_box.y, fixed->border_box.y);
        ctest_equal_bool("should solve mixed line width near 142.857px",
                         group->content_box.width > 142.f &&
                             group->content_box.width < 144.f,
                         true);
        ctest_equal_bool("should resolve percentage item near 30 percent",
                         percent->border_box.width > 42.f &&
                             percent->border_box.width < 44.f,
                         true);
}

void test_flex_layout_percentage_wrap(void)
{
        lcui_init();
        ui_widget_set_title(ui_root(), L"test flex layout percentage wrap");
        load_scene();

        ctest_describe("percentage width choice cards",
                       test_percentage_wrap_choice_cards);
        ctest_describe("four thirty percent flex items",
                       test_four_thirty_percent_items);
        ctest_describe("mixed percentage and fixed flex items",
                       test_mixed_percentage_and_fixed_width);

#ifndef LCUI_FLEX_LAYOUT_PERCENTAGE_WRAP_PREVIEW
        lcui_destroy();
#endif
}
