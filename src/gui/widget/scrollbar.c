/*
 * scrollbar.c -- LCUI's scrollbar widget
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
#include <LCUI/gui/css_parser.h>

/* clang-format off */

#define EFFECT_FRAMES 80

/** 惯性滚动效果的相关数据 */
typedef struct InertialScrollingRec_ {
	int start_pos;			/**< 开始移动时的位置 */
	int end_pos;			/**< 结束移动时的位置 */
	int timer;			/**< 定时器 */
	int interval;			/**< 定时器的间隔时间 */
	double speed;			/**< 滚动速度 */
	double speed_delta;		/**< 速度差（加速度） */
	int64_t timestamp;		/**< 开始时间 */
	LCUI_BOOL is_running;		/**< 当前效果是否正在运行 */
} InertialScrollingRec, *InertialScrolling;

/** 滚动条的相关数据 */
typedef struct LCUI_ScrollBarRec_ {
	LCUI_Widget box;		/**< a container containing scrollbar and target, the default is the parent of scrollbar */
	LCUI_Widget target;		/**< scroll target */
	LCUI_Widget slider;		/**< slider of scrollbar */
	LCUI_BOOL is_dragged;		/**< whether the target is dragged */
	LCUI_BOOL is_draggable;		/**< whether the target can be dragged */
	float slider_x, slider_y;	/**< 拖拽开始时的滑块位置 */
	int mouse_x, mouse_y;		/**< 拖拽开始时的鼠标坐标 */
	int touch_point_id;		/**< 触点的ID */
	int direction;			/**< 滚动条的方向（垂直或水平） */
	int scroll_step;		/**< 每次滚动的距离，主要针对使用鼠标滚轮触发的滚动 */
	int pos;			/**< 当前的位置 */
	int old_pos;			/**< 拖拽开始时的位置 */
	int distance;			/**< 滚动距离 */
	int64_t timestamp;		/**< 数据更新时间，主要针对触控拖动时的位置变化 */
	InertialScrollingRec effect;	/**< 用于实现惯性滚动效果的相关数据 */
} LCUI_ScrollBarRec, *LCUI_ScrollBar;

static struct LCUI_ScrollbarModule {
	int event_id;
	LCUI_WidgetPrototype prototype;
} self;

static const char *scrollbar_css = CodeToString(

scrollbar {
	top: 0;
	right: 0;
	width: 14px;
	height: 100%;
	position: absolute;
	background-color: #fafafa;
	border: 1px solid #eee;
}
.scrollbar-slider {
	top: 0;
	left: 0;
	width: 14px;
	height: 14px;
	min-width: 14px;
	min-height: 14px;
	position: absolute;
	background-color: #aaa;
}
.scrollbar-slider:hover {
	background-color: #bbb;
}
.scrollbar-slider:active {
	background-color: #999;
}
.scrollbar-target {
	top: 0;
	left: 0;
	position: relative;
}
.scrollbar-horizontal {
	right: auto;
	top: auto;
	bottom: 0;
	left: 0;
	width: 100%;
	height: 14px;
}

);

/* clang-format on */

static void OnInertialScrolling(void *arg)
{
	int pos;
	double distance, time;
	LCUI_ScrollBar scrollbar;
	InertialScrolling effect;
	LCUI_Widget w = arg;

	scrollbar = Widget_GetData(w, self.prototype);
	effect = &scrollbar->effect;
	time = (double)LCUI_GetTimeDelta(effect->timestamp) / 1000;
	distance = (effect->speed + 0.5 * effect->speed_delta * time) * time;
	pos = effect->end_pos + iround(distance);
	DEBUG_MSG("distance: %g, pos: %d, speed_delta: %g, speed: %g\n",
		  distance, pos, effect->speed_delta,
		  effect->speed + effect->speed_delta * time);
	while (effect->is_running) {
		double speed = effect->speed + effect->speed_delta * time;
		if ((effect->speed > 0 && speed <= 0) ||
		    (effect->speed < 0 && speed >= 0)) {
			break;
		}
		ScrollBar_SetPosition(w, pos);
		return;
	}
	LCUITimer_Free(effect->timer);
	effect->is_running = FALSE;
	effect->timer = -1;
}

static void InitInertialScrolling(InertialScrolling effect)
{
	effect->timer = -1;
	effect->end_pos = 0;
	effect->start_pos = 0;
	effect->timestamp = 0;
	effect->speed = 0;
	effect->speed_delta = 320;
	effect->is_running = FALSE;
	effect->interval = 1000 / EFFECT_FRAMES;
}

