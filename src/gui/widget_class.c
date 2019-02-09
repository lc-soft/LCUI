/*
 * widget_class.c -- The widget class operation set.
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
#include <LCUI/gui/widget_class.h>
#include <LCUI/gui/widget_style.h>
#include <LCUI/gui/widget_task.h>

static void Widget_MarkChildrenRefreshByClasses(LCUI_Widget w)
{
	LinkedListNode *node;

	if (w->rules && w->rules->ignore_classes_change) {
		return;
	}
	Widget_AddTask(w, LCUI_WTASK_REFRESH_STYLE);
	for (LinkedList_Each(node, &w->children)) {
		Widget_MarkChildrenRefreshByClasses(node->data);
	}
}

static int Widget_HandleClassesChange(LCUI_Widget w, const char *name)
{
	Widget_UpdateStyle(w, TRUE);
	if (w->rules && w->rules->ignore_classes_change) {
		return 0;
	}
	if (Widget_GetChildrenStyleChanges(w, 0, name) > 0) {
		Widget_MarkChildrenRefreshByClasses(w);
		return 1;
	}
	return 0;
}

int Widget_AddClass(LCUI_Widget w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		return 1;
	}
	if (strlist_add(&w->classes, class_name) <= 0) {
		return 0;
	}
	return Widget_HandleClassesChange(w, class_name);
}

LCUI_BOOL Widget_HasClass(LCUI_Widget w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		return TRUE;
	}
	return FALSE;
}

int Widget_RemoveClass(LCUI_Widget w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		Widget_HandleClassesChange(w, class_name);
		strlist_remove(&w->classes, class_name);
		return 1;
	}
	return 0;
}

void Widget_DestroyClasses(LCUI_Widget w)
{
	if (w->classes) {
		strlist_free(w->classes);
	}
	w->classes = NULL;
}
