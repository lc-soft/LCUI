/* rect.c -- Rectangle area handling
 *
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * withoverlapping_rect modification, are permitted provided that the following
 * conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software
 * withoverlapping_rect specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pandagl/rect.h>

pd_rect_t pd_rect(int x, int y, int width, int height)
{
        pd_rect_t rect = { x, y, width, height };
        return rect;
}

pd_rect_t pd_rect_crop(const pd_rect_t *rect, int container_width,
                       int container_height)
{
        pd_rect_t cropped_rect = { 0, 0, rect->width, rect->height };
        if (rect->x < 0) {
                cropped_rect.width += rect->x;
                cropped_rect.x = 0 - rect->x;
        }
        if (rect->x + rect->width > container_width) {
                cropped_rect.width -= (rect->x + rect->width - container_width);
        }

        if (rect->y < 0) {
                cropped_rect.height += rect->y;
                cropped_rect.y = 0 - rect->y;
        }
        if (rect->y + rect->height > container_height) {
                cropped_rect.height -=
                    (rect->y + rect->height - container_height);
        }
        return cropped_rect;
}

pd_bool_t pd_rect_correct(pd_rect_t *rect, int container_width,
                          int container_height)
{
        pd_bool_t overflow = PD_FALSE;

        if (rect->x < 0) {
                overflow = PD_TRUE;
                rect->width += rect->x;
                rect->x = 0;
        }
        if (rect->y < 0) {
                overflow = PD_TRUE;
                rect->height += rect->y;
                rect->y = 0;
        }

        if (rect->x + rect->width > container_width) {
                overflow = PD_TRUE;
                if (rect->x < container_width) {
                        rect->width = container_width - rect->x;
                } else {
                        rect->width = 0;
                }
        }
        if (rect->y + rect->height > container_height) {
                overflow = PD_TRUE;
                if (rect->y < container_height) {
                        rect->height = container_height - rect->y;
                } else {
                        rect->height = 0;
                }
        }
        return overflow;
}

pd_bool_t pd_rect_is_cover(const pd_rect_t *a, const pd_rect_t *b)
{
        if (a->x > b->x) {
                if (b->x + b->width <= a->x) {
                        return PD_FALSE;
                }
        } else {
                if (a->x + a->width <= b->x) {
                        return PD_FALSE;
                }
        }
        if (a->y > b->y) {
                if (b->y + b->height <= a->y) {
                        return PD_FALSE;
                }
        } else {
                if (a->y + a->height <= b->y) {
                        return PD_FALSE;
                }
        }
        return PD_TRUE;
}

pd_bool_t pd_rect_overlap(const pd_rect_t *a, const pd_rect_t *b,
                          pd_rect_t *overlapping_rect)
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
                return PD_FALSE;
        }
        return PD_TRUE;
}

void pd_rect_merge(pd_rect_t *merged_rect, const pd_rect_t *a,
                   const pd_rect_t *b)
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

void pd_rect_split(pd_rect_t *base, pd_rect_t *target, pd_rect_t rects[4])
{
        if (pd_rect_is_include(target, base)) {
                rects[0].x = target->x;
                rects[0].y = target->y;
                rects[0].width = base->x - target->x;
                rects[0].height = base->y + base->height - target->y;

                rects[1].x = target->x;
                rects[1].y = base->y + base->height;
                rects[1].width = base->x + base->width - target->x;
                rects[1].height = target->y + target->height - rects[1].y;

                rects[2].x = base->x + base->width;
                rects[2].y = base->y;
                rects[2].width = target->width - rects[1].width;
                rects[2].height = target->y + target->height - rects[2].y;

                rects[3].x = base->x;
                rects[3].y = target->y;
                rects[3].width = target->x + target->width - rects[3].x;
                rects[3].height = base->y - target->y;
                return;
        }
        /* Right */
        if (target->x >= base->x) {
                /* Top */
                if (target->y < base->y) {
                        rects[0].x = target->x;
                        rects[0].y = target->y;
                        rects[0].width = target->width;
                        rects[0].height = base->y - target->y;
                        rects[1].x = base->x + base->width;
                        rects[1].y = base->y;
                        rects[1].width =
                            target->x + target->width - base->x - base->width;
                        rects[1].height = target->y + target->height - base->y;
                } else {
                        /* Bottom */
                        rects[0].x = base->x + base->width;
                        rects[0].y = target->y;
                        rects[0].width =
                            target->x + target->width - base->x - base->width;
                        rects[0].height = target->height;
                        rects[1].x = target->x;
                        rects[1].y = base->y + base->height;
                        rects[1].width = base->x + base->width - target->x;
                        rects[1].height =
                            target->y + target->height - base->y - base->height;
                }
        } else {
                /* Top */
                if (target->y < base->y) {
                        rects[0].x = target->x;
                        rects[0].y = target->y;
                        rects[0].width = target->width;
                        rects[0].height = base->y - target->y;
                        rects[1].x = target->x;
                        rects[1].y = base->y;
                        rects[1].width = base->x - target->x;
                        rects[1].height = target->y + target->height - base->y;
                } else {
                        /* Bottom */
                        rects[0].x = target->x;
                        rects[0].y = target->y;
                        rects[0].width = base->x - target->x;
                        rects[0].height = target->height;
                        rects[1].x = base->x;
                        rects[1].y = base->y + base->height;
                        rects[1].width = target->x + target->width - base->x;
                        rects[1].height =
                            target->y + target->height - base->y - base->height;
                }
        }
        rects[2].x = 0;
        rects[2].y = 0;
        rects[2].width = 0;
        rects[2].height = 0;
        rects[3].x = 0;
        rects[3].y = 0;
        rects[3].width = 0;
        rects[3].height = 0;
}

