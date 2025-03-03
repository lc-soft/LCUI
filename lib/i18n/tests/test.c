﻿/*
 * lib/i18n/tests/test.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <i18n.h>
#include <ctest.h>

void test_i18n(void)
{
        size_t len;
        wchar_t lang[32];

        len = i18n_detect_language(lang, 32);
        ctest_equal_bool("i18n_detect_language()", len > 0, true);
        ctest_equal_bool(
            "i18n_load_language(L\"zh-CN\", \"locales/zh-CN.yml\")",
            i18n_load_language(L"zh-CN", "locales/zh-CN.yml"), true);

        ctest_equal_bool("i18n_load_language(L\"en\", \"locales/en.yml\")",
                         i18n_load_language(L"en", "locales/en.yml"), true);

        ctest_equal_bool(
            "i18n_load_language(L\"notfound\", \"locales/notfound.yml\")",
            i18n_load_language(L"notfound", "locales/notfound.yml"), false);

        i18n_change_language(L"en");

        ctest_equal_wcs("en: i18n_translate(L\"name\") == L\"English\")",
                        i18n_translate(L"name"), L"English");

        ctest_equal_wcs("en: i18n_translate(L\"button.ok\") == L\"Ok\")",
                        i18n_translate(L"button.ok"), L"Ok");

        ctest_equal_bool("en: i18n_translate(L\"button.notfound\") == NULL)",
                         i18n_translate(L"button.notfound") == NULL, true);

        i18n_change_language(L"zh-CN");

        ctest_equal_wcs("zh-CN: i18n_translate(L\"name\")",
                        i18n_translate(L"name"), L"\u4E2D\u6587");

        ctest_equal_wcs("zh-CN: i18n_translate(L\"button.ok\")",
                        i18n_translate(L"button.ok"), L"\u786E\u5B9A");

        ctest_equal_bool("zh-CN: i18n_translate(L\"button.notfound\") == NULL",
                         i18n_translate(L"button.notfound") == NULL, true);
        i18n_clear();
}

int main(void)
{
        logger_set_level(LOGGER_LEVEL_OFF);
        ctest_describe("i18n", test_i18n);
        return ctest_finish();
}
