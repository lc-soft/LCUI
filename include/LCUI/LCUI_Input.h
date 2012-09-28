/* ***************************************************************************
 * LCUI_Input.h -- The input devices handling module of LCUI
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
 * LCUI_Input.h -- LCUI的输入设备处理模块
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

#ifndef __LCUI_INPUT_H__
#define __LCUI_INPUT_H__

#define KEY_ESC    27

#define KEY_ENTER   10

#define _PC_

/* 这个是诺亚舟NP1500学习机的游戏键A和B键的键值 */
#define KEY_AA      342
#define KEY_BB      343

#ifndef _PC_

#define KEY_DOWN    172
#define KEY_UP      171
#define KEY_LEFT    174
#define KEY_RIGHT   173

#else

#define KEY_DOWN    184
#define KEY_UP      183
#define KEY_LEFT    186
#define KEY_RIGHT   185

#endif

#define KEY_DELETE  127
/*
#define KEY_DOWN    264
#define KEY_UP      260
#define KEY_LEFT    272
#define KEY_RIGHT   268
 */

#define	PRESSED	10
#define	FREE	5

#define MOUSE_LEFT_KEY		1
#define MOUSE_RIGHT_KEY		2

LCUI_BEGIN_HEADER

/****************************** Mouse *********************************/
int Mouse_LeftButton(LCUI_MouseEvent *event);
/*
 * 功能：检测鼠标事件中鼠标左键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标左键的键值
 *   0   鼠标左键已经释放
 *   1   鼠标左键处于按下状态
 **/ 

int Mouse_RightButton(LCUI_MouseEvent *event);
/*
 * 功能：检测鼠标事件中鼠标右键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标右键的键值
 *   0   鼠标右键已经释放
 *   1   鼠标右键处于按下状态
 **/ 

int Click_LeftButton (LCUI_MouseEvent *event);
/*
 * 功能：检测是否是按鼠标左键
 **/ 

void Send_Mouse_Event(int event_id, LCUI_MouseEvent *event);
/* 功能：向已关联鼠标事件的程序发送任务 */ 

int KeyQueue_Find(LCUI_Queue *queue, int key_code);
/* 功能:在按键键值队列里查找指定键值的位置 */ 

void Handle_Mouse_Event(int button_type, LCUI_MouseEvent *event);
/* 功能：处理鼠标产生的事件 */ 

int Check_Mouse_Support();
/* 功能：检测鼠标的支持 */ 

int Enable_Mouse_Input();
/* 功能：启用鼠标输入处理 */ 

int Disable_Mouse_Input();
/* 功能：禁用鼠标输入处理 */ 
/**************************** Mouse End *******************************/


/***************************** Key *************************************/ 
int Set_Raw(int t);

int Check_Key(void);
/* 
 * 功能：检测是否有按键输入 
 * 返回值：
 *   1   有按键输入
 *   2   无按键输入
 * */ 


int Get_Key(void);
/* 功能：获取被按下的按键的键值 */ 

int Find_Pressed_Key(int key);
/*
 * 功能：检测指定键值的按键是否处于按下状态
 * 返回值：
 *   1 存在
 *   0 不存在
 **/ 
 
int Enable_Key_Input();
/* 功能：启用按键输入处理 */ 

int Disable_Key_Input();
/* 功能：撤销按键输入处理 */ 
/*************************** Key End **********************************/


/************************* TouchScreen *********************************/ 
int Check_TouchScreen_Support();
/* 功能：检测是否支持触瓶 */ 

int Enable_TouchScreen_Input();
/* 功能：启用鼠标输入处理 */ 

int Disable_TouchScreen_Input();
/* 功能：撤销鼠标输入处理 */ 

/*********************** TouchScreen End *******************************/

LCUI_END_HEADER

#endif /* __LCUI_INPUT_H__ */
