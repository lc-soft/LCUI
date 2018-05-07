/*
 * border.c -- graph border draw support.
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


#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <math.h>

static void GetRoundBound( LCUI_Rect2 *bound, LCUI_Rect *rect,
			   int radius, LCUI_Pos center )
{
	bound->right = radius * 2;
	if( center.x + radius > rect->x + rect->width ) {
		bound->right -= (center.x + radius - rect->x - rect->width);
	}
	if( bound->right < 0 ) {
		bound->right = 0;
	}
	bound->left = center.x > radius ? 0 : radius - center.x;
	if( bound->left < 0 ) {
		bound->left = 0;
	}
	bound->bottom = radius * 2;
	if( center.y + radius > rect->y + rect->height ) {
		bound->bottom -= (center.y + radius - rect->y - rect->height);
	}
	bound->top = center.y > radius ? 0 : radius - center.y;
	if( bound->top < 0 ) {
		bound->top = 0;
	}
}

/** 绘制左上角的圆角，从左边框的上端到上边框的左端绘制一条圆角线 */
static int Graph_DrawRoundBorderLeftTop( LCUI_Graph *dst, LCUI_Pos center,
					 int radius, int width, 
					 LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	/* 预先计算xy轴坐标的有效范围 */
	GetRoundBound( &bound, &rect, radius, center );
	/* 预先计算圆心的线性坐标 */
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	/* 根据y轴计算各点的x轴坐标并填充点 */
	for( y = 0; y <= radius; ++y, center_px -= dst->width ) {
		if( radius - y >= bound.bottom || radius - y < bound.top ) {
			continue;
		}
		/* 计算出x轴整数坐标 */
		x = (int)sqrt( pow( radius, 2 ) - y*y );
		if( width > 0 && radius - x >= bound.left &&
		    radius - x <= bound.right ) {
			px = center_px - x;
			*px = color;
		}
		/* 定位到起点 */
		bound_px = px = center_px - center.x;
		if( radius - x > bound.right ) {
			bound_px += bound.right - bound.left;
		} else {
			bound_px += radius - x - bound.left;
		}
		/* 加上圆与背景图的左边距 */
		bound_px += (center.x - radius);
		for( ; px < bound_px; ++px ) {
			px->alpha = 0;
		}
		/* 计算需要向右填充的像素点的个数n */
		n = radius - x + width;
		n = n > radius ? x : width;
		/* 如果该点x轴坐标小于最小x轴坐标 */
		if( radius - x < bound.left ) {
			/* 重新确定起点坐标pos和填充的像素点的个数n */
			px = center_px - radius + bound.left - 1;
			n -= (bound.left - radius + x);
		} else {
			px = center_px - x;
		}
		/* 从下一个像素点开始 */
		++px;
		/* 如果填充的像素点超出了最大x轴范围 */
		if( radius - x + n > bound.right ) {
			/* 重新确定需要填充的像素点的个数n */
			n = bound.right - radius + x;
		}
		bound_px = px + n - 1;
		/* 开始填充当前点右边的n-1个像素点 */
		for( ; px < bound_px; ++px ) {
			*px = color;
		}
	}
	return 0;
}

