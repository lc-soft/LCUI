// Copyright to be defined

#ifndef PANDAGL_INCLUDE_PANDAGL_UTIL_RECT_H_
#define PANDAGL_INCLUDE_PANDAGL_UTIL_RECT_H_

#include <PandaGL/build.h>
#include <PandaGL/util/linkedlist.h>
#include <PandaGL/types.h>

INLINE pd_bool_t pd_rect_is_equals(const pd_rect_t *a, const pd_rect_t *b)
{
    return a->x == b->x && a->y == b->y && a->width == b->width &&
           a->height == b->height;
}

PD_API pd_bool_t pd_rect_validate_area(pd_rect_t *rect, int box_w, int box_h);


INLINE pd_bool_t pd_rect_is_include_rect(pd_rect_t* a, pd_rect_t* b) {
    return (b->x >= a->x && b->x + b->width <= a->x + a->width &&
        b->y >= a->y && b->y + b->height <= a->y + a->height);
}

PD_API pd_bool_t pd_rect_validate_area(pd_rect_t *rect, int box_w, int box_h);

/* 将数值转换成pd_rect型结构体 */
PD_API pd_rect_t pd_rect(int x, int y, int w, int h);

PD_API pd_bool_t pd_rect_is_cover_rect(const pd_rect_t *a, const pd_rect_t *b);

/**
 * 获取两个矩形中的重叠矩形
 * @param[in] a		矩形A
 * @param[in] b		矩形B
 * @param[out] out	矩形A和B重叠处的矩形
 * @returns 如果两个矩形重叠，则返回TRUE，否则返回FALSE
 */
PD_API pd_bool_t pd_rect_get_overlay_rect(const pd_rect_t *a,
					   const pd_rect_t *b, pd_rect_t *out);

/**
 * 根据重叠矩形 rect1，将矩形 rect2 分割成四个矩形
 * 分割方法如下：
 * ┏━━┳━━━━━━┓
 * ┃    ┃     3      ┃
 * ┃ 0  ┣━━━┳━━┃
 * ┃    ┃rect1 ┃    ┃
 * ┃    ┃      ┃ 2  ┃
 * ┣━━┻━━━┫    ┃
 * ┃     1      ┃    ┃
 * ┗━━━━━━┻━━┛
 *
 * rect2 必须被 rect1 完全包含
 */
PD_API void pd_rect_cut_four_rect(pd_rect_t *rect1, pd_rect_t *rect2,
                   pd_rect_t rects[4]);

PD_API void pd_rect_split(pd_rect_t *base, pd_rect_t *target,
                 pd_rect_t rects[4]);

PD_API void pd_rect_get_cut_area(int box_w, int box_h, pd_rect_t rect, pd_rect_t *cut);

#define pd_rect_list_clear(LIST) pd_linked_list_clear(LIST, free)

#endif  // !PANDAGL_INCLUDE_PANDAGL_UTIL_RECT_H_
