/*
 * lib/pandagl/examples/mix-rect-with-opacity/src/main.c
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
        int i, size;
        pd_canvas_t canvas;
        pd_canvas_t fore_canvas;
        pd_rect_t rect;

        pd_canvas_init(&canvas);
        pd_canvas_init(&fore_canvas);
        pd_canvas_create(&canvas, 150, 150);
        rect.x = 0;
        rect.y = 0;
        rect.width = 75;
        rect.height = 75;
        pd_canvas_fill_rect(&canvas, pd_rgb(255, 221, 0), rect);
        rect.x = 75;
        pd_canvas_fill_rect(&canvas, pd_rgb(102, 204, 0), rect);
        rect.x = 0;
        rect.y = 75;
        pd_canvas_fill_rect(&canvas, pd_rgb(0, 153, 255), rect);
        rect.x = 75;
        pd_canvas_fill_rect(&canvas, pd_rgb(255, 51, 0), rect);
        fore_canvas.opacity = 0.2f;
        fore_canvas.color_type = PD_COLOR_TYPE_ARGB;
        for (i = 0; i < 7; ++i) {
                size = 2 * (10 + 10 * i);
                pd_canvas_create(&fore_canvas, size, size);
                pd_canvas_fill(&fore_canvas, pd_rgb(255, 255, 255));
                pd_canvas_mix(&canvas, &fore_canvas, 75 - size / 2, 75 - size / 2,
                          false);
        }
        pd_write_png_file("test_mix_rect_with_opacity.png", &canvas);
        pd_canvas_destroy(&fore_canvas);
        pd_canvas_destroy(&canvas);
        return 0;
}