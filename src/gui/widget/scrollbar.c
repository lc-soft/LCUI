/* ***************************************************************************
* scrollbar.c -- LCUI's scrollbar widget
*
* Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
* scrollbar.c -- LCUI 的滚动条部件
*
* 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/cursor.h>
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/scrollbar.h>

#define EFFECT_FRAMES 50

/** 惯性滚动效果的相关数据 */
typedef struct InertialScrollingRec_ {
	int start_pos;			/**< 起始位置 */
	int end_pos;			/**< 结束位置 */
	int timer;			/**< 定时器 */
	int interval;			/**< 定时器的间隔时间 */
	int speed;			/**< 滚动速度 */
	int speed_delta;		/**< 速度差（加速度） */
	int64_t timestamp;		/**< 开始时间 */
	LCUI_BOOL is_running;		/**< 当前效果是否正在运行 */
} InertialScrollingRec, *InertialScrolling;

/** 滚动条的相关数据 */
typedef struct LCUI_ScrollBarRec_ {
	LCUI_Widget box;		/**< 容器 */
	LCUI_Widget layer;		/**< 滚动层 */
	LCUI_Widget slider;		/**< 滑块 */
	LCUI_BOOL is_dragging;		/**< 是否处于拖拽状态 */
	int slider_x, slider_y;		/**< 拖拽开始时的滑块位置 */
	int mouse_x, mouse_y;		/**< 拖拽开始时的鼠标坐标 */
	int direction;			/**< 滚动条的方向（垂直或水平） */
	int scroll_step;		/**< 每次滚动的距离，主要针对使用鼠标滚轮触发的滚动 */
	int pos;			/**< 当前的位置 */
	int old_pos;			/**< 拖拽开始时的位置 */
	int distance;			/**< 滚动距离 */
	InertialScrollingRec effect;	/**< 用于实现惯性滚动效果的相关数据 */
} LCUI_ScrollBarRec, *LCUI_ScrollBar;

static int scroll_event_id = -1;

static const char *scrollbar_css = ToString(

scrollbar {
top: 0;
right: 0;
width: 14px;
height: 100%;
position: absolute;
background-color: #fafafa;
border: 1px solid #eee;
}
scrollbar .slider {
top: 0;
left: 0;
width: 14px;
height: 14px;
min-width: 14px;
min-height: 14px;
position: absolute;
background-color: #888;
}
.scrolllayer {
position: relative;
top: 0;
left: 0;
}
.scrollbar-horizontal {
width: 100%;
height: 14px;
bottom: 0;
left: 0;
right: auto;
top: auto;
}

);

static void OnInertialScrolling( void *arg )
{
	int pos;
	LCUI_Widget w = arg;
	LCUI_ScrollBar scrollbar;
	InertialScrolling effect;
	scrollbar = w->private_data;
	effect = &scrollbar->effect;
	pos = scrollbar->pos + effect->speed;
	effect->speed += effect->speed_delta;
	ScrollBar_SetPosition( w, pos );
	if( effect->speed == 0 || !effect->is_running ||
	    (effect->speed_delta > 0) == (effect->speed > 0) ) {
		effect->speed = 0;
		effect->timer = -1;
		effect->is_running = FALSE;
		return;
	}
	effect->timer = LCUITimer_Set( effect->interval, 
				       OnInertialScrolling, w, FALSE );
}

static void ScrollBar_UpdateInertialScrolling( LCUI_Widget w )
{
	LCUI_ScrollBar scrollbar;
	scrollbar = w->private_data;
	scrollbar->effect.speed = 0;
	scrollbar->effect.is_running = FALSE;
	scrollbar->effect.start_pos = scrollbar->pos;
	scrollbar->effect.timestamp = LCUI_GetTickCount();
}

static void ScrollBar_StartInertialScrolling( LCUI_Widget w )
{
	int distance;
	int64_t time_delta;
	LCUI_ScrollBar scrollbar;
	InertialScrolling effect;
	scrollbar = w->private_data;
	effect = &scrollbar->effect;
	effect->end_pos = scrollbar->pos;
	distance = effect->end_pos - effect->start_pos;
	time_delta = LCUI_GetTicks( effect->timestamp );
	/* 计算每一帧的滚动距离（速度） */
	if( time_delta > 0 ) {
		effect->speed = distance * effect->interval / (int)time_delta;
	} else {
		effect->speed = 0;
	}
	/* 计算加速度，该值的正负符号始终与速度值的相反 */
	if( (effect->speed > 0) == (effect->speed_delta > 0) ) {
		effect->speed_delta = -effect->speed_delta;
	}
	effect->timestamp = LCUI_GetTickCount();
	if( effect->is_running ) {
		return;
	}
	effect->is_running = TRUE;
	effect->timer = LCUITimer_Set( effect->interval, 
				       OnInertialScrolling, w, FALSE );
	DEBUG_MSG("start_pos: %d, end_pos: %d\n", effect->start_pos, effect->end_pos);
	DEBUG_MSG("effect->speed: %d, distance: %d, time: %d\n", 
		   effect->speed, distance, (int)time_delta);
}

