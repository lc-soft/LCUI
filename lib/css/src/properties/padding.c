#include "../../include/css/computed.h"
#include "../../include/css/properties.h"
#include "../../include/css/style_value.h"
#include "../../include/css/style_decl.h"
#include "../../include/css/value.h"
#include "./helpers.h"

#define CSS_CASCADE_PADDING_SIDE(PROP_KEY)                              \
	static void set_padding_##PROP_KEY(                             \
	    css_computed_style_t *computed, uint8_t keyword,            \
	    css_numeric_value_t value, css_unit_t unit)                 \
	{                                                               \
		computed->padding_##PROP_KEY = value;                   \
		computed->unit_bits.padding_##PROP_KEY = unit;          \
		computed->type_bits.padding_##PROP_KEY = keyword;       \
	}                                                               \
	int css_cascade_padding_##PROP_KEY(                             \
	    const css_style_array_value_t input,                        \
	    css_computed_style_t *computed)                             \
	{                                                               \
		return css_cascade_length_auto(input, computed,         \
					       set_padding_##PROP_KEY); \
	}

CSS_CASCADE_PADDING_SIDE(top)
CSS_CASCADE_PADDING_SIDE(right)
CSS_CASCADE_PADDING_SIDE(bottom)
CSS_CASCADE_PADDING_SIDE(left)

int css_parse_padding(css_propdef_t *propdef, const char *input,
		      css_style_decl_t *s)
{
	css_style_value_t value;
	int len = css_parse_value(propdef->valdef, input, &value);

	if (len <= 0) {
		return len;
	}
	switch (css_style_value_get_array_length(&value)) {
	case 1:
		css_style_decl_add(s, css_key_padding_top, value.array_value);
		css_style_decl_add(s, css_key_padding_right, value.array_value);
		css_style_decl_add(s, css_key_padding_bottom,
				   value.array_value);
		css_style_decl_add(s, css_key_padding_left, value.array_value);
		break;
	case 2:
		css_style_decl_add(s, css_key_padding_top, value.array_value);
		css_style_decl_add(s, css_key_padding_bottom,
				   value.array_value);
		css_style_decl_add(s, css_key_padding_left,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_padding_right,
				   value.array_value + 1);
		break;
	case 3:
		css_style_decl_add(s, css_key_padding_top, value.array_value);
		css_style_decl_add(s, css_key_padding_left,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_padding_right,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_padding_bottom,
				   value.array_value + 2);
		break;
	case 4:
		css_style_decl_add(s, css_key_padding_top, value.array_value);
		css_style_decl_add(s, css_key_padding_right,
				   value.array_value + 1);
		css_style_decl_add(s, css_key_padding_bottom,
				   value.array_value + 2);
		css_style_decl_add(s, css_key_padding_left,
				   value.array_value + 3);
		break;
	default:
		break;
	}
	css_style_value_destroy(&value);
	return len;
}
