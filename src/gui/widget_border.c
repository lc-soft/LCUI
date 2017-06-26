/* ***************************************************************************
 * widget_boarder.c -- widget border style processing module.
 * 
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_boarder.c -- 部件的边框样式处理模块
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

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
			b->top.style = s->value;
			break;
		case key_border_right_style:
			b->right.style = s->value;
			break;
		case key_border_bottom_style:
			b->bottom.style = s->value;
			break;
		case key_border_left_style:
			b->left.style = s->value;
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

void Widget_SetBorder( LCUI_Widget w, float width, int style, LCUI_Color clr )
{
	Widget_SetStyle( w, key_border_top_color, clr, color );
	Widget_SetStyle( w, key_border_right_color, clr, color );
	Widget_SetStyle( w, key_border_bottom_color, clr, color );
	Widget_SetStyle( w, key_border_left_color, clr, color );
	Widget_SetStyle( w, key_border_top_width, width, px );
	Widget_SetStyle( w, key_border_right_width, width, px );
	Widget_SetStyle( w, key_border_bottom_width, width, px );
	Widget_SetStyle( w, key_border_left_width, width, px );
	Widget_SetStyle( w, key_border_top_style, style, style );
	Widget_SetStyle( w, key_border_right_style, style, style );
	Widget_SetStyle( w, key_border_bottom_style, style, style );
	Widget_SetStyle( w, key_border_left_style, style, style );
	Widget_UpdateStyle( w, FALSE );
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
		Widget_AddTask( w, WTT_RESIZE );
		Widget_AddTask( w, WTT_POSITION );
		return;
	}
	Widget_InvalidateArea( w, NULL, SV_BORDER_BOX );
}

/** 计算部件边框样式的实际值 */
static void Widget_ComputeBorder( LCUI_Widget w, LCUI_Border *out )
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

void Widget_PaintBorder( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_Rect box;
	LCUI_RectF fbox;
	LCUI_Border border;
	Widget_ComputeBorder( w, &border );
	fbox.x = w->box.border.x - w->box.graph.x;
	fbox.y = w->box.border.y - w->box.graph.y;
	fbox.width = w->box.border.width;
	fbox.height = w->box.border.height;
	box.x = LCUIMetrics_ComputeActual( fbox.x, SVT_PX );
	box.y = LCUIMetrics_ComputeActual( fbox.y, SVT_PX );
	box.width = LCUIMetrics_ComputeActual( fbox.width, SVT_PX );
	box.height = LCUIMetrics_ComputeActual( fbox.height, SVT_PX );
	Graph_DrawBorder( paint, &box, &border );
}
