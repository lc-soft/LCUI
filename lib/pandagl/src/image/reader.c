/*
 * lib/pandagl/src/image/reader.c: -- Image read operations set
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pandagl.h>
#include "bmp_private.h"
#include "png_private.h"
#include "jpeg_private.h"

typedef struct {
        const char *suffix;
        pd_image_reader_type_t type;
        void (*create)(pd_image_reader_t *);
        void (*destroy)(pd_image_reader_t *);
        jmp_buf *(*jmpbuf)(pd_image_reader_t *);
        pd_error_t (*reade_header)(pd_image_reader_t *);
        void (*start)(pd_image_reader_t *);
        void (*read_row)(pd_image_reader_t *, pd_canvas_t *);
        void (*finish)(pd_image_reader_t *);
} pd_image_reader_methods_t;

static pd_image_reader_methods_t pd_image_readers[] = {
#ifdef PANDAGL_HAS_LIBPNG
        { ".png", PD_PNG_READER, pd_png_reader_create, pd_png_reader_destroy,
          pd_png_reader_jmpbuf, pd_png_reader_read_header, pd_png_reader_start,
          pd_png_reader_read_row, pd_png_reader_finish },
#endif
#ifdef PANDAGL_HAS_LIBJPEG
        { ".jpeg .jpg", PD_JPEG_READER, pd_jpeg_reader_create,
          pd_jpeg_reader_destroy, pd_jpeg_reader_jmpbuf,
          pd_jpeg_reader_read_header, pd_jpeg_reader_start,
          pd_jpeg_reader_read_row, pd_jpeg_reader_finish },
#endif
        { ".bmp", PD_BMP_READER, pd_bmp_reader_create, pd_bmp_reader_destroy,
          pd_bmp_reader_jmpbuf, pd_bmp_reader_read_header, pd_bmp_reader_start,
          pd_bmp_reader_read_row, pd_bmp_reader_finish },
};

static pd_image_reader_type_t pd_image_reader_detect_suffix(
    const char *filename)
{
        int i;
        const char *suffix = NULL;

        for (i = 0; filename[i]; ++i) {
                if (filename[i] == '.') {
                        suffix = filename + i;
                }
        }
        if (!suffix) {
                return PD_UNKNOWN_READER;
        }
        for (i = 0; i < sizeof(pd_image_readers) / sizeof(pd_image_readers[0]);
             ++i) {
                if (strstr(pd_image_readers[i].suffix, suffix)) {
                        return pd_image_readers[i].type;
                }
        }
        return PD_UNKNOWN_READER;
}

static pd_image_reader_methods_t *pd_image_reader_get_methods(
    pd_image_reader_t *reader)
{
        int i;
        for (i = 0; i < sizeof(pd_image_readers) / sizeof(pd_image_readers[0]);
             ++i) {
                if (pd_image_readers[i].type == reader->type) {
                        return pd_image_readers + i;
                }
        }
        return NULL;
}

pd_image_reader_t *pd_image_reader_create(void)
{
        pd_image_reader_t *reader = calloc(1, sizeof(pd_image_reader_t));
        reader->passes = 1;
        return reader;
}

pd_image_reader_t *pd_image_reader_create_from_file(const char *filename)
{
        pd_image_reader_t *reader;
        pd_file_reader_t *file_reader;

        file_reader = pd_file_reader_create_from_file(filename);
        if (!file_reader) {
                return NULL;
        }
        reader = pd_image_reader_create();
        reader->type = pd_image_reader_detect_suffix(filename);
        reader->file_reader = file_reader;
        return reader;
}

void pd_image_reader_destroy(pd_image_reader_t *reader)
{
        if (reader->reader_data) {
                pd_image_reader_get_methods(reader)->destroy(reader);
        }
        pd_file_reader_destroy(reader->file_reader);
        reader->type = PD_UNKNOWN_READER;
        reader->header.type = PD_UNKNOWN_IMAGE;
        reader->reader_data = NULL;
        reader->file_reader = NULL;
        free(reader);
}

static pd_error_t pd_image_reader_try_read_header(pd_image_reader_t *reader)
{
        pd_error_t err;
        pd_image_reader_methods_t *methods =
            pd_image_reader_get_methods(reader);

        if (methods) {
                methods->create(reader);
                err = methods->reade_header(reader);
                if (err != PD_OK) {
                        methods->destroy(reader);
                        reader->reader_data = NULL;
                }
                return err;
        }
        return PD_ERROR_IMAGE_HEADER_INVALID;
}

pd_error_t pd_image_reader_read_header(pd_image_reader_t *reader)
{
        pd_error_t err;
        pd_image_reader_type_t type = reader->type;
        pd_image_reader_methods_t *methods;

        if (!reader->reader_data) {
                if (reader->type != PD_UNKNOWN_READER &&
                    pd_image_reader_try_read_header(reader) == PD_OK) {
                        return PD_OK;
                }
                type = reader->type;
                for (reader->type = PD_PNG_READER;
                     reader->type < PD_READER_COUNT; ++reader->type) {
                        if (reader->type == type) {
                                continue;
                        }
                        pd_file_reader_rewind(reader->file_reader);
                        err = pd_image_reader_try_read_header(reader);
                        if (err == PD_OK) {
                                break;
                        }
                }
        }
        methods = pd_image_reader_get_methods(reader);
        if (methods) {
                return methods->reade_header(reader);
        }
        reader->type = PD_UNKNOWN_READER;
        return PD_ERROR_IMAGE_TYPE_INCORRECT;
}

pd_error_t pd_image_reader_create_buffer(pd_image_reader_t *reader,
                                         pd_canvas_t *out)
{
        switch (reader->header.color_type) {
        case PD_COLOR_TYPE_ARGB:
                out->color_type = PD_COLOR_TYPE_ARGB;
                break;
        case PD_COLOR_TYPE_RGB:
                out->color_type = PD_COLOR_TYPE_RGB;
                break;
        default:
                return PD_ERROR_IMAGE_DATA_NOT_SUPPORTED;
        }
        return pd_canvas_create(out, reader->header.width,
                                reader->header.height);
}

jmp_buf *pd_image_reader_jmpbuf(pd_image_reader_t *reader)
{
        return pd_image_reader_get_methods(reader)->jmpbuf(reader);
}

void pd_image_reader_start(pd_image_reader_t *reader)
{
        pd_image_reader_get_methods(reader)->start(reader);
}

void pd_image_reader_finish(pd_image_reader_t *reader)
{
        pd_image_reader_get_methods(reader)->finish(reader);
}

void pd_image_reader_read_row(pd_image_reader_t *reader, pd_canvas_t *data)
{
        pd_image_reader_get_methods(reader)->read_row(reader, data);
        reader->read_row_index++;
}

pd_error_t pd_image_reader_read_data(pd_image_reader_t *reader,
                                     pd_canvas_t *out)
{
        pd_image_reader_methods_t *methods =
            pd_image_reader_get_methods(reader);

        if (!methods) {
                return PD_ERROR_IMAGE_TYPE_INCORRECT;
        }
        if (setjmp(*pd_image_reader_jmpbuf(reader))) {
                return PD_ERROR_IMAGE_READING;
        }
        if (pd_image_reader_create_buffer(reader, out) != PD_OK) {
                return PD_ERROR_IMAGE_DATA_NOT_SUPPORTED;
        }
        pd_image_reader_start(reader);
        for (reader->pass = 0; reader->pass < reader->passes; ++reader->pass) {
                reader->read_row_index = 0;
                while (reader->read_row_index < reader->header.height) {
                        pd_image_reader_read_row(reader, out);
                }
        }
        pd_image_reader_finish(reader);
        return PD_OK;
}

pd_error_t pd_read_image_from_file(const char *filepath, pd_canvas_t *out)
{
        pd_error_t err;
        pd_image_reader_t *reader;

        reader = pd_image_reader_create_from_file(filepath);
        if (!reader) {
                return PD_ERROR_NOT_FOUND;
        }
        err = pd_image_reader_read_header(reader);
        if (err == PD_OK) {
                err = pd_image_reader_read_data(reader, out);
        }
        pd_image_reader_destroy(reader);
        return err;
}
