#include "../properties.h"

int css_cascade_font_style(const css_style_array_value_t input,
			   css_computed_style_t* computed)
{
	uint8_t value = CSS_FONT_STYLE_NORMAL;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_OBLIQUE:
		value = CSS_FONT_STYLE_OBLIQUE;
		break;
	case CSS_KEYWORD_ITALIC:
		value = CSS_FONT_STYLE_ITALIC;
		break;
	default:
		break;
	}
	computed->type_bits.font_style = value;
	return 0;
}
