/* ***************************************************************************
 * main.h -- The main functions for the LCUI normal work
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * main.h -- 使LCUI能够正常工作的相关主要函数
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
 * ***************************************************************************/
#ifndef LCUI_MAIN_H
#define LCUI_MAIN_H

LCUI_BEGIN_HEADER

typedef void(*LCUI_AppTaskFunc)(void*, void*);

typedef struct LCUI_AppTaskRec_ {
	LCUI_AppTaskFunc func;		/**< 处理函数 */
	void *arg[2];			/**< 传给函数的两个参数 */
	void (*destroy_arg[2])(void*);	/**< 参数的销毁函数 */
} LCUI_AppTaskRec, *LCUI_AppTask;

enum LCUI_SysEventType {
	LCUI_NONE,
	LCUI_KEYDOWN,		/**< 键盘触发的按键按下事件 */
	LCUI_KEYPRESS,		/**< 按键输入事件，仅字母、数字等ANSI字符键可触发 */
	LCUI_KEYUP,		/**< 键盘触发的按键释放事件 */
	LCUI_MOUSE,		/**< 鼠标事件 */
	LCUI_MOUSEMOVE,		/**< 鼠标触发的鼠标移动事件 */
	LCUI_MOUSEDOWN,		/**< 鼠标触发的按钮按下事件 */
	LCUI_MOUSEUP,		/**< 鼠标触发的按钮释放事件 */
	LCUI_MOUSEWHEEL,	/**< 鼠标触发的滚轮滚动事件 */
	LCUI_TEXTINPUT,		/**< 输入法触发的文本输入事件 */
	LCUI_TOUCH,
	LCUI_TOUCHMOVE,
	LCUI_TOUCHDOWN,
	LCUI_TOUCHUP,
	LCUI_PAINT,
	LCUI_WIDGET,
	LCUI_QUIT,		/**< 在 LCUI 退出前触发的事件 */
	LCUI_USER = 100		/**< 用户事件，可以把这个当成系统事件与用户事件的分界 */
};

typedef struct LCUI_TouchPointRec_ {
	int x;
	int y;
	int id;
	int state;
	LCUI_BOOL is_primary;
} LCUI_TouchPointRec, *LCUI_TouchPoint;

typedef struct LCUI_PaintEvent_ {
	LCUI_Rect rect;
} LCUI_PaintEvent;

typedef struct LCUI_KeyboardEvent_ {
	int code;
} LCUI_KeyboardEvent;

typedef struct LCUI_MouseMotionEvent_ {
	int x, y;
	int xrel, yrel;
} LCUI_MouseMotionEvent;

typedef struct LCUI_MouseButtonEvent_ {
	int x, y;
	int button;
} LCUI_MouseButtonEvent;

typedef struct LCUI_MouseWheelEvent_ {
	int x, y;
	int delta;
} LCUI_MouseWheelEvent;

typedef struct LCUI_TouchEvent_ {
	int n_points;
	LCUI_TouchPoint points;
} LCUI_TouchEvent;

typedef struct LCUI_TextInputEvent_ {
	wchar_t *text;
	size_t length;
} LCUI_TextInputEvent;

typedef struct LCUI_SysEventRec_ {
	uint32_t type;
	void *data;
	union {
		LCUI_MouseMotionEvent motion;
		LCUI_MouseButtonEvent button;
		LCUI_MouseWheelEvent wheel;
		LCUI_TextInputEvent text;
		LCUI_KeyboardEvent key;
		LCUI_TouchEvent touch;
		LCUI_PaintEvent paint;
	};
} LCUI_SysEventRec, *LCUI_SysEvent;

typedef void(*LCUI_SysEventFunc)(LCUI_SysEvent, void*);

/** LCUI 应用程序驱动接口，封装了各个平台下的应用程序相关功能支持接口 */
typedef struct LCUI_AppDriverRec_ {
	void ( *ProcessEvents )(void);
	int( *BindSysEvent )(int, LCUI_EventFunc, void*, void( *)(void*));
	int( *UnbindSysEvent )(int, LCUI_EventFunc);
	int( *UnbindSysEvent2 )(int);
	void*(*GetData)(void);
} LCUI_AppDriverRec, *LCUI_AppDriver;

typedef struct LCUI_MainLoopRec_ {
	int state;		/**< 主循环的状态 */
	unsigned long int tid;	/**< 当前运行该主循环的线程的ID */
} LCUI_MainLoopRec, *LCUI_MainLoop;


LCUI_API int LCUI_BindEvent( int id, LCUI_SysEventFunc func, void *data,
			     void( *destroy_data )(void*) );

LCUI_API int LCUI_UnbindEvent( int handler_id );

LCUI_API int LCUI_TriggerEvent( LCUI_SysEvent e, void *arg );

LCUI_API int LCUI_CreateTouchEvent( LCUI_SysEvent e,
				    LCUI_TouchPoint points, int n_points );

LCUI_API void LCUI_DestroyEvent( LCUI_SysEvent e );

LCUI_API int LCUI_BindSysEvent( int event_id, LCUI_EventFunc func,
				void *data, void( *destroy_data )(void*) );

LCUI_API int LCUI_UnbindSysEvent( int event_id, LCUI_EventFunc func );

LCUI_API void *LCUI_GetAppData( void );

/** 处理当前所有事件 */
LCUI_API void LCUI_ProcessEvents( void );

/**
 * 添加任务
 * 该任务将会添加至 UI 线程中执行
 */
LCUI_API LCUI_BOOL LCUI_PostTask( LCUI_AppTask task );

/** LCUI_PostTask 的简化版本 */
#define LCUI_PostSimpleTask(FUNC, ARG1, ARG2) do {\
	LCUI_AppTaskRec task = { 0 };\
	task.arg[0] = (void*)ARG1;\
	task.arg[1] = (void*)ARG2;\
	task.func = (LCUI_AppTaskFunc)(FUNC);\
	LCUI_PostTask( &task );\
} while( 0 );

/** 销毁任务 */
LCUI_API void LCUI_DeleteTask( LCUI_AppTask task );

/* 运行任务 */
LCUI_API int LCUI_RunTask( LCUI_AppTask task );

/* 新建一个主循环 */
LCUI_API LCUI_MainLoop LCUIMainLoop_New( void );

/* 运行目标循环 */
LCUI_API int LCUIMainLoop_Run( LCUI_MainLoop loop );

/* 标记目标主循环需要退出 */
LCUI_API void LCUIMainLoop_Quit( LCUI_MainLoop loop );

/* 检测LCUI是否活动 */ 
LCUI_API LCUI_BOOL LCUI_IsActive( void );

LCUI_API void LCUI_InitBase( void );

LCUI_API void LCUI_InitApp( LCUI_AppDriver app );

/** 初始化 LCUI 各项功能 */ 
LCUI_API void LCUI_Init( void );

/**
 * 进入 LCUI 主循环
 * 调用该函数后，LCUI 会将当前线程作为 UI 线程，用于处理部件更新、布局、渲染等
 * 与图形界面相关的任务。
 */
LCUI_API int LCUI_Main( void );

/** 获取LCUI的版本 */
LCUI_API int LCUI_GetSelfVersion( char *out );

/** 释放LCUI占用的资源 */
LCUI_API int LCUI_Destroy( void );

/** 退出LCUI，释放LCUI占用的资源 */
LCUI_API void LCUI_Quit( void );

/** 退出 LCUI，并设置退出码 */
LCUI_API void LCUI_Exit( int code );

/** 检测当前是否在主线程上 */
LCUI_API LCUI_BOOL LCUI_IsOnMainLoop(void);

LCUI_END_HEADER

#endif
