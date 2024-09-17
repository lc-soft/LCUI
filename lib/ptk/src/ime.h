/*
 * lib/ptk/src/ime.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk/ime.h"

int ptk_ime_add_win32(void);
int ptk_ime_add_linux(void);
void ptk_ime_destroy(void);
void ptk_ime_init(void);
