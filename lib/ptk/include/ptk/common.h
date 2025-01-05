/*
 * lib/ptk/include/ptk/common.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "config.h"
#ifdef __cplusplus
#define PTK_BEGIN_DECLS extern "C" {
#define PTK_END_DECLS }
#else
#define PTK_BEGIN_DECLS
#define PTK_END_DECLS
#endif

#ifndef PTK_PUBLIC
#if defined(_MSC_VER) && !defined(PTK_STATIC_BUILD)
#ifdef PTK_DLL_EXPORT
#define PTK_PUBLIC __declspec(dllexport)
#else
#define PTK_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define PTK_PUBLIC extern __attribute__((visibility("default")))
#else
#define PTK_PUBLIC extern
#endif
#endif

#if defined(_WIN32) && !defined(__cplusplus)
#define PTK_INLINE __inline
#else
#define PTK_INLINE static inline
#endif

#ifdef _WIN32
	#define PTK_WIN32
	#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
	#define PTK_UWP
	#else
	#define PTK_WIN_DESKTOP
	#endif
#else
	#define PTK_LINUX
#endif
