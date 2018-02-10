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

typedef struct LCUI_XMLLoaderRec_ {
	char *key;		/**< 键，作为在视图加载完后传给事件处理器的额外参数 */
	char *filepath;		/**< 视图文件路径 */
	char *target_id;	/**< 目标容器部件的标识 */
	LCUI_Widget pack;	/**< 已经加载的视图内容包 */
	LCUI_Widget widget;	/**< 触发视图加载器的部件 */
} LCUI_XMLLoaderRec, *LCUI_XMLLoader;

static struct LCUI_Anchor {
	int event_id;
	LCUI_WidgetPrototype proto;
} self;

static void Loader_OnClearWidget( LCUI_Widget w,
				  LCUI_WidgetEvent e, void *arg )
{
	LCUI_XMLLoader loader = e->data;
	loader->widget = NULL;
}

static void XMLLoader_Destroy( LCUI_XMLLoader loader )
{
	if( loader->widget ) {
		Widget_UnbindEvent( loader->widget, "destroy",
				    Loader_OnClearWidget );
	}
	if( loader->key ) {
		free( loader->key );
	}
	loader->key = NULL;
	loader->pack = NULL;
	loader->widget = NULL;
	free( loader->target_id );
	free( loader->filepath );
	free( loader );
}

static LCUI_XMLLoader XMLLoader_New( LCUI_Widget w )
{
	LCUI_XMLLoader loader;
	const char *key = Widget_GetAttribute( w, "key" );
	loader = malloc( sizeof( LCUI_XMLLoaderRec ) );
	if( !loader ) {
		return NULL;
	}
	loader->widget = w;
	loader->filepath = strdup2( Widget_GetAttribute( w, "href" ) );
	loader->target_id = strdup2( Widget_GetAttribute( w, "target" ) );
	Widget_BindEvent( w, "destroy", Loader_OnClearWidget, loader, NULL );
	if( key ) {
		loader->key = strdup2( key );
	} else {
		loader->key = NULL;
	}
	return loader;
}

static void XMLLoader_AppendToTarget( LCUI_XMLLoader loader )
{
	LCUI_Widget target, root;
	LCUI_WidgetEventRec ev = { 0 };

	target = LCUIWidget_GetById( loader->target_id );
	if( !target ) {
		XMLLoader_Destroy( loader );
		return;
	}
	root = LCUIWidget_GetRoot();
	Widget_Append( target, loader->pack );
	Widget_Unwrap( loader->pack );
	ev.type = self.event_id;
	ev.cancel_bubble = TRUE;
	ev.target = loader->widget;
	Widget_TriggerEvent( root, &ev, loader->key );
	XMLLoader_Destroy( loader );
}

static void XMLLoader_Load( LCUI_XMLLoader loader )
{
	LCUI_Widget pack;
	char *path, dirname[] = "assets/views/";

	if( loader->filepath[0] != '/' ) {
		path = malloc( strsize( loader->filepath ) +
			       sizeof( dirname ) );
		if( !path ) {
			LOG( "[anchor] out of memory\n" );
			return;
		}
		strcpy( path, dirname );
		strcat( path, loader->filepath );
		pack = LCUIBuilder_LoadFile( path );
		free( path );
		if( pack ) {
			loader->pack = pack;
			LCUI_PostSimpleTask( XMLLoader_AppendToTarget,
					     loader, NULL );
			return;
		}
	}
	pack = LCUIBuilder_LoadFile( loader->filepath );
	if( pack ) {
		loader->pack = pack;
		LCUI_PostSimpleTask( XMLLoader_AppendToTarget, loader, NULL );
		return;
	}
	LOG( "[anchor] href (%s): cannot load xml resource\n",
	     loader->filepath );
	XMLLoader_Destroy( loader );
}

static void XMLLoader_StartLoad( LCUI_XMLLoader loader )
{
	LCUI_Widget target;
	LCUI_TaskRec task = { 0 };
	target = LCUIWidget_GetById( loader->target_id );
	if( !target ) {
		LOG( "[anchor] target (%s): not found\n", loader->target_id );
		return;
	}
	Widget_Empty( target );
	task.arg[0] = loader;
	task.func = (LCUI_TaskFunc)XMLLoader_Load;
	LCUI_PostAsyncTask( &task );
}

static int OpenUrl( const char *url )
{
#if defined(LCUI_BUILD_IN_WIN32) && defined(WINAPI_PARTITION_APP)
	return -1;
#else
	char cmd[512] = { 0 };
	snprintf( cmd, 511, URL_LUANCHER" %s", url );
	return system( cmd );
#endif
}

void Anchor_Open( LCUI_Widget w )
{
	LCUI_XMLLoader loader;
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
	loader = XMLLoader_New( w );
	if( !loader ) {
		LOG( "[anchor] out of memory\n" );
		return;
	}
	LCUI_PostSimpleTask( XMLLoader_StartLoad, loader, NULL );
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