static void Slider_OnMouseMove( LCUI_Widget slider, 
				LCUI_WidgetEvent e, void *arg )
{
	float n;
	LCUI_Pos pos;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	LCUI_Widget layer = scrollbar->layer;
	int x, y, box_size, size, layer_pos;

	if( !scrollbar->is_dragging || !scrollbar->layer ) {
		return;
	}
	LCUICursor_GetPos( &pos );
	if( scrollbar->direction == SBD_HORIZONTAL ) {
		y = 0;
		x = scrollbar->slider_x;
		x += pos.x - scrollbar->mouse_x;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.width;
		} else {
			box_size = w->parent->box.content.width;
		}
		layer_pos = scrollbar->layer->box.outer.width - box_size;
		size = w->box.content.width - slider->width;
		if( x > size ) {
			x = size;
		} else if( x < 0 ) {
			x = 0;
		}
		n = 0.0;
		if( size > 0 ) {
			n = 1.0 * slider->x / size;
			if( n > 1.0 ) {
				n = 1;
			}
		}
		layer_pos = layer_pos * n;
		SetStyle( layer->custom_style, key_left, -layer_pos, px );
	} else {
		x = 0;
		y = scrollbar->slider_y;
		y += pos.y - scrollbar->mouse_y;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.height;
		} else {
			box_size = w->parent->box.content.height;
		}
		layer_pos = scrollbar->layer->box.outer.height - box_size;
		size = w->box.content.height - slider->height;
		if( y > size ) {
			y = size;
		} else if( y < 0 ) {
			y = 0;
		}
		n = 0.0;
		if( size > 0 ) {
			n = 1.0 * slider->y / size;
			if( n > 1.0 ) {
				n = 1;
			}
		}
		layer_pos = layer_pos * n;
		SetStyle( layer->custom_style, key_top, -layer_pos, px );
	}
	if( scrollbar->pos != layer_pos ) {
		LCUI_WidgetEventRec e;
		e.type = scroll_event_id;
		Widget_TriggerEvent( layer, &e, &layer_pos );
	}
	scrollbar->pos = layer_pos;
	Widget_UpdateStyle( layer, FALSE );
	Widget_Move( slider, x, y );
}

static void Slider_OnMouseUp( LCUI_Widget slider, 
			      LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	Widget_UnbindEvent( slider, "mousemove", Slider_OnMouseMove );
	Widget_UnbindEvent( slider, "mouseup", Slider_OnMouseUp );
	Widget_ReleaseMouseCapture( slider );
	scrollbar->is_dragging = FALSE;
}

static void Slider_OnMouseDown( LCUI_Widget slider, 
				LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	if( scrollbar->is_dragging ) {
		return;
	}
	scrollbar->slider_x = slider->x;
	scrollbar->slider_y = slider->y;
	scrollbar->mouse_x = e->screen_x;
	scrollbar->mouse_y = e->screen_y;
	scrollbar->is_dragging = TRUE;
	Widget_SetMouseCapture( slider );
	Widget_BindEvent( slider, "mousemove", Slider_OnMouseMove, w, NULL );
	Widget_BindEvent( slider, "mouseup", Slider_OnMouseUp, w, NULL );
}

static void ScrollBar_OnInit( LCUI_Widget w )
{
	LCUI_Widget slider;
	LCUI_ScrollBar self;
	slider = LCUIWidget_New( NULL );
	self = Widget_NewPrivateData( w, LCUI_ScrollBarRec );
	self->direction = SBD_VERTICAL;
	self->is_dragging = FALSE;
	self->scroll_step = 40;
	self->slider = slider;
	self->layer = NULL;
	self->box = NULL;
	self->old_pos = 0;
	self->pos = 0;
	self->effect.timer = -1;
	self->effect.end_pos = 0;
	self->effect.start_pos = 0;
	self->effect.timestamp = 0;
	self->effect.speed = 0;
	self->effect.speed_delta = 1;
	self->effect.is_running = FALSE;
	self->effect.interval = 1000 / EFFECT_FRAMES;
	Widget_BindEvent( slider, "mousedown", 
			  Slider_OnMouseDown, w, NULL );
	Widget_AddClass( slider, "slider" );
	Widget_Append( w, slider );
}