/** 绘制左上角的圆角，从上边框的左端到左边框的上端绘制一条圆角线 */
static int Graph_DrawRoundBorderTopLeft( LCUI_Graph *dst, LCUI_Pos center,
					 int radius, int width,
					 LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	/* 预先计算xy轴坐标的有效范围 */
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( x = 0; x <= radius; ++x ) {
		if( radius - x >= bound.right || radius - x < bound.left ) {
			continue;
		}
		y = (int)sqrt( pow( radius, 2 ) - x*x );
		if( width > 0 && radius - y >= bound.top
		    && radius - y <= bound.bottom ) {
			px = center_px - y * dst->width - x;
			*px = color;
		}
		/* 计算起点坐标 */
		if( radius - y < bound.top ) {
			px = center_px;
			px -= ((radius + bound.top - 1)*dst->width);
		} else {
			px = center_px - y * dst->width - x;
		}
		if( radius - y > bound.bottom ) {
			n = bound.bottom - bound.top;
		} else {
			n = radius - y - bound.top;
		}
		px -= dst->width;
		/* 加上圆与背景图的上边距 */
		n += (center.y - radius);
		bound_px = px - n * dst->width;
		for( ; px > bound_px; px -= dst->width ) {
			px->alpha = 0;
		}
		/* 计算需要向下填充的像素点的个数n */
		n = radius - y + width;
		n = n > radius ? y : width;
		if( radius - y < bound.top ) {
			/* 重新确定起点坐标pos和填充的像素点的个数n */
			px = center_px - (radius + bound.top - 1)*dst->width;
			n -= (bound.top - radius + y);
		} else {
			px = center_px - y * dst->width - x;
		}
		/* 从下一行像素点开始 */
		px += dst->width;
		/* 如果填充的像素点超出了最大y轴范围 */
		if( radius - y + n > bound.bottom ) {
			/* 重新确定需要填充的像素点的个数n */
			n = bound.bottom - radius + y;
		}
		bound_px = px + (n - 1) * dst->width;
		/* 开始填充当前点下边的n-2个像素点 */
		for( ; px < bound_px; px += dst->width ) {
			*px = color;
		}
	}
	return 0;
}

/** 绘制右上角的圆角，从右边框的上端到上边框的右端绘制一条圆角线 */
static int Graph_DrawRoundBorderRightTop( LCUI_Graph *dst, LCUI_Pos center,
					  int radius, int width,
					  LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( y = 0; y <= radius; ++y, center_px -= dst->width ) {
		if( radius - y >= bound.bottom || radius - y < bound.top ) {
			continue;
		}
		x = (int)sqrt( pow( radius, 2 ) - y*y );
		if( width > 0 && radius + x >= bound.left
		    && radius + x <= bound.right ) {
			px = center_px + x;
			*px = color;
		}
		bound_px = center_px + bound.right - radius;
		if( radius + x < bound.left ) {
			px = center_px + bound.left - radius;
		} else {
			px = center_px + x;
		}
		for( ++px; px <= bound_px; ++px ) {
			px->alpha = 0;
		}
		/* 计算需要向左填充的像素点的个数n */
		n = radius + x - width;
		n = n < bound.left ? x + radius - bound.left : width;
		if( radius + x > bound.right ) {
			px = center_px - radius + bound.right - 1;
			n -= (radius + x - bound.right);
		} else {
			px = center_px + x;
		}
		bound_px = px - n;
		for( ; px > bound_px; --px ) {
			*px = color;
		}
	}
	return 0;
}

/* 绘制右上角的圆角，从上边框的右端到右边框的上端绘制一条圆角线 */
static int Graph_DrawRoundBorderTopRight( LCUI_Graph *dst, LCUI_Pos center,
					  int radius, int width,
					  LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( x = 0; x <= radius; ++x ) {
		if( radius + x >= bound.right || radius + x < bound.left ) {
			continue;
		}
		y = (int)sqrt( pow( radius, 2 ) - x*x );
		if( width > 0 && radius - y >= bound.top
		    && radius - y <= bound.bottom ) {
			px = center_px - y * dst->width + x;
			*px = color;
		}
		if( radius - y < bound.top ) {
			px = center_px;
			px -= ((radius + bound.top - 1)*dst->width);
		} else {
			px = center_px - y * dst->width + x;
		}
		if( radius - y > bound.bottom ) {
			n = bound.bottom - bound.top;
		} else {
			n = radius - y - bound.top;
		}
		px -= dst->width;
		n += (center.y - radius);
		bound_px = px - n * dst->width;
		for( ; px > bound_px; px -= dst->width ) {
			px->alpha = 0;
		}
		n = radius - y + width;
		n = n > radius ? y : width;
		if( radius - y < bound.top ) {
			px = center_px - (radius + bound.top - 1)*dst->width;
			n -= (bound.top - radius + y);
		} else {
			px = center_px - y * dst->width + x;
		}
		px += dst->width;
		if( radius - y + n > bound.bottom ) {
			n = bound.bottom - radius + y;
		}
		bound_px = px + (n - 1)*dst->width;
		for( ; px < bound_px; px += dst->width ) {
			*px = color;
		}
	}
	return 0;
}

