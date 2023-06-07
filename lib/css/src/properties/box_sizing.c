#include "../properties.h"

int css_cascade_box_sizing(const css_style_array_value_t input,
			   css_computed_style_t* computed)
{
	uint8_t value = CSS_BOX_SIZING_CONTENT_BOX;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_BORDER_BOX:
		value = CSS_BOX_SIZING_BORDER_BOX;
		break;
	default:
		break;
	}
	computed->type_bits.box_sizing = value;
	return 0;
}
