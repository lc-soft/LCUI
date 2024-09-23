/*
 * tests/cases/test_clipboard.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <LCUI.h>
#include <ctest-custom.h>

static void paste_text(void *arg)
{
        ui_widget_t *w = arg;
        ptk_event_t ev = { 0 };

        ev.type = PTK_EVENT_KEYDOWN;
        ev.key.code = KEY_V;
        ev.key.ctrl_key = true;
        ui_set_focus(w);
        ptk_post_event(&ev);
}

static void on_text1_focused(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ptk_event_t ev = { 0 };

        ev.type = PTK_EVENT_KEYDOWN;
        ev.key.code = KEY_C;
        ev.key.ctrl_key = true;
        ptk_post_event(&ev);
        ptk_set_timeout(100, paste_text, e->data);
}

static void focus_text1(void *arg)
{
        ui_set_focus(arg);
}

static void on_check_text(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ptk_clipboard_t *clipboard = arg;
        char actual_text[32] = "none";

        if (clipboard && clipboard->text) {
                encode_utf8(actual_text, clipboard->text, 31);
        }
        ctest_equal_str("check the pasted text", actual_text, "helloworld");
        lcui_quit();
}

void test_clipboard(void)
{
        ui_widget_t *text1, *text2;

        lcui_init();

        text1 = ui_create_widget("textinput");
        text2 = ui_create_widget("textinput");
        ui_root_append(text1);
        ui_root_append(text2);
        ui_textinput_set_text_w(text1, L"helloworld");
        ui_widget_on(text1, "focus", on_text1_focused, text2);
        ui_widget_on(text2, "paste", on_check_text, NULL);
        ptk_set_timeout(500, focus_text1, text1);
        lcui_main();
}
