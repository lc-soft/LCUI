/*
 * lib/ui-xml/tests/test_xml_parser.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui.h>
#include <ui_xml.h>
#include <ctest.h>

static void check_widget_attribute(void)
{
        ui_widget_t *w;

        w = ui_get_widget("test-attr-disabled-1");
        ctest_equal_bool("should be disabled for element-1", w->disabled, true);
        w = ui_get_widget("test-attr-disabled-2");
        ctest_equal_bool("should be disabled for element-2", w->disabled, true);
        w = ui_get_widget("test-attr-disabled-3");
        ctest_equal_bool("should be disabled for element-3", w->disabled, true);
        w = ui_get_widget("test-attr-disabled-4");
        ctest_equal_bool("should not be disabled for element-4", w->disabled,
                         false);
}

static void check_widget_loaded_from_nested_xml(void)
{
        ui_widget_t *w;

        w = ui_get_widget("test-nested-1");
        ctest_equal_bool("should exist for test-nested-1", w != NULL, true);
        w = ui_get_widget("test-nested-2");
        ctest_equal_bool("should exist for test-nested-2", w != NULL, true);
        w = ui_get_widget("test-nested-3");
        ctest_equal_bool("should exist for test-nested-3", w != NULL, true);
        w = ui_get_widget("test-nested-4");
        ctest_equal_bool("should exist for test-nested-4", w != NULL, true);
}

void test_ui_xml_parser(void)
{
        ui_widget_t *pack;

        ui_init();
        ctest_equal_bool(
            "should load XML file",
            (pack = ui_load_xml_file("test_xml_parser.xml")) != NULL, true);
        if (!pack) {
                ui_destroy();
                return;
        }
        ui_root_append(pack);
        ui_widget_unwrap(pack);
        ui_update();
        ctest_describe("widget attribute", check_widget_attribute);
        ctest_describe("widgets loaded from nested xml",
                       check_widget_loaded_from_nested_xml);
        ui_destroy();
}
