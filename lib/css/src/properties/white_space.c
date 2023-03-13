#include "../properties.h"

int css_cascade_white_space(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	computed->type_bits.white_space = CSS_WHITE_SPACE_NORMAL;
	switch (input[0].keyword_value) {
	case CSS_KEYWORD_NOWRAP:
		computed->type_bits.white_space = CSS_WHITE_SPACE_NOWRAP;
		break;
	default:
		break;
	}
	return 0;
}
