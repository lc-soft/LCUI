/*
 * lib/pandagl/include/pandagl/boxshadow.h: -- Box shadow drawing
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_BOXSHADOW_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_BOXSHADOW_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_PUBLIC void pd_get_boxshadow_canvas_rect(const pd_boxshadow_t *shadow,
					   const pd_rect_t *box_rect,
					   pd_rect_t *canvas_rect);

PD_PUBLIC int pd_paint_boxshadow(pd_context_t *ctx, const pd_boxshadow_t *shadow,
				const pd_rect_t *box, int content_width,
				int content_height);

PD_END_DECLS

#endif
