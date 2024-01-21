/*
 * lib/pandagl/src/font/library.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

pd_font_t *pd_font_create(const char *family_name, const char *style_name);

void pd_font_destroy(pd_font_t *font);
