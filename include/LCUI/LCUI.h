/*
 * LCUI.h -- common data type definitions, macro definitions and function
 * declarations.
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

#ifndef LCUI_H
#define LCUI_H

#define LCUI_VERSION "1.0.0"

#include <wchar.h>
#include <stdint.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define ASSIGN(NAME, TYPE) TYPE NAME = (TYPE)malloc( sizeof(TYPE##Rec) )
#define ZEROSET(NAME, TYPE) memset(NAME, 0, sizeof(TYPE##Rec))
#define NEW(TYPE, COUNT) (TYPE*)calloc(COUNT, sizeof(TYPE))
#define CodeToString(...) ""#__VA_ARGS__""

LCUI_BEGIN_HEADER

typedef unsigned int uint_t;
typedef unsigned char LCUI_BOOL;
typedef unsigned char uchar_t;
typedef void (*CallBackFunc)(void*,void*);

typedef union LCUI_RGB565_ {
	short unsigned int value;
	struct {
		uchar_t b:5;
		uchar_t g:6;
		uchar_t r:5;
	};
	struct {
		uchar_t blue:5;
		uchar_t green:6;
		uchar_t red:5;
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

/** 样式值枚举，用于代替使用字符串 */
typedef enum LCUI_StyleValue {
	SV_NONE,
	SV_AUTO,
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
	SV_NOWRAP
} LCUI_StyleValue;

/** 样式变量类型 */
typedef enum LCUI_StyleType {
	SVT_NONE,
	SVT_AUTO,
	SVT_SCALE,
	SVT_PX,
	SVT_PT,
	SVT_DIP,
	SVT_SP,
	SVT_COLOR,
	SVT_IMAGE,
	SVT_STYLE,
	SVT_VALUE,
	SVT_BOOL,
	SVT_STRING,
	SVT_WSTRING
} LCUI_StyleType;

#define SVT_px		SVT_PX
#define SVT_pt		SVT_PT
#define SVT_value	SVT_VALUE
#define SVT_int		SVT_VALUE
#define SVT_color	SVT_COLOR
#define SVT_scale	SVT_SCALE
#define SVT_style	SVT_STYLE
#define SVT_data	SVT_DATA
#define SVT_bool	SVT_BOOL
#define SVT_image	SVT_IMAGE
#define SVT_string	SVT_STRING
#define SVT_wstring	SVT_WSTRING
#define SVT_sp		SVT_SP
#define SVT_dp		SVT_DIP
#define SVT_dip		SVT_DIP
#define SVT_0		SVT_NONE
#define SVT_none	SVT_NONE

typedef struct LCUI_BoxShadowStyle {
	float x, y;
	float blur;
	float spread;
	LCUI_Color color;
} LCUI_BoxShadowStyle;

/** 盒形阴影相关参数 */
typedef struct LCUI_BoxShadow {
	int x, y;		/**< 位置 */
	int blur;		/**< 模糊距离 */
	int spread;		/**< 扩散大小 */
	LCUI_Color color;	/**<　颜色　*/
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

/** 弹性（Flex）布局相关样式 */
typedef struct LCUI_FlexLayoutStyle {
	LCUI_StyleValue wrap:8;
	LCUI_StyleValue flow:8;
	LCUI_StyleValue grow:8;
	LCUI_StyleValue shrink:8;
	LCUI_StyleValue basis:8;
	LCUI_StyleValue direction:8;
	LCUI_StyleValue align_self:8;
	LCUI_StyleValue align_items:8;
	LCUI_StyleValue align_content:8;
	LCUI_StyleValue justify_content:8;
} LCUI_FlexLayoutStyle;

/** 边框相关参数 */
typedef struct LCUI_Border {
	struct {
		int style;
		unsigned int width;
		LCUI_Color color;
	} top, right, bottom, left;
	unsigned int top_left_radius;
	unsigned int top_right_radius;
	unsigned int bottom_left_radius;
	unsigned int bottom_right_radius;
} LCUI_Border;

typedef struct LCUI_Graph_ LCUI_Graph;

typedef struct LCUI_GraphQuote_ {
	int top;		/**< 源图形中的引用区域的上间距 */
	int left;		/**< 源图形中的引用区域的左间距 */
	LCUI_BOOL is_valid;	/**< 标志，指示是否引用了另一图形 */
	LCUI_BOOL is_writable;	/**< 标志，指示源图形是否可写 */
	union {
		LCUI_Graph *source;		/**< 所引用的源图形 */
		const LCUI_Graph *source_ro;	/**< 所引用的源图形(只读) */
	};
} LCUI_GraphQuote;

struct LCUI_Graph_ {
	uint32_t width;			/**< 宽度 */
	uint32_t height;		/**< 高度 */
	LCUI_GraphQuote quote;		/**< 引用记录 */
	/** 像素数据缓存区 */
	union {
		uchar_t *bytes;		/**< 指针，用于一次访问一个字节的数据 */
		LCUI_ARGB *argb;	/**< 指针，用于一次访问一个像素的数据 */
	};
	int color_type;			/**< 色彩类型 */
	uint32_t bytes_per_pixel;	/**< 每个像素共占多少个字节 */
	uint32_t bytes_per_row;		/**< 每行像素共占多少个字节 */
	float opacity;			/**< 全局不透明度，取值范围为 0~1.0 */
	size_t mem_size;		/**< 像素数据缓冲区大小 */
	uchar_t *palette;		/**< 调色板 */
};

typedef struct LCUI_StyleRec_ {
	LCUI_BOOL is_valid:2;
	LCUI_StyleType type:6;
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
		int style;
		int val_style;
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
	LCUI_Graph image;			/**< 背景图 */
	LCUI_Color color;			/**< 背景色 */
	struct {
		LCUI_BOOL x, y;
	} repeat;				/**< 背景图是否重复 */
	LCUI_BackgroundPosition position;	/**< 定位方式 */
	LCUI_BackgroundSize size;		/**< 尺寸 */
} LCUI_BackgroundStyle;

typedef struct LCUI_Background {
	LCUI_Graph *image;	/**< 背景图 */
	LCUI_Color color;	/**< 背景色 */
	struct {
		LCUI_BOOL x, y;
	} repeat;		/**< 背景图是否重复 */
	struct {
		int x, y;
	} position;
	struct {
		int width, height;
	} size;
} LCUI_Background;

/** 进行绘制时所需的上下文 */
typedef struct LCUI_PaintContextRec_ {
	LCUI_Rect rect;			/**< 需要绘制的区域 */
	LCUI_Graph canvas;		/**< 绘制后的位图缓存（可称为：画布） */
	LCUI_BOOL with_alpha;		/**< 绘制时是否需要处理 alpha 通道 */
} LCUI_PaintContextRec, *LCUI_PaintContext;

typedef void (*FuncPtr)(void *);

LCUI_END_HEADER

#include <LCUI/util.h>
#include <LCUI/main.h>

#endif /* LCUI_H */
