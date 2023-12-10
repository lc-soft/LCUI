/*
 * lib/ui/src/ui_widget_style.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <css/computed.h>

LIBUI_INLINE float padding_x(ui_widget_t *w)
{
	return w->computed_style.padding_left + w->computed_style.padding_right;
}

LIBUI_INLINE float padding_y(ui_widget_t *w)
{
	return w->computed_style.padding_top + w->computed_style.padding_bottom;
}

LIBUI_INLINE float border_x(ui_widget_t *w)
{
	return w->computed_style.border_left_width +
	       w->computed_style.border_right_width;
}

LIBUI_INLINE float border_y(ui_widget_t *w)
{
	return w->computed_style.border_top_width +
	       w->computed_style.border_bottom_width;
}

LIBUI_INLINE float margin_x(ui_widget_t *w)
{
	return w->computed_style.margin_left + w->computed_style.margin_right;
}

LIBUI_INLINE float margin_y(ui_widget_t *w)
{
	return w->computed_style.margin_top + w->computed_style.margin_bottom;
}

LIBUI_INLINE float to_border_box_width(ui_widget_t *w, float content_width)
{
	return content_width + padding_x(w) + border_x(w);
}

LIBUI_INLINE float to_border_box_height(ui_widget_t *w, float content_height)
{
	return content_height + padding_y(w) + border_y(w);
}

LIBUI_INLINE bool ui_widget_is_inline(ui_widget_t *w)
{
	return w->computed_style.type_bits.display ==
		   CSS_DISPLAY_INLINE_BLOCK ||
	       w->computed_style.type_bits.display == CSS_DISPLAY_INLINE_FLEX;
}

LIBUI_INLINE bool ui_widget_has_absolute_position(ui_widget_t *w)
{
	return css_computed_position(&w->computed_style) ==
	       CSS_POSITION_ABSOLUTE;
}

LIBUI_INLINE bool ui_widget_in_layout_flow(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) != CSS_DISPLAY_NONE &&
	       !ui_widget_has_absolute_position(w);
}

LIBUI_INLINE bool ui_widget_has_block_display(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) == CSS_DISPLAY_BLOCK;
}

LIBUI_INLINE bool ui_widget_has_flex_display(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) == CSS_DISPLAY_FLEX ||
	       css_computed_display(&w->computed_style) ==
		   CSS_DISPLAY_INLINE_FLEX;
}

LIBUI_INLINE bool ui_widget_has_inline_block_display(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) ==
	       CSS_DISPLAY_INLINE_BLOCK;
}

LIBUI_INLINE bool ui_widget_has_fill_available_width(ui_widget_t *w)
{
	return (ui_widget_has_block_display(w) ||
		ui_widget_has_flex_display(w)) &&
	       !ui_widget_has_absolute_position(w);
}

size_t ui_widget_get_children_style_changes(ui_widget_t *w, int type,
					    const char *name);

void ui_widget_destroy_style(ui_widget_t *w);
