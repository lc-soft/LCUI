/*
 * ime.h -- Input Method Engine
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

#ifndef LCUI_IME_H
#define LCUI_IME_H

LCUI_BEGIN_HEADER

typedef struct LCUI_IMEHandlerRec_ {
	LCUI_BOOL (*prockey)(int, int);
	void (*totext)(int);
	LCUI_BOOL (*open)(void);
	LCUI_BOOL (*close)(void);
	void (*setcaret)(int, int);
} LCUI_IMEHandlerRec, *LCUI_IMEHandler;

/** 注册一个输入法 */
LCUI_API int LCUIIME_Register(const char *ime_name, LCUI_IMEHandler handler);

/** 选定输入法 */
LCUI_API LCUI_BOOL LCUIIME_Select(int ime_id);

LCUI_API LCUI_BOOL LCUIIME_SelectByName(const char *name);

/** 检测键值是否为字符键值 */
LCUI_API LCUI_BOOL LCUIIME_CheckCharKey(int key);

/** 切换至下一个输入法 */
LCUI_API void LCUIIME_Switch(void);

/** 检测输入法是否要处理按键事件 */
LCUI_API LCUI_BOOL LCUIIME_ProcessKey(LCUI_SysEvent e);

/** 提交输入法输入的内容至目标 */
LCUI_API int LCUIIME_Commit(const wchar_t *str, size_t len);

/* 初始化LCUI输入法模块 */
LCUI_API void LCUI_InitIME(void);

/* 停用LCUI输入法模块 */
LCUI_API void LCUI_FreeIME(void);

#ifdef LCUI_BUILD_IN_WIN32
int LCUI_RegisterWin32IME(void);
#else
int LCUI_RegisterLinuxIME(void);
#endif

LCUI_API void LCUIIME_SetCaret(int x, int y);

LCUI_END_HEADER

#endif