/** 绘制左下角的圆角，从左边框的下端到下边框的左端绘制一条圆角线 */
static int Graph_DrawRoundBorderLeftBottom( LCUI_Graph *dst, LCUI_Pos center,
					    int radius, int width,
					    LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( y = 0; y <= radius; ++y, center_px += dst->width ) {
		if( radius + y >= bound.bottom || radius + y < bound.top ) {
			continue;
		}
		x = (int)sqrt( pow( radius, 2 ) - y*y );
		if( width > 0 && radius - x >= bound.left
		    && radius - x <= bound.right ) {
			px = center_px - x;
			*px = color;
		}
		px = center_px - center.x;
		if( radius - x > bound.right ) {
			n = bound.right - bound.left;
		} else {
			n = radius - x - bound.left;
		}
		n += (center.x - radius);
		bound_px = px + n;
		for( ; px < bound_px; ++px ) {
			px->alpha = 0;
		}
		n = radius - x + width;
		n = n > radius ? x : width;
		if( radius - x < bound.left ) {
			px = center_px - radius + bound.left - 1;
			n -= (bound.left - radius + x);
		} else {
			px = center_px - x;
		}
		if( radius - x + n > bound.right ) {
			n = bound.right - radius + x;
		}
		++px;
		bound_px = px + n - 1;
		for( ; px < bound_px; ++px ) {
			*px = color;
		}
	}
	return 0;
}

/** 绘制左下角的圆角，从下边框的左端到左边框的下端绘制一条圆角线 */
static int Graph_DrawRoundBorderBottomLeft( LCUI_Graph *dst, LCUI_Pos center,
					    int radius, int width,
					    LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( x = 0; x <= radius; ++x ) {
		if( radius - x > bound.right || radius - x < bound.left ) {
			continue;
		}
		y = (int)sqrt( pow( radius, 2 ) - x*x );
		if( radius + y > bound.bottom ) {
			px = center_px;
			px += (bound.bottom - radius)*dst->width;
		} else {
			px = center_px + y * dst->width - x;
		}
		if( width > 0 && radius + y >= bound.top &&
		    radius + y <= bound.bottom ) {
			*px = color;
		}
		if( radius + y < bound.top ) {
			n = bound.bottom - bound.top;
		} else {
			n = bound.bottom - radius - y;
		}
		/* 加上圆与背景图的下边距 */
		n += (rect.height - center.y - radius);
		bound_px = px + n * dst->width;
		for( px += dst->width; px < bound_px; px += dst->width ) {
			px->alpha = 0;
		}
		/* 计算需要向上填充的像素点的个数n */
		n = radius + y - width;
		/* 判断是否超过圆的x轴对称线 */
		n = n < radius ? y : width;
		if( radius + y > bound.bottom ) {
			px = center_px + (bound.bottom - radius)*dst->width;
			n -= (radius + y - bound.bottom);
		} else {
			px = center_px + y * dst->width - x;
		}
		/* 从上一行像素点开始 */
		px -= dst->width;
		if( radius + y - n < bound.top ) {
			n = bound.top - radius - y;
		}
		bound_px = px - (n - 1) * dst->width;
		/* 开始填充当前点下边的n-1个像素点 */
		for( ; px > bound_px; px -= dst->width ) {
			*px = color;
		}
	}
	return 0;
}

