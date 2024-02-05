/*
 * lib/pandagl/src/image/png.c: -- LCUI PNG image file processing module.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pandagl.h>
#include "png_private.h"

#ifdef PANDAGL_HAS_LIBPNG
#include <png.h>

#define PNG_BYTES_TO_CHECK 4

typedef struct pd_png_reader_t {
        pd_error_t error;
        pd_file_reader_t *file_reader;
        png_structp png_ptr;
        png_infop info_ptr;
        jmp_buf *env;
        jmp_buf env_src;
} pd_png_reader_t;

static void pd_png_reader_on_read(png_structp png_ptr, png_bytep buffer,
                                  png_size_t size)
{
        size_t read_size;
        pd_image_reader_t *reader = png_get_io_ptr(png_ptr);
        pd_png_reader_t *png_reader = reader->reader_data;

        if (!reader || !png_reader || png_reader->error != PD_OK) {
                return;
        }
        read_size = pd_file_reader_read(reader->file_reader, buffer, size);
        if (read_size != size) {
                png_reader->error = PD_ERROR_IMAGE_HEADER_INVALID;
        }
}

void pd_png_reader_create(pd_image_reader_t *reader)
{
        png_structp png_ptr;
        png_infop info_ptr;
        pd_png_reader_t *png_reader = reader->reader_data;

        png_ptr =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
                return;
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
                png_destroy_read_struct(&png_ptr, (png_infopp)NULL,
                                        (png_infopp)NULL);
                return;
        }
        png_set_read_fn(png_ptr, reader, pd_png_reader_on_read);
        png_reader = calloc(1, sizeof(pd_png_reader_t));
        png_reader->png_ptr = png_ptr;
        png_reader->info_ptr = info_ptr;
        reader->reader_data = png_reader;
}

void pd_png_reader_destroy(pd_image_reader_t *reader)
{
        pd_png_reader_t *png_reader = reader->reader_data;
        if (png_reader->png_ptr) {
                png_destroy_read_struct(&png_reader->png_ptr,
                                        &png_reader->info_ptr, NULL);
        }
        png_reader->png_ptr = NULL;
        free(png_reader);
}

void pd_png_reader_start(pd_image_reader_t *reader)
{
        pd_png_reader_t *png_reader = reader->reader_data;
        reader->passes = png_set_interlace_handling(png_reader->png_ptr);
        png_set_bgr(png_reader->png_ptr);
        png_set_expand(png_reader->png_ptr);
        png_read_update_info(png_reader->png_ptr, png_reader->info_ptr);
}

void pd_png_reader_finish(pd_image_reader_t *reader)
{
        pd_png_reader_t *png_reader = reader->reader_data;
        png_read_end(png_reader->png_ptr, png_reader->info_ptr);
}

jmp_buf *pd_png_reader_jmpbuf(pd_image_reader_t *reader)
{
        pd_png_reader_t *png_reader = reader->reader_data;
#ifdef PNG_SETJMP_SUPPORTED
        return png_set_longjmp_fn(png_reader->png_ptr, longjmp,
                                  sizeof(jmp_buf));
#else
        return NULL;
#endif
}

pd_error_t pd_png_reader_read_header(pd_image_reader_t *reader)
{
        size_t n;
        pd_png_reader_t *png_reader = reader->reader_data;
        png_infop info_ptr = png_reader->info_ptr;
        png_structp png_ptr = png_reader->png_ptr;
        png_byte buf[PNG_BYTES_TO_CHECK];

        n = pd_file_reader_read(reader->file_reader, buf, PNG_BYTES_TO_CHECK);
        if (n < PNG_BYTES_TO_CHECK) {
                return PD_ERROR_IMAGE_HEADER_INVALID;
        }
        /* 检测数据是否为PNG的签名 */
        if (!png_check_sig(buf, PNG_BYTES_TO_CHECK)) {
                return PD_ERROR_IMAGE_TYPE_INCORRECT;
        }
        if (setjmp(png_jmpbuf(png_ptr))) {
                return PD_ERROR_IMAGE_READING;
        }
        png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);
        /* 读取PNG图片信息 */
        png_read_info(png_ptr, info_ptr);
        reader->header.width = png_get_image_width(png_ptr, info_ptr);
        reader->header.height = png_get_image_height(png_ptr, info_ptr);
        reader->header.bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        reader->header.color_type = png_get_color_type(png_ptr, info_ptr);
        reader->header.type = PD_PNG_IMAGE;
        switch (reader->header.color_type) {
        case PNG_COLOR_TYPE_RGB_ALPHA:
                reader->header.color_type = PD_COLOR_TYPE_ARGB;
                break;
        case PNG_COLOR_TYPE_RGB:
                reader->header.color_type = PD_COLOR_TYPE_RGB;
                break;
        default:
                return PD_ERROR_IMAGE_DATA_NOT_SUPPORTED;
        }
        return PD_OK;
}

