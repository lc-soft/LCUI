/*
 * widget_diff.c -- Comparison of differences in component styles and layouts
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include "widget_diff.h"

#define MEMCMP(A, B) memcmp(A, B, sizeof(*(A)))

void Widget_InitStyleDiff(LCUI_Widget w, LCUI_WidgetStyleDiff diff)
{
	diff->box = w->box;
	diff->can_render = TRUE;
	diff->should_add_invalid_area = FALSE;
	if (w->parent) {
		if (!w->parent->computed_style.visible) {
			diff->can_render = FALSE;
			return;
		}
		if (w->parent->invalid_area_type >=
		    LCUI_INVALID_AREA_TYPE_PADDING_BOX) {
			w->invalid_area_type =
			    LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
			return;
		}
	}
	if (w->state < LCUI_WSTATE_LAYOUTED) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	}
	diff->should_add_invalid_area = TRUE;
}

void Widget_BeginStyleDiff(LCUI_Widget w, LCUI_WidgetStyleDiff diff)
{
	const LCUI_WidgetStyle *style = &w->computed_style;

	diff->left = w->computed_style.left;
	diff->right = w->computed_style.right;
	diff->top = w->computed_style.top;
	diff->bottom = w->computed_style.bottom;
	diff->width = w->width;
	diff->height = w->height;
	diff->margin = w->margin;
	diff->padding = w->padding;
	diff->display = style->display;
	diff->z_index = style->z_index;
	diff->visible = style->visible;
	diff->opacity = style->opacity;
	diff->position = style->position;
	diff->shadow = style->shadow;
	diff->border = style->border;
	diff->background = style->background;
	diff->flex = style->flex;
}

INLINE void Widget_AddReflowTask(LCUI_Widget w)
{
	if (w) {
		if (w->parent && Widget_IsFlexLayoutStyleWorks(w)) {
			Widget_AddTask(w->parent, LCUI_WTASK_REFLOW);
		}
		Widget_AddTask(w, LCUI_WTASK_REFLOW);
	}
}

INLINE void Widget_AddReflowTaskToParent(LCUI_Widget w)
{
	if (w->computed_style.position == SV_ABSOLUTE) {
		return;
	}
	Widget_AddReflowTask(w->parent);
}

int Widget_EndStyleDiff(LCUI_Widget w, LCUI_WidgetStyleDiff diff)
{
	LinkedListNode *node;
	const LCUI_WidgetStyle *style = &w->computed_style;

	if (!diff->can_render) {
		return 0;
	}
	diff->can_render = style->visible;
	if (style->visible != diff->visible) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		if (style->visible) {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_SHOW, TRUE);
		} else {
			Widget_PostSurfaceEvent(w, LCUI_WEVENT_HIDE, TRUE);
		}
	}

	/* check layout related property changes */

	Widget_UpdateBoxSize(w);
	Widget_UpdateBoxPosition(w);
	if (MEMCMP(&diff->box.padding, &w->box.padding)) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		for (LinkedList_Each(node, &w->children)) {
			Widget_AddTask(node->data, LCUI_WTASK_POSITION);
			Widget_AddTask(node->data, LCUI_WTASK_RESIZE);
		}
		Widget_AddReflowTask(w);
	} else if (MEMCMP(&diff->box.canvas, &w->box.canvas)) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	}
	if (Widget_IsFlexLayoutStyleWorks(w)) {
		if (diff->flex.wrap != style->flex.wrap ||
		    diff->flex.direction != style->flex.direction ||
		    diff->flex.justify_content != style->flex.justify_content ||
		    diff->flex.align_content != style->flex.align_content ||
		    diff->flex.align_items != style->flex.align_items) {
			Widget_AddReflowTask(w);
		}
		if (diff->flex.grow != style->flex.grow ||
		    diff->flex.shrink != style->flex.shrink ||
		    diff->flex.basis != style->flex.basis) {
			Widget_AddReflowTaskToParent(w);
		}
	}
	if (diff->display != style->display) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		Widget_AddReflowTaskToParent(w);
		if (style->display != SV_NONE) {
			Widget_AddReflowTask(w);
		}
	} else if (diff->position != style->position) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		if (diff->position == SV_ABSOLUTE ||
		    style->position == SV_ABSOLUTE) {
			Widget_AddReflowTask(w);
		}
		Widget_AddReflowTaskToParent(w);
	}

	/* check repaint related property changes */

	if (!diff->should_add_invalid_area) {
		return 0;
	}
	if (diff->opacity != w->computed_style.opacity) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	} else if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_CANVAS_BOX) {
	} else if (diff->z_index != style->z_index &&
		   style->position != SV_STATIC) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	} else if (MEMCMP(&diff->shadow, &style->shadow)) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	} else if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_BORDER_BOX) {
	} else if (MEMCMP(&diff->border, &style->border)) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_BORDER_BOX;
	} else if (MEMCMP(&diff->background, &style->background)) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_BORDER_BOX;
	} else {
		return 0;
	}
	return 1;
}

