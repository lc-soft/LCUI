/*
 * tests/test_border.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <pandagl.h>
#include <ui_xml.h>
#include <platform/main.h>

int main(void)
{
	int ret = 0;
	ui_widget_t *root, *box;

	lcui_init();
	box = ui_load_xml_file("test_border.xml");
	if (!box) {
		lcui_destroy();
		return ret;
	}
	root = ui_root();
	ui_widget_append(root, box);
	ui_widget_unwrap(box);
	return lcui_main();
}
