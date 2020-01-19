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
#include <LCUI_Build.h>
#include <LCUI/types.h>
#include <LCUI/util.h>
#include <LCUI/graph.h>
#include <LCUI/font.h>

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

typedef struct LCUI_FontStyleNodeRec_ {
	/* 字体列表，按粗细程度存放 */
	LCUI_Font weights[FONT_WEIGHT_TOTAL_NUM];
} LCUI_FontStyleNodeRec, *LCUI_FontStyleNode;

typedef LCUI_FontStyleNodeRec LCUI_FontStyleList[FONT_STYLE_TOTAL_NUM];

typedef struct LCUI_FontCacheRec {
	LCUI_Font fonts[FONT_CACHE_SIZE];
} LCUI_FontCacheRec, *LCUI_FontCache;

/** 字体字族索引结点 */
typedef struct LCUI_FontFamilyNodeRec_ {
	char *family_name;		/**< 字体的字族名称  */
	LCUI_FontStyleList styles;	/**< 字体列表，按风格存放 */
} LCUI_FontFamilyNodeRec, *LCUI_FontFamilyNode;

/** 字体路径索引结点 */
typedef struct LCUI_FontPathNode {
	char *path;		/**< 路径  */
	LCUI_Font font;		/**< 被索引的字体信息 */
} LCUI_FontPathNode;

static struct LCUI_FontLibraryModule {
	int count;			/**< 计数器，主要用于为字体信息生成标识号 */
	int font_cache_num;		/**< 字体信息缓存区的数量 */
	LCUI_BOOL active;		/**< 标记，指示数据库是否初始化 */
	Dict *font_families;		/**< 字族信息库，以字族名称索引字体信息 */
	DictType font_families_type;	/**< 字族信息库的字典类型数据 */
	RBTree bitmap_cache;		/**< 字体位图缓存区 */
	LCUI_FontCache *font_cache;	/**< 字体信息缓存区 */
	LCUI_Font default_font;		/**< 默认字体的信息 */
	LCUI_Font incore_font;		/**< 内置字体的信息 */
	LCUI_FontEngine engines[2];	/**< 当前可用字体引擎列表 */
	LCUI_FontEngine *engine;	/**< 当前选择的字体引擎 */
} fontlib;

/* clang-format on */

#define FontBitmap_IsValid(fbmp) \
	((fbmp) && (fbmp)->width > 0 && (fbmp)->rows > 0)
#define SelectChar(ch) (RBTree *)RBTree_GetData(&fontlib.bitmap_cache, ch)
#define SelectFont(ch, font_id) (RBTree *)RBTree_GetData(ch, font_id)
#define SelectBitmap(font, size) (LCUI_FontBitmap *)RBTree_GetData(font, size)
#define SelectFontFamliy(family_name) \
	(LCUI_FontFamilyNode)         \
	    Dict_FetchValue(fontlib.font_families, family_name);
#define SelectFontStyle(FNODE, S) (&(FNODE)->styles[S])
#define SelectFontWeight(SNODE, W) ((SNODE)->weights[W / 100 - 1])
#define ClearFontWeight(SNODE, W)                     \
	do {                                          \
		(SNODE)->weights[W / 100 - 1] = NULL; \
	} while (0);
#define SetFontWeight(SNODE, FONT)                                 \
	do {                                                       \
		(SNODE)->weights[(FONT)->weight / 100 - 1] = FONT; \
	} while (0);

static LCUI_FontCache FontCache(void)
{
	LCUI_FontCache cache;
	if (!(cache = malloc(sizeof(LCUI_FontCacheRec)))) {
		return NULL;
	}
	memset(cache->fonts, 0, sizeof(cache->fonts));
	return cache;
}

static void DeleteFontCache(LCUI_FontCache cache)
{
	int i;
	for (i = 0; i < FONT_CACHE_SIZE; ++i) {
		if (cache->fonts[i]) {
			DeleteFont(cache->fonts[i]);
		}
		cache->fonts[i] = NULL;
	}
	free(cache);
}

