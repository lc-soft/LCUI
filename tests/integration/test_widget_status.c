/*
 * tests/integration/test_widget_status.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

/*
 * Status changes should only request a style refresh when a matching
 * :hover / :active / :focus rule exists.  A matched visual rule must not
 * reflow the widget or its parent, while a matched `content` rule on a Text
 * widget must update the text and reflow the affected subtree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <wchar.h>

static const char *widget_status_css = "\
.status-host {\
  display: block;\
  width: 200px;\
  height: 100px;\
}\
.status-plain {\
  display: block;\
  width: 80px;\
  height: 20px;\
  background-color: #101010;\
}\
.status-target {\
  display: block;\
  width: 80px;\
  height: 20px;\
  background-color: #111111;\
}\
.status-target:hover {\
  background-color: #222222;\
}\
.status-target:active {\
  background-color: #333333;\
}\
.status-target:focus {\
  background-color: #444444;\
}\
.status-text-parent {\
  display: block;\
  width: 120px;\
  height: auto;\
}\
.status-text {\
  display: block;\
  width: 80px;\
  white-space: normal;\
  font-size: 14px;\
  line-height: 20px;\
}\
.status-text:hover {\
  content: \"A much longer hover label that wraps\";\
}\
";

static void settle(void)
{
        int i;

        for (i = 0; i < 4; ++i) {
                ui_update();
        }
}

static ui_widget_t *create_status_widget(const char *host_class,
                                         const char *target_class,
                                         ui_widget_t **parent_out)
{
        ui_widget_t *parent = ui_create_widget(NULL);
        ui_widget_t *target = ui_create_widget(NULL);

        ui_widget_add_class(parent, host_class);
        ui_widget_add_class(target, target_class);
        ui_widget_append(parent, target);
        ui_widget_append(ui_root(), parent);
        if (parent_out) {
                *parent_out = parent;
        }
        return target;
}

static void destroy_status_widget(ui_widget_t *w)
{
        ui_widget_destroy(w->parent);
        ui_clear_trash();
}

static void test_unmatched_status_does_not_refresh_style(void)
{
        ui_widget_t *parent = NULL;
        ui_widget_t *target =
            create_status_widget("status-host", "status-plain", &parent);
        const char *statuses[] = { "hover", "active", "focus" };
        int i;

        settle();
        for (i = 0; i < 3; ++i) {
                ui_widget_add_status(target, statuses[i]);
                ctest_equal_bool("unmatched status should not request a target "
                                 "style refresh",
                                 target->update.should_refresh_style, false);
                ctest_equal_bool(
                    "unmatched status should not request a target update",
                    target->update.should_update_self, false);
                ctest_equal_bool(
                    "unmatched status should not request a parent reflow",
                    parent->update.should_reflow, false);
                settle();

                ui_widget_remove_status(target, statuses[i]);
                ctest_equal_bool("removing an unmatched status should not "
                                 "refresh the target",
                                 target->update.should_refresh_style, false);
                ctest_equal_bool(
                    "removing an unmatched status should not request a parent "
                    "reflow",
                    parent->update.should_reflow, false);
                settle();
        }

        destroy_status_widget(target);
}

static void test_matched_status_refreshes_without_reflow(void)
{
        ui_widget_t *parent = NULL;
        ui_widget_t *target =
            create_status_widget("status-host", "status-target", &parent);
        const struct {
                const char *name;
                css_color_value_t color;
        } statuses[] = {
                { "hover", css_color(255, 0x22, 0x22, 0x22) },
                { "active", css_color(255, 0x33, 0x33, 0x33) },
                { "focus", css_color(255, 0x44, 0x44, 0x44) },
        };
        float width;
        float height;
        float parent_height;
        int i;

        settle();
        width = target->border_box.width;
        height = target->border_box.height;
        parent_height = parent->border_box.height;
        for (i = 0; i < 3; ++i) {
                ui_widget_add_status(target, statuses[i].name);
                ctest_equal_bool(
                    "matched status should request a target style refresh",
                    target->update.should_refresh_style, true);
                ctest_equal_bool(
                    "matched status should not request a target reflow",
                    target->update.should_reflow, false);
                ctest_equal_bool(
                    "matched status should not request a parent reflow",
                    parent->update.should_reflow, false);
                settle();
                ctest_equal_int("matched status should apply its style",
                                target->computed_style.background_color,
                                statuses[i].color);
                ctest_equal_float("matched status should keep the target width",
                                  target->border_box.width, width);
                ctest_equal_float(
                    "matched status should keep the target height",
                    target->border_box.height, height);
                ctest_equal_float(
                    "matched status should keep the parent height",
                    parent->border_box.height, parent_height);

                ui_widget_remove_status(target, statuses[i].name);
                ctest_equal_bool(
                    "removing a matched status should request a style refresh",
                    target->update.should_refresh_style, true);
                ctest_equal_bool(
                    "removing a matched status should not request a target "
                    "reflow",
                    target->update.should_reflow, false);
                ctest_equal_bool(
                    "removing a matched status should not request a parent "
                    "reflow",
                    parent->update.should_reflow, false);
                settle();
                ctest_equal_int("removing a matched status should restore the "
                                "base style",
                                target->computed_style.background_color,
                                css_color(255, 0x11, 0x11, 0x11));
                ctest_equal_float("removing a matched status should keep the "
                                  "target width",
                                  target->border_box.width, width);
                ctest_equal_float("removing a matched status should keep the "
                                  "parent height",
                                  parent->border_box.height, parent_height);
        }

        destroy_status_widget(target);
}

static void test_status_content_change_reflows_text(void)
{
        ui_widget_t *parent = NULL;
        ui_widget_t *text;
        wchar_t content[128];
        float text_height;
        float parent_height;

        parent = ui_create_widget(NULL);
        text = ui_create_widget("text");
        ui_widget_add_class(parent, "status-text-parent");
        ui_widget_add_class(text, "status-text");
        ui_text_set_content(text, "Short");
        ui_widget_append(parent, text);
        ui_widget_append(ui_root(), parent);
        settle();

        text_height = text->border_box.height;
        parent_height = parent->border_box.height;
        ui_widget_add_status(text, "hover");
        ctest_equal_bool("content status should request a text style refresh",
                         text->update.should_refresh_style, true);
        settle();

        ui_text_get_content_w(text, content, 128);
        ctest_equal_bool(
            "content status should update the text",
            wcscmp(content, L"A much longer hover label that wraps") == 0,
            true);
        ctest_equal_bool("content status should grow the text height",
                         text->border_box.height > text_height, true);
        ctest_equal_bool("content status should reflow the parent height",
                         parent->border_box.height > parent_height, true);

        ui_widget_remove_status(text, "hover");
        settle();
        ui_text_get_content_w(text, content, 128);
        ctest_equal_bool("removing the content status should restore the text",
                         wcscmp(content, L"Short") == 0, true);
        ctest_equal_float("removing the content status should restore the text "
                          "height",
                          text->border_box.height, text_height);
        ctest_equal_float("removing the content status should restore the "
                          "parent height",
                          parent->border_box.height, parent_height);

        destroy_status_widget(text);
}

void test_widget_status(void)
{
        lcui_init();
        ui_load_css_string(widget_status_css, __FILE__);
        ui_widget_resize(ui_root(), 400, 300);

        ctest_describe("widget status unmatched",
                       test_unmatched_status_does_not_refresh_style);
        ctest_describe("widget status matched",
                       test_matched_status_refreshes_without_reflow);
        ctest_describe("widget status content",
                       test_status_content_change_reflows_text);

        lcui_destroy();
}
