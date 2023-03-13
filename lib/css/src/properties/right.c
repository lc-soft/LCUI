#include "../properties.h"
#include "./helpers.h"

static void set_right(css_computed_style_t* computed, uint8_t keyword,
		    css_numeric_value_t value, css_unit_t unit)
{
	computed->right = value;
	computed->unit_bits.right = unit;
	computed->type_bits.right = keyword;
}

int css_cascade_right(const css_style_array_value_t input,
		    css_computed_style_t* computed)
{
	return css_cascade_length_auto(input, computed, set_right);
}
