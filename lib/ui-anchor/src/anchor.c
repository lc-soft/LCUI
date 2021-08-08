/*
 * anchor.c -- The anchor widget, used to link view resource, its function is
 * similar to <a> element in HTML.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/ui.h>
#include <LCUI/app.h>
#include <LCUI/gui/widget/anchor.h>
#include <LCUI/gui/builder.h>

typedef struct LCUI_XMLLoaderRec_ {
	char* key; /**< 键，作为在视图加载完后传给事件处理器的额外参数 */
	char* filepath;      /**< 视图文件路径 */
	char* target_id;     /**< 目标容器部件的标识 */
	ui_widget_t* pack;   /**< 已经加载的视图内容包 */
	ui_widget_t* widget; /**< 触发视图加载器的部件 */
} LCUI_XMLLoaderRec, *LCUI_XMLLoader;

static struct LCUI_Anchor {
	int event_id;
	ui_widget_prototype_t* proto;
} self;

static void Loader_OnClearWidget(ui_widget_t* w, ui_event_t* e, void* arg)
{
	LCUI_XMLLoader loader = e->data;
	loader->widget = NULL;
}

static void XMLLoader_Destroy(LCUI_XMLLoader loader)
{
	if (loader->widget) {
		ui_widget_off(loader->widget, "destroy", Loader_OnClearWidget);
	}
	if (loader->key) {
		free(loader->key);
	}
	loader->key = NULL;
	loader->pack = NULL;
	loader->widget = NULL;
	free(loader->target_id);
	free(loader->filepath);
	free(loader);
}

static LCUI_XMLLoader XMLLoader_New(ui_widget_t* w)
{
	LCUI_XMLLoader loader;
	const char* key = ui_widget_get_attribute_value(w, "key");
	loader = malloc(sizeof(LCUI_XMLLoaderRec));
	if (!loader) {
		return NULL;
	}
	loader->widget = w;
	loader->filepath = strdup2(ui_widget_get_attribute_value(w, "href"));
	loader->target_id = strdup2(ui_widget_get_attribute_value(w, "target"));
	ui_widget_on(w, "destroy", Loader_OnClearWidget, loader, NULL);
	if (key) {
		loader->key = strdup2(key);
	} else {
		loader->key = NULL;
	}
	return loader;
}

static void XMLLoader_AppendToTarget(LCUI_XMLLoader loader)
{
	ui_widget_t *target, *root;
	ui_event_t ev = { 0 };

	target = ui_get_widget(loader->target_id);
	if (!target) {
		XMLLoader_Destroy(loader);
		return;
	}
	root = ui_root();
	ui_widget_append(target, loader->pack);
	ui_widget_unwrap(loader->pack);
	ev.type = self.event_id;
	ev.cancel_bubble = TRUE;
	ev.target = loader->widget;
	ui_widget_emit_event(root, ev, loader->key);
	XMLLoader_Destroy(loader);
}

static void XMLLoader_Load(LCUI_XMLLoader loader)
{
	ui_widget_t* pack;
	char *path, dirname[] = "assets/views/";

	if (loader->filepath[0] != '/') {
		path = malloc((strlen(loader->filepath) + 1) * sizeof(char) +
			      sizeof(dirname));
		if (!path) {
			logger_error("[anchor] out of memory\n");
			return;
		}
		strcpy(path, dirname);
		strcat(path, loader->filepath);
		pack = LCUIBuilder_LoadFile(path);
		free(path);
		if (pack) {
			loader->pack = pack;
			lcui_post_simple_task(XMLLoader_AppendToTarget, loader,
					      NULL);
			return;
		}
	}
	pack = LCUIBuilder_LoadFile(loader->filepath);
	if (pack) {
		loader->pack = pack;
		lcui_post_simple_task(XMLLoader_AppendToTarget, loader, NULL);
		return;
	}
	logger_error("[anchor] href (%s): cannot load xml resource\n",
		     loader->filepath);
	XMLLoader_Destroy(loader);
}

static void XMLLoader_StartLoad(LCUI_XMLLoader loader)
{
	ui_widget_t* target;
	LCUI_TaskRec task = { 0 };
	target = ui_get_widget(loader->target_id);
	if (!target) {
		logger_error("[anchor] target (%s): not found\n",
			     loader->target_id);
		return;
	}
	ui_widget_empty(target);
	task.arg[0] = loader;
	task.func = (LCUI_TaskFunc)XMLLoader_Load;
	lcui_post_async_task(&task, -1);
}

void Anchor_Open(ui_widget_t* w)
{
	LCUI_XMLLoader loader;
	const char* attr_href = ui_widget_get_attribute_value(w, "href");

	if (!attr_href) {
		logger_error("[anchor] href are required\n");
		return;
	}
	if (strstr(attr_href, "file:") == attr_href) {
		OpenUri(attr_href + 5);
		return;
	}
	if (strstr(attr_href, "http://") == attr_href ||
	    strstr(attr_href, "https://") == attr_href) {
		OpenUri(attr_href);
		return;
	}
	loader = XMLLoader_New(w);
	if (!loader) {
		logger_error("[anchor] out of memory\n");
		return;
	}
	lcui_post_simple_task(XMLLoader_StartLoad, loader, NULL);
}

static void Anchor_OnClick(ui_widget_t* w, ui_event_t* e, void* arg)
{
	LCUI_TaskRec task = { 0 };
	task.func = (LCUI_TaskFunc)Anchor_Open;
	task.arg[0] = w;
	lcui_post_async_task(&task, -1);
}

static void Anchor_OnInit(ui_widget_t* w)
{
	ui_widget_on(w, "click", Anchor_OnClick, NULL, NULL);
	self.proto->proto->init(w);
}

void LCUIWidget_AddAnchor(void)
{
	self.proto = ui_create_widget_prototype("a", "textview");
	self.proto->init = Anchor_OnInit;
	self.event_id = ui_alloc_event_id();
	ui_set_event_id(self.event_id, "loaded.anchor");
}
