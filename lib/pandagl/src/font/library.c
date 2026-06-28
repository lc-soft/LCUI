/*
 * lib/pandagl/src/font/library.c: -- The font info and font bitmap cache
 *
 * Copyright (c) 2018-2026, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
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
#define GLYPH_CACHE_DEFAULT_CAPACITY	8192

/**
 * 字形位图缓存使用扁平 hash 表 + LRU 双链表：
 * key   = (face_id, glyph_index, size) 打包为 64 位整数
 * value = glyph_cache_entry_t，包含位图数据和指向 LRU 链表节点的反向引用
 * 命中时移动到链首；超过 capacity 时从链尾驱逐。
 */

typedef struct font_style_node {
	/* 字体列表，按粗细程度存放 */
	pd_font_face_t *weights[PD_FONT_WEIGHT_TOTAL_NUM];
} font_style_node_t;

typedef struct font_cache {
	pd_font_face_t *fonts[FONT_CACHE_SIZE];
} font_cache_t;

/** 字体字族索引结点 */
typedef struct font_family_node {
	/** 字体的字族名称  */
	char *family_name;

	/** 字体列表，按风格存放 */
	font_style_node_t styles[PD_FONT_STYLE_TOTAL_NUM];
} font_family_node_t;

typedef struct glyph_cache_key {
	int face_id;
	unsigned glyph_index;
	int size;
} glyph_cache_key_t;

typedef struct glyph_cache_entry {
	uint64_t key;
	pd_glyph_bitmap_t bitmap;
	list_node_t *lru_node;
} glyph_cache_entry_t;

static struct font_library_module {
	int count;
	int font_cache_num;
	bool active;

	/** dict_t<string, font_family_node_t> */
	dict_t *font_families;

	/** dict_t<string, string> */
	dict_t *font_family_aliases;

	/** dict_t<uint64_t, glyph_cache_entry_t> */
	dict_t *glyph_cache;
	list_t glyph_lru;
	size_t glyph_cache_capacity;

	font_cache_t **font_cache;
	pd_font_face_t *default_font;
	pd_font_face_t *incore_font;
	pd_font_engine_t engines[2];
	pd_font_engine_t *engine;
} fontlib;

/* clang-format on */

PD_INLINE font_family_node_t *select_font_family_cache(const char *family_name)
{
        return dict_fetch_value(fontlib.font_families, family_name);
}

PD_INLINE uint64_t glyph_cache_pack_key(int face_id, unsigned glyph_index,
                                        int size)
{
        /* face_id 占高 16 位，glyph_index 占中间 32 位，size 占低 16 位。 */
        return ((uint64_t)(unsigned)face_id << 48) |
               ((uint64_t)glyph_index << 16) |
               (uint64_t)((unsigned)size & 0xFFFFu);
}

static uint64_t glyph_cache_key_hash(const void *key)
{
        return dict_gen_hash_function(key, (int)sizeof(uint64_t));
}

static int glyph_cache_key_compare(void *priv_data, const void *key1,
                                   const void *key2)
{
        (void)priv_data;
        return *(const uint64_t *)key1 == *(const uint64_t *)key2;
}

static void *glyph_cache_key_dup(void *priv_data, const void *key)
{
        uint64_t *copy;
        (void)priv_data;
        copy = malloc(sizeof(uint64_t));
        if (copy) {
                *copy = *(const uint64_t *)key;
        }
        return copy;
}

static void glyph_cache_key_destructor(void *priv_data, void *key)
{
        (void)priv_data;
        free(key);
}

static void glyph_cache_entry_destructor(void *priv_data, void *val)
{
        glyph_cache_entry_t *entry = val;
        (void)priv_data;
        if (!entry) {
                return;
        }
        if (entry->lru_node) {
                list_unlink(&fontlib.glyph_lru, entry->lru_node);
                list_node_free(entry->lru_node);
        }
        pd_glyph_bitmap_destroy(&entry->bitmap);
        free(entry);
}

static void glyph_cache_evict_one(void)
{
        list_node_t *node = list_get_first_node(&fontlib.glyph_lru);
        glyph_cache_entry_t *victim;

        if (!node) {
                return;
        }
        victim = node->data;
        /* dict_delete 触发 destructor 会再次解链 lru_node，
         * 所以这里只清掉反向引用并把 list_node 自己处理掉 */
        list_unlink(&fontlib.glyph_lru, node);
        list_node_free(node);
        victim->lru_node = NULL;
        dict_delete(fontlib.glyph_cache, &victim->key);
}

