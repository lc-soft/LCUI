/*
 * lib/pandagl/include/pandagl/rect.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_RECT_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_RECT_H

#include <yutil.h>
#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_INLINE bool pd_rect_is_include(pd_rect_t *a, pd_rect_t *b)
{
	return (b->x >= a->x && b->x + b->width <= a->x + a->width &&
		b->y >= a->y && b->y + b->height <= a->y + a->height);
}

/** 根据容器尺寸，获取指定区域中需要裁剪的区域 */
PD_PUBLIC pd_rect_t pd_rect_crop(const pd_rect_t *rect, int container_width,
				int container_height);

/** 将矩形区域纠正在容器有效区域内 */
PD_PUBLIC bool pd_rect_correct(pd_rect_t *rect, int container_width,
				 int container_height);

/** 检测矩形是否遮盖另一个矩形 */
PD_PUBLIC bool pd_rect_is_cover(const pd_rect_t *a, const pd_rect_t *b);

/**
 * 获取两个矩形中的重叠矩形
 * @param[in] a		矩形A
 * @param[in] b		矩形B
 * @param[out] out	矩形A和B重叠处的矩形
 * @returns 如果两个矩形重叠，则返回true，否则返回false
 */
PD_PUBLIC bool pd_rect_overlap(const pd_rect_t *a, const pd_rect_t *b,
				 pd_rect_t *overlapping_rect);

PD_PUBLIC void pd_rect_split(pd_rect_t *base, pd_rect_t *target,
			    pd_rect_t rects[4]);

PD_INLINE bool pd_rect_is_equal(const pd_rect_t *a, const pd_rect_t *b)
{
	return a->x == b->x && a->y == b->y && a->width == b->width &&
	       a->height == b->height;
}

/** 添加一个脏矩形记录 */
PD_PUBLIC int pd_rects_add(list_t *list, pd_rect_t *rect);

/** 删除脏矩形 */
PD_PUBLIC int pd_rects_remove(list_t *list, pd_rect_t *rect);

PD_PUBLIC void pd_rects_clear(list_t *list);

PD_PUBLIC int pd_rect_to_str(pd_rect_t *rect, char *str, unsigned max_len);

PD_END_DECLS

#endif
