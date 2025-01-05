/*
 * lib/ptk/src/linux/x11app.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <X11/Xlib.h>
#include "ptk/app.h"

Display *ptk_x11_get_display(void);
Window ptk_x11_get_main_window(void);

void ptk_x11app_driver_init(ptk_app_driver_t *driver);
void ptk_x11window_driver_init(ptk_window_driver_t *driver);
