#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/css.h"

static char *getdirname(const char *path)
{
	char *dirname;
	size_t i, pos;
	for (i = 0, pos = SIZE_MAX; path[i]; ++i) {
		if (path[i] == '/') {
			pos = i;
		}
	}
	if (pos > i) {
		return NULL;
	}
	dirname = malloc(sizeof(char) * i);
	if (!dirname) {
		return NULL;
	}
	for (i = 0; i < pos; ++i) {
		dirname[i] = path[i];
	}
	dirname[i] = 0;
	return dirname;
}

void css_style_parser_init(css_style_parser_t *parser, const char *space)
{
	parser->style_handler = NULL;
	parser->style_handler_arg = NULL;
	list_create(&parser->selectors);
	if (parser->space) {
		parser->space = strdup2(space);
		parser->dirname = getdirname(parser->space);
	} else {
		parser->dirname = NULL;
		parser->space = NULL;
	}
}

void css_style_parser_destroy(css_style_parser_t *parser)
{
	list_destroy(&parser->selectors,
		     (list_item_destructor_t)css_selector_destroy);
	if (parser->dirname) {
		free(parser->dirname);
	}
	if (parser->space) {
		free(parser->space);
	}
	parser->dirname = NULL;
}

void css_style_parser_set_property(css_style_parser_t *parser, int key,
				   css_unit_value_t *s)
{
	if (parser->style_handler) {
		parser->style_handler(key, s, parser->style_handler_arg);
	} else {
		parser->style->sheet[key] = *s;
	}
}

void css_style_parser_commit(css_style_parser_t *parser)
{
	list_node_t *node;
	/* 将记录的样式表添加至匹配到的选择器中 */
	for (list_each(node, &parser->selectors)) {
		css_add_style_sheet(node->data, parser->style,
				    parser->space);
	}
	list_destroy(&parser->selectors,
		     (list_item_destructor_t)css_selector_destroy);
	css_style_declaration_destroy(parser->style);
}
