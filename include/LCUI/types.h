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

#include <wchar.h>
#include <stdint.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#include <time.h>

#define LCUI_MAX_FRAMES_PER_SEC 120
#define LCUI_MAX_FRAME_MSEC ((int)(1000.0 / LCUI_MAX_FRAMES_PER_SEC + 0.5))
#define ASSIGN(NAME, TYPE) TYPE NAME = (TYPE)malloc(sizeof(TYPE##Rec))
#define ZEROSET(NAME, TYPE) memset(NAME, 0, sizeof(TYPE##Rec))
#define NEW(TYPE, COUNT) (TYPE *)calloc(COUNT, sizeof(TYPE))
#define CodeToString(...) "" #__VA_ARGS__ ""

LCUI_BEGIN_HEADER

typedef unsigned int uint_t;
typedef unsigned char LCUI_BOOL;
typedef unsigned char uchar_t;
typedef void (*CallBackFunc)(void *, void *);

/** 色彩模式 */
typedef enum LCUI_ColorType {
	LCUI_COLOR_TYPE_INDEX8,   /**< 8位索引 */
	LCUI_COLOR_TYPE_GRAY8,    /**< 8位灰度 */
	LCUI_COLOR_TYPE_RGB323,   /**< RGB323 */
	LCUI_COLOR_TYPE_ARGB2222, /**< ARGB2222 */
	LCUI_COLOR_TYPE_RGB555,   /**< RGB555 */
	LCUI_COLOR_TYPE_RGB565,   /**< RGB565 */
	LCUI_COLOR_TYPE_RGB888,   /**< RGB888 */
	LCUI_COLOR_TYPE_ARGB8888  /**< RGB8888 */
} LCUI_ColorType;

#define LCUI_COLOR_TYPE_RGB LCUI_COLOR_TYPE_RGB888
#define LCUI_COLOR_TYPE_ARGB LCUI_COLOR_TYPE_ARGB8888

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

typedef union LCUI_ARGB8888_ {
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
} LCUI_ARGB, LCUI_ARGB8888, LCUI_Color;

typedef struct LCUI_Pos_ {
	int x, y;
} LCUI_Pos;

typedef struct LCUI_Size_ {
	int width, height;
} LCUI_Size;

typedef struct LCUI_Rect_ {
	int x, y, width, height;
} LCUI_Rect;

typedef struct LCUI_Rect2_ {
	int left, top, right, bottom;
} LCUI_Rect2;

typedef struct LCUI_RectF_ {
	float x, y, width, height;
} LCUI_RectF;

typedef struct LCUI_Rect2F_ {
	float left, top, right, bottom;
} LCUI_Rect2F;

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

typedef struct LCUI_BoxShadowStyle {
	float x, y;
	float blur;
	float spread;
	LCUI_Color color;
} LCUI_BoxShadowStyle;

typedef struct LCUI_BoxShadow {
	int x, y;
	int blur;
	int spread;
	LCUI_Color color;
	int top_left_radius;
	int top_right_radius;
	int bottom_left_radius;
	int bottom_right_radius;
} LCUI_BoxShadow;

typedef struct LCUI_BorderStyle {
	struct {
		int style;
		float width;
		LCUI_Color color;
	} top, right, bottom, left;
	float top_left_radius;
	float top_right_radius;
	float bottom_left_radius;
	float bottom_right_radius;
} LCUI_BorderStyle;

typedef struct LCUI_BorderLine {
	int style;
	unsigned int width;
	LCUI_Color color;
} LCUI_BorderLine;

typedef struct LCUI_Border {
	LCUI_BorderLine top, right, bottom, left;
	unsigned int top_left_radius;
	unsigned int top_right_radius;
	unsigned int bottom_left_radius;
	unsigned int bottom_right_radius;
} LCUI_Border;

typedef struct LCUI_Graph_ LCUI_Graph;

typedef struct LCUI_GraphQuote_ {
	int top;
	int left;
	LCUI_BOOL is_valid;
	LCUI_BOOL is_writable;
	union {
		LCUI_Graph *source;
		const LCUI_Graph *source_ro;
	};
} LCUI_GraphQuote;

struct LCUI_Graph_ {
	unsigned width;
	unsigned height;
	LCUI_GraphQuote quote;
	union {
		uchar_t *bytes;
		LCUI_ARGB *argb;
	};
	int color_type;
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
		LCUI_Color color;
		LCUI_Color val_color;
		LCUI_Graph *image;
		LCUI_Graph *val_image;
		LCUI_BOOL val_bool;
	};
} LCUI_StyleRec, *LCUI_Style;

