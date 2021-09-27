﻿/*
 * widget_diff.h -- Comparison of differences in component styles and layouts
 *
 * Copyright (c) 2020, Liu chao <lc-soft@live.cn> All rights reserved.
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

/** for check widget difference */
typedef struct LCUI_WidgetStyleDiffRec_ {
	int z_index;
	int display;
	float left;
	float right;
	float top;
	float bottom;
	float width;
	float height;
	float opacity;
	LCUI_BOOL visible;
	LCUI_Rect2F margin;
	LCUI_Rect2F padding;
	LCUI_StyleValue position;
	pd_border_style_t border;
	pd_boxshadow_style_t shadow;
	LCUI_BackgroundStyle background;
	LCUI_WidgetBoxModelRec box;
	LCUI_FlexBoxLayoutStyle flex;
	LCUI_BOOL should_add_invalid_area;
} LCUI_WidgetStyleDiffRec, *LCUI_WidgetStyleDiff;

typedef struct LCUI_WidgetLayoutDiffRec_ {
	LCUI_BOOL should_add_invalid_area;
	LCUI_WidgetBoxModelRec box;
} LCUI_WidgetLayoutDiffRec, *LCUI_WidgetLayoutDiff;

void Widget_InitStyleDiff(LCUI_Widget w, LCUI_WidgetStyleDiff diff);

void Widget_BeginStyleDiff(LCUI_Widget w, LCUI_WidgetStyleDiff diff);

int Widget_EndStyleDiff(LCUI_Widget w, LCUI_WidgetStyleDiff diff);

void Widget_BeginLayoutDiff(LCUI_Widget w, LCUI_WidgetLayoutDiff diff);

int Widget_EndLayoutDiff(LCUI_Widget w, LCUI_WidgetLayoutDiff diff);
