/*
 * canvas.h -- canvas, used to draw custom graphics
 *
 * Copyright (c) 2019-2022, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIB_UI_WIDGETS_INCLUDE_CANVAS_H
#define LIB_UI_WIDGETS_INCLUDE_CANVAS_H

#include <ui.h>
#include "ui_widgets/common.h"

LIBUI_WIDGETS_BEGIN_DECLS

/** @see https://developer.mozilla.org/en-US/docs/Web/HTML/Element/canvas#attributes */

#define UI_CANVAS_DEFAULT_WIDTH 300
#define UI_CANVAS_DEFAULT_HEIGHT 150

typedef struct ui_canvas_rendering_context_t ui_canvas_context_t;

typedef struct ui_canvas_rendering_context_t {
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
