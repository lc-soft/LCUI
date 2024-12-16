/*
 * lib/ui-xml/src/ui_xml.c: -- the GUI build module, parse UI config code and build UI.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <yutil.h>
#include <css.h>
#include <pandagl.h>
#include <ui_xml.h>

#define WARN_TXT "[builder] warning: this module is not enabled before build.\n"

#ifdef LIBUI_XML_HAS_LIBXML2
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#define check_prop_name(PROP, NAME) \
        (xmlStrcasecmp(PROP->name, (xmlChar *)NAME) == 0)

enum ui_xml_parser_id { ID_ROOT, ID_UI, ID_WIDGET, ID_RESOURCE };

/** 解析器行为，用于决定解析器在解析完元素后的行为 */
enum ParserBehavior {
        PB_ERROR,   /**< 给出错误提示 */
        PB_WARNING, /**< 给出警告提示 */
        PB_NEXT,    /**< 处理下个元素 */
        PB_ENTER    /**< 进入子元素列表 */
};

typedef struct xml_parser xml_parser_t;
typedef int (*xml_parser_cb)(xml_parser_t *, xmlNodePtr);

typedef struct xml_node_parser {
        int id;
        const char *name;
        xml_parser_cb parse;
} xml_node_parser_t;

struct xml_parser {
        int id;
        ui_widget_t *root;
        ui_widget_t *widget;
        ui_widget_t *parent_widget;
        const ui_widget_prototype_t *widget_proto;
        xml_node_parser_t *parent_parser;
        const char *space;
};

#define EXIT(CODE)   \
        code = CODE; \
        goto exit;

static void xmlPrintErrorMessage(const xmlError *err)
{
        logger_error("[builder] %s (%d): error %d: %s\n",
                     err->file ? err->file : "(memory)", err->line, err->code,
                     err->message);
}

