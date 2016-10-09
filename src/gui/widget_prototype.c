/* ***************************************************************************
 * widget_prototype.c -- LCUI widget prototype library management module.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * widget_prototype.c -- LCUI 部件的原型管理模块
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

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

void LCUIWidget_ExitPrototype( void )
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
	proto->name = strdup( name );
	if( Dict_Add( self.prototypes, proto->name, proto ) == 0 ) {
		return proto;	
	}
	free( proto->name );
	free( proto );
	return NULL;
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
