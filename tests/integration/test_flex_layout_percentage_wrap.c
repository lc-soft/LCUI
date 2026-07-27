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

static void test_percentages_totaling_one_hundred(void)
{
        ui_widget_t *item1 = ui_get_widget("percent-100-item-1");
        ui_widget_t *item2 = ui_get_widget("percent-100-item-2");
        ui_widget_t *item3 = ui_get_widget("percent-100-item-3");

        ctest_equal_float("should keep second item on first line",
                          item2->border_box.y, item1->border_box.y);
        ctest_equal_float("should keep third item on first line",
                          item3->border_box.y, item1->border_box.y);
}

static void test_mixed_percentage_fixed_width_and_gap(void)
{
        ui_widget_t *group = ui_get_widget("mixed-gap-group");
        ui_widget_t *percent = ui_get_widget("mixed-gap-percent-item");
        ui_widget_t *fixed = ui_get_widget("mixed-gap-fixed-item");

        ctest_equal_float("should keep gapped mixed items on one line",
                          percent->border_box.y, fixed->border_box.y);
        ctest_equal_bool("should solve gapped mixed line width near 157px",
                         group->content_box.width > 156.f &&
                             group->content_box.width < 158.f,
                         true);
        ctest_equal_bool("should resolve gapped percentage item near 47px",
                         percent->border_box.width > 46.f &&
                             percent->border_box.width < 48.f,
                         true);
}

static void test_full_width_bordered_item_stability(void)
{
        ui_widget_t *group = ui_get_widget("full-width-group");
        ui_widget_t *item = ui_get_widget("full-width-item");
        float group_width = group->border_box.width;
        float item_width = item->border_box.width;

        ctest_equal_bool("should keep full width group finite and positive",
                         group_width > 0.f && group_width < 600.f, true);
        ctest_equal_bool("should keep full width item finite and positive",
                         item_width > 0.f && item_width < 600.f, true);
        ui_widget_request_refresh_style(item);
        ui_update();
        ctest_equal_float(
            "should keep full width group stable after style refresh",
            group->border_box.width, group_width);
        ctest_equal_float(
            "should keep full width item stable after style refresh",
            item->border_box.width, item_width);
}

static void test_intrinsic_percentage_item(void)
{
        ui_widget_t *group = ui_get_widget("intrinsic-percent-group");
        ui_widget_t *item = ui_get_widget("intrinsic-percent-item");
        ui_widget_t *child = ui_get_widget("intrinsic-percent-child");

        ctest_equal_bool("should contain fixed intrinsic child",
                         child->border_box.width <= item->content_box.width,
                         true);
        ctest_equal_bool("should round percentage group width up to 1/64px",
                         group->content_box.width >= 333.3f &&
                             group->content_box.width <= 333.5f,
                         true);
}

static void test_decorated_intrinsic_percentage_item(void)
{
        ui_widget_t *group = ui_get_widget("decorated-percent-group");
        ui_widget_t *item = ui_get_widget("decorated-percent-item");
        ui_widget_t *child = ui_get_widget("decorated-percent-child");

        ctest_equal_bool(
            "should solve decorated percentage group near 333.34375px",
            group->content_box.width >= 333.3f &&
                group->content_box.width <= 333.5f,
            true);
        ctest_equal_bool("should contain fixed child in percentage content box",
                         child->border_box.width <= item->content_box.width,
                         true);
}

static void test_minimum_percentage_item(void)
{
        ui_widget_t *group = ui_get_widget("min-percent-group");
        ui_widget_t *percent = ui_get_widget("min-percent-item");
        ui_widget_t *fixed = ui_get_widget("min-percent-fixed-item");

        ctest_equal_float("should keep minimum and fixed items on one line",
                          percent->border_box.y, fixed->border_box.y);
        ctest_equal_bool("should contain both minimum and fixed outer widths",
                         percent->outer_box.width + fixed->outer_box.width <=
                             group->content_box.width,
                         true);
        ctest_equal_bool("should solve minimum group width near 200px",
                         group->content_box.width >= 199.f &&
                             group->content_box.width <= 201.f,
                         true);
}

