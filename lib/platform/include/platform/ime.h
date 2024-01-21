/*
 * lib/platform/include/platform/ime.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBPLAT_INCLUDE_PLATFORM_IME_H
#define LIBPLAT_INCLUDE_PLATFORM_IME_H

#include "types.h"
#include "common.h"

LIBPLAT_BEGIN_DECLS

typedef struct ime_handler_t {
	bool (*prockey)(int, bool);
	void (*totext)(int);
	bool (*open)(void);
	bool (*close)(void);
	void (*setcaret)(int, int);
} ime_handler_t;

LIBPLAT_PUBLIC int ime_add(const char *ime_name, ime_handler_t *handler);

/** 选定输入法 */
LIBPLAT_PUBLIC bool ime_select(int ime_id);

LIBPLAT_PUBLIC bool ime_select_by_name(const char *name);

/** 检测键值是否为字符键值 */
LIBPLAT_PUBLIC bool ime_check_char_key(int key);

/** 切换至下一个输入法 */
LIBPLAT_PUBLIC void ime_switch(void);

/** 检测输入法是否要处理按键事件 */
LIBPLAT_PUBLIC bool ime_process_key(app_event_t *e);

/** 提交输入法输入的内容至目标 */
LIBPLAT_PUBLIC int ime_commit(const wchar_t *str, size_t len);

LIBPLAT_PUBLIC void ime_set_caret(int x, int y);

/* 初始化LCUI输入法模块 */
LIBPLAT_PUBLIC void app_init_ime(void);

/* 停用LCUI输入法模块 */
LIBPLAT_PUBLIC void app_destroy_ime(void);

LIBPLAT_END_DECLS

#endif
