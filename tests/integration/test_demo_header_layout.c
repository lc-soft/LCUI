/*
 * tests/integration/test_demo_header_layout.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * The doc-viewer demo code panel header is a flex row holding the file tabs
 * (`.demo-files`, flex: 1) and the language tabs (`.demo-languages`).
 * Selecting a language swaps which file tabs are visible: C shows one file,
 * XML shows `ui.xml` + `main.c`.
 *
 * A regression here lets the wider file group squeeze the language group, so
 * the language tabs wrap onto two rows and TSX drops to a second line.  These
 * tests rebuild the real header markup against the doc-viewer's generated CSS
 * and pin the language tabs to one row for every selection and width.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <stdio.h>
#include <string.h>
#include "../../examples/doc-viewer/app/global.css.h"

#define DEMO_BLOCK_WIDTH 736.0f

typedef struct {
        ui_widget_t *block;
        ui_widget_t *header;
        ui_widget_t *files;
        ui_widget_t *languages;
        ui_widget_t *file_tabs[5];
        ui_widget_t *language_tabs[3];
} demo_header_scene_t;

static demo_header_scene_t *demo_header_scene;

static ui_widget_t *create_box(const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(w, class_name);
        return w;
}

/* `width <= 0` keeps the tab sized by its text, which is how the real page
 * behaves.  A positive width pins the tab so a test can reproduce the layout
 * without depending on font metrics. */
static ui_widget_t *create_tab(const char *label, const char *language,
                               const char *value, float width)
{
        ui_widget_t *tab = ui_create_widget("text");

        ui_widget_add_class(tab, "demo-tab");
        ui_text_set_content(tab, label);
        if (width > 0) {
                char buf[32];
                snprintf(buf, sizeof(buf), "%.0fpx", width);
                ui_widget_set_style_string(tab, "width", buf);
                ui_widget_set_style_string(tab, "box-sizing", "border-box");
        }
        if (language) {
                ui_widget_set_attr(tab, "data-language", language);
        }
        ui_widget_set_attr(tab, "data-value", value);
        return tab;
}

static void demo_header_scene_build(float block_width, bool fixed_tab_width)
{
        static demo_header_scene_t scene;
        static const char *files[][2] = {
                { "c", "main.c" },   { "xml", "ui.xml" },
                { "xml", "main.c" }, { "tsx", "example.tsx" },
                { "tsx", "main.c" },
        };
        static const float file_widths[] = { 60.0f, 60.0f, 60.0f, 90.0f,
                                             60.0f };
        static const char *languages[] = { "C", "XML", "TSX" };
        static const char *language_values[] = { "c", "xml", "tsx" };
        ui_widget_t *copy;
        char buf[32];
        int i;

        demo_header_scene = &scene;
        scene.block = create_box("code-block");
        scene.header = create_box("code-block-header");
        scene.files = create_box("demo-files");
        scene.languages = create_box("demo-languages");

        for (i = 0; i < 5; ++i) {
                scene.file_tabs[i] =
                    create_tab(files[i][1], files[i][0], files[i][1],
                               fixed_tab_width ? file_widths[i] : 0);
                ui_widget_append(scene.files, scene.file_tabs[i]);
        }
        for (i = 0; i < 3; ++i) {
                scene.language_tabs[i] =
                    create_tab(languages[i], NULL, language_values[i],
                               fixed_tab_width ? 40.0f : 0);
                ui_widget_append(scene.languages, scene.language_tabs[i]);
        }
        copy = create_box("code-block-copy");

        ui_widget_append(scene.header, scene.files);
        ui_widget_append(scene.header, scene.languages);
        ui_widget_append(scene.header, copy);
        ui_widget_append(scene.block, scene.header);
        ui_widget_append(ui_root(), scene.block);
        snprintf(buf, sizeof(buf), "%.0fpx", block_width);
        ui_widget_set_style_string(scene.block, "width", buf);
        ui_widget_set_style_string(scene.block, "box-sizing", "border-box");
}

static void demo_header_scene_destroy(void)
{
        ui_widget_destroy(demo_header_scene->block);
        demo_header_scene = NULL;
}

/* Mirrors demo_provider_update(): show the file tabs of one language and
 * mark the matching tabs active.  The first file of the language becomes the
 * active file, which is what the provider does when the previous file does
 * not exist in the new language. */
static void demo_header_scene_select(const char *language)
{
        ui_widget_t *tab;
        int i;

        for (i = 0; i < 3; ++i) {
                tab = demo_header_scene->language_tabs[i];
                if (strcmp(ui_widget_get_attr(tab, "data-value"), language) ==
                    0) {
                        ui_widget_add_class(tab, "active");
                } else {
                        ui_widget_remove_class(tab, "active");
                }
        }
        for (i = 0; i < 5; ++i) {
                const char *tab_language;
                bool first;

                tab = demo_header_scene->file_tabs[i];
                tab_language = ui_widget_get_attr(tab, "data-language");
                first =
                    i == 0 || strcmp(ui_widget_get_attr(
                                         demo_header_scene->file_tabs[i - 1],
                                         "data-language"),
                                     tab_language) != 0;
                if (strcmp(tab_language, language) == 0) {
                        ui_widget_show(tab);
                        if (first) {
                                ui_widget_add_class(tab, "active");
                        } else {
                                ui_widget_remove_class(tab, "active");
                        }
                } else {
                        ui_widget_hide(tab);
                        ui_widget_remove_class(tab, "active");
                }
        }
        ui_update();
}

