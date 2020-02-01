/* graph.c -- The graphics processing module.
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

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/types.h>
#include <LCUI/util.h>
#include <LCUI/graph.h>

void Graph_PrintInfo(LCUI_Graph *graph)
{
	printf("address:%p\n", graph);
	if (!graph) {
		return;
	}

	printf("width:%d, ", graph->width);
	printf("height:%d, ", graph->height);
	printf("opacity:%.2f, ", graph->opacity);
	printf("%s\n",
	       graph->color_type == LCUI_COLOR_TYPE_ARGB ? "RGBA" : "RGB");
	if (graph->quote.is_valid) {
		printf("graph src:");
		Graph_PrintInfo(Graph_GetQuote(graph));
	}
}

void Graph_Init(LCUI_Graph *graph)
{
	graph->quote.is_valid = FALSE;
	graph->quote.source = NULL;
	graph->quote.top = 0;
	graph->quote.left = 0;
	graph->palette = NULL;
	graph->color_type = LCUI_COLOR_TYPE_RGB;
	graph->bytes = NULL;
	graph->opacity = 1.0;
	graph->mem_size = 0;
	graph->width = 0;
	graph->height = 0;
	graph->bytes_per_pixel = 3;
	graph->bytes_per_row = 0;
}

LCUI_Color RGB(uchar_t r, uchar_t g, uchar_t b)
{
	LCUI_Color color;
	color.red = r;
	color.green = g;
	color.blue = b;
	color.alpha = 255;
	return color;
}

LCUI_Color ARGB(uchar_t a, uchar_t r, uchar_t g, uchar_t b)
{
	LCUI_Color color;
	color.alpha = a;
	color.red = r;
	color.green = g;
	color.blue = b;
	return color;
}

static unsigned get_pixel_size(int color_type)
{
	switch (color_type) {
	case LCUI_COLOR_TYPE_INDEX8:
	case LCUI_COLOR_TYPE_GRAY8:
	case LCUI_COLOR_TYPE_RGB323:
	case LCUI_COLOR_TYPE_ARGB2222:
		return 1;
	case LCUI_COLOR_TYPE_RGB555:
	case LCUI_COLOR_TYPE_RGB565:
		return 2;
	case LCUI_COLOR_TYPE_RGB888:
		return 3;
	case LCUI_COLOR_TYPE_ARGB8888:
	default:
		break;
	}
	return 4;
}

/*----------------------------------- RGB ----------------------------------*/

static void PixelsFormatRGB(const uchar_t *in_pixels, uchar_t *out_pixels,
			    size_t pixel_count)
{
	const LCUI_ARGB8888 *p_px, *p_end_px;
	LCUI_ARGB8888 *p_out_px;
	uchar_t *p_out_byte;

	p_out_byte = out_pixels;
	p_out_px = (LCUI_ARGB8888 *)out_pixels;
	p_px = (const LCUI_ARGB8888 *)in_pixels;
	/* 遍历到倒数第二个像素为止 */
	p_end_px = p_px + pixel_count - 1;
	for (; p_px < p_end_px; ++p_px) {
		*p_out_px = *p_px;
		/* 将后3字节的数据当成4字节（LCUI_ARGB8888）的像素点访问 */
		p_out_px = (LCUI_ARGB8888 *)(((uchar_t *)p_out_px) + 3);
	}
	/* 最后一个像素，以逐个字节的形式写数据 */
	p_out_byte = (uchar_t *)(p_out_px + 1);
	*p_out_byte++ = p_px->blue;
	*p_out_byte++ = p_px->green;
	*p_out_byte++ = p_px->red;
}

static void PixelsFormatARGB(const uchar_t *in_pixels, uchar_t *out_pixels,
			     size_t pixel_count)
{
	LCUI_ARGB8888 *p_px, *p_end_px;
	const uchar_t *p_in_byte;

	p_in_byte = in_pixels;
	p_px = (LCUI_ARGB8888 *)out_pixels;
	p_end_px = p_px + pixel_count;
	while (p_px < p_end_px) {
		p_px->blue = *p_in_byte++;
		p_px->green = *p_in_byte++;
		p_px->red = *p_in_byte++;
		p_px->alpha = 255;
		++p_px;
	}
}

void PixelsFormat(const uchar_t *in_pixels, int in_color_type,
		  uchar_t *out_pixels, int out_color_type, size_t pixel_count)
{
	switch (in_color_type) {
	case LCUI_COLOR_TYPE_ARGB8888:
		if (out_color_type == LCUI_COLOR_TYPE_ARGB8888) {
			return;
		}
		PixelsFormatRGB(in_pixels, out_pixels, pixel_count);
		break;
	case LCUI_COLOR_TYPE_RGB888:
		if (out_color_type == LCUI_COLOR_TYPE_RGB888) {
			return;
		}
		PixelsFormatARGB(in_pixels, out_pixels, pixel_count);
		break;
	default:
		break;
	}
}

