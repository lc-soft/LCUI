/* ***************************************************************************
 * LCUI_Input.h -- The input devices handling module of LCUI.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * LCUI_Input.h -- LCUI的输入设备处理模块。
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_INPUT_H__
#define __LCUI_INPUT_H__

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

#ifdef LCUI_KEYBOARD_DRIVER_LINUX

#define LCUIKEY_ESC		27
#define LCUIKEY_ENTER		10
#define LCUIKEY_BACKSPACE	127
#define LCUIKEY_DELETE		295
#define LCUIKEY_HOMEPAGE	293
#define LCUIKEY_END		296

#define LCUIKEY_CAPITAL		-1
#define LCUIKEY_SHIFT		-1

#define LCUIKEY_AA      342
#define LCUIKEY_BB      343

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
#define LCUIKEY_TAB		VK_TAB

#define LCUIKEY_DOWN		VK_DOWN
#define LCUIKEY_UP		VK_UP
#define LCUIKEY_LEFT		VK_LEFT
#define LCUIKEY_RIGHT		VK_RIGHT

#define LCUIKEY_CAPITAL		VK_CAPITAL
#define LCUIKEY_SHIFT		VK_SHIFT
#define LCUIKEY_CONTROL		VK_CONTROL

#endif

#define LCUIKEY_LEFTBUTTON	1	/* 鼠标左键 */
#define LCUIKEY_RIGHTBUTTON	2	/* 鼠标右键 */
#define LCUIKEYSTATE_PRESSED	1
#define LCUIKEYSTATE_RELEASE	0

LCUI_BEGIN_HEADER

/*---------------------------------- Mouse ---------------------------------*/

/** 投递鼠标按键按下事件 */
LCUI_API int LCUI_PostMouseDownEvent( int key_code );

/** 投递鼠标按键释放事件 */
LCUI_API int LCUI_PostMouseUpEvent( int key_code );

/** 投递鼠标移动事件 */
LCUI_API void LCUI_PostMouseMoveEvent( LCUI_Pos new_pos );

/** 初始化鼠标输入模块 */
LCUI_API int LCUIModule_Mouse_Init( void );

/*-------------------------------- Mouse End -------------------------------*/


/*-------------------------------- Keyboard --------------------------------*/

/**
 * 检测指定键值的按键是否按了两次
 * @param key_code
 *	要检测的按键的键值
 * @param interval_time
 *	该按键倒数第二次按下时的时间与当前时间的最大间隔
 */
LCUI_API LCUI_BOOL LCUIKey_IsDoubleHit( int key_code, int interval_time );

/** 添加已被按下的按键 */
LCUI_API void LCUIKeyBoard_HitKey( int key_code );

/** 标记指定键值的按键已释放 */
LCUI_API void LCUIKeyBoard_ReleaseKey( int key_code );

/** 添加键盘的按键按下事件 */
LCUI_API void LCUI_PostKeyDownEvent( int key_code );

/** 添加键盘的按键释放事件 */
LCUI_API void LCUI_PostKeyUpEvent( int key_code );

/** 检测键盘是否有按键按下（类似于kbhit函数） */
LCUI_API LCUI_BOOL LCUIKeyboard_IsHit( void );

/** 获取被按下的按键的键值（类似于getch函数） */
LCUI_API int LCUIKeyboard_GetKey( void );

/** 初始化键盘输入模块 */
LCUI_API void LCUIModule_Keyboard_Init( void );

/** 停用键盘输入模块 */
LCUI_API void LCUIModule_Keyboard_End( void );
/*------------------------------ Keyboard End ------------------------------*/


/*------------------------------ TouchScreen -------------------------------*/ 
/** 启用触屏输入处理 */
LCUI_API LCUI_BOOL EnableTouchScreenInput(void);

/** 禁用触屏输入处理 */
LCUI_API LCUI_BOOL DisableTouchScreenInput(void);

/** 获取触屏的私有数据 */
LCUI_API void* Get_TouchScreen(void);

/** 初始化触屏输入模块 */
LCUI_API void LCUIModule_TouchScreen_Init( void );
/*---------------------------- TouchScreen End -----------------------------*/ 

LCUI_END_HEADER

#endif /* __LCUI_INPUT_H__ */
