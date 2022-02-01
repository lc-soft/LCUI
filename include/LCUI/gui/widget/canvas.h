/*
 * canvas.h -- canvas, used to draw custom graphics
 *
 * Copyright (c) 2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#ifndef LCUI_CANVAS_H
#define LCUI_CANVAS_H

LCUI_BEGIN_HEADER

typedef struct LCUI_CanvasRenderingContextRec_ LCUI_CanvasRenderingContextRec;
typedef struct LCUI_CanvasRenderingContextRec_ *LCUI_CanvasRenderingContext;
typedef LCUI_CanvasRenderingContext LCUI_CanvasContext;

struct LCUI_CanvasRenderingContextRec_ {
	LCUI_BOOL available;
	pd_color_t fill_color;
	pd_canvas_t buffer;
	ui_widget_t* canvas;
	list_node_t node;

	float scale;
	int width;
	int height;

	void (*fillRect)(LCUI_CanvasContext, int, int, int, int);
	void (*clearRect)(LCUI_CanvasContext, int, int, int, int);
	void (*release)(LCUI_CanvasContext);
};

LCUI_API LCUI_CanvasContext Canvas_GetContext(ui_widget_t* w);

void LCUIWidget_AddCanvas(void);

LCUI_END_HEADER

#endif
