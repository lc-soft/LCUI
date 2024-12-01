/*
 * lib/ui/include/ui/rect.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_RECT_H
#define LIB_UI_INCLUDE_UI_RECT_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC void ui_rect_from_pd_rect(ui_rect_t *ui_rect,
                                       const pd_rect_t *rect, float scale);

LIBUI_PUBLIC bool ui_rect_correct(ui_rect_t *rect, float container_width,
                                  float container_height);

LIBUI_INLINE bool ui_rect_has_point(ui_rect_t *rect, float x, float y)
{
        return x >= rect->x && y >= rect->y && x < rect->x + rect->width &&
               y < rect->y + rect->height;
}

LIBUI_INLINE bool ui_rect_is_equal(const ui_rect_t *a, const ui_rect_t *b)
{
        return (int)(100 * (a->x - b->x)) == 0 &&
               (int)(100 * (a->y - b->y)) == 0 &&
               (int)(100 * (a->width - b->width)) == 0 &&
               (int)(100 * (a->height - b->height)) == 0;
}

LIBUI_PUBLIC bool ui_rect_is_cover(const ui_rect_t *a, const ui_rect_t *b);

LIBUI_INLINE bool ui_rect_is_include(ui_rect_t *a, ui_rect_t *b)
{
        return (b->x >= a->x && b->x + b->width <= a->x + a->width &&
                b->y >= a->y && b->y + b->height <= a->y + a->height);
}

LIBUI_PUBLIC bool ui_rect_overlap(const ui_rect_t *a, const ui_rect_t *b,
                                  ui_rect_t *overlapping_rect);

LIBUI_PUBLIC void ui_rect_merge(ui_rect_t *merged_rect, const ui_rect_t *a,
                                const ui_rect_t *b);

LIBUI_PUBLIC int ui_rect_to_str(ui_rect_t *rect, char *str, unsigned max_len);

LIBUI_END_DECLS

#endif
