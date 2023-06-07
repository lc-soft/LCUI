#include "../properties.h"
#include "./helpers.h"

static void set_max_width(css_computed_style_t* computed, uint8_t keyword,
			  css_numeric_value_t value, css_unit_t unit)
{
	computed->max_width = value;
	computed->unit_bits.max_width = unit;
	computed->type_bits.max_width = keyword;
}

int css_cascade_max_width(const css_style_array_value_t input,
			  css_computed_style_t* computed)
{
	return css_cascade_length_auto(input, computed, set_max_width);
}
