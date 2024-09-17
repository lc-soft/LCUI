/*
 * tests/test_text_resize.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#define PREVIEW_MODE
#include "./cases/test_text_resize.c"
#include <ptk/main.h>

int main(int argc, char *argv[])
{
	lcui_init();

	build();
	lcui_set_timeout(2000, test_text_set_content, NULL);
	lcui_set_timeout(3000, test_text_set_short_content_css, NULL);
	lcui_set_timeout(4000, test_text_set_long_content_css, NULL);

	return lcui_main();
}
