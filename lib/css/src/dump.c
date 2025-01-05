/*
 * lib/css/src/dump.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdarg.h>
#include "dump.h"

size_t css_dump_to_stdout(css_dump_context_t *ctx, const char *fmt, ...)
{
	va_list args;
	size_t n;

	va_start(args, fmt);
	n = vprintf(fmt, args);
	ctx->len += n;
	va_end(args);
	return n;
}

size_t css_dump_to_buffer(css_dump_context_t *ctx, const char *fmt, ...)
{
	va_list args;
	size_t n;

	if (ctx->len >= ctx->max_len) {
		return 0;
	}
	va_start(args, fmt);
	n = vsnprintf(ctx->data + ctx->len, ctx->max_len - ctx->len, fmt, args);
	ctx->len += n;
	va_end(args);
	return n;
}
