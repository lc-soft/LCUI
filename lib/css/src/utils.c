/* parse.c -- Parse data from string
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/css/utils.h"

LCUI_BOOL css_parse_font_weight(const char *str, int *weight)
{
	int value;
	// TODO
	if (strcmp(str, "normal") == 0) {
		*weight = CSS_FONT_WEIGHT_NORMAL;
		return TRUE;
	}
	if (strcmp(str, "bold") == 0) {
		*weight = CSS_FONT_WEIGHT_BOLD;
		return TRUE;
	}
	if (sscanf(str, "%d", &value) != 1) {
		*weight = CSS_FONT_WEIGHT_NORMAL;
		return FALSE;
	}
	if (value < 100) {
		*weight = CSS_FONT_WEIGHT_100;
		return TRUE;
	}
	*weight = y_iround(value / 100.0) * 100;
	return TRUE;
}

LCUI_BOOL css_parse_font_style(const char *str, int *style)
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
		return FALSE;
	}
	return TRUE;
}
