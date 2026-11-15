/*
 * tests/helpers/preview_window.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_TESTS_HELPERS_PREVIEW_WINDOW_H
#define LCUI_TESTS_HELPERS_PREVIEW_WINDOW_H

#include <LCUI.h>

/* Preview binaries build a scene inside ui_root() and then need a native
 * window to look at. Content size is inherited from the root widget. */
static void preview_show_default_window(void)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *window;

        window = lcui_window_create_for_widget(ui_root(), &opt);
        lcui_window_show(window);
}

#endif
