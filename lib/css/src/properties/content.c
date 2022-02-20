#include "../../include/css/computed.h"
#include "../../include/css/properties.h"

int css_cascade_content(const css_style_array_value_t input,
			css_computed_style_t* computed)
{
	if (input[0].type == CSS_STRING_VALUE && input[0].string_value) {
		computed->content = strdup2(input[0].string_value);
		computed->type_bits.content = CSS_CONTENT_SET;
	} else {
		computed->content = NULL;
		computed->type_bits.content = CSS_CONTENT_NONE;
	}
	return 0;
}
