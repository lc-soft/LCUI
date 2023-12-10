/*
 * lib/ui/include/ui/css.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_CSS_H
#define LIB_UI_INCLUDE_UI_CSS_H

#include "common.h"

LIBUI_END_DECLS

LIBUI_PUBLIC int ui_load_css_file(const char *filepath);
LIBUI_PUBLIC size_t ui_load_css_string(const char *str, const char *space);

LIBUI_END_DECLS
#endif
