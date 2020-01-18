/* ***************************************************************************
 * builder.c -- the GUI build module, parse UI config code and build UI.
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include "config.h"
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/css_parser.h>

#define WARN_TXT "[builder] warning: this module is not enabled before build.\n"

#ifdef USE_LCUI_BUILDER
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define PropNameIs(PROP, NAME) (xmlStrcasecmp(PROP->name, (xmlChar *)NAME) == 0)

enum ParserID { ID_ROOT, ID_UI, ID_WIDGET, ID_RESOURCE };

/** 解析器行为，用于决定解析器在解析完元素后的行为 */
enum ParserBehavior {
	PB_ERROR,   /**< 给出错误提示 */
	PB_WARNING, /**< 给出警告提示 */
	PB_NEXT,    /**< 处理下个元素 */
	PB_ENTER    /**< 进入子元素列表 */
};

typedef struct XMLParserContextRec_ XMLParserContextRec, *XMLParserContext;
typedef int (*ParserFuncPtr)(XMLParserContext, xmlNodePtr);

typedef struct Parser {
	int id;
	const char *name;
	ParserFuncPtr parse;
} Parser, *ParserPtr;

struct XMLParserContextRec_ {
	int id;
	LCUI_Widget root;
	LCUI_Widget widget;
	LCUI_Widget parent_widget;
	LCUI_WidgetPrototypeC widget_proto;
	ParserPtr parent_parser;
	const char *space;
};

static struct ModuleContext {
	LCUI_BOOL active;
	RBTree parsers;
} self;

#define EXIT(CODE)   \
	code = CODE; \
	goto exit;

static void xmlPrintErrorMessage(xmlErrorPtr err)
{
	Logger_Error("[builder] %s (%d): error %d: %s\n",
		     err->file ? err->file : "(memory)", err->line, err->code,
		     err->message);
}

/** 解析 <resource> 元素，根据相关参数载入资源 */
static int ParseResource(XMLParserContext ctx, xmlNodePtr node)
{
	xmlAttrPtr prop;

	int code = PB_NEXT;
	char *prop_val, *type = NULL, *src = NULL;

	if (node->type != XML_ELEMENT_NODE) {
		return PB_NEXT;
	}
	prop = node->properties;
	while (prop) {
		prop_val = (char *)xmlGetProp(node, prop->name);
		if (PropNameIs(prop, "type")) {
			type = prop_val;
		} else if (PropNameIs(prop, "src")) {
			src = prop_val;
		} else {
			xmlFree(prop_val);
		}
		prop = prop->next;
	}
	if (!type && !src) {
		return PB_WARNING;
	}
	if (strstr(type, "application/font-")) {
		if (LCUIFont_LoadFile(src) < 1) {
			EXIT(PB_WARNING);
		}
	} else if (strcmp(type, "text/css") == 0) {
		if (src) {
			if (LCUI_LoadCSSFile(src) != 0) {
				EXIT(PB_WARNING);
			}
		}
		for (node = node->children; node; node = node->next) {
			if (node->type != XML_TEXT_NODE) {
				continue;
			}
			LCUI_LoadCSSString((char *)node->content, ctx->space);
		}
	} else if (strcmp(type, "text/xml") == 0) {
		LCUI_Widget pack;
		if (!src) {
			EXIT(PB_WARNING);
		}
		pack = LCUIBuilder_LoadFile(src);
		if (!pack) {
			EXIT(PB_WARNING);
		}
		if (ctx->parent_widget) {
			Widget_Append(ctx->parent_widget, pack);
		} else if (ctx->root) {
			Widget_Append(ctx->root, pack);
		} else {
			Widget_Destroy(pack);
			EXIT(PB_WARNING);
		}
		Widget_Unwrap(pack);
	}
exit:
	if (src) {
		xmlFree(src);
	}
	if (type) {
		xmlFree(type);
	}
	return code;
}

/** 解析 <ui> 元素，主要作用是创建一个容纳全部部件的根级部件 */
static int ParseUI(XMLParserContext ctx, xmlNodePtr node)
{
	if (node->type != XML_ELEMENT_NODE) {
		return PB_NEXT;
	}
	if (ctx->parent_parser && ctx->parent_parser->id != ID_ROOT) {
		return PB_ERROR;
	}
	ctx->widget = LCUIWidget_New(NULL);
	ctx->root = ctx->widget;
	return PB_ENTER;
}

/** 解析 <widget> 元素数据 */
static int ParseWidget(XMLParserContext ctx, xmlNodePtr node)
{
	xmlAttrPtr prop;
	char *prop_val = NULL, *prop_name, *type = NULL;
	LCUI_Widget w = NULL, parent = ctx->widget;

	if (ctx->parent_parser && ctx->parent_parser->id != ID_UI &&
	    ctx->parent_parser->id != ID_WIDGET) {
		return PB_ERROR;
	}
	switch (node->type) {
	case XML_ELEMENT_NODE:
		break;
	case XML_TEXT_NODE:
		Widget_SetText(parent, (char *)node->content);
		DEBUG_MSG("widget: %s, set text: %s\n", parent->type,
			  (char *)node->content);
		return PB_NEXT;
	default:
		return PB_ERROR;
	}
	if (ctx->widget_proto) {
		w = LCUIWidget_NewWithPrototype(ctx->widget_proto);
	} else {
		for (prop = node->properties; prop; prop = prop->next) {
			prop_val = (char *)xmlGetProp(node, prop->name);
			if (PropNameIs(prop, "type")) {
				type = prop_val;
				break;
			}
			if (prop_val) {
				xmlFree(prop_val);
			}
		}
		w = LCUIWidget_New(type);
		if (type) {
			xmlFree(type);
		}
	}
	if (!w) {
		return PB_ERROR;
	}
	DEBUG_MSG("create widget: %s\n", w->type);
	Widget_Append(parent, w);
	ctx->widget = w;
	for (prop = node->properties; prop; prop = prop->next) {
		prop_val = (char *)xmlGetProp(node, prop->name);
		if (PropNameIs(prop, "id")) {
			DEBUG_MSG("widget: %p, set id: %s\n", w, prop_val);
			Widget_SetId(w, prop_val);
		} else if (PropNameIs(prop, "class")) {
			DEBUG_MSG("widget: %p, add class: %s\n", w, prop_val);
			Widget_AddClass(w, prop_val);
		} else {
			prop_name = malloc(strsize((const char *)prop->name));
			strtolower(prop_name, (const char *)prop->name);
			Widget_SetAttribute(w, prop_name, prop_val);
			free(prop_name);
		}
		if (prop_val) {
			xmlFree(prop_val);
		}
	}
	return PB_ENTER;
}

