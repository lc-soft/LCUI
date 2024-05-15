/*
 * lib/pandagl/src/font/library.c: -- The font info and font bitmap cache module.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pandagl.h>
#include "bitmap.h"
#include "incore.h"
#include "freetype.h"

/* clang-format off */

#define FONT_CACHE_SIZE		32
#define FONT_CACHE_MAX_SIZE	1024

/**
 * 库中缓存的字体位图是分组存放的，共有三级分组，分别为：
 * 字符->字体信息->字体大小
 * 获取字体位图时的搜索顺序为：先找到字符的记录，然后在记录中的字体数据库里找
 * 到指定字体样式标识号的字体位图库，之后在字体位图库中找到指定像素大小的字体
 * 位图。
 */

typedef struct font_style_node_t {
	/* 字体列表，按粗细程度存放 */
	pd_font_t *weights[PD_FONT_WEIGHT_TOTAL_NUM];
} font_style_node_t;

typedef struct font_cache_t {
	pd_font_t *fonts[FONT_CACHE_SIZE];
} font_cache_t;

/** 字体字族索引结点 */
typedef struct font_family_node_t {
	/** 字体的字族名称  */
	char *family_name;

	/** 字体列表，按风格存放 */
	font_style_node_t styles[PD_FONT_STYLE_TOTAL_NUM];
} font_family_node_t;

static struct font_library_module_t {
	int count;			/**< 计数器，主要用于为字体信息生成标识号 */
	int font_cache_num;		/**< 字体信息缓存区的数量 */
	bool active;		/**< 标记，指示数据库是否初始化 */
	dict_t *font_families;		/**< 字族信息库，以字族名称索引字体信息 */
	dict_type_t font_families_type;	/**< 字族信息库的字典类型数据 */
	rbtree_t bitmap_cache;		/**< 字体位图缓存区 */
	font_cache_t **font_cache;	/**< 字体信息缓存区 */
	pd_font_t *default_font;		/**< 默认字体的信息 */
	pd_font_t *incore_font;		/**< 内置字体的信息 */
	font_engine_t engines[2];	/**< 当前可用字体引擎列表 */
	font_engine_t *engine;		/**< 当前选择的字体引擎 */
} fontlib;

/* clang-format on */

PD_INLINE rbtree_t *select_char_cache(wchar_t ch)
{
	return rbtree_get_data_by_key(&fontlib.bitmap_cache, ch);
}

PD_INLINE rbtree_t *select_font_cache(rbtree_t *font_cache, int font_id)
{
	return rbtree_get_data_by_key(font_cache, font_id);
}

PD_INLINE pd_font_bitmap_t *select_bitmap_cache(rbtree_t *bmp_cache, int size)
{
	return rbtree_get_data_by_key(bmp_cache, size);
}

PD_INLINE font_family_node_t *select_font_family_cache(const char *family_name)
{
	return dict_fetch_value(fontlib.font_families, family_name);
}

#ifdef PANDAGL_HAS_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

char *pd_font_library_get_font_path(const char *name)
{
#ifdef PANDAGL_HAS_FONTCONFIG
	char *path = NULL;
	size_t path_len;

	FcResult result;
	FcPattern *font;
	FcChar8 *file = NULL;
	FcConfig *config = FcInitLoadConfigAndFonts();
	FcPattern *pat = FcNameParse((const FcChar8 *)name);

	FcConfigSubstitute(config, pat, FcMatchPattern);
	FcDefaultSubstitute(pat);

	if ((font = FcFontMatch(config, pat, &result))) {
		if (FcPatternGetString(font, FC_FILE, 0, &file) ==
		    FcResultMatch) {
			path_len = strlen((char *)file);
			path = (char *)malloc(path_len + 1);
			strncpy(path, (char *)file, path_len);
			path[path_len] = 0;
		}
		FcPatternDestroy(font);
	}

	FcPatternDestroy(pat);
	FcConfigDestroy(config);

	return path;
#else
	return NULL;
#endif
}

pd_font_t *pd_font_create(const char *family_name, const char *style_name)
{
	pd_font_t *font;

	font = malloc(sizeof(pd_font_t));
	font->id = 0;
	font->data = NULL;
	font->engine = NULL;
	font->family_name = strdup2(family_name);
	font->style_name = strdup2(style_name);
	font->weight = pd_font_library_detect_weight(style_name);
	font->style = pd_font_library_detect_style(style_name);
	return font;
}

void pd_font_destroy(pd_font_t *font)
{
	free(font->family_name);
	free(font->style_name);
	font->engine->close(font->data);
	font->data = NULL;
	font->engine = NULL;
	free(font);
}

