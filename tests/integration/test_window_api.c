/*
 * tests/integration/test_window_api.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void helper_process_window_events(unsigned rounds)
{
        unsigned i;

        for (i = 0; i < rounds; ++i) {
                ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
                y_sleep(10);
        }
}

static void test_create_hidden_and_bind(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w;
        ui_widget_t *content;
        ui_widget_t *other;

        opt.title = L"probe";
        opt.width = 320;
        opt.height = 240;
        lcui_init();
        content = ui_create_widget(NULL);
        other = ui_create_widget(NULL);
        ui_root_append(content);
        ui_root_append(other);
        w = lcui_window_create(&opt);
        ctest_equal_bool("should create window", w != NULL, true);
        ctest_equal_bool("should be hidden",
                         ptk_window_is_visible(lcui_window_get_handle(w)),
                         false);
        ctest_equal_bool("should bind widget",
                         lcui_window_bind_widget(w, content), true);
        ctest_equal_bool("should get widget",
                         lcui_window_get_widget(w) == content, true);
        ctest_equal_bool("should map widget to window",
                         ui_server_get_window(content) != NULL, true);
        ctest_equal_bool("should rebind widget",
                         lcui_window_bind_widget(w, other), true);
        ctest_equal_bool("should map new widget",
                         ui_server_get_window(other) != NULL, true);
        ctest_equal_bool("should unmap old widget",
                         ui_server_get_window(content) == NULL, true);
        ctest_equal_bool("should unbind widget",
                         lcui_window_bind_widget(w, NULL), true);
        ctest_equal_bool("should unmap unbound widget",
                         ui_server_get_window(other) == NULL, true);
        lcui_window_destroy(w);
        ui_widget_destroy(content);
        ui_widget_destroy(other);
        lcui_destroy();
}

static void test_create_for_widget_inherits(void)
{
        lcui_window_options_t opt = { 0 };
        ui_widget_t *content;
        lcui_window_t *w;
        int width, height;

        lcui_init();
        content = ui_create_widget(NULL);
        ui_widget_set_title(content, L"Inherited");
        ui_widget_resize(content, 400, 300);
        ui_root_append(content);
        ui_update();
        w = lcui_window_create_for_widget(content, &opt);
        lcui_window_get_size(w, &width, &height);
        ctest_equal_int("should inherit width", width, 400);
        ctest_equal_int("should inherit height", height, 300);
        lcui_window_show(w);
        ctest_equal_bool("should still be hidden before first frame",
                         ptk_window_is_visible(lcui_window_get_handle(w)),
                         false);
        lcui_ui_update();
        lcui_ui_render();
        ctest_equal_bool("should be visible after first frame",
                         ptk_window_is_visible(lcui_window_get_handle(w)),
                         true);
        lcui_window_destroy(w);
        ui_widget_destroy(content);
        lcui_destroy();
}

static int close_count;

static void on_window_close(lcui_window_t *w, void *arg)
{
        ++close_count;
}

static void test_geometry_round_trip(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w;
        int x, y, cw, ch;

        lcui_init();
        opt.width = 300;
        opt.height = 200;
        w = lcui_window_create(&opt);
        lcui_window_set_size(w, 640, 480);
        lcui_window_get_size(w, &cw, &ch);
        ctest_equal_int("should set width", cw, 640);
        ctest_equal_int("should set height", ch, 480);
        lcui_window_set_position(w, 50, 60);
        lcui_window_get_position(w, &x, &y);
        ctest_equal_int("should set x", x, 50);
        ctest_equal_int("should set y", y, 60);
        lcui_window_destroy(w);
        lcui_destroy();
}

static void test_close_callback(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w;
        unsigned i;

        close_count = 0;
        lcui_init();
        opt.width = 200;
        opt.height = 150;
        w = lcui_window_create(&opt);
        lcui_window_on_close(w, on_window_close, NULL);
        lcui_window_close(w);
        for (i = 0; i < 4; ++i) {
                ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
        }
        ctest_equal_int("should call close callback", close_count, 1);
        lcui_window_destroy(w);
        lcui_destroy();
}

static void test_window_drives_content_size(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w;
        ui_widget_t *content;
        int cw, ch;

        lcui_init();
        opt.width = 300;
        opt.height = 200;
        content = ui_create_widget(NULL);
        ui_root_append(content);
        w = lcui_window_create_for_widget(content, &opt);
        lcui_window_set_size(w, 500, 400);
        helper_process_window_events(12);
        lcui_ui_update();
        ctest_equal_int("should resize content width",
                        (int)content->border_box.width, 500);
        ctest_equal_int("should resize content height",
                        (int)content->border_box.height, 400);
        ui_widget_resize(content, 200, 150);
        helper_process_window_events(12);
        lcui_window_get_size(w, &cw, &ch);
        ctest_equal_int("should not resize window from content", cw, 500);
        ctest_equal_int("should not resize window from content", ch, 400);
        lcui_window_destroy(w);
        ui_widget_destroy(content);
        lcui_destroy();
}

static void test_no_window_after_init(void)
{
        lcui_init();
        ctest_equal_bool("should not create a window in init",
                         ui_server_get_window(ui_root()) == NULL, true);
        lcui_destroy();
}

static void test_content_destroy_unbinds_window(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w;
        ui_widget_t *content;

        close_count = 0;
        opt.width = 200;
        opt.height = 150;
        lcui_init();
        content = ui_create_widget(NULL);
        ui_root_append(content);
        w = lcui_window_create(&opt);
        lcui_window_on_close(w, on_window_close, NULL);
        ctest_equal_bool("should bind content",
                         lcui_window_bind_widget(w, content), true);

        ui_widget_destroy(content);

        ctest_equal_bool("should clear the bound widget",
                         lcui_window_get_widget(w) == NULL, true);
        ctest_equal_bool("should keep the platform window",
                         lcui_window_get_handle(w) != NULL, true);
        ctest_equal_int("should not close the window", close_count, 0);
        lcui_window_destroy(w);
        lcui_destroy();
}

static void test_rebind_between_windows(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w1, *w2;
        ui_widget_t *content;

        opt.width = 200;
        opt.height = 150;
        lcui_init();
        content = ui_create_widget(NULL);
        ui_root_append(content);
        w1 = lcui_window_create(&opt);
        w2 = lcui_window_create(&opt);
        ctest_equal_bool("should bind to the first window",
                         lcui_window_bind_widget(w1, content), true);
        ctest_equal_bool("should rebind to the second window",
                         lcui_window_bind_widget(w2, content), true);
        ctest_equal_bool("should unbind from the first window",
                         lcui_window_get_widget(w1) == NULL, true);
        ctest_equal_bool("should keep the second window binding",
                         lcui_window_get_widget(w2) == content, true);
        ctest_equal_bool(
            "should map content to the second platform window",
            ui_server_get_window(content) == lcui_window_get_handle(w2), true);
        lcui_window_destroy(w1);
        lcui_window_destroy(w2);
        ui_widget_destroy(content);
        lcui_destroy();
}

static void test_post_close_api_safe(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *w;
        ui_widget_t *content;
        int x, y, cw, ch;
        unsigned i;

        opt.width = 300;
        opt.height = 200;
        lcui_init();
        w = lcui_window_create(&opt);
        lcui_window_set_position(w, 40, 50);
        lcui_window_close(w);
        for (i = 0; i < 4; ++i) {
                ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
        }
        lcui_window_get_size(w, &cw, &ch);
        lcui_window_get_position(w, &x, &y);
        ctest_equal_int("should keep the last width", cw, 300);
        ctest_equal_int("should keep the last height", ch, 200);
        ctest_equal_int("should keep the last x", x, 40);
        ctest_equal_int("should keep the last y", y, 50);
        lcui_window_show(w);
        lcui_window_hide(w);
        lcui_window_set_size(w, 100, 100);
        content = ui_create_widget(NULL);
        ctest_equal_bool("should reject binding after close",
                         lcui_window_bind_widget(w, content), false);
        ui_widget_destroy(content);
        lcui_window_destroy(w);
        lcui_destroy();
}

void test_window_api(void)
{
        ctest_describe("window object", test_create_hidden_and_bind);
        ctest_describe("create for widget", test_create_for_widget_inherits);
        ctest_describe("window geometry", test_geometry_round_trip);
        ctest_describe("window close callback", test_close_callback);
        ctest_describe("window drives content size",
                       test_window_drives_content_size);
        ctest_describe("no implicit window", test_no_window_after_init);
        ctest_describe("content destroy unbinds window",
                       test_content_destroy_unbinds_window);
        ctest_describe("rebind between windows", test_rebind_between_windows);
        ctest_describe("post close api", test_post_close_api_safe);
}
