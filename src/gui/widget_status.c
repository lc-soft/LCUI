/*
 * widget_status.c -- The widget status operation set.
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget_base.h>
#include <LCUI/gui/widget_status.h>
#include <LCUI/gui/widget_style.h>
#include <LCUI/gui/widget_tree.h>

int Widget_AddStatus(LCUI_Widget w, const char *status_name)
{
	if (strshas(w->status, status_name)) {
		return 0;
	}
	if (strsadd(&w->status, status_name) <= 0) {
		return 0;
	}
	Widget_HandleChildrenStyleChange(w, 1, status_name);
	Widget_UpdateStyle(w, TRUE);
	return 1;
}

LCUI_BOOL Widget_HasStatus(LCUI_Widget w, const char *status_name)
{
	if (strshas(w->status, status_name)) {
		return TRUE;
	}
	return FALSE;
}

int Widget_RemoveStatus(LCUI_Widget w, const char *status_name)
{
	if (strshas(w->status, status_name)) {
		Widget_HandleChildrenStyleChange(w, 1, status_name);
		strsdel(&w->status, status_name);
		Widget_UpdateStyle(w, TRUE);
		return 1;
	}
	return 0;
}

void Widget_UpdateStatus(LCUI_Widget widget)
{
	LCUI_Widget child;
	if (!widget->parent) {
		return;
	}
	if (widget->index == widget->parent->children.length - 1) {
		Widget_AddStatus(widget, "last-child");
		child = Widget_GetPrev(widget);
		if (child) {
			Widget_RemoveStatus(child, "last-child");
		}
	}
	if (widget->index == 0) {
		Widget_AddStatus(widget, "first-child");
		child = Widget_GetNext(widget);
		if (child) {
			Widget_RemoveStatus(child, "first-child");
		}
	}
}

void Widget_SetDisabled(LCUI_Widget w, LCUI_BOOL disabled)
{
	w->disabled = disabled;
	if (w->disabled) {
		Widget_AddStatus(w, "disabled");
	} else {
		Widget_RemoveStatus(w, "disabled");
	}
}

void Widget_DestroyStatus(LCUI_Widget w)
{
	if (w->status) {
		freestrs(w->status);
	}
	w->status = NULL;
}
