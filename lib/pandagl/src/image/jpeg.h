﻿/*
 * lib/pandagl/src/image/jpeg.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

int pd_jpeg_reader_read_header(pd_image_reader_t* reader);

int pd_jpeg_reader_init(pd_image_reader_t* reader);

int pd_jpeg_reader_read_data(pd_image_reader_t* reader, pd_canvas_t *graph);