static void UpdateInertialScrolling(InertialScrolling effect, int pos)
{
	effect->speed = 0;
	effect->is_running = FALSE;
	effect->start_pos = pos;
	effect->timestamp = LCUI_GetTime();
}

static void StartInertialScrolling(LCUI_Widget w)
{
	int distance;
	int64_t time_delta;

	LCUI_ScrollBar scrollbar;
	InertialScrolling effect;

	scrollbar = Widget_GetData(w, self.prototype);
	effect = &scrollbar->effect;
	effect->end_pos = scrollbar->pos;
	distance = effect->end_pos - effect->start_pos;
	time_delta = LCUI_GetTimeDelta(effect->timestamp);
	/* 根据距离计算当前移动速度 */
	if (time_delta > 0) {
		effect->speed = 1000.0 * distance / time_delta;
	} else {
		effect->speed = 0;
		return;
	}
	effect->speed_delta = -effect->speed;
	effect->timestamp = LCUI_GetTime();
	if (effect->is_running) {
		return;
	}
	effect->is_running = TRUE;
	if (effect->timer > 0) {
		LCUITimer_Free(effect->timer);
	}
	effect->timer =
	    LCUITimer_Set(effect->interval, OnInertialScrolling, w, TRUE);
	DEBUG_MSG("start_pos: %d, end_pos: %d\n", effect->start_pos,
		  effect->end_pos);
	DEBUG_MSG("effect->speed: %g, distance: %d, time: %d\n", effect->speed,
		  distance, (int)time_delta);
}

static void Slider_OnMouseMove(LCUI_Widget slider, LCUI_WidgetEvent e,
			       void *arg)
{
	LCUI_Widget target;
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar;
	float n, box_size, size, layer_pos, x, y;

	scrollbar = Widget_GetData(w, self.prototype);
	if (!scrollbar->is_dragged || !scrollbar->target) {
		return;
	}
	target = scrollbar->target;
	if (scrollbar->direction == SBD_HORIZONTAL) {
		y = 0;
		x = scrollbar->slider_x;
		x += e->motion.x - scrollbar->mouse_x;
		if (scrollbar->box) {
			box_size = scrollbar->box->box.content.width;
		} else {
			box_size = w->parent->box.content.width;
		}
		layer_pos = scrollbar->target->box.outer.width - box_size;
		size = w->box.content.width - slider->width;
		if (x > size) {
			x = size;
		} else if (x < 0) {
			x = 0;
		}
		n = 0.0;
		if (size > 0) {
			n = slider->x / size;
			if (n > 1.0) {
				n = 1;
			}
		}
		layer_pos = layer_pos * n;
		Widget_SetStyle(target, key_left, -layer_pos, px);
	} else {
		x = 0;
		y = scrollbar->slider_y;
		y += e->motion.y - scrollbar->mouse_y;
		if (scrollbar->box) {
			box_size = scrollbar->box->box.content.height;
		} else {
			box_size = w->parent->box.content.height;
		}
		layer_pos = scrollbar->target->box.outer.height - box_size;
		size = w->box.content.height - slider->height;
		if (y > size) {
			y = size;
		} else if (y < 0) {
			y = 0;
		}
		n = 0.0;
		if (size > 0) {
			n = slider->y / size;
			if (n > 1.0) {
				n = 1;
			}
		}
		layer_pos = layer_pos * n;
		Widget_SetStyle(target, key_top, -layer_pos, px);
	}
	if (scrollbar->pos != iround(layer_pos)) {
		LCUI_WidgetEventRec e = { 0 };
		e.type = self.event_id;
		e.cancel_bubble = TRUE;
		Widget_TriggerEvent(target, &e, &layer_pos);
	}
	scrollbar->pos = iround(layer_pos);
	Widget_UpdateStyle(target, FALSE);
	Widget_Move(slider, x, y);
}

static void Slider_OnMouseUp(LCUI_Widget slider, LCUI_WidgetEvent e, void *arg)
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);

	Widget_UnbindEvent(slider, "mousemove", Slider_OnMouseMove);
	Widget_UnbindEvent(slider, "mouseup", Slider_OnMouseUp);
	Widget_ReleaseMouseCapture(slider);
	scrollbar->is_dragged = FALSE;
}

