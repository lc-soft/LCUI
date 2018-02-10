/*
 * input.h -- The input devices handling module of LCUI.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LCUI_INPUT_H
#define LCUI_INPUT_H

#define LCUIKEY_BACKSPACE	8
#define LCUIKEY_TAB		9
#define LCUIKEY_ENTER		13
#define LCUIKEY_SHIFT		16
#define LCUIKEY_CONTROL		17
#define LCUIKEY_ALT		18
#define LCUIKEY_CAPITAL		20
#define LCUIKEY_ESCAPE		27
#define LCUIKEY_SPACE		' '
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
#define LCUIKEY_A		'A'
#define LCUIKEY_B		'B'
#define LCUIKEY_C		'C'
#define LCUIKEY_D		'D'
#define LCUIKEY_E		'E'
#define LCUIKEY_F		'F'
#define LCUIKEY_G		'G'
#define LCUIKEY_H		'H'
#define LCUIKEY_I		'I'
#define LCUIKEY_J		'J'
#define LCUIKEY_K		'K'
#define LCUIKEY_L		'L'
#define LCUIKEY_M		'M'
#define LCUIKEY_N		'N'
#define LCUIKEY_O		'O'
#define LCUIKEY_P		'P'
#define LCUIKEY_Q		'Q'
#define LCUIKEY_R		'R'
#define LCUIKEY_S		'S'
#define LCUIKEY_T		'T'
#define LCUIKEY_U		'U'
#define LCUIKEY_V		'V'
#define LCUIKEY_W		'W'
#define LCUIKEY_X		'X'
#define LCUIKEY_Y		'Y'
#define LCUIKEY_Z		'Z'
#define LCUIKEY_SEMICOLON	186
#define LCUIKEY_EQUAL		187
#define LCUIKEY_COMMA		188
#define LCUIKEY_MINUS		189
#define LCUIKEY_PERIOD		190
#define LCUIKEY_SLASH 		191
#define LCUIKEY_GRAVE		192
#define LCUIKEY_BRACKETLEFT	219
#define LCUIKEY_BACKSLASH	220
#define LCUIKEY_BRACKETRIGHT	221
#define LCUIKEY_APOSTROPHE	222

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

void LCUI_FreeKeyboard( void );

LCUI_END_HEADER

#endif /* LCUI_INPUT_H */
