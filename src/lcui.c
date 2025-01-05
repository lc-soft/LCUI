/*
 * src/lcui.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <time.h>
#include <ptk.h>
#include <LCUI/config.h>
#include <LCUI/app.h>
#include <LCUI/widgets.h>
#include <LCUI/settings.h>

extern void lcui_fonts_init(void);
extern void lcui_widgets_init(void);
extern void lcui_widgets_destroy(void);

const char *lcui_get_version(void)
{
        return PACKAGE_VERSION;
}

void lcui_init(void)
{
        logger_log(LOGGER_LEVEL_INFO,
                   "LCUI (LC's UI) version " PACKAGE_VERSION "\n"
                   "Build at "__DATE__
                   " - "__TIME__
                   "\n"
                   "Copyright (C) 2012-2024 Liu Chao <root@lc-soft.io>.\n"
                   "This is open source software, licensed under MIT. \n"
                   "See source distribution for detailed copyright notices.\n"
                   "To learn more, visit http://www.lcui.org.\n\n");

        lcui_app_init();
        lcui_fonts_init();
        lcui_widgets_init();
        lcui_reset_settings();
}

void lcui_destroy(void)
{
        lcui_widgets_destroy();
        lcui_app_destroy();
}

void lcui_exit(int code)
{
        ptk_app_exit(code);
}

void lcui_quit(void)
{
        lcui_exit(0);
}

int lcui_run(void)
{
        return lcui_app_process_events(PTK_PROCESS_EVENTS_UNTIL_QUIT);
}

int lcui_main(void)
{
        int exit_code = lcui_run();
        lcui_destroy();
        return exit_code;
}
