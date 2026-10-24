/*
 * tests/integration/test_toast.c: -- Toast widget tests
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

static int g_action_count;

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

static ui_widget_t *find_child_class(ui_widget_t *w, const char *class_name)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(w, index)); ++index) {
                if (ui_widget_has_class(child, class_name)) {
                        return child;
                }
                found = find_child_class(child, class_name);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static void close_toast(ui_widget_t *toast)
{
        ui_toast_close(toast);
        ui_update();
        ui_clear_trash();
}

static void test_icon_optional_layout(void)
{
        ui_toast_options_t basic = { 0 };
        ui_toast_options_t typed = { 0 };
        ui_widget_t *first;
        ui_widget_t *second;
        ui_widget_t *first_content;
        ui_widget_t *second_content;
        ui_widget_t *first_close;
        ui_widget_t *second_close;

        ui_widget_resize(ui_root(), 800, 600);
        basic.title = "Event created";
        basic.description = "Sunday, December 3 at 9:00 AM";
        basic.duration = UI_TOAST_DURATION_PERSISTENT;
        typed.type = UI_TOAST_TYPE_SUCCESS;
        typed.title = "Saved";
        typed.description = "Your changes have been saved.";
        typed.duration = UI_TOAST_DURATION_PERSISTENT;
        first = ui_toast_show(&basic);
        second = ui_toast_show(&typed);
        ui_update();
        first_content = ui_toast_get_content(first);
        second_content = ui_toast_get_content(second);
        first_close = find_child_class(first_content, "toast-close");
        second_close = find_child_class(second_content, "toast-close");

        ctest_equal_bool("default toast should not create an icon host",
                         find_child_type(first_content, "toast-icon") == NULL,
                         true);
        ctest_equal_float(
            "optional icon should not change the close button position",
            first_close->border_box.x, second_close->border_box.x);

        close_toast(second);
        close_toast(first);
}

static void test_show_structure(void)
{
        ui_toast_options_t options = { 0 };

        ui_widget_t *toast;
        ui_widget_t *viewport;
        ui_widget_t *content;
        ui_widget_t *body;

        options.title = "Event created";
        options.description = "Sunday, December 3 at 9:00 AM";
        options.action_text = "Undo";
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        ui_update();

        viewport = toast->parent;
        content = ui_toast_get_content(toast);
        body = find_child_type(content, "toast-body");
        ctest_equal_bool("viewport should be mounted under ui_root",
                         viewport == ui_toast_get_viewport(), true);
        ctest_equal_bool("viewport should be a root child",
                         viewport->parent == ui_root(), true);
        ctest_equal_bool("toast should be mounted in the viewport",
                         toast->parent == viewport, true);
        ctest_equal_bool("toast should have a content wrapper",
                         ui_check_widget_type(content, "toast-content"), true);
        ctest_equal_bool(
            "toast should have a title",
            body != NULL && find_child_type(body, "toast-title") != NULL, true);
        ctest_equal_bool(
            "toast should have a description",
            body != NULL && find_child_type(body, "toast-description") != NULL,
            true);
        ctest_equal_bool("toast should have an action",
                         find_child_class(content, "toast-action") != NULL,
                         true);
        ctest_equal_bool("toast should have a close button",
                         find_child_class(content, "toast-close") != NULL,
                         true);

        close_toast(toast);
}

static void test_viewport_is_singleton(void)
{
        ui_toast_options_t options = { 0 };

        ui_widget_t *first;
        ui_widget_t *second;

        options.duration = UI_TOAST_DURATION_PERSISTENT;
        first = ui_toast_show(&options);
        second = ui_toast_show(&options);
        ui_update();

        ctest_equal_bool("toasts should share one viewport",
                         first->parent == second->parent, true);

        close_toast(second);
        close_toast(first);
}

static void test_loading_type_uses_spinner(void)
{
        ui_toast_options_t options = { 0 };

        ui_widget_t *toast;
        ui_widget_t *content;
        ui_widget_t *icon_host;

        options.title = "Creating event...";
        options.type = UI_TOAST_TYPE_LOADING;
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        ui_update();
        content = ui_toast_get_content(toast);
        icon_host = find_child_type(content, "toast-icon");

        ctest_equal_bool("loading toast should have an icon host",
                         icon_host != NULL, true);
        ctest_equal_bool(
            "loading toast should use a spinner",
            icon_host != NULL && find_child_type(icon_host, "spinner") != NULL,
            true);
        ctest_equal_int("loading type should be readable",
                        ui_toast_get_type(toast), UI_TOAST_TYPE_LOADING);

        ui_toast_set_type(toast, UI_TOAST_TYPE_SUCCESS);
        ui_update();
        /* set_type recreates the icon host, so re-fetch it. */
        content = ui_toast_get_content(toast);
        icon_host = find_child_type(content, "toast-icon");
        ctest_equal_bool(
            "success toast should replace the spinner with an icon",
            icon_host != NULL &&
                find_child_type(icon_host, "spinner") == NULL &&
                find_child_type(icon_host, "icon") != NULL,
            true);

        close_toast(toast);
}

