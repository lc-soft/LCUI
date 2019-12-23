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

static inline float ToBorderBoxWidth(LCUI_Widget w, float content_width)
{
	return content_width + w->padding.left + w->padding.right +
	       w->computed_style.border.left.width +
	       w->computed_style.border.right.width;
}

static inline float ToBorderBoxHeight(LCUI_Widget w, float content_height)
{
	return content_height + w->padding.top + w->padding.bottom +
	       w->computed_style.border.top.width +
	       w->computed_style.border.bottom.width;
}

static inline float ToContentBoxWidth(LCUI_Widget w, float width)
{
	return width - w->padding.left - w->padding.right -
	       w->computed_style.border.left.width -
	       w->computed_style.border.right.width;
}

static inline float ToContentBoxHeight(LCUI_Widget w, float height)
{
	return height - w->padding.top - w->padding.bottom -
	       w->computed_style.border.top.width -
	       w->computed_style.border.bottom.width;
}

static void Widget_UpdateCanvasBox(LCUI_Widget w)
{
	LCUI_RectF *rg = &w->box.canvas;
	rg->x = w->x - Widget_GetBoxShadowOffsetX(w);
	rg->y = w->y - Widget_GetBoxShadowOffsetY(w);
	rg->width = Widget_GetCanvasWidth(w);
	rg->height = Widget_GetCanvasHeight(w);
}

static LCUI_BOOL Widget_ComputeStaticSize(LCUI_Widget w, float *width,
					  float *height)
{
	LCUI_WidgetBoxModelRec *box = &w->box;
	LCUI_WidgetStyle *style = &w->computed_style;

	/* If it is non-static layout */
	if (style->display == SV_NONE || style->position == SV_ABSOLUTE) {
		return FALSE;
	}
	if (Widget_HasParentDependentWidth(w)) {
		/* Compute the full size of child widget */
		Widget_AutoSize(w);
		/* Recompute the actual size of child widget later */
		Widget_AddTask(w, LCUI_WTASK_RESIZE);
	}
	/* If its width is a percentage, it ignores the effect of its outer
	 * and inner spacing on the static width. */
	if (Widget_CheckStyleType(w, key_width, scale)) {
		if (style->box_sizing == SV_BORDER_BOX) {
			*width = box->border.x + box->border.width;
		} else {
			*width = box->content.x + box->content.width;
			*width -= box->content.x - box->border.x;
		}
		*width -= box->outer.x - box->border.x;
	} else if (box->outer.width <= 0) {
		return FALSE;
	} else {
		*width = box->outer.x + box->outer.width;
	}
	if (Widget_CheckStyleType(w, key_height, scale)) {
		if (style->box_sizing == SV_BORDER_BOX) {
			*height = box->border.y + box->border.height;
		} else {
			*height = box->content.y + box->content.height;
			*height -= box->content.y - box->border.y;
		}
		*height -= box->outer.y - box->border.y;
	} else if (box->outer.height <= 0) {
		return FALSE;
	} else {
		*height = box->outer.y + box->outer.height;
	}
	return TRUE;
}

static void Widget_ComputeStaticContentSize(LCUI_Widget w, float *out_width,
					    float *out_height)
{
	LinkedListNode *node;
	float content_width = 0, content_height = 0, width, height;

	for (LinkedList_Each(node, &w->children_show)) {
		if (!Widget_ComputeStaticSize(node->data, &width, &height)) {
			continue;
		}
		content_width = max(content_width, width);
		content_height = max(content_height, height);
	}
	/* The child widget's coordinates are relative to the padding box,
	 * not the content box, so it needs to be converted */
	content_width -= w->padding.left;
	content_height -= w->padding.top;
	if (out_width && *out_width <= 0) {
		*out_width = content_width;
	}
	if (out_height && *out_height <= 0) {
		*out_height = content_height;
	}
}

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

static void Widget_SetSize(LCUI_Widget w, float width, float height)
{
	LCUI_RectF *box, *pbox;
	LCUI_BorderStyle *bbox;

	w->width = width;
	w->height = height;
	w->width = Widget_GetLimitedWidth(w, width);
	w->height = Widget_GetLimitedHeight(w, height);
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.content.width = w->width;
	w->box.content.height = w->height;
	w->box.padding.width = w->width;
	w->box.padding.height = w->height;
	pbox = &w->box.padding;
	bbox = &w->computed_style.border;
	/* 如果是以边框盒作为尺寸调整对象，则需根据边框盒计算内容框尺寸 */
	if (w->computed_style.box_sizing == SV_BORDER_BOX) {
		box = &w->box.content;
		pbox->width -= bbox->left.width + bbox->right.width;
		pbox->height -= bbox->top.width + bbox->bottom.width;
		box->width = pbox->width;
		box->height = pbox->height;
		box->width -= w->padding.left + w->padding.right;
		box->height -= w->padding.top + w->padding.bottom;
	} else {
		/* 否则是以内容框作为尺寸调整对象，需计算边框盒的尺寸 */
		box = &w->box.border;
		pbox->width += w->padding.left + w->padding.right;
		pbox->height += w->padding.top + w->padding.bottom;
		box->width = pbox->width;
		box->height = pbox->height;
		box->width += bbox->left.width + bbox->right.width;
		box->height += bbox->top.width + bbox->bottom.width;
	}
	w->width = w->box.border.width;
	w->height = w->box.border.height;
	w->box.outer.width = w->box.border.width;
	w->box.outer.height = w->box.border.height;
	w->box.outer.width += w->margin.left + w->margin.right;
	w->box.outer.height += w->margin.top + w->margin.bottom;
	Widget_UpdateCanvasBox(w);
}

