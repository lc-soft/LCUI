/*
 * lib/pandagl/src/image/bmp_private.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

void pd_bmp_reader_create(pd_image_reader_t *reader);
void pd_bmp_reader_destroy(pd_image_reader_t *reader);
jmp_buf *pd_bmp_reader_jmpbuf(pd_image_reader_t *reader);
pd_error_t pd_bmp_reader_read_header(pd_image_reader_t *reader);
void pd_bmp_reader_start(pd_image_reader_t *reader);
void pd_bmp_reader_read_row(pd_image_reader_t *reader, pd_canvas_t *graph);
void pd_bmp_reader_finish(pd_image_reader_t *reader);
