/* ***************************************************************************
 * LCUI.h -- Records with common data type definitions, macro definitions and
 * function declarations
 *
 * Copyright (C) 2012-2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * LCUI.h -- 记录着常用的数据类型定义，宏定义，以及函数声明
 *
 * 版权所有 (C) 2012-2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

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

typedef struct LCUI_BoxShadow {
	int x, y;		/**< 位置 */
	int blur;		/**< 模糊距离 */
	int spread;		/**< 扩散大小 */
	LCUI_Color color;	/**<　颜色　*/
} LCUI_BoxShadow;

/* 完整的边框信息 */
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
struct LCUI_Graph_ {
	uint32_t width;			/**< 宽度，一个整数值，单位为像素(px) */
	uint32_t height;			/**< 高度，一个整数值，单位为像素(px) */
	struct {
		int top;		/**< 源图形中的引用区域的上边距 */
		int left;		/**< 源图形中的引用区域的左边距 */
		LCUI_BOOL is_valid;	/**< 标志，指示是否引用了另一图形 */
		LCUI_Graph *source;	/**< 所引用的源图形 */
	} quote;
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
	SV_FLOAT_LEFT,
	SV_FLOAT_RIGHT,
	SV_BLOCK,
	SV_INLINE_BLOCK,
	SV_NOWRAP
} LCUI_StyleValue;

typedef struct LCUI_StyleRec_ {
	LCUI_BOOL is_valid:2;
	unsigned short int type;
	union {
		int value;
		int val_int;
		int val_0;
		int val_none;
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

typedef struct LCUI_Background {
	LCUI_Graph image;	/**< 背景图 */
	LCUI_Color color;	/**< 背景色 */
	int clip;		/**< 背景图的裁剪方式 */
	int origin;		/**< 相对于何种位置进行定位 */

	struct {
		LCUI_BOOL x, y;
	} repeat;		/**< 背景图是否重复 */
	struct {
		LCUI_BOOL using_value;
		union {
			struct {
				LCUI_StyleRec x, y;
			};
			int value;
		};
	} position;		/**< 定位方式 */
	struct {
		LCUI_BOOL using_value;
		union {
			struct {
				LCUI_StyleRec w, h;
			};
			int value;
		};
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
