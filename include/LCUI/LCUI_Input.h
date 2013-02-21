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

#ifdef LCUI_KEYBOARD_DRIVER_LINUX

#define LCUIKEY_ESC		27
#define LCUIKEY_ENTER		10
#define LCUIKEY_BACKSPACE	127
#define LCUIKEY_DELETE		295
#define LCUIKEY_HOMEPAGE	293
#define LCUIKEY_END		296

#define LCUIKEY_SPACE	' '
#define LCUIKEY_0	'0'
#define LCUIKEY_1	'1'
#define LCUIKEY_2	'2'
#define LCUIKEY_3	'3'
#define LCUIKEY_4	'4'
#define LCUIKEY_5	'5'
#define LCUIKEY_6	'6'
#define LCUIKEY_7	'7'
#define LCUIKEY_8	'8'
#define LCUIKEY_9	'9'

#define LCUIKEY_A	'A'
#define LCUIKEY_B	'B'
#define LCUIKEY_C	'C'
#define LCUIKEY_D	'D'
#define LCUIKEY_E	'E'
#define LCUIKEY_F	'F'
#define LCUIKEY_G	'G'
#define LCUIKEY_H	'H'
#define LCUIKEY_I	'I'
#define LCUIKEY_J	'J'
#define LCUIKEY_K	'K'
#define LCUIKEY_L	'L'
#define LCUIKEY_M	'M'
#define LCUIKEY_N	'N'
#define LCUIKEY_O	'O'
#define LCUIKEY_P	'P'
#define LCUIKEY_Q	'Q'
#define LCUIKEY_R	'R'
#define LCUIKEY_S	'S'
#define LCUIKEY_T	'T'
#define LCUIKEY_U	'U'
#define LCUIKEY_V	'V'
#define LCUIKEY_W	'W'
#define LCUIKEY_X	'X'
#define LCUIKEY_Y	'Y'
#define LCUIKEY_Z	'Z'

#define LCUIKEY_a	'a'
#define LCUIKEY_b	'b'
#define LCUIKEY_c	'c'
#define LCUIKEY_d	'd'
#define LCUIKEY_e	'e'
#define LCUIKEY_f	'f'
#define LCUIKEY_g	'g'
#define LCUIKEY_h	'h'
#define LCUIKEY_i	'i'
#define LCUIKEY_j	'j'
#define LCUIKEY_k	'k'
#define LCUIKEY_l	'l'
#define LCUIKEY_m	'm'
#define LCUIKEY_n	'n'
#define LCUIKEY_o	'o'
#define LCUIKEY_p	'p'
#define LCUIKEY_q	'q'
#define LCUIKEY_r	'r'
#define LCUIKEY_s	's'
#define LCUIKEY_t	'i'
#define LCUIKEY_u	'u'
#define LCUIKEY_v	'v'
#define LCUIKEY_w	'w'
#define LCUIKEY_x	'x'
#define LCUIKEY_y	'y'
#define LCUIKEY_z	'z'


/* 这个是诺亚舟NP1500学习机的游戏键A和B键的键值 */
#define KEY_AA      342
#define KEY_BB      343

#define _PC_
#ifndef _PC_
#define LCUIKEY_DOWN	172
#define LCUIKEY_UP	171
#define LCUIKEY_LEFT	174
#define LCUIKEY_RIGHT	173
#else
#define LCUIKEY_DOWN    184
#define LCUIKEY_UP      183
#define LCUIKEY_LEFT    186
#define LCUIKEY_RIGHT   185
#endif

#else /* WIN32 的键盘键值 */
#include <winuser.h>

#define LCUIKEY_ESC		VK_ESCAPE
#define LCUIKEY_ENTER		VK_RETURN
#define LCUIKEY_BACKSPACE	VK_BACK
#define LCUIKEY_DELETE		VK_DELETE
#define LCUIKEY_HOMEPAGE	VK_HOME
#define LCUIKEY_END		VK_END
#define LCUIKEY_SPACE		VK_SPACE
#define LCUIKEY_TAB		VK_TAB

#define LCUIKEY_DOWN    VK_DOWN
#define LCUIKEY_UP      VK_UP
#define LCUIKEY_LEFT    VK_LEFT
#define LCUIKEY_RIGHT   VK_RIGHT

#endif

#define	PRESSED	10
#define	FREE	5

#define MOUSE_LEFT_KEY		1
#define MOUSE_RIGHT_KEY		2

LCUI_BEGIN_HEADER

/****************************** Mouse *********************************/
/*
 * 功能：检测鼠标事件中鼠标左键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标左键的键值
 *   0   鼠标左键已经释放
 *   1   鼠标左键处于按下状态
 **/ 
int Mouse_LeftButton( LCUI_MouseButtonEvent *event );

/*
 * 功能：检测鼠标事件中鼠标右键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标右键的键值
 *   0   鼠标右键已经释放
 *   1   鼠标右键处于按下状态
 **/
int Mouse_RightButton( LCUI_MouseButtonEvent *event );

int Click_LeftButton (LCUI_MouseButtonEvent *event);
/*
 * 功能：检测是否是按鼠标左键
 **/ 

/* 功能：处理鼠标产生的事件 */
void LCUI_PushMouseEvent( LCUI_Pos new_pos, int button_type );

int Check_Mouse_Support();
/* 功能：检测鼠标的支持 */ 

LCUI_BOOL Enable_Mouse_Input();
/* 功能：启用鼠标输入处理 */ 

LCUI_BOOL Disable_Mouse_Input();
/* 功能：禁用鼠标输入处理 */ 

/* 初始化鼠标输入模块 */
int LCUIModule_Mouse_Init( void );
/**************************** Mouse End *******************************/


/***************************** Key *************************************/ 
/* 初始化键盘输入 */
int LCUIKeyboard_Init( void );

/* 停用键盘输入 */
int LCUIKeyboard_End( void );

/* 添加键盘的按键按下事件 */
void LCUIKeyboard_HitKey( int key_code );

/* 添加键盘的按键释放事件 */
void LCUIKeyboard_FreeKey( int key_code );

/* 检测是否有按键按下 */
LCUI_BOOL LCUIKeyboard_IsHit( void );

/* 功能：获取被按下的按键的键值 */
int LCUIKeyboard_Get( void );

/* 检测指定键值的按键是否处于按下状态 */
LCUI_BOOL LCUIKey_IsHit( int key_code );

/* 标记指定键值的按键已释放 */
void LCUIKey_Free( int key_code );

/* 初始化键盘输入模块 */
void LCUIModule_Keyboard_Init( void );

/* 停用键盘输入模块 */
void LCUIModule_Keyboard_End( void );
/*************************** Key End **********************************/


/************************* TouchScreen *********************************/ 
int Check_TouchScreen_Support();
/* 功能：检测是否支持触瓶 */ 

LCUI_BOOL Enable_TouchScreen_Input();
/* 功能：启用鼠标输入处理 */ 

LCUI_BOOL Disable_TouchScreen_Input();
/* 功能：撤销鼠标输入处理 */ 

void *Get_TouchScreen();
/* 获取触屏设备文件句柄 */

/* 初始化触屏输入模块 */
void LCUIModule_TouchScreen_Init( void );
/*********************** TouchScreen End *******************************/

LCUI_END_HEADER

#endif /* __LCUI_INPUT_H__ */