static void destroy_font_family_node(void *privdata, void *data)
{
	font_family_node_t *node = data;
	if (node->family_name) {
		free(node->family_name);
	}
	node->family_name = NULL;
	memset(node->styles, 0, sizeof(node->styles));
	free(node);
}

static void destroy_font_bitmap(void *arg)
{
	pd_font_bitmap_destroy(arg);
	free(arg);
}

static void destroy_tree_node(void *arg)
{
	rbtree_destroy(arg);
	free(arg);
}

pd_font_bitmap_t *pd_font_library_add_bitmap(wchar_t ch, int font_id, int size,
					  const pd_font_bitmap_t *bmp)
{
	pd_font_bitmap_t *bmp_cache;
	rbtree_t *tree_font, *tree_bmp;

	if (!fontlib.active) {
		return NULL;
	}
	/* 获取字符的字体信息集 */
	tree_font = select_char_cache(ch);
	if (!tree_font) {
		tree_font = malloc(sizeof(rbtree_t));
		if (!tree_font) {
			return NULL;
		}
		rbtree_init(tree_font);
		rbtree_set_destroy_func(tree_font, destroy_tree_node);
		rbtree_insert_by_key(&fontlib.bitmap_cache, ch, tree_font);
	}
	/* 当字体ID不大于0时，使用内置字体 */
	if (font_id <= 0) {
		font_id = fontlib.incore_font->id;
	}
	/* 获取相应字体样式标识号的字体位图库 */
	tree_bmp = select_font_cache(tree_font, font_id);
	if (!tree_bmp) {
		tree_bmp = malloc(sizeof(rbtree_t));
		if (!tree_bmp) {
			return NULL;
		}
		rbtree_init(tree_bmp);
		rbtree_set_destroy_func(tree_bmp, destroy_font_bitmap);
		rbtree_insert_by_key(tree_font, font_id, tree_bmp);
	}
	/* 在字体位图库中获取指定像素大小的字体位图 */
	bmp_cache = select_bitmap_cache(tree_bmp, size);
	if (!bmp_cache) {
		bmp_cache = malloc(sizeof(pd_font_bitmap_t));
		if (!bmp_cache) {
			return NULL;
		}
		rbtree_insert_by_key(tree_bmp, size, bmp_cache);
	}
	/* 拷贝数据至该空间内 */
	memcpy(bmp_cache, bmp, sizeof(pd_font_bitmap_t));
	return bmp_cache;
}

int pd_font_library_get_bitmap(unsigned ch, int font_id, int size,
			       const pd_font_bitmap_t **bmp)
{
	int ret;
	rbtree_t *ctx;
	pd_font_bitmap_t bmp_cache;

	*bmp = NULL;
	if (!fontlib.active) {
		return -2;
	}
	if (font_id <= 0) {
		if (fontlib.default_font) {
			font_id = fontlib.default_font->id;
		} else {
			font_id = fontlib.incore_font->id;
		}
	}
	do {
		if (!(ctx = select_char_cache(ch))) {
			break;
		}
		ctx = select_font_cache(ctx, font_id);
		if (!ctx) {
			break;
		}
		*bmp = select_bitmap_cache(ctx, size);
		if (*bmp) {
			return 0;
		}
		break;
	} while (0);
	if (ch == 0) {
		return -1;
	}
	pd_font_bitmap_init(&bmp_cache);
	ret = pd_font_library_render_bitmap(&bmp_cache, ch, font_id, size);
	if (ret == 0) {
		*bmp =
		    pd_font_library_add_bitmap(ch, font_id, size, &bmp_cache);
		return 0;
	}
	ret = pd_font_library_get_bitmap(0, font_id, size, bmp);
	if (ret != 0) {
		*bmp = pd_font_library_add_bitmap(0, font_id, size, &bmp_cache);
	}
	return -1;
}

static font_cache_t *font_cache_create(void)
{
	font_cache_t *cache;
	if (!(cache = malloc(sizeof(font_cache_t)))) {
		return NULL;
	}
	memset(cache->fonts, 0, sizeof(cache->fonts));
	return cache;
}

static void font_cache_destroy(font_cache_t *cache)
{
	int i;
	for (i = 0; i < FONT_CACHE_SIZE; ++i) {
		if (cache->fonts[i]) {
			pd_font_destroy(cache->fonts[i]);
		}
		cache->fonts[i] = NULL;
	}
	free(cache);
}

static pd_font_t *pd_font_library_get_cached_font(int id)
{
	if (id > fontlib.font_cache_num * FONT_CACHE_SIZE) {
		return NULL;
	}
	return fontlib.font_cache[id / FONT_CACHE_SIZE]
	    ->fonts[id % FONT_CACHE_SIZE];
}

