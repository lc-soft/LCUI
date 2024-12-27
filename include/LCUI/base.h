/*
 * include/LCUI/base.h
 *
 * Copyright (c) 2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_LCUI_BASE_H
#define LCUI_INCLUDE_LCUI_BASE_H

#include "common.h"

LCUI_BEGIN_HEADER

LCUI_API const char *lcui_get_version(void);

LCUI_API void lcui_init(void);

LCUI_API void lcui_destroy(void);

LCUI_API int lcui_run(void);

LCUI_API int lcui_main(void);

LCUI_API void lcui_exit(int code);

LCUI_API void lcui_quit(void);

LCUI_END_HEADER

#endif
