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

/* FIXME: remove LCUI_StyleValue
 * These values do not need to put in LCUI_StyleValue, because they are not
 * strongly related and should be defined separately where they are needed.
 */

typedef enum LCUI_StyleValue {
	SV_NONE,
	SV_AUTO,
	SV_NORMAL,
	SV_INHERIT,
	SV_INITIAL,
	SV_CONTAIN,
	SV_COVER,
	SV_LEFT,
	SV_CENTER,
	SV_RIGHT,
	SV_TOP,
	SV_TOP_LEFT,
	SV_TOP_CENTER,
	SV_TOP_RIGHT,
	SV_MIDDLE,
	SV_CENTER_LEFT,
	SV_CENTER_CENTER,
	SV_CENTER_RIGHT,
	SV_BOTTOM,
	SV_BOTTOM_LEFT,
	SV_BOTTOM_CENTER,
	SV_BOTTOM_RIGHT,
	SV_SOLID,
	SV_DOTTED,
	SV_DOUBLE,
	SV_DASHED,
	SV_CONTENT_BOX,
	SV_PADDING_BOX,
	SV_BORDER_BOX,
	SV_GRAPH_BOX,
	SV_STATIC,
	SV_RELATIVE,
	SV_ABSOLUTE,
	SV_BLOCK,
	SV_INLINE_BLOCK,
	SV_FLEX,
	SV_FLEX_START,
	SV_FLEX_END,
	SV_STRETCH,
	SV_SPACE_BETWEEN,
	SV_SPACE_AROUND,
	SV_SPACE_EVENLY,
	SV_WRAP,
	SV_NOWRAP,
	SV_ROW,
	SV_COLUMN
} LCUI_StyleValue;

/** 样式变量类型 */
typedef enum LCUI_StyleType {
	LCUI_STYPE_NONE,
	LCUI_STYPE_AUTO,
	LCUI_STYPE_SCALE,
	LCUI_STYPE_PX,
	LCUI_STYPE_PT,
	LCUI_STYPE_DIP,
	LCUI_STYPE_SP,
	LCUI_STYPE_COLOR,
	LCUI_STYPE_IMAGE,
	LCUI_STYPE_STYLE,
	LCUI_STYPE_INT,
	LCUI_STYPE_BOOL,
	LCUI_STYPE_STRING,
	LCUI_STYPE_WSTRING
} LCUI_StyleType;

#define LCUI_STYPE_px LCUI_STYPE_PX
#define LCUI_STYPE_pt LCUI_STYPE_PT
#define LCUI_STYPE_int LCUI_STYPE_INT
#define LCUI_STYPE_color LCUI_STYPE_COLOR
#define LCUI_STYPE_scale LCUI_STYPE_SCALE
#define LCUI_STYPE_style LCUI_STYPE_STYLE
#define LCUI_STYPE_bool LCUI_STYPE_BOOL
#define LCUI_STYPE_image LCUI_STYPE_IMAGE
#define LCUI_STYPE_string LCUI_STYPE_STRING
#define LCUI_STYPE_wstring LCUI_STYPE_WSTRING
#define LCUI_STYPE_sp LCUI_STYPE_SP
#define LCUI_STYPE_dp LCUI_STYPE_DIP
#define LCUI_STYPE_dip LCUI_STYPE_DIP
#define LCUI_STYPE_0 LCUI_STYPE_NONE
#define LCUI_STYPE_none LCUI_STYPE_NONE

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

typedef struct LCUI_StyleRec_ {
	LCUI_BOOL is_valid : 2;
	LCUI_StyleType type : 6;
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
		LCUI_StyleValue style;
		LCUI_StyleValue val_style;
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
} LCUI_StyleRec, *LCUI_Style;

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
