/* ***************************************************************************
 * LCUI_Work.h -- LCUI's other work
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
 * LCUI_Work.h -- LCUI 的其它工作
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

#ifndef __LCUI_WORK_H__
#define __LCUI_WORK_H__

#define RELATIVE_POS	1 /* 相对坐标 */
#define NORMAL_POS	0 /*普通坐标 */

typedef enum {
	EVENT_DRAG,	/* 部件的拖动事件 */
	EVENT_CLICKED,	/* 部件的点击事件 */
	EVENT_MOVE,	/* 部件的移动事件 */
	EVENT_KEYBOARD,	/* 按键事件 */
	EVENT_FOCUS_IN,	/* 得到焦点 */
	EVENT_FOCUS_OUT	/* 失去焦点 */
} WidgetEventType; 

typedef struct {
	uint8_t type;
	LCUI_Pos new_pos;	/* 部件的新全局坐标 */
	LCUI_Pos cursor_pos;	/* 鼠标游标的坐标 */
	int first_click;	/* 标志，是否为首次点击 */ 
	int end_click;		/* 标志，是否已经结束拖动 */
} LCUI_WidgetDragEvent; 

typedef struct {
	uint8_t type;
	BOOL focus;
} LCUI_WidgetFocusEvent;

typedef LCUI_KeyboardEvent LCUI_WidgetKeyboardEvent;

typedef union {
	uint8_t type;
	LCUI_WidgetDragEvent drag;
	LCUI_WidgetKeyboardEvent key;
	LCUI_WidgetFocusEvent focus;
} LCUI_WidgetEvent;

LCUI_BEGIN_HEADER

/***************************** Func ***********************************/
void NULL_Func();
/* 功能：空函数，不做任何操作 */ 

void FuncQueue_Init(LCUI_Queue *queue);
/* 功能：初始化函数指针队列 */ 

/********************* 处理部件拖动/点击事件 ******************************/
/* 将回调函数与部件的指定事件进行关联 */
int Widget_Event_Connect ( LCUI_Widget *widget, WidgetEventType event_id, 
			void (*func)(LCUI_Widget*, LCUI_WidgetEvent*) );

void Widget_Event_Init();
/* 功能：初始化部件事件处理 */ 

/*--------------------------- Focus Proc ------------------------------*/
BOOL 
Set_Focus( LCUI_Widget *widget );
/* 
 * 功能：为部件设置焦点
 * 说明：上个获得焦点的部件会得到EVENT_FOCUS_OUT事件，而当前获得焦点的部件会得到
 * EVENT_FOCUS_IN事件。
 * */ 

BOOL 
Cancel_Focus( LCUI_Widget *widget );
/* 
 * 功能：取消指定部件的焦点
 * 说明：该部件会得到EVENT_FOCUS_OUT事件，并且，会将焦点转移至其它部件
 * */ 

BOOL
Reset_Focus( LCUI_Widget* widget );
/* 复位指定部件内的子部件的焦点 */ 

/*------------------------- End Focus Proc ----------------------------*/

/*************************** Event End *********************************/

LCUI_END_HEADER

#endif

