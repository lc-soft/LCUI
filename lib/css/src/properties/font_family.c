#include "../../include/css/computed.h"
#include "../../include/css/properties.h"
#include "../../include/css/style_value.h"
#include "./helpers.h"

int css_cascade_font_family(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	unsigned i, len;
	const css_style_array_value_t arr = input[0].array_value;

	computed->font_family = NULL;
	computed->type_bits.font_family = CSS_FONT_FAMILY_INHERIT;
	if (input[0].type != CSS_ARRAY_VALUE) {
		return -1;
	}
	len = css_array_value_get_length(arr);
	if (len < 1) {
		return -1;
	}
	computed->font_family = malloc(sizeof(char*) * (len + 1));
	for (i = 0; i < len; ++i) {
		assert(arr[i].type == CSS_STRING_VALUE);
		computed->font_family[i] = strdup2(arr[i].string_value);
	}
	computed->font_family[len] = NULL;
	return 0;
}
