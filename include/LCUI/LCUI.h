/* ***************************************************************************
 * LCUI.h -- Records with common data type definitions, macro definitions and
 * function declarations
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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

/* 为了解决结构体被重复定义，用宏进行头文件保护(其它地方出现类似的内容，将不再注释) */
#ifndef __LCUI_H__  /* 如果没有定义 __LCUI_H__ 宏 */
#define __LCUI_H__  /* 定义 __LCUI_H__ 宏 */

#define LCUI_VERSION "1.0.0"

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define ToString(x) #x

LCUI_BEGIN_HEADER

typedef unsigned int uint_t;
typedef unsigned long int LCUI_ID;
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
	int w, h;
} LCUI_Size;

typedef struct LCUI_String_ {
	char   *string;
	uint_t length;
} LCUI_String;

typedef struct LCUI_WString_ {
	wchar_t *string;
	uint_t length;
} LCUI_WString;

typedef struct LCUI_Rect_ {
	union { int x, left; };
	union { int y, top; };
	union { int w, width; };
	union { int h, height; };
} LCUI_Rect;

typedef struct LCUI_Rect2_ {
	union { int left, l; };
	union { int top, t; };
	union { int right, r; };
	union { int bottom, b; };
} LCUI_Rect2;

/** 样式变量类型 */
typedef enum LCUI_StyleVarType {
	SVT_NONE,
	SVT_AUTO,
	SVT_SCALE,		/**< 比例 */
	SVT_PX,			/**< 像素 */
	SVT_PT,			/**< 点 */
	SVT_BOOLEAN,
	SVT_COLOR,		/**< 色彩 */
	SVT_color,
	SVT_image,
	SVT_style
} LCUI_StyleVarType;

#define SVT_px		SVT_PX
#define SVT_pt		SVT_PT
#define SVT_scale	SVT_SCALE
#define SVT_boolean	SVT_BOOLEAN

/** 用于描述样式的变量类型 */
typedef struct LCUI_StyleVar {
	LCUI_StyleVarType type;		/**< 哪一种类型 */
	union {
		int px;			/**< pixel, 像素 */
		int pt;			/**< point，点数（一般用于字体大小单位） */
		double scale;		/**< 比例 */
		LCUI_Color color;	/**< 色彩值 */
	};
} LCUI_StyleVar;

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
	union { int w, width; };	/**< 宽度，一个整数值，单位为像素(px) */
	union { int h, height; };	/**< 高度，一个整数值，单位为像素(px) */
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
	size_t bytes_per_pixel;		/**< 每个像素共占多少个字节 */
	size_t bytes_per_row;		/**< 每行像素共占多少个字节 */
	float opacity;			/**< 全局不透明度，取值范围为 0~1.0 */
	size_t mem_size;		/**< 像素数据缓冲区大小 */
	uchar_t *palette;		/**< 调色板 */
};

/** 样式值枚举，用于代替使用字符串 */
enum StyleValue {
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
	SV_CENTER_LEFT,
	SV_CENTER_CENTER,
	SV_CENTER_RIGHT,
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
	SV_GRAPH_BOX
};

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
				LCUI_StyleVar x, y;
			};
			int value;
		};
	} position;		/**< 定位方式 */
	struct {
		LCUI_BOOL using_value;
		union {
			struct {
				LCUI_StyleVar w, h;
			};
			int value;
		};
	} size;
} LCUI_Background;

/** 进行绘制时所需的上下文 */
typedef struct LCUI_PaintContextRec_ {
	LCUI_Rect rect;			/**< 需要绘制的区域 */
	LCUI_Graph canvas;		/**< 绘制后的位图缓存（可称为：画布） */
} LCUI_PaintContextRec_, *LCUI_PaintContext;

LCUI_END_HEADER

#define nobuff_printf(format, ...) \
	{ \
		printf(format, ##__VA_ARGS__); \
		fflush(stdout); \
	}

#include <LCUI/kernel/main.h>
#include <LCUI/kernel/event.h>
#include <LCUI/kernel/timer.h>
#include <LCUI/kernel/device.h>
#include <LCUI/misc.h>

#if defined (LCUI_BUILD_IN_WIN32) && defined(_WINDOWS) &&defined(I_NEED_WINMAIN)

#include <tchar.h>

#ifdef _UNICODE
#	define _tcstok_s wcstok_s
#	define _tcscpy_s wcscpy_s
#else
#	define _tcstok_s strtok_s
#	define _tcscpy_s strcpy_s
#endif

extern int main( int argc, char *argv[] );

int
#if !defined(_MAC)
#if defined(_M_CEE_PURE)
__clrcall
#else
WINAPI
#endif
#else
CALLBACK
#endif
WinMain (
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
    )
{
	int ret, len, i = 0, argc = 0;
	TCHAR *p_cmd_line, *cmdline_buff;
	TCHAR *token = NULL, *next_token = NULL;
	TCHAR **argv = NULL;

	Win32_LCUI_Init( hInstance );
	/* 获取命令行 */
	p_cmd_line = GetCommandLine();
	/* 计算命令行的长度 */
	len = _tcslen( p_cmd_line ) + 1;
	/* 分配一段相应长度的内存空间 */
	cmdline_buff = (TCHAR*)malloc( sizeof(TCHAR)*len );
	if( cmdline_buff == NULL ) {
		return -1;
	}
	/* 拷贝该命令行 */
	_tcscpy_s( cmdline_buff, len, p_cmd_line );
	/* 计算命令行中参数的个数 */
	token = _tcstok_s( cmdline_buff, _T(" \r\t\n"), &next_token );
	while( token != NULL ) {
		argc++;
		token = _tcstok_s( NULL, _T(" \r\t\n"), &next_token );
	}
	/* 根据参数个数来分配内存空间 */
	if( argc > 0 ) {
		argv = (TCHAR**)malloc(sizeof(TCHAR*)*argc);
	}
	if( argv == NULL ) {
		return -1;
	}
	/* 由于strtok_s函数会修改cmdline_buff里的内容，因此，需要重新拷贝一次 */
	_tcscpy_s( cmdline_buff, len, p_cmd_line );
	token = _tcstok_s( cmdline_buff, _T(" \r\t\n"), &next_token );
	while( token && i<argc ) {
		len = _tcslen( token ) + 1;
		argv[i] = (TCHAR*)malloc(sizeof(TCHAR)*len);
		if( argv[i] == NULL ) {
			return -1;
		}
		_tcscpy_s( argv[i], len, token );
		token = _tcstok_s( NULL, _T(" \r\t\n"), &next_token );
		++i;
	}
	/* 调用main函数 */
	ret = main( argc, (char**)argv );
	/* 释放之前申请的内存空间 */
	for(i=0; i<argc; ++i) {
		free( argv[i] );
	}
	free( argv );
	free( cmdline_buff );
	return ret;
}
#endif

#endif /* __LCUI_H__ */
