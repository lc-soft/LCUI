/*
 * lib/ptk/include/ptk/clipboard.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_CLIPBOARD_H
#define PTK_INCLUDE_PLATFORM_CLIPBOARD_H

#include "types.h"
#include "common.h"

PTK_BEGIN_DECLS

/**
 * Clipboard
 * References: https://docs.gtk.org/gtk3/class.Clipboard.html
 */

typedef struct ptk_clipboard {
	pd_canvas_t *image;
	wchar_t *text;
	size_t len;
} ptk_clipboard_t;

typedef void (*ptk_clipboard_callback_t)(ptk_clipboard_t *, void *);

PTK_PUBLIC int ptk_clipboard_request_text(ptk_clipboard_callback_t action, void *arg);
PTK_PUBLIC int ptk_clipboard_set_text(const wchar_t *text, size_t len);

PTK_END_DECLS

#endif
