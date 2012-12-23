/* ***************************************************************************
 * LCUI.h -- Records with common data type definitions, macro definitions and 
 * function declarations
 * 
 * Copyright (C) 2012 by
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
 * 版权所有 (C) 2012 归属于 
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

#define LCUI_VERSION "0.13.0"

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* 打开文件时的错误 */
#define SHORT_FILE	1
#define BIG_FILE	2
#define UNKNOWN_FORMAT	3
#define OPEN_ERROR	4

/* 图像类型 */
#define TYPE_PNG	1
#define TYPE_JPG	2
#define TYPE_BMP	3

#define PNG_BYTES_TO_CHECK 4

/* 状态 */ 
#define ACTIVE	1
#define KILLED	-1
#define REMOVE	-1
#define INSIDE	1

#define NO_ALPHA	0	 /* 无alpha通道 */
#define HAVE_ALPHA	1	 /* 有alpha通道 */

#define YES	1 /* 是 */
#define NO	0 /* 否 */

#define NONE	0 /* 无 */

#define VIEW	0 /* 显示 */
#define HIDE	1 /* 隐藏  */ 

#define DEFAULT	0 /* 缺省 */
#define CUSTOM	1 /* 自定义 */

/***************** 一些输入输出设备 *********************/
#define FB_DEV	"/dev/fb0"		/* 图形输出设备 */
#define TS_DEV	"/dev/jz_ts"		/* 触屏输入设备 */
#define MS_DEV	"/dev/input/mice"	/* 鼠标设备 */
/*****************************************************/

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef enum _BOOL
{
	IS_FALSE = 0,
	IS_TRUE = 1
}
BOOL;

#define FORCE	2

/* 触屏校准后的文件 */
#define LCUI_CALIBFILE "/mnt/Data/LC-SOFT/pointercal"

/* 定义了图形的混合方式 */
#define GRAPH_MIX_FLAG_OVERLAY	 0
#define GRAPH_MIX_FLAG_REPLACE	 1
/* 默认的字体文件路径 */
#define LCUI_DEFAULT_FONTFILE	"../fonts/msyh.ttf"

/* 鼠标事件 */
#define MOUSE_EVENT_MOVE	1
#define MOUSE_EVENT_CLICK	2

#define LABEL_TEXT_MAX_SIZE	2048

/****************** 图像的布局 *****************/
typedef enum _LAYOUT_TYPE
{
	LAYOUT_ZOOM	= 1,	  /* 缩放 */
	LAYOUT_STRETCH	= 2,	  /* 拉伸 */
	LAYOUT_CENTER	= 3,	  /* 居中 */
	LAYOUT_TILE	= 4,	  /* 平铺 */
	LAYOUT_NONE	= 0,	  /* 无 */
	LAYOUT_NORMAL	= 0 
}LAYOUT_TYPE;
/**********************************************/

/************ 任务的添加模式 ***************/
#define ADD_MODE_ADD_NEW	0 /* 新增 */
#define ADD_MODE_NOT_REPEAT	1 /* 不能重复 */
#define ADD_MODE_REPLACE	2 /* 覆盖 */

#define AND_ARG_F	1<<3	/* 第一个参数 */
#define AND_ARG_S 	1<<4	/* 第二个参数 */ 
/*****************************************/

#define LCUI_Menu_Style_Default	20
#define LCUI_Style_Default	1
#define LCUI_Style_Standard	2 /* LCUI标准风格 */
#define Custom_Style		5
#define NONE			0

/* 部件的位置类型 */
#define POS_TYPE_IN_WIDGET	1
#define POS_TYPE_IN_SCREEN	0

#define ICON_CUSTOM 2
#define ICON_LCUI   1
#define ICON_None   0


/****************** 图像的处理方式 *****************/
#define FILL_MODE_ZOOM		LAYOUT_ZOOM	/* 缩放 */
#define FILL_MODE_STRETCH  	LAYOUT_STRETCH	/* 拉伸 */
#define FILL_MODE_CENTER	LAYOUT_CENTER	/* 居中 */
#define FILL_MODE_TILE		LAYOUT_TILE	/* 平铺 */
#define FILL_MODE_NONE		LAYOUT_NONE	/* 无 */
#define FILL_MODE_NORMAL	LAYOUT_NORMAL 
/*************************************************/