static void ScrollBar_UpdateSize( LCUI_Widget w )
{
	float n = 1.0;
	int size, box_size;
	LCUI_ScrollBar scrollbar = w->private_data;
	LCUI_Widget slider = scrollbar->slider;
	if( !scrollbar->box ) {
		return;
	}
	if( scrollbar->direction == SBD_HORIZONTAL ) {
		if( scrollbar->layer ) {
			size = scrollbar->layer->box.outer.width;
		} else {
			size = 0;
		}
		box_size = scrollbar->box->width;
		if( size > box_size && box_size > 0 ) {
			n = 1.0 * box_size / size;
		}
		SetStyle( slider->custom_style, key_width, n, scale );
	} else {
		if( scrollbar->layer ) {
			size = scrollbar->layer->box.outer.height;
		} else {
			size = 0;
		}
		box_size = scrollbar->box->height;
		if( size > box_size && box_size > 0 ) {
			n = 1.0 * box_size / size;
		}
		SetStyle( slider->custom_style, key_height, n, scale );
	}
	ScrollBar_SetPosition( w, scrollbar->pos );
	Widget_UpdateStyle( slider, FALSE );
	if( n < 1.0 ) {
		Widget_Show( w );
	} else {
		Widget_Hide( w );
	}
}

static void ScrollLayer_OnWheel( LCUI_Widget layer, LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	int pos = ScrollBar_GetPosition( w );
	if( e->z_delta > 0 ) {
		pos -= scrollbar->scroll_step;
	} else {
		pos += scrollbar->scroll_step;
	}
	ScrollBar_SetPosition( w, pos );
}

/** 滚动层的触屏事件响应 */
static void ScrollLayer_OnTouch( LCUI_Widget layer, LCUI_WidgetEvent e, void *arg )
{
	int i, pos, distance;
	LCUI_TouchPoint point;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar;
	if( e->n_points < 1 ) {
		return;
	}
	scrollbar = w->private_data;
	for( point = NULL, i = 0; i < e->n_points; ++i ) {
		point = &e->points[i];
		if( point->is_primary ) {
			break;
		}
	}
	if( !point ) {
		return;
	}
	switch( point->state ) {
	case WET_TOUCHDOWN:
		scrollbar->distance = 0;
		scrollbar->effect.speed = 0;
		scrollbar->effect.is_running = FALSE;
		scrollbar->old_pos = scrollbar->pos;
		if( scrollbar->is_dragging ) {
			return;
		}
		Widget_SetTouchCapture( layer, point->id );
		scrollbar->mouse_x = point->x;
		scrollbar->mouse_y = point->y;
		break;
	case WET_TOUCHUP:
		Widget_ReleaseTouchCapture( layer, -1 );
		if( scrollbar->is_dragging ) {
			ScrollBar_StartInertialScrolling( w );
		}
		scrollbar->is_dragging = FALSE;
		Widget_BlockEvent( layer, FALSE );
		break;
	case WET_TOUCHMOVE:
		pos = scrollbar->old_pos;
		if( scrollbar->direction == SBD_HORIZONTAL ) {
			pos -= point->x - scrollbar->mouse_x;
		} else {
			pos -= point->y - scrollbar->mouse_y;
		}
		if( pos != scrollbar->pos ) {
			if( !scrollbar->is_dragging ) {
				scrollbar->is_dragging = TRUE;
				LCUIWidget_ClearEventTarget( NULL );
				Widget_BlockEvent( layer, TRUE );
			}
			distance = pos - scrollbar->pos;
			if( (scrollbar->distance > 0) != (distance > 0) ||
			    scrollbar->distance == 0 ) {
				ScrollBar_UpdateInertialScrolling( w );
			}
			scrollbar->distance = distance;
		}
		ScrollBar_SetPosition( w, pos );
	default: break;
	}
}

static void ScrollBar_OnUpdateSize( LCUI_Widget box, LCUI_WidgetEvent e, void *arg )
{
	ScrollBar_UpdateSize( e->data );
}

void ScrollBar_BindBox( LCUI_Widget w, LCUI_Widget box )
{
	LCUI_ScrollBar scrollbar = w->private_data;
	if( scrollbar->box ) {
		Widget_UnbindEvent( scrollbar->box, "resize",
				    ScrollBar_OnUpdateSize );
	}
	scrollbar->box = box;
	Widget_BindEvent( box, "resize", ScrollBar_OnUpdateSize, w, NULL );
	ScrollBar_UpdateSize( w );
}

