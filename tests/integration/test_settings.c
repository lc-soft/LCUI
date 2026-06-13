/*
 * tests/cases/test_settings.c
 *
 * Copyright (c) 2023, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#include <process.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <LCUI.h>
#include <ctest-custom.h>

static int test_mkdir(const char *path)
{
#ifdef _WIN32
        return _mkdir(path);
#else
        return mkdir(path, 0700);
#endif
}

static unsigned test_path_counter = 0;

static bool test_set_env(const char *name, const char *value)
{
#ifdef _WIN32
        return _putenv_s(name, value) == 0;
#else
        return setenv(name, value, 1) == 0;
#endif
}

static bool test_prepare_env(char *base_dir, size_t base_dir_size,
                             const char *app_id)
{
        int ret;
        const char *tmp_root;
#ifdef _WIN32
        int pid = _getpid();
        tmp_root = getenv("TEMP");
        if (!tmp_root || tmp_root[0] == 0) {
                tmp_root = "C:/Temp";
        }
#else
        int pid = getpid();
        tmp_root = getenv("TMPDIR");
        if (!tmp_root || tmp_root[0] == 0) {
                tmp_root = "/tmp";
        }
#endif

        snprintf(base_dir, base_dir_size, "%s/lcui-settings-%d-%lu", tmp_root,
                 pid, (unsigned long)time(NULL) + ++test_path_counter);
        ret = test_mkdir(base_dir);
        if (ret != 0 && errno != EEXIST) {
                return false;
        }
#ifdef _WIN32
        if (!test_set_env("APPDATA", base_dir)) {
                return false;
        }
#else
        if (!test_set_env("XDG_CONFIG_HOME", base_dir)) {
                return false;
        }
#endif
        if (!lcui_set_app_id(app_id)) {
                return false;
        }
        return true;
}

static bool test_write_config(const char *base_dir, const char *app_id,
                              const char *content)
{
        bool ok = false;
        char app_dir[512];
        char config_file[600];
        FILE *fp;

        snprintf(app_dir, sizeof(app_dir), "%s/%s", base_dir, app_id);
        if (test_mkdir(app_dir) != 0 && errno != EEXIST) {
                return false;
        }
        snprintf(config_file, sizeof(config_file), "%s/settings.ini", app_dir);
        fp = fopen(config_file, "w");
        if (!fp) {
                return false;
        }
        if (fputs(content, fp) >= 0) {
                ok = true;
        }
        fclose(fp);
        return ok;
}

static bool test_read_file_contains(const char *base_dir, const char *app_id,
                                    const char *text)
{
        bool found = false;
        char config_file[600];
        char content[1024];
        FILE *fp;
        size_t len;

        snprintf(config_file, sizeof(config_file), "%s/%s/settings.ini",
                 base_dir, app_id);
        fp = fopen(config_file, "r");
        if (!fp) {
                return false;
        }
        len = fread(content, 1, sizeof(content) - 1, fp);
        fclose(fp);
        content[len] = 0;
        if (strstr(content, text)) {
                found = true;
        }
        return found;
}

static void helper_process_window_events(unsigned rounds)
{
        unsigned i;

        for (i = 0; i < rounds; ++i) {
                ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
                sleep_ms(10);
        }
}

static bool helper_query_window_state(ptk_window_t *window,
                                      lcui_window_state_t *out)
{
        if (!window || !out) {
                return false;
        }
        ptk_window_get_position(window, &out->x, &out->y);
        out->width = ptk_window_get_width(window);
        out->height = ptk_window_get_height(window);
        out->maximized = ptk_window_is_maximized(window);
        return true;
}

static void test_settings_rendering_accessors(void)
{
        char base_dir[256];
        lcui_rendering_settings_t settings;

        ctest_equal_bool("prepare settings test env",
                         test_prepare_env(base_dir, sizeof(base_dir),
                                          "lcui-settings-rendering"),
                         true);
        lcui_init();
        ctest_equal_bool("should load settings", lcui_settings_load(), true);
        ctest_equal_bool("should get default rendering settings",
                         lcui_get_rendering(&settings), true);
        ctest_equal_int("should default frame rate cap to 120",
                        settings.fps_cap, LCUI_DEFAULT_FPS_CAP);
        ctest_equal_int("should default parallel rendering threads to 4",
                        settings.parallel_threads,
                        LCUI_DEFAULT_PARALLEL_THREADS);
        ctest_equal_bool("should disable paint flashing by default",
                         settings.paint_flashing, false);

        settings.fps_cap = 240;
        settings.parallel_threads = 8;
        settings.paint_flashing = true;
        ctest_equal_bool("should set rendering settings",
                         lcui_set_rendering(&settings), true);
        settings.fps_cap = 0;
        settings.parallel_threads = 0;
        settings.paint_flashing = false;
        ctest_equal_bool("should get updated rendering settings",
                         lcui_get_rendering(&settings), true);
        ctest_equal_int("should apply frame rate cap", settings.fps_cap, 240);
        ctest_equal_int("should apply parallel rendering threads",
                        settings.parallel_threads, 8);
        ctest_equal_bool("should apply paint flashing", settings.paint_flashing,
                         true);

        settings.fps_cap = 20;
        ctest_equal_bool("should reject out-of-range frame rate cap",
                         lcui_set_rendering(&settings), false);
        ctest_equal_bool("should keep current rendering settings",
                         lcui_get_rendering(&settings), true);
        ctest_equal_int("should keep previous frame rate cap", settings.fps_cap,
                        240);
        lcui_destroy();
}

static void test_settings_load_merge_validate_flush(void)
{
        const char *app_id = "lcui-settings-load";
        char base_dir[256];
        lcui_rendering_settings_t settings;

        ctest_equal_bool("prepare settings test env",
                         test_prepare_env(base_dir, sizeof(base_dir), app_id),
                         true);
        ctest_equal_bool("write malformed settings file",
                         test_write_config(base_dir, app_id,
                                           "[meta]\nversion=abc\n\n"
                                           "[rendering]\n"
                                           "fps_cap=999\n"
                                           "parallel_threads=8\n"
                                           "paint_flashing=oops\n"),
                         true);

        lcui_init();
        ctest_equal_bool("should load settings", lcui_settings_load(), true);
        ctest_equal_bool("should get merged rendering settings",
                         lcui_get_rendering(&settings), true);
        ctest_equal_int("should fallback fps_cap to default on invalid value",
                        settings.fps_cap, LCUI_DEFAULT_FPS_CAP);
        ctest_equal_int("should keep valid parallel_threads",
                        settings.parallel_threads, 8);
        ctest_equal_bool(
            "should fallback paint_flashing to default on invalid value",
            settings.paint_flashing, false);
        ctest_equal_bool("should flush normalized settings",
                         lcui_settings_flush(), true);
        ctest_equal_bool("should normalize version during flush",
                         test_read_file_contains(base_dir, app_id, "version=1"),
                         true);
        ctest_equal_bool(
            "should normalize fps_cap during flush",
            test_read_file_contains(base_dir, app_id, "fps_cap=120"), true);
        ctest_equal_bool(
            "should keep valid parallel_threads during flush",
            test_read_file_contains(base_dir, app_id, "parallel_threads=8"),
            true);
        ctest_equal_bool(
            "should normalize paint_flashing during flush",
            test_read_file_contains(base_dir, app_id, "paint_flashing=0"),
            true);

        lcui_settings_unload();
        ctest_equal_bool("write valid boolean rendering config",
                         test_write_config(base_dir, app_id,
                                           "[meta]\nversion=1\n\n"
                                           "[rendering]\n"
                                           "fps_cap=60\n"
                                           "parallel_threads=5\n"
                                           "paint_flashing=true\n"),
                         true);
        ctest_equal_bool("should reload valid rendering settings",
                         lcui_settings_load(), true);
        ctest_equal_bool("should get rendering settings after reload",
                         lcui_get_rendering(&settings), true);
        ctest_equal_int("should parse fps_cap from user config",
                        settings.fps_cap, 60);
        ctest_equal_int("should parse parallel_threads from user config",
                        settings.parallel_threads, 5);
        ctest_equal_bool("should parse true bool values from user config",
                         settings.paint_flashing, true);
        lcui_destroy();
}

static void test_window_state_accessors(void)
{
        char base_dir[256];
        lcui_window_state_t state;
        lcui_window_state_t output;

        ctest_equal_bool(
            "prepare window state test env",
            test_prepare_env(base_dir, sizeof(base_dir), "lcui-window-state"),
            true);
        lcui_init();
        state.x = -120;
        state.y = 48;
        state.width = 640;
        state.height = 480;
        state.maximized = true;
        ctest_equal_bool("should reject missing window state section",
                         lcui_get_window_state("main", &output), false);
        ctest_equal_bool("should reject invalid window state id",
                         lcui_set_window_state("", &state), false);
        ctest_equal_bool("should set window state",
                         lcui_set_window_state("main", &state), true);
        memset(&output, 0, sizeof(output));
        ctest_equal_bool("should get window state",
                         lcui_get_window_state("main", &output), true);
        ctest_equal_int("should round-trip window x", output.x, state.x);
        ctest_equal_int("should round-trip window y", output.y, state.y);
        ctest_equal_int("should round-trip window width", output.width,
                        state.width);
        ctest_equal_int("should round-trip window height", output.height,
                        state.height);
        ctest_equal_bool("should round-trip maximized flag", output.maximized,
                         state.maximized);
        lcui_destroy();
}

static void test_window_state_invalid_values(void)
{
        const char *app_id = "lcui-window-invalid";
        char base_dir[256];
        lcui_window_state_t state;

        ctest_equal_bool("prepare invalid window state env",
                         test_prepare_env(base_dir, sizeof(base_dir), app_id),
                         true);
        ctest_equal_bool("write invalid window state config",
                         test_write_config(base_dir, app_id,
                                           "[meta]\nversion=1\n\n"
                                           "[window.main]\n"
                                           "x=-10\n"
                                           "y=20\n"
                                           "width=0\n"
                                           "height=240\n"
                                           "maximized=maybe\n"),
                         true);
        lcui_init();
        ctest_equal_bool("should load settings with invalid window state",
                         lcui_settings_load(), true);
        ctest_equal_bool("should reject invalid window state values",
                         lcui_get_window_state("main", &state), false);
        ctest_equal_bool("should flush invalid window state removal",
                         lcui_settings_flush(), true);
        ctest_equal_bool(
            "should remove invalid window section on flush",
            test_read_file_contains(base_dir, app_id, "[window.main]"), false);
        lcui_destroy();
}

static void test_window_state_binding(void)
{
        const char *app_id = "lcui-window-bind";
        char base_dir[256];
        int new_width, new_height;
        lcui_window_state_t saved;
        lcui_window_state_t actual;
        ui_widget_t *preview_widget;
        ui_widget_t *widget;
        ptk_window_t *window;

        ctest_equal_bool("prepare window binding env",
                         test_prepare_env(base_dir, sizeof(base_dir), app_id),
                         true);
        ctest_equal_bool("write stored window state",
                         test_write_config(base_dir, app_id,
                                           "[meta]\nversion=1\n\n"
                                           "[window.main]\n"
                                           "x=32\n"
                                           "y=48\n"
                                           "width=420\n"
                                           "height=260\n"
                                           "maximized=0\n\n"
                                           "[window.preview]\n"
                                           "x=8\n"
                                           "y=12\n"
                                           "width=210\n"
                                           "height=180\n"
                                           "maximized=0\n"),
                         true);
        lcui_init();
        ctest_equal_bool("should load settings", lcui_settings_load(), true);
        preview_widget = ui_create_widget(NULL);
        ctest_equal_bool("should create preview widget", preview_widget != NULL,
                         true);
        ctest_equal_bool("should bind pending widget state",
                         lcui_bind_window_state(preview_widget, "preview"),
                         true);
        ui_root_append(preview_widget);
        ui_update();
        ctest_equal_int("should resize pending widget width",
                        (int)preview_widget->border_box.width, 210);
        ctest_equal_int("should resize pending widget height",
                        (int)preview_widget->border_box.height, 180);
        ui_widget_destroy(preview_widget);
        widget = ui_root();
        window = ui_server_get_window(widget);
        ctest_equal_bool("should get root window", window != NULL, true);
        helper_process_window_events(8);
        ctest_equal_bool("should bind window state",
                         lcui_bind_window_state(widget, "main"), true);
        helper_process_window_events(12);
        ctest_equal_bool("should query restored window geometry",
                         helper_query_window_state(window, &actual), true);
        ctest_equal_int("should restore saved width", actual.width, 420);
        ctest_equal_int("should restore saved height", actual.height, 260);
        ptk_window_set_size(window, 300, 220);
        helper_process_window_events(12);
        new_width = ptk_window_get_width(window);
        new_height = ptk_window_get_height(window);
        ptk_window_close(window);
        helper_process_window_events(12);
        ctest_equal_bool("should persist updated bound window state on close",
                         lcui_get_window_state("main", &saved), true);
        ctest_equal_int("should save updated width", saved.width, new_width);
        ctest_equal_int("should save updated height", saved.height, new_height);
        ctest_equal_bool(
            "should flush bound window section",
            test_read_file_contains(base_dir, app_id, "[window.main]"), true);
        lcui_destroy();
}

void test_settings(void)
{
        ctest_describe("rendering settings accessors",
                       test_settings_rendering_accessors);
        ctest_describe("settings load/merge/validate/flush",
                       test_settings_load_merge_validate_flush);
        ctest_describe("window state accessors", test_window_state_accessors);
        ctest_describe("window state invalid values",
                       test_window_state_invalid_values);
        ctest_describe("window state binding", test_window_state_binding);
}