/* 消息盒子里显示的图标 */
#define ICON_Warning	1
#define ICON_Error	2
#define ICON_Question	3
#define ICON_Asterisk	4
/* 消息盒子使用的按键方案 */
#define MB_None			0
#define MB_Abort_Retry_Ignore	1
#define MB_OK			2
#define MB_OK_Cancel		3
#define MB_Retry_Cancel		4
#define MB_Yes_No		5
#define MB_Yes_No_Cancel	6

/*************** 边框风格 *****************/
typedef enum _BORDER_STYLE
{
	BORDER_STYLE_NONE,	/* 无边框 */
	BORDER_STYLE_SOLID,	/* 实线 */
	BORDER_STYLE_DOTTED,	/* 点状 */
	BORDER_STYLE_DOUBLE,	/* 双线 */
	BORDER_STYLE_DASHED,	/* 虚线 */
}
BORDER_STYLE;
/*****************************************/

/************ 部件的背景模式 ***************/
typedef enum _BG_MODE
{
	BG_MODE_TRANSPARENT,
	BG_MODE_FILL_BACKCOLOR
}
BG_MODE;
/*****************************************/

/*----------------- 部件对齐方式 -------------------*/
typedef enum _ALIGN_TYPE
{
	ALIGN_NONE,		/* 无 */
	ALIGN_TOP_LEFT,	  	/* 向左上角对齐 */
	ALIGN_TOP_CENTER,	/* 向上中间对齐 */
	ALIGN_TOP_RIGHT,	/* 向右上角对齐 */
	ALIGN_MIDDLE_LEFT,	/* 向中央偏左对齐 */
	ALIGN_MIDDLE_CENTER,	/* 向正中央对齐 */
	ALIGN_MIDDLE_RIGHT,	/* 向中央偏由对齐 */
	ALIGN_BOTTOM_LEFT,	/* 向底部偏左对齐 */
	ALIGN_BOTTOM_CENTER,	/* 向底部居中对齐 */
	ALIGN_BOTTOM_RIGHT	/* 向底部偏右对齐 */
}
ALIGN_TYPE;
/*-------------------------------------------------*/

/*----------------- 部件的几种状态 ------------------*/
typedef enum _WIDGET_STATUS
{
	WIDGET_STATUS_NORMAL,	/* 普通状态 */
	WIDGET_STATUS_CLICKED,	/* 被鼠标点击，已松开 */
	WIDGET_STATUS_OVERLAY,	/* 被鼠标覆盖 */
	WIDGET_STATUS_CLICKING,	/* 被鼠标点击，未松开 */
	WIDGET_STATUS_FOCUS,	/* 焦点 */
	WIDGET_STATUS_DISABLE	/* 不可用 */
}
WIDGET_STATUS;
/*-------------------------------------------------*/

/****************** 图像的处理方式 *****************/
/* 缩放，缩放比例随着PictureBox部件的尺寸的改变而改变 */
#define SIZE_MODE_ZOOM		LAYOUT_ZOOM	

/* 固定缩放，用于照片查看器，PictureBox部件的尺寸的改变不影响缩放比列 */
#define SIZE_MODE_BLOCK_ZOOM	5

#define SIZE_MODE_STRETCH	LAYOUT_STRETCH	/* 拉伸 */
#define SIZE_MODE_CENTER	LAYOUT_CENTER	/* 居中 */
#define SIZE_MODE_TILE		LAYOUT_TILE	/* 平铺 */
#define SIZE_MODE_NONE		LAYOUT_NONE	/* 无 */
#define SIZE_MODE_NORMAL	LAYOUT_NORMAL 
/*************************************************/


/*********** 图像与文本的几种相对位置 *******************/
#define Overlay		10   /* 文字覆盖在图片上面 */
#define ImageAboveText	11   /* 图片在上，文字在下 */
#define TextAboveImage	12   /* 图片在下，文字在上 */
#define ImageBeforeText	13   /* 图片在左，文字在右 */
#define TextBeforeImage	14   /* 图片在右，文字在左 */
/***************************************************/

