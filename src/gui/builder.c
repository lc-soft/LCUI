/* ***************************************************************************
 * builder.c -- the GUI build module, parse UI config code and build UI.
 *
 * Copyright (C) 2015-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2015-2017 归属于 刘超 <lc-soft@live.cn>
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

//#define DEBUG
#include <stdio.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

#define WARN_TXT "[builder] warning: this module is not enabled before build.\n"

#ifdef USE_LCUI_BUILDER
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define PropNameIs(type) (xmlStrcasecmp(prop->name, BAD_CAST type) == 0)

enum ParserID {
	ID_ROOT,
	ID_UI,
	ID_WIDGET,
	ID_RESOURCE
};

/** 解析器行为，用于决定解析器在解析完元素后的行为 */
enum ParserBehavior {
	PB_ERROR,	/**< 给出错误提示 */
	PB_WARNING,	/**< 给出警告提示 */
	PB_NEXT,	/**< 处理下个元素 */
	PB_ENTER	/**< 进入子元素列表 */
};

typedef struct XMLParserContextRec_ XMLParserContextRec, *XMLParserContext;
typedef int(*ParserFuncPtr)(XMLParserContext, xmlNodePtr);

typedef struct Parser {
	int id;
	const char *name;
	ParserFuncPtr parse;
} Parser, *ParserPtr;

struct XMLParserContextRec_ {
	int id;
	LCUI_Widget widget;
	LCUI_Widget root;
	ParserPtr parent;
	const char *space;
};

static struct ModuleContext {
	LCUI_BOOL is_inited;
	RBTree parsers;
} self;

/** 解析<resource>元素，根据相关参数载入资源 */
static int ParseResource( XMLParserContext ctx, xmlNodePtr node )
{
	xmlAttrPtr prop;
	char *prop_val, *type = NULL, *src = NULL;

	if( node->type != XML_ELEMENT_NODE ) {
		return PB_NEXT;
	}
	prop = node->properties;
	while( prop ) {
		prop_val = (char*)xmlGetProp( node, prop->name );
		if( PropNameIs( "type" ) ) {
			type = prop_val;
		} else if( PropNameIs( "src" ) ) {
			src = prop_val;
		} else {
			xmlFree( prop_val );
		}
		prop = prop->next;
	}
	if( !type && !src ) {
		return PB_WARNING;
	}
	if( strstr( type, "application/font-" ) ) {
		LCUIFont_LoadFile( src );
	} else if( strstr( type, "text/css" ) ) {
		if( src ) {
			LCUI_LoadCSSFile( src );
		}
		for( node = node->children; node; node = node->next ) {
			if( node->type != XML_TEXT_NODE ) {
				continue;
			}
			LCUI_LoadCSSString( (char*)node->content, ctx->space );
		}
	}
	if( src ) {
		xmlFree( src );
	}
	if( type ) {
		xmlFree( type );
	}
	return PB_NEXT;
}

/** 解析<ui>元素，主要作用是创建一个容纳全部部件的根级部件 */
static int ParseUI( XMLParserContext ctx, xmlNodePtr node )
{
	if( node->type != XML_ELEMENT_NODE ) {
		return PB_NEXT;
	}
	if( ctx->parent && ctx->parent->id != ID_ROOT ) {
		return PB_ERROR;
	}
	ctx->widget = LCUIWidget_New(NULL);
	ctx->root = ctx->widget;
	return PB_ENTER;
}

/** 解析<widget>元素数据 */
static int ParseWidget( XMLParserContext ctx, xmlNodePtr node )
{
	xmlAttrPtr prop;
	char *prop_val = NULL;
	LCUI_Widget w = NULL, parent = ctx->widget;

	if( ctx->parent && ctx->parent->id != ID_UI &&
	    ctx->parent->id != ID_WIDGET ) {
		return PB_ERROR;
	}
	switch( node->type ) {
	case XML_ELEMENT_NODE:
		w = LCUIWidget_New( NULL );
		if( !w ) {
			return PB_ERROR;
		}
		DEBUG_MSG( "create widget: %p\n", w );
		Widget_Append( parent, w );
		ctx->widget = w;
		break;
	case XML_TEXT_NODE:
		if( !parent->proto || !parent->proto->settext ) {
			return PB_NEXT;
		}
		parent->proto->settext( parent, (char*)node->content );
		DEBUG_MSG( "widget: %s, set text: %s\n", parent->type,
			(char*)node->content );
		return PB_NEXT;
	default: return PB_ERROR;
	}
	for( prop = node->properties; prop; prop = prop->next ) {
		if( prop_val ) {
			xmlFree( prop_val );
		}
		prop_val = (char*)xmlGetProp( node, prop->name );
		if( PropNameIs( "type" ) ) {
			DEBUG_MSG( "widget: %p, set type: %s\n", w, prop_val );
			w->proto = LCUIWidget_GetPrototype( prop_val );
			if( w->proto && w->proto->init ) {
				w->proto->init( w );
				w->type = w->proto->name;
			} else {
				w->type = strdup2( prop_val );
			}
			continue;
		} else if( PropNameIs( "id" ) ) {
			DEBUG_MSG( "widget: %p, set id: %s\n", w, prop_val );
			Widget_SetId( w, prop_val );
			continue;
		} else if( PropNameIs( "class" ) ) {
			DEBUG_MSG( "widget: %p, add class: %s\n", w, prop_val );
			Widget_AddClass( w, prop_val );
			continue;
		}
		if( !w->proto || !w->proto->setattr ) {
			continue;
		}
		Widget_SetAttribute( w, (const char*)prop->name, prop_val );
		w->proto->setattr( w, (const char*)prop->name, prop_val );
	}
	if( prop_val ) {
		xmlFree( prop_val );
	}
	return PB_ENTER;
}

