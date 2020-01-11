/*
 * widget_size.c -- widget size processing APIs.
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>

/** 根据当前部件的内外间距，获取调整后宽度 */
static float Widget_GetAdjustedWidth(LCUI_Widget w, float width)
{
	/* 如果部件的宽度不具备自动填满剩余空间的特性，则不调整 */
	if (!Widget_HasFillAvailableWidth(w)) {
		return width;
	}
	if (!Widget_HasAutoStyle(w, key_margin_left)) {
		width -= w->margin.left;
	}
	if (!Widget_HasAutoStyle(w, key_margin_right)) {
		width -= w->margin.right;
	}
	return width;
}

float Widget_ComputeMaxAvaliableWidth(LCUI_Widget widget)
{
	LCUI_Widget w;
	float width = 0, padding = 0, margin = 0;

	for (w = widget->parent; w; w = w->parent) {
		if (!Widget_HasAutoStyle(w, key_width) ||
		    w->computed_style.max_width >= 0) {
			width = w->box.content.width;
			break;
		}
		if (Widget_HasFillAvailableWidth(w)) {
			margin += w->box.outer.width - w->box.border.width;
		}
		padding += w->box.border.width - w->box.content.width;
	}
	width -= padding + margin;
	if (Widget_HasAbsolutePosition(widget)) {
		width += widget->padding.left + widget->padding.right;
	}
	if (width < 0) {
		width = 0;
	}
	return width;
}

float Widget_ComputeMaxWidth(LCUI_Widget widget)
{
	float width;
	if (!Widget_HasAutoStyle(widget, key_width) &&
	    !Widget_HasParentDependentWidth(widget)) {
		return widget->box.border.width;
	}
	width = Widget_ComputeMaxAvaliableWidth(widget);
	if (!Widget_HasAutoStyle(widget, key_max_width)) {
		if (widget->computed_style.max_width > -1 &&
		    width < widget->computed_style.max_width) {
			width = widget->computed_style.max_width;
		}
	}
	return width;
}

float Widget_ComputeMaxContentWidth(LCUI_Widget w)
{
	float width = Widget_ComputeMaxWidth(w);
	width = Widget_GetAdjustedWidth(w, width);
	return width - w->padding.left - w->padding.right -
	       w->computed_style.border.left.width -
	       w->computed_style.border.right.width;
}

float Widget_ComputeFillAvailableWidth(LCUI_Widget w)
{
	float width;
	width = Widget_ComputeMaxAvaliableWidth(w);
	return Widget_GetAdjustedWidth(w, width);
}

float Widget_GetLimitedWidth(LCUI_Widget w, float width)
{
	LCUI_WidgetStyle *style;
	style = &w->computed_style;
	if (style->max_width > -1 && width > style->max_width) {
		width = style->max_width;
	}
	if (style->min_width > -1 && width < style->min_width) {
		width = style->min_width;
	}
	return width;
}

float Widget_GetLimitedHeight(LCUI_Widget w, float height)
{
	LCUI_WidgetStyle *style;
	style = &w->computed_style;
	if (style->max_height > -1 && height > style->max_height) {
		height = style->max_height;
	}
	if (style->min_height > -1 && height < style->min_height) {
		height = style->min_height;
	}
	return height;
}