typedef unsigned long int LCUI_ID; 
/* 先使用typedef为结构体创建同义字，之后再定义结构体 */
typedef struct	_LCUI_System		LCUI_System;
typedef struct	_LCUI_Cursor		LCUI_Cursor;
typedef struct	_LCUI_App		LCUI_App;
typedef struct	_LCUI_WString		LCUI_WString;
typedef struct	_LCUI_WChar_T		LCUI_WChar_T;
typedef struct	_LCUI_Rect		LCUI_Rect; 
typedef struct	_LCUI_String		LCUI_String;
typedef struct	_LCUI_Widget		LCUI_Widget;  
typedef struct	_LCUI_RGB		LCUI_RGB;
typedef struct	_LCUI_RGBA		LCUI_RGBA;
typedef struct	_LCUI_FontBMP		LCUI_FontBMP;
typedef struct	_LCUI_Graph		LCUI_Graph;
typedef struct	_LCUI_Font		LCUI_Font;
typedef struct	_LCUI_MouseEvent	LCUI_MouseEvent;
typedef struct	_LCUI_Key		LCUI_Key;
typedef struct	_LCUI_Pos		LCUI_Pos;
typedef struct	_LCUI_Size		LCUI_Size;
typedef struct	_Thread_Queue		Thread_Queue;
typedef struct	_Thread_TreeNode	Thread_TreeNode;
typedef struct	_LCUI_Screen		LCUI_Screen;
typedef struct	_Pixel			Pixel;

typedef unsigned char uchar_t;
typedef unsigned int uint_t;

/********** 按键信息 ************/
struct _LCUI_Key
{
	int code;
	int status;
};
/******************************/

/************ 保存RGB配色数据 *************/
struct _LCUI_RGB
{
	uchar_t red;
	uchar_t green;
	uchar_t blue;
}; 
/****************************************/

/************ 保存RGBA配色数据 *************/
struct _LCUI_RGBA
{
	uchar_t red;
	uchar_t green;
	uchar_t blue;
	uchar_t alpha;
}; 
/****************************************/

/*********** 坐标 ************/
struct _LCUI_Pos
{
	int x, y;
};
/*****************************/

/************* 尺寸 *************/
struct _LCUI_Size
{
	int w, h;
};
/*******************************/

/*************** 记录像素点信息 *******************/
struct _Pixel
{
	LCUI_Pos pos;	/* 位置 */ 
	LCUI_RGB rgb;	/* RGBA值 */
};
/***********************************************/

/*********** 字符串 ***********/
struct _LCUI_String
{
	char   *string; /* 字符串内容 */
	size_t size;	/* 占用的空间大小 */
};
/*****************************/

/****************** 区域数据1 **********************/
struct _LCUI_Rect /* 可记录需刷新的区域 */
{
	int x,y;
	int width,height;
	double center_x,center_y; /* 中心点的坐标 */
};
/**************************************************/

/********** 保存字体位图数据 ***************/
struct _LCUI_FontBMP
{
	int top;		/* 与顶边框的距离 */
	int left;		/* 与左边框的距离 */
	int width;		/* 位图宽度 */
	int rows;		/* 位图行数 */
	int pitch;
	uchar_t *buffer;	/* 字体位图数据 */
	short num_grays;
	char pixel_mode;
	LCUI_Pos advance;	/* XY轴的跨距 */
};
/*****************************************/

/************************wchar_t型字符***************************/
struct _LCUI_WChar_T
{
	wchar_t		char_code;	/* 字符码 */
	LCUI_FontBMP	bitmap;		/* 字符的位图数据 */
	LCUI_RGB	color;		/* 该文字的配色 */
	BOOL		update;		/* 标明这个字符是否需要刷新 */ 
	int		color_type;	/* 颜色类型(DEFAULT / CUSTOM) */		   
};
/****************************************************************/

