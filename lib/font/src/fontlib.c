/*
 * fontlibrary.c -- The font info and font bitmap cache module.
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

#include "config.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/util.h>
#include "internal.h"

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
	font_t *weights[FONT_WEIGHT_TOTAL_NUM];
} font_style_node_t;

typedef struct font_cache_t {
	font_t *fonts[FONT_CACHE_SIZE];
} font_cache_t;

/** 字体字族索引结点 */
typedef struct font_family_node_t {
	/** 字体的字族名称  */
	char *family_name;

	/** 字体列表，按风格存放 */
	font_style_node_t styles[FONT_STYLE_TOTAL_NUM];
} font_family_node_t;

static struct font_library_module_t {
	int count;			/**< 计数器，主要用于为字体信息生成标识号 */
	int font_cache_num;		/**< 字体信息缓存区的数量 */
	LCUI_BOOL active;		/**< 标记，指示数据库是否初始化 */
	dict_t *font_families;		/**< 字族信息库，以字族名称索引字体信息 */
	dict_type_t font_families_type;	/**< 字族信息库的字典类型数据 */
	rbtree_t bitmap_cache;		/**< 字体位图缓存区 */
	font_cache_t **font_cache;	/**< 字体信息缓存区 */
	font_t *default_font;		/**< 默认字体的信息 */
	font_t *incore_font;		/**< 内置字体的信息 */
	font_engine_t engines[2];	/**< 当前可用字体引擎列表 */
	font_engine_t *engine;		/**< 当前选择的字体引擎 */
} fontlib;

/* clang-format on */

#define select_char_cache(ch) \
	(rbtree_t *)rbtree_get_data_by_key(&fontlib.bitmap_cache, ch)
#define select_font_cache(ch, font_id) \
	(rbtree_t *)rbtree_get_data_by_key(ch, font_id)
#define select_bitmap_cache(font, size) \
	(font_bitmap_t *)rbtree_get_data_by_key(font, size)
#define select_font_family_cache(family_name)                         \
	(font_family_node_t *)dict_fetch_value(fontlib.font_families, \
					       family_name);
#define select_font_style_cache(FNODE, S) (&(FNODE)->styles[S])
#define select_font_weight_cache(SNODE, W) ((SNODE)->weights[W / 100 - 1])
#define clear_font_weight_cache(SNODE, W)             \
	do {                                          \
		(SNODE)->weights[W / 100 - 1] = NULL; \
	} while (0);
#define set_font_weight_cache(SNODE, FONT)                         \
	do {                                                       \
		(SNODE)->weights[(FONT)->weight / 100 - 1] = FONT; \
	} while (0);

#ifndef LCUI_PLATFORM_WIN32

#ifdef USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

char *fontlib_get_font_path(const char *name)
{
#ifdef USE_FONTCONFIG
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

#endif

font_t *font_create(const char *family_name, const char *style_name)
{
	font_t *font;

	font = malloc(sizeof(font_t));
	font->id = 0;
	font->data = NULL;
	font->engine = NULL;
	font->family_name = strdup2(family_name);
	font->style_name = strdup2(style_name);
	font->weight = fontlib_detect_weight(style_name);
	font->style = fontlib_detect_style(style_name);
	return font;
}

void font_destroy(font_t *font)
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
	font_bitmap_destroy(arg);
	free(arg);
}

static void destroy_tree_node(void *arg)
{
	rbtree_destroy(arg);
	free(arg);
}

font_bitmap_t *fontlib_add_bitmap(wchar_t ch, int font_id, int size,
				  const font_bitmap_t *bmp)
{
	font_bitmap_t *bmp_cache;
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
		bmp_cache = malloc(sizeof(font_bitmap_t));
		if (!bmp_cache) {
			return NULL;
		}
		rbtree_insert_by_key(tree_bmp, size, bmp_cache);
	}
	/* 拷贝数据至该空间内 */
	memcpy(bmp_cache, bmp, sizeof(font_bitmap_t));
	return bmp_cache;
}

int fontlib_get_bitmap(unsigned ch, int font_id, int size,
		       const font_bitmap_t **bmp)
{
	int ret;
	rbtree_t *ctx;
	font_bitmap_t bmp_cache;

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
	font_bitmap_init(&bmp_cache);
	ret = fontlib_render_bitmap(&bmp_cache, ch, font_id, size);
	if (ret == 0) {
		*bmp = fontlib_add_bitmap(ch, font_id, size, &bmp_cache);
		return 0;
	}
	ret = fontlib_get_bitmap(0, font_id, size, bmp);
	if (ret != 0) {
		*bmp = fontlib_add_bitmap(0, font_id, size, &bmp_cache);
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
			font_destroy(cache->fonts[i]);
		}
		cache->fonts[i] = NULL;
	}
	free(cache);
}

