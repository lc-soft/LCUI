/* types.h -- The common macros and types definition for LCUI
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

#ifndef LCUI_TYPES_H
#define LCUI_TYPES_H

#include <LCUI/header.h>
#include <wchar.h>
#include <stdint.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#include <time.h>

#define CodeToString(...) "" #__VA_ARGS__ ""

LCUI_BEGIN_HEADER

typedef unsigned int uint_t;
typedef unsigned char LCUI_BOOL;
typedef unsigned char uchar_t;
typedef void (*CallBackFunc)(void *, void *);

// used for PandaGL
typedef unsigned char pd_bool;

/** 色彩模式 */
typedef enum pd_color_type {
	PD_COLOR_TYPE_INDEX8,   /**< 8位索引 */
	PD_COLOR_TYPE_GRAY8,    /**< 8位灰度 */
	PD_COLOR_TYPE_RGB323,   /**< RGB323 */
	PD_COLOR_TYPE_ARGB2222, /**< ARGB2222 */
	PD_COLOR_TYPE_RGB555,   /**< RGB555 */
	PD_COLOR_TYPE_RGB565,   /**< RGB565 */
	PD_COLOR_TYPE_RGB888,   /**< RGB888 */
	PD_COLOR_TYPE_ARGB8888  /**< RGB8888 */
} pd_color_type;

#define PD_COLOR_TYPE_RGB PD_COLOR_TYPE_RGB888
#define PD_COLOR_TYPE_ARGB PD_COLOR_TYPE_ARGB8888

typedef union LCUI_RGB565_ {
	short unsigned int value;
	struct {
		uchar_t b : 5;
		uchar_t g : 6;
		uchar_t r : 5;
	};
	struct {
		uchar_t blue : 5;
		uchar_t green : 6;
		uchar_t red : 5;
	};
} LCUI_RGB565;

typedef union pd_color_t_ {
	int32_t value;
	struct {
		uchar_t b;
		uchar_t g;
		uchar_t r;
		uchar_t a;
	};
	struct {
		uchar_t blue;
		uchar_t green;
		uchar_t red;
		uchar_t alpha;
	};
} pd_color_t;

/** Position in plane coordinate system */
typedef struct pd_pos_t_ {
	int x, y;
} pd_pos_t;

typedef struct LCUI_Size_ {
	int width, height;
} LCUI_Size;

typedef struct pd_rect_t_ {
	int x, y, width, height;
} pd_rect_t;

typedef struct LCUI_Rect2_ {
	int left, top, right, bottom;
} LCUI_Rect2;

typedef struct pd_rectf_t_ {
	float x, y, width, height;
} pd_rectf_t;

typedef struct LCUI_Rect2F_ {
	float left, top, right, bottom;
} pd_rect_t2F;

typedef struct pd_boxshadow_t_ {
	int x, y;
	int blur;
	int spread;
	pd_color_t color;
	int top_left_radius;
	int top_right_radius;
	int bottom_left_radius;
	int bottom_right_radius;
} pd_boxshadow_t;

typedef struct pd_border_line_t {
	int style;
	unsigned int width;
	pd_color_t color;
} pd_border_line_t;

typedef struct pd_border_t {
	pd_border_line_t top, right, bottom, left;
	unsigned int top_left_radius;
	unsigned int top_right_radius;
	unsigned int bottom_left_radius;
	unsigned int bottom_right_radius;
} pd_border_t;

typedef struct pd_canvas_t_ pd_canvas_t;

typedef struct pd_graph_quote_t_ {
	int top;
	int left;
	pd_bool is_valid;
	pd_bool is_writable;
	union {
		pd_canvas_t *source;
		const pd_canvas_t *source_ro;
	};
} pd_canvas_quote_t;

struct pd_canvas_t_ {
	unsigned width;
	unsigned height;
	pd_canvas_quote_t quote;
	union {
		uchar_t *bytes;
		pd_color_t *argb;
	};
	pd_color_type color_type;
	unsigned bytes_per_pixel;
	unsigned bytes_per_row;
	float opacity;
	size_t mem_size;
	uchar_t *palette;
};

typedef struct pd_background_t {
	pd_canvas_t *image; /**< 背景图 */
	pd_color_t color;  /**< 背景色 */
	struct {
		LCUI_BOOL x, y;
	} repeat; /**< 背景图是否重复 */
	struct {
		int x, y;
	} position;
	struct {
		int width, height;
	} size;
} pd_background_t;

/** 进行绘制时所需的上下文 */
typedef struct pd_paint_context_t_ {
	pd_rect_t rect;    /**< 需要绘制的区域 */
	pd_canvas_t canvas; /**< 绘制后的位图缓存（可称为：画布） */
	pd_bool with_alpha; /**< 绘制时是否需要处理 alpha 通道 */
} pd_paint_context_t;

typedef void(*LCUI_TaskFunc)(void*, void*);

LCUI_END_HEADER

#endif