void Widget_BeginLayoutDiff(LCUI_Widget w, LCUI_WidgetLayoutDiff diff)
{
	diff->box = w->box;
	diff->should_add_invalid_area = FALSE;
	if (w->parent) {
		if (!w->parent->computed_style.visible) {
			return;
		}
		if (w->parent->invalid_area_type >=
		    LCUI_INVALID_AREA_TYPE_PADDING_BOX) {
			return;
		}
	}
	diff->should_add_invalid_area = TRUE;
}

int Widget_EndLayoutDiff(LCUI_Widget w, LCUI_WidgetLayoutDiff diff)
{
	LCUI_WidgetEventRec e;

	if (w->invalid_area_type >= LCUI_INVALID_AREA_TYPE_BORDER_BOX) {
		Widget_UpdateCanvasBox(w);
	}
	if (w->invalid_area_type == LCUI_INVALID_AREA_TYPE_CANVAS_BOX) {
	} else if (!LCUIRectF_IsEquals(&diff->box.canvas, &w->box.canvas)) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
	}
	if (diff->box.outer.x != w->box.outer.x ||
	    diff->box.outer.y != w->box.outer.y) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		Widget_PostSurfaceEvent(w, LCUI_WEVENT_MOVE,
					!w->task.skip_surface_props_sync);
		w->task.skip_surface_props_sync = TRUE;
		Widget_AddReflowTaskToParent(w);
	}
	if (diff->box.outer.width != w->box.outer.width ||
	    diff->box.outer.height != w->box.outer.height) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_CANVAS_BOX;
		e.target = w;
		e.data = NULL;
		e.type = LCUI_WEVENT_RESIZE;
		e.cancel_bubble = TRUE;
		Widget_PostEvent(w, &e, NULL, NULL);
		Widget_PostSurfaceEvent(w, LCUI_WEVENT_RESIZE,
					!w->task.skip_surface_props_sync);
		w->task.skip_surface_props_sync = TRUE;
		Widget_AddReflowTaskToParent(w);
	}
	if (!diff->should_add_invalid_area) {
		w->invalid_area_type = LCUI_INVALID_AREA_TYPE_NONE;
		return 0;
	}
	if (w->invalid_area_type < LCUI_INVALID_AREA_TYPE_PADDING_BOX) {
		return 0;
	}
	switch (w->invalid_area_type) {
	case LCUI_INVALID_AREA_TYPE_PADDING_BOX:
		w->invalid_area = diff->box.padding;
		break;
	case LCUI_INVALID_AREA_TYPE_BORDER_BOX:
		w->invalid_area = diff->box.border;
		break;
	default:
		w->invalid_area = diff->box.canvas;
		break;
	}
	while (w->parent) {
		w->parent->has_child_invalid_area = TRUE;
		w = w->parent;
	}
	return 1;
}
