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
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/scrollbar.h>

typedef struct LCUI_ScrollBarRec_ {
	LCUI_Widget box;	/**< 容器 */
	LCUI_Widget layer;	/**< 滚动层 */
	LCUI_Widget slider;	/**< 滑块 */
	LCUI_BOOL is_dragging;	/**< 是否处于拖拽状态 */
	int slider_x, slider_y;	/**< 拖拽开始时的滑块位置 */
	int mouse_x, mouse_y;	/**< 拖拽开始时的鼠标坐标 */
	int direction;		/**< 滚动条的方向（垂直或水平） */
	int eid_mousemove;
	int eid_mouseup;
	int eid_touch;
	int scroll_step;	/**< 每次滚动的距离，主要针对使用鼠标滚轮触发的滚动 */
	int pos;		/**< 当前的位置 */
	int old_pos;		/**< 拖拽开始时的位置 */
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

static void OnMouseUp( LCUI_SysEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	if( scrollbar->eid_mousemove > 0 ) {
		LCUI_UnbindEvent( scrollbar->eid_mousemove );
	}
	if( scrollbar->eid_mouseup > 0 ) {
		LCUI_UnbindEvent( scrollbar->eid_mouseup );
	}
	if( scrollbar->eid_touch > 0 ) {
		LCUI_UnbindEvent( scrollbar->eid_touch );
	}
	scrollbar->eid_mousemove = -1;
	scrollbar->eid_mouseup = -1;
	scrollbar->eid_touch = -1;
	scrollbar->is_dragging = FALSE;
}

static void OnMouseMove( LCUI_SysEvent e, void *arg )
{
	float n;
	LCUI_Pos pos;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	LCUI_Widget slider = scrollbar->slider;
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
	Widget_Move( scrollbar->slider, x, y );
}

static void OnTouch( LCUI_SysEvent e, void *arg )
{
	int pos;
	LCUI_TouchPoint point;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	if( e->touch.n_points < 1 ) {
		return;
	}
	point = &e->touch.points[0];
	if( point->state == LCUI_TOUCHMOVE ) {
		pos = scrollbar->old_pos - (point->y - scrollbar->mouse_y);
		ScrollBar_SetPosition( w, pos );
	}
}

static void OnMouseDown( LCUI_Widget slider, LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = slider->parent;
	LCUI_ScrollBar scrollbar = w->private_data;
	if( scrollbar->is_dragging ) {
		return;
	}
	scrollbar->slider_x = slider->x;
	scrollbar->slider_y = slider->y;
	scrollbar->mouse_x = e->screen_x;
	scrollbar->mouse_y = e->screen_y;
	scrollbar->is_dragging = TRUE;
	scrollbar->eid_mousemove = LCUI_BindEvent( LCUI_MOUSEMOVE, 
						   OnMouseMove, w, NULL );
	scrollbar->eid_mouseup = LCUI_BindEvent( LCUI_MOUSEUP, 
						 OnMouseUp, w, NULL );
}

static void ScrollBar_OnInit( LCUI_Widget w )
{
	LCUI_Widget slider;
	LCUI_ScrollBar self;
	slider = LCUIWidget_New( NULL );
	self = Widget_NewPrivateData( w, LCUI_ScrollBarRec );
	Widget_BindEvent( slider, "mousedown", OnMouseDown, NULL, NULL );
	Widget_AddClass( slider, "slider" );
	Widget_Append( w, slider );
	self->direction = SBD_VERTICAL;
	self->is_dragging = FALSE;
	self->scroll_step = 40;
	self->eid_mousemove = -1;
	self->eid_mouseup = -1;
	self->eid_touch = -1;
	self->slider = slider;
	self->layer = NULL;
	self->box = NULL;
	self->old_pos = 0;
	self->pos = 0;
}

static void ScrollBar_UpdateSize( LCUI_Widget w )
{
	float n = 1.0;
	int size, box_size;
	LCUI_ScrollBar scrollbar = w->private_data;
	LCUI_Widget slider = scrollbar->slider;
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

static void ScrollLayer_OnMouseDown( LCUI_Widget layer, LCUI_WidgetEvent e, void *arg )
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = w->private_data;
	if( scrollbar->is_dragging ) {
		return;
	}
	scrollbar->eid_touch = LCUI_BindEvent( LCUI_TOUCH, OnTouch, w, NULL );
	scrollbar->eid_mouseup = LCUI_BindEvent( LCUI_MOUSEUP, OnMouseUp, w, NULL );
	scrollbar->mouse_x = e->screen_x;
	scrollbar->mouse_y = e->screen_y;
	scrollbar->old_pos = scrollbar->pos;
	scrollbar->is_dragging = TRUE;
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
	Widget_BindEvent( layer, "mousedown", ScrollLayer_OnMouseDown, w, NULL );
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
