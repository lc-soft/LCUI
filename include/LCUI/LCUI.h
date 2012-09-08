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

#define LCUI_VERSION "0.12.5"

#include <pthread.h>
#include <ft2build.h>
#include <tslib.h>
#include FT_FREETYPE_H 

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

#ifndef false
#define false 0
#endif

#ifndef ture
#define ture 1
#endif

enum _BOOL
{
	IS_FALSE = 0,
	IS_TRUE = 1,
	IS_ERROR = -1
};

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
#define LAYOUT_ZOOM	1	  /* 缩放 */
#define LAYOUT_STRETCH	2	  /* 拉伸 */
#define LAYOUT_CENTER	3	  /* 居中 */
#define LAYOUT_TILE	4	  /* 平铺 */
#define LAYOUT_NONE	0	  /* 无 */
#define LAYOUT_NORMAL	0 
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
enum _LCUI_Border_Style
{
	BORDER_STYLE_NONE,
	BORDER_STYLE_STANDARD,
	BORDER_STYLE_LINE_BORDER,
	BORDER_STYLE_ROUND_BORDER
};
/*****************************************/

/************ 部件的背景模式 ***************/
enum _LCUI_BG_Mode
{
	BG_MODE_TRANSPARENT,
	BG_MODE_FILL_BACKCOLOR
};
/*****************************************/

/*********************** 定义了一些对齐方式 *********************/
enum _LCUI_Align
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
};
/*****************************************************************/

/*************** 部件的几种状态 ***************/
enum _LCUI_Widget_Status
{
	WIDGET_STATUS_NORMAL,	/* 普通状态 */
	WIDGET_STATUS_CLICKED,	/* 被鼠标点击，已松开 */
	WIDGET_STATUS_OVERLAY,	/* 被鼠标覆盖 */
	WIDGET_STATUS_CLICKING,	/* 被鼠标点击，未松开 */
	WIDGET_STATUS_FOCUS,	/* 焦点 */
	WIDGET_STATUS_DISABLE	/* 不可用 */
};
/*******************************************/

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
typedef struct	_LCUI_Border		LCUI_Border;
typedef struct	_LCUI_WChar_T		LCUI_WChar_T;
typedef struct	_LCUI_Rect		LCUI_Rect; 
typedef struct	_LCUI_String		LCUI_String;
typedef struct	_LCUI_Widget		LCUI_Widget;  
typedef struct	_LCUI_RGB		LCUI_RGB;
typedef struct	_LCUI_RGBA		LCUI_RGBA;
typedef struct	_LCUI_ImageInfo	LCUI_ImageInfo;
typedef struct	_LCUI_Bitmap		LCUI_Bitmap;
typedef struct	_LCUI_Graph		LCUI_Graph;
typedef struct	_LCUI_Font		LCUI_Font;
typedef struct	_LCUI_TS		LCUI_TS;
typedef struct	_LCUI_Mouse		LCUI_Mouse;
typedef struct	_LCUI_MouseEvent	LCUI_MouseEvent;
typedef struct	_LCUI_Key		LCUI_Key;
typedef struct	_LCUI_Pos		LCUI_Pos;
typedef struct	_LCUI_Size		LCUI_Size;
typedef struct	_Thread_Queue		Thread_Queue;
typedef struct	_Thread_TreeNode	Thread_TreeNode;
typedef struct	_LCUI_Screen		LCUI_Screen;
typedef struct	_Dirty_Rect		Dirty_Rect;
typedef struct	_Matrix		Matrix;
typedef struct	_Pixel			Pixel; 
typedef struct	_LCUI_Queue		LCUI_MovObjQueue;
typedef struct	_LCUI_Queue 		LCUI_AppList;


typedef enum	_LCUI_Border_Style	LCUI_Border_Style;
typedef enum	_LCUI_BG_Mode		LCUI_BG_Mode;
typedef enum	_LCUI_Widget_Status	LCUI_Widget_Status; 
typedef enum	_LCUI_Align		LCUI_Align;

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
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}; 
/****************************************/

/************ 保存RGBA配色数据 *************/
struct _LCUI_RGBA
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
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
	LCUI_Pos	pos;	/* 位置 */ 
	LCUI_RGB	rgb;	/* RGBA值 */
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

/***************************** 脏矩形 *********************************/
struct _Dirty_Rect
{
	int	redraw;		/* 
				 * 指定是否重绘，通常在进行局部刷新时，根
				 * 据它可以减少该局部区域不必要的图形混合
				 * 操作 
				 * */
									 
