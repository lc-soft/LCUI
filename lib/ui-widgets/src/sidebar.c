/*
 * sidebar.c -- sidebar widget
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
#include <LCUI.h>
#include <LCUI/ui.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/sidebar.h>
#include <LCUI/gui/css_parser.h>

typedef struct SideBarItemRec_ {
	wchar_t *id;
	ui_widget_t* icon;
	ui_widget_t* text;
	ui_widget_t* item;
} SideBarItemRec, *SideBarItem;

typedef struct SideBarRec_ {
	list_t items;
	LCUI_Style line_height;
} SideBarRec, *SideBar;

static struct SideBarModule {
	ui_widget_prototype_t *sidebar;
	ui_widget_prototype_t *item;
	ui_widget_prototype_t *toggle;
} self;

static const char sidebar_css[] = CodeToString(

sidebar {
	width: 224px;
	background-color: #fff;
}

sidebar sidebar-item {
	width: 100%;
	padding: 12px 16px 12px 7px;
	border-top: 1px solid #f3f3f3;
}

sidebar sidebar-item .text,
sidebar sidebar-item .icon {
	display: inline-block;
	font-size: 14px;
	color: #515253;
}

sidebar sidebar-item .icon {
	width: 30px;
	text-align: center;
}

sidebar sidebar-item:hover .text,
sidebar sidebar-item:hover .icon,
sidebar sidebar-item:active .text,
sidebar sidebar-item:active .icon {
	color: #262626;
}

sidebar sidebar-item:hover {
	background-color: #fbfbfb;
}

sidebar sidebar-item:active {
	background-color: #f3f3f3;
}

);

ui_widget_t* SideBar_AppendItem(ui_widget_t* sidebar, const wchar_t *id,
			       const wchar_t *icon, const wchar_t *text)
{
	SideBar sb;
	SideBarItem sbi;
	ui_widget_t* w;
	size_t len = id ? wcslen(id) + 1 : 0;
	wchar_t *newid = malloc(sizeof(wchar_t) * len);

	if (!newid) {
		return NULL;
	}
	sb = ui_widget_get_data(sidebar, self.sidebar);
	w = ui_create_widget("sidebar-item");
	sbi = ui_widget_get_data(sidebar, self.item);
	id ? wcscpy(newid, id) : (newid[0] = 0, NULL);
	sbi->id ? free(sbi->id) : 0;
	sbi->id = newid;
	ui_widget_append(sidebar, w);
	TextView_SetTextW(sbi->icon, icon);
	TextView_SetTextW(sbi->text, text);
	ui_widget_show(w);
	ui_widget_show(sbi->icon);
	ui_widget_show(sbi->text);
	list_append(&sb->items, sbi);
	return w;
}

static void SideBarItem_OnInit(ui_widget_t* w)
{
	const size_t data_size = sizeof(SideBarItemRec);
	SideBarItem sbi = ui_widget_add_data(w, self.item, data_size);
	ui_widget_add_data(w, self.item, data_size);
	sbi->icon = ui_create_widget("textview");
	sbi->text = ui_create_widget("textview");
	sbi->id = NULL;
	ui_widget_add_class(sbi->icon, "icon");
	ui_widget_add_class(sbi->text, "text");
	ui_widget_append(w, sbi->icon);
	ui_widget_append(w, sbi->text);
}

static void SideBar_OnInit(ui_widget_t* w)
{
	const size_t data_size = sizeof(SideBarRec);
	SideBar sb = ui_widget_add_data(w, self.sidebar, data_size);
	list_create(&sb->items);
}

static void OnToggle(ui_widget_t* w, ui_event_t* e, void *arg)
{
	ui_widget_t* sidebar = w->parent->parent;
	if (ui_widget_has_class(sidebar, "sidebar-mini")) {
		ui_widget_remove_class(sidebar, "sidebar-mini");
	} else {
		ui_widget_add_class(sidebar, "sidebar-mini");
	}
	ui_widget_refresh_style(sidebar);
	e->cancel_bubble = TRUE;
}

static void SideBarToggle_OnInit(ui_widget_t* w)
{
	ui_widget_on(w, "click", OnToggle, NULL, NULL);
}

void LCUIWidget_AddSideBar(void)
{
	self.sidebar = ui_create_widget_prototype("sidebar", NULL);
	self.item = ui_create_widget_prototype("sidebar-item", NULL);
	self.toggle = ui_create_widget_prototype("sidebar-toggle", NULL);
	self.sidebar->init = SideBar_OnInit;
	self.item->init = SideBarItem_OnInit;
	self.toggle->init = SideBarToggle_OnInit;
	ui_load_css_string(sidebar_css, __FILE__);
}
