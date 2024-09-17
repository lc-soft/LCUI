/*
 * tests/test_box_shadow.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ui_xml.h>
#include <ptk/main.h>

int main(void)
{
	int ret = 0;
	ui_widget_t *box;

	lcui_init();
	box = ui_load_xml_file("test_box_shadow.xml");
	if (!box) {
		lcui_destroy();
		return ret;
	}
	ui_root_append(box);
	ui_widget_unwrap(box);
	return lcui_main();
}
