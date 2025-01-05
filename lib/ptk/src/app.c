/*
 * lib/ptk/src/app.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk/app.h"
#include "ptk/events.h"
#include "ime.h"
#include "events.h"
#include "clipboard.h"

int ptk_init(const wchar_t *name)
{
        if (ptk_app_init(name) != 0) {
                return -1;
        }
        ptk_ime_init();
        ptk_events_init();
        ptk_clipboard_init();
        return 0;
}

int ptk_run(void)
{
        return ptk_process_native_events(PTK_PROCESS_EVENTS_UNTIL_QUIT);
}

void ptk_destroy(void)
{
        ptk_clipboard_destroy();
        ptk_events_destroy();
        ptk_ime_destroy();
        ptk_app_destroy();
}
