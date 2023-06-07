#include "../properties.h"

int css_cascade_display(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	uint8_t value = CSS_DISPLAY_BLOCK;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_NONE:
		value = CSS_DISPLAY_NONE;
		break;
	case CSS_KEYWORD_BLOCK:
		value = CSS_DISPLAY_BLOCK;
		break;
	case CSS_KEYWORD_INLINE_BLOCK:
		value = CSS_DISPLAY_INLINE_BLOCK;
		break;
	case CSS_KEYWORD_FLEX:
		value = CSS_DISPLAY_FLEX;
		break;
	default:
		break;
	}
	computed->type_bits.display = value;
	return 0;
}
