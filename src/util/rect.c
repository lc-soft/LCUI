/* rect.c -- Rectangle area handling
 *
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

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

LCUI_Rect Rect(int x, int y, int w, int h)
{
	LCUI_Rect r;
	r.x = x;
	r.y = y;
	r.width = w;
	r.height = h;
	return r;
}

void LCUIRect_ToRectF(const LCUI_Rect *rect, LCUI_RectF *rectf, float scale)
{
	rectf->x = rect->x * scale;
	rectf->y = rect->y * scale;
	rectf->width = rect->width * scale;
	rectf->height = rect->height * scale;
}

void LCUIRect_Scale(const LCUI_Rect *src, LCUI_Rect *dst, float scale)
{
	dst->x = iround(src->x * scale);
	dst->y = iround(src->y * scale);
	dst->width = iround(src->width * scale);
	dst->height = iround(src->height * scale);
}

void LCUIRectF_ToRect(const LCUI_RectF *rectf, LCUI_Rect *rect, float scale)
{
	rect->x = iround(rectf->x * scale);
	rect->y = iround(rectf->y * scale);
	rect->width = iround(rectf->width * scale);
	rect->height = iround(rectf->height * scale);
}

/* FIXME: need new shorter name */
void LCUIRect_GetCutArea(int box_w, int box_h, LCUI_Rect rect, LCUI_Rect *cut)
{
	cut->x = 0;
	cut->y = 0;
	cut->width = rect.width;
	cut->height = rect.height;
	if (rect.x < 0) {
		cut->width += rect.x;
		cut->x = 0 - rect.x;
	}
	if (rect.x + rect.width > box_w) {
		cut->width -= (rect.x + rect.width - box_w);
	}

	if (rect.y < 0) {
		cut->height += rect.y;
		cut->y = 0 - rect.y;
	}
	if (rect.y + rect.height > box_h) {
		cut->height -= (rect.y + rect.height - box_h);
	}
}

/* FIXME: need new shorter name */
LCUI_BOOL LCUIRect_ValidateArea(LCUI_Rect *rect, int box_w, int box_h)
{
	LCUI_BOOL overflow = FALSE;

	if (rect->x < 0) {
		overflow = TRUE;
		rect->width += rect->x;
		rect->x = 0;
	}
	if (rect->y < 0) {
		overflow = TRUE;
		rect->height += rect->y;
		rect->y = 0;
	}

	if (rect->x + rect->width > box_w) {
		overflow = TRUE;
		if (rect->x < box_w) {
			rect->width = box_w - rect->x;
		} else {
			rect->width = 0;
		}
	}
	if (rect->y + rect->height > box_h) {
		overflow = TRUE;
		if (rect->y < box_h) {
			rect->height = box_h - rect->y;
		} else {
			rect->height = 0;
		}
	}
	return overflow;
}

LCUI_BOOL LCUIRectF_ValidateArea(LCUI_RectF *rect, float box_w, float box_h)
{
	LCUI_BOOL overflow = FALSE;

	if (rect->x < 0) {
		overflow = TRUE;
		rect->width += rect->x;
		rect->x = 0;
	}
	if (rect->y < 0) {
		overflow = TRUE;
		rect->height += rect->y;
		rect->y = 0;
	}

	if (rect->x + rect->width - box_w > 0) {
		overflow = TRUE;
		if (rect->x - box_w < 0) {
			rect->width = box_w - rect->x;
		} else {
			rect->width = 0;
		}
	}
	if (rect->y + rect->height - box_h > 0) {
		overflow = TRUE;
		if (rect->y - box_h < 0) {
			rect->height = box_h - rect->y;
		} else {
			rect->height = 0;
		}
	}
	return overflow;
}

/* FIXME: need new shorter name */
LCUI_BOOL LCUIRect_IsCoverRect(const LCUI_Rect *a, const LCUI_Rect *b)
{
	if (a->x > b->x) {
		if (b->x + b->width <= a->x) {
			return FALSE;
		}
	} else {
		if (a->x + a->width <= b->x) {
			return FALSE;
		}
	}
	if (a->y > b->y) {
		if (b->y + b->height <= a->y) {
			return FALSE;
		}
	} else {
		if (a->y + a->height <= b->y) {
			return FALSE;
		}
	}
	return TRUE;
}

LCUI_BOOL LCUIRectF_IsCoverRect(const LCUI_RectF *a, const LCUI_RectF *b)
{
	if (a->x > b->x) {
		if (b->x + b->width <= a->x) {
			return FALSE;
		}
	} else {
		if (a->x + a->width <= b->x) {
			return FALSE;
		}
	}
	if (a->y > b->y) {
		if (b->y + b->height <= a->y) {
			return FALSE;
		}
	} else {
		if (a->y + a->height <= b->y) {
			return FALSE;
		}
	}
	return TRUE;
}