static void Widget_ComputeContentSize(LCUI_Widget w, float *width, float *height)
{
	float limited_width;
	float static_width = 0, static_height = 0;
	float content_width = width ? *width : 0;
	float content_height = height ? *height : 0;

	Widget_ComputeLimitSize(w);
	Widget_ComputeStaticContentSize(w, &static_width, &static_height);
	if (w->proto && w->proto->autosize) {
		w->proto->autosize(w, &content_width, &content_height);
	}
	if (width && *width > 0) {
		goto done;
	}
	content_width = max(content_width, static_width);
	if (w->computed_style.box_sizing == SV_BORDER_BOX) {
		content_width = ToBorderBoxWidth(w, content_width);
	}
	limited_width = Widget_GetLimitedWidth(w, content_width);
	if (limited_width != content_width) {
		content_height = 0;
		content_width = limited_width;
		if (w->computed_style.box_sizing == SV_BORDER_BOX) {
			content_width = ToContentBoxWidth(w, content_width);
		}
		if (w->proto && w->proto->autosize) {
			w->proto->autosize(w, &content_width, &content_height);
		}
	} else {
		if (w->computed_style.box_sizing == SV_BORDER_BOX) {
			content_width = ToContentBoxWidth(w, content_width);
		}
	}

done:
	content_height = max(content_height, static_height);
	if (width && *width <= 0) {
		*width = content_width;
	}
	if (height && *height <= 0) {
		*height = content_height;
	}
}

void Widget_AutoSize(LCUI_Widget w)
{
	float width = 0, height = 0;
	if (!Widget_CheckStyleType(w, key_width, scale)) {
		width = Widget_ComputeXMetric(w, key_width);
	}
	if (!Widget_CheckStyleType(w, key_height, scale)) {
		height = Widget_ComputeYMetric(w, key_height);
	}
	Widget_ComputeContentSize(w, &width, &height);
	width = Widget_GetAdjustedWidth(w, width);
	Widget_SetSize(w, ToBorderBoxWidth(w, width),
		       ToBorderBoxHeight(w, height));
}

void Widget_ComputeSizeStyle(LCUI_Widget w)
{
	float width, height;
	float max_width = -1, default_width = -1;

	Widget_ComputeLimitSize(w);
	width = Widget_ComputeXMetric(w, key_width);
	height = Widget_ComputeYMetric(w, key_height);
	if (width > 0) {
		width = Widget_GetLimitedWidth(w, width);
	}
	if (height > 0) {
		height = Widget_GetLimitedHeight(w, height);
	}
	if (!Widget_HasAutoStyle(w, key_width) &&
	    !Widget_HasAutoStyle(w, key_height)) {
		if (w->computed_style.box_sizing == SV_CONTENT_BOX) {
			width = ToBorderBoxWidth(w, width);
			height = ToBorderBoxHeight(w, height);
		}
		Widget_SetSize(w, width, height);
		return;
	}
	if (w->computed_style.box_sizing == SV_BORDER_BOX) {
		width = ToContentBoxWidth(w, width);
		height = ToContentBoxHeight(w, height);
	}
	if (Widget_HasAutoStyle(w, key_width) &&
	    Widget_HasFillAvailableWidth(w)) {
		width = Widget_ComputeFillAvailableWidth(w);
		width = ToContentBoxWidth(w, width);
		if (!Widget_HasStaticWidthParent(w) && w->parent) {
			default_width = w->parent->box.content.width;
			if (w->computed_style.box_sizing == SV_BORDER_BOX) {
				default_width =
				    ToContentBoxWidth(w, default_width);
			}
			max_width = width;
			width = 0;
		}
	}
	Widget_ComputeContentSize(w, &width, &height);
	if (default_width != -1 && width < default_width) {
		width = default_width;
	}
	if (max_width != -1 && width > max_width) {
		width = max_width;
	}
	if (w->computed_style.box_sizing == SV_BORDER_BOX) {
		width = ToBorderBoxWidth(w, width);
		height = ToBorderBoxHeight(w, height);
	}
	Widget_SetSize(w, width, height);
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
	return ToContentBoxWidth(w, width);
}

float Widget_ComputeFillAvailableWidth(LCUI_Widget w)
{
	float width;
	width = Widget_ComputeMaxAvaliableWidth(w);
	return Widget_GetAdjustedWidth(w, width);
}

void Widget_ComputeLimitSize(LCUI_Widget w)
{
	LCUI_WidgetStyle *style;
	style = &w->computed_style;
	style->max_width = -1;
	style->min_width = -1;
	style->max_height = -1;
	style->min_height = -1;
	if (Widget_CheckStyleValid(w, key_max_width)) {
		style->max_width = Widget_ComputeXMetric(w, key_max_width);
	}
	if (Widget_CheckStyleValid(w, key_min_width)) {
		style->min_width = Widget_ComputeXMetric(w, key_min_width);
	}
	if (Widget_CheckStyleValid(w, key_max_height)) {
		style->max_height = Widget_ComputeYMetric(w, key_max_height);
	}
	if (Widget_CheckStyleValid(w, key_min_height)) {
		style->min_height = Widget_ComputeYMetric(w, key_min_height);
	}
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
