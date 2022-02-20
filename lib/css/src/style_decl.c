#include "../include/css/properties.h"
#include "./dump.h"

css_style_decl_t *css_style_decl_create(void)
{
	css_style_decl_t *list;

	list = malloc(sizeof(css_style_decl_t));
	list_create(list);
	return list;
}

static void css_prop_destroy(css_prop_t *node)
{
	css_style_value_destroy(&node->style);
	free(node);
}

void css_style_decl_destroy(css_style_decl_t *list)
{
	list_destroy_without_node(list,
				  (list_item_destructor_t)css_prop_destroy);
	free(list);
}

css_prop_t *css_style_decl_alloc(css_style_decl_t *list, int key)
{
	css_prop_t *node;

	node = malloc(sizeof(css_prop_t));
	node->key = key;
	node->style.type = CSS_NO_VALUE;
	node->node.data = node;
	list_append_node(list, &node->node);
	return node;
}

void css_style_decl_add(css_style_decl_t *list, int key,
			const css_style_value_t *value)
{
	css_prop_t *prop = css_style_decl_alloc(list, key);
	if (value->type == CSS_ARRAY_VALUE) {
		prop->style = *value;
	} else {
		prop->style.type = CSS_ARRAY_VALUE;
		prop->style.array_value = NULL;
		css_style_value_set_array_length(&prop->style, 1);
		prop->style.array_value[0] = *value;
	}
}

void css_style_decl_set(css_style_decl_t *list, int key,
			const css_style_value_t *value)
{
	css_prop_t *prop;

	prop = css_style_decl_find(list, key);
	if (prop) {
		css_style_value_destroy(&prop->style);
		if (value->type == CSS_ARRAY_VALUE) {
			css_style_value_copy(&prop->style, value);
		} else {
			prop->style.type = CSS_ARRAY_VALUE;
			prop->style.array_value = NULL;
			css_style_value_set_array_length(&prop->style, 1);
			prop->style.array_value[0] = *value;
		}
		return;
	}
	css_style_decl_add(list, key, value);
}

int css_style_decl_remove(css_style_decl_t *list, int key)
{
	list_node_t *node;
	css_prop_t *snode;

	for (list_each(node, list)) {
		snode = node->data;
		if (snode->key == key) {
			list_unlink(list, node);
			css_style_value_destroy(&snode->style);
			free(snode);
			return 0;
		}
	}
	return -1;
}

void css_style_decl_merge(css_style_decl_t *dst, const css_style_decl_t *src)
{
	list_node_t *node;
	css_prop_t *dst_prop, *src_prop;

	for (list_each(node, src)) {
		src_prop = node->data;
		dst_prop = css_style_decl_alloc(dst, src_prop->key);
		css_style_value_copy(&dst_prop->style, &src_prop->style);
	}
}

css_prop_t *css_style_decl_find(css_style_decl_t *list, int key)
{
	list_node_t *node;
	css_prop_t *snode;

	for (list_each(node, list)) {
		snode = node->data;
		if (snode->key == key) {
			return snode;
		}
	}
	return NULL;
}

void css_dump_style_decl(const css_style_decl_t *list, css_dump_context_t *ctx)
{
	list_node_t *node;
	css_prop_t *rule;
	css_propdef_t *prop;

	DUMP("{\n");
	for (list_each(node, list)) {
		rule = node->data;
		if (rule->style.type == CSS_NO_VALUE) {
			continue;
		}
		prop = css_get_propdef(rule->key);
		DUMP("\t");
		if (prop) {
			DUMP(prop->name);
		} else {
			DUMPF("<unknown property %d>", rule->key);
		}
		DUMP(": ");
		css_dump_style_value(&rule->style, ctx);
		DUMP(";\n");
	}
	DUMP("}\n");
}

size_t css_print_style_decl(const css_style_decl_t *s)
{
	css_dump_context_t ctx = {
		.data = NULL, .len = 0, .max_len = 0, .func = css_dump_to_stdout
	};

	css_dump_style_decl(s, &ctx);
	return ctx.len;
}

size_t css_style_decl_to_string(const css_style_decl_t *s, char *str,
				size_t max_len)
{
	css_dump_context_t ctx = { .data = str,
				   .len = 0,
				   .max_len = max_len,
				   .func = css_dump_to_buffer };

	css_dump_style_decl(s, &ctx);
	return ctx.len;
}
