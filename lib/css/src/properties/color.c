#include "./helpers.h"

int css_cascade_color(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	switch (input[0].type) {
	case CSS_COLOR_VALUE:
		computed->color = input[0].color_value;
		computed->type_bits.color = CSS_COLOR_COLOR;
		break;
	default:
		return -1;
	}
	return 0;
}
