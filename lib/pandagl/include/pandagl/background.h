/*
 * lib/pandagl/include/pandagl/background.h: -- Background image draw support.
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_BACKGROUND_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_BACKGROUND_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

/**
 * 绘制背景
 * @param ctx 绘制器的上下文句柄
 * @param[in] box 背景区域
 * @param[in] bg 背景样式参数
 */
PD_PUBLIC void pd_paint_background(pd_context_t *ctx, const pd_background_t *bg,
				  const pd_rect_t *box);

PD_END_DECLS

#endif
