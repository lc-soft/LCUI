/*
 * tests/integration/test_sidebar.c: -- Sidebar widget tests
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

static void emit_click(ui_widget_t *target)
{
        ui_event_t e;

        ui_event_init(&e, "click");
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static void emit_widget_event(ui_widget_t *target, const char *name)
{
        ui_event_t e = { 0 };

        ui_event_init(&e, name);
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static void hover_at(float x, float y)
{
        ui_event_t e = { 0 };

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = x;
        e.mouse.y = y;
        ui_dispatch_event(&e);
        ui_update();
        ui_update();
}

static void layout_sidebar(ui_widget_t *provider, ui_widget_t *sidebar)
{
        ui_widget_append(provider, sidebar);
        ui_widget_append(ui_root(), provider);
        ui_widget_resize(ui_root(), 800, 600);
        ui_update();
}

static int provider_changes;

static void on_provider_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        provider_changes++;
}

static void test_provider_state(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();

        provider_changes = 0;
        ui_widget_on(provider, "open-change", on_provider_change, NULL);

        ctest_equal_bool("should default to open",
                         ui_sidebar_provider_get_open(provider), true);
        ui_sidebar_provider_set_open(provider, false);
        ctest_equal_bool("should become closed",
                         ui_sidebar_provider_get_open(provider), false);
        ctest_equal_bool("should add the collapsed status",
                         ui_widget_has_status(provider, "collapsed"), true);
        ui_sidebar_provider_toggle(provider);
        ctest_equal_bool("toggle should reopen the provider",
                         ui_sidebar_provider_get_open(provider), true);
        ctest_equal_bool("toggle should remove the collapsed status",
                         ui_widget_has_status(provider, "collapsed"), false);
        ctest_equal_int("should emit open-change for each transition",
                        provider_changes, 2);

        ui_widget_destroy(provider);
}

static void test_trigger(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *trigger = ui_create_sidebar_trigger();

        layout_sidebar(provider, sidebar);
        ui_widget_append(provider, trigger);
        ui_update();

        emit_click(trigger);
        ctest_equal_bool("trigger should close the provider",
                         ui_sidebar_provider_get_open(provider), false);
        emit_click(trigger);
        ctest_equal_bool("trigger should reopen the provider",
                         ui_sidebar_provider_get_open(provider), true);

        ui_widget_destroy(provider);
}

static void test_no_provider(void)
{
        ui_widget_t *trigger = ui_create_sidebar_trigger();

        ui_widget_append(ui_root(), trigger);
        ui_update();
        emit_click(trigger);
        ctest_equal_bool("trigger should be harmless without a provider",
                         ui_widget_has_status(trigger, "collapsed"), false);

        ui_widget_destroy(trigger);
}

static void test_nearest_provider(void)
{
        ui_widget_t *outer = ui_create_sidebar_provider();
        ui_widget_t *inner = ui_create_sidebar_provider();
        ui_widget_t *trigger = ui_create_sidebar_trigger();

        ui_widget_append(outer, inner);
        ui_widget_append(inner, trigger);
        ui_widget_append(ui_root(), outer);
        ui_update();
        ui_sidebar_provider_set_open(outer, false);

        emit_click(trigger);
        ctest_equal_bool("trigger should close the nearest provider",
                         ui_sidebar_provider_get_open(inner), false);
        ctest_equal_bool("trigger should leave the outer provider closed",
                         ui_sidebar_provider_get_open(outer), false);

        ui_widget_destroy(outer);
}

static void test_collapse_modes(void)
{
        ui_sidebar_collapsible_t modes[] = {
                UI_SIDEBAR_COLLAPSIBLE_OFFCANVAS,
                UI_SIDEBAR_COLLAPSIBLE_ICON,
                UI_SIDEBAR_COLLAPSIBLE_NONE,
        };
        float expected_widths[] = { 0, 48, 240 };
        int i;

        for (i = 0; i < 3; ++i) {
                ui_widget_t *provider = ui_create_sidebar_provider();
                ui_widget_t *sidebar = ui_create_sidebar();

                ui_sidebar_set_collapsible(sidebar, modes[i]);
                ctest_equal_int("sidebar should retain its collapse mode",
                                ui_sidebar_get_collapsible(sidebar), modes[i]);
                layout_sidebar(provider, sidebar);
                ui_sidebar_provider_set_open(provider, false);
                ui_update();
                ctest_equal_float(
                    "sidebar should use the expected collapsed width",
                    sidebar->border_box.width, expected_widths[i]);
                ui_widget_destroy(provider);
        }
}

static void test_rail(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *rail = ui_create_sidebar_rail();

        layout_sidebar(provider, sidebar);
        ui_widget_append(sidebar, rail);
        ui_update();

        emit_click(rail);
        ctest_equal_bool("rail should close the provider",
                         ui_sidebar_provider_get_open(provider), false);

        ui_widget_destroy(provider);
}

static void test_side(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();

        layout_sidebar(provider, sidebar);
        ui_sidebar_set_side(sidebar, UI_SIDEBAR_SIDE_RIGHT);
        ui_update();
        ctest_equal_bool("right side should add the right status",
                         ui_widget_has_status(sidebar, "right"), true);
        ctest_equal_bool("right side should use a left border",
                         sidebar->computed_style.border_left_width > 0, true);

        ui_sidebar_set_side(sidebar, UI_SIDEBAR_SIDE_LEFT);
        ui_update();
        ctest_equal_bool("left side should remove the right status",
                         ui_widget_has_status(sidebar, "right"), false);

        ui_widget_destroy(provider);
}

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

static void test_trigger_icon(void)
{
        ui_widget_t *trigger = ui_create_widget("sidebar-trigger");
        ui_widget_t *icon = find_child_type(trigger, "icon");

        ctest_equal_bool("trigger should create a panel-left icon",
                         icon != NULL, true);

        ui_widget_destroy(trigger);
}

static void test_theme_surface(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();

        layout_sidebar(provider, sidebar);
        ctest_equal_uint("sidebar should use the sidebar background",
                         sidebar->computed_style.background_color,
                         css_color(255, 0xfa, 0xfa, 0xfa));

        ui_widget_destroy(provider);
}

static void test_menu_icon_width(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *menu = ui_create_sidebar_menu();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_fluent_icon("book-open");
        ui_widget_t *label = ui_create_widget("text");

        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);
        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(label, "sidebar-menu-label");
        ui_widget_append(button, icon);
        ui_widget_append(button, label);
        ui_widget_append(menu, button);
        ui_widget_append(sidebar, menu);
        layout_sidebar(provider, sidebar);
        ui_sidebar_provider_set_open(provider, false);
        ui_update();

        ctest_equal_float("collapsed menu icons should keep a 16px width",
                          icon->border_box.width, 16.0f);

        ui_widget_destroy(provider);
}

static void test_sidebar_tooltip(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *menu = ui_create_sidebar_menu();
        ui_widget_t *button = ui_create_button();

        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);
        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_set_attr(button, "data-sidebar-tooltip", "Playground");
        ui_widget_append(menu, button);
        ui_widget_append(sidebar, menu);
        layout_sidebar(provider, sidebar);
        ui_update();

        ctest_equal_bool("tooltip attribute should be absent when expanded",
                         ui_widget_get_attr(button, "data-tooltip") == NULL,
                         true);
        ui_sidebar_provider_set_open(provider, false);
        ctest_equal_str("tooltip attribute should appear when collapsed",
                        ui_widget_get_attr(button, "data-tooltip"),
                        "Playground");
        ctest_equal_str("tooltip should be placed on the right side",
                        ui_widget_get_attr(button, "data-tooltip-side"),
                        "right");
        ui_sidebar_provider_set_open(provider, true);
        ctest_equal_bool("tooltip attribute should be removed when reopened",
                         ui_widget_get_attr(button, "data-tooltip") == NULL,
                         true);
        ctest_equal_bool(
            "tooltip side should be removed when reopened",
            ui_widget_get_attr(button, "data-tooltip-side") == NULL, true);

        ui_widget_destroy(provider);
}

static void test_profile_geometry(void)
{
        ui_widget_t *team = ui_create_widget(NULL);
        ui_widget_t *avatar = ui_create_widget(NULL);

        ui_widget_add_class(team, "sidebar-team-logo");
        ui_widget_add_class(avatar, "sidebar-avatar");
        ui_widget_append(ui_root(), team);
        ui_widget_append(ui_root(), avatar);
        ui_update();

        ctest_equal_float("team logo should be 32px wide",
                          team->border_box.width, 32.0f);
        ctest_equal_float("team logo should be 32px high",
                          team->border_box.height, 32.0f);
        ctest_equal_float("user avatar should be 32px wide",
                          avatar->border_box.width, 32.0f);
        ctest_equal_float("user avatar should be 32px high",
                          avatar->border_box.height, 32.0f);

        ui_widget_destroy(team);
        ui_widget_destroy(avatar);
}

static void test_structure(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *header = ui_create_sidebar_header();
        ui_widget_t *content = ui_create_sidebar_content();
        ui_widget_t *group = ui_create_sidebar_group();
        ui_widget_t *label = ui_create_sidebar_group_label();
        ui_widget_t *group_action = ui_create_sidebar_group_action();
        ui_widget_t *group_content = ui_create_sidebar_group_content();
        ui_widget_t *menu = ui_create_sidebar_menu();
        ui_widget_t *item = ui_create_sidebar_menu_item();
        ui_widget_t *action = ui_create_sidebar_menu_action();
        ui_widget_t *sub = ui_create_sidebar_menu_sub();
        ui_widget_t *sub_item = ui_create_sidebar_menu_sub_item();
        ui_widget_t *badge = ui_create_sidebar_menu_badge();

        ui_widget_append(group, label);
        ui_widget_append(group, group_action);
        ui_widget_append(group, group_content);
        ui_widget_append(group_content, menu);
        ui_widget_append(menu, item);
        ui_widget_append(item, action);
        ui_widget_append(item, sub);
        ui_widget_append(sub, sub_item);
        ui_widget_append(item, badge);
        ui_widget_append(content, group);
        ui_widget_append(sidebar, header);
        ui_widget_append(sidebar, content);
        ui_widget_append(provider, sidebar);
        ui_widget_append(ui_root(), provider);
        ui_update();

        ctest_equal_bool("provider should contain the sidebar",
                         find_child_type(provider, "sidebar") == sidebar, true);
        ctest_equal_bool("sidebar should contain the header",
                         find_child_type(sidebar, "sidebar-header") == header,
                         true);
        ctest_equal_bool("sidebar should contain the content",
                         find_child_type(sidebar, "sidebar-content") == content,
                         true);
        ctest_equal_bool("content should contain the group",
                         find_child_type(content, "sidebar-group") == group,
                         true);
        ctest_equal_bool("group should contain the label",
                         find_child_type(group, "sidebar-group-label") == label,
                         true);
        ctest_equal_bool(
            "group should contain the group action",
            find_child_type(group, "sidebar-group-action") == group_action,
            true);
        ctest_equal_bool(
            "group should contain the group content",
            find_child_type(group, "sidebar-group-content") == group_content,
            true);
        ctest_equal_bool("menu should contain the item",
                         find_child_type(menu, "sidebar-menu-item") == item,
                         true);
        ctest_equal_bool("item should contain the menu action",
                         find_child_type(item, "sidebar-menu-action") == action,
                         true);
        ctest_equal_bool("item should contain the sub menu",
                         find_child_type(item, "sidebar-menu-sub") == sub,
                         true);
        ctest_equal_bool(
            "sub menu should contain the sub item",
            find_child_type(sub, "sidebar-menu-sub-item") == sub_item, true);
        ctest_equal_bool("item should contain the badge",
                         find_child_type(item, "sidebar-menu-badge") == badge,
                         true);

        ui_widget_destroy(provider);
}

static void test_group_collapsible(void)
{
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *group = ui_create_sidebar_group();
        ui_widget_t *label = ui_create_sidebar_group_label();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *group_content = ui_create_sidebar_group_content();
        ui_widget_t *content = ui_create_collapsible_content();

        ui_widget_set_attr(label, "data-collapsible-trigger", "true");
        ui_text_set_content(text, "Help");
        ui_widget_append(label, text);
        ui_widget_append(group, label);
        ui_widget_append(content, group_content);
        ui_widget_append(group, content);
        ui_widget_append(collapsible, group);
        ui_widget_append(ui_root(), collapsible);
        ui_update();

        emit_click(text);
        ui_update();
        ctest_equal_bool("group label should open the collapsible",
                         ui_collapsible_get_open(collapsible), true);

        ui_widget_destroy(collapsible);
}

static void test_menu_button_class(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *anchor = ui_create_widget("a");
        ui_widget_t *anchor_label = ui_create_widget("text");
        css_color_value_t before;

        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(label, "sidebar-menu-label");
        ui_text_set_content(label, "Home");
        ui_widget_append(button, label);
        ui_widget_append(ui_root(), button);
        ui_widget_add_class(anchor, "sidebar-menu-button");
        ui_widget_add_class(anchor_label, "sidebar-menu-label");
        ui_text_set_content(anchor_label, "Inbox");
        ui_widget_append(anchor, anchor_label);
        ui_widget_append(ui_root(), anchor);
        ui_update();

        ctest_equal_bool("button should retain the menu class",
                         ui_widget_has_class(button, "sidebar-menu-button"),
                         true);
        ctest_equal_float("menu button should use 12.8px text",
                          button->computed_style.font_size, 12.8f);
        ctest_equal_bool("anchor should retain the menu class",
                         ui_widget_has_class(anchor, "sidebar-menu-button"),
                         true);

        before = anchor->computed_style.background_color;
        ui_widget_add_class(anchor, "active");
        ui_update();
        ctest_equal_bool("active menu item should change its background",
                         anchor->computed_style.background_color != before,
                         true);

        ui_widget_destroy(button);
        ui_widget_destroy(anchor);
}

static void test_icon_labels_hidden(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *menu = ui_create_sidebar_menu();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *label = ui_create_widget("text");

        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);
        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(label, "sidebar-menu-label");
        ui_text_set_content(label, "Home");
        ui_widget_append(button, label);
        ui_widget_append(menu, button);
        ui_widget_append(sidebar, menu);
        ui_widget_append(provider, sidebar);
        ui_widget_append(ui_root(), provider);
        ui_widget_resize(ui_root(), 800, 600);
        ui_update();

        ui_sidebar_provider_set_open(provider, false);
        ui_update();
        ctest_equal_bool("icon mode should hide menu label text",
                         ui_widget_is_visible(label), false);
        ctest_equal_bool("icon mode should retain the menu button",
                         ui_widget_is_visible(button), true);

        ui_widget_destroy(provider);
}

static void test_group_label_has_no_hover(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *group = ui_create_sidebar_group();
        ui_widget_t *label = ui_create_sidebar_group_label();

        ui_widget_append(group, label);
        ui_widget_append(sidebar, group);
        layout_sidebar(provider, sidebar);
        ui_widget_add_status(label, "hover");
        ui_update();

        ctest_equal_bool("group label should not change background on hover",
                         label->computed_style.background_color !=
                             css_color(255, 0xf5, 0xf5, 0xf5),
                         true);

        ui_widget_destroy(provider);
}

static void test_projects_hidden_in_icon_mode(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *projects = ui_create_sidebar_group();

        ui_widget_add_class(projects, "sidebar-projects");
        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);
        ui_widget_append(sidebar, projects);
        layout_sidebar(provider, sidebar);
        ui_sidebar_provider_set_open(provider, false);
        ui_update();

        ctest_equal_bool("projects should be hidden in icon mode",
                         ui_widget_is_visible(projects), false);

        ui_widget_destroy(provider);
}

static void test_profile_compact_in_icon_mode(void)
{
        ui_widget_t *provider = ui_create_sidebar_provider();
        ui_widget_t *sidebar = ui_create_sidebar();
        ui_widget_t *team = ui_create_widget("button");
        ui_widget_t *user = ui_create_widget("button");

        ui_widget_add_class(team, "sidebar-menu-button");
        ui_widget_add_class(team, "sidebar-team");
        ui_widget_add_class(user, "sidebar-menu-button");
        ui_widget_add_class(user, "sidebar-user");
        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);
        ui_widget_append(sidebar, team);
        ui_widget_append(sidebar, user);
        layout_sidebar(provider, sidebar);
        ui_sidebar_provider_set_open(provider, false);
        ui_update();

        ctest_equal_float("team trigger should become 32px wide",
                          team->border_box.width, 32.0f);
        ctest_equal_float("team trigger should become 32px high",
                          team->border_box.height, 32.0f);
        ctest_equal_float("user trigger should become 32px wide",
                          user->border_box.width, 32.0f);

        ui_widget_destroy(provider);
}

static void test_content_composition(void)
{
        ui_widget_t *content = ui_create_sidebar_content();
        ui_widget_t *scroll = ui_create_scrollarea();
        ui_widget_t *scroll_content = ui_create_scrollarea_content();

        ui_widget_append(scroll, scroll_content);
        ui_widget_append(content, scroll);
        ui_widget_append(ui_root(), content);
        ui_update();

        ctest_equal_bool("content should own the scroll area",
                         find_child_type(content, "scrollarea") == scroll,
                         true);
        ctest_equal_bool(
            "scroll area should own its content",
            find_child_type(scroll, "scrollarea-content") == scroll_content,
            true);

        ui_widget_destroy(content);
}

static void test_project_action_layout(void)
{
        ui_widget_t *item = ui_create_sidebar_menu_item();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *action = ui_create_sidebar_menu_action();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        int action_x;
        int action_y;

        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(menu, "sidebar-menu-action-menu");
        ui_widget_add_class(action, "sidebar-menu-action-trigger");
        ui_widget_set_attr(action, "data-dropdown-menu-trigger", "true");
        ui_widget_append(action, ui_create_fluent_icon("more-horizontal"));
        ui_widget_append(content, ui_create_dropdown_menu_item());
        ui_widget_append(menu, action);
        ui_widget_append(menu, content);
        ui_widget_append(item, button);
        ui_widget_append(item, menu);
        ui_widget_append(ui_root(), item);
        ui_widget_resize(ui_root(), 800, 600);
        ui_update();

        action_x = (int)(menu->border_box.x + action->border_box.x);
        action_y = (int)(item->border_box.y + menu->border_box.y +
                         action->border_box.y);
        ctest_equal_bool(
            "menu wrapper should have a paint box",
            menu->border_box.width == 20.0f && menu->border_box.height == 20.0f,
            true);
        ctest_equal_bool("menu wrapper should not push the button down",
                         item->border_box.height == button->border_box.height,
                         true);
        ctest_equal_int("action should sit at the item right edge", action_x,
                        (int)item->border_box.width - 24);
        ctest_equal_int("action should align with the item top", action_y, 5);
        ctest_equal_bool("action should start transparent",
                         action->computed_style.opacity == 0.0f, true);
        ctest_equal_bool("hidden action should ignore pointer events",
                         action->computed_style.type_bits.pointer_events ==
                             CSS_POINTER_EVENTS_NONE,
                         true);

        hover_at(400, 15);
        ctest_equal_bool("hovering the row should add the item hover status",
                         ui_widget_has_status(item, "hover"), true);
        ctest_equal_bool("hover should make the action opaque",
                         action->computed_style.opacity == 1.0f, true);
        ctest_equal_bool("visible action should accept pointer events",
                         action->computed_style.type_bits.pointer_events ==
                             CSS_POINTER_EVENTS_AUTO,
                         true);
        ctest_equal_int("hover should not move the action",
                        (int)(menu->border_box.x + action->border_box.x),
                        action_x);

        emit_click(action);
        ui_update();
        ctest_equal_bool("clicking the action should open the menu",
                         ui_dropdown_menu_is_open(menu), true);
        hover_at(700, 400);
        ctest_equal_bool("leaving the row should drop the hover status",
                         ui_widget_has_status(item, "hover"), false);
        ctest_equal_bool("an open menu should keep the action opaque",
                         action->computed_style.opacity == 1.0f, true);
        emit_click(action);
        ui_update();
        ctest_equal_bool("closing the menu should hide the action again",
                         action->computed_style.opacity == 0.0f, true);

        ui_widget_destroy(item);
        ui_clear_trash();
}

static void test_menu_action_visibility(void)
{
        ui_widget_t *item = ui_create_sidebar_menu_item();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *action = ui_create_sidebar_menu_action();

        ui_widget_add_class(button, "sidebar-menu-button");
        ui_widget_add_class(action, "sidebar-menu-action-trigger");
        ui_widget_set_attr(action, "data-dropdown-menu-trigger", "true");
        ui_widget_append(action, ui_create_fluent_icon("more-horizontal"));
        ui_widget_append(item, button);
        ui_widget_append(item, action);
        ui_widget_append(ui_root(), item);
        ui_update();

        ctest_equal_bool("action trigger should start transparent",
                         action->computed_style.opacity == 0.0f, true);
        ui_widget_add_status(item, "hover");
        ui_update();
        ctest_equal_bool("hover should make the action trigger opaque",
                         action->computed_style.opacity == 1.0f, true);
        ui_widget_remove_status(item, "hover");
        ui_update();
        ctest_equal_bool("removing hover should hide the action trigger",
                         action->computed_style.opacity == 0.0f, true);

        emit_widget_event(button, "focus");
        ui_update();
        ctest_equal_bool("focus should add the item focus status",
                         ui_widget_has_status(item, "focus"), true);
        ctest_equal_bool("focus should make the action trigger opaque",
                         action->computed_style.opacity == 1.0f, true);
        emit_widget_event(button, "blur");
        ui_update();
        ctest_equal_bool("blur should remove the item focus status",
                         ui_widget_has_status(item, "focus"), false);
        ctest_equal_bool("blur should hide the action trigger",
                         action->computed_style.opacity == 0.0f, true);

        ui_widget_destroy(item);
        ui_clear_trash();
}

void test_sidebar(void)
{
        lcui_init();

        ctest_describe("sidebar provider", test_provider_state);
        ctest_describe("sidebar trigger", test_trigger);
        ctest_describe("sidebar without provider", test_no_provider);
        ctest_describe("sidebar nearest provider", test_nearest_provider);
        ctest_describe("sidebar collapse modes", test_collapse_modes);
        ctest_describe("sidebar rail", test_rail);
        ctest_describe("sidebar side", test_side);
        ctest_describe("sidebar trigger icon", test_trigger_icon);
        ctest_describe("sidebar theme surface", test_theme_surface);
        ctest_describe("sidebar menu icon width", test_menu_icon_width);
        ctest_describe("sidebar tooltip", test_sidebar_tooltip);
        ctest_describe("sidebar profile geometry", test_profile_geometry);
        ctest_describe("sidebar group label hover",
                       test_group_label_has_no_hover);
        ctest_describe("sidebar projects visibility",
                       test_projects_hidden_in_icon_mode);
        ctest_describe("sidebar profile compact",
                       test_profile_compact_in_icon_mode);
        ctest_describe("sidebar structure", test_structure);
        ctest_describe("sidebar group collapsible", test_group_collapsible);
        ctest_describe("sidebar menu button class", test_menu_button_class);
        ctest_describe("sidebar icon labels", test_icon_labels_hidden);
        ctest_describe("sidebar content composition", test_content_composition);
        ctest_describe("sidebar project action layout",
                       test_project_action_layout);
        ctest_describe("sidebar menu action visibility",
                       test_menu_action_visibility);

        lcui_destroy();
}
