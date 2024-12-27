/*
 * src/lcui_fonts.c
 *
 * Copyright (c) 2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui.h>
#include <ptk.h>
#include <pandagl.h>

#ifdef PTK_WIN32
static void lcui_windows_fonts_init(void)
{
        size_t i;
        int *ids = NULL;
        const char *names[] = { "Consola", "Simsun", "Microsoft YaHei", NULL };
        const char *fonts[] = { "C:/Windows/Fonts/consola.ttf",
                                "C:/Windows/Fonts/simsun.ttc",
                                "C:/Windows/Fonts/msyh.ttf",
                                "C:/Windows/Fonts/msyh.ttc" };

        for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
                pd_font_library_load_file(fonts[i]);
        }
        i = pd_font_library_query(&ids, PD_FONT_STYLE_NORMAL,
                                  PD_FONT_WEIGHT_NORMAL, names);
        if (i > 0) {
                pd_font_library_set_default_font(ids[i - 1]);
        }
        free(ids);
}

#else

#ifdef HAVE_FONTCONFIG

static void lcui_fc_fonts_init(void)
{
        size_t i;
        char *path;
        int *ids = NULL;
        const char *names[] = { "Noto Sans CJK", "Ubuntu",
                                "WenQuanYi Micro Hei", NULL };
        const char *fonts[] = { "Ubuntu", "Noto Sans CJK SC",
                                "WenQuanYi Micro Hei" };

        for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
                path = pd_font_library_get_font_path(fonts[i]);
                pd_font_library_load_file(path);
                free(path);
        }
        i = pd_font_library_query(&ids, PD_FONT_STYLE_NORMAL,
                                  PD_FONT_WEIGHT_NORMAL, names);
        if (i > 0) {
                pd_font_library_set_default_font(ids[i - 1]);
        }
        free(ids);
}

#else

static void lcui_linux_fonts_init(void)
{
        size_t i;
        int *ids = NULL;
        const char *names[] = { "Noto Sans CJK SC", "Ubuntu", "Ubuntu Mono",
                                "WenQuanYi Micro Hei", NULL };
        const char *fonts[] = {
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-LI.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-L.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-MI.ttf",
                "/usr/share/fonts/truetype/ubuntu/UbuntuMono-BI.ttf",
                "/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf",
                "/usr/share/fonts/truetype/ubuntu/UbuntuMono-RI.ttf",
                "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-RI.ttf",
                "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-RI.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-M.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-MI.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-L.ttf",
                "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-LI.ttf",
                "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
                "/usr/share/fonts/opentype/noto/NotoSansCJK.ttc",
                "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc"
        };

        for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
                pd_font_library_load_file(fonts[i]);
        }
        i = pd_font_library_query(&ids, PD_FONT_STYLE_NORMAL,
                                  PD_FONT_WEIGHT_NORMAL, names);
        if (i > 0) {
                pd_font_library_set_default_font(ids[i - 1]);
        }
        free(ids);
}
#endif

#endif

void lcui_fonts_init(void)
{
#ifdef PTK_WIN32
        lcui_windows_fonts_init();
#elif defined(HAVE_FONTCONFIG)
        logger_debug("[font] fontconfig enabled\n");
        lcui_fc_fonts_init();
#else
        lcui_linux_fonts_init();
#endif
}
