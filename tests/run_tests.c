/*
 * tests/run_tests.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <wchar.h>
#include <LCUI.h>
#include <ptk/main.h>
#include <ctest-custom.h>
#include "run_tests.h"

int main(int argc, char **argv)
{
	logger_set_level(LOGGER_LEVEL_ALL);
	ctest_describe("test settings", test_settings);
	ctest_describe("test thread", test_thread);
	ctest_describe("test font load", test_font_load);
	ctest_describe("test image reader", test_image_reader);
	ctest_describe("test xml parser", test_xml_parser);
	ctest_describe("test mainloop", test_mainloop);
	ctest_describe("test clipboard", test_clipboard);
	ctest_describe("test widget event", test_widget_event);
	ctest_describe("test widget opacity", test_widget_opacity);
	ctest_describe("test text resize", test_text_resize);
	ctest_describe("test textinput", test_textinput);
	ctest_describe("test scrollbar", test_scrollbar);
	ctest_describe("test widget rect", test_widget_rect);
	ctest_describe("test block layout", test_block_layout);
	ctest_describe("test flex layout", test_flex_layout);
	return ctest_finish();
}
