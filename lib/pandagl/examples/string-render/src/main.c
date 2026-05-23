/*
 * lib/pandagl/examples/string-render/src/main.c
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
        int ret;
        pd_canvas_t img;
        pd_pos_t pos = { 0, 80 };
        pd_rect_t area = { 0, 0, 320, 240 };
        pd_text_t* txt = pd_text_create();
        pd_text_style_t txtstyle;

        pd_font_library_init();

        pd_canvas_init(&img);
        pd_canvas_create(&img, 320, 240);
        pd_canvas_fill(&img, pd_rgb(240, 240, 240));

        pd_text_style_Init(&txtstyle);
        txtstyle.pixel_size = 24;
        txtstyle.has_pixel_size = true;

        pd_text_set_fixed_size(txt, 320, 240);
        pd_text_set_style(txt, &txtstyle);
        pd_text_set_align(txt, PD_TEXT_ALIGN_CENTER);
        pd_text_write(txt, L"这是一段测试文本\nHello, World!", NULL);
        pd_text_update(txt, NULL);

        pd_text_render_to(txt, area, pos, &img);
        ret = pd_write_png_file("test_string_render.png", &img);
        pd_canvas_destroy(&img);

        pd_font_library_destroy();
        return ret;
}