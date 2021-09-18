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