static int Graph_RGBToARGB(LCUI_Graph *graph)
{
	size_t x, y;
	LCUI_ARGB *px_des, *px_row_des, *buffer;
	uchar_t *byte_row_src, *byte_src;

	graph->mem_size = sizeof(LCUI_ARGB) * graph->width * graph->height;
	buffer = malloc(graph->mem_size);
	if (!buffer) {
		return -ENOMEM;
	}
	px_row_des = buffer;
	byte_row_src = graph->bytes;
	for (y = 0; y < graph->height; ++y) {
		px_des = px_row_des;
		byte_src = byte_row_src;
		for (x = 0; x < graph->width; ++x) {
			px_des->b = *byte_src++;
			px_des->g = *byte_src++;
			px_des->r = *byte_src++;
			px_des->a = 255;
			++px_des;
		}
		byte_row_src += graph->bytes_per_row;
		px_row_des += graph->width;
	}
	free(graph->argb);
	graph->argb = buffer;
	graph->color_type = LCUI_COLOR_TYPE_ARGB8888;
	return 0;
}

static int Graph_CutRGB(const LCUI_Graph *graph, LCUI_Rect rect,
			LCUI_Graph *buff)
{
	int y;
	uchar_t *byte_src_row, *byte_des_row;

	buff->color_type = graph->color_type;
	if (0 != Graph_Create(buff, rect.width, rect.height)) {
		return -1;
	}
	buff->opacity = graph->opacity;
	byte_des_row = buff->bytes;
	byte_src_row = graph->bytes + rect.y * graph->bytes_per_row;
	byte_src_row += rect.x * graph->bytes_per_pixel;
	for (y = 0; y < rect.height; ++y) {
		memcpy(byte_des_row, byte_src_row, buff->bytes_per_row);
		byte_des_row += buff->bytes_per_row;
		byte_src_row += graph->bytes_per_row;
	}
	return 0;
}

static void Graph_ReplaceToARGB(LCUI_Graph *des, LCUI_Rect des_rect,
				const LCUI_Graph *src, int src_x, int src_y)
{
	int y;
	uchar_t *byte_row_des, *byte_row_src;

	byte_row_src = src->bytes + src_y * src->bytes_per_row;
	byte_row_src += src_x * src->bytes_per_pixel;
	byte_row_des = des->bytes + des_rect.y * des->bytes_per_row;
	byte_row_des += des_rect.x * des->bytes_per_pixel;
	for (y = 0; y < des_rect.height; ++y) {
		/* 将前景图当前行像素转换成ARGB格式，并直接覆盖至背景图上 */
		PixelsFormat(byte_row_src, src->color_type, byte_row_des,
			     des->color_type, des_rect.width);
		byte_row_src += src->bytes_per_row;
		byte_row_des += des->bytes_per_row;
	}
}

static void Graph_ReplaceRGB(LCUI_Graph *des, LCUI_Rect des_rect,
			     const LCUI_Graph *src, int src_x, int src_y)
{
	int y;
	uchar_t *byte_row_des, *byte_row_src;
	byte_row_src = src->bytes + src_y * src->bytes_per_row;
	byte_row_des = des->bytes + des_rect.y * des->bytes_per_row;
	byte_row_src += src_x * src->bytes_per_pixel;
	byte_row_des += des_rect.x * src->bytes_per_pixel;
	for (y = 0; y < des_rect.height; ++y) {
		memcpy(byte_row_des, byte_row_src, des->bytes_per_row);
		byte_row_src += src->bytes_per_row;
		byte_row_des += des->bytes_per_row;
	}
}

#define Graph_MixRGB Graph_ReplaceRGB

