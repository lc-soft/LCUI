/*
 * lib/pandagl/src/context.c: -- The PandaGL drawing context
 *
 * Copyright (c) 2022-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <pandagl.h>

pd_context_t* pd_context_create(pd_canvas_t *canvas, pd_rect_t *rect)
{
	pd_context_t* paint = (pd_context_t*)malloc(sizeof(pd_context_t));
	paint->rect = *rect;
	paint->with_alpha = PD_FALSE;
	pd_canvas_init(&paint->canvas);
	pd_canvas_quote(&paint->canvas, canvas, &paint->rect);
	return paint;
}

void pd_context_destroy(pd_context_t* paint)
{
	free(paint);
}