/** 绘制右下角的圆角，从右边框的下端到下边框的右端绘制一条圆角线 */
static int Graph_DrawRoundBorderRightBottom( LCUI_Graph *dst, LCUI_Pos center,
					     int radius, int width,
					     LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( y = 0; y <= radius; ++y, center_px += dst->width ) {
		if( radius + y >= bound.bottom || radius + y < bound.top ) {
			continue;
		}
		x = (int)sqrt( pow( radius, 2 ) - y*y );
		if( radius + x < bound.left ) {
			px = center_px + bound.left - radius;
		} else {
			px = center_px + x;
		}
		if( width > 0 && radius + x >= bound.left
		    && radius + x < bound.right ) {
			*px = color;
		}
		bound_px = center_px + bound.right - radius;
		for( ++px; px < bound_px; ++px ) {
			px->alpha = 0;
		}
		n = radius + x - width;
		n = n < bound.left ? x + radius - bound.left : width;
		if( radius + x > bound.right ) {
			px = center_px - radius + bound.right - 1;
			n -= (radius + x - bound.right);
		} else {
			px = center_px + x;
		}
		bound_px = px - n;
		for( ; px > bound_px; --px ) {
			*px = color;
		}
	}
	return 0;
}

/** 绘制右下角的圆角，从下边框的右端到右边框的下端绘制一条圆角线 */
static int Graph_DrawRoundBorderBottomRight( LCUI_Graph *dst, LCUI_Pos center,
					     int radius, int width,
					     LCUI_Color color )
{
	int x, y, n;
	LCUI_Rect rect;
	LCUI_Rect2 bound;
	LCUI_ARGB *px, *center_px, *bound_px;

	Graph_GetValidRect( dst, &rect );
	dst = Graph_GetQuote( dst );
	if( !Graph_IsValid( dst ) ) {
		return -1;
	}
	GetRoundBound( &bound, &rect, radius, center );
	center_px = dst->argb + center.x + rect.x;
	center_px += (rect.y + center.y) * dst->width;
	for( x = 0; x <= radius; ++x ) {
		if( radius + x >= bound.right || radius + x < bound.left ) {
			continue;
		}
		y = (int)sqrt( pow( radius, 2 ) - x*x );
		if( radius + y > bound.bottom ) {
			px = center_px;
			px += (bound.bottom - radius)*dst->width;
		} else {
			px = center_px + y * dst->width + x;
		}
		if( width > 0 && radius + y >= bound.top
		    && radius + y < bound.bottom ) {
			*px = color;
		}
		if( radius + y < bound.top ) {
			n = bound.bottom - bound.top;
		} else {
			n = bound.bottom - radius - y;
		}
		n += (rect.height - center.y - radius);
		bound_px = px + n * dst->width;
		for( px += dst->width; px < bound_px; px += dst->width ) {
			px->alpha = 0;
		}
		n = radius + y - width;
		n = n < radius ? y : width;
		if( radius + y > bound.bottom ) {
			px = center_px + (bound.bottom - radius)*dst->width;
			n -= (radius + y - bound.bottom);
		} else {
			px = center_px + y * dst->width + x;
		}
		if( radius + y - n < bound.top ) {
			n = bound.top - radius - y;
		}
		bound_px = px - n * dst->width;
		for( px -= dst->width; px > bound_px; px -= dst->width ) {
			*px = color;
		}
	}
	return 0;
}

