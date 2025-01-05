/*
 * lib/css/src/debug.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifdef DEBUG
#define DEBUG_MSG _DEBUG_MSG
#else
#define DEBUG_MSG(format, ...)
#endif

#define _DEBUG_MSG(format, ...)                                       \
	logger_log(LOGGER_LEVEL_DEBUG, __FILE__ ":%d: %s(): " format, \
		   __LINE__, __FUNCTION__, ##__VA_ARGS__)
