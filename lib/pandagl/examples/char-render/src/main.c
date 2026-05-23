/*
 * lib/pandagl/examples/char-render/src/main.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>

int main(void)
{
        int ret, fid;
        pd_canvas_t img;
        pd_font_bitmap_t bmp;
        pd_pos_t pos = { 25, 25 };
        pd_color_t bg = pd_rgb(240, 240, 240);
        pd_color_t color = pd_rgb(255, 0, 0);

        pd_font_library_init();

        pd_canvas_init(&img);
        pd_canvas_create(&img, 100, 100);
        pd_canvas_fill(&img, bg);

        ret = pd_font_library_load_file("C:/Windows/fonts/simsun.ttc");
        while (ret == 0) {
                fid = pd_font_library_get_font_id("SimSun", 0, 0);
                if (fid < 0) {
                        break;
                }
                ret = pd_font_library_render_bitmap(&bmp, L'字', fid, 48);
                if (ret != 0) {
                        break;
                }
                pd_canvas_mix_font_bitmap(&img, pos, &bmp, color);
                pd_write_png_file("test_char_render.png", &img);
                pd_font_bitmap_destroy(&bmp);
                pd_canvas_destroy(&img);
                break;
        }

        pd_font_library_destroy();
        return ret;
}