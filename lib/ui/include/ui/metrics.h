/*
 * lib/ui/include/ui/metrics.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_METRICS_H
#define LIB_UI_INCLUDE_UI_METRICS_H

#include "common.h"
#include "types.h"
#include <css/utils.h>

LIBUI_PUBLIC ui_metrics_t ui_metrics;

LIBUI_BEGIN_DECLS

LIBUI_INLINE float ui_get_actual_scale(void)
{
        return css_metrics_actual_scale(&ui_metrics);
}

LIBUI_INLINE int ui_compute(float value)
{
        return (int)(ui_get_actual_scale() * value);
}

LIBUI_INLINE void ui_compute_rect(pd_rect_t *actual_rect, const ui_rect_t *rect)
{
        actual_rect->x = ui_compute(rect->x);
        actual_rect->y = ui_compute(rect->y);
        actual_rect->width = ui_compute(rect->width);
        actual_rect->height = ui_compute(rect->height);
}

/** 设置密度 */
LIBUI_PUBLIC void ui_set_density(float density);

/** 设置缩放密度 */
LIBUI_PUBLIC void ui_set_scaled_density(float density);

/** 设置密度等级 */
LIBUI_PUBLIC void ui_set_density_level(ui_density_Level_t level);

/** 设置缩放密度等级 */
LIBUI_PUBLIC void ui_set_scaled_density_level(ui_density_Level_t level);

/** 设置全局缩放比例 */
LIBUI_PUBLIC void ui_set_scale(float scale);

LIBUI_END_DECLS

#endif
