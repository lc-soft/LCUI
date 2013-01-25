
#ifndef __LCUI_KERNEL_MAIN_H__
#define __LCUI_KERNEL_MAIN_H__

LCUI_BEGIN_HEADER

/************************ LCUI程序的数据 *******************************/
typedef struct _LCUI_App 
{
	LCUI_ID id; /* LCUI程序的ID，如果是以线程方式运行的话，这个就是线程ID */ 
	BOOL stop_loop;
	
	LCUI_Queue	tasks;	/* 程序的任务队列 */
	LCUI_Queue	events;		/* 事件队列 */
	LCUI_Queue	widget_lib;	/* 部件类型库 */
	
	int encoding_type;	/* 字符编码类型，默认为UTF-8 */
}
LCUI_App;
/**********************************************************************/

/****************************** 屏幕信息 ********************************/
typedef struct _LCUI_Screen
{
	LCUI_Size	size;		/* 屏幕尺寸 */
	char		*fb_dev_name;	/* 图形输出设备的名称 */
	uchar_t	*fb_mem;	/* 指向图像输出设备映射到的内存的指针 */
	int		fb_dev_fd;	/* 图形显示设备的句柄 */
	size_t		smem_len;	/* 内存空间的大小 */
	int		bits;		/* 每个像素的用多少位表示 */
}
LCUI_Screen;
/***********************************************************************/

/***************************整个LCUI的数据 *****************************/
typedef struct _LCUI_System
{
	int status;		/* 状态 */ 
	BOOL init;		/* 指示LCUI是否初始化过 */
	BOOL need_sync_area;	/* 指示是否需要转移部件中记录的区域数据 */ 
	
	thread_t self_id;		/* 保存LCUI主程序的线程的ID */
	thread_t display_thread;	/* 保存核心处理的线程的ID */
	thread_t timer_thread;		/* 定时器列表处理线程的ID */
	thread_t dev_thread;		/* 设备输入数据处理线程的ID */
	Thread_TreeNode thread_tree; /* 线程关系树 */
	
	LCUI_Cursor	cursor;		/* 游标 */
	
	LCUI_Screen	screen;		/* 屏幕信息 */
	LCUI_Queue	mouse_event;	/* 鼠标事件 */
	LCUI_Queue	press_key;	/* 保存已被按下的按键的键值 */
	LCUI_Queue	key_event;	/* 保存与按键事件关联的数据 */
	LCUI_Queue	dev_list;	/* 设备列表 */
	LCUI_Queue	widget_list;	/* 部件队列，对应它的显示顺序 */
	LCUI_Widget	*focus_widget;	/* 获得焦点的部件 */
	LCUI_Queue	invalid_area;	/* 需要刷新的区域 */
	LCUI_Queue	timer_list;	/* 定时器列表 */
	LCUI_Queue	app_list;	/* LCUI程序列表 */ 
	LCUI_Font	default_font;	/* 默认的字体数据 */ 
	
	LCUI_GraphLayer *root_glayer;
}
LCUI_System;
/***********************************************************************/

extern LCUI_System  LCUI_Sys;

/************************* App Management *****************************/
LCUI_App *Find_App(LCUI_ID id);
/* 功能：根据程序的ID，获取指向程序数据结构的指针 */ 

LCUI_App* Get_Self_AppPointer();
/* 功能：获取程序的指针 */ 

/* 退出主循环 */
void LCUI_StopMainLoop( void );
/*********************** App Management End ***************************/

BOOL LCUI_Active();
/* 功能：检测LCUI是否活动 */ 

int LCUI_Init(int argc, char *argv[]);
/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */ 

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

#endif
