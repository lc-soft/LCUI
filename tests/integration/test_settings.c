/*
 * tests/cases/test_settings.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
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
                         lcui_settings_get_rendering(&settings), true);
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
                         lcui_settings_set_rendering(&settings), true);
        settings.fps_cap = 0;
        settings.parallel_threads = 0;
        settings.paint_flashing = false;
        ctest_equal_bool("should get updated rendering settings",
                         lcui_settings_get_rendering(&settings), true);
        ctest_equal_int("should apply frame rate cap", settings.fps_cap, 240);
        ctest_equal_int("should apply parallel rendering threads",
                        settings.parallel_threads, 8);
        ctest_equal_bool("should apply paint flashing", settings.paint_flashing,
                         true);

        settings.fps_cap = 20;
        ctest_equal_bool("should reject out-of-range frame rate cap",
                         lcui_settings_set_rendering(&settings), false);
        ctest_equal_bool("should keep current rendering settings",
                         lcui_settings_get_rendering(&settings), true);
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
                         lcui_settings_get_rendering(&settings), true);
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
                         lcui_settings_get_rendering(&settings), true);
        ctest_equal_int("should parse fps_cap from user config",
                        settings.fps_cap, 60);
        ctest_equal_int("should parse parallel_threads from user config",
                        settings.parallel_threads, 5);
        ctest_equal_bool("should parse true bool values from user config",
                         settings.paint_flashing, true);
        lcui_destroy();
}

void test_settings(void)
{
        ctest_describe("rendering settings accessors",
                       test_settings_rendering_accessors);
        ctest_describe("settings load/merge/validate/flush",
                       test_settings_load_merge_validate_flush);
}
