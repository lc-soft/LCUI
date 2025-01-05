/*
 * lib/pandagl/src/font/bitmap.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

/** 初始化字体位图 */
void pd_font_bitmap_init(pd_font_bitmap_t *bitmap);

/** 释放字体位图占用的资源 */
void pd_font_bitmap_destroy(pd_font_bitmap_t *bitmap);
