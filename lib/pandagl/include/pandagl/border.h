/*
 * lib/pandagl/include/pandagl/border.h: -- Border drawing
 *
 * Copyright (c) 2018-2019-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_BORDER_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_BORDER_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_PUBLIC int pd_crop_border_content(pd_context_t *ctx,
				    const pd_border_t *border,
				    const pd_rect_t *box);

PD_PUBLIC int pd_paint_border(pd_context_t *ctx, const pd_border_t *border,
			     const pd_rect_t *box);

PD_END_DECLS

#endif
