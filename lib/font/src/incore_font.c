/*
 * in_core_font.c -- basic in-core font-engine.
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


#include <string.h>
#include <stdlib.h>
#include "internal.h"

enum in_core_font_type {
	FONT_INCONSOLATA
};

static int incore_font_engine_open(const char *filepath, font_t ***outfonts)
{
	int *code;
	font_t **fonts, *font;
	if (strcmp(filepath, "in-core.inconsolata") != 0) {
		return 0;
	}
	code = malloc(sizeof(int));
	*code = FONT_INCONSOLATA;
	font = font_create("inconsolata", "Regular");
	fonts = malloc(sizeof(font_t*));
	font->data = code;
	fonts[0] = font;
	*outfonts = fonts;
	return 1;
}

static void incore_font_engine_close(void *face)
{
	free(face);
}

static int incore_font_engine_render(font_bitmap_t *bmp, unsigned ch,
			     int pixel_size, font_t *font)
{
	int *code = (int*)font->data;
	switch (*code) {
	case FONT_INCONSOLATA:
	default:
		return inconsolata_font_render_bitmap(bmp, ch, pixel_size);
	}
	return -1;
}

int incore_font_engine_create(font_engine_t *engine)
{
	engine->render = incore_font_engine_render;
	engine->close = incore_font_engine_close;
	engine->open = incore_font_engine_open;
	strcpy(engine->name, "in-core");
	return 0;
}

int incore_font_engine_destroy(void)
{
	return 0;
}
