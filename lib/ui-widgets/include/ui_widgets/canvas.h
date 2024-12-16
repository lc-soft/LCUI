/*
 * lib/ui-widgets/include/ui_widgets/canvas.h: -- canvas, used to draw custom graphics
 *
 * Copyright (c) 2019-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_CANVAS_H
#define LIB_UI_WIDGETS_INCLUDE_CANVAS_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

/** @see https://developer.mozilla.org/en-US/docs/Web/HTML/Element/canvas#attributes */

#define UI_CANVAS_DEFAULT_WIDTH 300
#define UI_CANVAS_DEFAULT_HEIGHT 150

typedef struct ui_canvas_rendering_context ui_canvas_context_t;

typedef struct ui_canvas_rendering_context {
	bool available;
	pd_color_t fill_color;
	pd_canvas_t buffer;
	ui_widget_t* canvas;
	list_node_t node;

	float scale;
	int width;
	int height;

	/**
	 * Draws a rectangle that is filled according to the current `fill_color`.
	 * Reference: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fillRect
	*/
	void (*fill_rect)(ui_canvas_context_t*, int, int, int, int);

	/**
	 * Erases the pixels in a rectangular area by setting them to transparent black.
	 * Reference: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/clearRect
	*/
	void (*clear_rect)(ui_canvas_context_t*, int, int, int, int);


	void (*release)(ui_canvas_context_t*);
} ui_canvas_rendering_context_t;

/**
 * Get a drawing context on the canvas
 * Reference: https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement/getContext
*/
LIBUI_WIDGETS_PUBLIC ui_canvas_context_t *ui_canvas_get_context(ui_widget_t* w);

LIBUI_WIDGETS_PUBLIC void ui_register_canvas(void);

LIBUI_WIDGETS_END_DECLS

#endif
