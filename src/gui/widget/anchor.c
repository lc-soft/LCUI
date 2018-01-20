/* ***************************************************************************
 * anchor.c -- The anchor widget, used to link view resource, its function is
 * similar to <a> element in HTML.
 *
 * Copyright (C) 2017-2018 by Liu Chao <lc-soft@live.cn>
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
 * anchor.c -- 锚点部件，用于链接视图资源，功能类似于 HTML 中的 <a> 标签
 *
 * 版权所有 (C) 2017-2018 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/anchor.h>
#include <LCUI/gui/builder.h>

#ifdef LCUI_BUILD_IN_WIN32
#define URL_LUANCHER "start"
#else
#define URL_LUANCHER "xdg-open"
#endif

static struct LCUI_Anchor {
	int event_id;
	LCUI_WidgetPrototype proto;
} self;

void AppendToTarget( LCUI_Widget w, LCUI_Widget box )
{
	const char *attr_target;
	LCUI_Widget target, root;
	LCUI_WidgetEventRec ev = { 0 };

	attr_target = Widget_GetAttribute( w, "target" );
	if( !attr_target ) {
		return;
	}
	target = LCUIWidget_GetById( attr_target );
	if( !target ) {
		return;
	}
	root = LCUIWidget_GetRoot();
	Widget_Append( target, box );
	Widget_Unwrap( box );
	ev.target = w;
	ev.type = self.event_id;
	ev.cancel_bubble = TRUE;
	Widget_TriggerEvent( root, &ev, NULL );
}

void LoadXMLFile( LCUI_Widget w )
{
	LCUI_Widget pack;
	char *href, prefix[] = "assets/views/";
	const char *attr_href = Widget_GetAttribute( w, "href" );

	if( attr_href[0] != '/' ) {
		href = malloc( strsize( attr_href ) + sizeof( prefix ) );
		if( !href ) {
			LOG( "[anchor] out of memory\n" );
			return;
		}
		strcpy( href, prefix );
		strcat( href, attr_href );
		pack = LCUIBuilder_LoadFile( href );
		free( href );
		if( pack ) {
			LCUI_PostSimpleTask( AppendToTarget, w, pack );
			return;
		}
	}
	pack = LCUIBuilder_LoadFile( attr_href );
	if( pack ) {
		LCUI_PostSimpleTask( AppendToTarget, w, pack );
		return;
	}
	LOG( "[anchor] href (%s): cannot load xml resource\n", attr_href );
}

void StartLoadXMLFile( LCUI_Widget w )
{
	LCUI_Widget target;
	LCUI_TaskRec task = { 0 };
	const char *attr_target = Widget_GetAttribute( w, "target" );
	if( !attr_target ) {
		LOG( "[anchor] target are required\n" );
		return;
	}
	target = LCUIWidget_GetById( attr_target );
	if( !target ) {
		LOG( "[anchor] target (%s): not found\n", attr_target );
		return;
	}
	Widget_Empty( target );
	task.arg[0] = w;
	task.func = (LCUI_TaskFunc)LoadXMLFile;
	LCUI_PostAsyncTask( &task );
}

static int OpenUrl( const char *url )
{
	char cmd[512] = { 0 };
	snprintf( cmd, 511, URL_LUANCHER" %s", url );
	return system( cmd );
}

void Anchor_Open( LCUI_Widget w )
{
	const char *attr_href = Widget_GetAttribute( w, "href" );
	if( !attr_href ) {
		LOG( "[anchor] href are required\n" );
		return;
	}
	if( strstr( attr_href, "file:" ) == attr_href ) {
		OpenUrl( attr_href + 5 );
		return;
	}
	if( strstr( attr_href, "http://" ) == attr_href ||
	    strstr( attr_href, "https://" ) == attr_href ) {
		OpenUrl( attr_href );
		return;
	}
	LCUI_PostSimpleTask( StartLoadXMLFile, w, NULL );
}

static void Anchor_OnClick( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	LCUI_TaskRec task = { 0 };
	task.func = (LCUI_TaskFunc)Anchor_Open;
	task.arg[0] = w;
	LCUI_PostAsyncTask( &task );
}

static void Anchor_OnInit( LCUI_Widget w )
{
	Widget_BindEvent( w, "click", Anchor_OnClick, NULL, NULL );
	self.proto->proto->init( w );
}

void LCUIWidget_AddAnchor( void )
{
	self.proto = LCUIWidget_NewPrototype( "a", "textview" );
	self.proto->init = Anchor_OnInit;
	self.event_id = LCUIWidget_AllocEventId();
	LCUIWidget_SetEventName( self.event_id, "loaded.anchor" );
}
