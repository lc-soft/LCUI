/* ***************************************************************************
 * LCUI.h -- Records with common data type definitions, macro definitions and
 * function declarations
 *
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * 版权所有 (C) 2012-2013 归属于
 * 刘超
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

#define LCUI_VERSION "0.15.0"

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

/* 状态 */
#define ACTIVE	1
#define KILLED	-1
#define REMOVE	-1
#define INSIDE	1

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* 触屏校准后的文件 */
#define LCUI_CALIBFILE "/mnt/Data/LC-SOFT/pointercal"

/************ 任务的添加模式 ***************/
#define ADD_MODE_ADD_NEW	0 /* 新增 */
#define ADD_MODE_NOT_REPEAT	1 /* 不能重复 */
#define ADD_MODE_REPLACE	2 /* 覆盖 */

#define AND_ARG_F	1<<3	/* 第一个参数 */
#define AND_ARG_S 	1<<4	/* 第二个参数 */
/*****************************************/

LCUI_BEGIN_HEADER

typedef unsigned char LCUI_BOOL;

typedef unsigned long int LCUI_ID;

typedef void (*CallBackFunc)(void*,void*);

typedef struct	LCUI_Widget_	LCUI_Widget;

typedef unsigned char uchar_t;
typedef unsigned int uint_t;

#ifdef LCUI_BUILD_IN_WIN32
typedef __int64 int64_t;
#else
typedef long long int64_t;
#endif


/********** 按键信息 ************/
typedef struct LCUI_Key_ {
	int code;
	int state;
} LCUI_Key;
/******************************/

/*--------- RGB配色数据 ---------*/
typedef struct LCUI_RGB_ {
	uchar_t red;
	uchar_t green;
	uchar_t blue;
} LCUI_RGB;
/*----------- END -------------*/

/*--------- RGBA配色数据 --------*/
typedef struct LCUI_RGBA_ {
	uchar_t red;
	uchar_t green;
	uchar_t blue;
	uchar_t alpha;
} LCUI_RGBA;
/*----------- END -------------*/

/*------- 二维坐标 --------*/
typedef struct LCUI_Pos_ {
	int x, y;
} LCUI_Pos;
/*--------- END ----------*/

/*------- 尺寸 --------*/
typedef struct LCUI_Size_ {
	int w, h;
} LCUI_Size;
/*------- END --------*/

/*------------- 像素点信息 ------------*/
typedef struct Pixel_ {
	LCUI_Pos pos;	/* 位置 */
	LCUI_RGB rgb;	/* RGBA值 */
} Pixel;
/*--------------- END ---------------*/

/*---------------- 字符串 ----------------*/
typedef struct LCUI_String_ {
	char   *string; /* 字符串内容 */
	uint_t length;	/* 长度 */
} LCUI_String;
/*----------------- END -----------------*/

/*------------------- 区域数据 ---------------------*/
typedef struct LCUI_Rect_ {
	int x,y;
	int width,height;
	double center_x,center_y; /* 中心点的坐标 */
} LCUI_Rect;
/*--------------------- END ----------------------*/

/*---------- 宽字符串 ----------*/
typedef struct LCUI_WString_ {
	wchar_t *string;
	uint_t length;
} LCUI_WString;
/*------------ END ------------*/

LCUI_END_HEADER
	
#include LC_THREAD_H
#include LC_QUEUE_H

LCUI_BEGIN_HEADER

enum GraphColorTyle {
	COLOR_TYPE_RGB,
	COLOR_TYPE_RGBA
};

/*---------------------------- 图形数据 -------------------------------*/
typedef struct LCUI_Graph_ LCUI_Graph;
struct LCUI_Graph_ {
	int x, y;		/**< 源图形中的引用区域所在的坐标 */
	int w, h;		/**< 图形的尺寸 */
	
	int color_type;		/**< 色彩类型 */
	LCUI_BOOL quote;	/**< 标志，指示是否引用了另一图形 */
	LCUI_Graph *src;	/**< 所引用的源图形 */
	uchar_t	alpha;		/**< 全局透明度 */
	uchar_t** rgba;		/**< 像素数据缓冲区 */
	size_t mem_size;	/**< 像素数据缓冲区大小 */

	LCUI_Mutex mutex;	/**< 互斥锁，用于数据保护 */
};
/*------------------------------ END ---------------------------------*/

/*---------------- 用于表示像素或百分比 -----------------*/
typedef struct PX_P_t_ {
	LCUI_BOOL which_one;	/** 指定用哪个类型的变量 */
	int px;			/** 数值，单位为像素 */
	double scale;		/** 比例 */
} IntOrFloat_t;
/*---------------------- END -------------------------*/

/*----------------- 用于表示字体大小 --------------------*/
typedef struct PX_PT_t_ {
	LCUI_BOOL which_one;	/** 指定用哪个类型的变量 */
	int px;			/** pixel, 字体大小（像素） */
	int pt;			/** point，字体大小（点数） */
} PX_PT_t;
/*---------------------- END -------------------------*/

/*------------------- 内边距和外边距 --------------------*/
typedef struct common_box_ {
	int top, bottom, left, right;
} LCUI_Margin, LCUI_Padding;
/*---------------------- END -------------------------*/

/*----------------- 自动尺寸调整模式 --------------------*/
typedef enum AUTOSIZE_MODE_ {
	AUTOSIZE_MODE_GROW_AND_SHRINK,	/** 增大和缩小 */
	AUTOSIZE_MODE_GROW_ONLY		/** 只增大 */
}
AUTOSIZE_MODE;
/*---------------------- END --------------------------*/

LCUI_END_HEADER

#define MAX_APP_IDLE_TIME	50
#define MAX_LCUI_IDLE_TIME	50

#define nobuff_printf(format, ...) \
	{ \
		printf(format, ##__VA_ARGS__); \
		fflush(stdout); \
	}

#include LC_MISC_H
#include LC_KERNEL_SLEEPER_H
#include LC_KERNEL_MAIN_H
#include LC_KERNEL_DEV_H
#include LC_KERNEL_TIMER_H
#include LC_KERNEL_TASK_H
#include LC_KERNEL_EVENT_H

#if defined (LCUI_BUILD_IN_WIN32) && defined(_WINDOWS) &&defined(I_NEED_WINMAIN)

#ifdef _UNICODE
	#define _tcstok_s wcstok_s
	#define _tcscpy_s wcscpy_s
#else
	#define _tcstok_s strtok_s
	#define _tcscpy_s strcpy_s
#endif

/* 若是UNICODE字符，请手动将参数argv转换由char为wchar_t类型 */
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
	TCHAR *pCmdLine, *cmdline_buff;
	TCHAR *token = NULL, *next_token = NULL;
	TCHAR **argv = NULL;
	
	Win32_LCUI_Init( hInstance );
	/* 获取命令行 */
	pCmdLine = GetCommandLine();
	/* 计算命令行的长度 */
	len = _tcslen( pCmdLine ) + 1;
	/* 分配一段相应长度的内存空间 */
	cmdline_buff = (TCHAR*)malloc( sizeof(TCHAR)*len );
	if( cmdline_buff == NULL ) {
		return -1;
	}
	/* 拷贝该命令行 */
	_tcscpy_s( cmdline_buff, len, pCmdLine );
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
	_tcscpy_s( cmdline_buff, len, pCmdLine );
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


