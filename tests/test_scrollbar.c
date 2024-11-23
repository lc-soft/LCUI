/*
 * tests/test_scrollbar.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#define PREVIEW_MODE
#include "./cases/test_scrollbar.c"
#include <ptk/main.h>

int main(int argc, char **argv)
{
	lcui_init();
	ui_widget_resize(ui_root(), 800, 640);
	ui_load_css_string(test_css, __FILE__);
	/* We have two ways to build content view */
	build_content_view_from_xml();
	build_content_view();
	return lcui_main();
}
