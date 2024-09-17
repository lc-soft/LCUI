/*
 * lib/ptk/include/ptk/ime.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_IME_H
#define PTK_INCLUDE_PLATFORM_IME_H

#include "types.h"
#include "common.h"

PTK_BEGIN_DECLS

typedef struct ptk_ime_handler_t {
	bool (*prockey)(int, bool);
	void (*totext)(int);
	bool (*open)(void);
	bool (*close)(void);
	void (*setcaret)(int, int);
} ptk_ime_handler_t;

PTK_PUBLIC int ptk_ime_add(const char *ime_name, ptk_ime_handler_t *handler);

/** 选定输入法 */
PTK_PUBLIC bool ptk_ime_select(int ptk_ime_id);

PTK_PUBLIC bool ptk_ime_select_by_name(const char *name);

/** 检测键值是否为字符键值 */
PTK_PUBLIC bool ptk_ime_check_char_key(int key);

/** 切换至下一个输入法 */
PTK_PUBLIC void ptk_ime_switch(void);

/** 检测输入法是否要处理按键事件 */
PTK_PUBLIC bool ptk_ime_process_key(ptk_event_t *e);

/** 提交输入法输入的内容至目标 */
PTK_PUBLIC int ptk_ime_commit(const wchar_t *str, size_t len);

PTK_PUBLIC void ptk_ime_set_caret(int x, int y);

PTK_END_DECLS

#endif
