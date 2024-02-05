/*
 * lib/pandagl/include/pandagl/image.h: -- Image read and write operations set
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_IMAGE_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_IMAGE_H

#include <setjmp.h>
#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

typedef enum pd_image_reader_type_t {
        PD_UNKNOWN_READER,
        PD_PNG_READER,
        PD_JPEG_READER,
        PD_BMP_READER,
        PD_READER_COUNT
} pd_image_reader_type_t;

#define PD_UNKNOWN_IMAGE PD_UNKNOWN_READER
#define PD_PNG_IMAGE PD_PNG_READER
#define PD_JPEG_IMAGE PD_JPEG_READER
#define PD_BMP_IMAGE PD_BMP_READER

#define pd_image_reader_set_jump(READER) \
        (READER)->env &&setjmp(*((READER)->env))

typedef struct pd_image_header_t {
        int type;
        int bit_depth;
        pd_color_type_t color_type;
        unsigned int width, height;
} pd_image_header_t;

typedef struct pd_image_reader_t {
        char error_message[256];
        pd_image_reader_type_t type;
        pd_file_reader_t *file_reader;
        pd_image_header_t header;
        unsigned read_row_index;
        unsigned pass;
        unsigned passes;
        void *reader_data;
} pd_image_reader_t;

PD_PUBLIC pd_image_reader_t *pd_image_reader_create(void);
PD_PUBLIC pd_image_reader_t *pd_image_reader_create_from_file(
    const char *filename);

PD_PUBLIC void pd_image_reader_destroy(pd_image_reader_t *reader);
PD_PUBLIC jmp_buf *pd_image_reader_jmpbuf(pd_image_reader_t *reader);
PD_PUBLIC pd_error_t pd_image_reader_read_header(pd_image_reader_t *reader);
PD_PUBLIC void pd_image_reader_start(pd_image_reader_t *reader);
PD_PUBLIC pd_error_t pd_image_reader_create_buffer(pd_image_reader_t *reader,
                                                   pd_canvas_t *out);
PD_PUBLIC void pd_image_reader_read_row(pd_image_reader_t *reader,
                                        pd_canvas_t *data);
PD_PUBLIC void pd_image_reader_finish(pd_image_reader_t *reader);
PD_PUBLIC pd_error_t pd_image_reader_read_data(pd_image_reader_t *reader,
                                               pd_canvas_t *out);

PD_PUBLIC pd_error_t pd_read_image_from_file(const char *filepath,
                                             pd_canvas_t *out);

PD_PUBLIC int pd_write_png_file(const char *file_name,
                                const pd_canvas_t *graph);

PD_END_DECLS

#endif