/* FIXME: need new shorter name */
LCUI_BOOL LCUIRect_GetOverlayRect(const LCUI_Rect *a, const LCUI_Rect *b,
				  LCUI_Rect *out)
{
	if (a->x > b->x) {
		if (b->x + b->width > a->x + a->width) {
			out->width = a->width;
		} else {
			out->width = b->x + b->width - a->x;
		}
		out->x = a->x;
	} else {
		if (a->x + a->width > b->x + b->width) {
			out->width = b->width;
		} else {
			out->width = a->x + a->width - b->x;
		}
		out->x = b->x;
	}
	if (a->y > b->y) {
		if (b->y + b->height > a->y + a->height) {
			out->height = a->height;
		} else {
			out->height = b->y + b->height - a->y;
		}
		out->y = a->y;
	} else {
		if (a->y + a->height > b->y + b->height) {
			out->height = b->height;
		} else {
			out->height = a->y + a->height - b->y;
		}
		out->y = b->y;
	}
	if (out->width <= 0 || out->height <= 0) {
		return FALSE;
	}
	return TRUE;
}

LCUI_BOOL LCUIRectF_GetOverlayRect(const LCUI_RectF *a, const LCUI_RectF *b,
				   LCUI_RectF *out)
{
	if (a->x > b->x) {
		if (b->x + b->width > a->x + a->width) {
			out->width = a->width;
		} else {
			out->width = b->x + b->width - a->x;
		}
		out->x = a->x;
	} else {
		if (a->x + a->width > b->x + b->width) {
			out->width = b->width;
		} else {
			out->width = a->x + a->width - b->x;
		}
		out->x = b->x;
	}
	if (a->y > b->y) {
		if (b->y + b->height > a->y + a->height) {
			out->height = a->height;
		} else {
			out->height = b->y + b->height - a->y;
		}
		out->y = a->y;
	} else {
		if (a->y + a->height > b->y + b->height) {
			out->height = b->height;
		} else {
			out->height = a->y + a->height - b->y;
		}
		out->y = b->y;
	}
	if (out->width <= 0 || out->height <= 0) {
		return FALSE;
	}
	return TRUE;
}

void LCUIRect_MergeRect(LCUI_Rect *big, const LCUI_Rect *a, const LCUI_Rect *b)
{
	if (a->x < b->x) {
		big->x = a->x;
	} else {
		big->x = b->x;
	}
	if (a->x + a->width < b->x + b->width) {
		big->width = b->x + b->width - big->x;
	} else {
		big->width = a->x + a->width - big->x;
	}
	if (a->y < b->y) {
		big->y = a->y;
	} else {
		big->y = b->y;
	}
	if (a->y + a->height < b->y + b->height) {
		big->height = b->y + b->height - big->y;
	} else {
		big->height = a->y + a->height - big->y;
	}
}

void LCUIRectF_MergeRect(LCUI_RectF *big, const LCUI_RectF *a, const LCUI_RectF *b)
{
	if (a->x < b->x) {
		big->x = a->x;
	} else {
		big->x = b->x;
	}
	if (a->x + a->width < b->x + b->width) {
		big->width = b->x + b->width - big->x;
	} else {
		big->width = a->x + a->width - big->x;
	}
	if (a->y < b->y) {
		big->y = a->y;
	} else {
		big->y = b->y;
	}
	if (a->y + a->height < b->y + b->height) {
		big->height = b->y + b->height - big->y;
	} else {
		big->height = a->y + a->height - big->y;
	}
}

/* FIXME: need new shorter name */
void LCUIRect_CutFourRect(LCUI_Rect *rect1, LCUI_Rect *rect2,
			  LCUI_Rect rects[4])
{
	rects[0].x = rect2->x;
	rects[0].y = rect2->y;
	rects[0].width = rect1->x - rect2->x;
	rects[0].height = rect1->y + rect1->height - rect2->y;

	rects[1].x = rect2->x;
	rects[1].y = rect1->y + rect1->height;
	rects[1].width = rect1->x + rect1->width - rect2->x;
	rects[1].height = rect2->y + rect2->height - rects[1].y;

	rects[2].x = rect1->x + rect1->width;
	rects[2].y = rect1->y;
	rects[2].width = rect2->width - rects[1].width;
	rects[2].height = rect2->y + rect2->height - rects[2].y;

	rects[3].x = rect1->x;
	rects[3].y = rect2->y;
	rects[3].width = rect2->x + rect2->width - rects[3].x;
	rects[3].height = rect1->y - rect2->y;
}

