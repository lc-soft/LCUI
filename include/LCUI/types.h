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

/* FIXME: remove css_keyword_value_t
 * These values do not need to put in css_keyword_value_t, because they are not
 * strongly related and should be defined separately where they are needed.
 */

typedef enum css_keyword_value_t {
	CSS_KEYWORD_NONE,
	CSS_KEYWORD_AUTO,
	CSS_KEYWORD_NORMAL,
	CSS_KEYWORD_INHERIT,
	CSS_KEYWORD_INITIAL,
	CSS_KEYWORD_CONTAIN,
	CSS_KEYWORD_COVER,
	CSS_KEYWORD_LEFT,
	CSS_KEYWORD_CENTER,
	CSS_KEYWORD_RIGHT,
	CSS_KEYWORD_TOP,
	CSS_KEYWORD_TOP_LEFT,
	CSS_KEYWORD_TOP_CENTER,
	CSS_KEYWORD_TOP_RIGHT,
	CSS_KEYWORD_MIDDLE,
	CSS_KEYWORD_CENTER_LEFT,
	CSS_KEYWORD_CENTER_CENTER,
	CSS_KEYWORD_CENTER_RIGHT,
	CSS_KEYWORD_BOTTOM,
	CSS_KEYWORD_BOTTOM_LEFT,
	CSS_KEYWORD_BOTTOM_CENTER,
	CSS_KEYWORD_BOTTOM_RIGHT,
	CSS_KEYWORD_SOLID,
	CSS_KEYWORD_DOTTED,
	CSS_KEYWORD_DOUBLE,
	CSS_KEYWORD_DASHED,
	CSS_KEYWORD_CONTENT_BOX,
	CSS_KEYWORD_PADDING_BOX,
	CSS_KEYWORD_BORDER_BOX,
	CSS_KEYWORD_GRAPH_BOX,
	CSS_KEYWORD_STATIC,
	CSS_KEYWORD_RELATIVE,
	CSS_KEYWORD_ABSOLUTE,
	CSS_KEYWORD_BLOCK,
	CSS_KEYWORD_INLINE_BLOCK,
	CSS_KEYWORD_FLEX,
	CSS_KEYWORD_FLEX_START,
	CSS_KEYWORD_FLEX_END,
	CSS_KEYWORD_STRETCH,
	CSS_KEYWORD_SPACE_BETWEEN,
	CSS_KEYWORD_SPACE_AROUND,
	CSS_KEYWORD_SPACE_EVENLY,
	CSS_KEYWORD_WRAP,
	CSS_KEYWORD_NOWRAP,
	CSS_KEYWORD_ROW,
	CSS_KEYWORD_COLUMN
} css_keyword_value_t;

/** 样式变量类型 */
typedef enum css_unit_t {
	CSS_UNIT_NONE,
	CSS_UNIT_AUTO,
	CSS_UNIT_SCALE,
	CSS_UNIT_PX,
	CSS_UNIT_PT,
	CSS_UNIT_DIP,
	CSS_UNIT_SP,
	CSS_UNIT_COLOR,
	CSS_UNIT_IMAGE,
	CSS_UNIT_STYLE,
	CSS_UNIT_INT,
	CSS_UNIT_BOOL,
	CSS_UNIT_STRING,
	CSS_UNIT_WSTRING
} css_unit_t;

#define CSS_UNIT_px CSS_UNIT_PX
#define CSS_UNIT_pt CSS_UNIT_PT
#define CSS_UNIT_int CSS_UNIT_INT
#define CSS_UNIT_color CSS_UNIT_COLOR
#define CSS_UNIT_scale CSS_UNIT_SCALE
#define CSS_UNIT_style CSS_UNIT_STYLE
#define CSS_UNIT_bool CSS_UNIT_BOOL
#define CSS_UNIT_image CSS_UNIT_IMAGE
#define CSS_UNIT_string CSS_UNIT_STRING
#define CSS_UNIT_wstring CSS_UNIT_WSTRING
#define CSS_UNIT_sp CSS_UNIT_SP
#define CSS_UNIT_dp CSS_UNIT_DIP
#define CSS_UNIT_dip CSS_UNIT_DIP
#define CSS_UNIT_0 CSS_UNIT_NONE
#define CSS_UNIT_none CSS_UNIT_NONE

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

typedef struct css_unit_value_t {
	LCUI_BOOL is_valid : 2;
	css_unit_t type : 6;
	union {
		int val_int;
		int val_0;
		int val_none;
		float value;
		float px;
		float val_px;
		float pt;
		float val_pt;
		float dp;
		float val_dp;
		float dip;
		float val_dip;
		float sp;
		float val_sp;
		css_keyword_value_t style;
		css_keyword_value_t val_style;
		float scale;
		float val_scale;
		char *string;
		char *val_string;
		wchar_t *wstring;
		wchar_t *val_wstring;
		pd_color_t color;
		pd_color_t val_color;
		pd_canvas_t *image;
		pd_canvas_t *val_image;
		LCUI_BOOL val_bool;
	};
} css_unit_value_t;

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
