/*
 * widget_layout.c -- the widget layout processing module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/metrics.h>
#include "layout/block.h"
#include "layout/flexbox.h"
#include "widget_diff.h"

void Widget_Reflow(LCUI_Widget w, LCUI_LayoutRule rule)
{
	LCUI_WidgetEventRec ev = { 0 };

	switch (w->computed_style.display) {
	case SV_BLOCK:
	case SV_INLINE_BLOCK:
		LCUIBlockLayout_Reflow(w, rule);
		break;
	case SV_FLEX:
		LCUIFlexBoxLayout_Reflow(w, rule);
		break;
	case SV_NONE:
	default:
		break;
	}
	ev.cancel_bubble = TRUE;
	ev.type = LCUI_WEVENT_AFTERLAYOUT;
	Widget_TriggerEvent(w, &ev, NULL);
	DEBUG_MSG("id: %s, type: %s, size: (%g, %g)\n", w->id, w->type,
		  w->width, w->height);
}

LCUI_BOOL Widget_AutoReflow(LCUI_Widget w, LCUI_LayoutRule rule)
{
	float content_width = w->box.padding.width;
	float content_height = w->box.padding.height;
	LCUI_WidgetLayoutDiffRec diff;

	Widget_BeginLayoutDiff(w, &diff);
	Widget_ComputeSizeStyle(w);
	Widget_UpdateBoxSize(w);
	Widget_UpdateBoxPosition(w);
	Widget_AddState(w, LCUI_WSTATE_LAYOUTED);
	if (content_width == w->box.padding.width &&
	    content_height == w->box.padding.height) {
		return FALSE;
	}
	Widget_Reflow(w, rule);
	Widget_EndLayoutDiff(w, &diff);
	w->task.states[LCUI_WTASK_REFLOW] = FALSE;
	return TRUE;
}
