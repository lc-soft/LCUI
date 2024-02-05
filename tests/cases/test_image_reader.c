/*
 * tests/cases/test_image_reader.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <LCUI.h>
#include <ctest-custom.h>

void test_image_reader(void)
{
        pd_canvas_t img;
        int i;
        char file[256], *formats[] = { "png", "bmp", "jpg" };

        for (i = 0; i < 3; ++i) {
                pd_canvas_init(&img);
                snprintf(file, 255, "test_image_reader.%s", formats[i]);
                logger_debug("image file: %s\n", file);
                ctest_equal_int("check pd_read_image_from_file",
                                pd_read_image_from_file(file, &img), 0);
                ctest_equal_int("check image width with ReadImageFile",
                                img.width, 91);
                ctest_equal_int("check image height with ReadImageFile",
                                img.height, 69);
                pd_canvas_destroy(&img);
        }
}