void ScrollBar_BindLayer( LCUI_Widget w, LCUI_Widget layer )
{
	LCUI_ScrollBar scrollbar = w->private_data;
	if( scrollbar->layer ) {
		Widget_UnbindEvent( scrollbar->layer, "resize",
				    ScrollBar_OnUpdateSize );
	}
	scrollbar->layer = layer;
	Widget_BindEvent( layer, "resize", ScrollBar_OnUpdateSize, w, NULL );
	Widget_BindEvent( layer, "mousewheel", ScrollLayer_OnWheel, w, NULL );
	Widget_BindEvent( layer, "touch", ScrollLayer_OnTouch, w, NULL );
	ScrollBar_UpdateSize( w );
}

int ScrollBar_GetPosition( LCUI_Widget w )
{
	LCUI_ScrollBar scrollbar = w->private_data;
	return scrollbar->pos;
}

void ScrollBar_SetPosition( LCUI_Widget w, int pos )
{
	int box_size, size, slider_pos;
	LCUI_ScrollBar scrollbar = w->private_data;
	LCUI_Widget slider = scrollbar->slider;
	LCUI_Widget layer = scrollbar->layer;
	LCUI_WidgetEvent e;

	if( !layer ) {
		return;
	}
	memset( &e, 0, sizeof( e ) );
	if( scrollbar->direction == SBD_HORIZONTAL ) {
		size = scrollbar->layer->box.outer.width;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.width;
		} else {
			box_size = w->parent->box.content.width;
		}
		if( pos + box_size > size ) {
			pos = size - box_size;
		}
		if( pos < 0 ) {
			pos = 0;
		}
		slider_pos = w->box.content.width - slider->width;
		slider_pos = slider_pos * pos / (size - box_size);
		SetStyle( slider->custom_style, key_left, slider_pos, px );
		SetStyle( layer->custom_style, key_left, -pos, px );
	} else {
		size = scrollbar->layer->box.outer.height;
		if( scrollbar->box ) {
			box_size = scrollbar->box->box.content.height;
		} else {
			box_size = w->parent->box.content.height;
		}
		if( pos + box_size > size ) {
			pos = size - box_size;
		}
		if( pos < 0 ) {
			pos = 0;
		}
		slider_pos = w->box.content.height - slider->height;
		if( size == box_size ) {
			slider_pos = 0;
		} else {
			slider_pos = slider_pos * pos / (size - box_size);
		}
		SetStyle( slider->custom_style, key_top, slider_pos, px );
		SetStyle( layer->custom_style, key_top, -pos, px );
	}
	if( scrollbar->pos != pos ) {
		LCUI_WidgetEventRec e;
		e.type = scroll_event_id;
		Widget_TriggerEvent( layer, &e, &pos );
	}
	scrollbar->pos = pos;
	Widget_UpdateStyle( slider, FALSE );
	Widget_UpdateStyle( layer, FALSE );
}

void ScrollBar_SetDirection( LCUI_Widget w, int direction )
{
	LCUI_ScrollBar scrollbar = w->private_data;
	if( direction == SBD_HORIZONTAL ) {
		Widget_RemoveClass( w, "scrollbar-Horizontal" );
	} else {
		Widget_AddClass( w, "scrollbar-Horizontal" );
	}
	scrollbar->direction = direction;
}

static void ScrollBar_OnSetAttr( LCUI_Widget w, const char *name, const char *value )
{
	LCUI_Widget target;
	if( strcmp( name, "data-parent" ) == 0 ) {
		target = LCUIWidget_GetById( value );
		if( target ) {
			ScrollBar_BindBox( w, target );
		}
	} else if( strcmp( name, "data-layer" ) == 0 ) {
		target = LCUIWidget_GetById( value );
		if( target ) {
			ScrollBar_BindLayer( w, target );
		}
	}
}

void LCUIWidget_AddTScrollBar( void )
{
	LCUI_WidgetClass *wc = LCUIWidget_AddClass( "scrollbar" );
	wc->methods.init = ScrollBar_OnInit;
	wc->methods.set_attr = ScrollBar_OnSetAttr;
	scroll_event_id = LCUIWidget_AllocEventId();
	LCUIWidget_SetEventName( scroll_event_id, "scroll" );
	LCUICSS_LoadString( scrollbar_css );
}