	LCUI_Size	size;			/* 尺寸 */
	Pixel		*first_sample_point;	/* 储存第一次采样的采样点 */
	Pixel		*second_sample_point;	/* 储存第二次采样的采样点 */
	int		total_point;		/* 总采样点数 */
};
/*********************************************************************/

/***************************** 矩阵 *********************************/
struct _Matrix
{
	Dirty_Rect	**rect;			/* 组成矩阵的脏矩形 */
	LCUI_Size	size, pixel_size;	/* 不同单位的尺寸 */
	LCUI_Pos	pos;			/* 用于记录在屏幕矩阵的起点位置 */
};
/********************************************************************/


/****************************** 图片信息 *********************************/
struct _LCUI_ImageInfo
{
	int	type;		/* 存储图片类型 */
	int	width, height;	/* 高 */
	int	bit_depth;	/* 位深 */
	int	channel;	/* 是否需要透明度，分配内存时会根据它分配 */
};/*  图片信息 */
/************************************************************************/

/*******************保存字体位图数据***************************/
struct _LCUI_Bitmap
{
	unsigned char  **data;	/* 字体位图数据 */
	unsigned char  alpha;	/* 全局透明度 */
	
	int	width;	   /* 位图的宽度 */
	int	height;	  /* 位图的高度 */
	int	malloc;	  /* 是否分配了内存用于存储 */ 
};/* 字体位图数据 */
/**************************************************************/

/************************wchar_t型字符***************************/
struct _LCUI_WChar_T
{
	wchar_t		char_code;	/* 字符码 */
	LCUI_Bitmap	bitmap;		/* 字符的位图数据 */
	LCUI_RGB	color;		/* 该文字的配色 */
	int		update;		/* 标明这个字符是否需要刷新 */ 
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

/***************************** 图片数据 ********************************/
struct _LCUI_Graph
{
	int	type;		/* 图片类型 */
	int	bit_depth;	/* 位深 */
	
	int quote;		/* 指示是否引用其它图层中的图形 */
	LCUI_Graph *src;	/* 所引用的对象 */
	LCUI_Pos pos;		/* 在引用另一个图层中的图形时，会保存区域的起点位置 */
	int	width, height;	/* 尺寸 */
	
	unsigned char	**rgba;	/* 图片数组 */
	unsigned char	alpha;	/* 全局透明度，表示整张图片的透明度，默认为255 */
	int		flag;	/* 是否需要透明度，分配内存时会根据它分配 */
	int		malloc;	/* 是否分配了内存 */
	thread_rwlock	lock;	/* 锁，用于数据保护 */
};/*  存储图片数据 */
/**********************************************************************/

/******************************保存字体信息********************************/
struct _LCUI_Font/* 字体信息数据 */
{
	int		type;		/* 类型(DEFAULT / CUSTOM) */
	unsigned int	size;		/* 字体大小 */
	LCUI_RGB	fore_color;	/* 前景色 */
	LCUI_String	font_file;	/* 字体文件的路径 */
	LCUI_String	family_name;	/* 字体名称 */
	LCUI_String	style_name;	/* 字体风格名称 */
	int		space;		/* 字体的左右间距（单位为像素） */
	int		linegap;	/* 字体的行距（单位为像素） */
	FT_Library	ft_lib;		/* FreeType2库的句柄  */
	FT_Face		ft_face;	/* FreeType2的face对象的句柄 */
	int		load_flags;	/* 字形载入标志 */
	int		render_mode;	/* 字形转换模式标志 */
	int		status;		/* 状态，是否打开了字体库 */
};
/************************************************************************/


/********************** 鼠标相关信息 ***************************/
struct _LCUI_Mouse
{
	int fd, status;		 /* 句柄，状态 */
	float move_speed;	 /* 鼠标移动速度，1.0为正常速度 */
	pthread_t thread;	 /* 处理鼠标输入的线程的ID */
	LCUI_EventQueue event; /* 记录鼠标事件相关的信息 */
};
/*************************************************************/


/************************ 鼠标事件 **************************/
struct _LCUI_MouseEvent
{
	LCUI_Widget	*widget;	/* 当前鼠标指针覆盖到的部件 */
	LCUI_Key	key;		/* 按键键值以及状态 */
	LCUI_Pos	global_pos;	/* 鼠标指针的全局坐标 */
	LCUI_Pos	pos;		/* 与当前覆盖到的部件的相对坐标 */
};
/***********************************************************/


/*********** 触屏相关 **************/
struct _LCUI_TS
{
	struct tsdev *td;
	int status;  /* 状态 */ 
	pthread_t thread;
};
/**********************************/
 

/*************** 边框 ******************/
struct _LCUI_Border
{
	int left,top,right,bottom;
};
/*************************************/

/******************************* 部件 **********************************/
struct _LCUI_Widget /* 存储窗口内所有控件的图形数据 */
{
	int		lock_display;	/* 指示是否锁定部件的显示顺序 */
	
