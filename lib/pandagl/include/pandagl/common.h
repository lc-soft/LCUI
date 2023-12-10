/*
 * lib/pandagl/include/pandagl/common.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "config.h"
#ifdef __cplusplus
#define PD_BEGIN_DECLS extern "C" {
#define PD_END_DECLS }
#else
#define PD_BEGIN_DECLS
#define PD_END_DECLS
#endif

#ifndef PD_PUBLIC
#if defined(_MSC_VER) && !defined(PANDAGL_STATIC_BUILD)
#ifdef PANDAGL_DLL_EXPORT
#define PD_PUBLIC __declspec(dllexport)
#else
#define PD_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define PD_PUBLIC extern __attribute__((visibility("default")))
#else
#define PD_PUBLIC extern
#endif
#endif

#if defined(_WIN32) && !defined(__cplusplus)
#define PD_INLINE __inline
#else
#define PD_INLINE static inline
#endif