static void glyph_cache_touch(glyph_cache_entry_t *entry)
{
        if (!entry->lru_node) {
                return;
        }
        if (entry->lru_node == fontlib.glyph_lru.tail.prev) {
                return;
        }
        list_unlink(&fontlib.glyph_lru, entry->lru_node);
        list_append_node(&fontlib.glyph_lru, entry->lru_node);
}

static unsigned resolve_glyph_index(unsigned ch, int font_id)
{
        pd_font_face_t *face = pd_font_get(font_id);

        if (!face || !face->engine || !face->engine->get_glyph_index) {
                return ch;
        }
        return face->engine->get_glyph_index(face, ch);
}

#ifdef PANDAGL_HAS_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

char *pd_font_find_path(const char *name)
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

pd_font_face_t *pd_font_face_create(const char *family_name,
                                    const char *style_name)
{
        pd_font_face_t *font;

        font = malloc(sizeof(pd_font_face_t));
        font->id = 0;
        font->data = NULL;
        font->engine = NULL;
        font->family_name = y_strdup(family_name);
        font->style_name = y_strdup(style_name);
        font->weight = pd_font_parse_weight(style_name);
        font->style = pd_font_parse_style(style_name);
        return font;
}

void pd_font_face_destroy(pd_font_face_t *font)
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

pd_glyph_bitmap_t *pd_font_cache_add_bitmap(wchar_t ch, int font_id, int size,
                                            const pd_glyph_bitmap_t *bmp)
{
        uint64_t key;
        unsigned glyph_index;
        glyph_cache_entry_t *entry;

        if (!fontlib.active) {
                return NULL;
        }
        /* 当字体ID不大于0时，使用内置字体 */
        if (font_id <= 0) {
                font_id = fontlib.incore_font->id;
        }
        glyph_index = resolve_glyph_index(ch, font_id);
        key = glyph_cache_pack_key(font_id, glyph_index, size);
        entry = dict_fetch_value(fontlib.glyph_cache, &key);
        if (entry) {
                /* 已存在则就地覆盖位图数据 */
                pd_glyph_bitmap_destroy(&entry->bitmap);
                memcpy(&entry->bitmap, bmp, sizeof(pd_glyph_bitmap_t));
                glyph_cache_touch(entry);
                return &entry->bitmap;
        }
        /* 容量到上限时驱逐链尾 */
        while (fontlib.glyph_cache_capacity > 0 &&
               fontlib.glyph_lru.length >= fontlib.glyph_cache_capacity) {
                glyph_cache_evict_one();
        }
        entry = malloc(sizeof(glyph_cache_entry_t));
        if (!entry) {
                return NULL;
        }
        entry->key = key;
        memcpy(&entry->bitmap, bmp, sizeof(pd_glyph_bitmap_t));
        entry->lru_node = NULL;
        if (dict_add(fontlib.glyph_cache, &key, entry) != DICT_OK) {
                free(entry);
                return NULL;
        }
        entry->lru_node = malloc(sizeof(list_node_t));
        if (entry->lru_node) {
                entry->lru_node->data = entry;
                entry->lru_node->prev = NULL;
                entry->lru_node->next = NULL;
                list_append_node(&fontlib.glyph_lru, entry->lru_node);
        }
        return &entry->bitmap;
}

int pd_font_cache_get_bitmap(unsigned ch, int font_id, int size,
                             const pd_glyph_bitmap_t **bmp)
{
        int ret;
        uint64_t key;
        unsigned glyph_index;
        glyph_cache_entry_t *entry;
        pd_glyph_bitmap_t tmp;

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
        glyph_index = resolve_glyph_index(ch, font_id);
        key = glyph_cache_pack_key(font_id, glyph_index, size);
        entry = dict_fetch_value(fontlib.glyph_cache, &key);
        if (entry) {
                glyph_cache_touch(entry);
                *bmp = &entry->bitmap;
                return 0;
        }
        if (ch == 0) {
                return -1;
        }
        pd_glyph_bitmap_init(&tmp);
        ret = pd_font_render_glyph(&tmp, ch, font_id, size);
        if (ret == 0) {
                *bmp = pd_font_cache_add_bitmap(ch, font_id, size, &tmp);
                return 0;
        }
        /* 渲染失败：回退到 .notdef（ch == 0）位图 */
        ret = pd_font_cache_get_bitmap(0, font_id, size, bmp);
        if (ret != 0) {
                *bmp = pd_font_cache_add_bitmap(0, font_id, size, &tmp);
        }
        return -1;
}

void pd_font_cache_set_capacity(size_t capacity)
{
        fontlib.glyph_cache_capacity = capacity;
        if (capacity == 0) {
                return;
        }
        while (fontlib.glyph_lru.length > capacity) {
                glyph_cache_evict_one();
        }
}

