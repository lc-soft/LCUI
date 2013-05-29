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
 * 版权所有 (C) 2013 归属于
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

/************************ LCUI程序的数据 *******************************/
typedef struct _LCUI_App 
{
	LCUI_ID id;		/* LCUI程序的ID，如果是以线程方式运行的话，这个就是线程ID */ 
	void (*func)(void);	/* 在LCUI退出时调用的函数 */
	LCUI_Queue tasks;	/* 程序的任务队列 */
	LCUI_Queue events;	/* 事件队列 */
	LCUI_Queue widget_lib;	/* 部件类型库 */
}
LCUI_App;
/**********************************************************************/

/***************************整个LCUI的数据 *****************************/
typedef struct _LCUI_System
{
	int state;		/* 状态 */ 
	int mode;		/* LCUI的运行模式 */

	LCUI_BOOL init;		/* 指示LCUI是否初始化过 */
	LCUI_BOOL need_sync_area;	/* 指示是否需要转移部件中记录的区域数据 */ 
	
	LCUI_Thread self_id;		/* 保存LCUI主程序的线程的ID */
	LCUI_Thread display_thread;	/* 保存核心处理的线程的ID */
	LCUI_Thread timer_thread;	/* 定时器列表处理线程的ID */
	LCUI_Thread dev_thread;		/* 设备输入数据处理线程的ID */

	LCUI_Queue sys_event_slots;	/* 事件槽记录 */
	LCUI_Queue user_event_slots;	/* 事件槽记录 */
	LCUI_Queue press_key;		/* 保存已被按下的按键的键值 */
	LCUI_Queue dev_list;		/* 设备列表 */
	LCUI_Queue timer_list;		/* 定时器列表 */
	LCUI_Queue app_list;		/* LCUI程序列表 */
	LCUI_Queue widget_list;		/* 部件队列，对应它的显示顺序 */
	LCUI_Queue widget_msg;		/* 部件消息 */
	LCUI_GraphLayer *root_glayer;	/* 根图层 */
} LCUI_System;
/***********************************************************************/

typedef struct {
	LCUI_ID app_id;
	LCUI_BOOL quit;
	LCUI_BOOL running;
	int level;
} LCUI_MainLoop;

extern LCUI_System  LCUI_Sys;

#ifdef LCUI_BUILD_IN_WIN32
LCUI_API void Win32_LCUI_Init( HINSTANCE hInstance );
#endif

/*------------------------------ LCUIApp -----------------------------*/
/* 根据程序的ID，获取指向程序数据结构的指针 */
LCUI_API LCUI_App* LCUIApp_Find( LCUI_ID id );

/* 获取指向程序数据的指针 */
LCUI_API LCUI_App* LCUIApp_GetSelf( void );

/* 获取程序ID */
LCUI_API LCUI_ID LCUIApp_GetSelfID( void );

/* 注册终止函数，以在LCUI程序退出时调用 */
LCUI_API int LCUIApp_AtQuit( void (*callback_func)(void));
/*--------------------------- End LCUIApp ----------------------------*/

/*--------------------------- Main Loop ------------------------------*/
/* 新建一个主循环 */
LCUI_API LCUI_MainLoop* LCUI_MainLoop_New( void );

/* 设定主循环等级，level值越高，处理主循环退出时，也越早处理该循环 */
LCUI_API int LCUI_MainLoop_Level( LCUI_MainLoop *loop, int level );

/* 运行目标循环 */
LCUI_API int LCUI_MainLoop_Run( LCUI_MainLoop *loop );

/* 标记目标主循环需要退出 */
LCUI_API int LCUI_MainLoop_Quit( LCUI_MainLoop *loop );

/*----------------------- End MainLoop -------------------------------*/

LCUI_API LCUI_BOOL LCUI_Active();
/* 功能：检测LCUI是否活动 */ 

/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */ 
LCUI_API int LCUI_Init( int mode, void *arg );

/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
LCUI_API int LCUI_Main( void );

/* 获取LCUI的版本 */
LCUI_API int LCUI_GetSelfVersion( char *out );

/* 用于退出LCUI，释放LCUI占用的资源 */
LCUI_API void LCUI_Quit( void );

LCUI_END_HEADER

#endif
