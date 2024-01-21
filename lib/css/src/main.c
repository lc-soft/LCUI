/*
 * lib/css/src/main.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../include/css.h"

void css_init(void)
{
	css_init_value_definitons();
	css_init_data_types();
	css_init_library();
	css_init_keywords();
	css_init_properties();
}

void css_destroy(void)
{
	css_destroy_properties();
	css_destroy_keywords();
	css_destroy_library();
	css_destroy_value_definitons();
}
