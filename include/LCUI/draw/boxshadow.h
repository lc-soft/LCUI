/*
 * boxshadow.h -- Box shadow draw support.
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

#ifndef LCUI_DRAW_BOXSHADOW_H
#define LCUI_DRAW_BOXSHADOW_H

#define SHADOW_WIDTH(sd) (sd->blur + sd->spread)

 /* 计算Box在添加阴影后的宽度 */
LCUI_API int BoxShadow_GetBoxWidth( const LCUI_BoxShadow *shadow, int w );

/** 计算Box在添加阴影后的高度 */
LCUI_API int BoxShadow_GetBoxHeight( const LCUI_BoxShadow *shadow, int h );

/** 计算Box在添加阴影后的宽度 */
LCUI_API int BoxShadow_GetWidth( const LCUI_BoxShadow *shadow, int box_w );

/** 计算Box在添加阴影后的高度 */
LCUI_API int BoxShadow_GetHeight( const LCUI_BoxShadow *shadow, int box_h );

LCUI_API int BoxShadow_GetBoxX( const LCUI_BoxShadow *shadow );

LCUI_API int BoxShadow_GetBoxY( const LCUI_BoxShadow *shadow );

LCUI_API int BoxShadow_GetY( const LCUI_BoxShadow *shadow );

LCUI_API int BoxShadow_GetX( const LCUI_BoxShadow *shadow );

LCUI_API void BoxShadow_GetCanvasRect( const LCUI_BoxShadow *shadow,
				       const LCUI_Rect *box_rect,
				       LCUI_Rect *canvas_rect );

LCUI_API int BoxShadow_Paint( const LCUI_BoxShadow *shadow,
			      const LCUI_Rect *box,
			      LCUI_PaintContext paint );

#endif
