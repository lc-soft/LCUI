/* ***************************************************************************
 * sidebar.c -- sidebar widget
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * sidebar.c -- LCUI 的侧边栏部件
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/sidebar.h>

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
	int len = id ? wcslen( id ) + 1 : 0;
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

void LCUIWidget_AddSideBar(void)
{
	self.sidebar = LCUIWidget_NewPrototype( "sidebar", NULL );
	self.item = LCUIWidget_NewPrototype( "sidebar-item", NULL );
	self.toggle = LCUIWidget_NewPrototype( "sidebar-toggle", NULL );
	self.sidebar->init = SideBar_OnInit;
	self.item->init = SideBarItem_OnInit;
	self.toggle->init = SideBarToggle_OnInit;
	LCUI_LoadCSSString( sidebar_css, __FILE__ );
}
