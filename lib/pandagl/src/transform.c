/*
 * lib/pandagl/src/transform.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <pandagl.h>

static uint8_t canvas_bilinear_resampling_core(uint8_t a, uint8_t b, uint8_t c,
                                               uint8_t d, float dx, float dy)
{
        return (uint8_t)(a * (1 - dx) * (1 - dy) + b * (dx) * (1 - dy) +
                         c * (dy) * (1 - dx) + d * (dx * dy));
}

int pd_canvas_rotate(const pd_canvas_t *src, pd_canvas_t *dst, double radians)
{
        pd_rect_t rect;
        int x, y, ix, iy, ix1, iy1;
        float cos_r, sin_r;
        float cx, cy, sx, sy, fx, fy;
        pd_color_t a, b, c, d, t_color;
        unsigned w, h;

        if (!pd_canvas_is_valid(src)) {
                return -1;
        }

        if (src->color_type != PD_COLOR_TYPE_RGB &&
            src->color_type != PD_COLOR_TYPE_ARGB) {
                return -1;
        }

        /* Get quote rect and source pointer */
        pd_canvas_get_quote_rect(src, &rect);
        src = pd_canvas_get_quote_source_readonly(src);
        w = (unsigned)rect.width;
        h = (unsigned)rect.height;
        cx = w / 2.0f;
        cy = h / 2.0f;

        cos_r = (float)cos(radians);
        sin_r = (float)sin(radians);

        /* Create dst ARGB canvas with same dimensions as src quote rect */
        pd_canvas_init(dst);
        dst->color_type = PD_COLOR_TYPE_ARGB;
        if (pd_canvas_create(dst, w, h) < 0) {
                return -1;
        }

        for (y = 0; y < (int)h; y++) {
                for (x = 0; x < (int)w; x++) {
                        /* Inverse rotation: dst pixel -> src sample coords */
                        float dx = x - cx;
                        float dy = y - cy;

                        sx = cos_r * dx + sin_r * dy + cx;
                        sy = -sin_r * dx + cos_r * dy + cy;

                        /* Out of bounds: write transparent */
                        if (sx < 0 || sy < 0 || sx > (float)(w - 1) ||
                            sy > (float)(h - 1)) {
                                pd_canvas_set_pixel(dst, x, y,
                                                    pd_argb(0, 0, 0, 0));
                                continue;
                        }

                        /* Bilinear interpolation: sample 2x2 neighborhood */
                        ix = (int)sx;
                        iy = (int)sy;
                        fx = sx - ix;
                        fy = sy - iy;

                        /* Clamp neighbor indices to avoid OOB on
                         * bottom/right edge (pd_canvas_pixel_at only
                         * wraps x, not y) */
                        ix1 = ix + 1 < (int)w ? ix + 1 : ix;
                        iy1 = iy + 1 < (int)h ? iy + 1 : iy;

                        a = pd_canvas_get_pixel(src, ix + rect.x, iy + rect.y);
                        b = pd_canvas_get_pixel(src, ix1 + rect.x, iy + rect.y);
                        c = pd_canvas_get_pixel(src, ix + rect.x, iy1 + rect.y);
                        d = pd_canvas_get_pixel(src, ix1 + rect.x,
                                                iy1 + rect.y);
                        t_color.b = canvas_bilinear_resampling_core(
                            a.b, b.b, c.b, d.b, fx, fy);
                        t_color.g = canvas_bilinear_resampling_core(
                            a.g, b.g, c.g, d.g, fx, fy);
                        t_color.r = canvas_bilinear_resampling_core(
                            a.r, b.r, c.r, d.r, fx, fy);
                        t_color.a = canvas_bilinear_resampling_core(
                            a.a, b.a, c.a, d.a, fx, fy);
                        pd_canvas_set_pixel(dst, x, y, t_color);
                }
        }

        return 0;
}