static void Slider_OnMouseDown(LCUI_Widget slider, LCUI_WidgetEvent e,
			       void *arg)
{
	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);

	if (scrollbar->is_dragged) {
		return;
	}
	scrollbar->slider_x = slider->x;
	scrollbar->slider_y = slider->y;
	scrollbar->mouse_x = e->motion.x;
	scrollbar->mouse_y = e->motion.y;
	scrollbar->is_dragged = TRUE;
	Widget_SetMouseCapture(slider);
	Widget_BindEvent(slider, "mousemove", Slider_OnMouseMove, w, NULL);
	Widget_BindEvent(slider, "mouseup", Slider_OnMouseUp, w, NULL);
}

static void ScrollBar_OnLink(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);
	if (!scrollbar->box) {
		ScrollBar_BindBox(w, w->parent);
	}
}

static void ScrollBar_OnBoxDestroy(LCUI_Widget box, LCUI_WidgetEvent e, void *arg)
{
	LCUI_Widget w = e->data;

	ScrollBar_BindBox(e->data, box == w->parent ? NULL : w->parent);
}

static void ScrollBar_OnInit(LCUI_Widget w)
{
	LCUI_Widget slider;
	LCUI_ScrollBar scrollbar;
	const size_t data_size = sizeof(LCUI_ScrollBarRec);

	slider = LCUIWidget_New(NULL);
	scrollbar = Widget_AddData(w, self.prototype, data_size);
	scrollbar->direction = SBD_VERTICAL;
	scrollbar->is_dragged = FALSE;
	scrollbar->is_draggable = FALSE;
	scrollbar->scroll_step = 64;
	scrollbar->slider = slider;
	scrollbar->target = NULL;
	scrollbar->box = NULL;
	scrollbar->old_pos = 0;
	scrollbar->pos = 0;
	scrollbar->touch_point_id = -1;
	InitInertialScrolling(&scrollbar->effect);
	Widget_BindEvent(slider, "mousedown", Slider_OnMouseDown, w, NULL);
	Widget_BindEvent(w, "link", ScrollBar_OnLink, NULL, NULL);
	Widget_AddClass(slider, "scrollbar-slider");
	Widget_Append(w, slider);
}

static void ScrollBar_UpdateSize(LCUI_Widget w)
{
	float n = 1.0, size, box_size;
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);
	LCUI_Widget slider = scrollbar->slider;

	if (!scrollbar->box) {
		return;
	}
	if (scrollbar->direction == SBD_HORIZONTAL) {
		if (scrollbar->target) {
			size = scrollbar->target->box.outer.width;
		} else {
			size = 0;
		}
		box_size = scrollbar->box->width;
		if (size > box_size && box_size > 0) {
			n = box_size / size;
		}
		Widget_SetStyle(slider, key_width, n, scale);
	} else {
		if (scrollbar->target) {
			size = scrollbar->target->box.outer.height;
		} else {
			size = 0;
		}
		box_size = scrollbar->box->height;
		if (size > box_size && box_size > 0) {
			n = box_size / size;
		}
		Widget_SetStyle(slider, key_height, n, scale);
	}
	ScrollBar_SetPosition(w, scrollbar->pos);
	Widget_UpdateStyle(slider, FALSE);
	if (n < 1.0) {
		Widget_Show(w);
	} else {
		Widget_Hide(w);
	}
}

static void ScrollBox_OnWheel(LCUI_Widget box, LCUI_WidgetEvent e, void *arg)
{
	int pos, new_pos;

	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);

	pos = ScrollBar_GetPosition(w);
	if (e->wheel.delta > 0) {
		new_pos = pos - scrollbar->scroll_step;
	} else {
		new_pos = pos + scrollbar->scroll_step;
	}
	/* If the position of the scroll bar is changed, then prevent
	 * the event bubbling, to avoid change the parent scroll bars */
	if (pos != ScrollBar_SetPosition(w, new_pos)) {
		e->cancel_bubble = TRUE;
	}
}

