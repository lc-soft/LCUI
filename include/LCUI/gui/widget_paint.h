/*
 * widget_paint.h -- LCUI widget paint module.
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

#ifndef __WIDGET_PAINT_H
#define __WIDGET_PAINT_H

LCUI_BEGIN_HEADER

/**
 * 标记部件中的无效区域
 * @param[in] w		区域所在的部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 * @returns 标记成功返回 TRUE，如果该区域处于屏幕可见区域外则标记失败，返回FALSE
 */
LCUI_API LCUI_BOOL Widget_InvalidateArea(LCUI_Widget widget,
					 LCUI_RectF *in_rect, int box_type);

/**
 * 取出部件中的无效区域
 * @param[in] w		部件
 * @param[out] rects	输出的区域列表
 * @return 无效区域的数量
 */
LCUI_API size_t Widget_GetInvalidArea(LCUI_Widget w, LinkedList *rects);

/**
 * 将部件中的矩形区域转换成指定范围框内有效的矩形区域
 * @param[in]	w		目标部件
 * @param[in]	in_rect		相对于部件呈现框的矩形区域
 * @param[out]	out_rect	转换后的区域
 * @param[in]	box_type	转换后的区域所处的范围框
 */
LCUI_API int Widget_ConvertArea(LCUI_Widget w, LCUI_Rect *in_rect,
				LCUI_Rect *out_rect, int box_type);

/** 将 LCUI_RectF 类型数据转换为无效区域 */
LCUI_API void RectFToInvalidArea(const LCUI_RectF *rect, LCUI_Rect *area);

/** 将 LCUI_Rect 类型数据转换为无效区域 */
LCUI_API void RectToInvalidArea(const LCUI_Rect *rect, LCUI_Rect *area);

/**
 * 渲染指定部件呈现的图形内容
 * @param[in] w		部件
 * @param[in] paint 	进行绘制时所需的上下文
 * @return		返回实际渲染的部件的数量
 */
LCUI_API size_t Widget_Render(LCUI_Widget w, LCUI_PaintContext paint);

LCUI_API void LCUIWidget_InitRenderer(void);

LCUI_API void LCUIWidget_FreeRenderer(void);

LCUI_END_HEADER

#endif
