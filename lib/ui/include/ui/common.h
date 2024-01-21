/*
 * lib/ui/include/ui/common.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_COMMON_H
#define LIB_UI_INCLUDE_UI_COMMON_H

#include "config.h"

#ifdef __cplusplus
#define LIBUI_BEGIN_DECLS extern "C" {
#define LIBUI_END_DECLS }
#else
#define LIBUI_BEGIN_DECLS
#define LIBUI_END_DECLS
#endif

#ifndef LIBUI_PUBLIC
#if defined(_MSC_VER) && !defined(LIBUI_STATIC_BUILD)
#ifdef LIBUI_DLL_EXPORT
#define LIBUI_PUBLIC __declspec(dllexport)
#else
#define LIBUI_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBUI_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBUI_PUBLIC extern
#endif
#endif

#if defined(_WIN32) && !defined(__cplusplus)
#define LIBUI_INLINE __inline
#else
#define LIBUI_INLINE static inline
#endif

#ifdef DEBUG
#define DEBUG_MSG _DEBUG_MSG
#else
#define DEBUG_MSG(format, ...)
#endif

#define _DEBUG_MSG(format, ...)                                       \
	logger_log(LOGGER_LEVEL_DEBUG, __FILE__ ":%d: %s(): " format, \
		   __LINE__, __FUNCTION__, ##__VA_ARGS__)

#endif