static Parser parser_list[] = { { ID_UI, "ui", ParseUI },
				{ ID_WIDGET, "w", ParseWidget },
				{ ID_WIDGET, "widget", ParseWidget },
				{ ID_RESOURCE, "resource", ParseResource } };

static int CompareName(void *data, const void *keydata)
{
	return strcmp(((Parser *)data)->name, (const char *)keydata);
}

static void LCUIBuilder_Init(void)
{
	int i, len;
	Parser *p;

	RBTree_Init(&self.parsers);
	RBTree_OnCompare(&self.parsers, CompareName);
	len = sizeof(parser_list) / sizeof(parser_list[0]);
	for (i = 0; i < len; ++i) {
		p = &parser_list[i];
		RBTree_CustomInsert(&self.parsers, p->name, p);
	}
	self.active = TRUE;
}

/** 解析 xml 文档结点 */
static void ParseNode(XMLParserContext ctx, xmlNodePtr node)
{
	ParserPtr p;
	XMLParserContextRec cur_ctx;
	LCUI_WidgetPrototype proto;

	for (; node; node = node->next) {
		proto = NULL;
		if (node->type == XML_COMMENT_NODE) {
			continue;
		}
		if (node->type == XML_ELEMENT_NODE) {
			p = RBTree_CustomGetData(&self.parsers, node->name);
			if (!p) {
				proto =
				    LCUIWidget_GetPrototype((char *)node->name);
				/* If there is no suitable parser, but a widget
				 * prototype with the same name already exists,
				 * use the widget parser
				 */
				if (proto) {
					p = &parser_list[1];
				} else {
					continue;
				}
			}
		} else {
			p = ctx->parent_parser;
			if (!p) {
				continue;
			}
		}
		cur_ctx = *ctx;
		cur_ctx.parent_widget = ctx->widget;
		cur_ctx.widget_proto = proto;
		switch (p->parse(&cur_ctx, node)) {
		case PB_ENTER:
			cur_ctx.parent_parser = p;
			ParseNode(&cur_ctx, node->children);
			break;
		case PB_NEXT:
			break;
		case PB_WARNING:
			Logger_Warning("[builder] %s (%d): warning: %s node.\n",
				       node->doc->name, node->line, node->name);
			break;
		case PB_ERROR:
		default:
			Logger_Error("[builder] %s (%d): error: %s node.\n",
				     node->doc->name, node->line, node->name);
			break;
		}
		if (!ctx->root && cur_ctx.root) {
			ctx->root = cur_ctx.root;
		}
	}
}
#endif

LCUI_Widget LCUIBuilder_LoadString(const char *str, int size)
{
#ifndef USE_LCUI_BUILDER
	Logger_Warning(WARN_TXT);
#else
	xmlDocPtr doc;
	xmlNodePtr cur;
	XMLParserContextRec ctx;

	memset(&ctx, 0, sizeof(ctx));
	doc = xmlParseMemory(str, size);
	if (!doc) {
		xmlPrintErrorMessage(xmlGetLastError());
		Logger_Error("[builder] failed to parse xml form memory\n");
		goto FAILED;
	}
	cur = xmlDocGetRootElement(doc);
	if (xmlStrcasecmp(cur->name, BAD_CAST "lcui-app")) {
		Logger_Error("[builder] error root node name: %s\n", cur->name);
		goto FAILED;
	}
	if (!self.active) {
		LCUIBuilder_Init();
	}
	ParseNode(&ctx, cur->children);
FAILED:
	if (doc) {
		xmlFreeDoc(doc);
	}
	return ctx.root;
#endif
	return NULL;
}

LCUI_Widget LCUIBuilder_LoadFile(const char *filepath)
{
#ifndef USE_LCUI_BUILDER
	Logger_Warning(WARN_TXT);
#else
	xmlDocPtr doc;
	xmlNodePtr cur;
	XMLParserContextRec ctx;

	memset(&ctx, 0, sizeof(ctx));
	ctx.space = filepath;
	doc = xmlParseFile(filepath);
	if (!doc) {
		xmlPrintErrorMessage(xmlGetLastError());
		Logger_Error("[builder] failed to parse xml form file\n");
		goto FAILED;
	}
	cur = xmlDocGetRootElement(doc);
	if (xmlStrcasecmp(cur->name, BAD_CAST "lcui-app")) {
		Logger_Error("[builder] error root node name: %s\n", cur->name);
		goto FAILED;
	}
	if (!self.active) {
		LCUIBuilder_Init();
	}
	ParseNode(&ctx, cur->children);
FAILED:
	if (doc) {
		xmlFreeDoc(doc);
	}
	return ctx.root;
#endif
	return NULL;
}
