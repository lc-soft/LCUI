/*
 * tests/integration/test_dropdown_menu.c: -- DropdownMenu widget tests
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

typedef struct dropdown_fixture {
        ui_widget_t *menu;
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *portal;
} dropdown_fixture_t;

static int g_select_count;
static int g_checked_count;
static int g_value_count;
static ui_widget_t *g_event_item;

static void on_select(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_select_count;
        g_event_item = arg;
}

static void on_checked_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_checked_count;
        g_event_item = arg;
}

static void on_value_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_value_count;
        g_event_item = arg;
}

static void reset_events(void)
{
        g_select_count = 0;
        g_checked_count = 0;
        g_value_count = 0;
        g_event_item = NULL;
}

static void settle(void)
{
        ui_update();
        ui_update();
        ui_update();
}

static void click_widget(ui_widget_t *w)
{
        ui_event_t event;

        ui_event_init(&event, "click");
        ui_widget_emit_event(w, event, NULL);
}

static void emit_keydown(ui_widget_t *target, int code)
{
        ui_event_t event = { 0 };

        if (!target) {
                target = ui_root();
        }
        ui_event_init(&event, "keydown");
        event.key.code = (unsigned char)code;
        ui_widget_emit_event(target, event, NULL);
}

static ui_widget_t *find_by_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, type)) {
                        return child;
                }
                found = find_by_type(child, type);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static int count_by_type(ui_widget_t *w, const char *type)
{
        int count = ui_check_widget_type(w, type) ? 1 : 0;
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                count += count_by_type(child, type);
        }
        return count;
}

static bool has_indicator(ui_widget_t *item)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(item, i)); ++i) {
                if (ui_widget_has_class(child, "dropdown-menu-indicator")) {
                        return true;
                }
        }
        return false;
}

static void build_fixture(dropdown_fixture_t *fx)
{
        fx->menu = ui_create_dropdown_menu();
        fx->trigger = ui_create_dropdown_menu_trigger();
        fx->content = ui_create_dropdown_menu_content();
        ui_widget_set_attr(fx->trigger, "data-test", "trigger");
        ui_widget_append(fx->trigger, ui_create_widget("text"));
        ui_widget_append(fx->menu, fx->trigger);
        ui_widget_append(fx->menu, fx->content);
        ui_root_append(fx->menu);
        settle();
        fx->portal = find_by_type(fx->menu, "portal");
}

static ui_widget_t *append_item(ui_widget_t *content, const char *text)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();
        ui_widget_t *label = ui_create_widget("text");

        ui_text_set_content(label, text);
        ui_widget_append(item, label);
        ui_widget_append(content, item);
        return item;
}

static ui_widget_t *append_checkbox(ui_widget_t *content, const char *text)
{
        ui_widget_t *item = ui_create_dropdown_menu_checkbox_item();
        ui_widget_t *label = ui_create_widget("text");

        ui_text_set_content(label, text);
        ui_widget_append(item, label);
        ui_widget_append(content, item);
        return item;
}

static void destroy_fixture(dropdown_fixture_t *fx)
{
        ui_widget_destroy(fx->menu);
        ui_clear_trash();
}

static void test_default_state(void)
{
        dropdown_fixture_t fx;

        build_fixture(&fx);
        ctest_equal_bool("should start closed",
                         ui_dropdown_menu_is_open(fx.menu), false);
        ctest_equal_bool("should add the closed status",
                         ui_widget_has_status(fx.menu, "closed"), true);
        ctest_equal_bool("should not add the open status",
                         ui_widget_has_status(fx.menu, "open"), false);
        ctest_equal_bool("content should start hidden",
                         ui_widget_is_visible(fx.content), false);

        destroy_fixture(&fx);
}

static void test_default_open(void)
{
        dropdown_fixture_t fx;

        build_fixture(&fx);
        ui_widget_set_attr(fx.menu, "default-open", "true");
        settle();
        ctest_equal_bool("default-open should open the menu",
                         ui_dropdown_menu_is_open(fx.menu), true);
        ctest_equal_bool("default-open should add the open status",
                         ui_widget_has_status(fx.menu, "open"), true);
        ctest_equal_bool("default-open should hide the closed status",
                         ui_widget_has_status(fx.menu, "closed"), false);

        destroy_fixture(&fx);
}

static void test_open_close_api(void)
{
        dropdown_fixture_t fx;

        build_fixture(&fx);
        ctest_equal_bool("trigger and content should be bound",
                         fx.portal != NULL, true);
        ctest_equal_bool("content should be reparented to the portal root",
                         fx.content->parent == ui_get_portal_root(), true);

        ui_dropdown_menu_open(fx.menu);
        settle();
        ctest_equal_bool("open should set state",
                         ui_dropdown_menu_is_open(fx.menu), true);
        ctest_equal_bool("open should show the content",
                         ui_widget_is_visible(fx.content), true);
        ctest_equal_str("content should open below the trigger",
                        ui_widget_get_attr(fx.portal, "side"), "bottom");
        ctest_equal_str("content should align to the trigger start",
                        ui_widget_get_attr(fx.portal, "align"), "start");

        ui_dropdown_menu_close(fx.menu);
        settle();
        ctest_equal_bool("close should set state",
                         ui_dropdown_menu_is_open(fx.menu), false);
        ctest_equal_bool("close should hide the content",
                         ui_widget_is_visible(fx.content), false);

        ui_dropdown_menu_set_open(fx.menu, true);
        settle();
        ctest_equal_bool("set_open should open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        destroy_fixture(&fx);
}

static void test_match_trigger_width(void)
{
        dropdown_fixture_t fx;

        build_fixture(&fx);
        ui_widget_set_style_string(fx.trigger, "width", "320px");
        ui_widget_set_attr(fx.menu, "match-trigger-width", "true");
        append_item(fx.content, "A menu item");
        settle();
        ui_dropdown_menu_open(fx.menu);
        settle();

        ctest_equal_float("menu content should match the trigger width",
                          fx.content->border_box.width,
                          fx.trigger->border_box.width);

        destroy_fixture(&fx);
}

static void test_trigger_click(void)
{
        dropdown_fixture_t fx;

        build_fixture(&fx);
        click_widget(fx.trigger);
        settle();
        ctest_equal_bool("first trigger click should open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        click_widget(fx.trigger);
        settle();
        ctest_equal_bool("second trigger click should close",
                         ui_dropdown_menu_is_open(fx.menu), false);

        destroy_fixture(&fx);
}

static void test_outside_and_escape(void)
{
        dropdown_fixture_t fx;
        ui_event_t event;

        build_fixture(&fx);
        ui_dropdown_menu_open(fx.menu);
        settle();
        ui_event_init(&event, "portal-outside");
        ui_widget_emit_event(fx.portal, event, NULL);
        ctest_equal_bool("portal-outside should close",
                         ui_dropdown_menu_is_open(fx.menu), false);

        ui_dropdown_menu_open(fx.menu);
        settle();
        emit_keydown(ui_root(), KEY_ESCAPE);
        ctest_equal_bool("escape should close",
                         ui_dropdown_menu_is_open(fx.menu), false);

        destroy_fixture(&fx);
}

static void test_item_select(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *item;

        build_fixture(&fx);
        item = append_item(fx.content, "Profile");
        reset_events();
        ui_widget_on(fx.menu, "select", on_select, NULL);
        ui_dropdown_menu_open(fx.menu);
        settle();

        click_widget(item);
        ctest_equal_int("item click should emit select", g_select_count, 1);
        ctest_equal_bool("select should report the item", g_event_item == item,
                         true);
        ctest_equal_bool("item click should close the menu",
                         ui_dropdown_menu_is_open(fx.menu), false);

        destroy_fixture(&fx);
}

static void test_disabled_item(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *item;

        build_fixture(&fx);
        item = append_item(fx.content, "Delete");
        reset_events();
        ui_widget_on(fx.menu, "select", on_select, NULL);
        ui_widget_set_disabled(item, true);
        ui_dropdown_menu_open(fx.menu);
        settle();

        ctest_equal_bool("disabled item should carry the disabled status",
                         ui_widget_has_status(item, "disabled"), true);
        click_widget(item);
        ctest_equal_int("disabled item should not emit select", g_select_count,
                        0);
        ctest_equal_bool("disabled item should keep the menu open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        destroy_fixture(&fx);
}

static void test_checkbox_item(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *item;

        build_fixture(&fx);
        item = append_checkbox(fx.content, "Show bookmarks");
        reset_events();
        ui_widget_on(fx.menu, "checked-change", on_checked_change, NULL);
        ui_dropdown_menu_open(fx.menu);
        settle();

        ctest_equal_bool("checkbox item should auto-insert an indicator",
                         has_indicator(item), true);
        click_widget(item);
        ctest_equal_int("checkbox click should emit checked-change",
                        g_checked_count, 1);
        ctest_equal_str("checkbox should become checked",
                        ui_widget_get_attr(item, "checked"), "true");
        ctest_equal_bool("checked checkbox should carry the status",
                         ui_widget_has_status(item, "checked"), true);
        ctest_equal_bool("checkbox click should keep the menu open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        click_widget(item);
        ctest_equal_str("second click should uncheck",
                        ui_widget_get_attr(item, "checked"), "false");
        ctest_equal_bool("unchecked checkbox should drop the status",
                         ui_widget_has_status(item, "checked"), false);
        ctest_equal_int("second click should emit again", g_checked_count, 2);
        ctest_equal_bool("checkbox toggling should keep the menu open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        destroy_fixture(&fx);
}

static void test_radio_group(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *group;
        ui_widget_t *first;
        ui_widget_t *second;

        build_fixture(&fx);
        group = ui_create_dropdown_menu_radio_group();
        first = ui_create_dropdown_menu_radio_item();
        second = ui_create_dropdown_menu_radio_item();
        ui_widget_set_attr(first, "value", "top");
        ui_widget_set_attr(second, "value", "bottom");
        ui_widget_append(group, first);
        ui_widget_append(group, second);
        ui_widget_append(fx.content, group);
        ui_widget_set_attr(group, "value", "top");
        ui_widget_append(group, ui_create_dropdown_menu_label());
        settle();

        ctest_equal_bool("group value should check the matching item",
                         ui_widget_has_status(first, "checked"), true);
        ctest_equal_bool("group value should leave others unchecked",
                         ui_widget_has_status(second, "checked"), false);

        reset_events();
        ui_widget_on(fx.menu, "value-change", on_value_change, NULL);
        ui_dropdown_menu_open(fx.menu);
        settle();
        ctest_equal_bool("radio item should auto-insert an indicator",
                         has_indicator(first) && has_indicator(second), true);
        click_widget(second);
        ctest_equal_str("radio click should update the group value",
                        ui_widget_get_attr(group, "value"), "bottom");
        ctest_equal_bool("new radio should be checked",
                         ui_widget_has_status(second, "checked"), true);
        ctest_equal_bool("old radio should be unchecked",
                         ui_widget_has_status(first, "checked"), false);
        ctest_equal_int("radio click should emit value-change", g_value_count,
                        1);
        ctest_equal_bool("value-change should report the item",
                         g_event_item == second, true);
        ctest_equal_bool("radio click should keep the menu open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        click_widget(first);
        ctest_equal_str("second radio click should switch the value",
                        ui_widget_get_attr(group, "value"), "top");
        ctest_equal_bool("switching radio should keep the menu open",
                         ui_dropdown_menu_is_open(fx.menu), true);

        destroy_fixture(&fx);
}

static void test_item_variant(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *item;

        build_fixture(&fx);
        item = append_item(fx.content, "Delete");
        ui_dropdown_menu_item_set_variant(item,
                                          UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);
        ctest_equal_bool("destructive variant should add the status",
                         ui_widget_has_status(item, "destructive"), true);
        ui_dropdown_menu_item_set_variant(item, UI_DROPDOWN_MENU_ITEM_DEFAULT);
        ctest_equal_bool("default variant should drop the status",
                         ui_widget_has_status(item, "destructive"), false);

        destroy_fixture(&fx);
}

static void test_keyboard_navigation(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *first;
        ui_widget_t *second;
        ui_widget_t *third;

        build_fixture(&fx);
        first = append_item(fx.content, "One");
        second = append_item(fx.content, "Two");
        third = append_item(fx.content, "Three");
        reset_events();
        ui_widget_on(fx.menu, "select", on_select, NULL);
        ui_dropdown_menu_open(fx.menu);
        settle();

        ctest_equal_bool("open should not highlight an item",
                         ui_widget_has_status(first, "highlighted"), false);
        ctest_equal_bool("open should focus the first item",
                         ui_get_focus_in(ui_root()) == first, true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_DOWN);
        ctest_equal_bool("down should highlight the first item",
                         ui_widget_has_status(first, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_DOWN);
        ctest_equal_bool("second down should highlight the next item",
                         ui_widget_has_status(second, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_END);
        ctest_equal_bool("end should highlight the last item",
                         ui_widget_has_status(third, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_DOWN);
        ctest_equal_bool("down should wrap to the first item",
                         ui_widget_has_status(first, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_UP);
        ctest_equal_bool("up should wrap to the last item",
                         ui_widget_has_status(third, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_HOME);
        ctest_equal_bool("home should highlight the first item",
                         ui_widget_has_status(first, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_DOWN);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_ENTER);
        ctest_equal_int("enter should activate the highlighted item",
                        g_select_count, 1);
        ctest_equal_bool("enter activation should report the item",
                         g_event_item == second, true);
        ctest_equal_bool("enter activation should close the menu",
                         ui_dropdown_menu_is_open(fx.menu), false);

        destroy_fixture(&fx);
}

static void test_typeahead(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *profile;
        ui_widget_t *settings;
        ui_widget_t *support;

        build_fixture(&fx);
        profile = append_item(fx.content, "Profile");
        append_item(fx.content, "Billing");
        settings = append_item(fx.content, "Settings");
        support = append_item(fx.content, "Support");
        reset_events();
        ui_dropdown_menu_open(fx.menu);
        settle();

        emit_keydown(ui_get_focus_in(ui_root()), KEY_S);
        ctest_equal_bool("typing s should highlight Settings",
                         ui_widget_has_status(settings, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_S);
        ctest_equal_bool("typing s again should cycle to Support",
                         ui_widget_has_status(support, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_P);
        ctest_equal_bool("typing p after s should keep the current item",
                         ui_widget_has_status(support, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_P);
        ctest_equal_bool("typing p again should highlight Profile",
                         ui_widget_has_status(profile, "highlighted"), true);
        ctest_equal_int("typeahead should not activate an item", g_select_count,
                        0);

        destroy_fixture(&fx);
}

static void test_keyboard_skips_disabled(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *first;
        ui_widget_t *second;
        ui_widget_t *third;

        build_fixture(&fx);
        first = append_item(fx.content, "One");
        second = append_item(fx.content, "Two");
        third = append_item(fx.content, "Three");
        ui_widget_set_disabled(second, true);
        ui_dropdown_menu_open(fx.menu);
        settle();
        emit_keydown(ui_get_focus_in(ui_root()), KEY_DOWN);
        ctest_equal_bool("down should select the first enabled item",
                         ui_widget_has_status(first, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_DOWN);
        ctest_equal_bool("down should skip disabled items",
                         ui_widget_has_status(third, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_UP);
        ctest_equal_bool("up should skip disabled items",
                         ui_widget_has_status(first, "highlighted"), true);

        destroy_fixture(&fx);
}

static void build_submenu(dropdown_fixture_t *fx, ui_widget_t **sub,
                          ui_widget_t **sub_trigger, ui_widget_t **sub_content,
                          ui_widget_t **sub_item)
{
        ui_widget_t *group = ui_create_dropdown_menu_group();

        build_fixture(fx);
        *sub = ui_create_dropdown_menu_sub();
        *sub_trigger = ui_create_dropdown_menu_sub_trigger();
        *sub_content = ui_create_dropdown_menu_sub_content();
        *sub_item = ui_create_dropdown_menu_item();
        ui_widget_append(*sub_trigger, ui_create_widget("text"));
        ui_widget_append(*sub_content, *sub_item);
        ui_widget_append(*sub, *sub_trigger);
        ui_widget_append(*sub, *sub_content);
        ui_widget_append(group, *sub);
        ui_widget_append(fx->content, group);
        settle();
}

static void test_submenu(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *sub;
        ui_widget_t *sub_trigger;
        ui_widget_t *sub_content;
        ui_widget_t *sub_item;

        build_submenu(&fx, &sub, &sub_trigger, &sub_content, &sub_item);
        ctest_equal_bool("sub content should use the portal root",
                         sub_content->parent == ui_get_portal_root(), true);
        ui_dropdown_menu_open(fx.menu);
        settle();

        click_widget(sub_trigger);
        settle();
        ctest_equal_bool("sub trigger should open the submenu",
                         ui_dropdown_menu_is_open(sub), true);
        ctest_equal_bool("sub content should become visible",
                         ui_widget_is_visible(sub_content), true);
        ctest_equal_str("sub content should open to the right",
                        ui_widget_get_attr(find_by_type(sub, "portal"), "side"),
                        "right");

        reset_events();
        ui_widget_on(fx.menu, "select", on_select, NULL);
        click_widget(sub_item);
        ctest_equal_int("sub item should emit select", g_select_count, 1);
        ctest_equal_bool("sub item should close the root menu",
                         ui_dropdown_menu_is_open(fx.menu), false);
        ctest_equal_bool("sub item should close the submenu",
                         ui_dropdown_menu_is_open(sub), false);

        destroy_fixture(&fx);
}

static void test_submenu_keyboard(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *sub;
        ui_widget_t *sub_trigger;
        ui_widget_t *sub_content;
        ui_widget_t *sub_item;

        build_submenu(&fx, &sub, &sub_trigger, &sub_content, &sub_item);
        ui_dropdown_menu_open(fx.menu);
        settle();
        ctest_equal_bool("sub trigger should not be highlighted on open",
                         ui_widget_has_status(sub_trigger, "highlighted"),
                         false);
        ctest_equal_bool("open should focus the sub trigger",
                         ui_get_focus_in(ui_root()) == sub_trigger, true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_RIGHT);
        settle();
        ctest_equal_bool("right should open the submenu",
                         ui_dropdown_menu_is_open(sub), true);
        ctest_equal_bool("opened submenu should highlight its first item",
                         ui_widget_has_status(sub_item, "highlighted"), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_LEFT);
        ctest_equal_bool("left should close the submenu",
                         ui_dropdown_menu_is_open(sub), false);
        ctest_equal_bool("left should return focus to the sub trigger",
                         ui_widget_has_status(sub_trigger, "highlighted"),
                         true);

        destroy_fixture(&fx);
}

static void test_escape_closes_submenu_first(void)
{
        dropdown_fixture_t fx;
        ui_widget_t *sub;
        ui_widget_t *sub_trigger;
        ui_widget_t *sub_content;
        ui_widget_t *sub_item;

        build_submenu(&fx, &sub, &sub_trigger, &sub_content, &sub_item);
        ui_dropdown_menu_open(fx.menu);
        settle();
        click_widget(sub_trigger);
        settle();
        emit_keydown(ui_get_focus_in(ui_root()), KEY_ESCAPE);
        ctest_equal_bool("escape should close the submenu first",
                         ui_dropdown_menu_is_open(sub), false);
        ctest_equal_bool("escape should keep the root menu open",
                         ui_dropdown_menu_is_open(fx.menu), true);
        emit_keydown(ui_get_focus_in(ui_root()), KEY_ESCAPE);
        ctest_equal_bool("second escape should close the root menu",
                         ui_dropdown_menu_is_open(fx.menu), false);

        destroy_fixture(&fx);
}

static void test_only_one_menu_open(void)
{
        dropdown_fixture_t first;
        dropdown_fixture_t second;

        build_fixture(&first);
        build_fixture(&second);
        ui_dropdown_menu_open(first.menu);
        settle();
        ui_dropdown_menu_open(second.menu);
        settle();
        ctest_equal_bool("opening another menu should close the first",
                         ui_dropdown_menu_is_open(first.menu), false);
        ctest_equal_bool("the new menu should stay open",
                         ui_dropdown_menu_is_open(second.menu), true);

        destroy_fixture(&second);
        destroy_fixture(&first);
}

static void test_destroy_while_open(void)
{
        dropdown_fixture_t fx;

        build_fixture(&fx);
        ui_dropdown_menu_open(fx.menu);
        settle();
        ui_widget_destroy(fx.content);
        ui_clear_trash();
        ctest_equal_bool("destroying content should close the menu",
                         ui_dropdown_menu_is_open(fx.menu), false);
        ui_widget_destroy(fx.menu);
        ui_clear_trash();

        build_fixture(&fx);
        ui_dropdown_menu_open(fx.menu);
        settle();
        destroy_fixture(&fx);
}

static void test_data_trigger(void)
{
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *item = ui_create_dropdown_menu_item();

        ui_button_set_text(button, "Open");
        ui_widget_set_attr(button, "data-dropdown-menu-trigger", "true");
        ui_widget_append(content, item);
        ui_widget_append(menu, button);
        ui_widget_append(menu, content);
        ui_widget_append(ui_root(), menu);
        settle();

        ctest_equal_bool("data trigger should bind the menu",
                         ui_dropdown_menu_is_open(menu), false);
        click_widget(button);
        settle();
        ctest_equal_bool("data trigger should open the menu",
                         ui_dropdown_menu_is_open(menu), true);
        ctest_equal_bool("data trigger menu should show its content",
                         ui_widget_is_visible(content), true);

        ui_widget_destroy(menu);
        ui_clear_trash();
}

void test_dropdown_menu(void)
{
        lcui_init();

        ctest_describe("dropdown menu default state", test_default_state);
        ctest_describe("dropdown menu default open", test_default_open);
        ctest_describe("dropdown menu open close api", test_open_close_api);
        ctest_describe("dropdown menu trigger width", test_match_trigger_width);
        ctest_describe("dropdown menu trigger click", test_trigger_click);
        ctest_describe("dropdown menu data trigger", test_data_trigger);
        ctest_describe("dropdown menu outside and escape",
                       test_outside_and_escape);
        ctest_describe("dropdown menu item select", test_item_select);
        ctest_describe("dropdown menu disabled item", test_disabled_item);
        ctest_describe("dropdown menu checkbox item", test_checkbox_item);
        ctest_describe("dropdown menu radio group", test_radio_group);
        ctest_describe("dropdown menu item variant", test_item_variant);
        ctest_describe("dropdown menu keyboard navigation",
                       test_keyboard_navigation);
        ctest_describe("dropdown menu keyboard skips disabled",
                       test_keyboard_skips_disabled);
        ctest_describe("dropdown menu typeahead", test_typeahead);
        ctest_describe("dropdown menu submenu", test_submenu);
        ctest_describe("dropdown menu submenu keyboard", test_submenu_keyboard);
        ctest_describe("dropdown menu escape closes submenu first",
                       test_escape_closes_submenu_first);
        ctest_describe("dropdown menu only one open", test_only_one_menu_open);
        ctest_describe("dropdown menu destroy while open",
                       test_destroy_while_open);

        lcui_destroy();
}
