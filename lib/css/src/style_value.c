#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <css/keywords.h>
#include <css/value.h>
#include <css/library.h>
#include <css/properties.h>
#include "./dump.h"

unsigned css_array_value_get_length(const css_style_array_value_t val)
{
	unsigned len;

	if (!val) {
		return 0;
	}
	for (len = 0; val[len].type != CSS_NO_VALUE; ++len)
		;
	return len;
}

int css_array_value_set_length(css_style_array_value_t *arr, unsigned new_len)
{
	unsigned len;
	css_style_array_value_t new_arr;

	len = css_array_value_get_length(*arr);
	new_arr = realloc(*arr, (new_len + 1) * sizeof(css_style_value_t));
	if (!new_arr) {
		return -1;
	}
	for (; len < new_len; ++len) {
		new_arr[len].type = CSS_INVALID_VALUE;
	}
	new_arr[new_len].type = CSS_NO_VALUE;
	*arr = new_arr;
	return 0;
}

unsigned css_array_value_concat(css_style_array_value_t *arr1,
				css_style_array_value_t *arr2)
{
	unsigned len1 = css_array_value_get_length(*arr1);
	unsigned len2 = css_array_value_get_length(*arr2);
	unsigned new_len = len1 + len2;

	if (css_array_value_set_length(arr1, new_len) != 0) {
		return len1;
	}
	for (len2 = 0; len1 < new_len + 1; ++len1, ++len2) {
		(*arr1)[len1] = (*arr2)[len2];
	}
	free(*arr2);
	*arr2 = NULL;
	return new_len;
}

css_style_array_value_t css_array_value_duplicate(
    const css_style_array_value_t arr)
{
	unsigned i, len;
	css_style_array_value_t new_arr;

	len = css_array_value_get_length(arr);
	new_arr = malloc(sizeof(css_style_value_t) * (len + 1));
	if (!new_arr) {
		return NULL;
	}
	for (i = 0; i <= len; ++i) {
		css_style_value_copy(new_arr + i, arr + i);
	}
	return new_arr;
}

void css_array_value_destroy(css_style_array_value_t val)
{
	unsigned i;

	if (!val) {
		return;
	}
	for (i = 0; val[i].type != CSS_NO_VALUE; ++i) {
		css_style_value_destroy(val + i);
	}
	free(val);
}

void css_style_value_destroy(css_style_value_t *val)
{
	switch (val->type) {
	case CSS_ARRAY_VALUE:
		css_array_value_destroy(val->array_value);
		val->array_value = NULL;
		break;
	case CSS_UNPARSED_VALUE:
		free(val->unparsed_value);
		break;
	case CSS_STRING_VALUE:
		free(val->string_value);
		val->string_value = NULL;
		break;
	default:
		break;
	}
	val->type = CSS_NO_VALUE;
}

unsigned css_style_value_get_array_length(const css_style_value_t *val)
{
	return css_array_value_get_length(val->array_value);
}

int css_style_value_set_array_length(css_style_value_t *val, unsigned new_len)
{
	return css_array_value_set_length(&val->array_value, new_len);
}

void css_style_value_copy(css_style_value_t *dst, const css_style_value_t *src)
{
	switch (src->type) {
	case CSS_STRING_VALUE:
	case CSS_UNPARSED_VALUE:
		if (src->string_value) {
			dst->string_value = strdup2(src->string_value);
		} else {
			dst->string_value = NULL;
		}
		break;
	case CSS_ARRAY_VALUE:
		dst->array_value = css_array_value_duplicate(src->array_value);
		break;
	default:
		*dst = *src;
		break;
	}
	dst->type = src->type;
}

size_t css_style_value_concat_array(css_style_value_t *val1,
				    css_style_value_t *val2)
{
	assert(val1->type == CSS_ARRAY_VALUE && val2->type == CSS_ARRAY_VALUE);
	return css_array_value_concat(&val1->array_value, &val2->array_value);
}

// https://developer.mozilla.org/en-US/docs/Web/API/CSSStyleValue/parse

css_style_value_t *css_style_value_parse(const char *property,
					 const char *css_text)
{
	const css_propdef_t *propdef;
	css_style_value_t *value;

	propdef = css_get_propdef_by_name(property);
	if (!propdef) {
		return NULL;
	}
	value = malloc(sizeof(css_style_value_t));
	value->type = CSS_INVALID_VALUE;
	if (css_parse_value(propdef->valdef, css_text, value) > 0) {
		return value;
	}
	free(value);
	return NULL;
}

void css_dump_style_value(const css_style_value_t *s, css_dump_context_t *ctx)
{
	const char *unit;
	union {
		uint32_t value;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
	} color;

	switch (s->type) {
	case CSS_INVALID_VALUE:
		DUMP("<invalid value>");
		break;
	case CSS_COLOR_VALUE:
		color.value = s->color_value;
		if (color.a < 255) {
			DUMPF("rgba(%d,%d,%d,%g)", color.r, color.g, color.b,
			      color.a / 255.0);
		} else {
			DUMPF("#%02x%02x%02x", color.r, color.g, color.b);
		}
		break;
	case CSS_STRING_VALUE:
		DUMP(s->string_value);
		break;
	case CSS_KEYWORD_VALUE:
		DUMP(css_get_keyword_name(s->keyword_value));
		break;
	case CSS_NUMERIC_VALUE:
		DUMPF("%g", s->numeric_value);
		break;
	case CSS_UNIT_VALUE:
		switch (s->unit_value.unit) {
		case CSS_UNIT_PERCENT:
			unit = "%";
			break;
		case CSS_UNIT_DIP:
			unit = "dip";
			break;
		case CSS_UNIT_PT:
			unit = "pt";
			break;
		case CSS_UNIT_SP:
			unit = "sp";
			break;
		case CSS_UNIT_PX:
		default:
			unit = "px";
			break;
		}
		DUMPF("%g%s", s->unit_value.value, unit);
		break;
	case CSS_UNPARSED_VALUE:
		DUMP(s->unparsed_value);
		break;
	case CSS_ARRAY_VALUE: {
		unsigned arr_i, arr_len;

		arr_len = css_style_value_get_array_length(s);
		if (arr_len > 1) {
			DUMP("[");
		}
		for (arr_i = 0; arr_i < arr_len; ++arr_i) {
			css_dump_style_value(s->array_value + arr_i, ctx);
			if (arr_i + 1 < arr_len) {
				DUMP(", ");
			}
		}
		if (arr_len > 1) {
			DUMP("]");
		}
		break;
	}
	default:
		break;
	}
}

size_t css_style_value_to_string(const css_style_value_t *s, char *str,
				 size_t max_len)
{
	css_dump_context_t ctx = { .data = str,
				   .len = 0,
				   .max_len = max_len,
				   .func = css_dump_to_buffer };
	css_dump_style_value(s, &ctx);
	return ctx.len;
}

size_t css_print_style_value(const css_style_value_t *s)
{
	css_dump_context_t ctx = {
		.data = NULL, .len = 0, .max_len = 0, .func = css_dump_to_stdout
	};
	css_dump_style_value(s, &ctx);
	return ctx.len;
}
