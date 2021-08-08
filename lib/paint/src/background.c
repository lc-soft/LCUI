﻿/*
 * background.c -- canvas background image draw support.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
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

#include <LCUI/header.h>
#include <LCUI/types.h>
#include <LCUI/graph.h>
#include <LCUI/util.h>

void pd_background_paint(const pd_background_t *bg, const pd_rect_t *box,
			 pd_paint_context_t *paint)
{
	double scale;
	pd_canvas_t canvas, buffer;
	pd_rect_t rect, read_rect;
	int x, y, width, height;
	/* 获取当前绘制区域与背景内容框的重叠区域 */
	if (!pd_rect_get_overlay_rect(box, &paint->rect, &rect)) {
		return;
	}
	rect.x -= paint->rect.x;
	rect.y -= paint->rect.y;
	pd_canvas_init(&buffer);
	pd_canvas_quote(&canvas, &paint->canvas, &rect);
	pd_canvas_fill_rect(&canvas, bg->color, NULL, TRUE);
	if (!bg->image) {
		return;
	}
	/* 将坐标转换为相对于背景内容框 */
	rect.x += paint->rect.x - box->x;
	rect.y += paint->rect.y - box->y;
	/* 保存背景图像区域 */
	read_rect.x = x = bg->position.x;
	read_rect.y = y = bg->position.y;
	read_rect.width = width = bg->size.width;
	read_rect.height = height = bg->size.height;
	/* 获取当前绘制区域与背景图像的重叠区域 */
	if (!pd_rect_get_overlay_rect(&read_rect, &rect, &read_rect)) {
		return;
	}
	/* 转换成相对于图像的坐标 */
	read_rect.x -= bg->position.x;
	read_rect.y -= bg->position.y;
	/* 如果尺寸没有变化则直接引用 */
	if (bg->size.width == bg->image->width &&
	    bg->size.height == bg->image->height) {
		pd_canvas_quote_read_only(&canvas, bg->image, &read_rect);
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
		pd_canvas_quote_read_only(&canvas, bg->image, &rect);
		width = read_rect.width;
		height = read_rect.height;
		/* 按比例进行缩放 */
		pd_canvas_zoom(&canvas, &buffer, FALSE, width, height);
		pd_canvas_quote_read_only(&canvas, &buffer, NULL);
	}
	/* 计算相对于绘制区域的坐标 */
	x += read_rect.x + box->x - paint->rect.x;
	y += read_rect.y + box->y - paint->rect.y;
	pd_canvas_mix(&paint->canvas, &canvas, x, y, bg->color.alpha < 255);
	pd_canvas_free(&buffer);
}