static int pd_font_library_add_cached_font(pd_font_t *font)
{
	size_t size;
	font_cache_t **caches, *cache;

	if (font->id > FONT_CACHE_MAX_SIZE) {
		logger_error("[font] font cache size is the max size\n");
		return -1;
	}
	while (font->id >= fontlib.font_cache_num * FONT_CACHE_SIZE) {
		fontlib.font_cache_num += 1;
		size = fontlib.font_cache_num * sizeof(font_cache_t);
		caches = realloc(fontlib.font_cache, size);
		if (!caches) {
			fontlib.font_cache_num -= 1;
			return -ENOMEM;
		}
		cache = font_cache_create();
		if (!cache) {
			return -ENOMEM;
		}
		caches[fontlib.font_cache_num - 1] = cache;
		fontlib.font_cache = caches;
	}
	fontlib.font_cache[font->id / FONT_CACHE_SIZE]
	    ->fonts[font->id % FONT_CACHE_SIZE] = font;
	return 0;
}

pd_font_weight_t pd_font_library_detect_weight(const char *str)
{
	char *buf;
	pd_font_weight_t weight = PD_FONT_WEIGHT_NORMAL;

	buf = strdup2(str);
	if (!buf) {
		return weight;
	}
	strtolower(buf, str);
	if (strstr(buf, "thin")) {
		weight = PD_FONT_WEIGHT_THIN;
	} else if (strstr(buf, "semilight")) {
		weight = PD_FONT_WEIGHT_LIGHT;
	} else if (strstr(buf, "light")) {
		weight = PD_FONT_WEIGHT_EXTRA_LIGHT;
	} else if (strstr(buf, "medium")) {
		weight = PD_FONT_WEIGHT_MEDIUM;
	} else if (strstr(buf, "semibold")) {
		weight = PD_FONT_WEIGHT_SEMI_BOLD;
	} else if (strstr(buf, "bold")) {
		weight = PD_FONT_WEIGHT_BOLD;
	} else if (strstr(buf, "black")) {
		weight = PD_FONT_WEIGHT_BLACK;
	}
	free(buf);
	return weight;
}

pd_font_style_t pd_font_library_detect_style(const char *str)
{
	char *buf;
	pd_font_style_t style = PD_FONT_STYLE_NORMAL;

	buf = strdup2(str);
	if (!buf) {
		return style;
	}
	strtolower(buf, str);
	if (strstr(buf, "oblique")) {
		style = PD_FONT_STYLE_OBLIQUE;
	} else if (strstr(buf, "italic")) {
		style = PD_FONT_STYLE_ITALIC;
	}
	free(buf);
	return style;
}

int pd_font_library_add_font(pd_font_t *font)
{
	pd_font_t *exists_font;
	font_family_node_t *node;
	font_style_node_t *style_node;

	node = select_font_family_cache(font->family_name);
	if (!node) {
		node = malloc(sizeof(font_family_node_t));
		node->family_name = strdup2(font->family_name);
		memset(node->styles, 0, sizeof(node->styles));
		dict_add(fontlib.font_families, node->family_name, node);
	}
	style_node = node->styles + font->style;
	exists_font = style_node->weights[font->weight- 1];
	if (exists_font) {
		font->id = exists_font->id;
		if (fontlib.default_font &&
		    font->id == fontlib.default_font->id) {
			fontlib.default_font = font;
		}
		style_node->weights[font->weight - 1] = NULL;
		pd_font_destroy(exists_font);
	} else {
		font->id = ++fontlib.count;
	}
	style_node->weights[font->weight - 1] = font;
	pd_font_library_add_cached_font(font);
	return font->id;
}

pd_font_t *pd_font_library_get_font(int id)
{
	if (!fontlib.active) {
		return NULL;
	}
	if (id < 0 || id >= fontlib.font_cache_num * FONT_CACHE_SIZE) {
		return NULL;
	}
	return pd_font_library_get_cached_font(id);
}

static pd_font_weight_t find_bolder_weight(font_style_node_t *snode,
					pd_font_weight_t weight)
{
	for (weight += 1; weight <= PD_FONT_WEIGHT_BLACK; weight += 1) {
		if (snode->weights[weight]) {
			return weight;
		}
	}
	return PD_FONT_WEIGHT_NONE;
}

static pd_font_weight_t find_lighter_weight(font_style_node_t *snode,
					 pd_font_weight_t weight)
{
	for (weight -= 1; weight >= PD_FONT_WEIGHT_THIN; weight -= 1) {
		if (snode->weights[weight]) {
			return weight;
		}
	}
	return PD_FONT_WEIGHT_NONE;
}

