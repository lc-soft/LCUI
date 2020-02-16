/*
 * widget_style.h -- widget style library module for LCUI.
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

#ifndef LCUI_WIDGET_STYLE_LIBRARY_H
#define LCUI_WIDGET_STYLE_LIBRARY_H

/** 初始化 */
void LCUIWidget_InitStyle(void);

/** 销毁，释放资源 */
void LCUIWidget_FreeStyle(void);

/** 打印部件的样式表 */
LCUI_API void Widget_PrintStyleSheets(LCUI_Widget w);

/** Set widget style by string */
LCUI_API void Widget_SetStyleString(LCUI_Widget w, const char *name,
				    const char *value);

LCUI_API void Widget_ComputePaddingStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeMarginStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeProperties(LCUI_Widget w);

LCUI_API void Widget_ComputeWidthLimitStyle(LCUI_Widget w,
					    LCUI_LayoutRule rule);

LCUI_API void Widget_ComputeHeightLimitStyle(LCUI_Widget w,
					     LCUI_LayoutRule rule);

LCUI_API void Widget_ComputeHeightStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeWidthStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeSizeStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeVisibilityStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeDisplayStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeOpacityStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeZIndexStyle(LCUI_Widget w);

LCUI_API void Widget_ComputePositionStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeFlexBoxStyle(LCUI_Widget w);

LCUI_API void Widget_ComputeFlexBasisStyle(LCUI_Widget w);

/** 更新当前部件的样式 */
LCUI_API void Widget_UpdateStyle(LCUI_Widget w, LCUI_BOOL is_refresh_all);

/** 更新当前部件的子级部件样式 */
LCUI_API void Widget_UpdateChildrenStyle(LCUI_Widget w,
					 LCUI_BOOL is_refresh_all);

LCUI_API void Widget_AddTaskByStyle(LCUI_Widget w, int key);

/** 直接更新当前部件的样式 */
LCUI_API void Widget_ExecUpdateStyle(LCUI_Widget w, LCUI_BOOL is_update_all);

LCUI_API void Widget_DestroyStyleSheets(LCUI_Widget w);

/** 获取选择器结点 */
LCUI_SelectorNode Widget_GetSelectorNode(LCUI_Widget w);

/** 获取选择器 */
LCUI_API LCUI_Selector Widget_GetSelector(LCUI_Widget w);

/** 获取样式受到影响的子级部件数量 */
LCUI_API size_t Widget_GetChildrenStyleChanges(LCUI_Widget w, int type,
					       const char *name);

#endif
