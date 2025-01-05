/*
 * lib/ptk/src/linux/ime.c: -- The input method engine support for linux.
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk.h"

#ifdef PTK_LINUX
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef PTK_HAS_LIBX11

static bool ptk_x11ime_process_key(int key, bool pressed)
{
        return false;
}

static void ptk_x11ime_to_text(int ch)
{
        wchar_t text[2] = { ch, 0 };
        ptk_ime_commit(text, 2);
}

static void on_key_press(ptk_event_t *e, void *arg)
{
        wchar_t text[2] = { e->key.code, 0 };
        ptk_ime_commit(text, 2);
}

static bool ptk_x11ime_open(void)
{
        ptk_on_event(PTK_EVENT_KEYPRESS, on_key_press, NULL);
        return true;
}

static bool ptk_x11ime_close(void)
{
        ptk_off_event(PTK_EVENT_KEYPRESS, on_key_press);
        return true;
}
#endif

int ptk_ime_add_linux(void)
{
#ifdef PTK_HAS_LIBX11
        ptk_ime_handler_t handler;
        if (ptk_get_app_id() == PTK_APP_ID_LINUX_X11) {
                handler.prockey = ptk_x11ime_process_key;
                handler.totext = ptk_x11ime_to_text;
                handler.close = ptk_x11ime_close;
                handler.open = ptk_x11ime_open;
                handler.setcaret = NULL;
                return ptk_ime_add("LCUI X11 Input Method", &handler);
        }
#endif
        return -1;
}

#endif
