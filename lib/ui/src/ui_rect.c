/*
 * lib/ui/src/ui_rect.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <ui/rect.h>
#include <pandagl/rect.h>

void ui_rect_from_pd_rect(ui_rect_t *ui_rect, const pd_rect_t *rect, float scale)
{
        ui_rect->x = rect->x / scale;
        ui_rect->y = rect->y / scale;
        ui_rect->width = rect->width / scale;
        ui_rect->height = rect->height / scale;
}

bool ui_rect_correct(ui_rect_t *rect, float container_width,
                     float container_height)
{
        bool overflow = false;

        if (rect->x < 0) {
                overflow = true;
                rect->width += rect->x;
                rect->x = 0;
        }
        if (rect->y < 0) {
                overflow = true;
                rect->height += rect->y;
                rect->y = 0;
        }

        if (rect->x + rect->width - container_width > 0) {
                overflow = true;
                if (rect->x - container_width < 0) {
                        rect->width = container_width - rect->x;
                } else {
                        rect->width = 0;
                }
        }
        if (rect->y + rect->height - container_height > 0) {
                overflow = true;
                if (rect->y - container_height < 0) {
                        rect->height = container_height - rect->y;
                } else {
                        rect->height = 0;
                }
        }
        return overflow;
}

bool ui_rect_is_cover(const ui_rect_t *a, const ui_rect_t *b)
{
        if (a->x > b->x) {
                if (b->x + b->width <= a->x) {
                        return false;
                }
        } else {
                if (a->x + a->width <= b->x) {
                        return false;
                }
        }
        if (a->y > b->y) {
                if (b->y + b->height <= a->y) {
                        return false;
                }
        } else {
                if (a->y + a->height <= b->y) {
                        return false;
                }
        }
        return true;
}

bool ui_rect_overlap(const ui_rect_t *a, const ui_rect_t *b,
                     ui_rect_t *overlapping_rect)
{
        if (a->x > b->x) {
                if (b->x + b->width > a->x + a->width) {
                        overlapping_rect->width = a->width;
                } else {
                        overlapping_rect->width = b->x + b->width - a->x;
                }
                overlapping_rect->x = a->x;
        } else {
                if (a->x + a->width > b->x + b->width) {
                        overlapping_rect->width = b->width;
                } else {
                        overlapping_rect->width = a->x + a->width - b->x;
                }
                overlapping_rect->x = b->x;
        }
        if (a->y > b->y) {
                if (b->y + b->height > a->y + a->height) {
                        overlapping_rect->height = a->height;
                } else {
                        overlapping_rect->height = b->y + b->height - a->y;
                }
                overlapping_rect->y = a->y;
        } else {
                if (a->y + a->height > b->y + b->height) {
                        overlapping_rect->height = b->height;
                } else {
                        overlapping_rect->height = a->y + a->height - b->y;
                }
                overlapping_rect->y = b->y;
        }
        if (overlapping_rect->width <= 0 || overlapping_rect->height <= 0) {
                return false;
        }
        return true;
}

void ui_rect_merge(ui_rect_t *merged_rect, const ui_rect_t *a,
                   const ui_rect_t *b)
{
        if (a->x + a->width < b->x + b->width) {
                merged_rect->width = b->x + b->width;
        } else {
                merged_rect->width = a->x + a->width;
        }
        if (a->y + a->height < b->y + b->height) {
                merged_rect->height = b->y + b->height;
        } else {
                merged_rect->height = a->y + a->height;
        }
        merged_rect->x = y_min(a->x, b->x);
        merged_rect->y = y_min(a->y, b->y);
        merged_rect->width -= merged_rect->x;
        merged_rect->height -= merged_rect->y;
}

int ui_rect_to_str(ui_rect_t *rect, char *str, unsigned max_len)
{
        return snprintf(str, max_len, "(%g, %g, %g, %g)", rect->x, rect->y,
                        rect->width, rect->height);
}