static LCUI_Font GetFontCache(int id)
{
	if (id > fontlib.font_cache_num * FONT_CACHE_SIZE) {
		return NULL;
	}
	return fontlib.font_cache[id / FONT_CACHE_SIZE]
	    ->fonts[id % FONT_CACHE_SIZE];
}

static int SetFontCache(LCUI_Font font)
{
	size_t size;
	LCUI_FontCache *caches, cache;

	if (font->id > FONT_CACHE_MAX_SIZE) {
		Logger_Error("[font] font cache size is the max size\n");
		return -1;
	}
	while (font->id >= fontlib.font_cache_num * FONT_CACHE_SIZE) {
		fontlib.font_cache_num += 1;
		size = fontlib.font_cache_num * sizeof(LCUI_FontCache);
		caches = realloc(fontlib.font_cache, size);
		if (!caches) {
			fontlib.font_cache_num -= 1;
			return -ENOMEM;
		}
		cache = FontCache();
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

LCUI_FontWeight LCUIFont_DetectWeight(const char *str)
{
	char *buf;
	LCUI_FontWeight weight = FONT_WEIGHT_NORMAL;
	if (!(buf = malloc(strsize(str)))) {
		return weight;
	}
	strtolower(buf, str);
	if (strstr(buf, "thin")) {
		weight = FONT_WEIGHT_THIN;
	} else if (strstr(buf, "light")) {
		weight = FONT_WEIGHT_EXTRA_LIGHT;
	} else if (strstr(buf, "semilight")) {
		weight = FONT_WEIGHT_LIGHT;
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

LCUI_FontStyle LCUIFont_DetectStyle(const char *str)
{
	char *buf;
	LCUI_FontStyle style = FONT_STYLE_NORMAL;

	if (!(buf = malloc(strsize(str)))) {
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

LCUI_Font Font(const char *family_name, const char *style_name)
{
	ASSIGN(font, LCUI_Font);
	font->id = 0;
	font->data = NULL;
	font->engine = NULL;
	font->family_name = strdup2(family_name);
	font->style_name = strdup2(style_name);
	font->weight = LCUIFont_DetectWeight(style_name);
	font->style = LCUIFont_DetectStyle(style_name);
	return font;
}

void DeleteFont(LCUI_Font font)
{
	free(font->family_name);
	free(font->style_name);
	font->engine->close(font->data);
	font->data = NULL;
	font->engine = NULL;
	free(font);
}

static void DestroyFontFamilyNode(void *privdata, void *data)
{
	LCUI_FontFamilyNode node = data;
	if (node->family_name) {
		free(node->family_name);
	}
	node->family_name = NULL;
	memset(node->styles, 0, sizeof(node->styles));
	free(node);
}

static void DestroyFontBitmap(void *arg)
{
	FontBitmap_Free(arg);
	free(arg);
}

static void DestroyTreeNode(void *arg)
{
	RBTree_Destroy(arg);
	free(arg);
}

int LCUIFont_Add(LCUI_Font font)
{
	LCUI_Font exists_font;
	LCUI_FontFamilyNode node;
	LCUI_FontStyleNode snode;
	node = SelectFontFamliy(font->family_name);
	if (!node) {
		node = NEW(LCUI_FontFamilyNodeRec, 1);
		node->family_name = strdup2(font->family_name);
		memset(node->styles, 0, sizeof(node->styles));
		Dict_Add(fontlib.font_families, node->family_name, node);
	}
	snode = SelectFontStyle(node, font->style);
	exists_font = SelectFontWeight(snode, font->weight);
	if (exists_font) {
		font->id = exists_font->id;
		if (fontlib.default_font &&
		    font->id == fontlib.default_font->id) {
			fontlib.default_font = font;
		}
		ClearFontWeight(snode, font->weight);
		DeleteFont(exists_font);
	} else {
		font->id = ++fontlib.count;
	}
	SetFontWeight(snode, font);
	SetFontCache(font);
	return font->id;
}

LCUI_Font LCUIFont_GetById(int id)
{
	if (!fontlib.active) {
		return NULL;
	}
	if (id < 0 || id >= fontlib.font_cache_num * FONT_CACHE_SIZE) {
		return NULL;
	}
	return GetFontCache(id);
}

size_t LCUIFont_UpdateWeight(const int *font_ids, LCUI_FontWeight weight,
			     int **new_font_ids)
{
	int id, *ids;
	LCUI_Font font;
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
		font = LCUIFont_GetById(font_ids[i]);
		id = LCUIFont_GetId(font->family_name, font->style, weight);
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

size_t LCUIFont_UpdateStyle(const int *font_ids, LCUI_FontStyle style,
			    int **new_font_ids)
{
	int id, *ids;
	LCUI_Font font;
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
		font = LCUIFont_GetById(font_ids[i]);
		id = LCUIFont_GetId(font->family_name, style, font->weight);
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

size_t LCUIFont_GetIdByNames(int **font_ids, LCUI_FontStyle style,
			     LCUI_FontWeight weight, const char *names)
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
	ids = NEW(int, count + 1);
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
		ids[count] = LCUIFont_GetId(name, style, weight);
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

static LCUI_FontWeight FindBolderWeight(LCUI_FontStyleNode snode,
					LCUI_FontWeight weight)
{
	for (weight += 100; weight <= FONT_WEIGHT_BLACK; weight += 100) {
		if (SelectFontWeight(snode, weight)) {
			return weight;
		}
	}
	return FONT_WEIGHT_NONE;
}

static LCUI_FontWeight FindLighterWeight(LCUI_FontStyleNode snode,
					 LCUI_FontWeight weight)
{
	for (weight -= 100; weight >= FONT_WEIGHT_THIN; weight -= 100) {
		if (SelectFontWeight(snode, weight)) {
			return weight;
		}
	}
	return FONT_WEIGHT_NONE;
}

/**
 * 在未找到指定粗细程度的字体时进行回退，找到合适的字体
 * 回退规则的参考文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight#Fallback_weights
 */
static LCUI_FontWeight FontWeightFallback(LCUI_FontStyleNode snode,
					  LCUI_FontWeight weight)
{
	if (weight > FONT_WEIGHT_MEDIUM) {
		return FindBolderWeight(snode, weight);
	}
	if (weight < FONT_WEIGHT_NORMAL) {
		return FindLighterWeight(snode, weight);
	}
	if (weight == FONT_WEIGHT_NORMAL) {
		if (SelectFontWeight(snode, FONT_WEIGHT_MEDIUM)) {
			return FONT_WEIGHT_MEDIUM;
		}
	} else if (weight == FONT_WEIGHT_MEDIUM) {
		if (SelectFontWeight(snode, FONT_WEIGHT_NORMAL)) {
			return FONT_WEIGHT_NORMAL;
		}
	}
	weight = FindLighterWeight(snode, weight);
	if (weight != FONT_WEIGHT_NONE) {
		return weight;
	}
	return FONT_WEIGHT_NONE;
}

int LCUIFont_GetId(const char *family_name, LCUI_FontStyle style,
		   LCUI_FontWeight weight)
{
	int style_num;
	LCUI_FontWeight w;
	LCUI_FontStyleNode snode;
	LCUI_FontFamilyNode fnode;

	if (!fontlib.active) {
		return -1;
	}
	fnode = SelectFontFamliy(family_name);
	if (!fnode) {
		return -2;
	}
	if (weight == 0) {
		weight = FONT_WEIGHT_NORMAL;
	}
	for (style_num = style; style_num >= 0; --style_num) {
		snode = &fnode->styles[style_num];
		if (SelectFontWeight(snode, weight)) {
			return SelectFontWeight(snode, weight)->id;
		}
		w = FontWeightFallback(snode, weight);
		if (w) {
			return SelectFontWeight(snode, w)->id;
		}
	}
	return -3;
}

int LCUIFont_GetDefault(void)
{
	if (!fontlib.default_font) {
		return -1;
	}
	return fontlib.default_font->id;
}

void LCUIFont_SetDefault(int id)
{
	LCUI_Font font = LCUIFont_GetById(id);
	if (font) {
		fontlib.default_font = font;
		Logger_Debug("[font] select: %s\n", font->family_name);
	}
}

LCUI_FontBitmap *LCUIFont_AddBitmap(wchar_t ch, int font_id, int size,
				    const LCUI_FontBitmap *bmp)
{
	LCUI_FontBitmap *bmp_cache;
	RBTree *tree_font, *tree_bmp;

	if (!fontlib.active) {
		return NULL;
	}
	/* 获取字符的字体信息集 */
	tree_font = SelectChar(ch);
	if (!tree_font) {
		tree_font = NEW(RBTree, 1);
		if (!tree_font) {
			return NULL;
		}
		RBTree_Init(tree_font);
		RBTree_OnDestroy(tree_font, DestroyTreeNode);
		RBTree_Insert(&fontlib.bitmap_cache, ch, tree_font);
	}
	/* 当字体ID不大于0时，使用内置字体 */
	if (font_id <= 0) {
		font_id = fontlib.incore_font->id;
	}
	/* 获取相应字体样式标识号的字体位图库 */
	tree_bmp = SelectFont(tree_font, font_id);
	if (!tree_bmp) {
		tree_bmp = NEW(RBTree, 1);
		if (!tree_bmp) {
			return NULL;
		}
		RBTree_Init(tree_bmp);
		RBTree_OnDestroy(tree_bmp, DestroyFontBitmap);
		RBTree_Insert(tree_font, font_id, tree_bmp);
	}
	/* 在字体位图库中获取指定像素大小的字体位图 */
	bmp_cache = SelectBitmap(tree_bmp, size);
	if (!bmp_cache) {
		bmp_cache = NEW(LCUI_FontBitmap, 1);
		if (!bmp_cache) {
			return NULL;
		}
		RBTree_Insert(tree_bmp, size, bmp_cache);
	}
	/* 拷贝数据至该空间内 */
	memcpy(bmp_cache, bmp, sizeof(LCUI_FontBitmap));
	return bmp_cache;
}

int LCUIFont_GetBitmap(wchar_t ch, int font_id, int size,
		       const LCUI_FontBitmap **bmp)
{
	int ret;
	RBTree *ctx;
	LCUI_FontBitmap bmp_cache;

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
		if (!(ctx = SelectChar(ch))) {
			break;
		}
		ctx = SelectFont(ctx, font_id);
		if (!ctx) {
			break;
		}
		*bmp = SelectBitmap(ctx, size);
		if (*bmp) {
			return 0;
		}
		break;
	} while (0);
	if (ch == 0) {
		return -1;
	}
	FontBitmap_Init(&bmp_cache);
	ret = LCUIFont_RenderBitmap(&bmp_cache, ch, font_id, size);
	if (ret == 0) {
		*bmp = LCUIFont_AddBitmap(ch, font_id, size, &bmp_cache);
		return 0;
	}
	ret = LCUIFont_GetBitmap(0, font_id, size, bmp);
	if (ret != 0) {
		*bmp = LCUIFont_AddBitmap(0, font_id, size, &bmp_cache);
	}
	return -1;
}

static int LCUIFont_LoadFileEx(LCUI_FontEngine *engine, const char *file)
{
	LCUI_Font *fonts;
	int i, num_fonts, id;

	Logger_Debug("[font] load file: %s\n", file);
	if (!engine) {
		return -1;
	}
	num_fonts = fontlib.engine->open(file, &fonts);
	if (num_fonts < 1) {
		Logger_Debug("[font] failed to load file: %s\n", file);
		return -2;
	}
	for (i = 0; i < num_fonts; ++i) {
		fonts[i]->engine = engine;
		id = LCUIFont_Add(fonts[i]);
		Logger_Debug("[font] add family: %s, style name: %s, id: %d\n",
			    fonts[i]->family_name, fonts[i]->style_name, id);
	}
	free(fonts);
	return 0;
}

int LCUIFont_LoadFile(const char *filepath)
{
	return LCUIFont_LoadFileEx(fontlib.engine, filepath);
}

/** 打印字体位图的信息 */
void FontBitmap_PrintInfo(LCUI_FontBitmap *bitmap)
{
	printf("address:%p\n", bitmap);
	if (!bitmap) {
		return;
	}
	printf("top: %d, left: %d, width:%d, rows:%d\n", bitmap->top, bitmap->left,
	    bitmap->width, bitmap->rows);
}

/** 初始化字体位图 */
void FontBitmap_Init(LCUI_FontBitmap *bitmap)
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0;
	bitmap->buffer = NULL;
}

/** 释放字体位图占用的资源 */
void FontBitmap_Free(LCUI_FontBitmap *bitmap)
{
	if (bitmap->buffer) {
		free(bitmap->buffer);
	}
	FontBitmap_Init(bitmap);
}

/** 创建字体位图 */
int FontBitmap_Create(LCUI_FontBitmap *bitmap, int width, int rows)
{
	size_t size;
	if (width < 0 || rows < 0) {
		FontBitmap_Free(bitmap);
		return -1;
	}
	if (FontBitmap_IsValid(bitmap)) {
		FontBitmap_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->rows = rows;
	size = width * rows * sizeof(uchar_t);
	bitmap->buffer = (uchar_t *)malloc(size);
	if (bitmap->buffer == NULL) {
		return -2;
	}
	return 0;
}

/** 在屏幕打印以0和1表示字体位图 */
int FontBitmap_Print(LCUI_FontBitmap *fontbmp)
{
	int x, y, m;
	for (y = 0; y < fontbmp->rows; ++y) {
		m = y * fontbmp->width;
		for (x = 0; x < fontbmp->width; ++x, ++m) {
			if (fontbmp->buffer[m] > 128) {
				printf("#");
			} else if (fontbmp->buffer[m] > 64) {
				printf("-");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

static void FontBitmap_MixARGB(LCUI_Graph *graph, LCUI_Rect *write_rect,
			       const LCUI_FontBitmap *bmp, LCUI_Color color,
			       LCUI_Rect *read_rect)
{
	int x, y;
	LCUI_Color c;
	LCUI_ARGB *px, *px_row_des;
	uchar_t *byte_ptr, *byte_row_ptr;

	byte_row_ptr = bmp->buffer + read_rect->y * bmp->width;
	px_row_des = graph->argb + write_rect->y * graph->width;
	byte_row_ptr += read_rect->x;
	px_row_des += write_rect->x;
	for (y = 0; y < read_rect->height; ++y) {
		px = px_row_des;
		byte_ptr = byte_row_ptr;
		for (x = 0; x < read_rect->width; ++x, ++byte_ptr, ++px) {
			c = color;
			c.alpha = (uchar_t)(*byte_ptr * color.alpha / 255.0);
			LCUI_OverPixel(px, &c);
		}
		px_row_des += graph->width;
		byte_row_ptr += bmp->width;
	}
}

static void FontBitmap_MixRGB(LCUI_Graph *graph, LCUI_Rect *write_rect,
			      const LCUI_FontBitmap *bmp, LCUI_Color color,
			      LCUI_Rect *read_rect)
{
	int x, y;
	uchar_t *byte_src, *byte_row_src, *byte_row_des, *byte_des, alpha;
	byte_row_src = bmp->buffer + read_rect->y * bmp->width + read_rect->x;
	byte_row_des = graph->bytes + write_rect->y * graph->bytes_per_row;
	byte_row_des += write_rect->x * graph->bytes_per_pixel;
	for (y = 0; y < read_rect->height; ++y) {
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for (x = 0; x < read_rect->width; ++x) {
			alpha = (uchar_t)(*byte_src * color.alpha / 255);
			ALPHA_BLEND(*byte_des, color.b, alpha);
			++byte_des;
			ALPHA_BLEND(*byte_des, color.g, alpha);
			++byte_des;
			ALPHA_BLEND(*byte_des, color.r, alpha);
			++byte_des;
			++byte_src;
		}
		byte_row_des += graph->bytes_per_row;
		byte_row_src += bmp->width;
	}
}

int FontBitmap_Mix(LCUI_Graph *graph, LCUI_Pos pos, const LCUI_FontBitmap *bmp,
		   LCUI_Color color)
{
	LCUI_Graph write_slot;
	LCUI_Rect r_rect, w_rect;
	if (pos.x > (int)graph->width || pos.y > (int)graph->height) {
		return -2;
	}
	/* 获取写入区域 */
	w_rect.x = pos.x;
	w_rect.y = pos.y;
	w_rect.width = bmp->width;
	w_rect.height = bmp->rows;
	/* 获取需要裁剪的区域 */
	LCUIRect_GetCutArea(graph->width, graph->height, w_rect, &r_rect);
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	Graph_Quote(&write_slot, graph, &w_rect);
	Graph_GetValidRect(&write_slot, &w_rect);
	/* 获取背景图引用的源图形 */
	graph = Graph_GetQuote(graph);
	if (graph->color_type == LCUI_COLOR_TYPE_ARGB) {
		FontBitmap_MixARGB(graph, &w_rect, bmp, color, &r_rect);
	} else {
		FontBitmap_MixRGB(graph, &w_rect, bmp, color, &r_rect);
	}
	return 0;
}

int LCUIFont_RenderBitmap(LCUI_FontBitmap *buff, wchar_t ch, int font_id,
			  int pixel_size)
{
	LCUI_Font font = fontlib.default_font;
	do {
		if (font_id < 0 || !fontlib.engine) {
			break;
		}
		font = LCUIFont_GetById(font_id);
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

static void LCUIFont_InitBase(void)
{
	fontlib.count = 0;
	fontlib.font_cache_num = 1;
	fontlib.font_cache = NEW(LCUI_FontCache, 1);
	fontlib.font_cache[0] = FontCache();
	RBTree_Init(&fontlib.bitmap_cache);
	fontlib.font_families_type = DictType_StringKey;
	fontlib.font_families_type.valDestructor = DestroyFontFamilyNode;
	fontlib.font_families = Dict_Create(&fontlib.font_families_type, NULL);
	RBTree_OnDestroy(&fontlib.bitmap_cache, DestroyTreeNode);
	fontlib.active = TRUE;
}

static void LCUIFont_InitEngine(void)
{
	int fid;
	/* 先初始化内置的字体引擎 */
	fontlib.engine = &fontlib.engines[0];
	LCUIFont_InitInCoreFont(fontlib.engine);
	LCUIFont_LoadFile("in-core.inconsolata");
	fid = LCUIFont_GetId("inconsolata", 0, 0);
	fontlib.incore_font = LCUIFont_GetById(fid);
	fontlib.default_font = fontlib.incore_font;
	/* 然后看情况启用其它字体引擎 */
#ifdef LCUI_FONT_ENGINE_FREETYPE
	if (LCUIFont_InitFreeType(&fontlib.engines[1]) == 0) {
		fontlib.engine = &fontlib.engines[1];
	}
#endif
	if (fontlib.engine && fontlib.engine != &fontlib.engines[0]) {
		Logger_Debug("[font] current font engine is: %s\n",
		    fontlib.engine->name);
	} else {
		Logger_Warning("[font] warning: not font engine support!\n");
	}
}

static void LCUIFont_FreeBase(void)
{
	if (!fontlib.active) {
		return;
	}
	fontlib.active = FALSE;
	while (fontlib.font_cache_num > 0) {
		--fontlib.font_cache_num;
		DeleteFontCache(fontlib.font_cache[fontlib.font_cache_num]);
	}
	Dict_Release(fontlib.font_families);
	RBTree_Destroy(&fontlib.bitmap_cache);
	free(fontlib.font_cache);
	fontlib.font_cache = NULL;
}

static void LCUIFont_FreeEngine(void)
{
	LCUIFont_ExitInCoreFont();
#ifdef LCUI_FONT_ENGINE_FREETYPE
	LCUIFont_ExitFreeType();
#endif
}

#ifdef LCUI_BUILD_IN_WIN32
static void LCUIFont_LoadFontsForWindows(void)
{
	size_t i;
	int *ids = NULL;
	const char *names = "Consola, Simsun, Microsoft YaHei";
	const char *fonts[] = { "C:/Windows/Fonts/consola.ttf",
				"C:/Windows/Fonts/simsun.ttc",
				"C:/Windows/Fonts/msyh.ttf",
				"C:/Windows/Fonts/msyh.ttc" };

	for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
		LCUIFont_LoadFile(fonts[i]);
	}
	i = LCUIFont_GetIdByNames(&ids, FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL,
				  names);
	if (i > 0) {
		LCUIFont_SetDefault(ids[i - 1]);
	}
	free(ids);
}

#else

#ifdef USE_FONTCONFIG

static void LCUIFont_LoadFontsByFontConfig(void)
{
	size_t i;
	char *path;
	int *ids = NULL;
	const char *names = "Noto Sans CJK, Ubuntu, WenQuanYi Micro Hei";
	const char *fonts[] = { "Ubuntu", "Noto Sans CJK SC",
				"WenQuanYi Micro Hei" };

	for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
		path = Fontconfig_GetPath(fonts[i]);
		LCUIFont_LoadFile(path);
		free(path);
	}
	i = LCUIFont_GetIdByNames(&ids, FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL,
				  names);
	if (i > 0) {
		LCUIFont_SetDefault(ids[i - 1]);
	}
	free(ids);
}

#else

static void LCUIFont_LoadFontsForLinux(void)
{
	size_t i;
	int *ids = NULL;
	const char *names = "Noto Sans CJK SC, Ubuntu, WenQuanYi Micro Hei";
	const char *fonts[] = {
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-RI.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-M.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-MI.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-L.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-LI.ttf",
		"/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
		"/usr/share/fonts/opentype/noto/NotoSansCJK.ttc",
		"/usr/share/fonts/truetype/wqy/wqy-microhei.ttc"
	};

	for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
		LCUIFont_LoadFile(fonts[i]);
	}
	i = LCUIFont_GetIdByNames(&ids, FONT_STYLE_NORMAL, FONT_WEIGHT_NORMAL,
				  names);
	if (i > 0) {
		LCUIFont_SetDefault(ids[i - 1]);
	}
	free(ids);
}
#endif

#endif

static void LCUIFont_LoadDefaultFonts(void)
{
#ifdef LCUI_BUILD_IN_WIN32
	LCUIFont_LoadFontsForWindows();
#elif defined(USE_FONTCONFIG)
	Logger_Debug("[font] fontconfig enabled\n");
	LCUIFont_LoadFontsByFontConfig();
#else
	LCUIFont_LoadFontsForLinux();
#endif
}

void LCUI_InitFontLibrary(void)
{
	LCUIFont_InitBase();
	LCUIFont_InitEngine();
	LCUIFont_LoadDefaultFonts();
}

void LCUI_FreeFontLibrary(void)
{
	LCUIFont_FreeBase();
	LCUIFont_FreeEngine();
}