/** 容器的触屏事件响应 */
static void ScrollBox_OnTouch(LCUI_Widget box, LCUI_WidgetEvent e, void *arg)
{
	uint_t time_delta;
	int i, pos, distance;

	LCUI_Widget w = e->data;
	LCUI_ScrollBar scrollbar;
	LCUI_TouchPoint point;

	if (e->touch.n_points < 1) {
		return;
	}
	scrollbar = Widget_GetData(w, self.prototype);
	if (scrollbar->touch_point_id == -1) {
		point = &e->touch.points[0];
		/* 如果这个触点的状态不是 TOUCHDOWN，则说明是上次触控拖拽操
		 * 作时的多余触点，直接忽略这次触控事件 */
		if (point->state != LCUI_WEVENT_TOUCHDOWN) {
			return;
		}
		scrollbar->touch_point_id = point->id;
	} else {
		for (point = NULL, i = 0; i < e->touch.n_points; ++i) {
			point = &e->touch.points[i];
			if (point->id == scrollbar->touch_point_id) {
				break;
			}
		}
		if (!point) {
			return;
		}
	}
	switch (point->state) {
	case LCUI_WEVENT_TOUCHDOWN:
		scrollbar->distance = 0;
		scrollbar->effect.speed = 0;
		scrollbar->effect.is_running = FALSE;
		scrollbar->old_pos = scrollbar->pos;
		if (scrollbar->is_dragged) {
			return;
		}
		scrollbar->mouse_x = point->x;
		scrollbar->mouse_y = point->y;
		scrollbar->is_draggable = TRUE;
		break;
	case LCUI_WEVENT_TOUCHUP:
		Widget_ReleaseTouchCapture(box, -1);
		time_delta = (uint_t)LCUI_GetTimeDelta(scrollbar->timestamp);
		if (scrollbar->is_dragged && time_delta < 50) {
			StartInertialScrolling(w);
		}
		scrollbar->touch_point_id = -1;
		scrollbar->is_dragged = FALSE;
		Widget_BlockEvent(box, FALSE);
		break;
	case LCUI_WEVENT_TOUCHMOVE:
		if (!scrollbar->is_draggable) {
			break;
		}
		e->cancel_bubble = TRUE;
		pos = scrollbar->old_pos;
		if (scrollbar->direction == SBD_HORIZONTAL) {
			pos -= point->x - scrollbar->mouse_x;
		} else {
			pos -= point->y - scrollbar->mouse_y;
		}
		if (pos == scrollbar->pos) {
			break;
		}
		distance = pos - scrollbar->pos;
		if ((scrollbar->distance > 0) != (distance > 0) ||
		    scrollbar->distance == 0) {
			UpdateInertialScrolling(&scrollbar->effect,
						scrollbar->pos);
		}
		scrollbar->distance = distance;
		scrollbar->timestamp = LCUI_GetTime();
		ScrollBar_SetPosition(w, pos);
		if (scrollbar->is_dragged) {
			break;
		}
		/* If the position of the scroll bar is not changed, then
		 * mark current drag action should be ignore */
		if (scrollbar->is_draggable &&
		    scrollbar->old_pos == scrollbar->pos) {
			scrollbar->is_dragged = FALSE;
			scrollbar->is_draggable = FALSE;
			e->cancel_bubble = FALSE;
			break;
		}
		/* start drag action and block all events of box */
		scrollbar->is_dragged = TRUE;
		LCUIWidget_ClearEventTarget(NULL);
		Widget_BlockEvent(box, TRUE);
		Widget_SetTouchCapture(box, point->id);
	default:
		break;
	}
}

static void ScrollBar_OnUpdateSize(LCUI_Widget box, LCUI_WidgetEvent e,
				   void *arg)
{
	ScrollBar_UpdateSize(e->data);
}

static void ScrollBar_OnSetPosition(LCUI_Widget box, LCUI_WidgetEvent e,
				    void *arg)
{
	int *pos = arg;
	ScrollBar_SetPosition(e->data, *pos);
	e->cancel_bubble = TRUE;
}

void ScrollBar_BindBox(LCUI_Widget w, LCUI_Widget box)
{
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);

	if (scrollbar->box) {
		Widget_UnbindEvent(scrollbar->box, "resize",
				   ScrollBar_OnUpdateSize);
		Widget_UnbindEvent(scrollbar->box, "setscroll",
				   ScrollBar_OnSetPosition);
		Widget_UnbindEvent(scrollbar->box, "mousewheel",
				   ScrollBox_OnWheel);
		Widget_UnbindEvent(scrollbar->box, "touch", ScrollBox_OnTouch);
		Widget_UnbindEvent(scrollbar->box, "destroy",
				   ScrollBar_OnBoxDestroy);
	}
	scrollbar->box = box;
	if (box) {
		Widget_BindEvent(box, "resize", ScrollBar_OnUpdateSize, w,
				 NULL);
		Widget_BindEvent(box, "setscroll", ScrollBar_OnSetPosition, w,
				 NULL);
		Widget_BindEvent(box, "mousewheel", ScrollBox_OnWheel, w, NULL);
		Widget_BindEvent(box, "touch", ScrollBox_OnTouch, w, NULL);
		Widget_BindEvent(box, "destroy", ScrollBar_OnBoxDestroy, w,
				 NULL);
	}
	ScrollBar_UpdateSize(w);
}