static int Graph_HorizFlipRGB(const LCUI_Graph *graph, LCUI_Graph *buff)
{
	int x, y;
	size_t n_bytes;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if (!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	buff->color_type = graph->color_type;
	if (0 != Graph_Create(buff, rect.width, rect.height)) {
		return -2;
	}

	for (y = 0; y < rect.height; ++y) {
		byte_des = buff->bytes + y * buff->bytes_per_row;
		n_bytes = (rect.y + y) * graph->bytes_per_row;
		n_bytes += (rect.x + rect.width - 1) * 3;
		byte_src = buff->bytes + n_bytes;
		for (x = 0; x < rect.width; ++x) {
			*byte_des++ = *byte_src--;
			*byte_des++ = *byte_src--;
			*byte_des++ = *byte_src--;
		}
	}
	return 0;
}

static int Graph_VertiFlipRGB(const LCUI_Graph *graph, LCUI_Graph *buff)
{
	int y;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if (!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	buff->opacity = graph->opacity;
	buff->color_type = graph->color_type;
	if (0 != Graph_Create(buff, rect.width, rect.height)) {
		return -2;
	}
	/* 引用最后一行像素 */
	byte_src = graph->bytes;
	byte_src += (rect.y + rect.height - 1) * graph->bytes_per_row;
	byte_src += rect.x * graph->bytes_per_pixel;
	byte_des = buff->bytes;
	/* 交换上下每行像素 */
	for (y = 0; y < rect.height; ++y) {
		memcpy(byte_des, byte_src, buff->bytes_per_row);
		byte_src -= graph->bytes_per_row;
		byte_des += buff->bytes_per_row;
	}
	return 0;
}

static int Graph_FillRectRGB(LCUI_Graph *graph, LCUI_Color color,
			     LCUI_Rect rect)
{
	int x, y;
	LCUI_Graph canvas;
	uchar_t *rowbytep, *bytep;

	if (!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_Quote(&canvas, graph, &rect);
	Graph_GetValidRect(&canvas, &rect);
	graph = Graph_GetQuote(&canvas);
	rowbytep = graph->bytes + rect.y * graph->bytes_per_row;
	rowbytep += rect.x * graph->bytes_per_pixel;
	for (y = 0; y < rect.height; ++y) {
		bytep = rowbytep;
		for (x = 0; x < rect.width; ++x) {
			*bytep++ = color.blue;
			*bytep++ = color.green;
			*bytep++ = color.red;
		}
		rowbytep += graph->bytes_per_row;
	}
	return 0;
}

/*--------------------------------- End RGB --------------------------------*/

/*---------------------------------- ARGB ----------------------------------*/

static int Graph_ARGBToRGB(LCUI_Graph *graph)
{
	size_t x, y;
	LCUI_ARGB *px_src, *px_row_src;
	uchar_t *buffer, *byte_row_des, *byte_des;

	graph->mem_size = sizeof(uchar_t) * graph->width * graph->height * 3;
	buffer = malloc(graph->mem_size);
	if (!buffer) {
		return -1;
	}
	byte_row_des = buffer;
	px_row_src = graph->argb;
	for (y = 0; y < graph->height; ++y) {
		px_src = px_row_src;
		byte_des = byte_row_des;
		for (x = 0; x < graph->width; ++x) {
			px_src->b = *byte_des++;
			px_src->g = *byte_des++;
			px_src->r = *byte_des++;
			px_src->a = 255;
			++px_src;
		}
		byte_row_des += graph->bytes_per_row;
		px_row_src += graph->width;
	}
	free(graph->argb);
	graph->bytes = buffer;
	graph->color_type = LCUI_COLOR_TYPE_RGB888;
	graph->bytes_per_pixel = 3;
	return 0;
}

static int Graph_CutARGB(const LCUI_Graph *graph, LCUI_Rect rect,
			 LCUI_Graph *buff)
{
	int x, y;
	LCUI_ARGB *pixel_src, *pixel_des;

	buff->color_type = graph->color_type;
	if (0 != Graph_Create(buff, rect.width, rect.height)) {
		return -1;
	}

	buff->opacity = graph->opacity;
	for (y = 0; y < rect.height; ++y) {
		pixel_des = buff->argb + y * buff->width;
		pixel_src = graph->argb;
		pixel_src += (rect.y + y) * graph->width + rect.x;
		for (x = 0; x < rect.width; ++x) {
			*pixel_des++ = *pixel_src++;
		}
	}
	return 0;
}

/* FIXME: improve alpha blending method
 * Existing alpha blending methods are inefficient and need to be optimized
 */

static void Graph_MixARGBWithAlpha(LCUI_Graph *dst, LCUI_Rect des_rect,
				   const LCUI_Graph *src, int src_x, int src_y)
{
	int x, y;
	LCUI_ARGB *px_src, *px_dst;
	LCUI_ARGB *px_row_src, *px_row_des;
	double a, out_a, out_r, out_g, out_b, src_a;
	px_row_src = src->argb + src_y * src->width + src_x;
	px_row_des = dst->argb + des_rect.y * dst->width + des_rect.x;
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dst = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			src_a = px_src->a / 255.0;
			a = (1.0 - src_a) * px_dst->a / 255.0;
			out_r = px_dst->r * a + px_src->r * src_a;
			out_g = px_dst->g * a + px_src->g * src_a;
			out_b = px_dst->b * a + px_src->b * src_a;
			out_a = src_a + a;
			if (out_a > 0) {
				out_r /= out_a;
				out_g /= out_a;
				out_b /= out_a;
			}
			px_dst->r = (uchar_t)(out_r + 0.5);
			px_dst->g = (uchar_t)(out_g + 0.5);
			px_dst->b = (uchar_t)(out_b + 0.5);
			px_dst->a = (uchar_t)(255.0 * out_a + 0.5);
			++px_src;
			++px_dst;
		}
		px_row_des += dst->width;
		px_row_src += src->width;
	}
	return;

mix_with_opacity:
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dst = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			src_a = px_src->a / 255.0 * src->opacity;
			a = (1.0 - src_a) * px_dst->a / 255.0;
			out_r = px_dst->r * a + px_src->r * src_a;
			out_g = px_dst->g * a + px_src->g * src_a;
			out_b = px_dst->b * a + px_src->b * src_a;
			out_a = src_a + a;
			if (out_a > 0) {
				out_r /= out_a;
				out_g /= out_a;
				out_b /= out_a;
			}
			px_dst->r = (uchar_t)(out_r + 0.5);
			px_dst->g = (uchar_t)(out_g + 0.5);
			px_dst->b = (uchar_t)(out_b + 0.5);
			px_dst->a = (uchar_t)(255.0 * out_a + 0.5);
			++px_src;
			++px_dst;
		}
		px_row_des += dst->width;
		px_row_src += src->width;
	}
}

static void Graph_MixARGB(LCUI_Graph *dest, LCUI_Rect des_rect,
			  const LCUI_Graph *src, int src_x, int src_y)
{
	int x, y;
	uchar_t a;
	LCUI_ARGB *px_src, *px_dest;
	LCUI_ARGB *px_row_src, *px_row_des;
	px_row_src = src->argb + src_y * src->width + src_x;
	px_row_des = dest->argb + des_rect.y * dest->width + des_rect.x;
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dest = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			PIXEL_BLEND(px_dest, px_src, px_src->a);
			++px_src;
			++px_dest;
		}
		px_row_des += dest->width;
		px_row_src += src->width;
	}
	return;

