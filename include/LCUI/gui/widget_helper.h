/*
 * widget.c -- GUI widget helper APIs.
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

#ifndef LCUI_WIDGET_HELPER_H
#define LCUI_WIDGET_HELPER_H

LCUI_BEGIN_HEADER

#define Widget_IsVisible(W) (W)->computed_style.visible

/** 设置内边距 */
LCUI_API void Widget_SetPadding(LCUI_Widget w, float top, float right,
				float bottom, float left);

/** 设置外边距 */
LCUI_API void Widget_SetMargin(LCUI_Widget w, float top, float right,
			       float bottom, float left);

LCUI_API void Widget_SetBorderColor(LCUI_Widget w, LCUI_Color color);

LCUI_API void Widget_SetBorderWidth(LCUI_Widget w, float width);

LCUI_API void Widget_SetBorderStyle(LCUI_Widget w, int style);

/** 设置边框样式 */
LCUI_API void Widget_SetBorder(LCUI_Widget w, float width, int style,
			       LCUI_Color clr);

/** 设置阴影样式 */
LCUI_API void Widget_SetBoxShadow(LCUI_Widget w, float x, float y, float blur,
				  LCUI_Color color);

/** 移动部件位置 */
LCUI_API void Widget_Move(LCUI_Widget w, float left, float top);

/** 调整部件尺寸 */
LCUI_API void Widget_Resize(LCUI_Widget w, float width, float height);

LCUI_API LCUI_Style Widget_GetStyle(LCUI_Widget w, int key);

LCUI_API int Widget_UnsetStyle(LCUI_Widget w, int key);

LCUI_API LCUI_Style Widget_GetInheritedStyle(LCUI_Widget w, int key);

LCUI_API LCUI_BOOL Widget_CheckStyleBooleanValue(LCUI_Widget w, int key,
						 LCUI_BOOL value);

LCUI_API LCUI_BOOL Widget_CheckStyleValid(LCUI_Widget w, int key) ;

LCUI_API void Widget_SetVisibility(LCUI_Widget w, const char *value);

LCUI_API void Widget_SetVisible(LCUI_Widget w);

LCUI_API void Widget_SetHidden(LCUI_Widget w);

LCUI_API void Widget_Show(LCUI_Widget w);

LCUI_API void Widget_Hide(LCUI_Widget w);

LCUI_API void Widget_SetPosition(LCUI_Widget w, LCUI_StyleValue position);

LCUI_API void Widget_SetOpacity(LCUI_Widget w, float opacity);

LCUI_API void Widget_SetBoxSizing(LCUI_Widget w, LCUI_StyleValue sizing);

/** Collect all child widget that have a ref attribute specified */
LCUI_API Dict *Widget_CollectReferences(LCUI_Widget w);

/**
 * Get the first widget that match the type by testing the widget itself and
 * traversing up through its ancestors.
 */
LCUI_API LCUI_Widget Widget_GetClosest(LCUI_Widget w, const char *type);

LCUI_END_HEADER

#endif
