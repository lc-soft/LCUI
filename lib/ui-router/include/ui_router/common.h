/*
 * lib/ui-router/include/ui_router/common.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "version.h"
#ifdef __cplusplus
#define LIBUI_ROUTER_BEGIN_DECLS extern "C" {
#define LIBUI_ROUTER_END_DECLS }
#else
#define LIBUI_ROUTER_BEGIN_DECLS
#define LIBUI_ROUTER_END_DECLS
#endif

#ifndef LIBUI_ROUTER_PUBLIC
#if defined(_MSC_VER) && !defined(LIBUI_ROUTER_STATIC_BUILD)
#ifdef LIBUI_ROUTER_DLL_EXPORT
#define LIBUI_ROUTER_PUBLIC __declspec(dllexport)
#else
#define LIBUI_ROUTER_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBUI_ROUTER_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBUI_ROUTER_PUBLIC extern
#endif
#endif
