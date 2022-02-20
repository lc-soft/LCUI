#include "../../include/css/computed.h"
#include "../../include/css/properties.h"

int css_cascade_opacity(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	computed->opacity = input[0].numeric_value;
	computed->type_bits.opacity = CSS_OPACITY_SET;
	return 0;
}
