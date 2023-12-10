/*
 * lib/pandagl/src/image/png-private.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

int pd_png_reader_init(pd_image_reader_t* reader);

int pd_png_reader_read_header(pd_image_reader_t* reader);

int pd_png_reader_read_data(pd_image_reader_t* reader, pd_canvas_t *graph);

int pd_write_png_file(const char *file_name, const pd_canvas_t *graph);
