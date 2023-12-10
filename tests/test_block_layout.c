/*
 * tests/test_block_layout.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#define PREVIEW_MODE
#include "./cases/test_block_layout.c"
#include <platform/main.h>

int main(int argc, char **argv)
{
	logger_set_level(LOGGER_LEVEL_ERROR);
	test_block_layout();
	return lcui_main();
}