/**
 * 在未找到指定字重的字体时进行回退，找到合适的字体
 * 回退规则的参考文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight#Fallback_weights
 */
static pd_font_weight_t font_weight_fallback(font_style_node_t *snode,
					  pd_font_weight_t weight)
{
	if (weight > PD_FONT_WEIGHT_MEDIUM) {
		return find_bolder_weight(snode, weight);
	}
	if (weight < PD_FONT_WEIGHT_NORMAL) {
		return find_lighter_weight(snode, weight);
	}
	if (weight == PD_FONT_WEIGHT_NORMAL) {
		if (snode->weights[PD_FONT_WEIGHT_MEDIUM - 1]) {
			return PD_FONT_WEIGHT_MEDIUM;
		}
	} else if (weight == PD_FONT_WEIGHT_MEDIUM) {
		if (snode->weights[PD_FONT_WEIGHT_NORMAL - 1]) {
			return PD_FONT_WEIGHT_NORMAL;
		}
	}
	weight = find_lighter_weight(snode, weight);
	if (weight != PD_FONT_WEIGHT_NONE) {
		return weight;
	}
	return PD_FONT_WEIGHT_NONE;
}

int pd_font_library_get_font_id(const char *family_name, pd_font_style_t style,
				pd_font_weight_t weight)
{
	int style_num;
	pd_font_weight_t w;
	font_style_node_t *snode;
	font_family_node_t *fnode;

	if (!fontlib.active) {
		return -1;
	}
	fnode = select_font_family_cache(family_name);
	if (!fnode) {
		return -2;
	}
	if (weight == 0) {
		weight = PD_FONT_WEIGHT_NORMAL;
	}
	for (style_num = style; style_num >= 0; --style_num) {
		snode = &fnode->styles[style_num];
		if (snode->weights[weight - 1]) {
			return snode->weights[weight - 1]->id;
		}
		w = font_weight_fallback(snode, weight);
		if (w) {
			return snode->weights[w - 1]->id;
		}
	}
	return -3;
}

size_t pd_font_library_update_font_weight(const int *font_ids,
					  pd_font_weight_t weight,
					  int **new_font_ids)
{
	int id, *ids;
	pd_font_t *font;
	size_t i, count, len;

	if (!font_ids) {
		return 0;
	}
	for (len = 0; font_ids[len]; ++len)
		;
	if (len < 1) {
		return 0;
	}
	ids = malloc((len + 1) * sizeof(int));
	if (!ids) {
		return 0;
	}
	for (i = 0, count = 0; i < len; ++i) {
		font = pd_font_library_get_font(font_ids[i]);
		id = pd_font_library_get_font_id(font->family_name, font->style,
						 weight);
		if (id > 0) {
			ids[count++] = id;
		}
	}
	ids[count] = 0;
	if (new_font_ids && count > 0) {
		*new_font_ids = ids;
	} else {
		*new_font_ids = NULL;
		free(ids);
	}
	return count;
}

size_t pd_font_library_update_font_style(const int *font_ids,
					 pd_font_style_t style, int **new_font_ids)
{
	int id, *ids;
	pd_font_t *font;
	size_t i, count, len;

	if (!font_ids) {
		return 0;
	}
	for (len = 0; font_ids[len]; ++len)
		;
	if (len < 1) {
		return 0;
	}
	ids = malloc((len + 1) * sizeof(int));
	if (!ids) {
		return 0;
	}
	for (i = 0, count = 0; i < len; ++i) {
		font = pd_font_library_get_font(font_ids[i]);
		id = pd_font_library_get_font_id(font->family_name, style,
						 font->weight);
		if (id > 0) {
			ids[count++] = id;
		}
	}
	ids[count] = 0;
	if (new_font_ids && count > 0) {
		*new_font_ids = ids;
	} else {
		*new_font_ids = NULL;
		free(ids);
	}
	return count;
}

unsigned pd_font_library_query(int **font_ids, pd_font_style_t style,
			       pd_font_weight_t weight, const char *const *names)
{
	int *ids;
	unsigned i, count, loaded_count;

	*font_ids = NULL;
	if (!names) {
		return 0;
	}
	for (count = 0; names[count]; ++count)
		;
	if (count < 1) {
		return 0;
	}
	ids = malloc(sizeof(int) * (count + 1));
	if (!ids) {
		return 0;
	}
	for (loaded_count = 0, i = 0; i < count; ++i) {
		ids[loaded_count] =
		    pd_font_library_get_font_id(names[i], style, weight);
		if (ids[loaded_count] > 0) {
			++loaded_count;
		}
	}
	ids[loaded_count] = 0;
	if (loaded_count < 1) {
		free(ids);
		ids = NULL;
	}
	*font_ids = ids;
	return loaded_count;
}

