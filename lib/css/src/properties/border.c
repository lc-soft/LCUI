#include "../properties.h"
#include "./helpers.h"

#define CSS_CASCADE_BORDER_SIDE_STYLE(PROP_KEY)                        \
	int css_cascade_border_##PROP_KEY##_style(                     \
	    const css_style_array_value_t input,                       \
	    css_computed_style_t *computed)                            \
	{                                                              \
		uint8_t value = CSS_BORDER_STYLE_NONE;                 \
                                                                       \
		switch (input[0].keyword_value) {                      \
		case CSS_KEYWORD_SOLID:                                \
			value = CSS_BORDER_STYLE_SOLID;                \
			break;                                         \
		default:                                               \
			break;                                         \
		}                                                      \
		computed->type_bits.border_##PROP_KEY##_style = value; \
		return 0;                                              \
	}

#define CSS_CASCADE_BORDER_ANGLE_RADIUS(PROP_KEY)                      \
	static void set_border_##PROP_KEY##_radius(                    \
	    css_computed_style_t *computed, uint8_t keyword,           \
	    css_numeric_value_t value, css_unit_t unit)                \
	{                                                              \
		computed->border_##PROP_KEY##_radius = value;          \
		computed->unit_bits.border_##PROP_KEY##_radius = unit; \
	}                                                              \
                                                                       \
	int css_cascade_border_##PROP_KEY##_radius(                    \
	    const css_style_array_value_t input,                       \
	    css_computed_style_t *computed)                            \
	{                                                              \
		return css_cascade_length_auto(                        \
		    input, computed, set_border_##PROP_KEY##_radius);  \
	}

#define CSS_CASCADE_BORDER_SIDE_WIDTH(PROP_KEY)                                \
	static void set_border_##PROP_KEY##_width(                             \
	    css_computed_style_t *computed, uint8_t type,                      \
	    css_numeric_value_t value, css_unit_t unit)                        \
	{                                                                      \
		computed->border_##PROP_KEY##_width = value;                   \
		computed->unit_bits.border_##PROP_KEY##_width = unit;          \
	}                                                                      \
                                                                               \
	int css_cascade_border_##PROP_KEY##_width(                             \
	    const css_style_array_value_t input,                               \
	    css_computed_style_t *computed)                                    \
	{                                                                      \
		return css_cascade_length_auto(input, computed,                \
					       set_border_##PROP_KEY##_width); \
	}

#define CSS_CASCADE_BORDER_SIDE_COLOR(PROP_KEY)                      \
	static void set_border_##PROP_KEY##_color(                   \
	    css_computed_style_t *computed, uint8_t type,            \
	    css_color_value_t value)                                 \
	{                                                            \
		computed->border_##PROP_KEY##_color = value;         \
	}                                                            \
                                                                     \
	int css_cascade_border_##PROP_KEY##_color(                   \
	    const css_style_array_value_t input,                     \
	    css_computed_style_t *computed)                          \
	{                                                            \
		return css_cascade_bg_border_color(                  \
		    input, computed, set_border_##PROP_KEY##_color); \
	}

CSS_CASCADE_BORDER_SIDE_STYLE(top)
CSS_CASCADE_BORDER_SIDE_STYLE(right)
CSS_CASCADE_BORDER_SIDE_STYLE(bottom)
CSS_CASCADE_BORDER_SIDE_STYLE(left)
CSS_CASCADE_BORDER_SIDE_WIDTH(top)
CSS_CASCADE_BORDER_SIDE_WIDTH(right)
CSS_CASCADE_BORDER_SIDE_WIDTH(bottom)
CSS_CASCADE_BORDER_SIDE_WIDTH(left)
CSS_CASCADE_BORDER_SIDE_COLOR(top)
CSS_CASCADE_BORDER_SIDE_COLOR(right)
CSS_CASCADE_BORDER_SIDE_COLOR(bottom)
CSS_CASCADE_BORDER_SIDE_COLOR(left)
CSS_CASCADE_BORDER_ANGLE_RADIUS(top_left)
CSS_CASCADE_BORDER_ANGLE_RADIUS(top_right)
CSS_CASCADE_BORDER_ANGLE_RADIUS(bottom_left)
CSS_CASCADE_BORDER_ANGLE_RADIUS(bottom_right)

int css_parse_border_width(css_propdef_t *propdef, const char *input,
			   css_style_decl_t *s)
{
	css_style_value_t value;
	int len = css_parse_value(propdef->valdef, input, &value);

	if (len <= 0) {
		return len;
	}
	switch (css_style_value_get_array_length(&value)) {
	case 1:
		css_style_decl_add(s, css_key_border_top_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_right_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_width,
				   value.array_value);
		break;
	case 2:
		css_style_decl_add(s, css_key_border_top_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_width,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_right_width,
				   value.array_value + 1);
		break;
	case 3:
		css_style_decl_add(s, css_key_border_top_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_width,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_right_width,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_width,
				   value.array_value + 2);
		break;
	case 4:
		css_style_decl_add(s, css_key_border_top_width,
				   value.array_value);
		css_style_decl_add(s, css_key_border_right_width,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_width,
				   value.array_value + 2);
		css_style_decl_add(s, css_key_border_left_width,
				   value.array_value + 3);
		break;
	default:
		break;
	}
	css_style_value_destroy(&value);
	return len;
}

int css_parse_border_style(css_propdef_t *propdef, const char *input,
			   css_style_decl_t *s)
{
	css_style_value_t value;
	int len = css_parse_value(propdef->valdef, input, &value);

	if (len <= 0) {
		return len;
	}
	switch (css_style_value_get_array_length(&value)) {
	case 1:
		css_style_decl_add(s, css_key_border_top_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_right_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_style,
				   value.array_value);
		break;
	case 2:
		css_style_decl_add(s, css_key_border_top_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_style,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_right_style,
				   value.array_value + 1);
		break;
	case 3:
		css_style_decl_add(s, css_key_border_top_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_style,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_right_style,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_style,
				   value.array_value + 2);
		break;
	case 4:
		css_style_decl_add(s, css_key_border_top_style,
				   value.array_value);
		css_style_decl_add(s, css_key_border_right_style,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_style,
				   value.array_value + 2);
		css_style_decl_add(s, css_key_border_left_style,
				   value.array_value + 3);
		break;
	default:
		break;
	}
	css_style_value_destroy(&value);
	return len;
}

int css_parse_border_color(css_propdef_t *propdef, const char *input,
			   css_style_decl_t *s)
{
	css_style_value_t value;
	int len = css_parse_value(propdef->valdef, input, &value);

	if (len <= 0) {
		return len;
	}
	switch (css_style_value_get_array_length(&value)) {
	case 1:
		css_style_decl_add(s, css_key_border_top_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_right_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_color,
				   value.array_value);
		break;
	case 2:
		css_style_decl_add(s, css_key_border_top_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_color,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_right_color,
				   value.array_value + 1);
		break;
	case 3:
		css_style_decl_add(s, css_key_border_top_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_left_color,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_right_color,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_color,
				   value.array_value + 2);
		break;
	case 4:
		css_style_decl_add(s, css_key_border_top_color,
				   value.array_value);
		css_style_decl_add(s, css_key_border_right_color,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_color,
				   value.array_value + 2);
		css_style_decl_add(s, css_key_border_left_color,
				   value.array_value + 3);
		break;
	default:
		break;
	}
	css_style_value_destroy(&value);
	return len;
}

int css_parse_border_radius(css_propdef_t *propdef, const char *input,
			    css_style_decl_t *s)
{
	css_style_value_t value;
	int len = css_parse_value(propdef->valdef, input, &value);

	if (len <= 0) {
		return len;
	}
	switch (css_style_value_get_array_length(&value)) {
	case 1:
		css_style_decl_add(s, css_key_border_top_left_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_top_right_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_left_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_right_radius,
				   value.array_value);
		break;
	case 2:
		css_style_decl_add(s, css_key_border_top_left_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_bottom_right_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_top_right_radius,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_left_radius,
				   value.array_value + 1);
		break;
	case 3:
		css_style_decl_add(s, css_key_border_top_left_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_top_right_radius,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_left_radius,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_right_radius,
				   value.array_value + 2);
		break;
	case 4:
		css_style_decl_add(s, css_key_border_top_left_radius,
				   value.array_value);
		css_style_decl_add(s, css_key_border_top_right_radius,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_border_bottom_right_radius,
				   value.array_value + 2);
		css_style_decl_add(s, css_key_border_bottom_left_radius,
				   value.array_value + 3);
		break;
	default:
		break;
	}
	css_style_value_destroy(&value);
	return len;
}

int css_parse_border(css_propdef_t *propdef, const char *input,
		     css_style_decl_t *s)
{
	int i, len;
	uint8_t has_width = 0;
	uint8_t has_style = 0;
	uint8_t has_color = 0;
	css_style_value_t value;
	css_style_value_t *v;

	len = css_parse_value(propdef->valdef, input, &value);
	if (len <= 0) {
		return len;
	}
	for (i = 0; i < 3; ++i) {
		v = value.array_value + i;
		if (v->type == CSS_NO_VALUE) {
			break;
		}
		if (!has_width && v->type == CSS_UNIT_VALUE) {
			css_style_decl_add(s, css_key_border_top_width, v);
			css_style_decl_add(s, css_key_border_right_width, v);
			css_style_decl_add(s, css_key_border_bottom_width, v);
			css_style_decl_add(s, css_key_border_left_width, v);
			has_width = 1;
		} else if (!has_style && v->type == CSS_KEYWORD_VALUE) {
			css_style_decl_add(s, css_key_border_top_style, v);
			css_style_decl_add(s, css_key_border_right_style, v);
			css_style_decl_add(s, css_key_border_bottom_style, v);
			css_style_decl_add(s, css_key_border_left_style, v);
			has_style = 1;
		} else if (!has_color && v->type == CSS_COLOR_VALUE) {
			css_style_decl_add(s, css_key_border_top_color, v);
			css_style_decl_add(s, css_key_border_right_color, v);
			css_style_decl_add(s, css_key_border_bottom_color, v);
			css_style_decl_add(s, css_key_border_left_color, v);
			has_color = 1;
		} else {
			break;
		}
	}
	css_style_value_destroy(&value);
	return len;
}
