﻿#include "../include/font.h"

font_t *font_create(const char *family_name, const char *style_name);
void font_destroy(font_t *font);
int incore_font_engine_create(font_engine_t *engine);
int incore_font_engine_destroy(void);
int freetype_engine_init(font_engine_t *engine);
int freetype_engine_destroy(void);
