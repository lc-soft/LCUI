
#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_RECT_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_RECT_H

#include <LCUI/def.h>
#include <yutil.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API pd_rect_t pd_rect(int x, int y, int width, int height);

INLINE pd_bool pd_rect_is_include(pd_rect_t *a, pd_rect_t *b)
{
	return (b->x >= a->x && b->x + b->width <= a->x + a->width &&
		b->y >= a->y && b->y + b->height <= a->y + a->height);
}

/** 根据容器尺寸，获取指定区域中需要裁剪的区域 */
LCUI_API pd_rect_t pd_rect_crop(const pd_rect_t *rect, int container_width,
				int container_height);

/** 将矩形区域纠正在容器有效区域内 */
LCUI_API pd_bool pd_rect_correct(pd_rect_t *rect, int container_width,
				 int container_height);

/** 检测矩形是否遮盖另一个矩形 */
LCUI_API pd_bool pd_rect_is_cover(const pd_rect_t *a, const pd_rect_t *b);

/**
 * 获取两个矩形中的重叠矩形
 * @param[in] a		矩形A
 * @param[in] b		矩形B
 * @param[out] out	矩形A和B重叠处的矩形
 * @returns 如果两个矩形重叠，则返回TRUE，否则返回FALSE
 */
LCUI_API pd_bool pd_rect_overlap(const pd_rect_t *a, const pd_rect_t *b,
				 pd_rect_t *overlapping_rect);

LCUI_API void pd_rect_split(pd_rect_t *base, pd_rect_t *target,
			    pd_rect_t rects[4]);

INLINE pd_bool pd_rect_is_equal(const pd_rect_t *a, const pd_rect_t *b)
{
	return a->x == b->x && a->y == b->y && a->width == b->width &&
	       a->height == b->height;
}

/** 添加一个脏矩形记录 */
LCUI_API int pd_rects_add(list_t *list, pd_rect_t *rect);

/** 删除脏矩形 */
LCUI_API int pd_rects_remove(list_t *list, pd_rect_t *rect);

LCUI_API void pd_rects_clear(list_t *list);

LCUI_END_HEADER

#endif
