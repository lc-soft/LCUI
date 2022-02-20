#include "../../include/css/computed.h"
#include "../../include/css/properties.h"

int css_cascade_pointer_events(const css_style_array_value_t input,
			       css_computed_style_t* computed)
{
	uint8_t value = CSS_POINTER_EVENTS_AUTO;

	switch (input[0].keyword_value) {
	case CSS_KEYWORD_NONE:
		value = CSS_POINTER_EVENTS_NONE;
	default:
		break;
	}
	computed->type_bits.pointer_events = value;
	return 0;
}
