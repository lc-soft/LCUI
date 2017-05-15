/* ***************************************************************************
 * widget_shadow.c -- widget shadow style processing module.
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
 * widget_shadow.c -- 部件的阴影样式处理模块
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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/draw/boxshadow.h>

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

/** 计算部件阴影样式 */
static void Widget_ComputeBoxShadowStyle( LCUI_Widget w )
{
	int key;
	LCUI_Style s;
	LCUI_BoxShadowStyle *sd;
	sd = &w->computed_style.shadow;
	memset( sd, 0, sizeof( LCUI_BoxShadowStyle ) );
	for( key = key_box_shadow_start; key <= key_box_shadow_end; ++key ) {
		s = &w->style->sheet[key];
		if( !s->is_valid ) {
			continue;
		}
		switch( key ) {
		case key_box_shadow_x:
			sd->x = ComputeXMetric( w, s );
			break;
		case key_box_shadow_y:
			sd->y = ComputeYMetric( w, s );
			break;
		case key_box_shadow_spread:
			sd->spread = LCUIMetrics_Compute( s->value, s->type );
			break;
		case key_box_shadow_blur:
			sd->blur = LCUIMetrics_Compute( s->value, s->type );
			break;
		case key_box_shadow_color:
			sd->color = s->color;
			break;
		default: break;
		}
	}
}

void Widget_UpdateBoxShadow( LCUI_Widget w )
{
	int i;
	LCUI_Rect rects[4], rb, rg;
	LCUI_BoxShadowStyle *sd = &w->computed_style.shadow;
	LCUI_BoxShadowStyle old_sd = w->computed_style.shadow;
	Widget_ComputeBoxShadowStyle( w );
	/* 如果阴影变化并未导致图层尺寸变化，则只重绘阴影 */
	if( sd->x == old_sd.x && sd->y == old_sd.y &&
	    sd->blur == old_sd.blur ) {
		RectF2Rect( w->box.border, rb );
		RectF2Rect( w->box.graph, rg );
		LCUIRect_CutFourRect( &rb, &rg, rects );
		for( i = 0; i < 4; ++i ) {
			rects[i].x -= roundi( w->box.graph.x );
			rects[i].y -= roundi( w->box.graph.y );
			Widget_InvalidateArea( w, &rects[i], SV_GRAPH_BOX );
		}
		return;
	}
	Widget_AddTask( w, WTT_RESIZE );
	Widget_AddTask( w, WTT_POSITION );
}

float Widget_GetBoxShadowOffsetX( LCUI_Widget w )
{
	const LCUI_BoxShadowStyle *shadow;
	shadow = &w->computed_style.shadow;
	if( shadow->x >= SHADOW_WIDTH( shadow ) ) {
		return 0;
	}
	return SHADOW_WIDTH( shadow ) - shadow->x;
}

float Widget_GetBoxShadowOffsetY( LCUI_Widget w )
{
	const LCUI_BoxShadowStyle *shadow;
	shadow = &w->computed_style.shadow;
	if( shadow->y >= SHADOW_WIDTH( shadow ) ) {
		return 0;
	}
	return SHADOW_WIDTH( shadow ) - shadow->y;
}

float Widget_GetGraphWidth( LCUI_Widget widget )
{
	float width;
	const LCUI_BoxShadowStyle *shadow;
	width = widget->box.border.width;
	shadow = &widget->computed_style.shadow;
	if( shadow->x >= SHADOW_WIDTH( shadow ) ) {
		return width + SHADOW_WIDTH( shadow ) + shadow->x;
	} else if( shadow->x <= -SHADOW_WIDTH( shadow ) ) {
		return width + SHADOW_WIDTH( shadow ) - shadow->x;
	}
	return width + SHADOW_WIDTH( shadow ) * 2;
}

float Widget_GetGraphHeight( LCUI_Widget widget )
{
	float height;
	const LCUI_BoxShadowStyle *shadow;
	height = widget->box.border.height;
	shadow = &widget->computed_style.shadow;
	if( shadow->y >= SHADOW_WIDTH( shadow ) ) {
		return height + SHADOW_WIDTH( shadow ) + shadow->y;
	} else if( shadow->y <= -SHADOW_WIDTH( shadow ) ) {
		return height + SHADOW_WIDTH( shadow ) - shadow->y;
	}
	return height + SHADOW_WIDTH( shadow ) * 2;
}

void Widget_SetBoxShadow( LCUI_Widget w, float x, float y,
			  float blur, LCUI_Color color )
{
	Widget_SetStyle( w, key_box_shadow_x, x, px );
	Widget_SetStyle( w, key_box_shadow_y, y, px );
	Widget_SetStyle( w, key_box_shadow_blur, blur, px );
	Widget_SetStyle( w, key_box_shadow_color, color, color );
	Widget_UpdateStyle( w, FALSE );
}

/** 计算部件阴影样式的实际值 */
static void Widget_ComputeBoxShadow( LCUI_Widget w, LCUI_BoxShadow *out )
{
	LCUI_BoxShadowStyle *s;
	s = &w->computed_style.shadow;
	out->x = LCUIMetrics_ComputeActual( s->x, SVT_PX );
	out->y = LCUIMetrics_ComputeActual( s->y, SVT_PX );
	out->blur = LCUIMetrics_ComputeActual( s->blur, SVT_PX );
	out->spread = LCUIMetrics_ComputeActual( s->spread, SVT_PX );
	out->color = s->color;
}

void Widget_PaintBoxShadow( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_Rect box;
	LCUI_BoxShadow shadow;
	box.x = box.y = 0;
	Widget_ComputeBoxShadow( w, &shadow );
	box.width = LCUIMetrics_ComputeActual( w->box.graph.width, SVT_PX );
	box.height = LCUIMetrics_ComputeActual( w->box.graph.height, SVT_PX );
	Graph_DrawBoxShadow( paint, &box, &shadow );	
}