static font_t *fontlib_get_cached_font(int id)
{
	if (id > fontlib.font_cache_num * FONT_CACHE_SIZE) {
		return NULL;
	}
	return fontlib.font_cache[id / FONT_CACHE_SIZE]
	    ->fonts[id % FONT_CACHE_SIZE];
}

static int fontlib_add_cached_font(font_t *font)
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

font_weight_t fontlib_detect_weight(const char *str)
{
	char *buf;
	font_weight_t weight = FONT_WEIGHT_NORMAL;

	buf = strdup2(str);
	if (!buf) {
		return weight;
	}
	strtolower(buf, str);
	if (strstr(buf, "thin")) {
		weight = FONT_WEIGHT_THIN;
	} else if (strstr(buf, "semilight")) {
		weight = FONT_WEIGHT_LIGHT;
	} else if (strstr(buf, "light")) {
		weight = FONT_WEIGHT_EXTRA_LIGHT;
	} else if (strstr(buf, "medium")) {
		weight = FONT_WEIGHT_MEDIUM;
	} else if (strstr(buf, "semibold")) {
		weight = FONT_WEIGHT_SEMI_BOLD;
	} else if (strstr(buf, "bold")) {
		weight = FONT_WEIGHT_BOLD;
	} else if (strstr(buf, "black")) {
		weight = FONT_WEIGHT_BLACK;
	}
	free(buf);
	return weight;
}

font_style_t fontlib_detect_style(const char *str)
{
	char *buf;
	font_style_t style = FONT_STYLE_NORMAL;

	buf = strdup2(str);
	if (!buf) {
		return style;
	}
	strtolower(buf, str);
	if (strstr(buf, "oblique")) {
		style = FONT_STYLE_OBLIQUE;
	} else if (strstr(buf, "italic")) {
		style = FONT_STYLE_ITALIC;
	}
	free(buf);
	return style;
}

int fontlib_add_font(font_t *font)
{
	font_t *exists_font;
	font_family_node_t *node;
	font_style_node_t *style_node;

	node = select_font_family_cache(font->family_name);
	if (!node) {
		node = malloc(sizeof(font_family_node_t));
		node->family_name = strdup2(font->family_name);
		memset(node->styles, 0, sizeof(node->styles));
		dict_add(fontlib.font_families, node->family_name, node);
	}
	style_node = select_font_style_cache(node, font->style);
	exists_font = select_font_weight_cache(style_node, font->weight);
	if (exists_font) {
		font->id = exists_font->id;
		if (fontlib.default_font &&
		    font->id == fontlib.default_font->id) {
			fontlib.default_font = font;
		}
		clear_font_weight_cache(style_node, font->weight);
		font_destroy(exists_font);
	} else {
		font->id = ++fontlib.count;
	}
	set_font_weight_cache(style_node, font);
	fontlib_add_cached_font(font);
	return font->id;
}

font_t *fontlib_get_font(int id)
{
	if (!fontlib.active) {
		return NULL;
	}
	if (id < 0 || id >= fontlib.font_cache_num * FONT_CACHE_SIZE) {
		return NULL;
	}
	return fontlib_get_cached_font(id);
}

static font_weight_t find_bolder_weight(font_style_node_t *snode,
					font_weight_t weight)
{
	for (weight += 100; weight <= FONT_WEIGHT_BLACK; weight += 100) {
		if (select_font_weight_cache(snode, weight)) {
			return weight;
		}
	}
	return FONT_WEIGHT_NONE;
}

static font_weight_t find_lighter_weight(font_style_node_t *snode,
					 font_weight_t weight)
{
	for (weight -= 100; weight >= FONT_WEIGHT_THIN; weight -= 100) {
		if (select_font_weight_cache(snode, weight)) {
			return weight;
		}
	}
	return FONT_WEIGHT_NONE;
}

/**
 * 在未找到指定字重的字体时进行回退，找到合适的字体
 * 回退规则的参考文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight#Fallback_weights
 */
static font_weight_t font_weight_fallback(font_style_node_t *snode,
					  font_weight_t weight)
{
	if (weight > FONT_WEIGHT_MEDIUM) {
		return find_bolder_weight(snode, weight);
	}
	if (weight < FONT_WEIGHT_NORMAL) {
		return find_lighter_weight(snode, weight);
	}
	if (weight == FONT_WEIGHT_NORMAL) {
		if (select_font_weight_cache(snode, FONT_WEIGHT_MEDIUM)) {
			return FONT_WEIGHT_MEDIUM;
		}
	} else if (weight == FONT_WEIGHT_MEDIUM) {
		if (select_font_weight_cache(snode, FONT_WEIGHT_NORMAL)) {
			return FONT_WEIGHT_NORMAL;
		}
	}
	weight = find_lighter_weight(snode, weight);
	if (weight != FONT_WEIGHT_NONE) {
		return weight;
	}
	return FONT_WEIGHT_NONE;
}

