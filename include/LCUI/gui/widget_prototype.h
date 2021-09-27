/*
 * widget_prototype.h -- LCUI widget prototype library management module.
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

#ifndef LCUI_WIDGET_PROTOTYPE_H
#define LCUI_WIDGET_PROTOTYPE_H

LCUI_BEGIN_HEADER

LCUI_API void LCUIWidget_InitPrototype(void);

LCUI_API void LCUIWidget_FreePrototype(void);

LCUI_API LCUI_WidgetPrototype LCUIWidget_GetPrototype(const char *name);

LCUI_API LCUI_WidgetPrototype LCUIWidget_NewPrototype(const char *name,
						      const char *parent_name);

/** 判断部件类型 */
LCUI_API LCUI_BOOL Widget_CheckType(LCUI_Widget w, const char *type);

/** 判断部件原型 */
LCUI_API LCUI_BOOL Widget_CheckPrototype(LCUI_Widget w,
					 LCUI_WidgetPrototypeC proto);

LCUI_API void *Widget_GetData(LCUI_Widget widget, LCUI_WidgetPrototype proto);

LCUI_API void *Widget_AddData(LCUI_Widget widget,
			      LCUI_WidgetPrototype proto, size_t data_size);

/** 清除部件自带的原型数据 */
LCUI_API void Widget_ClearPrototype(LCUI_Widget widget);

LCUI_END_HEADER

#endif
