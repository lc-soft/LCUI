/*
 * widget_tree.h -- The widget tree operation set.
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

#ifndef LCUI_WIDGET_TREE_H
#define LCUI_WIDGET_TREE_H

LCUI_BEGIN_HEADER

/** 将部件与子部件列表断开链接 */
LCUI_API int Widget_Unlink(LCUI_Widget widget);

/** 向子部件列表追加部件 */
LCUI_API int Widget_Append(LCUI_Widget container, LCUI_Widget widget);

/** 将部件插入到子部件列表的开头处 */
LCUI_API int Widget_Prepend(LCUI_Widget parent, LCUI_Widget widget);

/** 移除部件，并将其子级部件转移至父部件内 */
LCUI_API int Widget_Unwrap(LCUI_Widget widget);

/** 清空部件内的子级部件 */
LCUI_API void Widget_Empty(LCUI_Widget widget);

/** 获取上一个部件 */
LCUI_API LCUI_Widget Widget_GetPrev(LCUI_Widget w);

/** 获取下一个部件 */
LCUI_API LCUI_Widget Widget_GetNext(LCUI_Widget w);

/** 获取一个子部件 */
LCUI_API LCUI_Widget Widget_GetChild(LCUI_Widget w, size_t index);

/** Traverse the child widget tree */
LCUI_API size_t Widget_Each(LCUI_Widget w,
			    void (*callback)(LCUI_Widget, void *), void *arg);

/** 获取当前点命中的最上层可见部件 */
LCUI_API LCUI_Widget Widget_At(LCUI_Widget widget, int x, int y);

LCUI_API void Widget_DestroyChildren(LCUI_Widget w);

LCUI_API void Widget_PrintTree(LCUI_Widget w);

LCUI_END_HEADER

#endif
