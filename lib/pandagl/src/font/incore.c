/*
 * lib/pandagl/src/font/incore.c: -- basic in-core font-engine.
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
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

static int pd_incore_font_open(const char *filepath, pd_font_t ***outfonts)
{
	int *code;
	pd_font_t **fonts, *font;
	if (strcmp(filepath, "in-core.inconsolata") != 0) {
		return 0;
	}
	code = malloc(sizeof(int));
	*code = FONT_INCONSOLATA;
	font = pd_font_create("inconsolata", "Regular");
	fonts = malloc(sizeof(pd_font_t*));
	font->data = code;
	fonts[0] = font;
	*outfonts = fonts;
	return 1;
}

static void pd_incore_font_close(void *face)
{
	free(face);
}

static int pd_incore_font_render(pd_font_bitmap_t *bmp, unsigned ch,
			     int pixel_size, pd_font_t *font)
{
	int *code = (int*)font->data;
	switch (*code) {
	case FONT_INCONSOLATA:
	default:
		return pd_render_inconsolata_font_bitmap(bmp, ch, pixel_size);
	}
	return -1;
}

int pd_incore_font_create(font_engine_t *engine)
{
	engine->render = pd_incore_font_render;
	engine->close = pd_incore_font_close;
	engine->open = pd_incore_font_open;
	strcpy(engine->name, "in-core");
	return 0;
}

int pd_incore_font_destroy(void)
{
	return 0;
}
