/*
 * widget_prototype.c -- LCUI widget prototype library management module.
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


#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

static struct LCUI_WidgetPrototypeModule {
	Dict *prototypes;
	DictType dicttype;
	LCUI_WidgetPrototypeRec empty_prototype;
} self = {0};

static void DeletePrototype( void *privdata, void *data )
{
	LCUI_WidgetPrototype proto = data;
	free( proto->name );
	free( proto );
}

void LCUIWidget_InitPrototype( void )
{
	self.dicttype = DictType_StringKey;
	self.dicttype.valDestructor = DeletePrototype;
	self.prototypes = Dict_Create( &self.dicttype, NULL );
}

void LCUIWidget_FreePrototype( void )
{
	Dict_Release( self.prototypes );
}

LCUI_WidgetPrototype LCUIWidget_GetPrototype( const char *name )
{
	return Dict_FetchValue( self.prototypes, name );
}

LCUI_WidgetPrototype LCUIWidget_NewPrototype( const char *name,
					      const char *parent_name )
{
	LCUI_WidgetPrototype proto;
	if( Dict_FetchValue( self.prototypes, name ) ) {
		return NULL;
	}
	proto = NEW( LCUI_WidgetPrototypeRec, 1 );
	if( parent_name ) {
		LCUI_WidgetPrototype parent;
		parent = Dict_FetchValue( self.prototypes, parent_name );
		if( parent ) {
			*proto = *parent;
			proto->proto = parent;
		} else {
			*proto = self.empty_prototype;
		}
	} else {
		*proto = self.empty_prototype;
	}
	proto->name = strdup2( name );
	if( Dict_Add( self.prototypes, proto->name, proto ) == 0 ) {
		return proto;	
	}
	free( proto->name );
	free( proto );
	return NULL;
}

LCUI_BOOL Widget_CheckType( LCUI_Widget w, const char *type )
{
	LCUI_WidgetPrototypeC proto;

	if( ! w || !w->type ) {
		return FALSE;
	}
	if( strcmp( w->type, type ) == 0 ) {
		return TRUE;
	}
	if( !w->proto ) {
		return FALSE;
	}
	for( proto = w->proto->proto; proto; proto = proto->proto ) {
		if( strcmp( proto->name, type ) == 0 ) {
			return TRUE;
		}
	}
	return FALSE;
}

void *Widget_GetData( LCUI_Widget widget, LCUI_WidgetPrototype proto )
{
	uint_t i;
	if( !widget->data.list || !proto ) {
		return NULL;
	}
	for( i = 0; i < widget->data.length; ++i ) {
		if( widget->data.list[i].proto == proto ) {
			return widget->data.list[i].data;
		}
	}
	return NULL;
}

void *Widget_AddData( LCUI_Widget widget,
		      LCUI_WidgetPrototype proto, size_t data_size )
{
	void *data;
	LCUI_WidgetDataEntryRec *list;
	const size_t size = sizeof( LCUI_WidgetDataEntryRec );
	list = realloc( widget->data.list, size * (widget->data.length + 1) );
	if( !list ) {
		return NULL;
	}
	data = malloc( data_size );
	list[widget->data.length].data = data;
	list[widget->data.length].proto = proto;
	widget->data.list = list;
	widget->data.length += 1;
	return data;
}

void Widget_ClearPrototype( LCUI_Widget widget )
{
	if( widget->proto && widget->proto->destroy ) {
		widget->proto->destroy( widget );
	}
	while( widget->data.length > 0 ) {
		widget->data.length -= 1;
		free( widget->data.list[widget->data.length].data );
	}
	if( widget->data.list ) {
		free( widget->data.list );
	}
	if( widget->type && !widget->proto ) {
		free( widget->type );
		widget->type = NULL;
	}
	widget->proto = NULL;
}
