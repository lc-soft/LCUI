/*
 * lib/router/include/router/common.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "version.h"
#ifdef __cplusplus
#define LIBROUTER_BEGIN_DECLS extern "C" {
#define LIBROUTER_END_DECLS }
#else
#define LIBROUTER_BEGIN_DECLS
#define LIBROUTER_END_DECLS
#endif

#ifndef LIBROUTER_PUBLIC
#if defined(_MSC_VER) && !defined(LIBROUTER_STATIC_BUILD)
#ifdef LIBROUTER_DLL_EXPORT
#define LIBROUTER_PUBLIC __declspec(dllexport)
#else
#define LIBROUTER_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBROUTER_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBROUTER_PUBLIC extern
#endif
#endif
