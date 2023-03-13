#include "../properties.h"

int css_cascade_word_break(const css_style_array_value_t input,
			    css_computed_style_t* computed)
{
	computed->type_bits.word_break = CSS_WORD_BREAK_NORMAL;
	switch (input[0].keyword_value) {
	case CSS_KEYWORD_BREAK_ALL:
		computed->type_bits.word_break = CSS_WORD_BREAK_BREAK_ALL;
		break;
	default:
		break;
	}
	return 0;
}
