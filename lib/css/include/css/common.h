/*
 * lib/css/include/css/common.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "config.h"
#ifdef __cplusplus
#define LIBCSS_BEGIN_DECLS extern "C" {
#define LIBCSS_END_DECLS }
#else
#define LIBCSS_BEGIN_DECLS
#define LIBCSS_END_DECLS
#endif

#ifndef LIBCSS_PUBLIC
#if defined(_MSC_VER) && !defined(LIBCSS_STATIC_BUILD)
#ifdef LIBCSS_DLL_EXPORT
#define LIBCSS_PUBLIC __declspec(dllexport)
#else
#define LIBCSS_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBCSS_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBCSS_PUBLIC extern
#endif
#endif

#if defined(_WIN32) && !defined(__cplusplus)
#define LIBCSS_INLINE __inline
#else
#define LIBCSS_INLINE static inline
#endif
