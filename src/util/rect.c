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


//#define DEBUG
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

#define LCUIRect_IsIncludeRect(a,b)	\
	b->x >= a->x && b->x + b->width <= a->x + a->width \
	&& b->y >= a->y && b->y + b->height <= a->y + a->height

LCUI_Rect Rect( int x, int y, int w, int h )
{
	LCUI_Rect r;
	r.x = x;
	r.y = y;
	r.width = w;
	r.height = h;
	return r;
}

void LCUIRect_ToRectF( const LCUI_Rect *rect,
		       LCUI_RectF *rectf, float scale )
{
	rectf->x = rect->x * scale;
	rectf->y = rect->y * scale;
	rectf->width = rect->width * scale;
	rectf->height = rect->height * scale;
}

void LCUIRect_Scale( const LCUI_Rect *src,
		     LCUI_Rect *dst, float scale )
{
	dst->x = iround( src->x * scale );
	dst->y = iround( src->y * scale );
	dst->width = iround( src->width * scale );
	dst->height = iround( src->height * scale );
}

void LCUIRectF_ToRect( const LCUI_RectF *rectf,
		       LCUI_Rect *rect, float scale )
{
	rect->x = iround( rectf->x * scale );
	rect->y = iround( rectf->y * scale );
	rect->width = iround( rectf->width * scale );
	rect->height = iround( rectf->height * scale );
}

void LCUIRect_GetCutArea( int box_w, int box_h,
			  LCUI_Rect rect, LCUI_Rect *cut )
{
	cut->x = 0;
	cut->y = 0;
	cut->width = rect.width;
	cut->height = rect.height;
	/* 获取需裁剪的区域 */
	if(rect.x < 0) {
		cut->width += rect.x;
		cut->x = 0 - rect.x;
	}
	if(rect.x + rect.width > box_w) {
		cut->width -= (rect.x + rect.width - box_w);
	}

	if(rect.y < 0) {
		cut->height += rect.y;
		cut->y = 0 - rect.y;
	}
	if(rect.y + rect.height > box_h) {
		cut->height -= (rect.y + rect.height - box_h);
	}
}

void LCUIRect_ValidateArea( LCUI_Rect *rect, int box_w, int box_h )
{
	if( rect->x < 0 ) {
		rect->width += rect->x;
		rect->x = 0;
	}
	if( rect->y < 0 ) {
		rect->height += rect->y;
		rect->y = 0;
	}

	if( rect->x + rect->width > box_w ) {
		if( rect->x < box_w ) {
			rect->width = box_w - rect->x;
		} else {
			rect->width = 0;
		}
	}
	if( rect->y + rect->height > box_h ) {
		if( rect->y < box_h ) {
			rect->height = box_h - rect->y;
		} else {
			rect->height = 0;
		}
	}
}

void LCUIRectF_ValidateArea( LCUI_RectF *rect, float box_w, float box_h )
{
	if( rect->x < 0 ) {
		rect->width += rect->x;
		rect->x = 0;
	}
	if( rect->y < 0 ) {
		rect->height += rect->y;
		rect->y = 0;
	}

	if( rect->x + rect->width - box_w > 0 ) {
		if( rect->x - box_w < 0 ) {
			rect->width = box_w - rect->x;
		} else {
			rect->width = 0;
		}
	}
	if( rect->y + rect->height - box_h > 0 ) {
		if( rect->y - box_h < 0 ) {
			rect->height = box_h - rect->y;
		} else {
			rect->height = 0;
		}
	}
}

LCUI_BOOL LCUIRect_IsCoverRect( LCUI_Rect *rect1, LCUI_Rect *rect2 )
{
	if( rect1->x > rect2->x ) {
		if( rect2->x + rect2->width <= rect1->x ) {
			return FALSE;
		}
	} else {
		if( rect1->x + rect1->width <= rect2->x ) {
			return FALSE;
		}
	}
	if( rect1->y > rect2->y ) {
		if( rect2->y + rect2->height <= rect1->y ) {
			return FALSE;
		}
	} else {
		if( rect1->y + rect1->height <= rect2->y ) {
			return FALSE;
		}
	}
	return TRUE;
}

LCUI_BOOL LCUIRect_GetOverlayRect( const LCUI_Rect *a,
				   const LCUI_Rect *b,
				   LCUI_Rect *out )
{
	if( a->x > b->x ) {
		if( b->x + b->width > a->x + a->width ) {
			out->width = a->width;
		} else {
			out->width = b->x + b->width - a->x;
		}
		out->x = a->x;
	} else {
		if( a->x + a->width > b->x + b->width ) {
			out->width = b->width;
		} else {
			out->width = a->x + a->width - b->x;
		}
		out->x = b->x;
	}
	if( a->y > b->y ) {
		if( b->y + b->height > a->y + a->height ) {
			out->height = a->height;
		} else {
			out->height = b->y + b->height - a->y;
		}
		out->y = a->y;
	} else {
		if( a->y + a->height > b->y + b->height ) {
			out->height = b->height;
		} else {
			out->height = a->y + a->height - b->y;
		}
		out->y = b->y;
	}
	if( out->width <= 0 || out->height <= 0 ) {
		return FALSE;
	}
	return TRUE;
}

