/*
 * tests/integration/test_icon.c: -- Icon widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void test_defaults(void)
{
        ui_widget_t *icon = ui_create_icon();

        ctest_equal_bool("should inherit the text prototype",
                         ui_check_widget_type(icon, "text"), true);
        ctest_equal_bool("should use the shared Fluent font class",
                         ui_widget_has_class(icon, "fui-icon"), true);
        ctest_equal_bool("should omit the old Fluent regular font class",
                         ui_widget_has_class(icon, "fui-icon-regular"), false);

        ui_widget_destroy(icon);
}

static void test_fluent_factory(void)
{
        ui_widget_t *icon = ui_create_fluent_icon("checkmark");

        ctest_equal_bool("should create a Fluent icon with its name class",
                         ui_widget_has_class(icon, "fui-icon-checkmark"), true);
        ctest_equal_bool("should use the shared Fluent font class",
                         ui_widget_has_class(icon, "fui-icon"), true);
        ctest_equal_bool("should use automatic design size by default",
                         ui_widget_has_class(icon, "fui-icon-glyph-0"), false);
        ui_widget_destroy(icon);
}

static void test_glyph_class(void)
{
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, "access-time");
        ctest_equal_bool("should add the namespaced Fluent icon class",
                         ui_widget_has_class(icon, "fui-icon-access-time"),
                         true);
        ctest_equal_bool("should omit a glyph size class for the default size",
                         ui_widget_has_class(icon, "fui-icon-glyph-24"), false);
        ctest_equal_bool("should omit the unnamespaced icon name class",
                         ui_widget_has_class(icon, "access-time"), false);
        ctest_equal_bool("should omit the legacy regular glyph class",
                         ui_widget_has_class(icon, "access-time-24-regular"),
                         false);

        ui_icon_set_size(icon, 20);
        ctest_equal_bool("should remove the previous glyph class",
                         ui_widget_has_class(icon, "access-time-24-regular"),
                         false);
        ctest_equal_bool("should omit the legacy size glyph class",
                         ui_widget_has_class(icon, "access-time-20-regular"),
                         false);
        ctest_equal_bool("should add the requested glyph size class",
                         ui_widget_has_class(icon, "fui-icon-glyph-20"), true);
        ctest_equal_bool("should omit the unnamespaced glyph size class",
                         ui_widget_has_class(icon, "size-20"), false);

        ui_icon_set_size(icon, 0);
        ctest_equal_bool(
            "should remove the glyph size class when size is automatic",
            ui_widget_has_class(icon, "fui-icon-glyph-20"), false);
        ctest_equal_bool("should omit a zero glyph size class",
                         ui_widget_has_class(icon, "fui-icon-glyph-0"), false);

        ui_widget_destroy(icon);
}

static void test_library_and_fill(void)
{
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, "access-time");
        ui_icon_set_filled(icon, true);
        ctest_equal_bool("should keep the namespaced Fluent icon class",
                         ui_widget_has_class(icon, "fui-icon-access-time"),
                         true);
        ctest_equal_bool("should add the namespaced filled modifier",
                         ui_widget_has_class(icon, "fui-icon-filled"), true);
        ctest_equal_bool("should omit the unnamespaced filled modifier",
                         ui_widget_has_class(icon, "filled"), false);
        ctest_equal_bool("should keep the shared Fluent font class",
                         ui_widget_has_class(icon, "fui-icon"), true);
        ctest_equal_bool("should omit the old Fluent regular font class",
                         ui_widget_has_class(icon, "fui-icon-regular"), false);
        ctest_equal_bool("should replace the regular glyph class",
                         ui_widget_has_class(icon, "access-time-24-regular"),
                         false);
        ctest_equal_bool("should omit the legacy filled glyph class",
                         ui_widget_has_class(icon, "access-time-24-filled"),
                         false);
        ui_icon_set_size(icon, 20);
        ctest_equal_bool("should add glyph size class while filled",
                         ui_widget_has_class(icon, "fui-icon-glyph-20"), true);
        ctest_equal_bool("should omit the legacy filled size glyph class",
                         ui_widget_has_class(icon, "access-time-20-filled"),
                         false);

        ui_widget_destroy(icon);
}

static void test_lucide_class(void)
{
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_library(icon, UI_ICON_LIBRARY_LUCIDE);
        ui_icon_set_name(icon, "access-time");
        ui_icon_set_size(icon, 20);
        ui_icon_set_filled(icon, true);

        ctest_equal_bool("should use the Lucide base class",
                         ui_widget_has_class(icon, "lucide-icon"), true);
        ctest_equal_bool("should use a namespaced Lucide icon class",
                         ui_widget_has_class(icon, "lucide-icon-access-time"),
                         true);
        ctest_equal_bool("should use a namespaced Lucide glyph size class",
                         ui_widget_has_class(icon, "lucide-icon-glyph-20"),
                         true);
        ctest_equal_bool("should omit the old Lucide base class",
                         ui_widget_has_class(icon, "lui-icon-regular"), false);
        ctest_equal_bool("should ignore filled for Lucide",
                         ui_widget_has_class(icon, "filled"), false);
        ctest_equal_bool("should omit the old Lucide glyph class",
                         ui_widget_has_class(icon, "access-time-20-regular"),
                         false);
        ctest_equal_bool("should omit a filled Lucide glyph class",
                         ui_widget_has_class(icon, "access-time-20-filled"),
                         false);

        ui_widget_destroy(icon);
}

static wchar_t get_icon_content(ui_widget_t *icon)
{
        wchar_t content[16] = { 0 };

        ui_text_get_content_w(icon, content, 16);
        return content[0];
}

static void test_content_selection(void)
{
        static const char css[] =
            ".fui-icon { content: \"\\e000\"; }\n"
            ".fui-icon.fui-icon-checkmark { content: \"\\e001\"; }\n"
            ".fui-icon.fui-icon-checkmark.fui-icon-filled { content: "
            "\"\\e101\"; }\n"
            ".fui-icon.fui-icon-checkmark.fui-icon-glyph-48 { content: "
            "\"\\e002\"; }\n"
            ".fui-icon.fui-icon-checkmark.fui-icon-filled.fui-icon-glyph-48 { "
            "content: \"\\e102\"; }\n";
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, "checkmark");
        ui_root_append(icon);
        ui_load_css_string(css, "fluent-icon.css");
        ui_update();
        ctest_equal_int("should use the base regular content",
                        get_icon_content(icon), 0xe001);

        ui_icon_set_filled(icon, true);
        ui_update();
        ctest_equal_int("should switch content with the filled class",
                        get_icon_content(icon), 0xe101);

        ui_icon_set_size(icon, 48);
        ui_update();
        ctest_equal_int("should select the filled size glyph",
                        get_icon_content(icon), 0xe102);

        ui_icon_set_filled(icon, false);
        ui_update();
        ctest_equal_int("should select the regular size glyph",
                        get_icon_content(icon), 0xe002);

        ui_icon_set_size(icon, 0);
        ui_update();
        ctest_equal_bool("should omit a glyph size class in automatic mode",
                         ui_widget_has_class(icon, "fui-icon-glyph-0"), false);
        ctest_equal_int("should use the base glyph in automatic size mode",
                        get_icon_content(icon), 0xe001);
        ui_widget_destroy(icon);
}

void test_icon(void)
{
        lcui_init();

        ctest_describe("icon defaults", test_defaults);
        ctest_describe("fluent icon factory", test_fluent_factory);
        ctest_describe("icon glyph class", test_glyph_class);
        ctest_describe("icon library and fill", test_library_and_fill);
        ctest_describe("icon Lucide classes", test_lucide_class);
        ctest_describe("icon CSS content", test_content_selection);

        lcui_destroy();
}
