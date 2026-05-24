/*
 * lib/pandagl/src/font/incore.c: -- basic in-core font-engine.
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <stdlib.h>
#include <pandagl.h>
#include "library.h"
#include "incore.h"
#include "inconsolata.h"

enum in_core_font_type {
	FONT_INCONSOLATA
};

static int pd_incore_font_open(const char *filepath, pd_font_face_t ***outfonts)
{
	int *code;
	pd_font_face_t **fonts, *font;
	if (strcmp(filepath, "in-core.inconsolata") != 0) {
		return 0;
	}
	code = malloc(sizeof(int));
	*code = FONT_INCONSOLATA;
	font = pd_font_face_create("inconsolata", "Regular");
	fonts = malloc(sizeof(pd_font_face_t*));
	font->data = code;
	fonts[0] = font;
	*outfonts = fonts;
	return 1;
}

static void pd_incore_font_close(void *face)
{
	free(face);
}

static int pd_incore_font_render(pd_glyph_bitmap_t *bmp, unsigned ch,
			     int pixel_size, pd_font_face_t *font)
{
	int *code = (int*)font->data;
	switch (*code) {
	case FONT_INCONSOLATA:
	default:
		return pd_render_inconsolata_font_bitmap(bmp, ch, pixel_size);
	}
	return -1;
}

static unsigned pd_incore_font_get_glyph_index(pd_font_face_t *face, unsigned ch)
{
	(void)face;
	/* 内置位图字体没有 cmap，字符码即字形索引 */
	return ch;
}

int pd_incore_font_create(pd_font_engine_t *engine)
{
	engine->render = pd_incore_font_render;
	engine->close = pd_incore_font_close;
	engine->open = pd_incore_font_open;
	engine->get_glyph_index = pd_incore_font_get_glyph_index;
	strcpy(engine->name, "in-core");
	return 0;
}

int pd_incore_font_destroy(void)
{
	return 0;
}