static void on_test_action(ui_widget_t *toast, void *arg)
{
        ++g_action_count;
}

static void test_action_callback(void)
{
        ui_toast_options_t options = { 0 };

        ui_widget_t *toast;
        ui_widget_t *content;
        ui_widget_t *action;
        ui_event_t event;

        options.title = "Event created";
        options.action_text = "Undo";
        options.on_action = on_test_action;
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        ui_update();
        content = ui_toast_get_content(toast);
        action = find_child_class(content, "toast-action");

        g_action_count = 0;
        ui_event_init(&event, "click");
        ui_widget_emit_event(action, event, NULL);
        ctest_equal_int("clicking the action should invoke its callback",
                        g_action_count, 1);

        close_toast(toast);
}

static void test_custom_actions(void)
{
        ui_toast_options_t options = { 0 };

        ui_widget_t *toast;
        ui_widget_t *actions;
        ui_widget_t *cancel;
        ui_widget_t *confirm;

        options.title = "Delete event?";
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        actions = ui_toast_get_actions(toast);
        cancel = ui_create_button();
        confirm = ui_create_button();
        ui_button_set_text(cancel, "Cancel");
        ui_button_set_text(confirm, "Confirm");
        ui_widget_append(actions, cancel);
        ui_widget_append(actions, confirm);
        ui_update();

        ctest_equal_bool("custom action row should contain both buttons",
                         ui_widget_get_child(actions, 0) == cancel &&
                             ui_widget_get_child(actions, 1) == confirm,
                         true);

        close_toast(toast);
}

static void test_auto_dismiss(void)
{
        ui_toast_options_t options = { 0 };
        ui_widget_t *toast;
        ui_widget_t *viewport;

        options.title = "Auto dismiss";
        options.duration = 1;
        toast = ui_toast_show(&options);
        viewport = toast->parent;
        ui_update();
        y_sleep(20);
        ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
        ui_update();
        ui_clear_trash();

        ctest_equal_bool("expired toast should be removed",
                         ui_widget_get_child(viewport, 0) == NULL, true);
}

static void test_close_button(void)
{
        ui_toast_options_t options = { 0 };

        ui_widget_t *toast;
        ui_widget_t *viewport;
        ui_widget_t *close_button;
        ui_event_t event;

        options.title = "Event created";
        options.duration = UI_TOAST_DURATION_PERSISTENT;
        toast = ui_toast_show(&options);
        ui_update();
        viewport = toast->parent;
        close_button =
            find_child_class(ui_toast_get_content(toast), "toast-close");

        ui_event_init(&event, "click");
        ui_widget_emit_event(close_button, event, NULL);
        ui_update();
        ui_clear_trash();

        ctest_equal_bool("close button should remove the toast",
                         ui_widget_get_child(viewport, 0) == NULL, true);
}

void test_toast(void)
{
        lcui_init();

        ctest_describe("toast icon layout", test_icon_optional_layout);
        ctest_describe("toast structure", test_show_structure);
        ctest_describe("toast viewport singleton", test_viewport_is_singleton);
        ctest_describe("toast loading type", test_loading_type_uses_spinner);
        ctest_describe("toast action callback", test_action_callback);
        ctest_describe("toast custom actions", test_custom_actions);
        ctest_describe("toast auto dismiss", test_auto_dismiss);
        ctest_describe("toast close button", test_close_button);

        lcui_destroy();
}