static void test_percentage_gap(void)
{
        ui_widget_t *group = ui_get_widget("percent-gap-group");
        ui_widget_t *percent = ui_get_widget("percent-gap-item");
        ui_widget_t *fixed = ui_get_widget("percent-gap-fixed-item");
        float group_width = group->content_box.width;
        float percent_width = percent->border_box.width;
        float fixed_x = fixed->border_box.x;
        float gap = fixed_x - percent->border_box.x - percent_width;
        int i;

        ctest_equal_float("should keep percentage gap items on one line",
                          percent->border_box.y, fixed->border_box.y);
        ctest_equal_bool("should solve percentage gap group near 166.667px",
                         group_width >= 166.f && group_width <= 168.f, true);
        ctest_equal_bool("should resolve percentage gap near 16.667px",
                         gap >= 16.f && gap <= 17.f, true);
        for (i = 0; i < 3; ++i) {
                ui_widget_request_refresh_style(group);
                ui_update();
        }
        ctest_equal_float("should keep percentage gap group stable",
                          group->content_box.width, group_width);
        ctest_equal_float("should keep percentage gap item stable",
                          percent->border_box.width, percent_width);
        ctest_equal_float("should keep percentage gap position stable",
                          fixed->border_box.x, fixed_x);
}

static void test_decorated_empty_percentage_item(void)
{
        ui_widget_t *group = ui_get_widget("decorated-empty-percent-group");
        ui_widget_t *percent = ui_get_widget("decorated-empty-percent-item");
        ui_widget_t *fixed = ui_get_widget("decorated-empty-fixed-item");

        ctest_equal_float("should keep decorated empty items on one line",
                          percent->border_box.y, fixed->border_box.y);
        ctest_equal_bool(
            "should solve decorated empty percentage group near 174.296875px",
            group->content_box.width >= 174.28f &&
                group->content_box.width <= 174.31f,
            true);
        ctest_equal_bool(
            "should resolve decorated percentage outer width near 74.286px",
            percent->outer_box.width >= 74.28f &&
                percent->outer_box.width <= 74.31f,
            true);
        ctest_equal_bool("should preserve fixed item width near 100px",
                         fixed->outer_box.width >= 99.99f &&
                             fixed->outer_box.width <= 100.01f,
                         true);
}

static void test_full_percentage_row_wrap(void)
{
        ui_widget_t *group = ui_get_widget("full-percent-wrap-group");
        ui_widget_t *percent = ui_get_widget("full-percent-wrap-item");
        ui_widget_t *fixed = ui_get_widget("full-percent-wrap-fixed");

        ctest_equal_bool("should wrap fixed item after full percentage row",
                         fixed->border_box.y > percent->border_box.y, true);
        ctest_equal_bool(
            "should keep full percentage group width finite and positive",
            group->content_box.width > 0.f && group->content_box.width < 600.f,
            true);
        ctest_equal_bool("should derive full percentage group near 100px",
                         group->content_box.width >= 99.f &&
                             group->content_box.width <= 101.f,
                         true);
}

static void test_percentage_row_wrap(void)
{
        ui_widget_t *group = ui_get_widget("percent-row-wrap-group");
        ui_widget_t *item30 = ui_get_widget("percent-row-wrap-item-30");
        ui_widget_t *item70 = ui_get_widget("percent-row-wrap-item-70");
        ui_widget_t *fixed = ui_get_widget("percent-row-wrap-fixed");

        ctest_equal_float("should keep 30 and 70 percent items on first row",
                          item70->border_box.y, item30->border_box.y);
        ctest_equal_bool("should wrap fixed item after percentage row",
                         fixed->border_box.y > item30->border_box.y, true);
        ctest_equal_bool(
            "should keep percentage row group width finite and positive",
            group->content_box.width > 0.f && group->content_box.width < 600.f,
            true);
        ctest_equal_bool("should derive percentage row group near 100px",
                         group->content_box.width >= 99.f &&
                             group->content_box.width <= 101.f,
                         true);
}

