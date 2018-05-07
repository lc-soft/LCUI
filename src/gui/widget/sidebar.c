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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/sidebar.h>
#include <LCUI/gui/css_parser.h>

typedef struct SideBarItemRec_ {
	wchar_t *id;
	LCUI_Widget icon;
	LCUI_Widget text;
	LCUI_Widget item;
} SideBarItemRec, *SideBarItem;

typedef struct SideBarRec_ {
	LinkedList items;
	LCUI_Style line_height;
} SideBarRec, *SideBar;

static struct SideBarModule {
	LCUI_WidgetPrototype sidebar;
	LCUI_WidgetPrototype item;
	LCUI_WidgetPrototype toggle;
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

LCUI_Widget SideBar_AppendItem( LCUI_Widget sidebar, const wchar_t *id,
				const wchar_t *icon, const wchar_t *text )
{
	SideBar sb;
	SideBarItem sbi;
	LCUI_Widget w;
	size_t len = id ? wcslen( id ) + 1 : 0;
	wchar_t *newid = NEW( wchar_t, len );
	if( !newid ) {
		return NULL;
	}
	sb = Widget_GetData( sidebar, self.sidebar );
	w = LCUIWidget_New( "sidebar-item" );
	sbi = Widget_GetData( sidebar, self.item );
	id ? wcscpy( newid, id ) : (newid[0] = 0, NULL);
	sbi->id ? free( sbi->id ) : 0;
	sbi->id = newid;
	Widget_Append( sidebar, w );
	TextView_SetTextW( sbi->icon, icon );
	TextView_SetTextW( sbi->text, text );
	Widget_Show( w );
	Widget_Show( sbi->icon );
	Widget_Show( sbi->text );
	LinkedList_Append( &sb->items, sbi );
	return w;
}

static void SideBarItem_OnInit( LCUI_Widget w )
{
	const size_t data_size = sizeof( SideBarItemRec );
	SideBarItem sbi = Widget_AddData( w, self.item, data_size );
	Widget_AddData( w, self.item, data_size );
	sbi->icon = LCUIWidget_New("textview");
	sbi->text = LCUIWidget_New("textview");
	sbi->id = NULL;
	Widget_AddClass( sbi->icon, "icon" );
	Widget_AddClass( sbi->text, "text" );
	Widget_Append( w, sbi->icon );
	Widget_Append( w, sbi->text );
}

static void SideBar_OnInit( LCUI_Widget w )
{
	const size_t data_size = sizeof( SideBarRec );
	SideBar sb = Widget_AddData( w, self.sidebar, data_size );
	LinkedList_Init( &sb->items );
}

static void OnToggle( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget sidebar = w->parent->parent;
	if( Widget_HasClass( sidebar, "sidebar-mini" ) ) {
		Widget_RemoveClass( sidebar, "sidebar-mini" );
	} else {
		Widget_AddClass( sidebar, "sidebar-mini" );
	}
	Widget_UpdateStyle( sidebar, TRUE );
	e->cancel_bubble = TRUE;
}

static void SideBarToggle_OnInit( LCUI_Widget w )
{
	Widget_BindEvent( w, "click", OnToggle, NULL, NULL );
}

void LCUIWidget_AddSideBar( void )
{
	self.sidebar = LCUIWidget_NewPrototype( "sidebar", NULL );
	self.item = LCUIWidget_NewPrototype( "sidebar-item", NULL );
	self.toggle = LCUIWidget_NewPrototype( "sidebar-toggle", NULL );
	self.sidebar->init = SideBar_OnInit;
	self.item->init = SideBarItem_OnInit;
	self.toggle->init = SideBarToggle_OnInit;
	LCUI_LoadCSSString( sidebar_css, __FILE__ );
}