void pd_png_reader_read_row(pd_image_reader_t *reader, pd_canvas_t *data)
{
        pd_png_reader_t *png_reader = reader->reader_data;
        png_read_row(png_reader->png_ptr,
                     data->bytes + reader->read_row_index * data->bytes_per_row,
                     NULL);
}

int pd_write_png_file(const char *file_name, const pd_canvas_t *data)
{
        png_byte color_type;
        png_structp png_ptr;
        png_infop info_ptr;
        png_bytep *row_pointers;

        FILE *fp;
        unsigned char *p;
        int y, x;
        size_t row_size;
        pd_rect_t rect;

        if (!pd_canvas_is_valid(data)) {
                logger_error("canvas is not valid\n");
                return -1;
        }
        /* create file */
        fp = fopen(file_name, "wb");
        if (!fp) {
                logger_error("file %s could not be opened for writing\n",
                             file_name);
                return -1;
        }
        /* initialize stuff */
        png_ptr =
            png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
                fclose(fp);
                logger_error("png_create_write_struct failed\n");
                return -1;
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
                fclose(fp);
                logger_error("png_create_info_struct failed\n");
                png_destroy_write_struct(&png_ptr, &info_ptr);
                return -1;
        }
        if (setjmp(png_jmpbuf(png_ptr))) {
                fclose(fp);
                logger_error("error during init_io\n");
                png_destroy_write_struct(&png_ptr, &info_ptr);
                return -1;
        }
        png_init_io(png_ptr, fp);
        if (data->color_type == PD_COLOR_TYPE_ARGB8888) {
                color_type = PNG_COLOR_TYPE_RGB_ALPHA;
        } else {
                color_type = PNG_COLOR_TYPE_RGB;
        }
        /* write header */
        png_set_IHDR(png_ptr, info_ptr, data->width, data->height, 8,
                     color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                     PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);
        /* write bytes */

        pd_canvas_get_quote_rect(data, &rect);
        data = pd_canvas_get_quote_source_readonly(data);
        row_size = png_get_rowbytes(png_ptr, info_ptr);
        row_pointers = (png_bytep *)malloc(rect.height * sizeof(png_bytep));

        for (y = 0; y < rect.height; ++y) {
                p = pd_canvas_pixel_at(data, rect.x, rect.y + y);
                row_pointers[y] = png_malloc(png_ptr, row_size);
                if (data->color_type == PD_COLOR_TYPE_ARGB) {
                        for (x = 0; x < row_size; p += 4) {
                                row_pointers[y][x++] = p[2];    // r
                                row_pointers[y][x++] = p[1];    // g
                                row_pointers[y][x++] = p[0];    // b
                                row_pointers[y][x++] = p[3];    // a
                        }
                } else {
                        for (x = 0; x < row_size; p += 3) {
                                row_pointers[y][x++] = p[2];    // r
                                row_pointers[y][x++] = p[1];    // g
                                row_pointers[y][x++] = p[0];    // b
                        }
                }
        }
        png_write_image(png_ptr, row_pointers);
        /* cleanup heap allocation */
        for (y = 0; y < rect.height; ++y) {
                free(row_pointers[y]);
        }
        free(row_pointers);
        /* end write */
        png_write_end(png_ptr, NULL);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return PD_OK;
}

#endif
