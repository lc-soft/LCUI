/*
 * lib/css/src/dump.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_DUMP_H
#define LIBCSS_INCLUDE_CSS_DUMP_H

#include <css/style_value.h>
#include <css/style_decl.h>

typedef struct css_dump_context_t css_dump_context_t;

struct css_dump_context_t {
	char *data;
	size_t len;
	size_t max_len;
	size_t (*func)(css_dump_context_t *, const char *, ...);
};

#define DUMP(_STR) ctx->func(ctx, "%s", _STR);

#define DUMPF(_FMT, ...) ctx->func(ctx, _FMT, ##__VA_ARGS__);

size_t css_dump_to_stdout(css_dump_context_t *ctx, const char *fmt, ...);
size_t css_dump_to_buffer(css_dump_context_t *ctx, const char *fmt, ...);

void css_dump_style_value(const css_style_value_t *s, css_dump_context_t *ctx);
void css_dump_style_decl(const css_style_decl_t *list, css_dump_context_t *ctx);

#endif
