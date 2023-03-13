#include "../properties.h"
#include "./helpers.h"

int css_cascade_font_size(const css_style_array_value_t input,
			  css_computed_style_t* computed)
{
	computed->type_bits.font_size = CSS_FONT_SIZE_MEDIUM;
	switch (input[0].type) {
	case CSS_UNIT_VALUE:
		computed->font_size = input[0].unit_value.value;
		computed->unit_bits.font_size = input[0].unit_value.unit;
		computed->type_bits.font_size = CSS_FONT_SIZE_DIMENSION;
		break;
	case CSS_KEYWORD_VALUE:
		switch (input[0].keyword_value) {
		case CSS_KEYWORD_SMALL:
			computed->type_bits.font_size = CSS_FONT_SIZE_SMALL;
			break;
		case CSS_KEYWORD_LARGE:
			computed->type_bits.font_size = CSS_FONT_SIZE_LARGE;
			break;
		default:
			break;
		}
		break;
	default:
		return -1;
	}
	return 0;
}