/** 解析 <resource> 元素，根据相关参数载入资源 */
static int ui_builder_parse_resource_node(xml_parser_t *parser, xmlNodePtr node)
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
                if (check_prop_name(prop, "type")) {
                        type = prop_val;
                } else if (check_prop_name(prop, "src")) {
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
                if (pd_font_library_load_file(src) < 1) {
                        EXIT(PB_WARNING);
                }
        } else if (strcmp(type, "text/css") == 0) {
                if (src) {
                        if (ui_load_css_file(src) != 0) {
                                EXIT(PB_WARNING);
                        }
                }
                for (node = node->children; node; node = node->next) {
                        if (node->type != XML_TEXT_NODE) {
                                continue;
                        }
                        ui_load_css_string((char *)node->content,
                                           parser->space);
                }
        } else if (strcmp(type, "text/xml") == 0) {
                ui_widget_t *pack;
                if (!src) {
                        EXIT(PB_WARNING);
                }
                pack = ui_load_xml_file(src);
                if (!pack) {
                        EXIT(PB_WARNING);
                }
                if (parser->parent_widget) {
                        ui_widget_append(parser->parent_widget, pack);
                } else if (parser->root) {
                        ui_widget_append(parser->root, pack);
                } else {
                        ui_widget_remove(pack);
                        EXIT(PB_WARNING);
                }
                ui_widget_unwrap(pack);
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
static int ui_builder_parse_ui_node(xml_parser_t *parser, xmlNodePtr node)
{
        if (node->type != XML_ELEMENT_NODE) {
                return PB_NEXT;
        }
        if (parser->parent_parser && parser->parent_parser->id != ID_ROOT) {
                return PB_ERROR;
        }
        parser->widget = ui_create_widget(NULL);
        parser->root = parser->widget;
        return PB_ENTER;
}

/** 解析 <widget> 元素数据 */
static int ui_builder_parse_widget_node(xml_parser_t *parser, xmlNodePtr node)
{
        xmlAttrPtr prop;
        char *prop_val = NULL, *prop_name, *type = NULL;
        ui_widget_t *w = NULL, *parent = parser->widget;

        if (parser->parent_parser && parser->parent_parser->id != ID_UI &&
            parser->parent_parser->id != ID_WIDGET) {
                return PB_ERROR;
        }
        switch (node->type) {
        case XML_ELEMENT_NODE:
                break;
        case XML_TEXT_NODE:
                ui_widget_set_text(parent, (char *)node->content);
                DEBUG_MSG("widget: %s, set text: %s\n", parent->type,
                          (char *)node->content);
                return PB_NEXT;
        default:
                return PB_ERROR;
        }
        if (parser->widget_proto) {
                w = ui_create_widget_with_prototype(parser->widget_proto);
        } else {
                for (prop = node->properties; prop; prop = prop->next) {
                        prop_val = (char *)xmlGetProp(node, prop->name);
                        if (check_prop_name(prop, "type")) {
                                type = prop_val;
                                break;
                        }
                        if (prop_val) {
                                xmlFree(prop_val);
                        }
                }
                w = ui_create_widget(type);
                if (type) {
                        xmlFree(type);
                }
        }
        if (!w) {
                return PB_ERROR;
        }
        DEBUG_MSG("create widget: %s\n", w->type);
        ui_widget_append(parent, w);
        parser->widget = w;
        for (prop = node->properties; prop; prop = prop->next) {
                prop_val = (char *)xmlGetProp(node, prop->name);
                if (check_prop_name(prop, "id")) {
                        DEBUG_MSG("widget: %p, set id: %s\n", w, prop_val);
                        ui_widget_set_id(w, prop_val);
                } else if (check_prop_name(prop, "class")) {
                        DEBUG_MSG("widget: %p, add class: %s\n", w, prop_val);
                        ui_widget_add_class(w, prop_val);
                } else {
                        prop_name = strdup2((const char *)prop->name);
                        strtolower(prop_name, (const char *)prop->name);
                        ui_widget_set_attr(w, prop_name, prop_val);
                        free(prop_name);
                }
                if (prop_val) {
                        xmlFree(prop_val);
                }
        }
        return PB_ENTER;
}

static xml_node_parser_t *ui_builder_get_node_parser(const char *name)
{
        size_t i;
        static xml_node_parser_t parsers[] = {
                { ID_WIDGET, "w", ui_builder_parse_widget_node },
                { ID_WIDGET, "widget", ui_builder_parse_widget_node },
                { ID_RESOURCE, "resource", ui_builder_parse_resource_node },
                { ID_UI, "ui", ui_builder_parse_ui_node }
        };

        for (i = 0; i < sizeof(parsers) / sizeof(xml_node_parser_t); ++i) {
                if (strcmp(parsers[i].name, name) == 0) {
                        return &parsers[i];
                }
        }
        return NULL;
}

/** 解析 xml 文档结点 */
static void ui_builder_parse_node(xml_parser_t *parser, xmlNodePtr node)
{
        xml_node_parser_t *p;
        xml_parser_t cur_parser;
        ui_widget_prototype_t *proto;

        for (; node; node = node->next) {
                proto = NULL;
                if (node->type == XML_COMMENT_NODE) {
                        continue;
                }
                if (node->type == XML_ELEMENT_NODE) {
                        p = ui_builder_get_node_parser(
                            (const char *)node->name);
                        if (!p) {
                                proto =
                                    ui_get_widget_prototype((char *)node->name);
                                /* If there is no suitable parser, but a widget
                                 * prototype with the same name already exists,
                                 * use the widget parser
                                 */
                                if (proto) {
                                        p = ui_builder_get_node_parser("w");
                                } else {
                                        continue;
                                }
                        }
                } else {
                        p = parser->parent_parser;
                        if (!p) {
                                continue;
                        }
                }
                cur_parser = *parser;
                cur_parser.parent_widget = parser->widget;
                cur_parser.widget_proto = proto;
                switch (p->parse(&cur_parser, node)) {
                case PB_ENTER:
                        cur_parser.parent_parser = p;
                        ui_builder_parse_node(&cur_parser, node->children);
                        break;
                case PB_NEXT:
                        break;
                case PB_WARNING:
                        logger_warning("[builder] %s (%d): warning: %s node.\n",
                                       node->doc->name, node->line, node->name);
                        break;
                case PB_ERROR:
                default:
                        logger_error("[builder] %s (%d): error: %s node.\n",
                                     node->doc->name, node->line, node->name);
                        break;
                }
                if (!parser->root && cur_parser.root) {
                        parser->root = cur_parser.root;
                }
        }
}
#endif

ui_widget_t *ui_load_xml_string(const char *str, int size)
{
#ifndef LIBUI_XML_HAS_LIBXML2
        logger_warning(WARN_TXT);
#else
        xmlDocPtr doc;
        xmlNodePtr cur;
        xml_parser_t parser;

        memset(&parser, 0, sizeof(parser));
        doc = xmlParseMemory(str, size);
        if (!doc) {
                xmlPrintErrorMessage(xmlGetLastError());
                logger_error("[builder] %s\n",
                             "failed to parse xml form memory");
                goto FAILED;
        }
        cur = xmlDocGetRootElement(doc);
        if (xmlStrcasecmp(cur->name, BAD_CAST "lcui-app")) {
                logger_error("[builder] error root node name: %s\n", cur->name);
                goto FAILED;
        }
        ui_builder_parse_node(&parser, cur->children);
FAILED:
        if (doc) {
                xmlFreeDoc(doc);
        }
        return parser.root;
#endif
        return NULL;
}

ui_widget_t *ui_load_xml_file(const char *filepath)
{
#ifndef LIBUI_XML_HAS_LIBXML2
        logger_warning(WARN_TXT);
#else
        xmlDocPtr doc;
        xmlNodePtr cur;
        xml_parser_t parser;

        memset(&parser, 0, sizeof(parser));
        parser.space = filepath;
        doc = xmlParseFile(filepath);
        if (!doc) {
                xmlPrintErrorMessage(xmlGetLastError());
                logger_error("[builder] %s\n", "failed to parse xml form file");
                goto FAILED;
        }
        cur = xmlDocGetRootElement(doc);
        if (xmlStrcasecmp(cur->name, BAD_CAST "lcui-app")) {
                logger_error("[builder] error root node name: %s\n", cur->name);
                goto FAILED;
        }
        ui_builder_parse_node(&parser, cur->children);
FAILED:
        if (doc) {
                xmlFreeDoc(doc);
        }
        return parser.root;
#endif
        return NULL;
}