int fontlib_get_font_id(const char *family_name, font_style_t style,
			font_weight_t weight)
{
	int style_num;
	font_weight_t w;
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
		weight = FONT_WEIGHT_NORMAL;
	}
	for (style_num = style; style_num >= 0; --style_num) {
		snode = &fnode->styles[style_num];
		if (select_font_weight_cache(snode, weight)) {
			return select_font_weight_cache(snode, weight)->id;
		}
		w = font_weight_fallback(snode, weight);
		if (w) {
			return select_font_weight_cache(snode, w)->id;
		}
	}
	return -3;
}

size_t fontlib_update_font_weight(const int *font_ids, font_weight_t weight,
				  int **new_font_ids)
{
	int id, *ids;
	font_t *font;
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
		font = fontlib_get_font(font_ids[i]);
		id =
		    fontlib_get_font_id(font->family_name, font->style, weight);
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

size_t fontlib_update_font_style(const int *font_ids, font_style_t style,
				 int **new_font_ids)
{
	int id, *ids;
	font_t *font;
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
		font = fontlib_get_font(font_ids[i]);
		id =
		    fontlib_get_font_id(font->family_name, style, font->weight);
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

size_t fontlib_query(int **font_ids, font_style_t style, font_weight_t weight,
		     const char *names)
{
	int *ids;
	char name[256];
	const char *p;
	size_t count, i;

	*font_ids = NULL;
	if (!names) {
		return 0;
	}
	for (p = names, count = 1; *p; ++p) {
		if (*p == ',') {
			++count;
		}
	}
	if (p - names == 0) {
		return 0;
	}
	ids = malloc(sizeof(int) * (count + 1));
	if (!ids) {
		return 0;
	}
	for (p = names, count = 0, i = 0;; ++p) {
		if (*p != ',' && *p) {
			name[i++] = *p;
			continue;
		}
		name[i] = 0;
		strtrim(name, name, "'\"\n\r\t ");
		ids[count] = fontlib_get_font_id(name, style, weight);
		if (ids[count] > 0) {
			++count;
		}
		i = 0;
		if (!*p) {
			break;
		}
	}
	ids[count] = 0;
	if (count < 1) {
		free(ids);
		ids = NULL;
	}
	*font_ids = ids;
	return count;
}

int fontlib_get_default_font(void)
{
	if (!fontlib.default_font) {
		return -1;
	}
	return fontlib.default_font->id;
}

void fontlib_set_default_font(int id)
{
	font_t *font = fontlib_get_font(id);
	if (font) {
		fontlib.default_font = font;
		logger_debug("[font] select: %s\n", font->family_name);
	}
}

int fontlib_load_file(const char *filepath)
{
	font_t **fonts;
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
		id = fontlib_add_font(fonts[i]);
		logger_debug("[font] add font: %d, family: %s, style name: %s, "
			     "weight: %d\n",
			     id, fonts[i]->family_name, fonts[i]->style_name,
			     fonts[i]->weight);
	}
	free(fonts);
	return 0;
}

int fontlib_render_bitmap(font_bitmap_t *buff, unsigned ch, int font_id,
			  int pixel_size)
{
	font_t *font = fontlib.default_font;
	do {
		if (font_id < 0 || !fontlib.engine) {
			break;
		}
		font = fontlib_get_font(font_id);
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

static void fontlib_init_base(void)
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
	fontlib.active = TRUE;
}

static void fontlib_init_engine(void)
{
	int fid;
	/* 先初始化内置的字体引擎 */
	fontlib.engine = &fontlib.engines[0];
	incore_font_engine_create(fontlib.engine);
	fontlib_load_file("in-core.inconsolata");
	fid = fontlib_get_font_id("inconsolata", 0, 0);
	fontlib.incore_font = fontlib_get_font(fid);
	fontlib.default_font = fontlib.incore_font;
	/* 然后看情况启用其它字体引擎 */
#ifdef USE_FREETYPE
	if (freetype_engine_init(&fontlib.engines[1]) == 0) {
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

static void fontlib_destroy_base(void)
{
	if (!fontlib.active) {
		return;
	}
	fontlib.active = FALSE;
	while (fontlib.font_cache_num > 0) {
		--fontlib.font_cache_num;
		font_cache_destroy(fontlib.font_cache[fontlib.font_cache_num]);
	}
	dict_destroy(fontlib.font_families);
	rbtree_destroy(&fontlib.bitmap_cache);
	free(fontlib.font_cache);
	fontlib.font_cache = NULL;
}

static void fontlib_destroy_engine(void)
{
	incore_font_engine_destroy();
#ifdef USE_FREETYPE
	freetype_engine_destroy();
#endif
}

void fontlib_init(void)
{
	fontlib_init_base();
	fontlib_init_engine();
}

void fontlib_destroy(void)
{
	fontlib_destroy_base();
	fontlib_destroy_engine();
}
