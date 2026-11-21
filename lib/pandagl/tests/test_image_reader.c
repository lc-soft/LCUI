/*
 * lib/pandagl/tests/test_image_reader.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pandagl.h>
#include <yutil.h>
#include <ctest.h>

void test_pandagl_image_reader(void)
{
        pd_canvas_t img;
        int i;
        char file[256], *formats[] = { "png", "bmp", "jpg" };

        for (i = 0; i < 3; ++i) {
                pd_canvas_init(&img);
                snprintf(file, 255, "test_image_reader.%s", formats[i]);
                logger_debug("image file: %s\n", file);
                ctest_equal_int("should read image from file successfully",
                                pd_read_image_from_file(file, &img), 0);
                ctest_equal_int("should have correct image width", img.width,
                                91);
                ctest_equal_int("should have correct image height", img.height,
                                69);
                pd_canvas_destroy(&img);
        }
}

static size_t read_fixture(const char *filename, unsigned char **data)
{
        FILE *fp;
        long size;
        size_t count;

        *data = NULL;
        fp = fopen(filename, "rb");
        if (!fp) {
                return 0;
        }
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        rewind(fp);
        if (size <= 0) {
                fclose(fp);
                return 0;
        }
        *data = malloc((size_t)size);
        if (!*data) {
                fclose(fp);
                return 0;
        }
        count = fread(*data, 1, (size_t)size, fp);
        fclose(fp);
        return count;
}

void test_pandagl_image_memory_reader(void)
{
        pd_canvas_t img;
        pd_image_reader_t *reader;
        unsigned char *data = NULL;
        size_t size;

        size = read_fixture("test_image_reader.png", &data);
        ctest_equal_bool("should load fixture into memory", size > 0, true);
        reader = pd_image_reader_create_from_memory(data, size);
        ctest_equal_bool("should create image reader from memory",
                         reader != NULL, true);
        ctest_equal_int("should detect PNG from memory",
                        pd_image_reader_read_header(reader), PD_OK);
        pd_canvas_init(&img);
        ctest_equal_int("should decode PNG from memory",
                        pd_image_reader_read_data(reader, &img), PD_OK);
        ctest_equal_int("should have fixture width", (int)img.width, 91);
        ctest_equal_int("should have fixture height", (int)img.height, 69);
        pd_canvas_destroy(&img);
        pd_image_reader_destroy(reader);
        free(data);

        ctest_equal_bool("should reject empty memory reader",
                         pd_image_reader_create_from_memory(NULL, 0) == NULL,
                         true);
}
