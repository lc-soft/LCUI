/*
 * lib/ptk/src/linux/x11clipboard.h
 *
 * Copyright (c) 2024-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk/clipboard.h"

int ptk_x11clipboard_request_text(ptk_clipboard_callback_t callback, void *arg);
int ptk_x11clipboard_set_text(const wchar_t *text, size_t len);
void ptk_x11clipboard_init(void);
void ptk_x11clipboard_destroy(void);