	LCUI_String	type;		/* 部件的类型 */
	LCUI_ID	type_id;	/* 部件的类型ID */
	LCUI_String	style;		/* 部件的风格，对某些部件有效 */
	
	LCUI_Widget	*parent;	/* 保存父部件的指针 */
	LCUI_Queue	child;		/* 保存子部件的指针 */
	
	LCUI_ID app_id;	/* 所属程序的id */
	
	int response_flag;	/* 标志，指示是否响应部件的状态改变 */
	
	int enabled;		/* 部件是否启用 */
	int visible;		/* 确定该部件是可见的还是隐藏的 */
	int status;		/* 部件的状态 */
	
	void *private_data;   /* 该部件私有的数据的指针，其它的是各个部件公用的数据 */
	
	LCUI_Pos	pos;		/* 位置 */
	/* max_pos和min_pos限制了widget的移动范围 */
	int		limit_pos;	/* 指定是否限制部件的位置 */
	LCUI_Pos	max_pos;	/* 最大位置 */
	LCUI_Pos	min_pos;	/* 最小位置 */
	int		pos_type;	/* 位置类型 */
	int		align;		/* 部件的布局 */
	LCUI_Pos	offset;		/* xy轴的偏移量 */
	
	int		auto_size;	/* 指定是否自动调整自身的大小，以适应内容的大小 */ 
	int		limit_size;	/* 指定是否限制尺寸大小 */
	LCUI_Size	size;		/* 部件的尺寸，单位为像素 */
	/* max_size和min_size限制了widget的尺寸变动范围 */
	LCUI_Size	max_size;	/* 最大尺寸 */
	LCUI_Size	min_size;	/* 最小尺寸 */
	
	LCUI_RGB  back_color;  /* 背景色 */
	LCUI_RGB  fore_color;  /* 前景色 */
	
	LCUI_Border	border;		/* 边框 */
	LCUI_RGB	border_color;	/* 边框颜色 */
	int		border_style;	/* 边框类型(NONE:没有) */
	
	LCUI_Queue	event;		/* 保存部件的事件关联的数据 */
	LCUI_Queue	update_area;	/* 部件内需要刷新的区域 */ 
	
	LCUI_Queue	data;		/* 记录需要进行更新的数据 */
	
	int		bg_mode;	/* 背景模式，指定没有背景时是使用透明背景还是使用背景色填充 */
	LCUI_Graph	background_image;	 /* 背景图 */
	int		background_image_layout; /* 背景图的布局 */
	
	LCUI_Widget	*focus; /* 获得焦点的部件 */
	LCUI_Graph	graph; /* 部件的图形数据 */ 
	/* 以下是函数指针，闲函数名太长的话，可以直接用下面的 */
	void (*resize)(LCUI_Widget*, LCUI_Size);
	void (*move)(LCUI_Widget*, LCUI_Pos); 
	void (*show)(LCUI_Widget*);
	void (*hide)(LCUI_Widget*);
	void (*disable)(LCUI_Widget*);
	void (*enable)(LCUI_Widget*);
	void (*set_align)(LCUI_Widget*, LCUI_Align, LCUI_Pos); 
	void (*set_alpha)(LCUI_Widget*, unsigned char); 
	void (*set_border)(LCUI_Widget*, LCUI_RGB, LCUI_Border);
};
/**********************************************************************/



/************************ LCUI程序的数据 *******************************/
struct _LCUI_App 
{
	int		stop_loop;	/* 一个标志，用于指示是否停止主循环 */
	int		flag;		/* 一个标志，目前是将它作为是否处理部件局部刷新区域数据的标志 */ 
	LCUI_ID	id;		/* LCUI程序的ID,果是以线程方式运行的话，这个就是线程ID */ 
	
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
#define MAX_LCUI_IDLE_TIME  50000



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
	unsigned char	*fb_mem;	/* 指向图像输出设备映射到的内存的指针 */
	int		fb_dev_fd;	/* 图形显示设备的句柄 */
	size_t		smem_len;	/* 内存空间的大小 */
	int		bits;		/* 每个像素的用多少位表示 */
	//Matrix		*matrix;		/* 屏幕脏矩形矩阵 */
};
/***********************************************************************/

/************ 线程树的结点 ***************/
struct _Thread_TreeNode
{
	Thread_TreeNode *parent;	/* 父线程结点指针 */
	pthread_t tid;			/* 父线程ID */
	Thread_Queue child;		/* 子线程队列 */
};
/***************************************/


/***************************整个LCUI的数据 *****************************/
struct _LCUI_System
{
	Thread_TreeNode thread_tree; /* 线程关系树 */
	
