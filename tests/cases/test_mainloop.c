/*
 * tests/cases/test_mainloop.c
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

static void handle_refresh(void *arg)
{
        ui_refresh_style();
}

static void handle_quit(void *arg)
{
        lcui_quit();
}

static void handle_btn_click(ui_widget_t *w, ui_event_t *ui_event, void *arg)
{
        lcui_set_timeout(0, handle_refresh, NULL);
        lcui_set_timeout(1, handle_quit, NULL);
        lcui_process_events(PTK_PROCESS_EVENTS_UNTIL_QUIT);
        lcui_quit();
}

static void handle_trigger_btn_click(void *arg)
{
        ptk_event_t e = { 0 };

        e.type = PTK_EVENT_MOUSEDOWN;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        e.mouse.x = 5;
        e.mouse.y = 5;
        ptk_post_event(&e);
        e.type = PTK_EVENT_MOUSEUP;
        ptk_post_event(&e);
}

static void observer_thread(void *arg)
{
        int i;
        bool *exited = arg;

        for (i = 0; i < 10 && !*exited; ++i) {
                sleep_ms(100);
        }
        ctest_equal_bool("main loop should exit within 1000ms", *exited, true);
        if (!*exited) {
                exit(-ctest_finish());
                return;
        }
        thread_exit(NULL);
}

void test_mainloop(void)
{
        thread_t tid;
        ui_widget_t *btn;
        bool exited = false;

        lcui_init();
        btn = ui_create_widget("button");
        ui_button_set_text(btn, "button");
        ui_widget_on(btn, "click", handle_btn_click, NULL);
        ui_root_append(btn);
        /* Observe whether the main loop has exited in a new thread */
        thread_create(&tid, observer_thread, &exited);
        /* Trigger the click event after the first frame is updated */
        lcui_set_timeout(50, handle_trigger_btn_click, btn);
        lcui_main();
        exited = true;
        thread_join(tid, NULL);
}