static void demo_header_scene_update(void)
{
        ui_widget_resize(ui_root(), 1024.0f, 800.0f);
        ui_update();
        ui_update();
}

static bool tabs_share_one_row(ui_widget_t **tabs, int count)
{
        float y = tabs[0]->outer_box.y;
        int i;

        for (i = 1; i < count; ++i) {
                if (tabs[i]->outer_box.y - y > 1.0f ||
                    y - tabs[i]->outer_box.y > 1.0f) {
                        return false;
                }
        }
        return true;
}

static void check_language_tabs_stay_on_one_row(void)
{
        static const char *languages[] = { "c", "xml", "tsx", "xml" };
        int i;

        for (i = 0; i < (int)(sizeof(languages) / sizeof(languages[0])); ++i) {
                demo_header_scene_select(languages[i]);
                ctest_equal_bool(
                    "language tabs should share one row",
                    tabs_share_one_row(demo_header_scene->language_tabs, 3),
                    true);
        }
}

static void test_text_sized_tabs_share_one_row(void)
{
        check_language_tabs_stay_on_one_row();
}

static void test_fixed_tabs_share_one_row(void)
{
        check_language_tabs_stay_on_one_row();
}

static void test_narrow_header_keeps_language_tabs(void)
{
        check_language_tabs_stay_on_one_row();
}

static void test_language_group_height_stays_one_line(void)
{
        demo_header_scene_select("c");
        ctest_equal_bool(
            "language group should be one line tall for C",
            demo_header_scene->languages->border_box.height < 56.0f, true);
        demo_header_scene_select("xml");
        ctest_equal_bool(
            "language group should be one line tall for XML",
            demo_header_scene->languages->border_box.height < 56.0f, true);
}

static void test_language_group_keeps_content_width(void)
{
        float c_width;
        float xml_width;

        demo_header_scene_select("c");
        c_width = demo_header_scene->languages->border_box.width;
        demo_header_scene_select("xml");
        xml_width = demo_header_scene->languages->border_box.width;

        ctest_equal_bool(
            "language group should not shrink when XML shows two files",
            xml_width >= c_width - 1.0f, true);
}

static void test_file_group_stays_inside_header(void)
{
        float header_right;
        float file_right;

        demo_header_scene_select("xml");
        header_right = demo_header_scene->header->border_box.x +
                       demo_header_scene->header->border_box.width;
        file_right = demo_header_scene->files->border_box.x +
                     demo_header_scene->files->border_box.width;

        ctest_equal_bool("file group should stay inside the header",
                         file_right <= header_right + 1.0f, true);
        ctest_equal_bool(
            "file group should not overlap the language group",
            file_right <= demo_header_scene->languages->border_box.x + 1.0f,
            true);
}

static void with_scene(float block_width, bool fixed_tab_width,
                       void (*check)(void))
{
        demo_header_scene_build(block_width, fixed_tab_width);
        demo_header_scene_update();
        check();
        demo_header_scene_destroy();
        ui_update();
}

static void run_text_sized_tabs(void)
{
        with_scene(DEMO_BLOCK_WIDTH, false, test_text_sized_tabs_share_one_row);
}

static void run_fixed_tabs(void)
{
        with_scene(DEMO_BLOCK_WIDTH, true, test_fixed_tabs_share_one_row);
}

static void run_narrow_header(void)
{
        with_scene(360.0f, true, test_narrow_header_keeps_language_tabs);
}

static void run_language_height(void)
{
        with_scene(DEMO_BLOCK_WIDTH, true,
                   test_language_group_height_stays_one_line);
}

static void run_language_width(void)
{
        with_scene(DEMO_BLOCK_WIDTH, true,
                   test_language_group_keeps_content_width);
}

static void run_file_width(void)
{
        with_scene(DEMO_BLOCK_WIDTH, true, test_file_group_stays_inside_header);
}

void test_demo_header_layout(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *win;

        lcui_init();
        ui_load_css_string_global_chunks();
        ui_widget_set_title(ui_root(), L"test demo header layout");
        win = lcui_window_create_for_widget(ui_root(), &opt);
        ctest_equal_bool("should create a window", win != NULL, true);

        ctest_describe("demo header text sized tabs", run_text_sized_tabs);
        ctest_describe("demo header fixed tabs", run_fixed_tabs);
        ctest_describe("demo header narrow width", run_narrow_header);
        ctest_describe("demo header language height", run_language_height);
        ctest_describe("demo header language width", run_language_width);
        ctest_describe("demo header file width", run_file_width);

        lcui_destroy();
}