	pthread_t	self_id;	/* 保存LCUI主程序的线程ID */
	pthread_t	core_thread;	/* 保存核心处理的线程ID */
	pthread_t	key_thread;	/* 保存按键处理的线程ID */
	
	int		status;		/* 状态 */
	int		init;		/* 一个标志，表示LCUI是否初始化过 */
	int		shift_flag;	/* 一个标志，表示是否需要转移部件中记录的区域数据 */
	
	LCUI_ID max_app_idle_time;	/* 程序最大的空闲时间 */
	LCUI_ID max_lcui_idle_time;	/* LCUI最大的空闲时间 */
	
	LCUI_Cursor	cursor;		/* 游标 */
	LCUI_Mouse	mouse;		/* 鼠标 */
	LCUI_TS	ts;	 	/* 触屏 */
	
	LCUI_Screen	screen;		/* 屏幕信息 */
	
	LCUI_Queue	press_key;	/* 保存已被按下的按键的键值 */
	LCUI_Queue	key_event;	/* 保存与按键事件关联的数据 */
	
	LCUI_Queue	widget_list;	/* 部件队列，对应它的显示顺序 */
	LCUI_AppList	app_list;	/* LCUI程序列表 */ 
	LCUI_Font	default_font;	/* 默认的字体数据 */ 
	LCUI_Queue	update_area;	/* 窗口外需要刷新的区域 */
};
/***********************************************************************/

extern LCUI_System  LCUI_Sys;

LCUI_BEGIN_HEADER

void Set_Default_Font(char *fontfile);
/* 
 * 功能：设定默认的字体文件路径
 * 说明：需要在LCUI初始化前使用，因为LCUI初始化时会打开默认的字体文件
 *  */ 

/************************* App Manage *********************************/
LCUI_App *Find_App(LCUI_ID id);
/* 功能：根据程序的ID，获取指向程序数据结构的指针 */ 

LCUI_App* Get_Self_AppPointer();
/* 功能：获取程序的指针 */ 

void Main_Loop_Quit();
/* 功能：让程序退出主循环 */ 
/*********************** App Manage End ******************************/

int Get_Screen_Width ();
/*
 * 功能：获取屏幕宽度
 * 返回值：屏幕的宽度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */ 

int Get_Screen_Height ();
/*
 * 功能：获取屏幕高度
 * 返回值：屏幕的高度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */ 

LCUI_Size Get_Screen_Size ();
/* 功能：获取屏幕尺寸 */ 

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color);
/* 功能：填充指定位置的像素点的颜色 */ 

void Write_Graph_To_FB (LCUI_Graph * src, LCUI_Pos pos);
/* 
 * 功能：写入图形数据至帧缓存，从而在屏幕上显示图形 
 * 说明：此函数的主要代码，参考自mgaveiw 0.1.4 的mga_vfb.c文件中的
 * write_to_fb函数的源代码.
 * */
 
void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out);
/* 
 * 功能：通过内存中的图像数据，捕获将在屏幕上显示的图像
 * 说明：效率较低，因为需要进行合成。
 *  */ 

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out);
/* 
 * 功能：直接读取帧缓冲中的图像数据
 * 说明：效率较高，但捕获的图像有可能会有问题。
 * */ 

int Add_Screen_Refresh_Area (LCUI_Rect rect);
/* 功能：在整个屏幕内添加需要刷新的区域 */ 

void nobuff_print(const char *fmt, ...);
/* 功能：打印正执行的操作的信息 */

int Get_Screen_Bits();
/* 功能：获取屏幕中的每个像素的表示所用的位数 */ 

LCUI_Pos Get_Screen_Center_Point();
/* 功能：获取屏幕中心点的坐标 */ 

int LCUI_Active();
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