int pd_font_library_get_default_font(void)
{
	if (!fontlib.default_font) {
		return -1;
	}
	return fontlib.default_font->id;
}

void pd_font_library_set_default_font(int id)
{
	pd_font_t *font = pd_font_library_get_font(id);
	if (font) {
		fontlib.default_font = font;
		logger_debug("[font] select: %s\n", font->family_name);
	}
}

int pd_font_library_load_file(const char *filepath)
{
	pd_font_t **fonts;
	int i, num_fonts, id;

	logger_debug("[font] load file: %s\n", filepath);
	if (!fontlib.engine) {
		return -1;
	}
	num_fonts = fontlib.engine->open(filepath, &fonts);
	if (num_fonts < 1) {
		logger_debug("[font] failed to load file: %s\n", filepath);
		return -2;
	}
	for (i = 0; i < num_fonts; ++i) {
		fonts[i]->engine = fontlib.engine;
		id = pd_font_library_add_font(fonts[i]);
		logger_debug("[font] add font: %d, family: %s, style name: %s, "
			     "weight: %d\n",
			     id, fonts[i]->family_name, fonts[i]->style_name,
			     fonts[i]->weight);
	}
	free(fonts);
	return 0;
}

int pd_font_library_render_bitmap(pd_font_bitmap_t *buff, unsigned ch, int font_id,
				  int pixel_size)
{
	pd_font_t *font = fontlib.default_font;
	do {
		if (font_id < 0 || !fontlib.engine) {
			break;
		}
		font = pd_font_library_get_font(font_id);
		if (font) {
			break;
		}
		if (fontlib.default_font) {
			font = fontlib.default_font;
		} else {
			font = fontlib.incore_font;
		}
		break;
	} while (0);
	if (!font) {
		return -1;
	}
	return font->engine->render(buff, ch, pixel_size, font);
}

static void pd_font_library_init_base(void)
{
	fontlib.count = 0;
	fontlib.font_cache_num = 1;
	fontlib.font_cache = malloc(sizeof(font_cache_t));
	fontlib.font_cache[0] = font_cache_create();
	rbtree_init(&fontlib.bitmap_cache);
	dict_init_string_key_type(&fontlib.font_families_type);
	fontlib.font_families_type.val_destructor = destroy_font_family_node;
	fontlib.font_families = dict_create(&fontlib.font_families_type, NULL);
	rbtree_set_destroy_func(&fontlib.bitmap_cache, destroy_tree_node);
	fontlib.active = PD_TRUE;
}

static void pd_font_library_init_engine(void)
{
	int fid;
	/* 先初始化内置的字体引擎 */
	fontlib.engine = &fontlib.engines[0];
	pd_incore_font_create(fontlib.engine);
	pd_font_library_load_file("in-core.inconsolata");
	fid = pd_font_library_get_font_id("inconsolata", 0, 0);
	fontlib.incore_font = pd_font_library_get_font(fid);
	fontlib.default_font = fontlib.incore_font;
	/* 然后看情况启用其它字体引擎 */
#ifdef PANDAGL_HAS_FREETYPE
	if (pd_freetype_engine_init(&fontlib.engines[1]) == 0) {
		fontlib.engine = &fontlib.engines[1];
	}
#endif
	if (fontlib.engine && fontlib.engine != &fontlib.engines[0]) {
		logger_debug("[font] current font engine is: %s\n",
			     fontlib.engine->name);
	} else {
		logger_warning("[font] warning: not font engine support!\n");
	}
}

static void pd_font_library_destroy_base(void)
{
	if (!fontlib.active) {
		return;
	}
	fontlib.active = PD_FALSE;
	while (fontlib.font_cache_num > 0) {
		--fontlib.font_cache_num;
		font_cache_destroy(fontlib.font_cache[fontlib.font_cache_num]);
	}
	dict_destroy(fontlib.font_families);
	rbtree_destroy(&fontlib.bitmap_cache);
	free(fontlib.font_cache);
	fontlib.font_cache = NULL;
}

static void pd_font_library_destroy_engine(void)
{
	pd_incore_font_destroy();
#ifdef PANDAGL_HAS_FREETYPE
	pd_freetype_engine_destroy();
#endif
}

void pd_font_library_init(void)
{
	pd_font_library_init_base();
	pd_font_library_init_engine();
}

void pd_font_library_destroy(void)
{
	pd_font_library_destroy_base();
	pd_font_library_destroy_engine();
}
