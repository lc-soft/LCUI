/*
 * include/LCUI/fonts.h
 *
 * Copyright (c) 2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_FONTS_H
#define LCUI_INCLUDE_LCUI_FONTS_H

#include "common.h"

LCUI_BEGIN_HEADER

LCUI_API bool lcui_fonts_set_default(const char *family_name);

#define lcui_set_default_font lcui_fonts_set_default

#endif
