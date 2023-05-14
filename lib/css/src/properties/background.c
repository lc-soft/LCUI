#include "../properties.h"

int css_cascade_background_color(const css_style_array_value_t input,
				 css_computed_style_t *s)
{
	switch (input[0].type) {
	case CSS_COLOR_VALUE:
		s->background_color = input[0].color_value;
		return 1;
	default:
		break;
	}
	return 0;
}

int css_cascade_background_position_x(const css_style_array_value_t input,
				      css_computed_style_t *s)
{
	switch (input[0].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_LEFT:
			s->background_position_x = 0;
			s->unit_bits.background_position_x = CSS_UNIT_PX;
			break;
		case CSS_KEYWORD_CENTER:
			s->background_position_x = 50;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_RIGHT:
			s->background_position_x = 100;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			break;
		default:
			return 0;
		}
		break;
	case CSS_UNIT_VALUE:
		s->background_position_x = input[0].unit_value.value;
		s->unit_bits.background_position_x = input[0].unit_value.unit;
		break;
	default:
		return 0;
	}
	s->type_bits.background_position_x = CSS_BACKGROUND_POSITION_SET;
	return 1;
}

int css_cascade_background_position_y(const css_style_array_value_t input,
				      css_computed_style_t *s)
{
	switch (input[0].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_TOP:
			s->background_position_y = 0;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_CENTER:
			s->background_position_y = 50;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_BOTTOM:
			s->background_position_y = 100;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		default:
			return 0;
		}
		break;
	case CSS_UNIT_VALUE:
		s->background_position_y = input[0].unit_value.value;
		s->unit_bits.background_position_y = input[0].unit_value.unit;
		break;
	default:
		return 0;
	}
	s->type_bits.background_position_y = CSS_BACKGROUND_POSITION_SET;
	return 1;
}

static int css_cascade_background_position_1(
    const css_style_array_value_t input, css_computed_style_t *s)
{
	switch (input[0].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_CENTER:
			s->background_position_x = 50;
			s->background_position_y = 50;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_TOP:
			s->background_position_x = 50;
			s->background_position_y = 0;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_RIGHT:
			s->background_position_x = 100;
			s->background_position_y = 50;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_BOTTOM:
			s->background_position_x = 50;
			s->background_position_y = 100;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		case CSS_KEYWORD_LEFT:
			s->background_position_x = 0;
			s->background_position_y = 50;
			s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
			s->unit_bits.background_position_y = CSS_UNIT_PERCENT;
			break;
		default:
			return 0;
		}
		return 1;
	case CSS_UNIT_VALUE:
		s->background_position_x = input[0].unit_value.value;
		s->background_position_y = 50;
		s->unit_bits.background_position_x = CSS_UNIT_PERCENT;
		s->unit_bits.background_position_y = input[0].unit_value.unit;
		break;
	default:
		return 0;
	}
	s->type_bits.background_position_x = CSS_BACKGROUND_POSITION_SET;
	s->type_bits.background_position_y = CSS_BACKGROUND_POSITION_SET;
	return 1;
}

static int css_cascade_background_position_2(
    const css_style_array_value_t input, css_computed_style_t *s)
{
	if (css_cascade_background_position_x(input, s) > 0) {
		if (css_cascade_background_position_y(input + 1, s) > 0) {
			return 2;
		}
	} else if (css_cascade_background_position_y(input, s) > 0 &&
		   css_cascade_background_position_x(input + 1, s) > 0) {
		return 2;
	}
	return 0;
}

/**
 * FIXME: 仅支持纯数值或纯关键字的值，暂不支持关键字和数值混写
 * 关键字和数值混写依赖 calc() 特性，暂不考虑实现
 * 例如：right 50px bottom 50px，应该转换为：
 * background-position-x = calc(100% - 50px)
 * background-position-y = calc(100% - 50px)
 */
