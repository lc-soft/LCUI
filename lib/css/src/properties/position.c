#include "../../include/css/computed.h"
#include "../../include/css/properties.h"

int css_cascade_position(const css_style_array_value_t input,
			 css_computed_style_t* computed)
{
	uint8_t value = CSS_POSITION_STATIC;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_ABSOLUTE:
		value = CSS_POSITION_ABSOLUTE;
		break;
	case CSS_KEYWORD_RELATIVE:
		value = CSS_POSITION_RELATIVE;
		break;
	default:
		break;
	}
	computed->type_bits.position = value;
	return 0;
}
