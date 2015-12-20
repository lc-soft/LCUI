/* ***************************************************************************
 * rect.c -- Rectangle area handling
 * 
 * Copyright (C) 2012-2014 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * rect.c -- 矩形区域处理
 *
 * 版权所有 (C) 2012-2014 归属于
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

#define LCUIRect_IsIncludeRect(a,b)	\
	b->x >= a->x && b->x + b->w <= a->x + a->w \
	&& b->y >= a->y && b->y + b->h <= a->y + a->h

#define LCUIRect_IsMergeable(a, b)

/* 将数值转换成LCUI_Rect型结构体 */
LCUI_Rect Rect( int x, int y, int w, int h )
{
	LCUI_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

/** 根据容器尺寸，获取指定区域中需要裁剪的区域 */
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

/** 将矩形区域范围调整在容器有效范围内 */
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

/** 检测矩形是否遮盖另一个矩形 */
LCUI_BOOL LCUIRect_IsCoverRect( LCUI_Rect rect1, LCUI_Rect rect2 )
{
	if( rect1.x > rect2.x ) {
		if( rect2.x + rect2.w <= rect1.x ) {
			return FALSE;
		}
	} else {
		if( rect1.x + rect1.w <= rect2.x ) {
			return FALSE;
		}
	}
	if( rect1.y > rect2.y ) {
		if( rect2.y + rect2.h <= rect1.y ) {
			return FALSE;
		}
	} else {
		if( rect1.y + rect1.h <= rect2.y ) {
			return FALSE;
		}
	}
	return TRUE;
}

/** 
 * 获取两个矩形中的重叠矩形
 * @param[in] a		矩形A
 * @param[in] b		矩形B
 * @param[out] out	矩形A和B重叠处的矩形
 * @returns 如果两个矩形重叠，则返回TRUE，否则返回FALSE
 */
LCUI_BOOL LCUIRect_GetOverlayRect( const LCUI_Rect *a, 
				   const LCUI_Rect *b, 
				   LCUI_Rect *out )
{
	if( a->x > b->x ) {
		if( b->x + b->w > a->x + a->w ) {
			out->w = a->w;
		} else {
			out->w = b->x + b->w - a->x;
		}
		out->x = a->x;
	} else {
		if( a->x + a->w > b->x + b->w ) {
			out->w = b->w;
		} else {
			out->w = a->x + a->w - b->x;
		}
		out->x = b->x;
	}
	if( a->y > b->y ) {
		if( b->y + b->h > a->y + a->h ) {
			out->h = a->h;
		} else {
			out->h = b->y + b->h - a->y;
		}
		out->y = a->y;
	} else {
		if( a->y + a->h > b->y + b->h ) {
			out->h = b->h;
		} else {
			out->h = a->y + a->h - b->y;
		}
		out->y = b->y;
	}
	if( out->w <= 0 || out->h <= 0 ) {
		return FALSE;
	}
	return TRUE;
}

static void LCUIRect_MergeRect( LCUI_Rect *big, LCUI_Rect *a, LCUI_Rect *b )
{
	if( a->x < b->x ) {
		big->x = a->x;
	} else {
		big->x = b->x;
	}
	if( a->x + a->w < b->x + b->w ) {
		big->w = b->x + b->w - big->x;
	} else {
		big->w = a->x + a->w - big->x;
	}if( a->y < b->y ) {
		big->y = a->y;
	} else {
		big->y = b->y;
	}
	if( a->y + a->h < b->y + b->h ) {
		big->h = b->y + b->h - big->y;
	} else {
		big->h = a->y + a->h - big->y;
	}
}

void LCUIRect_CutFourRect( LCUI_Rect *rect1, LCUI_Rect *rect2, 
			   LCUI_Rect rects[4] )
{
	rects[0].x = rect2->x;
	rects[0].y = rect2->y;
	rects[0].w = rect1->x - rect2->x;
	rects[0].h = rect1->y + rect1->h - rect2->y;

	rects[1].x = rect2->x;
	rects[1].y = rect1->y + rect1->h;
	rects[1].w = rect1->x + rect1->w - rect2->x;
	rects[1].h = rect2->y + rect2->h - rects[1].y;

	rects[2].x = rect1->x + rect1->w;
	rects[2].y = rect1->y;
	rects[2].w = rect2->w - rects[1].w;
	rects[2].h = rect2->y + rect2->h - rects[2].y;

	rects[3].x = rect1->x;
	rects[3].y = rect2->y;
	rects[3].w = rect2->x + rect2->w - rects[3].x;
	rects[3].h = rect1->y - rect2->y;
}

int RectList_Add( LinkedList *list, LCUI_Rect *rect )
{
	int i, ret;
	LinkedListNode *node, *prev;
	LCUI_Rect *r, or, tr[4];

	if( rect->w <= 0 || rect->h <= 0 ) {
		return -1;
	}
	DEBUG_MSG("list: %p, total: %d, rect(%d,%d,%d,%d)\n", list, 
		   list->length, rect->x, rect->y, rect->w, rect->h);
	LinkedList_ForEach( node, list ) {
		r = (LCUI_Rect*)node->data;
		DEBUG_MSG("r(%d,%d,%d,%d)\n", 
			r->x, r->y, r->w, r->h);
		/* 如果被现有的矩形包含 */
		if( LCUIRect_IsIncludeRect( r, rect ) ) {
			DEBUG_MSG("not add\n");
			return -2;
		}
		/* 如果包含现有的矩形 */
		if( LCUIRect_IsIncludeRect( rect, r ) ) {
			DEBUG_MSG("delete\n");
			prev = node->prev;
			free( node->data );
			LinkedList_DeleteNode( list, node );
			node = prev;
			continue;
		}
		/* 如果与现有的矩形不重叠 */
		if( !LCUIRect_GetOverlayRect( rect, r, &or ) ) {
			DEBUG_MSG("skip\n");
			continue;
		}
		DEBUG_MSG("overlay rect(%d,%d,%d,%d)\n",or.x, or.y,
			   or.w, or.h);
		/* 如果两个矩形相距很近，则直接合并 */
		if( (rect->w - or.w < 20 && rect->h - or.h < 20) || 
		    (r->w - or.w < 20 && r->h - or.h < 20) ) {
			LCUIRect_MergeRect( &or, r, rect );
			DEBUG_MSG("merge rect, rect(%d,%d,%d,%d)\n",
				   or.x, or.y, or.w, or.h);
			free( node->data );
			LinkedList_DeleteNode( list, node );
			return RectList_Add( list, &or );
		}
		LCUIRect_CutFourRect( &or, rect, tr );
		for( ret=0,i=0; i<4&&ret==0; ++i ) {
			ret = RectList_Add( list, &tr[i] );
		}
		if( ret != 0 ) {
			return -3;
		}
		return 1;
	}
	DEBUG_MSG("add\n");
	r = malloc( sizeof(LCUI_Rect) );
	*r = *rect;
	LinkedList_Append( list, r );
	return 0;
}

int RectList_Delete( LinkedList *list, LCUI_Rect *rect )
{
	int i;
	LinkedListNode *prev, *node;
	LCUI_Rect *p_rect, o_rect, tmp_rect[4];

	if( rect->w <= 0 || rect->h <= 0 ) {
		return -1;
	}
	DEBUG_MSG("list: %p, total: %d, rect(%d,%d,%d,%d)\n", list, 
		   list->length, rect->x, rect->y, rect->w, rect->h);
	LinkedList_ForEach( node, list ) {
		p_rect = (LCUI_Rect*)node->data;
		DEBUG_MSG("p_rect(%d,%d,%d,%d)\n", p_rect->x, 
			   p_rect->y, p_rect->w, p_rect->h);
		/* 如果包含现有的矩形 */
		if( LCUIRect_IsIncludeRect( rect, p_rect ) ) {
			DEBUG_MSG("delete\n");
			prev = node->prev;
			free(  node->data );
			LinkedList_DeleteNode( list, node );
			node = prev;
			continue;
		}
		/* 如果被现有的矩形包含，则分割现有矩形 */
		if( LCUIRect_IsIncludeRect( p_rect, rect ) ) {
			DEBUG_MSG("cut 4\n");
			LCUIRect_CutFourRect( rect, p_rect, tmp_rect );
			prev = node->prev;
			free(  node->data );
			LinkedList_DeleteNode( list, node );
			node = prev;
			for( i=0; i<4; ++i ) {
				if( tmp_rect[i].w <= 0
				|| tmp_rect[i].h <= 0 ) {
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
		DEBUG_MSG("cut 2\n");
		LCUIRect_CutFourRect( &o_rect, p_rect, tmp_rect );
		prev = node->prev;
		free(  node->data );
		LinkedList_DeleteNode( list, node );
		node = prev;
		for( i=0; i<4; ++i ) {
			if( tmp_rect[i].w <= 0 || tmp_rect[i].h <= 0 ) {
				continue;
			}
			LinkedList_Insert( list, 0, &tmp_rect[i] );
		}
	}
	DEBUG_MSG("quit\n");
	return 1;
}
