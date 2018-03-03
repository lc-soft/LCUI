/*
 * widget_boarder.c -- widget border style processing module.
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

#include <string.h>
#include <math.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>

static float ComputeXMetric( LCUI_Widget w, LCUI_Style s )
{
	if( s->type == SVT_SCALE ) {
		return w->width * s->scale;
	}
	return LCUIMetrics_Compute( s->value, s->type );
}

static float ComputeYMetric( LCUI_Widget w, LCUI_Style s )
{
	if( s->type == SVT_SCALE ) {
		return w->height * s->scale;
	}
	return LCUIMetrics_Compute( s->value, s->type );
}

void Widget_ComputeBorderStyle( LCUI_Widget w )
{
	int key;
	LCUI_Style s;
	LCUI_BorderStyle *b;
	b = &w->computed_style.border;
	memset( b, 0, sizeof( LCUI_BorderStyle ) );
	for( key = key_border_start; key < key_border_end; ++key ) {
		s = &w->style->sheet[key];
		if( !s->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_border_top_color:
			b->top.color = s->color;
			break;
		case key_border_right_color:
			b->right.color = s->color;
			break;
		case key_border_bottom_color:
			b->bottom.color = s->color;
			break;
		case key_border_left_color:
			b->left.color = s->color;
			break;
		case key_border_top_width:
			b->top.width = ComputeXMetric( w, s );
			break;
		case key_border_right_width:
			b->right.width = ComputeYMetric( w, s );
			break;
		case key_border_bottom_width:
			b->bottom.width = ComputeXMetric( w, s );
			break;
		case key_border_left_width:
			b->left.width = ComputeYMetric( w, s );
			break;
		case key_border_top_style:
			b->top.style = s->val_style;
			break;
		case key_border_right_style:
			b->right.style = s->val_style;
			break;
		case key_border_bottom_style:
			b->bottom.style = s->val_style;
			break;
		case key_border_left_style:
			b->left.style = s->val_style;
			break;
		case key_border_top_left_radius:
			b->top_left_radius = ComputeXMetric( w, s );
			break;
		case key_border_top_right_radius:
			b->top_right_radius = ComputeXMetric( w, s );
			break;
		case key_border_bottom_left_radius:
			b->bottom_left_radius = ComputeXMetric( w, s );
			break;
		case key_border_bottom_right_radius:
			b->bottom_right_radius = ComputeXMetric( w, s );
			break;
		default: break;
		}
	}
}

static unsigned int ComputeActual( float width )
{
	unsigned int w;
	w = LCUIMetrics_ComputeActual( width, SVT_PX );
	if( width > 0 && w < 1 ) {
		return 1;
	}
	return w;
}

void Widget_UpdateBorder( LCUI_Widget w )
{
	LCUI_BorderStyle ob, *nb;
	ob = w->computed_style.border;
	Widget_ComputeBorderStyle( w );
	nb = &w->computed_style.border;
	/* 如果边框变化并未导致图层尺寸变化的话，则只重绘边框 */
	if( ob.top.width != nb->top.width || 
	    ob.right.width != nb->right.width ||
	    ob.bottom.width != nb->bottom.width ||
	    ob.left.width != nb->left.width ) {
		Widget_AddTask( w, LCUI_WTASK_RESIZE );
		Widget_AddTask( w, LCUI_WTASK_POSITION );
		return;
	}
	Widget_InvalidateArea( w, NULL, SV_BORDER_BOX );
}

/** 计算部件边框样式的实际值 */
void Widget_ComputeBorder( LCUI_Widget w, LCUI_Border *out )
{
	LCUI_BorderStyle *s;
	s = &w->computed_style.border;
	out->top.color = s->top.color;
	out->left.color = s->left.color;
	out->right.color = s->right.color;
	out->bottom.color = s->bottom.color;
	out->top.style = s->top.style;
	out->left.style = s->left.style;
	out->right.style = s->right.style;
	out->bottom.style = s->bottom.style;
	out->top.width = ComputeActual( s->top.width );
	out->left.width = ComputeActual( s->left.width );
	out->right.width = ComputeActual( s->right.width );
	out->bottom.width = ComputeActual( s->bottom.width );
	out->top_left_radius = ComputeActual( s->bottom_left_radius );
	out->top_right_radius = ComputeActual( s->bottom_right_radius );
	out->bottom_left_radius = ComputeActual( s->bottom_left_radius );
	out->bottom_right_radius = ComputeActual( s->bottom_right_radius );
}

void Widget_PaintBorder( LCUI_Widget w, LCUI_PaintContext paint,
			 LCUI_WidgetActualStyle style )
{
	LCUI_Rect box;
	box.x = style->border_box.x - style->canvas_box.x;
	box.y = style->border_box.y - style->canvas_box.y;
	box.width = style->border_box.width;
	box.height = style->border_box.height;
	Border_Paint( &style->border, &box, paint );
}