/************** wchar_t型字符串 ***********/
struct _LCUI_WString
{
	LCUI_WChar_T *string;
	int	update;
	size_t size;
};
/*****************************************/

#include LC_QUEUE_H
#include LC_WORK_H
#include LC_THREAD_H

/***************************** 图层数据 ********************************/
struct _LCUI_Graph
{
	int	type;		/* 图片类型 */
	int	bit_depth;	/* 位深 */
	
	thread_rwlock	lock;	/* 锁，用于数据保护 */
	
	BOOL quote;		/* 指示是否引用其它图层中的图形 */
	LCUI_Graph *src;	/* 所引用的对象 */
	LCUI_Pos pos;		/* 在引用另一个图层中的图形时，会保存区域的起点位置 */
	int width, height;	/* 尺寸 */
	
	uchar_t**	rgba;	/* 图片数组 */
	uchar_t	alpha;	/* 全局透明度，表示整张图片的透明度，默认为255 */
	
	BOOL have_alpha;	/* 标志，指定是否需要透明度，分配内存时会根据它分配 */
	BOOL is_opaque;		/* 标志，指定该图形是否为不透明 */
	BOOL not_visible;	/* 标志，指定该图形是否不可见，也就是全透明 */
};
/**********************************************************************/

/******************************保存字体信息********************************/
struct _LCUI_Font/* 字体信息数据 */
{
	int		type;		/* 类型(DEFAULT / CUSTOM) */ 
	LCUI_String	font_file;	/* 字体文件的路径 */
	LCUI_String	family_name;	/* 字体名称 */
	LCUI_String	style_name;	/* 字体风格名称 */ 
	void*		ft_lib;		/* FreeType2库的句柄  */
	void*		ft_face;	/* FreeType2的face对象的句柄 */
	int		load_flags;	/* 字形载入标志 */
	int		render_mode;	/* 字形转换模式标志 */
	int		status;		/* 状态，是否打开了字体库 */
};
/************************************************************************/


/************************ 鼠标事件 **************************/
struct _LCUI_MouseEvent
{
	LCUI_Widget	*widget;	/* 当前鼠标指针覆盖到的部件 */
	LCUI_Key	key;		/* 按键键值以及状态 */
	LCUI_Pos	global_pos;	/* 鼠标指针的全局坐标 */
	LCUI_Pos	pos;		/* 与当前覆盖到的部件的相对坐标 */
};
/***********************************************************/

/* 完整的边框信息 */
typedef struct _LCUI_Border
{
	int top_width;
	int bottom_width;
	int left_width;
	int right_width;
	BORDER_STYLE top_style;
	BORDER_STYLE bottom_style;
	BORDER_STYLE left_style;
	BORDER_STYLE right_style;
	LCUI_RGB top_color;
	LCUI_RGB bottom_color;
	LCUI_RGB left_color;
	LCUI_RGB right_color;
	int top_left_radius;
	int top_right_radius;
	int bottom_left_radius;
	int bottom_right_radius;
}
LCUI_Border;

/***************************** 定位类型 ********************************/
typedef enum _POS_TYPE
{
	POS_TYPE_STATIC,
	POS_TYPE_RELATIVE,
	POS_TYPE_ABSOLUTE,
	POS_TYPE_FIXED,
}
POS_TYPE;
/*
 * absolute:
 * 	绝对定位，相对于 static 定位以外的第一个父元素进行定位。
 * 	元素的位置通过 "left", "top", "right" 以及 "bottom" 属性进行规定。
 * fixed：
 * 	绝对定位，相对于屏幕进行定位。
 * 	元素的位置通过 "left", "top", "right" 以及 "bottom" 属性进行规定。 
 * static：
 * 	默认值。没有定位，忽略 top, bottom, left, right 或者 z-index。 
 */
/**********************************************************************/

/*------------ 部件停靠类型 --------------*/
typedef enum _DOCK_TYPE
{
	DOCK_TYPE_NONE,
	DOCK_TYPE_TOP,
	DOCK_TYPE_LEFT,
	DOCK_TYPE_RIGHT,
	DOCK_TYPE_FILL,
	DOCK_TYPE_BOTTOM
}
DOCK_TYPE;
/*---------------- END -----------------*/

