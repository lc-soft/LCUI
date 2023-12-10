/*
 * lib/css/src/data_types.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <css/value.h>
#include <css/data_types.h>
#include <css/style_value.h>
#include <css/utils.h>
#include "parser.h"

libcss_bool_t css_parse_numeric_value(css_style_value_t *s, const char *str)
{
	s->type = CSS_NUMERIC_VALUE;
	return sscanf(str, "%f", &s->numeric_value) == 1;
}

libcss_bool_t css_parse_integer_value(css_style_value_t *s, const char *str)
{
	int num;
	s->type = CSS_NUMERIC_VALUE;
	if (sscanf(str, "%d", &num) == 1) {
		s->numeric_value = 1.0f * num;
		return 1;
	}
	return 0;
}

libcss_bool_t css_parse_unit_value(css_style_value_t *s, const char *str)
{
	int n = 0;
	const char *p;
	char num_str[32] = { 0 };
	char unit[4];
	css_numeric_value_t num;
	libcss_bool_t has_point = LIBCSS_FALSE;

	if (str == NULL) {
		return LIBCSS_FALSE;
	}
	/* 先取出数值 */
	for (p = str; *p && n < 30; ++p) {
		if (*p >= '0' && *p <= '9')
			;
		else if (*p == '-' || *p == '+') {
			if (n > 0) {
				n = 0;
				break;
			}
		} else if (*p == '.') {
			if (has_point) {
				n = 0;
				break;
			}
			has_point = LIBCSS_TRUE;
		} else {
			break;
		}
		num_str[n++] = *p;
	}
	if (n == 0) {
		return LIBCSS_FALSE;
	}
	strncpy(unit, p, 3);
	unit[3] = 0;
	sscanf(num_str, "%f", &num);
	if (strncmp(unit, "%", 4) == 0) {
		s->unit_value.unit = CSS_UNIT_PERCENT;
	} else if (strncmp(unit, "px", 4) == 0 || num == 0) {
		s->unit_value.unit = CSS_UNIT_PX;
	} else if (strncmp(unit, "dp", 4) == 0) {
		s->unit_value.unit = CSS_UNIT_DIP;
	} else if (strncmp(unit, "sp", 4) == 0) {
		s->unit_value.unit = CSS_UNIT_SP;
	} else if (strncmp(unit, "pt", 4) == 0) {
		s->unit_value.unit = CSS_UNIT_PT;
	} else {
		return LIBCSS_FALSE;
	}
	num_str[n] = 0;
	s->type = CSS_UNIT_VALUE;
	s->unit_value.value = num;
	return LIBCSS_TRUE;
}

libcss_bool_t css_parse_percentage_value(css_style_value_t *s, const char *str)
{
	if (css_parse_unit_value(s, str) &&
	    s->unit_value.unit == CSS_UNIT_PERCENT) {
		return LIBCSS_TRUE;
	}
	s->type = CSS_INVALID_VALUE;
	return LIBCSS_FALSE;
}

libcss_bool_t css_parse_length_value(css_style_value_t *s, const char *str)
{
	if (css_parse_unit_value(s, str) &&
	    s->unit_value.unit != CSS_UNIT_PERCENT) {
		return LIBCSS_TRUE;
	}
	s->type = CSS_INVALID_VALUE;
	return LIBCSS_FALSE;
}

libcss_bool_t css_parse_string_value(css_style_value_t *val, const char *str)
{
	size_t len;

	len = strlen(str);
	val->type = CSS_STRING_VALUE;
	if (len > 0) {
		if (str[0] == '"' && str[len - 1] == '"') {
			val->string_value = strdup2(str + 1);
			val->string_value[len - 2] = 0;
		} else {
			val->string_value = strdup2(str);
		}
	} else {
		val->string_value = NULL;
	}
	return LIBCSS_TRUE;
}

libcss_bool_t css_parse_url_value(css_style_value_t *s, const char *str)
{
	size_t n;
	const char *p, *head, *tail;

	p = str;
	tail = head = strstr(p, "url(");
	if (!head) {
		return LIBCSS_FALSE;
	}
	while (p) {
		tail = p;
		p = strstr(p + 1, ")");
	}
	if (tail == head) {
		return LIBCSS_FALSE;
	}
	head += 4;
	if (*head == '"') {
		++head;
	}
	n = tail - head;
	s->string_value = malloc((n + 1) * sizeof(char));
	if (!s->string_value) {
		return LIBCSS_FALSE;
	}
	strncpy(s->string_value, head, n);
	s->string_value[n] = 0;
	if (n > 0 && s->string_value[n - 1] == '"') {
		n -= 1;
		s->string_value[n] = 0;
	}
	s->type = CSS_STRING_VALUE;
	return LIBCSS_TRUE;
}

static libcss_bool_t css_parse_rgba(css_style_value_t *val, const char *str)
{
	float data[4];
	char num_str[16];
	int i, j, count;

	if (!strstr(str, "rgba(")) {
		return LIBCSS_FALSE;
	}
	for (count = 0, i = 5, j = 0; str[i]; ++i) {
		if (str[i] == ',' || str[i] == ')') {
			if (sscanf(num_str, "%f", &data[count++]) != 1) {
				return LIBCSS_FALSE;
			}
			j = 0;
			if (str[i] == ')') {
				break;
			}
		} else if ((str[i] == '.' ||
			    (str[i] >= '0' && str[i] <= '9')) &&
			   j < 15) {
			num_str[j++] = str[i];
			num_str[j] = 0;
		}
	}
	if (count != 4) {
		return LIBCSS_FALSE;
	}
	val->type = CSS_COLOR_VALUE;
	val->color_value = css_color((uint8_t)(255 * data[3]), (uint8_t)data[0],
				     (uint8_t)data[1], (uint8_t)data[2]);
	return LIBCSS_TRUE;
}