static void test_fixed_before_full_percentage_wrap(void)
{
        ui_widget_t *group = ui_get_widget("fixed-before-percent-group");
        ui_widget_t *fixed = ui_get_widget("fixed-before-percent-fixed");
        ui_widget_t *percent = ui_get_widget("fixed-before-percent-item");

        ctest_equal_bool("should wrap full percentage item after fixed item",
                         percent->border_box.y > fixed->border_box.y, true);
        ctest_equal_bool("should keep wrapped group width near 100px",
                         group->content_box.width >= 99.f &&
                             group->content_box.width <= 101.f,
                         true);
}

static void test_decorated_percentage_before_percentage_wrap(void)
{
        ui_widget_t *group = ui_get_widget("decorated-before-percent-group");
        ui_widget_t *decorated = ui_get_widget("decorated-before-percent-item");
        ui_widget_t *next = ui_get_widget("decorated-before-percent-next");

        ctest_equal_bool(
            "should wrap percentage item after decorated percentage item",
            next->border_box.y > decorated->border_box.y, true);
        ctest_equal_bool(
            "should keep decorated percentage group finite and positive",
            group->content_box.width > 0.f && group->content_box.width < 600.f,
            true);
}

static void test_fixed_wrap_content_contributions(void)
{
        ui_widget_t *group = ui_get_widget("fixed-contribution-group");
        ui_widget_t *item1 = ui_get_widget("fixed-contribution-item-1");
        ui_widget_t *item2 = ui_get_widget("fixed-contribution-item-2");

        ctest_equal_bool("should have max content width near 200px",
                         group->max_content_width >= 199.f &&
                             group->max_content_width <= 201.f,
                         true);
        ctest_equal_bool("should have min content width near 100px",
                         group->min_content_width >= 99.f &&
                             group->min_content_width <= 101.f,
                         true);
        ctest_equal_bool("should allow both fixed items on the used row",
                         item2->border_box.y == item1->border_box.y, true);
        ctest_equal_bool("should allow used group width near 200px",
                         group->content_box.width >= 199.f &&
                             group->content_box.width <= 201.f,
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
        ctest_describe("percentages totaling one hundred",
                       test_percentages_totaling_one_hundred);
        ctest_describe("mixed percentage fixed width and gap",
                       test_mixed_percentage_fixed_width_and_gap);
        ctest_describe("full width bordered item stability",
                       test_full_width_bordered_item_stability);
        ctest_describe("intrinsic percentage flex item",
                       test_intrinsic_percentage_item);
        ctest_describe("decorated intrinsic percentage flex item",
                       test_decorated_intrinsic_percentage_item);
        ctest_describe("minimum percentage flex item",
                       test_minimum_percentage_item);
        ctest_describe("percentage flex gap", test_percentage_gap);
        ctest_describe("decorated empty percentage flex item",
                       test_decorated_empty_percentage_item);
        ctest_describe("full percentage row wrap",
                       test_full_percentage_row_wrap);
        ctest_describe("percentage row totaling one hundred wrap",
                       test_percentage_row_wrap);
        ctest_describe("fixed before full percentage wrap",
                       test_fixed_before_full_percentage_wrap);
        ctest_describe("decorated percentage before percentage wrap",
                       test_decorated_percentage_before_percentage_wrap);
        ctest_describe("fixed wrap content contributions",
                       test_fixed_wrap_content_contributions);

#ifndef LCUI_FLEX_LAYOUT_PERCENTAGE_WRAP_PREVIEW
        lcui_destroy();
#endif
}