size_t pd_font_cache_count(void)
{
        return fontlib.glyph_lru.length;
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
                        pd_font_face_destroy(cache->fonts[i]);
                }
                cache->fonts[i] = NULL;
        }
        free(cache);
}

static pd_font_face_t *pd_font_library_get_cached_font(int id)
{
        if (id > fontlib.font_cache_num * FONT_CACHE_SIZE) {
                return NULL;
        }
        return fontlib.font_cache[id / FONT_CACHE_SIZE]
            ->fonts[id % FONT_CACHE_SIZE];
}

static int pd_font_library_add_cached_font(pd_font_face_t *font)
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

pd_font_weight_t pd_font_parse_weight(const char *str)
{
        char *buf;
        pd_font_weight_t weight = PD_FONT_WEIGHT_NORMAL;

        buf = y_strdup(str);
        if (!buf) {
                return weight;
        }
        y_strlower(buf, str);
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

pd_font_style_t pd_font_parse_style(const char *str)
{
        char *buf;
        pd_font_style_t style = PD_FONT_STYLE_NORMAL;

        buf = y_strdup(str);
        if (!buf) {
                return style;
        }
        y_strlower(buf, str);
        if (strstr(buf, "oblique")) {
                style = PD_FONT_STYLE_OBLIQUE;
        } else if (strstr(buf, "italic")) {
                style = PD_FONT_STYLE_ITALIC;
        }
        free(buf);
        return style;
}

int pd_font_register(pd_font_face_t *font)
{
        pd_font_face_t *exists_font;
        font_family_node_t *node;
        font_style_node_t *style_node;

        node = select_font_family_cache(font->family_name);
        if (!node) {
                node = malloc(sizeof(font_family_node_t));
                node->family_name = y_strdup(font->family_name);
                memset(node->styles, 0, sizeof(node->styles));
                dict_add(fontlib.font_families, node->family_name, node);
        }
        style_node = node->styles + font->style;
        exists_font = style_node->weights[font->weight - 1];
        if (exists_font) {
                font->id = exists_font->id;
                if (fontlib.default_font &&
                    font->id == fontlib.default_font->id) {
                        fontlib.default_font = font;
                }
                style_node->weights[font->weight - 1] = NULL;
                pd_font_face_destroy(exists_font);
        } else {
                font->id = ++fontlib.count;
        }
        style_node->weights[font->weight - 1] = font;
        pd_font_library_add_cached_font(font);
        return font->id;
}

pd_font_face_t *pd_font_get(int id)
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

int pd_font_get_id(const char *family_name, pd_font_style_t style,
                   pd_font_weight_t weight)
{
        int style_num;
        const char *name;
        pd_font_weight_t w;
        font_style_node_t *snode;
        font_family_node_t *fnode;

        if (!fontlib.active) {
                return -1;
        }
        name = dict_fetch_value(fontlib.font_family_aliases, family_name);
        if (!name) {
                name = family_name;
        }
        fnode = select_font_family_cache(name);
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

size_t pd_font_replace_weight(const int *font_ids, pd_font_weight_t weight,
                              int **new_font_ids)
{
        int id, *ids;
        pd_font_face_t *font;
        size_t i, count, len;

        if (!font_ids) {
                return 0;
        }
        for (len = 0; font_ids[len]; ++len);
        if (len < 1) {
                return 0;
        }
        ids = malloc((len + 1) * sizeof(int));
        if (!ids) {
                return 0;
        }
        for (i = 0, count = 0; i < len; ++i) {
                font = pd_font_get(font_ids[i]);
                id = pd_font_get_id(font->family_name, font->style, weight);
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

size_t pd_font_replace_style(const int *font_ids, pd_font_style_t style,
                             int **new_font_ids)
{
        int id, *ids;
        pd_font_face_t *font;
        size_t i, count, len;

        if (!font_ids) {
                return 0;
        }
        for (len = 0; font_ids[len]; ++len);
        if (len < 1) {
                return 0;
        }
        ids = malloc((len + 1) * sizeof(int));
        if (!ids) {
                return 0;
        }
        for (i = 0, count = 0; i < len; ++i) {
                font = pd_font_get(font_ids[i]);
                id = pd_font_get_id(font->family_name, style, font->weight);
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

unsigned pd_font_query(int **font_ids, pd_font_style_t style,
                       pd_font_weight_t weight, const char *const *names)
{
        int *ids;
        unsigned i, count, loaded_count;

        *font_ids = NULL;
        if (!names) {
                return 0;
        }
        for (count = 0; names[count]; ++count);
        if (count < 1) {
                return 0;
        }
        ids = malloc(sizeof(int) * (count + 1));
        if (!ids) {
                return 0;
        }
        for (loaded_count = 0, i = 0; i < count; ++i) {
                ids[loaded_count] = pd_font_get_id(names[i], style, weight);
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

int pd_font_get_default(void)
{
        if (!fontlib.default_font) {
                return -1;
        }
        return fontlib.default_font->id;
}

void pd_font_set_default(int id)
{
        pd_font_face_t *font = pd_font_get(id);
        if (font) {
                fontlib.default_font = font;
                logger_debug("[font] select: %s\n", font->family_name);
        }
}

int pd_font_load_file(const char *filepath)
{
        pd_font_face_t **fonts;
        int i, num_fonts, id;

        logger_debug("[font] load file: %s\n", filepath);
        if (!fontlib.engine) {
                return -1;
        }
        num_fonts = fontlib.engine->open(filepath, &fonts);
        if (num_fonts < 1) {
                logger_error("[font] failed to load file: %s\n", filepath);
                return -2;
        }
        for (i = 0; i < num_fonts; ++i) {
                fonts[i]->engine = fontlib.engine;
                id = pd_font_register(fonts[i]);
                logger_debug("[font] add font: %d, family: %s, style name: %s, "
                             "weight: %d\n",
                             id, fonts[i]->family_name, fonts[i]->style_name,
                             fonts[i]->weight);
        }
        free(fonts);
        return 0;
}

int pd_font_render_glyph(pd_glyph_bitmap_t *buff, unsigned ch, int font_id,
                         int pixel_size)
{
        pd_font_face_t *font = fontlib.default_font;
        do {
                if (font_id < 0 || !fontlib.engine) {
                        break;
                }
                font = pd_font_get(font_id);
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

bool pd_font_family_set_alias(const char *alias, const char *family_name)
{
        return dict_add(fontlib.font_family_aliases, (void *)alias,
                        (void *)family_name) == DICT_OK;
}

static void font_family_dict_val_destructor(void *privdata, void *data)
{
        free(data);
}

static void *font_family_dict_val_dup(void *privdata, const void *data)
{
        return y_strdup(data);
}

static void pd_font_library_init_base(void)
{
        static dict_type_t dict_type;
        static dict_type_t alias_dict_type;
        static dict_type_t glyph_dict_type;

        fontlib.count = 0;
        fontlib.font_cache_num = 1;
        fontlib.font_cache = malloc(sizeof(font_cache_t));
        fontlib.font_cache[0] = font_cache_create();
        dict_init_string_key_type(&dict_type);
        dict_init_string_copy_key_type(&alias_dict_type);
        dict_type.val_destructor = destroy_font_family_node;
        alias_dict_type.val_destructor = font_family_dict_val_destructor;
        alias_dict_type.val_dup = font_family_dict_val_dup;
        fontlib.font_families = dict_create(&dict_type, NULL);
        fontlib.font_family_aliases = dict_create(&alias_dict_type, NULL);

        /* glyph 位图缓存：key 为打包后的 64 位整数 */
        memset(&glyph_dict_type, 0, sizeof(glyph_dict_type));
        glyph_dict_type.hash_function = glyph_cache_key_hash;
        glyph_dict_type.key_compare = glyph_cache_key_compare;
        glyph_dict_type.key_dup = glyph_cache_key_dup;
        glyph_dict_type.key_destructor = glyph_cache_key_destructor;
        glyph_dict_type.val_destructor = glyph_cache_entry_destructor;
        fontlib.glyph_cache = dict_create(&glyph_dict_type, NULL);
        list_create(&fontlib.glyph_lru);
        fontlib.glyph_cache_capacity = GLYPH_CACHE_DEFAULT_CAPACITY;

        fontlib.active = true;
}

static void pd_font_library_init_engine(void)
{
        int fid;
        /* 先初始化内置的字体引擎 */
        fontlib.engine = &fontlib.engines[0];
        pd_incore_font_create(fontlib.engine);
        pd_font_load_file("in-core.inconsolata");
        fid = pd_font_get_id("inconsolata", 0, 0);
        fontlib.incore_font = pd_font_get(fid);
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
        fontlib.active = false;
        while (fontlib.font_cache_num > 0) {
                --fontlib.font_cache_num;
                font_cache_destroy(fontlib.font_cache[fontlib.font_cache_num]);
        }
        dict_destroy(fontlib.font_family_aliases);
        dict_destroy(fontlib.font_families);
        /* dict 的 val_destructor 会解链并释放各 entry 的 LRU 节点 */
        dict_destroy(fontlib.glyph_cache);
        free(fontlib.font_cache);
        fontlib.font_cache = NULL;
        fontlib.font_families = NULL;
        fontlib.font_family_aliases = NULL;
        fontlib.glyph_cache = NULL;
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