/*---------------- 用于表示像素或百分比 -----------------*/
typedef struct _PX_P_t
{
	BOOL which_one;		/* 指定用哪个类型的变量 */
	int px;			/* 数值，单位为像素 */
	double scale;		/* 比例 */
}
PX_P_t;
/*---------------------- END -------------------------*/

/*----------------- 用于表示字体大小 --------------------*/
typedef struct _PX_PT_t
{
	BOOL which_one;		/* 指定用哪个类型的变量 */
	int px;			/* pixel, 字体大小（像素） */
	int pt;			/* point，字体大小（点数） */
}
PX_PT_t;
/*---------------------- END -------------------------*/

/*------------------- 内边距和外边距 --------------------*/
typedef struct _LCUI_Margin
{
	int top, bottom, left, right;
}
LCUI_Margin, LCUI_Padding;
/*---------------------- END -------------------------*/

/*----------------- 自动尺寸调整模式 --------------------*/
typedef enum _AUTOSIZE_MODE
{
	AUTOSIZE_MODE_GROW_AND_SHRINK,	/* 增大和缩小 */
	AUTOSIZE_MODE_GROW_ONLY		/* 只增大 */
}
AUTOSIZE_MODE;
/*----------------------------------------------------*/

/******************************* 部件 **********************************/
struct _LCUI_Widget 
{
	LCUI_ID app_id; /* 所属程序的ID */
	
	LCUI_Pos pos;	/* 已计算出的实际位置 */
	LCUI_Pos max_pos;
	LCUI_Pos min_pos;
	/*------------ 位置限制（描述） ---------------*/ 
	PX_P_t x, y;
	PX_P_t max_x, min_x;
	PX_P_t max_y, min_y;
	/*------------------ END -------------------*/
	
	LCUI_Size size; /* 已计算出的实际尺寸，单位为像素 */
	LCUI_Size max_size;
	LCUI_Size min_size;
	
	/*------------ 尺寸限制（描述） ---------------*/ 
	PX_P_t w, h;
	PX_P_t max_w, min_w;
	PX_P_t max_h, min_h;
	/*------------------ END -------------------*/
	
	WIDGET_STATUS status;	/* 部件的状态 */
	BOOL status_response;	/* 是否响应部件的状态改变 */
	
	BOOL enabled;	/* 是否启用 */
	BOOL visible;	/* 是否可见 */
	BOOL inherit_alpha; /* 是否继承父部件的透明度 */
	
	BOOL		auto_size;	/* 指定是否自动调整自身的大小，以适应内容的大小 */
	AUTOSIZE_MODE	auto_size_mode;	/* 自动尺寸调整模式 */
	
	BOOL		focus;		/* 指定该部件是否需要焦点 */
	LCUI_Widget*	focus_widget;	/* 获得焦点的子部件 */
	
	int z_index;	/* 堆叠顺序 */
	
	int		clickable_mode;		/* 确定在对比像素alpha值时，是要“小于”还是“不小于”才使条件成立 */
	uchar_t	clickable_area_alpha;	/* 指定部件图层中的区域的alpha值小于/不小于多少时可被鼠标点击，默认为0，最大为255 */
	
	LCUI_String	type;		/* 部件的类型 */
	LCUI_ID	type_id;	/* 部件的类型ID */
	LCUI_String	style;		/* 部件的风格，对某些部件有效 */
	LCUI_ID	style_id;	/* 部件的风格的ID */
	LCUI_Widget	*parent;	/* 父部件 */
	LCUI_Queue	child;		/* 子部件集 */
	
	/*----------------- 部件布局相关 ----------------*/
	POS_TYPE	pos_type;	/* 位置类型 */
	ALIGN_TYPE	align;		/* 布局 */
	LCUI_Pos	offset;		/* x，y轴的偏移量 */
	DOCK_TYPE	dock;		/* 停靠位置 */
	/*------------------ END ----------------------*/
	
	/*------------ 外边距和内边距 ---------------*/ 
	LCUI_Margin	margin;
	LCUI_Padding	padding;
	/*---------------- END -------------------*/
	
