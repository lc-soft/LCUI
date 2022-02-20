#include <errno.h>
#include "../include/css/keywords.h"

typedef struct css_keyword_t {
	int key;
	char *name;
} css_keyword_t;

static struct css_keywords_module_t {
	css_keyword_t **list;
	unsigned used;
	unsigned size;

	/**
	 * dict_t<string, css_keyword_t*>
	 */
	dict_t *name_map;

} css_keywords;

static void keyword_destroy(void *data)
{
	css_keyword_t *kw = data;
	free(kw->name);
	free(kw);
}

static css_keyword_t *keyword_create(int key, const char *name)
{
	css_keyword_t *kw;
	kw = malloc(sizeof(css_keyword_t));
	kw->name = strdup2(name);
	kw->key = key;
	return kw;
}

static int css_set_keyword(int key, const char *name)
{
	unsigned i;
	css_keyword_t **list;
	css_keyword_t *kw;

	if ((unsigned)key >= css_keywords.size) {
		i = css_keywords.size;
		css_keywords.size += 32;
		list = realloc(css_keywords.list,
			       css_keywords.size * sizeof(css_keyword_t *));
		if (!list) {
			return -ENOMEM;
		}
		for (; i < css_keywords.size; ++i) {
			list[i] = NULL;
		}
		css_keywords.list = list;
	}
	assert(css_keywords.list[key] == NULL);
	kw = keyword_create(key, name);
	css_keywords.list[key] = kw;
	css_keywords.used++;
	if (css_keywords.used <= (unsigned)key) {
		css_keywords.used = (unsigned)key + 1;
	}
	if (dict_add(css_keywords.name_map, kw->name, kw) != 0) {
		keyword_destroy(kw);
		return -ENOMEM;
	}
	return 0;
}

int css_register_keyword(const char *name)
{
	int key = (int)css_keywords.used;
	if (css_get_keyword_key(name) > 0) {
		return -1;
	}
	if (css_set_keyword(key, name) == 0) {
		return key;
	}
	return -1;
}

int css_get_keyword_key(const char *name)
{
	css_keyword_t *kw;

	kw = dict_fetch_value(css_keywords.name_map, name);
	if (kw) {
		return kw->key;
	}
	return -1;
}

const char *css_get_keyword_name(int val)
{
	if ((unsigned)val < css_keywords.size && css_keywords.list[val]) {
		return css_keywords.list[val]->name;
	}
	return NULL;
}

void css_init_keywords(void)
{
	static dict_type_t names_dt = { 0 };

	dict_init_string_key_type(&names_dt);
	css_keywords.name_map = dict_create(&names_dt, NULL);
	css_keywords.used = 0;
	css_keywords.size = 0;

	css_set_keyword(CSS_KEYWORD_NONE, "none");
	css_set_keyword(CSS_KEYWORD_AUTO, "auto");
	css_set_keyword(CSS_KEYWORD_INHERIT, "inherit");
	css_set_keyword(CSS_KEYWORD_INITIAL, "initial");
	css_set_keyword(CSS_KEYWORD_NORMAL, "normal");

	css_set_keyword(CSS_KEYWORD_LEFT, "left");
	css_set_keyword(CSS_KEYWORD_CENTER, "center");
	css_set_keyword(CSS_KEYWORD_RIGHT, "right");
	css_set_keyword(CSS_KEYWORD_TOP, "top");
	css_set_keyword(CSS_KEYWORD_MIDDLE, "middle");
	css_set_keyword(CSS_KEYWORD_BOTTOM, "bottom");

	css_set_keyword(CSS_KEYWORD_STATIC, "static");
	css_set_keyword(CSS_KEYWORD_RELATIVE, "relative");
	css_set_keyword(CSS_KEYWORD_ABSOLUTE, "absolute");

	css_set_keyword(CSS_KEYWORD_BLOCK, "block");
	css_set_keyword(CSS_KEYWORD_INLINE_BLOCK, "inline-block");
	css_set_keyword(CSS_KEYWORD_FLEX, "flex");

	css_set_keyword(CSS_KEYWORD_START, "start");
	css_set_keyword(CSS_KEYWORD_END, "end");
	css_set_keyword(CSS_KEYWORD_FLEX_START, "flex-start");
	css_set_keyword(CSS_KEYWORD_FLEX_END, "flex-end");
	css_set_keyword(CSS_KEYWORD_STRETCH, "stretch");
	css_set_keyword(CSS_KEYWORD_SPACE_BETWEEN, "space-between");
	css_set_keyword(CSS_KEYWORD_SPACE_AROUND, "space-around");
	css_set_keyword(CSS_KEYWORD_SPACE_EVENLY, "space-evenly");

	css_set_keyword(CSS_KEYWORD_ROW, "row");
	css_set_keyword(CSS_KEYWORD_COLUMN, "column");

	css_set_keyword(CSS_KEYWORD_NOWRAP, "nowrap");
	css_set_keyword(CSS_KEYWORD_WRAP, "wrap");

	css_set_keyword(CSS_KEYWORD_CONTENT_BOX, "content-box");
	css_set_keyword(CSS_KEYWORD_PADDING_BOX, "padding-box");
	css_set_keyword(CSS_KEYWORD_BORDER_BOX, "border-box");
	css_set_keyword(CSS_KEYWORD_GRAPH_BOX, "graph-box");

	css_set_keyword(CSS_KEYWORD_SOLID, "solid");
	css_set_keyword(CSS_KEYWORD_DOTTED, "dotted");
	css_set_keyword(CSS_KEYWORD_DOUBLE, "double");
	css_set_keyword(CSS_KEYWORD_DASHED, "dashed");

	css_set_keyword(CSS_KEYWORD_CONTAIN, "contain");
	css_set_keyword(CSS_KEYWORD_COVER, "cover");
	css_set_keyword(CSS_KEYWORD_REPEAT, "repeat");
	css_set_keyword(CSS_KEYWORD_REPEAT_X, "repeat-x");
	css_set_keyword(CSS_KEYWORD_REPEAT_Y, "repeat-y");
	css_set_keyword(CSS_KEYWORD_NO_REPEAT, "no-repeat");

	css_set_keyword(CSS_KEYWORD_BREAK_ALL, "break-all");

	css_set_keyword(CSS_KEYWORD_ITALIC, "italic");
	css_set_keyword(CSS_KEYWORD_OBLIQUE, "oblique");
	css_set_keyword(CSS_KEYWORD_SMALL, "small");
	css_set_keyword(CSS_KEYWORD_MEDIUM, "medium");
	css_set_keyword(CSS_KEYWORD_LARGE, "large");
	css_set_keyword(CSS_KEYWORD_BOLD, "bold");

	css_set_keyword(CSS_KEYWORD_VISIBLE, "visible");
	css_set_keyword(CSS_KEYWORD_HIDDEN, "hidden");
}

void css_destroy_keywords(void)
{
	unsigned i;

	dict_destroy(css_keywords.name_map);
	for (i = 0; i < css_keywords.size; ++i) {
		if (css_keywords.list[i]) {
			keyword_destroy(css_keywords.list[i]);
			css_keywords.list[i] = NULL;
		}
	}
	free(css_keywords.list);
	css_keywords.list = NULL;
	css_keywords.name_map = NULL;
	css_keywords.size = 0;
	css_keywords.used = 0;
}
