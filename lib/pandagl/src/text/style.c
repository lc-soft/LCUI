/*
 * lib/pandagl/src/text/style.c: -- text style processing module.
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pandagl.h>

void pd_text_style_init(pd_text_style_t *data)
{
	data->has_style = PD_FALSE;
	data->has_weight = PD_FALSE;
	data->has_family = PD_FALSE;
	data->has_pixel_size = PD_FALSE;
	data->has_back_color = PD_FALSE;
	data->has_fore_color = PD_FALSE;
	data->font_ids = NULL;
	data->style = PD_FONT_STYLE_NORMAL;
	data->weight = PD_FONT_WEIGHT_NORMAL;
	data->fore_color.value = 0xff333333;
	data->back_color.value = 0xffffffff;
	data->pixel_size = 13;
}

int pd_text_style_copy_family(pd_text_style_t *dst, pd_text_style_t *src)
{
	size_t len;

	if (!src->font_ids) {
		return 0;
	}
	for (len = 0; src->font_ids[len]; ++len)
		;
	len += 1;
	if (dst->font_ids) {
		free(dst->font_ids);
	}
	dst->font_ids = malloc(len * sizeof(int));
	if (!dst->font_ids) {
		return -ENOMEM;
	}
	dst->has_family = PD_TRUE;
	memcpy(dst->font_ids, src->font_ids, len * sizeof(int));
	return 0;
}

int pd_text_style_copy(pd_text_style_t *dst, pd_text_style_t *src)
{
	*dst = *src;
	dst->font_ids = NULL;
	return pd_text_style_copy_family(dst, src);
}

void pd_text_style_destroy(pd_text_style_t *data)
{
	if (data->font_ids) {
		free(data->font_ids);
	}
	data->font_ids = NULL;
}

void pd_text_style_merge(pd_text_style_t *base, pd_text_style_t *target)
{
	int *font_ids = NULL;
	base->has_family = PD_TRUE;
	pd_text_style_copy_family(base, target);
	if (target->has_style && !base->has_style &&
	    target->style != PD_FONT_STYLE_NORMAL) {
		base->has_style = PD_TRUE;
		base->style = target->style;
	}
	if (pd_font_library_update_font_style(base->font_ids, base->style,
					      &font_ids) > 0) {
		free(base->font_ids);
		base->font_ids = font_ids;
	}
	if (target->has_weight && !base->has_weight &&
	    target->weight != PD_FONT_WEIGHT_NORMAL) {
		base->has_weight = PD_TRUE;
		base->weight = target->weight;
	}
	if (pd_font_library_update_font_weight(base->font_ids, base->weight,
					       &font_ids) > 0) {
		free(base->font_ids);
		base->font_ids = font_ids;
	}
}

void pd_text_style_set_size(pd_text_style_t *ts, int pixel_size)
{
	ts->pixel_size = pixel_size;
	ts->has_pixel_size = PD_TRUE;
}

void pd_text_style_set_fore_color(pd_text_style_t *ts, pd_color_t color)
{
	ts->fore_color = color;
	ts->has_fore_color = PD_TRUE;
}

void pd_text_style_set_back_color(pd_text_style_t *ts, pd_color_t color)
{
	ts->back_color = color;
	ts->has_back_color = PD_TRUE;
}

int pd_text_style_set_weight(pd_text_style_t *ts, pd_font_weight_t weight)
{
	int *font_ids;
	ts->weight = weight;
	ts->has_weight = PD_TRUE;
	if (pd_font_library_update_font_weight(ts->font_ids, weight,
					       &font_ids) > 0) {
		free(ts->font_ids);
		ts->font_ids = font_ids;
		return 0;
	}
	return -1;
}

int pd_text_style_set_style(pd_text_style_t *ts, pd_font_style_t style)
{
	int *font_ids;
	ts->style = style;
	ts->has_style = PD_TRUE;
	if (pd_font_library_update_font_style(ts->font_ids, style, &font_ids) >
	    0) {
		free(ts->font_ids);
		ts->font_ids = font_ids;
		return 0;
	}
	return -1;
}

int pd_text_style_set_font(pd_text_style_t *ts, const char *const *names)
{
	size_t count;

	if (ts->has_family && ts->font_ids) {
		free(ts->font_ids);
	}
	ts->font_ids = NULL;
	ts->has_family = PD_FALSE;
	count =
	    pd_font_library_query(&ts->font_ids, ts->style, ts->weight, names);
	if (count > 0) {
		ts->has_family = PD_TRUE;
		return 0;
	}
	return -1;
}

int pd_text_style_set_default_font(pd_text_style_t *ts)
{
	if (ts->has_family && ts->font_ids) {
		free(ts->font_ids);
		ts->has_family = PD_FALSE;
	}
	ts->font_ids = malloc(sizeof(int) * 2);
	if (!ts->font_ids) {
		ts->font_ids = NULL;
		return -ENOMEM;
	}
	ts->has_family = PD_TRUE;
	ts->font_ids[0] = pd_font_library_get_default_font();
	ts->font_ids[1] = 0;
	return 0;
}
