#include <pandagl.h>
#include <LCUI/css/style_value.h>
#include "internal.h"

void ui_widget_paint_background(ui_widget_t *w, pd_context_t *ctx,
				ui_widget_actual_style_t *style)
{
	pd_rect_t box;
	pd_background_t bg;
	css_computed_style_t *s = &w->computed_style;

	if (!s->background_image && css_color_alpha(s->background_color) < 1) {
		return;
	}
	bg.color.value = s->background_color;
	if (s->type_bits.background_image == CSS_BACKGROUND_IMAGE_DATA) {
		bg.image = (pd_canvas_t*)s->background_image;
	} else {
		bg.image = NULL;
	}
	bg.x = ui_compute(s->background_position_x);
	bg.y = ui_compute(s->background_position_y);
	bg.width = ui_compute(s->background_width);
	bg.height = ui_compute(s->background_height);
	bg.repeat_x =
	    s->type_bits.background_repeat == CSS_BACKGROUND_REPEAT_REPEAT ||
	    s->type_bits.background_repeat == CSS_BACKGROUND_REPEAT_REPEAT_X;
	bg.repeat_y =
	    s->type_bits.background_repeat == CSS_BACKGROUND_REPEAT_REPEAT ||
	    s->type_bits.background_repeat == CSS_BACKGROUND_REPEAT_REPEAT_Y;

	box.x = style->padding_box.x - style->canvas_box.x;
	box.y = style->padding_box.y - style->canvas_box.y;
	box.width = style->padding_box.width;
	box.height = style->padding_box.height;
	pd_paint_background(ctx, &bg, &box);
}
