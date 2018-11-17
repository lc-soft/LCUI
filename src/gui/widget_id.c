/*
 * widget_id.c -- The widget ID operation set.
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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/gui/widget_base.h>
#include <LCUI/gui/widget_id.h>

static struct LCUI_WidgetIdLibraryModule {
	Dict *ids;
	DictType dt_ids;
	LCUI_Mutex mutex;
} self;

static int Widget_FreeId(LCUI_Widget w)
{
	LinkedList *list;
	LinkedListNode *node;

	if (!w->id) {
		return -1;
	}
	list = Dict_FetchValue(self.ids, w->id);
	if (!list) {
		return -2;
	}
	for (LinkedList_Each(node, list)) {
		if (node->data == w) {
			free(w->id);
			w->id = NULL;
			LinkedList_Unlink(list, node);
			LinkedListNode_Delete(node);
			return 0;
		}
	}
	return -3;
}

int Widget_DestroyId(LCUI_Widget w)
{
	int ret;
	LCUIMutex_Lock(&self.mutex);
	ret = Widget_FreeId(w);
	LCUIMutex_Unlock(&self.mutex);
	return ret;
}

int Widget_SetId(LCUI_Widget w, const char *idstr)
{
	LinkedList *list;

	Widget_DestroyId(w);
	if (!idstr) {
		return -1;
	}
	LCUIMutex_Lock(&self.mutex);
	w->id = strdup2(idstr);
	if (!w->id) {
		goto error_exit;
	}
	list = Dict_FetchValue(self.ids, w->id);
	if (!list) {
		list = malloc(sizeof(LinkedList));
		if (!list) {
			goto error_exit;
		}
		LinkedList_Init(list);
		if (Dict_Add(self.ids, w->id, list) != 0) {
			free(list);
			goto error_exit;
		}
	}
	if (!LinkedList_Append(list, w)) {
		goto error_exit;
	}
	LCUIMutex_Unlock(&self.mutex);
	return 0;

error_exit:
	LCUIMutex_Unlock(&self.mutex);
	if (w->id) {
		free(w->id);
		w->id = NULL;
	}
	return -2;
}

LCUI_Widget LCUIWidget_GetById(const char *id)
{
	LinkedList *list;
	LCUI_Widget w = NULL;

	if (!id) {
		return NULL;
	}
	LCUIMutex_Lock(&self.mutex);
	list = Dict_FetchValue(self.ids, id);
	if (list) {
		w = LinkedList_Get(list, 0);
	}
	LCUIMutex_Unlock(&self.mutex);
	return w;
}

static void OnClearWidgetList(void *privdata, void *data)
{
	LinkedList *list = data;
	LinkedList_Clear(list, NULL);
	free(list);
}

void LCUIWidget_InitIdLibrary(void)
{
	LCUIMutex_Init(&self.mutex);
	self.dt_ids = DictType_StringCopyKey;
	self.dt_ids.valDestructor = OnClearWidgetList;
	self.ids = Dict_Create(&self.dt_ids, NULL);
}

void LCUIWidget_FreeIdLibrary(void)
{
	Dict_Release(self.ids);
	LCUIMutex_Destroy(&self.mutex);
	self.ids = NULL;
}