static Parser parser_list[] = {
	{ID_UI, "ui", ParseUI },
	{ID_WIDGET, "widget", ParseWidget },
	{ID_RESOURCE, "resource", ParseResource }
};

static int CompareName( void *data, const void *keydata )
{
	return strcmp(((Parser*)data)->name, (const char*)keydata);
}

static void LCUIBuilder_Init( void )
{
	int i, len;
	Parser *p;

	RBTree_Init( &self.parsers );
	RBTree_OnCompare( &self.parsers, CompareName );
	len = sizeof(parser_list) / sizeof(parser_list[0]);
	for( i = 0; i < len; ++i ) {
		p = &parser_list[i];
		RBTree_CustomInsert( &self.parsers, p->name, p );
	}
	self.is_inited = TRUE;
}

/** 解析 xml 文档结点 */
static void ParseNode( XMLParserContext ctx, xmlNodePtr node )
{
	ParserPtr p;
	XMLParserContextRec cur_ctx;
	for( ; node; node = node->next ) {
		if( node->type == XML_ELEMENT_NODE ) {
			p = RBTree_CustomGetData( &self.parsers, node->name );
		} else {
			p = ctx->parent;
		}
		if( !p ) {
			continue;
		}
		cur_ctx = *ctx;
		switch( p->parse(&cur_ctx, node) ) {
		case PB_ENTER:
			cur_ctx.parent = p;
			ParseNode( &cur_ctx, node->children );
			break;
		case PB_NEXT: break;
		case PB_WARNING:
			LOG( "[builder] %s (%d): warning: %s node.\n",
				node->doc->name, node->line, node->name );
			break;
		case PB_ERROR:
		default:
			LOG( "[builder] %s (%d): error: %s node.\n",
				node->doc->name, node->line, node->name );
			break;
		}
		if( !ctx->root && cur_ctx.root ) {
			ctx->root = cur_ctx.root;
		}
	}
}
#endif

LCUI_Widget LCUIBuilder_LoadString( const char *str, int size )
{
#ifndef USE_LCUI_BUILDER
	LOG(WARN_TXT);
#else
	xmlDocPtr doc;
	xmlNodePtr cur;
	XMLParserContextRec ctx;

	ctx.root = NULL;
	ctx.widget = NULL;
	ctx.parent = NULL;
	ctx.space = NULL;
	doc = xmlParseMemory( str, size );
	if( !doc ) {
		LOG( "[builder] Failed to parse xml form memory\n" );
		goto FAILED;
	}
	cur = xmlDocGetRootElement( doc );
	if( xmlStrcasecmp(cur->name, BAD_CAST"lcui-app") ) {
		LOG( "[builder] error root node name: %s\n", cur->name );
		goto FAILED;
	}
	if( !self.is_inited ) {
		LCUIBuilder_Init();
	}
	ParseNode( &ctx, cur->children );
FAILED:
	if( doc ) {
		xmlFreeDoc( doc );
	}
	return ctx.root;
#endif
	return NULL;
}

LCUI_Widget LCUIBuilder_LoadFile( const char *filepath )
{
#ifndef USE_LCUI_BUILDER
	LOG(WARN_TXT);
#else
	xmlDocPtr doc;
	xmlNodePtr cur;
	XMLParserContextRec ctx;

	ctx.root = NULL;
	ctx.widget = NULL;
	ctx.parent = NULL;
	ctx.space = filepath;
	doc = xmlParseFile( filepath );
	if( !doc ) {
		LOG( "[builder] Failed to parse xml file: %s\n", filepath );
		goto FAILED;
	}
	cur = xmlDocGetRootElement( doc );
	if( xmlStrcasecmp(cur->name, BAD_CAST"lcui-app") ) {
		LOG( "[builder] error root node name: %s\n", cur->name );
		goto FAILED;
	}
	if( !self.is_inited ) {
		LCUIBuilder_Init();
	}
	ParseNode( &ctx, cur->children );
FAILED:
	if( doc ) {
		xmlFreeDoc( doc );
	}
	return ctx.root;
#endif
	return NULL;
}