	LCUI_Border	border;		/* 边框 */
	
	LCUI_RGB  back_color;  /* 背景色 */
	LCUI_RGB  fore_color;  /* 前景色 */
	
	int		bg_mode;  /* 背景模式，指定在无背景时是使用透明背景还是使用背景色填充 */
	int		background_image_layout; /* 背景图的布局 */
	LCUI_Graph	background_image;	 /* 背景图 */
	
	void *private_data;   /* 该部件私有数据的指针，其它的是各个部件公用的数据 */ 
	
	LCUI_Queue	event;		/* 保存部件的事件关联的数据 */
	LCUI_Queue	update_area;	/* 部件内需要刷新的区域 */
	LCUI_Queue	data;		/* 记录需要进行更新的数据 */ 
	LCUI_Graph	graph;		/* 部件的图层 */ 
	
	/* 以下是函数指针，闲函数名太长的话，可以直接用下面的 */
	void (*resize)(LCUI_Widget*, LCUI_Size);
	void (*move)(LCUI_Widget*, LCUI_Pos); 
	void (*show)(LCUI_Widget*);
	void (*hide)(LCUI_Widget*);
	void (*disable)(LCUI_Widget*);
	void (*enable)(LCUI_Widget*);
	void (*set_align)(LCUI_Widget*, ALIGN_TYPE, LCUI_Pos); 
	void (*set_alpha)(LCUI_Widget*, unsigned char); 
	void (*set_border)(LCUI_Widget*, LCUI_Border);
};
/**********************************************************************/



/************************ LCUI程序的数据 *******************************/
struct _LCUI_App 
{
	BOOL		stop_loop;	/* 一个标志，用于指示是否停止主循环 */
	LCUI_ID	id;		/* LCUI程序的ID，如果是以线程方式运行的话，这个就是线程ID */ 
	
	LCUI_Queue	task_queue;	/* 程序的任务队列 */
	LCUI_Queue	widget_lib;	/* 部件类型库 */
	
	int encoding_type;	/* 字符编码类型，默认为UTF-8 */
};
/**********************************************************************/


/***********  储存游标的位置 **************/
struct _LCUI_Cursor
{
	LCUI_Pos	pos;	/* 坐标 */
	int		visible;/* 是否可见 */
	LCUI_Graph	graph;	/* 游标的图形 */
};
/****************************************/

#define MAX_APP_IDLE_TIME	50000
#define MAX_LCUI_IDLE_TIME	50000

/************ 线程队列 **************/
struct _Thread_Queue
{
	Thread_TreeNode **queue;	/* 储存队列成员 */
	int max_num;			/* 最大成员数量 */
	int total_num;			/* 当前成员总数 */
};
/***********************************/

/****************************** 屏幕信息 ********************************/
struct _LCUI_Screen
{
	LCUI_Size	size;		/* 屏幕尺寸 */
	LCUI_Graph	buff;		/* 保存启动LCUI前的屏幕内容 */
	char		*fb_dev_name;	/* 图形输出设备的名称 */
	uchar_t	*fb_mem;	/* 指向图像输出设备映射到的内存的指针 */
	int		fb_dev_fd;	/* 图形显示设备的句柄 */
	size_t		smem_len;	/* 内存空间的大小 */
	int		bits;		/* 每个像素的用多少位表示 */
};
/***********************************************************************/

/************ 线程树的结点 ***************/
struct _Thread_TreeNode
{
	Thread_TreeNode *parent;	/* 父线程结点指针 */
	thread_t tid;			/* 父线程ID */
	Thread_Queue child;		/* 子线程队列 */
};
/***************************************/

/***************************整个LCUI的数据 *****************************/
struct _LCUI_System
{	
	int status;		/* 状态 */ 
	BOOL init;		/* 指示LCUI是否初始化过 */
	BOOL need_shift_area;	/* 指示是否需要转移部件中记录的区域数据 */ 
	