mix_with_opacity:
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_dest = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			a = (uchar_t)(px_src->a * src->opacity);
			PIXEL_BLEND(px_dest, px_src, a);
			++px_src;
			++px_dest;
		}
		px_row_des += dest->width;
		px_row_src += src->width;
	}
}

static void Graph_MixARGBToRGB(LCUI_Graph *des, LCUI_Rect des_rect,
			       const LCUI_Graph *src, int src_x, int src_y)
{
	int x, y;
	uchar_t a;
	LCUI_ARGB *px, *px_row;
	uchar_t *rowbytep, *bytep;

	/* 计算并保存第一行的首个像素的位置 */
	px_row = src->argb + src_y * src->width + src_x;
	rowbytep = des->bytes + des_rect.y * des->bytes_per_row;
	rowbytep += des_rect.x * des->bytes_per_pixel;
	if (src->opacity < 1.0) {
		goto mix_with_opacity;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px = px_row;
		bytep = rowbytep;
		for (x = 0; x < des_rect.width; ++x, ++px) {
			*bytep = _ALPHA_BLEND(*bytep, px->b, px->a);
			++bytep;
			*bytep = _ALPHA_BLEND(*bytep, px->g, px->a);
			++bytep;
			*bytep = _ALPHA_BLEND(*bytep, px->r, px->a);
			++bytep;
		}
		rowbytep += des->bytes_per_row;
		px_row += src->width;
	}
	return;

mix_with_opacity:
	for (y = 0; y < des_rect.height; ++y) {
		px = px_row;
		bytep = rowbytep;
		for (x = 0; x < des_rect.width; ++x, ++px) {
			a = (uchar_t)(px->a * src->opacity);
			*bytep = _ALPHA_BLEND(*bytep, px->b, a);
			++bytep;
			*bytep = _ALPHA_BLEND(*bytep, px->g, a);
			++bytep;
			*bytep = _ALPHA_BLEND(*bytep, px->r, a);
			++bytep;
		}
		rowbytep += des->bytes_per_row;
		px_row += src->width;
	}
}

static int Graph_ReplaceARGB(LCUI_Graph *des, LCUI_Rect des_rect,
			     const LCUI_Graph *src, int src_x, int src_y)
{
	int x, y, row_size;
	LCUI_ARGB *px_row_src, *px_row_des, *px_src, *px_des;
	px_row_src = src->argb + src_y * src->width + src_x;
	px_row_des = des->argb + des_rect.y * des->width + des_rect.x;
	if (src->opacity - 1.0f < 0.01f) {
		row_size = sizeof(LCUI_ARGB) * des_rect.width;
		for (y = 0; y < des_rect.height; ++y) {
			memcpy(px_row_des, px_row_src, row_size);
			px_row_src += src->width;
			px_row_des += des->width;
		}
		return 0;
	}
	for (y = 0; y < des_rect.height; ++y) {
		px_src = px_row_src;
		px_des = px_row_des;
		for (x = 0; x < des_rect.width; ++x) {
			px_des->b = px_src->b;
			px_des->g = px_src->g;
			px_des->r = px_src->r;
			px_des->a = (uchar_t)(src->opacity * px_src->a);
		}
		px_row_src += src->width;
		px_row_des += des->width;
	}
	return 0;
}