void ScrollBar_BindTarget(LCUI_Widget w, LCUI_Widget target)
{
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);

	if (scrollbar->target) {
		Widget_RemoveClass(scrollbar->target, "scrollbar-target");
		Widget_UnbindEvent(scrollbar->target, "resize",
				   ScrollBar_OnUpdateSize);
	}
	scrollbar->target = target;
	Widget_AddClass(target, "scrollbar-target");
	Widget_BindEvent(target, "resize", ScrollBar_OnUpdateSize, w, NULL);
	ScrollBar_UpdateSize(w);
}

int ScrollBar_GetPosition(LCUI_Widget w)
{
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);
	return scrollbar->pos;
}

int ScrollBar_SetPosition(LCUI_Widget w, int pos)
{
	float new_pos, box_size, size, slider_pos;
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);
	LCUI_Widget slider = scrollbar->slider;
	LCUI_Widget target = scrollbar->target;
	LCUI_WidgetEvent e = { 0 };

	if (!target) {
		return 0;
	}
	new_pos = 1.0f * pos;
	memset(&e, 0, sizeof(e));
	if (scrollbar->direction == SBD_HORIZONTAL) {
		size = scrollbar->target->box.outer.width;
		if (scrollbar->box) {
			box_size = scrollbar->box->box.content.width;
		} else {
			box_size = w->parent->box.content.width;
		}
		if (new_pos + box_size > size) {
			new_pos = size - box_size;
		}
		if (new_pos < 0.0f) {
			new_pos = 0.0f;
		}
		slider_pos = w->box.content.width - slider->width;
		slider_pos = slider_pos * new_pos / (size - box_size);
		Widget_SetStyle(slider, key_left, slider_pos, px);
		Widget_SetStyle(target, key_left, -new_pos, px);
	} else {
		size = scrollbar->target->box.outer.height;
		if (scrollbar->box) {
			box_size = scrollbar->box->box.content.height;
		} else {
			box_size = w->parent->box.content.height;
		}
		if (new_pos + box_size > size) {
			new_pos = size - box_size;
		}
		if (new_pos < 0) {
			new_pos = 0;
		}
		slider_pos = w->box.content.height - slider->height;
		if (size == box_size) {
			slider_pos = 0;
		} else {
			slider_pos = slider_pos * new_pos / (size - box_size);
		}
		Widget_SetStyle(slider, key_top, slider_pos, px);
		Widget_SetStyle(target, key_top, -new_pos, px);
	}
	pos = iround(new_pos);
	if (scrollbar->pos != pos) {
		LCUI_WidgetEventRec e = { 0 };
		e.type = self.event_id;
		e.cancel_bubble = TRUE;
		Widget_TriggerEvent(target, &e, &new_pos);
	}
	scrollbar->pos = pos;
	Widget_UpdateStyle(slider, FALSE);
	Widget_UpdateStyle(target, FALSE);
	return pos;
}

void ScrollBar_SetDirection(LCUI_Widget w, int direction)
{
	LCUI_ScrollBar scrollbar = Widget_GetData(w, self.prototype);

	if (direction == SBD_HORIZONTAL) {
		Widget_RemoveClass(w, "scrollbar-horizontal");
	} else {
		Widget_AddClass(w, "scrollbar-horizontal");
	}
	scrollbar->direction = direction;
}

static void ScrollBar_OnSetAttr(LCUI_Widget w, const char *name,
				const char *value)
{
	LCUI_Widget target;

	if (strcmp(name, "parent") == 0) {
		target = LCUIWidget_GetById(value);
		if (target) {
			ScrollBar_BindBox(w, target);
		}
	} else if (strcmp(name, "target") == 0) {
		target = LCUIWidget_GetById(value);
		if (target) {
			ScrollBar_BindTarget(w, target);
		}
	}
}

void LCUIWidget_AddTScrollBar(void)
{
	int setscroll_event_id;

	self.prototype = LCUIWidget_NewPrototype("scrollbar", NULL);
	self.prototype->init = ScrollBar_OnInit;
	self.prototype->setattr = ScrollBar_OnSetAttr;
	self.event_id = LCUIWidget_AllocEventId();
	setscroll_event_id = LCUIWidget_AllocEventId();
	LCUIWidget_SetEventName(self.event_id, "scroll");
	LCUIWidget_SetEventName(setscroll_event_id, "setscroll");
	LCUI_LoadCSSString(scrollbar_css, __FILE__);
}