typedef enum LCUI_SizingRule_ {
	LCUI_SIZING_RULE_NONE,
	LCUI_SIZING_RULE_FIXED,
	LCUI_SIZING_RULE_FILL,
	LCUI_SIZING_RULE_FIT_CONTENT
} LCUI_SizingRule;

typedef struct LCUI_FlexLayoutStyle {
	/**
	 * The flex shrink factor of a flex item
	 * See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/flex-shrink
	 */
	float shrink;

	/* the flex grow factor of a flex item main size
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex-grow
	 */
	float grow;

	/**
	 * The initial main size of a flex item
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex-basis
	 */
	float basis;

	LCUI_StyleValue wrap : 8;
	LCUI_StyleValue direction : 8;

	/**
	 * Sets the align-self value on all direct children as a group
	 * See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/align-items
	 */
	LCUI_StyleValue align_items : 8;

	/**
	 * Sets the distribution of space between and around content items along
	 * a flexbox's cross-axis
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/align-content
	 */
	LCUI_StyleValue align_content : 8;

	/**
	 * Defines how the browser distributes space between and around content
	 * items along the main-axis of a flex container See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/justify-content
	 */
	LCUI_StyleValue justify_content : 8;
} LCUI_FlexBoxLayoutStyle;

typedef struct LCUI_BoundBoxRec {
	LCUI_StyleRec top, right, bottom, left;
} LCUI_BoundBox;

typedef struct LCUI_BackgroundPosition {
	LCUI_BOOL using_value;
	union {
		struct {
			LCUI_StyleRec x, y;
		};
		int value;
	};
} LCUI_BackgroundPosition;

typedef struct LCUI_BackgroundSize {
	LCUI_BOOL using_value;
	union {
		struct {
			LCUI_StyleRec width, height;
		};
		int value;
	};
} LCUI_BackgroundSize;

typedef struct LCUI_BackgroundStyle {
	LCUI_Graph image; /**< 背景图 */
	LCUI_Color color; /**< 背景色 */
	struct {
		LCUI_BOOL x, y;
	} repeat;                         /**< 背景图是否重复 */
	LCUI_BackgroundPosition position; /**< 定位方式 */
	LCUI_BackgroundSize size;         /**< 尺寸 */
} LCUI_BackgroundStyle;

typedef struct LCUI_Background {
	LCUI_Graph *image; /**< 背景图 */
	LCUI_Color color;  /**< 背景色 */
	struct {
		LCUI_BOOL x, y;
	} repeat; /**< 背景图是否重复 */
	struct {
		int x, y;
	} position;
	struct {
		int width, height;
	} size;
} LCUI_Background;

/** 进行绘制时所需的上下文 */
typedef struct LCUI_PaintContextRec_ {
	LCUI_Rect rect;    /**< 需要绘制的区域 */
	LCUI_Graph canvas; /**< 绘制后的位图缓存（可称为：画布） */
	LCUI_BOOL with_alpha; /**< 绘制时是否需要处理 alpha 通道 */
} LCUI_PaintContextRec, *LCUI_PaintContext;

typedef void (*FuncPtr)(void *);

typedef struct LCUI_WidgetTasksRec_ {
	clock_t time;
	size_t update_count;
	size_t refresh_count;
	size_t layout_count;
	size_t user_task_count;
	size_t destroy_count;
	size_t destroy_time;
} LCUI_WidgetTasksProfileRec, *LCUI_WidgetTasksProfile;

typedef struct LCUI_FrameProfileRec_ {
	size_t timers_count;
	clock_t timers_time;

	size_t events_count;
	clock_t events_time;

	size_t render_count;
	clock_t render_time;
	clock_t present_time;

	LCUI_WidgetTasksProfileRec widget_tasks;
} LCUI_FrameProfileRec, *LCUI_FrameProfile;

typedef struct LCUI_ProfileRec_ {
	clock_t start_time;
	clock_t end_time;
	unsigned frames_count;
	LCUI_FrameProfileRec frames[LCUI_MAX_FRAMES_PER_SEC];
} LCUI_ProfileRec, *LCUI_Profile;

LCUI_END_HEADER

#endif
