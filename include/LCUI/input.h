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

/* clang-format off */
#define LCUI_KEY_BACKSPACE	8
#define LCUI_KEY_TAB		9
#define LCUI_KEY_ENTER		13
#define LCUI_KEY_SHIFT		16
#define LCUI_KEY_CONTROL	17
#define LCUI_KEY_ALT		18
#define LCUI_KEY_CAPITAL	20
#define LCUI_KEY_ESCAPE		27
#define LCUI_KEY_SPACE		' '
#define LCUI_KEY_PAGEUP		33
#define LCUI_KEY_PAGEDOWN	34
#define LCUI_KEY_END		35
#define LCUI_KEY_HOME		36
#define LCUI_KEY_LEFT		37
#define LCUI_KEY_UP		38
#define LCUI_KEY_RIGHT		39
#define LCUI_KEY_DOWN		40
#define LCUI_KEY_INSERT		45
#define LCUI_KEY_DELETE		46
#define LCUI_KEY_0		'0'
#define LCUI_KEY_1		'1'
#define LCUI_KEY_2		'2'
#define LCUI_KEY_3		'3'
#define LCUI_KEY_4		'4'
#define LCUI_KEY_5		'5'
#define LCUI_KEY_6		'6'
#define LCUI_KEY_7		'7'
#define LCUI_KEY_8		'8'
#define LCUI_KEY_9		'9'
#define LCUI_KEY_A		'A'
#define LCUI_KEY_B		'B'
#define LCUI_KEY_C		'C'
#define LCUI_KEY_D		'D'
#define LCUI_KEY_E		'E'
#define LCUI_KEY_F		'F'
#define LCUI_KEY_G		'G'
#define LCUI_KEY_H		'H'
#define LCUI_KEY_I		'I'
#define LCUI_KEY_J		'J'
#define LCUI_KEY_K		'K'
#define LCUI_KEY_L		'L'
#define LCUI_KEY_M		'M'
#define LCUI_KEY_N		'N'
#define LCUI_KEY_O		'O'
#define LCUI_KEY_P		'P'
#define LCUI_KEY_Q		'Q'
#define LCUI_KEY_R		'R'
#define LCUI_KEY_S		'S'
#define LCUI_KEY_T		'T'
#define LCUI_KEY_U		'U'
#define LCUI_KEY_V		'V'
#define LCUI_KEY_W		'W'
#define LCUI_KEY_X		'X'
#define LCUI_KEY_Y		'Y'
#define LCUI_KEY_Z		'Z'
#define LCUI_KEY_SEMICOLON	186
#define LCUI_KEY_EQUAL		187
#define LCUI_KEY_COMMA		188
#define LCUI_KEY_MINUS		189
#define LCUI_KEY_PERIOD		190
#define LCUI_KEY_SLASH 		191
#define LCUI_KEY_GRAVE		192
#define LCUI_KEY_BRACKETLEFT	219
#define LCUI_KEY_BACKSLASH	220
#define LCUI_KEY_BRACKETRIGHT	221
#define LCUI_KEY_APOSTROPHE	222
#define LCUI_KEY_LEFTBUTTON	1	/* 鼠标左键 */
#define LCUI_KEY_RIGHTBUTTON	2	/* 鼠标右键 */
#define LCUI_KSTATE_PRESSED	1
#define LCUI_KSTATE_RELEASE	0

/* clang-format on */

LCUI_BEGIN_HEADER

/** 检测指定键值的按键是否处于按下状态 */
LCUI_API LCUI_BOOL LCUIKeyboard_IsHit(int key_code);

/**
 * 检测指定键值的按键是否按了两次
 * @param key_code 要检测的按键的键值
 * @param interval_time 该按键倒数第二次按下时的时间与当前时间的最大间隔
 */
LCUI_API LCUI_BOOL LCUIKeyboard_IsDoubleHit(int key_code, int interval_time);

/** 添加已被按下的按键 */
LCUI_API void LCUIKeyboard_HitKey(int key_code);

/** 标记指定键值的按键已释放 */
LCUI_API void LCUIKeyboard_ReleaseKey(int key_code);

void LCUI_InitKeyboard(void);

void LCUI_FreeKeyboard(void);

LCUI_END_HEADER

#endif /* LCUI_INPUT_H */
