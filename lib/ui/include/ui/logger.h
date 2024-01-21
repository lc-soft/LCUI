/*
 * lib/ui/include/ui/logger.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_LOGGER_H
#define LIB_UI_INCLUDE_UI_LOGGER_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC int ui_logger_log(logger_level_e level, ui_widget_t *w,
			       const char *fmt, ...);

#define ui_debug(W, ...) ui_logger_log(LOGGER_LEVEL_DEBUG, W, ##__VA_ARGS__)

LIBUI_END_DECLS

#endif
