#include "../../include/css/computed.h"
#include "../../include/css/properties.h"

int css_cascade_z_index(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	if (input[0].type == CSS_NUMERIC_VALUE) {
		computed->z_index = input[0].numeric_value;
	} else {
		computed->z_index = 0;
	}
	computed->type_bits.z_index = CSS_Z_INDEX_SET;
	return 0;
}