static libcss_bool_t css_parse_rgb(css_style_value_t *val, const char *str)
{
	float data[3];
	char num_str[16];
	int i, j, count;

	if (!strstr(str, "rgb(")) {
		return LIBCSS_FALSE;
	}
	for (count = 0, i = 4, j = 0; str[i]; ++i) {
		if (str[i] == ',' || str[i] == ')') {
			if (sscanf(num_str, "%f", &data[count++]) != 1) {
				return LIBCSS_FALSE;
			}
			j = 0;
			if (str[i] == ')') {
				break;
			}
		} else if ((str[i] == '.' ||
			    (str[i] >= '0' && str[i] <= '9')) &&
			   j < 15) {
			num_str[j++] = str[i];
			num_str[j] = 0;
		}
	}
	if (count != 3) {
		return LIBCSS_FALSE;
	}
	val->type = CSS_COLOR_VALUE;
	val->color_value = css_color(255, (uint8_t)data[0], (uint8_t)data[1],
				     (uint8_t)data[2]);
	return LIBCSS_TRUE;
}

libcss_bool_t css_parse_color_value(css_style_value_t *val, const char *str)
{
	const char *p;
	int len = 0, status = 0, r, g, b;
	for (p = str; *p; ++p, ++len) {
		switch (*p) {
		case '#':
			len == 0 ? status = 3 : 0;
			break;
		case 'r':
			status == 0 ? status = 1 : 0;
			break;
		case 'g':
			status == 1 ? status <<= 1 : 0;
			break;
		case 'b':
			status == 2 ? status <<= 1 : 0;
			break;
		case 'a':
			status == 4 ? status <<= 1 : 0;
			break;
		default:
			if (status < 3) {
				break;
			}
		}
	}
	switch (status) {
	case 3:
		status = 0;
		if (len == 4) {
			status = sscanf(str, "#%1X%1X%1X", &r, &g, &b);
			r *= 255 / 0xf;
			g *= 255 / 0xf;
			b *= 255 / 0xf;
		} else if (len == 7) {
			status = sscanf(str, "#%2X%2X%2X", &r, &g, &b);
		}
		break;
	case 4:
		return css_parse_rgb(val, str);
	case 8:
		return css_parse_rgba(val, str);
	default:
		break;
	}
	if (status == 3) {
		val->type = CSS_COLOR_VALUE;
		val->color_value = css_color(0xff, r, g, b);
		return LIBCSS_TRUE;
	}
	if (strcmp("transparent", str) == 0) {
		val->type = CSS_COLOR_VALUE;
		val->color_value = CSS_COLOR_TRANSPARENT;
		return LIBCSS_TRUE;
	}
	return LIBCSS_FALSE;
}

libcss_bool_t css_parse_font_family_value(css_style_value_t *val, const char *str)
{
	char name[256];
	const char *p;
	unsigned quotes = 0;
	unsigned count, i;
	uint8_t finished = 0;

	val->type = CSS_ARRAY_VALUE;
	val->array_value = NULL;
	for (i = 0, p = str, count = 0; 1; ++p) {
		switch (*p) {
		case '"':
		case '\'':
			if (i > 0) {
				if (quotes < 1) {
					return LIBCSS_FALSE;
				}
				quotes--;
				if (quotes < 1) {
					finished = 1;
				}
			} else {
				quotes++;
			}
			break;
		case ',':
			if (quotes > 0) {
				goto save;
			}
			if (i < 1) {
				return LIBCSS_FALSE;
			}
			goto append;
		CASE_WHITE_SPACE:
			if (!finished && i > 0) {
				goto save;
			}
			continue;
		case 0:
			if (i > 0) {
				goto append;
			}
			break;
		default:
			if (finished) {
				return LIBCSS_FALSE;
			}
			break;
		}
		if (!*p) {
			break;
		}
	save:
		if (i < sizeof(name) / sizeof(char)) {
			name[i++] = *p;
			name[i] = 0;
		}
		continue;
	append:
		css_style_value_set_array_length(val, count + 1);
		val->array_value[count].type = CSS_STRING_VALUE;
		val->array_value[count].string_value = strdup2(name);
		count++;
		if (!*p) {
			break;
		}
	}
	return LIBCSS_TRUE;
}

void css_init_data_types(void)
{
	css_register_value_type("length", css_parse_length_value);
	css_register_value_type("percentage", css_parse_percentage_value);
	css_register_value_type("color", css_parse_color_value);
	css_register_value_type("string", css_parse_string_value);
	css_register_value_type("number", css_parse_numeric_value);
	css_register_value_type("integer", css_parse_integer_value);
	/** FIXME: 处理相对路径 */
	css_register_value_type("url", css_parse_url_value);
	css_register_value_type("font-family", css_parse_font_family_value);
}
