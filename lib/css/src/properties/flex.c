/*
 * lib/css/src/properties/flex.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"
#include "./helpers.h"

static void set_flex_basis(css_computed_style_t *computed, uint8_t keyword,
			   css_numeric_value_t value, css_unit_t unit)
{
	computed->flex_basis = value;
	computed->unit_bits.flex_basis = unit;
	computed->type_bits.flex_basis = keyword;
}

int css_cascade_flex_basis(const css_style_array_value_t input,
			   css_computed_style_t *computed)
{
	return css_cascade_length_auto(input, computed, set_flex_basis);
}

int css_cascade_flex_direction(const css_style_array_value_t input,
			       css_computed_style_t *computed)
{
	uint8_t value = CSS_FLEX_DIRECTION_ROW;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_COLUMN:
		value = CSS_FLEX_DIRECTION_COLUMN;
	default:
		break;
	}
	computed->type_bits.flex_direction = value;
	return 0;
}

int css_cascade_flex_grow(const css_style_array_value_t input,
			  css_computed_style_t *computed)
{
	if (input[0].type != CSS_NUMERIC_VALUE) {
		return -1;
	}
	computed->flex_grow = input[0].numeric_value;
	if (computed->flex_grow < 0) {
		computed->flex_grow = 0;
	}
	computed->type_bits.flex_grow = CSS_FLEX_GROW_SET;
	return 0;
}

int css_cascade_flex_shrink(const css_style_array_value_t input,
			    css_computed_style_t *computed)
{
	if (input[0].type != CSS_NUMERIC_VALUE) {
		return -1;
	}
	computed->flex_shrink = input[0].numeric_value;
	if (computed->flex_shrink < 0) {
		computed->flex_shrink = 1;
	}
	computed->type_bits.flex_shrink = CSS_FLEX_SHRINK_SET;
	return 0;
}

int css_cascade_flex_wrap(const css_style_array_value_t input,
			  css_computed_style_t *computed)
{
	uint8_t value = CSS_FLEX_WRAP_NOWRAP;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_WRAP:
		value = CSS_FLEX_WRAP_WRAP;
	default:
		break;
	}
	computed->type_bits.flex_wrap = value;
	return 0;
}

static void css_parse_flex_1(const css_style_array_value_t input,
			     css_style_decl_t *s)
{
	css_style_value_t val[] = {
		{ .type = CSS_NUMERIC_VALUE, .numeric_value = 0 },
		{ .type = CSS_NUMERIC_VALUE, .numeric_value = 1 },
		{ .type = CSS_KEYWORD_VALUE, .keyword_value = CSS_KEYWORD_AUTO }
	};

	switch (input[0].type) {
	case CSS_NUMERIC_VALUE:
		// flex: <number> 1 0
		val[0].numeric_value = input[0].numeric_value;
		val[2].type = CSS_NUMERIC_VALUE;
		val[2].numeric_value = 0;
		break;
	case CSS_UNIT_VALUE:
		// flex: 1 1 <width>
		val[0].numeric_value = 1;
		val[2] = input[0];
		break;
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_AUTO:
			// flex: 1 1 auto
			val[0].numeric_value = 1;
			break;
		case CSS_KEYWORD_NONE:
			// flex: 0 0 auto
			val[0].numeric_value = 0;
			val[1].numeric_value = 0;
			break;
		case CSS_KEYWORD_INITIAL:
			// flex: 0 1 auto
			break;
		default:
			return;
		}
		break;
	default:
		return;
	}
	css_style_decl_add(s, css_prop_flex_grow, val);
	css_style_decl_add(s, css_prop_flex_shrink, val + 1);
	css_style_decl_add(s, css_prop_flex_basis, val + 2);
}

static void css_parse_flex_2(const css_style_array_value_t input,
			     css_style_decl_t *s)
{
	if (input[0].type != CSS_NUMERIC_VALUE) {
		return;
	}
	css_style_decl_add(s, css_prop_flex_grow, input);
	switch (input[1].type) {
	case CSS_NUMERIC_VALUE:
		css_style_decl_add(s, css_prop_flex_shrink, input + 1);
		break;
	case CSS_UNIT_VALUE:
		css_style_decl_add(s, css_prop_flex_basis, input + 1);
		break;
	default:
		break;
	}
}

static void css_parse_flex_3(const css_style_array_value_t input,
			     css_style_decl_t *s)
{
	int8_t i;
	int8_t has_flex_grow = 0;
	int8_t has_flex_shrink = 0;
	int8_t has_flex_basis = 0;

	for (i = 0; i < 3 && input[i].type != CSS_NO_VALUE; ++i) {
		if (!has_flex_grow && input[i].type == CSS_NUMERIC_VALUE) {
			css_style_decl_add(s, css_prop_flex_grow, input + i);
			has_flex_grow = 1;
		} else if (!has_flex_shrink &&
			   input[i].type == CSS_NUMERIC_VALUE) {
			css_style_decl_add(s, css_prop_flex_shrink, input + i);
			has_flex_shrink = 1;
		} else if (!has_flex_basis &&
			   (input[i].type == CSS_KEYWORD_VALUE ||
			    input[i].type == CSS_UNIT_VALUE)) {
			css_style_decl_add(s, css_prop_flex_basis, input + i);
			has_flex_basis = 1;
		} else {
			break;
		}
	}
}

int css_parse_flex(css_propdef_t *propdef, const char *input,
		   css_style_decl_t *s)
{
	css_style_value_t value;
	int len = css_parse_value(propdef->valdef, input, &value);

	if (len <= 0) {
		return len;
	}
	switch (css_style_value_get_array_length(&value)) {
	case 1:
		css_parse_flex_1(value.array_value, s);
		break;
	case 2:
		css_parse_flex_2(value.array_value, s);
		break;
	case 3:
		css_parse_flex_3(value.array_value, s);
		break;
	default:
		break;
	}
	css_style_value_destroy(&value);
	return len;
}
