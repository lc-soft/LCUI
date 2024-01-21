/*
 * lib/pandagl/src/background.c: -- canvas background image draw support.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <yutil.h>
#include <pandagl.h>

void pd_paint_background(pd_context_t *ctx, const pd_background_t *bg,
                         const pd_rect_t *box)
{
        double scale;
        pd_canvas_t canvas, buffer;
        pd_rect_t rect, read_rect;
        int x, y, width, height;

        /* 获取当前绘制区域与背景内容框的重叠区域 */
        if (!pd_rect_overlap(box, &ctx->rect, &rect)) {
                return;
        }
        rect.x -= ctx->rect.x;
        rect.y -= ctx->rect.y;
        pd_canvas_init(&buffer);
        pd_canvas_quote(&canvas, &ctx->canvas, &rect);
        pd_canvas_fill(&canvas, bg->color);
        if (!bg->image) {
                return;
        }
        /* 将坐标转换为相对于背景内容框 */
        rect.x += ctx->rect.x - box->x;
        rect.y += ctx->rect.y - box->y;
        /* 保存背景图像区域 */
        read_rect.x = x = bg->x;
        read_rect.y = y = bg->y;
        read_rect.width = width = bg->width;
        read_rect.height = height = bg->height;
        /* 获取当前绘制区域与背景图像的重叠区域 */
        if (!pd_rect_overlap(&read_rect, &rect, &read_rect)) {
                return;
        }
        /* 转换成相对于图像的坐标 */
        read_rect.x -= bg->x;
        read_rect.y -= bg->y;
        /* 如果尺寸没有变化则直接引用 */
        if (bg->width == bg->image->width && bg->height == bg->image->height) {
                pd_canvas_quote(&canvas, bg->image, &read_rect);
        } else {
                rect = read_rect;
                /* 根据宽高的缩放比例，计算实际需要引用的区域 */
                if (width != bg->image->width) {
                        scale = 1.0 * bg->image->width / width;
                        rect.x = y_iround(rect.x * scale);
                        rect.width = y_iround(rect.width * scale);
                }
                if (height != bg->image->height) {
                        scale = 1.0 * bg->image->height / height;
                        rect.y = y_iround(rect.y * scale);
                        rect.height = y_iround(rect.height * scale);
                }
                /* 引用源背景图像的一块区域 */
                pd_canvas_quote(&canvas, bg->image, &rect);
                width = read_rect.width;
                height = read_rect.height;
                /* 按比例进行缩放 */
                pd_canvas_zoom(&canvas, &buffer, PD_FALSE, width, height);
                pd_canvas_quote(&canvas, &buffer, NULL);
        }
        /* 计算相对于绘制区域的坐标 */
        x += read_rect.x + box->x - ctx->rect.x;
        y += read_rect.y + box->y - ctx->rect.y;
        pd_canvas_mix(&ctx->canvas, &canvas, x, y, bg->color.a < 255);
        pd_canvas_destroy(&buffer);
}
