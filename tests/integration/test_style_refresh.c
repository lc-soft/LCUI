/*
 * tests/integration/test_style_refresh.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * A status change (hover/focus) only refreshes the matched style.  It must
 * not collapse the used box of auto-sized widgets for one layout pass, or
 * cards resize and text reflows whenever the pointer moves over them.
 */

#include <LCUI.h>
#include <float.h>
#include <ctest-custom.h>

typedef struct {
        float min_width;
        float min_height;
} refresh_probe_t;

static refresh_probe_t probe;

static void record_box(ui_widget_t *w, ui_event_t *e, void *arg)
{
        if (w->content_box.width < probe.min_width) {
                probe.min_width = w->content_box.width;
        }
        if (w->content_box.height < probe.min_height) {
                probe.min_height = w->content_box.height;
        }
}

static ui_widget_t *create_box(const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_add_class(w, class_name);
        return w;
}

static void settle(void)
{
        ui_update();
        ui_update();
        ui_update();
}

static void test_hover_keeps_box(void)
{
        ui_widget_t *parent = create_box("test-refresh-parent");
        ui_widget_t *box = create_box("test-refresh-box");
        ui_widget_t *text = ui_create_widget("text");
        float width;
        float height;

        ui_widget_add_class(text, "test-refresh-text");
        ui_text_set_content(text,
                            "Hovering the card must keep its box stable.");
        ui_widget_append(box, text);
        ui_widget_append(parent, box);
        ui_widget_append(ui_root(), parent);
        ui_widget_resize(ui_root(), 800, 600);
        settle();

        width = box->content_box.width;
        height = box->content_box.height;
        probe.min_width = FLT_MAX;
        probe.min_height = FLT_MAX;
        ui_widget_on(box, "afterlayout", record_box, NULL);

        ui_widget_add_status(box, "hover");
        settle();

        ctest_equal_float("hover keeps the box width", box->content_box.width,
                          width);
        ctest_equal_float("hover keeps the box height", box->content_box.height,
                          height);
        ctest_equal_bool("hover never observes a collapsed width",
                         probe.min_width > width * 0.9f, true);
        ctest_equal_bool("hover never observes a collapsed height",
                         probe.min_height > height * 0.9f, true);

        ui_widget_destroy(parent);
        ui_clear_trash();
}

void test_style_refresh(void)
{
        lcui_init();
        ui_load_css_file("test_style_refresh.css");
        ui_widget_set_title(ui_root(), L"test style refresh");

        ctest_describe("style refresh hover", test_hover_keeps_box);

        lcui_destroy();
}
