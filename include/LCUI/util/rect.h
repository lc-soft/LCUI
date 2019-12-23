/*
 * rect.h -- Rectangle area handling
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#ifndef LCUI_UTIL_RECT_H
#define LCUI_UTIL_RECT_H

#define LCUIRect_IsIncludeRect(a, b)                                       \
	((b)->x >= (a)->x && (b)->x + (b)->width <= (a)->x + (a)->width && \
	 (b)->y >= (a)->y && (b)->y + (b)->height <= (a)->y + (a)->height)

#define LCUIRectF_IsIncludeRect LCUIRect_IsIncludeRect

LCUI_BEGIN_HEADER

/* 将数值转换成LCUI_Rect型结构体 */
LCUI_API LCUI_Rect Rect(int x, int y, int w, int h);

/** 根据容器尺寸，获取指定区域中需要裁剪的区域 */
LCUI_API void LCUIRect_GetCutArea(int box_w, int box_h, LCUI_Rect rect,
				  LCUI_Rect *cut);

#define LCUIRect_HasPoint(rect, X, Y)                                         \
	(X >= (rect)->x && Y >= (rect)->y && X < (rect)->x + (rect)->width && \
	 Y < (rect)->y + (rect)->height)

/** 将矩形区域范围调整在容器有效范围内 */
LCUI_API LCUI_BOOL LCUIRect_ValidateArea(LCUI_Rect *rect, int box_w, int box_h);

LCUI_API LCUI_BOOL LCUIRectF_ValidateArea(LCUI_RectF *rect, float box_w,
					  float box_h);

LCUI_API void LCUIRect_ToRectF(const LCUI_Rect *rect, LCUI_RectF *rectf,
			       float scale);

LCUI_API void LCUIRect_Scale(const LCUI_Rect *src, LCUI_Rect *dst, float scale);

LCUI_API void LCUIRectF_ToRect(const LCUI_RectF *rectf, LCUI_Rect *rect,
			       float scale);

/** 检测矩形是否遮盖另一个矩形 */
LCUI_API LCUI_BOOL LCUIRect_IsCoverRect(const LCUI_Rect *a, const LCUI_Rect *b);

LCUI_API LCUI_BOOL LCUIRectF_IsCoverRect(const LCUI_RectF *a,
					 const LCUI_RectF *b);

/**
 * 获取两个矩形中的重叠矩形
 * @param[in] a		矩形A
 * @param[in] b		矩形B
 * @param[out] out	矩形A和B重叠处的矩形
 * @returns 如果两个矩形重叠，则返回TRUE，否则返回FALSE
 */
LCUI_API LCUI_BOOL LCUIRect_GetOverlayRect(const LCUI_Rect *a,
					   const LCUI_Rect *b, LCUI_Rect *out);

LCUI_API LCUI_BOOL LCUIRectF_GetOverlayRect(const LCUI_RectF *a,
					    const LCUI_RectF *b,
					    LCUI_RectF *out);

/** 合并两个矩形 */
LCUI_API void LCUIRect_MergeRect(LCUI_Rect *big, const LCUI_Rect *a,
				 const LCUI_Rect *b);

LCUI_API void LCUIRectF_MergeRect(LCUI_RectF *big, const LCUI_RectF *a,
				  const LCUI_RectF *b);

/**
 * 根据重叠矩形 rect1，将矩形 rect2 分割成四个矩形
 * 分割方法如下：
 * ┏━━┳━━━━━━┓
 * ┃    ┃     3      ┃
 * ┃ 0  ┣━━━┳━━┃
 * ┃    ┃rect1 ┃    ┃
 * ┃    ┃      ┃ 2  ┃
 * ┣━━┻━━━┫    ┃
 * ┃     1      ┃    ┃
 * ┗━━━━━━┻━━┛
 *
 * rect2 必须被 rect1 完全包含
 */
LCUI_API void LCUIRect_CutFourRect(LCUI_Rect *rect1, LCUI_Rect *rect2,
				   LCUI_Rect rects[4]);

LCUI_API void LCUIRect_Split(LCUI_Rect *base, LCUI_Rect *target,
			     LCUI_Rect rects[4]);

INLINE LCUI_BOOL LCUIRectF_IsEquals(const LCUI_RectF *a, const LCUI_RectF *b)
{
	return (int)(100 * (a->x - b->x)) == 0 &&
	       (int)(100 * (a->y - b->y)) == 0 &&
	       (int)(100 * (a->width - b->width)) == 0 &&
	       (int)(100 * (a->height - b->height)) == 0;
}

INLINE LCUI_BOOL LCUIRect_IsEquals(const LCUI_Rect *a, const LCUI_Rect *b)
{
	return a->x == b->x && a->y == b->y && a->width == b->width &&
	       a->height == b->height;
}

LCUI_API int RectList_AddEx(LinkedList *list, LCUI_Rect *rect,
			    LCUI_BOOL auto_merge);

/** 添加一个脏矩形记录 */
LCUI_API int RectList_Add(LinkedList *list, LCUI_Rect *rect);

/** 删除脏矩形 */
LCUI_API int RectList_Delete(LinkedList *list, LCUI_Rect *rect);

#define RectList_Clear(LIST) LinkedList_Clear(LIST, free)

LCUI_END_HEADER

#endif
