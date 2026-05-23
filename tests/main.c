/*
 * tests/main.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest.h>

extern void test_settings(void);
extern void test_clipboard(void);
extern void test_mainloop(void);
extern void test_router_components(void);
extern void test_widget_event(void);
extern void test_widget_rect(void);
extern void test_text_resize(void);
extern void test_textinput(void);
extern void test_font_face(void);
extern void test_widget_opacity(void);
extern void test_block_layout(void);
extern void test_flex_layout(void);
extern void test_scrollbar(void);

static const ctest_suite_t suites[] = {
        { "settings", test_settings },
        { "clipboard", test_clipboard },
        { "mainloop", test_mainloop },
        { "router components", test_router_components },
        { "widget event", test_widget_event },
        { "widget rect", test_widget_rect },
        { "text resize", test_text_resize },
        { "textinput", test_textinput },
        { "font face", test_font_face },
        { "widget opacity", test_widget_opacity },
        { "block layout", test_block_layout },
        { "flex layout", test_flex_layout },
        { "scrollbar", test_scrollbar },
        { NULL, NULL }
};

int main(int argc, char **argv)
{
        logger_set_level(LOGGER_LEVEL_ERROR);
        if (ctest_parse_args(argc, argv) != 0) {
                return 1;
        }
        return ctest_run_suites(suites);
}
