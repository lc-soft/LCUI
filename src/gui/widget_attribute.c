/*
 * widget_attribute.c -- The widget attribute operation set.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget_base.h>
#include <LCUI/gui/widget_attribute.h>
#include <LCUI/gui/widget_status.h>

static struct LCUI_WidgetAttribleModule {
	DictType dt_attributes;
	LCUI_BOOL available;
} self;

static void OnClearWidgetAttribute(void *privdata, void *data)
{
	LCUI_WidgetAttribute attr = data;
	if (attr->value.destructor) {
		attr->value.destructor(attr->value.data);
	}
	free(attr->name);
	attr->name = NULL;
	attr->value.data = NULL;
	free(attr);
}

int Widget_SetAttributeEx(LCUI_Widget w, const char *name, void *value,
			  int value_type, void (*value_destructor)(void *))
{
	LCUI_WidgetAttribute attr;

	if (!self.available) {
		Dict_InitStringKeyType(&self.dt_attributes);
		self.dt_attributes.valDestructor = OnClearWidgetAttribute;
		self.available = TRUE;
	}
	if (!w->attributes) {
		w->attributes = Dict_Create(&self.dt_attributes, NULL);
	}
	attr = Dict_FetchValue(w->attributes, name);
	if (attr) {
		if (attr->value.destructor) {
			attr->value.destructor(attr->value.data);
		}
	} else {
		attr = NEW(LCUI_WidgetAttributeRec, 1);
		attr->name = strdup2(name);
		Dict_Add(w->attributes, attr->name, attr);
	}
	attr->value.data = value;
	attr->value.type = value_type;
	attr->value.destructor = value_destructor;
	return 0;
}

int Widget_SetAttribute(LCUI_Widget w, const char *name, const char *value)
{
	int ret;
	char *value_str = NULL;

	if (value) {
		value_str = strdup2(value);
		if (!value_str) {
			return -ENOMEM;
		}
		if (strcmp(name, "disabled") == 0) {
			if (!value || strcmp(value, "false") != 0) {
				Widget_SetDisabled(w, TRUE);
			} else {
				Widget_SetDisabled(w, FALSE);
			}
		}
		ret = Widget_SetAttributeEx(w, name, value_str,
					    LCUI_STYPE_STRING, free);
	} else {
		ret = Widget_SetAttributeEx(w, name, NULL, LCUI_STYPE_NONE,
					    NULL);
	}
	if (w->proto && w->proto->setattr) {
		w->proto->setattr(w, name, value_str);
	}
	return ret;
}

const char *Widget_GetAttribute(LCUI_Widget w, const char *name)
{
	LCUI_WidgetAttribute attr;
	if (!w->attributes) {
		return NULL;
	}
	attr = Dict_FetchValue(w->attributes, name);
	if (attr) {
		return attr->value.string;
	}
	return NULL;
}

void Widget_DestroyAttributes(LCUI_Widget w)
{
	if (w->attributes) {
		Dict_Release(w->attributes);
	}
	w->attributes = NULL;
}