int css_cascade_background_position(const css_style_array_value_t input,
				    css_computed_style_t *s)
{
	unsigned len;

	for (len = 0; len < 4 && input[len].type != CSS_NO_VALUE; ++len) {
		if (input[len].type != CSS_KEYWORD_VALUE &&
		    input[len].type != CSS_UNIT_VALUE) {
			break;
		}
	}
	switch (len) {
	case 1:
		return css_cascade_background_position_1(input, s);
	case 2:
		return css_cascade_background_position_2(input, s);
	default:
		break;
	}
	return 0;
}

static int css_cascade_background_size_1(const css_style_array_value_t input,
					 css_computed_style_t *s)
{
	switch (input[0].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_AUTO:
			s->type_bits.background_width =
			    CSS_BACKGROUND_SIZE_AUTO;
			break;
		case CSS_KEYWORD_COVER:
			s->type_bits.background_width =
			    CSS_BACKGROUND_SIZE_CONVER;
			break;
		case CSS_KEYWORD_CONTAIN:
			s->type_bits.background_width =
			    CSS_BACKGROUND_SIZE_CONTAIN;
			break;
		default:
			return 0;
		}
		break;
	case CSS_UNIT_VALUE:
		s->background_width = input[0].unit_value.value;
		s->unit_bits.background_width = input[0].unit_value.unit;
		s->type_bits.background_width = CSS_BACKGROUND_SIZE_SET;
		break;
	default:
		return 0;
	}
	s->background_height = 0;
	s->type_bits.background_height = CSS_BACKGROUND_SIZE_AUTO;
	return 0;
}

static int css_cascade_background_size_2(const css_style_array_value_t input,
					 css_computed_style_t *s)
{
	switch (input[0].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_AUTO:
			s->type_bits.background_width =
			    CSS_BACKGROUND_SIZE_AUTO;
			break;
		default:
			return 0;
		}
		break;
	case CSS_UNIT_VALUE:
		s->background_width = input[0].unit_value.value;
		s->unit_bits.background_width = input[0].unit_value.unit;
		s->type_bits.background_width = CSS_BACKGROUND_SIZE_SET;
		break;
	default:
		return 0;
	}
	switch (input[1].type) {
	case CSS_KEYWORD_VALUE:
		switch (input[1].keyword_value) {
		case CSS_KEYWORD_AUTO:
			s->type_bits.background_height =
			    CSS_BACKGROUND_SIZE_AUTO;
			break;
		default:
			return 0;
		}
		break;
	case CSS_UNIT_VALUE:
		s->background_height = input[0].unit_value.value;
		s->unit_bits.background_height = input[0].unit_value.unit;
		s->type_bits.background_height = CSS_BACKGROUND_SIZE_SET;
		break;
	default:
		return 0;
	}
	return 2;
}

int css_cascade_background_size(const css_style_array_value_t input,
				css_computed_style_t *s)
{
	unsigned len;

	for (len = 0; len < 2 && input[len].type != CSS_NO_VALUE; ++len) {
		if (input[len].type != CSS_KEYWORD_VALUE &&
		    input[len].type != CSS_UNIT_VALUE) {
			break;
		}
		if (input[len].type == CSS_KEYWORD_VALUE &&
		    input[len].keyword_value != CSS_KEYWORD_AUTO &&
		    input[len].keyword_value != CSS_KEYWORD_CONTAIN &&
		    input[len].keyword_value != CSS_KEYWORD_COVER) {
			break;
		}
	}
	switch (len) {
	case 1:
		return css_cascade_background_size_1(input, s);
	case 2:
		return css_cascade_background_size_2(input, s);
	default:
		break;
	}
	return 0;
}

