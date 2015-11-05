/* ***************************************************************************
 * builder.c -- the GUI build module, parse UI config code and build UI.
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * builder.c -- 图形界面构建模块，主要用于解析界面配置代码并生成相应的图形界面
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

#define WARNING_TEXT "[builder] warning: this module is not enabled before build."

#ifdef USE_LCUI_BUILDER
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

static int ParseWidget( LCUI_Widget w, xmlNodePtr node )
{
	int count = 0;
	xmlAttrPtr prop;
	LCUI_Widget child;
	LCUI_WidgetClass *wc = LCUIWidget_GetClass( w->type );

	for( ; node; node = node->next ) {
		switch( node->type ) {
		case XML_ELEMENT_NODE: 
			child = LCUIWidget_New( node->name );
			if( !child ) {
				continue;
			}
			count += ParseWidget( child, node->children );
			Widget_Append( w, child );
			break;
		case XML_TEXT_NODE:
			if( !wc || !wc->methods.set_text ) {
				break;
			}
			wc->methods.set_text( w, node->content );
		default: break;
		}
		if( !wc || !wc->methods.set_attr ) {
			continue;
		}
		prop = node->properties;
		while( prop ) {
			wc->methods.set_attr( w, prop->name, 
					      xmlGetProp(node, prop->name) );
			prop = prop->next;
		}
	}
	return count;
}
#endif

LCUI_Widget LCUIBuilder_LoadString( const char *str, int size )
{
#ifndef USE_LCUI_BUILDER
	printf(WARNING_TEXT);
#else
	xmlDocPtr doc;
	xmlNodePtr cur;
	LCUI_Widget root;

	doc = xmlParseMemory( str, size );
	if( !doc ) {
		printf( "[builder] Failed to parse xml form memory\n" );
		goto FAILED;
	}
	cur = xmlDocGetRootElement( doc );
	if( xmlStrcasecmp(cur->name, BAD_CAST"LCUI") ) {
		printf( "[builder] error root node name: %s\n", cur->name );
		goto FAILED;
	}
	root = LCUIWidget_New(NULL);
	ParseWidget( root, cur->children );
	return root;
FAILED:
	if( doc ) {
		xmlFreeDoc( doc );
	}
#endif
	return NULL;
}

LCUI_Widget LCUIBuilder_LoadFile( const char *filepath )
{
#ifndef USE_LCUI_BUILDER
	printf(WARNING_TEXT);
#else
	xmlDocPtr doc;
	xmlNodePtr cur;
	LCUI_Widget root;

	doc = xmlParseFile( filepath );
	if( !doc ) {
		printf( "[builder] Failed to parse xml file: %s\n", filepath );
		goto FAILED;
	}
	cur = xmlDocGetRootElement( doc );
	if( xmlStrcasecmp(cur->name, "LCUI") ) {
		printf( "[builder] error root node name: %s\n", cur->name );
		goto FAILED;
	}
	root = LCUIWidget_New(NULL);
	ParseWidget( root, cur->children );
	return root;
FAILED:
	if( doc ) {
		xmlFreeDoc( doc );
	}
#endif
	return NULL;
}