LCUI_BOOL LCUIRectF_GetOverlayRect( const LCUI_RectF *a,
				    const LCUI_RectF *b,
				    LCUI_RectF *out )
{
	if( a->x > b->x ) {
		if( b->x + b->width > a->x + a->width ) {
			out->width = a->width;
		} else {
			out->width = b->x + b->width - a->x;
		}
		out->x = a->x;
	} else {
		if( a->x + a->width > b->x + b->width ) {
			out->width = b->width;
		} else {
			out->width = a->x + a->width - b->x;
		}
		out->x = b->x;
	}
	if( a->y > b->y ) {
		if( b->y + b->height > a->y + a->height ) {
			out->height = a->height;
		} else {
			out->height = b->y + b->height - a->y;
		}
		out->y = a->y;
	} else {
		if( a->y + a->height > b->y + b->height ) {
			out->height = b->height;
		} else {
			out->height = a->y + a->height - b->y;
		}
		out->y = b->y;
	}
	if( out->width <= 0 || out->height <= 0 ) {
		return FALSE;
	}
	return TRUE;
}

void LCUIRect_MergeRect( LCUI_Rect *big, LCUI_Rect *a, LCUI_Rect *b )
{
	if( a->x < b->x ) {
		big->x = a->x;
	} else {
		big->x = b->x;
	}
	if( a->x + a->width < b->x + b->width ) {
		big->width = b->x + b->width - big->x;
	} else {
		big->width = a->x + a->width - big->x;
	}
	if( a->y < b->y ) {
		big->y = a->y;
	} else {
		big->y = b->y;
	}
	if( a->y + a->height < b->y + b->height ) {
		big->height = b->y + b->height - big->y;
	} else {
		big->height = a->y + a->height - big->y;
	}
}

void LCUIRect_CutFourRect( LCUI_Rect *rect1, LCUI_Rect *rect2,
			   LCUI_Rect rects[4] )
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

int RectList_Add( LinkedList *list, LCUI_Rect *rect )
{
	LCUI_Rect *added_rect, union_rect;
	LinkedListNode *node, *prev;
	if( rect->width <= 0 || rect->height <= 0 ) {
		return -1;
	}
	for( LinkedList_Each( node, list ) ) {
		added_rect = node->data;
		/* 如果被现有的矩形包含 */
		if( LCUIRect_IsIncludeRect( added_rect, rect ) ) {
			return -2;
		}
		/* 如果包含现有的矩形 */
		if( LCUIRect_IsIncludeRect( rect, added_rect ) ) {
			prev = node->prev;
			free( node->data );
			LinkedList_DeleteNode( list, node );
			node = prev;
			continue;
		}
		/* 如果与现有的矩形不重叠 */
		if( !LCUIRect_GetOverlayRect( rect, added_rect, &union_rect ) ) {
			continue;
		}
		/* 直接合并 */
		LCUIRect_MergeRect( &union_rect, added_rect, rect );
		free( node->data );
		LinkedList_DeleteNode( list, node );
		return RectList_Add( list, &union_rect );
	}
	added_rect = NEW( LCUI_Rect, 1 );
	*added_rect = *rect;
	LinkedList_Append( list, added_rect );
	return 0;
}

int RectList_Delete( LinkedList *list, LCUI_Rect *rect )
{
	int i;
	LinkedListNode *prev, *node;
	LCUI_Rect *p_rect, o_rect, tmp_rect[4];

	if( rect->width <= 0 || rect->height <= 0 ) {
		return -1;
	}
	for( LinkedList_Each( node, list ) ) {
		p_rect = node->data;
		/* 如果包含现有的矩形 */
		if( LCUIRect_IsIncludeRect( rect, p_rect ) ) {
			prev = node->prev;
			free( node->data );
			LinkedList_DeleteNode( list, node );
			node = prev;
			continue;
		}
		/* 如果被现有的矩形包含，则分割现有矩形 */
		if( LCUIRect_IsIncludeRect( p_rect, rect ) ) {
			LCUIRect_CutFourRect( rect, p_rect, tmp_rect );
			prev = node->prev;
			free( node->data );
			LinkedList_DeleteNode( list, node );
			node = prev;
			for( i = 0; i < 4; ++i ) {
				if( tmp_rect[i].width <= 0
				    || tmp_rect[i].height <= 0 ) {
					continue;
				}
				LinkedList_Insert( list, 0, &tmp_rect[0] );
			}
			/*
			 * 既然现有矩形包含了这个矩形，那么不用继续遍历了，
			 * 因为不会有其它矩形会与这个矩形相交，直接退出即可。
			 */
			return 0;
		}
		/* 如果与现有的矩形不重叠 */
		if( !LCUIRect_GetOverlayRect( rect, p_rect, &o_rect ) ) {
			continue;
		}
		LCUIRect_CutFourRect( &o_rect, p_rect, tmp_rect );
		prev = node->prev;
		free( node->data );
		LinkedList_DeleteNode( list, node );
		node = prev;
		for( i = 0; i < 4; ++i ) {
			if( tmp_rect[i].width <= 0 || tmp_rect[i].height <= 0 ) {
				continue;
			}
			LinkedList_Insert( list, 0, &tmp_rect[i] );
		}
	}
	return 1;
}