int pd_rects_add(list_t *list, pd_rect_t *rect)
{
        int x_distance, y_distance;

        pd_rect_t *p, merged_rect;
        list_node_t *node, *prev;

        if (rect->width <= 0 || rect->height <= 0) {
                return -1;
        }
        for (list_each(node, list)) {
                p = node->data;
                /* 如果被现有的矩形包含 */
                if (pd_rect_is_include(p, rect)) {
                        return -2;
                }
                /* 如果包含现有的矩形 */
                if (pd_rect_is_include(rect, p)) {
                        prev = node->prev;
                        free(node->data);
                        list_delete_node(list, node);
                        node = prev;
                        continue;
                }
                x_distance = p->x + p->width - rect->x - rect->width;
                y_distance = p->y + p->height - rect->y - rect->height;
                if ((x_distance <= 10 && x_distance >= -10) &&
                    (y_distance <= 10 && y_distance >= -10)) {
                        pd_rect_merge(&merged_rect, p, rect);
                        free(node->data);
                        list_delete_node(list, node);
                        return pd_rects_add(list, &merged_rect);
                }
        }
        p = malloc(sizeof(pd_rect_t));
        *p = *rect;
        list_append(list, p);
        return 0;
}

int pd_rects_remove(list_t *list, pd_rect_t *rect)
{
        int i;

        pd_bool_t deletable;
        pd_rect_t *p, child_rects[4];

        list_t extra_list;
        list_node_t *prev, *node;

        if (rect->width <= 0 || rect->height <= 0) {
                return -1;
        }
        list_create(&extra_list);
        for (list_each(node, list)) {
                p = node->data;
                /* 如果包含现有的矩形 */
                if (pd_rect_is_include(rect, p)) {
                        prev = node->prev;
                        free(node->data);
                        list_delete_node(list, node);
                        node = prev;
                        continue;
                }
                if (!pd_rect_is_cover(p, rect)) {
                        continue;
                }
                deletable = PD_TRUE;
                pd_rect_split(rect, p, child_rects);
                for (i = 0; i < 4; ++i) {
                        if (child_rects[i].width <= 0 ||
                            child_rects[i].height <= 0) {
                                continue;
                        }
                        if (deletable) {
                                prev = node->prev;
                                free(node->data);
                                list_delete_node(list, node);
                                node = prev;
                                deletable = PD_FALSE;
                        }
                        p = malloc(sizeof(pd_rect_t));
                        *p = child_rects[i];
                        list_append(&extra_list, p);
                }
        }
        list_concat(list, &extra_list);
        return 0;
}

void pd_rects_clear(list_t *rects)
{
        list_destroy(rects, free);
}

int pd_rect_to_str(pd_rect_t *rect, char *str, unsigned max_len)
{
        return snprintf(str, max_len, "(%d, %d, %d, %d)", rect->x, rect->y,
                        rect->width, rect->height);
}
