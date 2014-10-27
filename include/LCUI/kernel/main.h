/* ***************************************************************************
 * main.h -- The main functions for the LCUI normal work
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
 * main.h -- 使LCUI能够正常工作的相关主要函数
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
#ifndef __LCUI_KERNEL_MAIN_H__
#define __LCUI_KERNEL_MAIN_H__

LCUI_BEGIN_HEADER

/**************** 任务的添加模式 *******************/
#define ADD_MODE_ADD_NEW	0 /* 新增 */
#define ADD_MODE_NOT_REPEAT	1 /* 不能重复 */
#define ADD_MODE_REPLACE	2 /* 覆盖 */

#define AND_ARG_F	1<<3	/* 第一个参数 */
#define AND_ARG_S 	1<<4	/* 第二个参数 */
/***************************************************/

typedef struct {
	/* 
	 * 函数ID，部件库需要这ID标识函数类型，往程序的任务队列添加
	 * 任务也需要该ID来标识目标程序ID 
	 * */
	LCUI_ID id;
	void (*func)(void*,void*);	/* 函数指针 */
	
	/* 以下参数该怎么传给回调函数，具体要看是如何处理事件的 */  
	void *arg[2];			/* 传给函数的两个参数 */
	LCUI_BOOL destroy_arg[2];	/* 指定是否在调用完回调函数后，销毁参数 */
} LCUI_Func, LCUI_Task;

enum LCUI_SystemEventType {
	LCUI_KEYDOWN,		/**< 按键输入事件，键盘上任意键均可触发 */
	LCUI_KEYPRESS,		/**< 按键输入事件，仅字母、数字等ANSI字符键可触发 */
	LCUI_KEYUP,		/**< 按键释放事件 */
	LCUI_MOUSEMOVE,		/**< 鼠标移动事件 */
	LCUI_MOUSEDOWN,		/**< 鼠标按键按下事件 */
	LCUI_MOUSEUP,		/**< 鼠标按键释放事件 */
	LCUI_QUIT,		/**< 退出事件，在LCUI退出时触发 */
	LCUI_USER		/**< 用户事件，可以把这个当成系统事件与用户事件的分界 */
};

typedef struct {
	int type;			/**< 事件类型标识号 */
	const char *type_name;		/**< 事件类型名称 */
	int which;			/**< 指示按了哪个键或按钮 */
	int x, y;			/**< 当前鼠标坐标 */
	int offset_x, offset_y;		/**< 鼠标的坐标偏移量 */
	void *data;			/**< 附加数据 */
	void (*destroy_data)(void*);	/**< 用于销毁数据的回调函数 */
} LCUI_SystemEvent;

typedef void* LCUI_MainLoop;

#ifdef LCUI_BUILD_IN_WIN32
#include <Windows.h>
LCUI_API void Win32_LCUI_Init( HINSTANCE hInstance );
#endif

/*-------------------------- system event <START> ---------------------------*/

/** 绑定事件 */
LCUI_API int LCUI_BindEvent( const char *event_name,
		    void(*func)(LCUI_SystemEvent*,void*),
		    void *func_arg, void (*arg_destroy)(void*) );

/** 解除事件绑定 */
LCUI_API int LCUI_UnbindEvent( int event_handler_id );

/** 投递事件 */
LCUI_API int LCUI_PostEvent( const char *name, LCUI_SystemEvent *event );

/*--------------------------- system event <END> ----------------------------*/

/*--------------------------- Main Loop ------------------------------*/
/* 新建一个主循环 */
LCUI_API LCUI_MainLoop LCUI_MainLoop_New( void );

/* 运行目标循环 */
LCUI_API int LCUI_MainLoop_Run( LCUI_MainLoop loop );

/* 标记目标主循环需要退出 */
LCUI_API void LCUI_MainLoop_Quit( LCUI_MainLoop loop );

/*----------------------- End MainLoop -------------------------------*/

LCUI_API int LCUI_AddTask( LCUI_Task *task );

/** 从程序任务队列中删除有指定回调函数的任务 */
LCUI_API int LCUI_RemoveTask( CallBackFunc task_func, LCUI_BOOL need_lock );

/** 锁住任务的运行 */
void LCUI_LockRunTask(void);

/** 解锁任务的运行 */
void LCUI_UnlockRunTask(void);

/* 检测LCUI是否活动 */ 
LCUI_API LCUI_BOOL LCUI_Active(void);

/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */ 
LCUI_API int LCUI_Init( int w, int h, int mode );

/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
LCUI_API int LCUI_Main( void );

/* 获取LCUI的版本 */
LCUI_API int LCUI_GetSelfVersion( char *out );

/* 注册终止函数，以在LCUI程序退出时调用 */
LCUI_API int LCUI_AtExit( void (*func)(void));

/* 退出LCUI，释放LCUI占用的资源 */
LCUI_API void LCUI_Quit( void );

LCUI_API void LCUI_Exit( int exit_code );

/** 检测当前是否在主线程上 */
LCUI_API LCUI_BOOL LCUI_IsOnMainLoop(void);

LCUI_END_HEADER

#endif
