#include "../properties.h"

int css_cascade_box_shadow(const css_style_array_value_t input,
			   css_computed_style_t* s)
{
	unsigned count;

	s->box_shadow_x = 0;
	s->box_shadow_y = 0;
	s->box_shadow_blur = 0;
	s->box_shadow_spread = 0;
	if (input[0].type == CSS_KEYWORD_VALUE) {
		if (input[0].keyword_value == CSS_KEYWORD_NONE) {
			s->type_bits.box_shadow = CSS_BOX_SHADOW_NONE;
		}
		return 0;
	}
	for (count = 0; count < 4; ++count) {
		if (input[count].type == CSS_NO_VALUE ||
		    input[count].type == CSS_COLOR_VALUE) {
			break;
		}
	}
	s->type_bits.box_shadow = CSS_BOX_SHADOW_SET;
	s->box_shadow_x = input[0].unit_value.value;
	s->box_shadow_y = input[1].unit_value.value;
	s->unit_bits.box_shadow_x = input[0].unit_value.unit;
	s->unit_bits.box_shadow_y = input[1].unit_value.unit;
	if (count > 2) {
		s->box_shadow_blur = input[2].unit_value.value;
		s->unit_bits.box_shadow_blur = input[2].unit_value.unit;
	} else {
		s->box_shadow_blur = 0;
		s->unit_bits.box_shadow_blur = 0;
	}
	if (count > 3) {
		s->box_shadow_spread = input[3].unit_value.value;
		s->unit_bits.box_shadow_spread = input[3].unit_value.unit;
	} else {
		s->box_shadow_spread = 0;
		s->unit_bits.box_shadow_spread = 0;
	}
	if (input[count].type == CSS_COLOR_VALUE) {
		s->box_shadow_color = input[count].color_value;
	} else {
		s->box_shadow_color = CSS_COLOR_TRANSPARENT;
	}
	return 0;
}
