/* ***************************************************************************
 * builder.c -- the GUI build module, parse UI config code and build UI.
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
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/css_parser.h>

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
	char *prop_val = NULL, *prop_name;
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
		prop_name = malloc( strsize( (const char*)prop->name ) );
		strtolower( prop_name, (const char*)prop->name );
		Widget_SetAttribute( w, prop_name, prop_val );
		if( w->proto && w->proto->setattr ) {
			w->proto->setattr( w, prop_name, prop_val );
		}
		free( prop_name );
	}
	if( prop_val ) {
		xmlFree( prop_val );
	}
	return PB_ENTER;
}

static Parser parser_list[] = {
	{ ID_UI, "ui", ParseUI },
	{ ID_WIDGET, "w", ParseWidget },
	{ ID_WIDGET, "widget", ParseWidget },
	{ ID_RESOURCE, "resource", ParseResource }
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