void LCUIRect_Split(LCUI_Rect *base, LCUI_Rect *target, LCUI_Rect rects[4])
{
	if (LCUIRect_IsIncludeRect(target, base)) {
		LCUIRect_CutFourRect(base, target, rects);
		return;
	}
	/* Right */
	if (target->x >= base->x) {
		/* Top */
		if (target->y < base->y) {
			rects[0].x = target->x;
			rects[0].y = target->y;
			rects[0].width = target->width;
			rects[0].height = base->y - target->y;
			rects[1].x = base->x + base->width;
			rects[1].y = base->y;
			rects[1].width =
			    target->x + target->width - base->x - base->width;
			rects[1].height = target->y + target->height - base->y;
		} else {
			/* Bottom */
			rects[0].x = base->x + base->width;
			rects[0].y = target->y;
			rects[0].width =
			    target->x + target->width - base->x - base->width;
			rects[0].height = target->height;
			rects[1].x = target->x;
			rects[1].y = base->y + base->height;
			rects[1].width = base->x + base->width - target->x;
			rects[1].height =
			    target->y + target->height - base->y - base->height;
		}
	} else {
		/* Top */
		if (target->y < base->y) {
			rects[0].x = target->x;
			rects[0].y = target->y;
			rects[0].width = target->width;
			rects[0].height = base->y - target->y;
			rects[1].x = target->x;
			rects[1].y = base->y;
			rects[1].width = base->x - target->x;
			rects[1].height = target->y + target->height - base->y;
		} else {
			/* Bottom */
			rects[0].x = target->x;
			rects[0].y = target->y;
			rects[0].width = base->x - target->x;
			rects[0].height = target->height;
			rects[1].x = base->x;
			rects[1].y = base->y + base->height;
			rects[1].width = target->x + target->width - base->x;
			rects[1].height =
			    target->y + target->height - base->y - base->height;
		}
	}
	rects[2].x = 0;
	rects[2].y = 0;
	rects[2].width = 0;
	rects[2].height = 0;
	rects[3].x = 0;
	rects[3].y = 0;
	rects[3].width = 0;
	rects[3].height = 0;
}

int RectList_AddEx(LinkedList *list, LCUI_Rect *rect, LCUI_BOOL auto_merge)
{
	int x_distance, y_distance;

	LCUI_Rect *p, union_rect;
	LinkedListNode *node, *prev;

	if (rect->width <= 0 || rect->height <= 0) {
		return -1;
	}
	for (LinkedList_Each(node, list)) {
		p = node->data;
		/* 如果被现有的矩形包含 */
		if (LCUIRect_IsIncludeRect(p, rect)) {
			return -2;
		}
		/* 如果包含现有的矩形 */
		if (LCUIRect_IsIncludeRect(rect, p)) {
			prev = node->prev;
			free(node->data);
			LinkedList_DeleteNode(list, node);
			node = prev;
			continue;
		}
		if (!auto_merge) {
			continue;
		}
		x_distance = p->x + p->width - rect->x - rect->width;
		y_distance = p->y + p->height - rect->y - rect->height;
		if ((x_distance <= 10 && x_distance >= -10) &&
		    (y_distance <= 10 && y_distance >= -10)) {
			LCUIRect_MergeRect(&union_rect, p, rect);
			free(node->data);
			LinkedList_DeleteNode(list, node);
			return RectList_Add(list, &union_rect);
		}
	}
	p = NEW(LCUI_Rect, 1);
	*p = *rect;
	LinkedList_Append(list, p);
	return 0;
}

int RectList_Add(LinkedList *list, LCUI_Rect *rect)
{
	return RectList_AddEx(list, rect, TRUE);
}

int RectList_Delete(LinkedList *list, LCUI_Rect *rect)
{
	int i;

	LCUI_BOOL deletable;
	LCUI_Rect *p, child_rects[4];

	LinkedList extra_list;
	LinkedListNode *prev, *node;

	if (rect->width <= 0 || rect->height <= 0) {
		return -1;
	}
	LinkedList_Init(&extra_list);
	for (LinkedList_Each(node, list)) {
		p = node->data;
		/* 如果包含现有的矩形 */
		if (LCUIRect_IsIncludeRect(rect, p)) {
			prev = node->prev;
			free(node->data);
			LinkedList_DeleteNode(list, node);
			node = prev;
			continue;
		}
		if (!LCUIRect_IsCoverRect(p, rect)) {
			continue;
		}
		deletable = TRUE;
		LCUIRect_Split(rect, p, child_rects);
		for (i = 0; i < 4; ++i) {
			if (child_rects[i].width <= 0 ||
			    child_rects[i].height <= 0) {
				continue;
			}
			if (deletable) {
				prev = node->prev;
				free(node->data);
				LinkedList_DeleteNode(list, node);
				node = prev;
				deletable = FALSE;
			}
			RectList_AddEx(&extra_list, &child_rects[i], FALSE);
		}
	}
	LinkedList_Concat(list, &extra_list);
	return 1;
}