	thread_t self_id;		/* 保存LCUI主程序的线程的ID */
	thread_t display_thread;	/* 保存核心处理的线程的ID */
	thread_t timer_thread;		/* 定时器列表处理线程的ID */
	thread_t dev_thread;		/* 设备输入数据处理线程的ID */
	Thread_TreeNode thread_tree; /* 线程关系树 */
	
	LCUI_ID max_app_idle_time;	/* 程序最大的空闲时间 */
	LCUI_ID max_lcui_idle_time;	/* LCUI最大的空闲时间 */
	
	LCUI_Cursor	cursor;		/* 游标 */
	
	LCUI_Screen	screen;		/* 屏幕信息 */
	LCUI_Queue	mouse_event;	/* 鼠标事件 */
	LCUI_Queue	press_key;	/* 保存已被按下的按键的键值 */
	LCUI_Queue	key_event;	/* 保存与按键事件关联的数据 */
	LCUI_Queue	dev_list;	/* 设备列表 */
	LCUI_Queue	widget_list;	/* 部件队列，对应它的显示顺序 */
	LCUI_Widget	*focus_widget;	/* 获得焦点的部件 */
	LCUI_Queue	update_area;	/* 需要刷新的区域 */
	LCUI_Queue	timer_list;	/* 定时器列表 */
	LCUI_Queue	app_list;	/* LCUI程序列表 */ 
	LCUI_Font	default_font;	/* 默认的字体数据 */ 
};
/***********************************************************************/

#define nobuff_printf(format, ...) \
	{ \
		printf(format, ##__VA_ARGS__); \
		fflush(stdout); \
	}

extern LCUI_System  LCUI_Sys;

LCUI_BEGIN_HEADER

/* 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
int LCUI_Dev_Add(	BOOL (*init_func)(), 
			BOOL (*proc_func)(), 
			BOOL (*destroy_func)() );

/*----------------------------- Timer --------------------------------*/
/* 
 * 功能：设置定时器，在指定的时间后调用指定回调函数 
 * 说明：时间单位为毫秒，调用后会返回该定时器的标识符; 
 * 如果要用于循环定时处理某些任务，可将 reuse 置为 1，否则置于 0。
 * */
int set_timer( long int n_ms, void (*callback_func)(void), BOOL reuse );

/*
 * 功能：释放定时器
 * 说明：当不需要定时器时，可以使用该函数释放定时器占用的资源
 * 返回值：正常返回0，指定ID的定时器不存在则返回-1.
 * */
int free_timer( int timer_id );

/*
 * 功能：暂停定时器的使用 
 * 说明：一般用于往复定时的定时器
 * */
int pause_timer( int timer_id );

int continue_timer( int timer_id );
/* 继续使用定时器 */

/* 重设定时器的时间 */
int reset_timer( int timer_id, long int n_ms );
/*---------------------------- End Timer -----------------------------*/

/************************* App Management *****************************/
LCUI_App *Find_App(LCUI_ID id);
/* 功能：根据程序的ID，获取指向程序数据结构的指针 */ 

LCUI_App* Get_Self_AppPointer();
/* 功能：获取程序的指针 */ 

void Main_Loop_Quit();
/* 功能：让程序退出主循环 */
/*********************** App Management End ***************************/

BOOL LCUI_Active();
/* 功能：检测LCUI是否活动 */ 

int LCUI_Init(int argc, char *argv[]);
/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */ 

int Need_Main_Loop(LCUI_App *app);
/* 功能：检测主循环是否需要继续进行 */ 

int LCUI_Main ();
/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */ 

int Get_LCUI_Version(char *out);
/* 功能：获取LCUI的版本 */ 

int Get_EncodingType();
/* 获取字符编码类型 */

int Using_GB2312();
/* 
 * 说明：如果你的系统只能使用GB2312编码，不能使用UTF-8编码，可以使用这
 * 个函数进行设置，让相关函数正常转换字符编码 
 * */ 

LCUI_END_HEADER

#include LC_MISC_H

/* 如果是使用C++语言，那么就包含LCUI的C++版头文件 */
#ifdef __cplusplus
#include LC_LCUI_HPP
#endif

#endif /* __LCUI_H__ */


