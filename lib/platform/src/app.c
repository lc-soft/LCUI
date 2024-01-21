/*
 * lib/platform/src/app.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "app.h"
#include "ime.h"
#include "events.h"
#include "clipboard.h"

int app_init(const wchar_t *name)
{
	if (app_init_engine(name) != 0) {
		return -1;
	}
	app_init_ime();
	app_init_events();
	clipboard_init();
	return 0;
}

void app_destroy(void)
{
	clipboard_destroy();
	app_destroy_events();
	app_destroy_ime();
	app_destroy_engine();
}