int css_cascade_background_repeat(const css_style_array_value_t input,
				  css_computed_style_t *s)
{
	s->type_bits.background_repeat = CSS_BACKGROUND_REPEAT_REPEAT;
	if (input[0].type != CSS_KEYWORD_VALUE) {
		return 0;
	}
	switch (input[0].keyword_value) {
	case CSS_KEYWORD_REPEAT:
		s->type_bits.background_repeat = CSS_BACKGROUND_REPEAT_REPEAT;
		break;
	case CSS_KEYWORD_REPEAT_X:
		s->type_bits.background_repeat = CSS_BACKGROUND_REPEAT_REPEAT_X;
		break;
	case CSS_KEYWORD_REPEAT_Y:
		s->type_bits.background_repeat = CSS_BACKGROUND_REPEAT_REPEAT_Y;
		break;
	case CSS_KEYWORD_NO_REPEAT:
		s->type_bits.background_repeat =
		    CSS_BACKGROUND_REPEAT_NO_REPEAT;
		break;
	default:
		return 0;
	}
	return 1;
}

int css_cascade_background_image(const css_style_array_value_t input,
				 css_computed_style_t *s)
{
	s->background_image = NULL;
	s->type_bits.background_image = CSS_BACKGROUND_IMAGE_NONE;
	if (input[0].type == CSS_STRING_VALUE) {
		s->background_image = strdup2(input[0].image_value);
		s->type_bits.background_image = CSS_BACKGROUND_IMAGE_IMAGE;
		return 1;
	} else if (input[0].type == CSS_KEYWORD_VALUE &&
		   input[0].keyword_value == CSS_KEYWORD_NONE) {
		return 1;
	}
	return 0;
}

int css_parse_background_position(const char *input, css_style_decl_t *s)
{
	unsigned len;
	const char *cur;
	uint8_t has_x = 0;
	uint8_t has_y = 0;
	css_style_value_t value;
	css_propdef_t *prop_x = css_get_propdef(css_key_background_position_x);
	css_propdef_t *prop_y = css_get_propdef(css_key_background_position_y);

	for (cur = input, len = 0; *cur; cur += len) {
		if (!has_x &&
		    (len = css_parse_value(prop_x->valdef, cur, &value) > 0)) {
			css_style_decl_add(s, css_key_background_position_x,
					   &value);
			has_x = 1;
		} else if (!has_y && (len = css_parse_value(prop_y->valdef, cur,
							    &value) > 0)) {
			css_style_decl_add(s, css_key_background_position_y,
					   &value);
			has_y = 1;
		} else {
			break;
		}
		css_style_value_destroy(&value);
	}
	return (int)(cur - input);
}

int css_parse_background(css_propdef_t *propdef, const char *input,
			 css_style_decl_t *s)
{
	int len;
	const char *cur;
	uint8_t has_image = 0;
	uint8_t has_color = 0;
	uint8_t has_position = 0;
	uint8_t has_size = 0;
	uint8_t has_repeat = 0;
	css_propdef_t *prop_image = css_get_propdef(css_key_background_image);
	css_propdef_t *prop_color = css_get_propdef(css_key_background_color);
	css_propdef_t *prop_size = css_get_propdef(css_key_background_size);
	css_propdef_t *prop_repeat = css_get_propdef(css_key_background_repeat);
	css_style_value_t value;

	for (cur = input, len = 0; *cur; cur += len) {
		if (!has_image && (len = css_parse_value(prop_image->valdef,
							 cur, &value)) > 0) {
			css_style_decl_add(s, css_key_background_image, &value);
			has_image = 1;
		} else if (!has_color &&
			   (len = css_parse_value(prop_color->valdef, cur,
						  &value)) > 0) {
			css_style_decl_add(s, css_key_background_color, &value);
			has_color = 1;
		} else if (!has_position &&
			   (len = css_parse_background_position(cur, s) > 0)) {
			has_position = 1;
		} else if (!has_size &&
			   (len = css_parse_value(prop_size->valdef, cur,
						  &value)) > 0) {
			css_style_decl_add(s, css_key_background_size, &value);
			has_size = 1;
		} else if (!has_repeat &&
			   (len = css_parse_value(prop_repeat->valdef, cur,
						  &value)) > 0) {
			css_style_decl_add(s, css_key_background_repeat,
					   &value);
			has_repeat = 1;
		} else {
			break;
		}
	}
	return (int)(cur - input);
}
