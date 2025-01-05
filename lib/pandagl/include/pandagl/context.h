/*
 * lib/pandagl/include/pandagl/context.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_CONTEXT_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_CONTEXT_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_PUBLIC pd_context_t* pd_context_create(pd_canvas_t *canvas, pd_rect_t *rect);

PD_PUBLIC void pd_context_destroy(pd_context_t* paint);

PD_END_DECLS

#endif
