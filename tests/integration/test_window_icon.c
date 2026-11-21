/*
 * tests/integration/test_window_icon.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <LCUI.h>
#include <ctest-custom.h>

#ifdef _WIN32
#include <windows.h>
#endif

static unsigned char *read_fixture(const char *filename, size_t *size)
{
        FILE *fp;
        long length;
        unsigned char *data;

        *size = 0;
        fp = fopen(filename, "rb");
        if (!fp) {
                return NULL;
        }
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        rewind(fp);
        if (length <= 0) {
                fclose(fp);
                return NULL;
        }
        data = malloc((size_t)length);
        if (!data) {
                fclose(fp);
                return NULL;
        }
        *size = fread(data, 1, (size_t)length, fp);
        fclose(fp);
        if (*size == 0) {
                free(data);
                return NULL;
        }
        return data;
}

static lcui_window_t *create_window(const char *icon)
{
        lcui_window_options_t opt = { 0 };

        opt.title = L"icon probe";
        opt.width = 320;
        opt.height = 240;
        opt.icon = icon;
        return lcui_window_create(&opt);
}

#ifdef _WIN32
static HICON get_window_icon(lcui_window_t *w, WPARAM type)
{
        HWND hwnd = (HWND)ptk_window_get_handle(lcui_window_get_handle(w));

        return (HICON)SendMessageW(hwnd, WM_GETICON, type, 0);
}

static bool get_icon_size(HICON icon, int *width, int *height)
{
        BITMAP bitmap;
        ICONINFO info;

        if (!icon || !GetIconInfo(icon, &info)) {
                return false;
        }
        if (!GetObject(info.hbmColor, sizeof(bitmap), &bitmap)) {
                DeleteObject(info.hbmColor);
                DeleteObject(info.hbmMask);
                return false;
        }
        *width = bitmap.bmWidth;
        *height = bitmap.bmHeight;
        DeleteObject(info.hbmColor);
        DeleteObject(info.hbmMask);
        return true;
}

static int expected_icon_size(unsigned dpi, bool big)
{
        unsigned scale = dpi * 100 / 96;

        if (scale >= 175) {
                return big ? 64 : 32;
        }
        if (scale >= 125) {
                return big ? 48 : 24;
        }
        return big ? 32 : 16;
}
#endif

static void test_icon_from_file(void)
{
        lcui_window_t *w;
#ifdef _WIN32
        int width, height;
        unsigned dpi;
#endif

        lcui_init();
        w = create_window(NULL);
        ctest_equal_bool("should create window", w != NULL, true);
        ctest_equal_bool("should set icon from file",
                         lcui_window_set_icon(w, "test_background.png"), true);
#ifdef _WIN32
        ctest_equal_bool("should set small icon handle",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
        ctest_equal_bool("should set big icon handle",
                         get_window_icon(w, ICON_BIG) != NULL, true);
        dpi = ptk_window_get_dpi(lcui_window_get_handle(w));
        ctest_equal_bool(
            "should read small icon bitmap",
            get_icon_size(get_window_icon(w, ICON_SMALL), &width, &height),
            true);
        ctest_equal_int("small icon width should follow dpi", width,
                        expected_icon_size(dpi, false));
        ctest_equal_int("small icon height should follow dpi", height,
                        expected_icon_size(dpi, false));
        ctest_equal_bool(
            "should read big icon bitmap",
            get_icon_size(get_window_icon(w, ICON_BIG), &width, &height), true);
        ctest_equal_int("big icon width should follow dpi", width,
                        expected_icon_size(dpi, true));
        ctest_equal_int("big icon height should follow dpi", height,
                        expected_icon_size(dpi, true));
#endif
        ctest_equal_bool("should reject missing icon file",
                         lcui_window_set_icon(w, "missing-icon.png"), false);
        lcui_window_destroy(w);
        lcui_destroy();
}

static void test_icon_from_data(void)
{
        lcui_window_t *w;
        unsigned char *data;
        size_t size;

        data = read_fixture("test_background.png", &size);
        ctest_equal_bool("should load icon fixture", data != NULL, true);
        lcui_init();
        w = create_window(NULL);
        ctest_equal_bool("should set icon from data",
                         lcui_window_set_icon_from_data(w, data, size), true);
#ifdef _WIN32
        ctest_equal_bool("should set icon handle from data",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        ctest_equal_bool("should clear custom icon",
                         lcui_window_set_icon(w, NULL), true);
#ifdef _WIN32
        ctest_equal_bool("should restore default icon after clear",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        ctest_equal_bool("should reject invalid icon data",
                         lcui_window_set_icon_from_data(w, "not an image", 12),
                         false);
        lcui_window_destroy(w);
        lcui_destroy();
        free(data);
}

static void test_default_icon(void)
{
        lcui_window_t *w;

        lcui_init();
        w = create_window(NULL);
#ifdef _WIN32
        ctest_equal_bool("should apply built-in default icon",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        lcui_set_default_window_icon_enabled(false);
#ifdef _WIN32
        ctest_equal_bool("should clear default icon when disabled",
                         get_window_icon(w, ICON_SMALL) == NULL, true);
#endif
        lcui_set_default_window_icon_enabled(true);
#ifdef _WIN32
        ctest_equal_bool("should restore default icon when enabled",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        lcui_window_destroy(w);
        lcui_destroy();
}

static void test_icon_from_options(void)
{
        lcui_window_t *w;

        lcui_init();
        w = create_window("test_background.png");
        ctest_equal_bool("should create window with icon option", w != NULL,
                         true);
#ifdef _WIN32
        ctest_equal_bool("should apply icon from options",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        lcui_window_destroy(w);
        lcui_destroy();
}

static void test_default_icon_override(void)
{
        lcui_window_t *w;
        unsigned char *data;
        size_t size;

        data = read_fixture("test_background.png", &size);
        ctest_equal_bool("should load override fixture", data != NULL, true);
        lcui_init();
        w = create_window(NULL);
        ctest_equal_bool("should set global default icon",
                         lcui_set_default_window_icon("test_background.png"),
                         true);
#ifdef _WIN32
        ctest_equal_bool("should update existing window icon",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        ctest_equal_bool("should set global default icon from data",
                         lcui_set_default_window_icon_from_data(data, size),
                         true);
#ifdef _WIN32
        ctest_equal_bool("should keep existing window icon after data override",
                         get_window_icon(w, ICON_SMALL) != NULL, true);
#endif
        ctest_equal_bool("should reset global default icon",
                         lcui_set_default_window_icon(NULL), true);
        ctest_equal_bool("should reset global default icon from data",
                         lcui_set_default_window_icon_from_data(NULL, 0), true);
        lcui_window_destroy(w);
        lcui_destroy();
        free(data);
}

void test_window_icon(void)
{
        ctest_describe("window icon from file", test_icon_from_file);
        ctest_describe("window icon from data", test_icon_from_data);
        ctest_describe("window default icon", test_default_icon);
        ctest_describe("window icon options", test_icon_from_options);
        ctest_describe("window default icon override",
                       test_default_icon_override);
}
