/* ***************************************************************************
 * LCUI_Input.h -- The input devices handling module of LCUI.
 * 
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_INPUT_H
#define LCUI_INPUT_H

#ifdef LCUI_BUILD_IN_WIN32
#include <winuser.h>

#define LCUIKEY_ESCAPE		VK_ESCAPE
#define LCUIKEY_ENTER		VK_RETURN
#define LCUIKEY_BACKSPACE	VK_BACK
#define LCUIKEY_DELETE		VK_DELETE
#define LCUIKEY_HOME		VK_HOME
#define LCUIKEY_END		VK_END
#define LCUIKEY_TAB		VK_TAB

#define LCUIKEY_DOWN		VK_DOWN
#define LCUIKEY_UP		VK_UP
#define LCUIKEY_LEFT		VK_LEFT
#define LCUIKEY_RIGHT		VK_RIGHT

#define LCUIKEY_CAPITAL		VK_CAPITAL
#define LCUIKEY_SHIFT		VK_SHIFT
#define LCUIKEY_CONTROL		VK_CONTROL

#else

#define LCUIKEY_BACKSPACE	8
#define LCUIKEY_TAB		9
#define LCUIKEY_ENTER		13
#define LCUIKEY_SHIFT		16
#define LCUIKEY_CONTROL		17
#define LCUIKEY_ALT		18
#define LCUIKEY_CAPITAL		20
#define LCUIKEY_ESCAPE		27
#define LCUIKEY_PAGEUP		33
#define LCUIKEY_PAGEDOWN	34
#define LCUIKEY_END		35
#define LCUIKEY_HOME		36
#define LCUIKEY_LEFT		37
#define LCUIKEY_UP		38
#define LCUIKEY_RIGHT		39
#define LCUIKEY_DOWN		40

#define LCUIKEY_INSERT		45
#define LCUIKEY_DELETE		46

#endif

#define LCUIKEY_SPACE		' '
#define LCUIKEY_COMMA		','
#define LCUIKEY_PERIOD		'.'
#define LCUIKEY_SLASH 		'/'
#define LCUIKEY_MINUS		'-'
#define LCUIKEY_EQUAL		'='
#define LCUIKEY_BRACKETLEFT	'['
#define LCUIKEY_BRACKETRIGHT	']'
#define LCUIKEY_BACKSLASH	'\\'
#define LCUIKEY_SEMICOLON	';'
#define LCUIKEY_APOSTROPHE	'\''
#define LCUIKEY_GRAVE		'`'
#define LCUIKEY_0		'0'
#define LCUIKEY_1		'1'
#define LCUIKEY_2		'2'
#define LCUIKEY_3		'3'
#define LCUIKEY_4		'4'
#define LCUIKEY_5		'5'
#define LCUIKEY_6		'6'
#define LCUIKEY_7		'7'
#define LCUIKEY_8		'8'
#define LCUIKEY_9		'9'

#define LCUIKEY_A		'a'
#define LCUIKEY_B		'b'
#define LCUIKEY_C		'c'
#define LCUIKEY_D		'd'
#define LCUIKEY_E		'e'
#define LCUIKEY_F		'f'
#define LCUIKEY_G		'g'
#define LCUIKEY_H		'h'
#define LCUIKEY_I		'i'
#define LCUIKEY_J		'j'
#define LCUIKEY_K		'k'
#define LCUIKEY_L		'l'
#define LCUIKEY_M		'm'
#define LCUIKEY_N		'n'
#define LCUIKEY_O		'o'
#define LCUIKEY_P		'p'
#define LCUIKEY_Q		'q'
#define LCUIKEY_R		'r'
#define LCUIKEY_S		's'
#define LCUIKEY_T		't'
#define LCUIKEY_U		'u'
#define LCUIKEY_V		'v'
#define LCUIKEY_W		'w'
#define LCUIKEY_X		'x'
#define LCUIKEY_Y		'y'
#define LCUIKEY_Z		'z'

#define LCUIKEY_LEFTBUTTON	1	/* 鼠标左键 */
#define LCUIKEY_RIGHTBUTTON	2	/* 鼠标右键 */
#define LCUIKEYSTATE_PRESSED	1
#define LCUIKEYSTATE_RELEASE	0

LCUI_BEGIN_HEADER

/** 检测指定键值的按键是否处于按下状态 */
LCUI_API LCUI_BOOL LCUIKeyboard_IsHit( int key_code );

/**
* 检测指定键值的按键是否按了两次
* @param key_code 要检测的按键的键值
* @param interval_time 该按键倒数第二次按下时的时间与当前时间的最大间隔
*/
LCUI_API LCUI_BOOL LCUIKeyboard_IsDoubleHit( int key_code, int interval_time );

/** 添加已被按下的按键 */
LCUI_API void LCUIKeyboard_HitKey( int key_code );

/** 标记指定键值的按键已释放 */
LCUI_API void LCUIKeyboard_ReleaseKey( int key_code );

void LCUI_InitKeyboard( void );

void LCUI_ExitKeyboard( void );

LCUI_END_HEADER

#endif /* LCUI_INPUT_H */