static int Graph_HorizFlipARGB(const LCUI_Graph *graph, LCUI_Graph *buff)
{
	int x, y;
	LCUI_Rect rect;
	LCUI_ARGB *pixel_src, *pixel_des;

	if (!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	buff->opacity = graph->opacity;
	buff->color_type = graph->color_type;
	if (0 != Graph_Create(buff, rect.width, rect.height)) {
		return -2;
	}

	for (y = 0; y < rect.height; ++y) {
		pixel_des = buff->argb + y * buff->width;
		pixel_src = graph->argb + (rect.y + y) * graph->width;
		pixel_src += rect.x + rect.width - 1;
		for (x = 0; x < rect.width; ++x) {
			*pixel_des++ = *pixel_src--;
		}
	}
	return 0;
}

static int Graph_VertiFlipARGB(const LCUI_Graph *graph, LCUI_Graph *buff)
{
	int y;
	LCUI_Rect rect;
	uchar_t *byte_src, *byte_des;

	if (!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	buff->opacity = graph->opacity;
	buff->color_type = graph->color_type;
	if (0 != Graph_Create(buff, rect.width, rect.height)) {
		return -2;
	}
	byte_src = graph->bytes;
	byte_src += (rect.y + rect.height - 1) * graph->bytes_per_row;
	byte_src += rect.x * graph->bytes_per_pixel;
	byte_des = buff->bytes;
	for (y = 0; y < rect.height; ++y) {
		memcpy(byte_des, byte_src, buff->bytes_per_row);
		byte_src -= graph->bytes_per_row;
		byte_des += buff->bytes_per_row;
	}
	return 0;
}

static int Graph_FillRectARGB(LCUI_Graph *graph, LCUI_Color color,
			      LCUI_Rect rect, LCUI_BOOL with_alpha)
{
	int x, y;
	LCUI_Graph canvas;
	LCUI_ARGB *pixel, *pixel_row;

	if (!Graph_IsValid(graph)) {
		return -1;
	}
	Graph_Quote(&canvas, graph, &rect);
	Graph_GetValidRect(&canvas, &rect);
	graph = Graph_GetQuote(&canvas);
	pixel_row = graph->argb + rect.y * graph->width + rect.x;
	if (with_alpha) {
		for (y = 0; y < rect.height; ++y) {
			pixel = pixel_row;
			for (x = 0; x < rect.width; ++x) {
				*pixel++ = color;
			}
			pixel_row += graph->width;
		}
	} else {
		for (y = 0; y < rect.height; ++y) {
			pixel = pixel_row;
			for (x = 0; x < rect.width; ++x) {
				color.alpha = pixel->alpha;
				*pixel++ = color;
			}
			pixel_row += graph->width;
		}
	}
	return 0;
}

static uchar_t Graph_BilinearResamplingCore(uchar_t a, uchar_t b, uchar_t c,
					    uchar_t d, float dx, float dy)
{
	return (uchar_t)(a * (1 - dx) * (1 - dy) + b * (dx) * (1 - dy) +
			 c * (dy) * (1 - dx) + d * (dx * dy));
}

/*-------------------------------- End ARGB --------------------------------*/

int Graph_SetColorType(LCUI_Graph *graph, int color_type)
{
	if (graph->color_type == color_type) {
		return -1;
	}
	switch (graph->color_type) {
	case LCUI_COLOR_TYPE_ARGB8888:
		switch (color_type) {
		case LCUI_COLOR_TYPE_RGB888:
			return Graph_ARGBToRGB(graph);
		default:
			break;
		}
		break;
	case LCUI_COLOR_TYPE_RGB888:
		switch (color_type) {
		case LCUI_COLOR_TYPE_ARGB8888:
			return Graph_RGBToARGB(graph);
		default:
			break;
		}
		break;
	default:
		break;
	}
	return -2;
}

int Graph_Create(LCUI_Graph *graph, unsigned width, unsigned height)
{
	size_t size;
	if (width > 10000 || height > 10000) {
		Logger_Error("graph size is too large!");
		abort();
	}
	if (width < 1 || height < 1) {
		Graph_Free(graph);
		return -1;
	}
	graph->bytes_per_pixel = get_pixel_size(graph->color_type);
	graph->bytes_per_row = graph->bytes_per_pixel * width;
	size = graph->bytes_per_row * height;
	if (Graph_IsValid(graph)) {
		/* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
		if (graph->mem_size >= size) {
			memset(graph->bytes, 0, graph->mem_size);
			graph->width = width;
			graph->height = height;
			return 0;
		}
		Graph_Free(graph);
	}
	graph->mem_size = size;
	graph->bytes = calloc(1, size);
	if (!graph->bytes) {
		graph->width = 0;
		graph->height = 0;
		return -2;
	}
	graph->width = width;
	graph->height = height;
	return 0;
}

LCUI_BOOL Graph_IsValid(const LCUI_Graph *graph)
{
	if (graph->quote.is_valid) {
		return graph->quote.source && graph->quote.source->width > 0 &&
		       graph->quote.source->height > 0;
	}
	return graph->bytes && graph->height > 0 && graph->width > 0;
}

void Graph_Copy(LCUI_Graph *des, const LCUI_Graph *src)
{
	const LCUI_Graph *graph;
	if (!des || !Graph_IsValid(src)) {
		return;
	}
	graph = Graph_GetQuote(src);
	des->color_type = graph->color_type;
	/* 创建合适尺寸的Graph */
	Graph_Create(des, src->width, src->height);
	Graph_Replace(des, src, 0, 0);
	des->opacity = src->opacity;
}

void Graph_Free(LCUI_Graph *graph)
{
	/* 解除引用 */
	if (graph && graph->quote.is_valid) {
		graph->quote.source = NULL;
		graph->quote.is_valid = FALSE;
		return;
	}
	if (graph->bytes) {
		free(graph->bytes);
		graph->bytes = NULL;
	}
	graph->width = 0;
	graph->height = 0;
	graph->mem_size = 0;
}

int Graph_QuoteReadOnly(LCUI_Graph *self, const LCUI_Graph *source,
			const LCUI_Rect *rect)
{
	LCUI_Rect quote_rect;
	if (!rect) {
		quote_rect.x = 0;
		quote_rect.y = 0;
		quote_rect.width = source->width;
		quote_rect.height = source->height;
	} else {
		quote_rect = *rect;
	}
	LCUIRect_ValidateArea(&quote_rect, source->width, source->height);
	/* 如果引用源本身已经引用了另一个源 */
	if (source->quote.is_valid) {
		quote_rect.x += source->quote.left;
		quote_rect.y += source->quote.top;
		source = source->quote.source;
	}
	if (quote_rect.width <= 0 || quote_rect.height <= 0) {
		self->width = 0;
		self->height = 0;
		self->opacity = 1.0;
		self->quote.left = 0;
		self->quote.top = 0;
		self->bytes = NULL;
		self->quote.source = NULL;
		self->quote.is_valid = FALSE;
		return -EINVAL;
	}
	self->opacity = 1.0;
	self->bytes = NULL;
	self->mem_size = 0;
	self->width = quote_rect.width;
	self->height = quote_rect.height;
	self->color_type = source->color_type;
	self->bytes_per_pixel = source->bytes_per_pixel;
	self->bytes_per_row = source->bytes_per_row;
	self->quote.is_valid = TRUE;
	self->quote.source_ro = source;
	self->quote.left = quote_rect.x;
	self->quote.top = quote_rect.y;
	self->quote.is_writable = FALSE;
	return 0;
}

int Graph_Quote(LCUI_Graph *self, LCUI_Graph *source, const LCUI_Rect *rect)
{
	int ret = Graph_QuoteReadOnly(self, source, rect);
	self->quote.is_writable = TRUE;
	return ret;
}

void Graph_GetValidRect(const LCUI_Graph *graph, LCUI_Rect *rect)
{
	if (graph->quote.is_valid) {
		rect->x = graph->quote.left;
		rect->y = graph->quote.top;
	} else {
		rect->x = 0;
		rect->y = 0;
	}
	rect->width = graph->width;
	rect->height = graph->height;
}

int Graph_SetAlphaBits(LCUI_Graph *graph, uchar_t *a, size_t size)
{
	size_t i;
	if (size > (size_t)(graph->width * graph->height)) {
		size = (size_t)(graph->width * graph->height);
	}
	if (graph->color_type != LCUI_COLOR_TYPE_ARGB) {
		return -2;
	}
	for (i = 0; i < size; ++i) {
		graph->argb[i].a = a[i];
	}
	return 0;
}

int Graph_SetRedBits(LCUI_Graph *graph, uchar_t *r, size_t size)
{
	size_t i;
	uchar_t *pByte;

	if (size > (size_t)(graph->width * graph->height)) {
		size = (size_t)(graph->width * graph->height);
	}
	if (graph->color_type == LCUI_COLOR_TYPE_ARGB) {
		for (i = 0; i < size; ++i) {
			graph->argb[i].r = r[i];
		}
	} else {
		pByte = graph->bytes + 2;
		for (i = 0; i < size; ++i) {
			*pByte = r[i];
			pByte += 3;
		}
	}
	return 0;
}

int Graph_SetGreenBits(LCUI_Graph *graph, uchar_t *g, size_t size)
{
	size_t i;
	uchar_t *pByte;

	if (size > (size_t)(graph->width * graph->height)) {
		size = (size_t)(graph->width * graph->height);
	}
	if (graph->color_type == LCUI_COLOR_TYPE_ARGB) {
		for (i = 0; i < size; ++i) {
			graph->argb[i].g = g[i];
		}
	} else {
		pByte = graph->bytes + 1;
		for (i = 0; i < size; ++i) {
			*pByte = g[i];
			pByte += 3;
		}
	}
	return 0;
}

int Graph_SetBlueBits(LCUI_Graph *graph, uchar_t *b, size_t size)
{
	size_t i;
	uchar_t *pByte;

	if (size > (size_t)(graph->width * graph->height)) {
		size = (size_t)(graph->width * graph->height);
	}
	if (graph->color_type == LCUI_COLOR_TYPE_ARGB) {
		for (i = 0; i < size; ++i) {
			graph->argb[i].b = b[i];
		}
	} else {
		pByte = graph->bytes;
		for (i = 0; i < size; ++i) {
			*pByte = b[i];
			pByte += 3;
		}
	}
	return 0;
}

int Graph_Zoom(const LCUI_Graph *graph, LCUI_Graph *buff, LCUI_BOOL keep_scale,
	       int width, int height)
{
	LCUI_Rect rect;
	int x, y, src_x, src_y;
	double scale_x = 0.0, scale_y = 0.0;
	if (!Graph_IsValid(graph) || (width <= 0 && height <= 0)) {
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	if (width > 0) {
		scale_x = 1.0 * rect.width / width;
	}
	if (height > 0) {
		scale_y = 1.0 * rect.height / height;
	}
	if (width <= 0) {
		scale_x = scale_y;
		width = (int)(0.5 + 1.0 * graph->width / scale_x);
	}
	if (height <= 0) {
		scale_y = scale_x;
		height = (int)(0.5 + 1.0 * graph->height / scale_y);
	}
	/* 如果保持宽高比 */
	if (keep_scale) {
		if (scale_x < scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = graph->color_type;
	if (Graph_Create(buff, width, height) < 0) {
		return -2;
	}
	if (graph->color_type == LCUI_COLOR_TYPE_ARGB) {
		LCUI_ARGB *px_src, *px_des, *px_row_src;
		for (y = 0; y < height; ++y) {
			src_y = (int)(y * scale_y);
			px_row_src = graph->argb;
			px_row_src += (src_y + rect.y) * graph->width + rect.x;
			px_des = buff->argb + y * width;
			for (x = 0; x < width; ++x) {
				src_x = (int)(x * scale_x);
				px_src = px_row_src + src_x;
				*px_des++ = *px_src;
			}
		}
	} else {
		uchar_t *byte_src, *byte_des, *byte_row_src;
		for (y = 0; y < height; ++y) {
			src_y = (int)(y * scale_y);
			byte_row_src = graph->bytes;
			byte_row_src += (src_y + rect.y) * graph->bytes_per_row;
			byte_row_src += rect.x * graph->bytes_per_pixel;
			byte_des = buff->bytes + y * buff->bytes_per_row;
			for (x = 0; x < width; ++x) {
				src_x = (int)(x * scale_x);
				src_x *= graph->bytes_per_pixel;
				byte_src = byte_row_src + src_x;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src++;
				*byte_des++ = *byte_src;
			}
		}
	}
	return 0;
}

int Graph_ZoomBilinear(const LCUI_Graph *graph, LCUI_Graph *buff,
		       LCUI_BOOL keep_scale, int width, int height)
{
	LCUI_Rect rect;
	LCUI_ARGB a, b, c, d, t_color;

	int x, y, i, j;
	float x_diff, y_diff;
	double scale_x = 0.0, scale_y = 0.0;

	if (graph->color_type != LCUI_COLOR_TYPE_RGB &&
	    graph->color_type != LCUI_COLOR_TYPE_ARGB) {
		/* fall back to nearest scaling */
		Logger_Debug("[graph] unable to perform bilinear scaling, "
			     "fallback...\n");
		return Graph_Zoom(graph, buff, keep_scale, width, height);
	}
	if (!Graph_IsValid(graph) || (width <= 0 && height <= 0)) {
		return -1;
	}
	/* 获取引用的有效区域，以及指向引用的对象的指针 */
	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	if (width > 0) {
		scale_x = 1.0 * rect.width / width;
	}
	if (height > 0) {
		scale_y = 1.0 * rect.height / height;
	}
	if (width <= 0) {
		scale_x = scale_y;
		width = (int)(0.5 + 1.0 * graph->width / scale_x);
	}
	if (height <= 0) {
		scale_y = scale_x;
		height = (int)(0.5 + 1.0 * graph->height / scale_y);
	}
	/* 如果保持宽高比 */
	if (keep_scale) {
		if (scale_x < scale_y) {
			scale_y = scale_x;
		} else {
			scale_x = scale_y;
		}
	}
	buff->color_type = graph->color_type;
	if (Graph_Create(buff, width, height) < 0) {
		return -2;
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			/*
			 * Qmn = (m, n).
			 * Qxy1 = (x2 - x) / (x2 - x1) * Q11 + (x - x1) / (x2
			 * - x1) * Q21
			 * Qxy2 = (x2 - x) / (x2 - x1) * Q12 + (x - x1) / (x2
			 * - x1) * Q22
			 * Qxy = (y2 - y) / (y2 - y1) * Qxy1 + (y - y1) / (y2 -
			 * y1) * Qxy2
			 */
			x = (int)(scale_x * j);
			y = (int)(scale_y * i);
			x_diff = (float)((scale_x * j) - x);
			y_diff = (float)((scale_y * i) - y);
			Graph_GetPixel(graph, x + rect.x + 0, y + rect.y + 0,
				       a);
			Graph_GetPixel(graph, x + rect.x + 1, y + rect.y + 0,
				       b);
			Graph_GetPixel(graph, x + rect.x + 0, y + rect.y + 1,
				       c);
			Graph_GetPixel(graph, x + rect.x + 1, y + rect.y + 1,
				       d);
			t_color.b = Graph_BilinearResamplingCore(
			    a.b, b.b, c.b, d.b, x_diff, y_diff);
			t_color.g = Graph_BilinearResamplingCore(
			    a.g, b.g, c.g, d.g, x_diff, y_diff);
			t_color.r = Graph_BilinearResamplingCore(
			    a.r, b.r, c.r, d.r, x_diff, y_diff);
			t_color.a = Graph_BilinearResamplingCore(
			    a.a, b.a, c.a, d.a, x_diff, y_diff);
			Graph_SetPixel(buff, j, i, t_color);
		}
	}
	return 0;
}

int Graph_Cut(const LCUI_Graph *graph, LCUI_Rect rect, LCUI_Graph *buff)
{
	if (!Graph_IsValid(graph)) {
		return -2;
	}
	LCUIRect_ValidateArea(&rect, graph->width, graph->height);
	if (rect.width <= 0 || rect.height <= 0) {
		return -3;
	}
	switch (graph->color_type) {
	case LCUI_COLOR_TYPE_ARGB8888:
		return Graph_CutARGB(graph, rect, buff);
	case LCUI_COLOR_TYPE_RGB888:
		return Graph_CutRGB(graph, rect, buff);
	default:
		break;
	}
	return -4;
}

int Graph_HorizFlip(const LCUI_Graph *graph, LCUI_Graph *buff)
{
	switch (graph->color_type) {
	case LCUI_COLOR_TYPE_RGB888:
		return Graph_HorizFlipRGB(graph, buff);
	case LCUI_COLOR_TYPE_ARGB8888:
		return Graph_HorizFlipARGB(graph, buff);
	default:
		break;
	}
	return -1;
}

int Graph_VertiFlip(const LCUI_Graph *graph, LCUI_Graph *buff)
{
	switch (graph->color_type) {
	case LCUI_COLOR_TYPE_RGB888:
		return Graph_VertiFlipRGB(graph, buff);
	case LCUI_COLOR_TYPE_ARGB8888:
		return Graph_VertiFlipARGB(graph, buff);
	default:
		break;
	}
	return -1;
}

int Graph_FillRect(LCUI_Graph *graph, LCUI_Color color, LCUI_Rect *rect,
		   LCUI_BOOL with_alpha)
{
	LCUI_Rect rect2;
	if (rect) {
		rect2 = *rect;
	} else {
		rect2.x = rect2.y = 0;
		rect2.width = graph->width;
		rect2.height = graph->height;
	}
	switch (graph->color_type) {
	case LCUI_COLOR_TYPE_RGB888:
		return Graph_FillRectRGB(graph, color, rect2);
	case LCUI_COLOR_TYPE_ARGB8888:
		return Graph_FillRectARGB(graph, color, rect2, with_alpha);
	default:
		break;
	}
	return -1;
}

int Graph_FillAlpha(LCUI_Graph *graph, uchar_t alpha)
{
	int x, y;
	LCUI_Rect rect;
	LCUI_ARGB *pixel, *pixel_row;

	Graph_GetValidRect(graph, &rect);
	graph = Graph_GetQuote(graph);
	if (!Graph_IsValid(graph)) {
		return -1;
	}
	if (!Graph_HasAlpha(graph)) {
		return -2;
	}
	pixel_row = graph->argb + rect.y * graph->width + rect.x;
	for (y = 0; y < rect.height; ++y) {
		pixel = pixel_row;
		for (x = 0; x < rect.width; ++x) {
			pixel->alpha = alpha;
			++pixel;
		}
		pixel_row += graph->width;
	}
	return 0;
}

int Graph_Tile(LCUI_Graph *buff, const LCUI_Graph *graph, LCUI_BOOL replace,
	       LCUI_BOOL with_alpha)
{
	int ret = 0;
	unsigned x, y;

	if (!Graph_IsValid(graph) || !Graph_IsValid(buff)) {
		return -1;
	}
	for (y = 0; y < buff->height; y += graph->height) {
		for (x = 0; x < buff->width; x += graph->width) {
			if (replace) {
				ret += Graph_Replace(buff, graph, y, x);
				continue;
			}
			ret += Graph_Mix(buff, graph, y, x, with_alpha);
		}
	}
	return ret;
}

typedef void (*MixerPtr)(LCUI_Graph *, LCUI_Rect, const LCUI_Graph *, int, int);

int Graph_Mix(LCUI_Graph *back, const LCUI_Graph *fore, int left, int top,
	      LCUI_BOOL with_alpha)
{
	LCUI_Graph w_slot;
	LCUI_Rect r_rect, w_rect;
	MixerPtr mixer = NULL;

	if (!Graph_IsWritable(back) || !Graph_IsValid(fore)) {
		return -1;
	}
	w_rect.x = left;
	w_rect.y = top;
	w_rect.width = fore->width;
	w_rect.height = fore->height;
	LCUIRect_GetCutArea(back->width, back->height, w_rect, &r_rect);
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	Graph_Quote(&w_slot, back, &w_rect);
	/* 获取实际操作区域 */
	Graph_GetValidRect(&w_slot, &w_rect);
	Graph_GetValidRect(fore, &r_rect);
	if (w_rect.width <= 0 || w_rect.height <= 0 || r_rect.width <= 0 ||
	    r_rect.height <= 0) {
		return -2;
	}
	top = r_rect.y;
	left = r_rect.x;
	/* 获取引用的源图像 */
	fore = Graph_GetQuote(fore);
	back = Graph_GetQuote(back);
	switch (fore->color_type) {
	case LCUI_COLOR_TYPE_RGB888:
		if (back->color_type == LCUI_COLOR_TYPE_RGB888) {
			mixer = Graph_ReplaceRGB;
		} else {
			mixer = Graph_ReplaceToARGB;
		}
		break;
	case LCUI_COLOR_TYPE_ARGB8888:
		if (back->color_type == LCUI_COLOR_TYPE_RGB888) {
			mixer = Graph_MixARGBToRGB;
		} else {
			if (with_alpha) {
				mixer = Graph_MixARGBWithAlpha;
			} else {
				mixer = Graph_MixARGB;
			}
		}
	default:
		break;
	}
	if (mixer) {
		mixer(back, w_rect, fore, left, top);
		return 0;
	}
	return -3;
}

int Graph_Replace(LCUI_Graph *back, const LCUI_Graph *fore, int left, int top)
{
	LCUI_Graph write_slot;
	LCUI_Rect read_rect, write_rect;

	if (!Graph_IsWritable(back) || !Graph_IsValid(fore)) {
		return -1;
	}
	write_rect.x = left;
	write_rect.y = top;
	write_rect.width = fore->width;
	write_rect.height = fore->height;
	Graph_Quote(&write_slot, back, &write_rect);
	Graph_GetValidRect(&write_slot, &write_rect);
	Graph_GetValidRect(fore, &read_rect);
	if (write_rect.width <= 0 || write_rect.height <= 0 ||
	    read_rect.width <= 0 || read_rect.height <= 0) {
		return -2;
	}
	left = read_rect.x;
	top = read_rect.y;
	fore = Graph_GetQuote(fore);
	back = Graph_GetQuote(back);
	switch (fore->color_type) {
	case LCUI_COLOR_TYPE_RGB888:
		Graph_ReplaceRGB(back, write_rect, fore, left, top);
		break;
	case LCUI_COLOR_TYPE_ARGB8888:
		Graph_ReplaceARGB(back, write_rect, fore, left, top);
	default:
		break;
	}
	return -1;
}
