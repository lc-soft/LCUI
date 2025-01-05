/*
 * lib/css/src/utils.c: -- Parse data from string
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <css/utils.h>

bool css_parse_font_weight(const char *str, int *weight)
{
	int value;
	// TODO
	if (strcmp(str, "normal") == 0) {
		*weight = CSS_FONT_WEIGHT_NORMAL;
		return true;
	}
	if (strcmp(str, "bold") == 0) {
		*weight = CSS_FONT_WEIGHT_BOLD;
		return true;
	}
	if (sscanf(str, "%d", &value) != 1) {
		*weight = CSS_FONT_WEIGHT_NORMAL;
		return false;
	}
	if (value < 100) {
		*weight = CSS_FONT_WEIGHT_100;
		return true;
	}
	*weight = y_iround(value / 100.0) * 100;
	return true;
}

bool css_parse_font_style(const char *str, int *style)
{
	char value[64] = "";
	// TODO
	strtrim(value, str, NULL);
	if (strcmp(value, "normal") == 0) {
		*style = CSS_FONT_STYLE_NORMAL;
	} else if (strcmp(value, "italic") == 0) {
		*style = CSS_FONT_STYLE_ITALIC;
	} else if (strcmp(value, "oblique") == 0) {
		*style = CSS_FONT_STYLE_OBLIQUE;
	} else {
		return false;
	}
	return true;
}