int Border_Paint( const LCUI_Border *border,
		  const LCUI_Rect *box,
		  LCUI_PaintContext paint )
{
	LCUI_Pos pos;
	LCUI_Graph canvas;
	LCUI_Rect bound, rect;

	if( !Graph_IsValid( &paint->canvas ) ) {
		return -1;
	}
	/* 左上角的圆角 */
	bound.x = box->x;
	bound.y = box->y;
	bound.width = border->top_left_radius;
	bound.height = border->top_left_radius;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &rect ) ) {
		pos.x = border->top_left_radius;
		pos.y = border->top_left_radius;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &rect );
		Graph_DrawRoundBorderLeftTop( &canvas, pos,
					      border->top_left_radius,
					      border->left.width,
					      border->left.color );
		Graph_DrawRoundBorderTopLeft( &canvas, pos,
					      border->top_left_radius,
					      border->top.width,
					      border->top.color );
	}
	/* 右上角的圆角 */
	bound.y = box->y;
	bound.width = border->top_right_radius;
	bound.height = border->top_right_radius;
	bound.x = box->x + box->width - border->top_right_radius;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &rect ) ) {
		pos.x = 0;
		pos.y = border->top_right_radius;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &rect );
		Graph_DrawRoundBorderRightTop( &canvas, pos,
					       border->top_right_radius,
					       border->right.width,
					       border->right.color );
		Graph_DrawRoundBorderTopRight( &canvas, pos,
					       border->top_right_radius,
					       border->top.width,
					       border->top.color );
	}
	/* 左下角的圆角 */
	bound.x = box->x;
	bound.width = border->bottom_left_radius;
	bound.height = border->bottom_left_radius;
	bound.y = box->y + box->height - border->bottom_left_radius;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &rect ) ) {
		pos.y = 0;
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		pos.x = border->bottom_left_radius;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &rect );
		Graph_DrawRoundBorderLeftBottom( &canvas, pos,
						 border->bottom_left_radius,
						 border->left.width,
						 border->left.color );
		Graph_DrawRoundBorderBottomLeft( &canvas, pos,
						 border->bottom_left_radius,
						 border->bottom.width,
						 border->bottom.color );
	}
	/* 右下角的圆角 */
	bound.width = border->bottom_left_radius;
	bound.height = border->bottom_left_radius;
	bound.x = box->x + box->width - border->bottom_right_radius;
	bound.y = box->y + box->height - border->bottom_right_radius;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &rect ) ) {
		pos.x = pos.y = 0;
		pos.x -= rect.x - bound.x;
		pos.y -= rect.y - bound.y;
		rect.x -= paint->rect.x;
		rect.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &rect );
		Graph_DrawRoundBorderRightBottom( &canvas, pos,
						  border->bottom_right_radius,
						  border->right.width,
						  border->right.color );
		Graph_DrawRoundBorderBottomRight( &canvas, pos,
						  border->bottom_right_radius,
						  border->bottom.width,
						  border->bottom.color );
	}
	/* 绘制上边框线 */
	bound.x = box->x + border->top_left_radius;
	bound.y = box->y;
	bound.width = box->width - border->top_right_radius;
	bound.width -= border->top_left_radius;
	bound.height = border->top.width;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		Graph_FillRect( &canvas, border->top.color, NULL, TRUE );
	}
	/* 绘制下边框线 */
	bound.x = box->x + border->bottom_left_radius;
	bound.y = box->y + box->height - border->bottom.width;
	bound.width = box->width - border->bottom_right_radius;
	bound.width -= border->bottom_left_radius;
	bound.height = border->bottom.width;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		Graph_FillRect( &canvas, border->bottom.color, NULL, TRUE );
	}
	/* 绘制左边框线 */
	bound.y = box->y + border->top_left_radius;
	bound.x = box->x;
	bound.width = border->left.width;
	bound.height = box->height - border->top_left_radius;
	bound.height -= border->bottom_left_radius;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		Graph_FillRect( &canvas, border->left.color, NULL, TRUE );
	}
	/* 绘制右边框线 */
	bound.x = box->x + box->width - border->right.width;
	bound.y = box->y + border->top_right_radius;
	bound.width = border->right.width;
	bound.height = box->height - border->top_right_radius;
	bound.height -= border->bottom_right_radius;
	if( LCUIRect_GetOverlayRect( &bound, &paint->rect, &bound ) ) {
		bound.x -= paint->rect.x;
		bound.y -= paint->rect.y;
		Graph_Quote( &canvas, &paint->canvas, &bound );
		Graph_FillRect( &canvas, border->right.color, NULL, TRUE );
	}
	return 0;
}
