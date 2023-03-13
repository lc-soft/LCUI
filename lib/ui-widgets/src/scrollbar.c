/*
 * scrollbar.c -- Scrollbar widget
 *
 * Copyright (c) 2018-2022, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <LCUI/timer.h>
#include <LCUI/ui.h>
#include <css.h>
#include "../include/scrollbar.h"

/* clang-format off */

#define EFFECT_FRAMES 80

/** 惯性滚动效果的相关数据 */
typedef struct ui_scroll_effect_t {
	float start_pos;	/**< 开始移动时的位置 */
	float end_pos;		/**< 结束移动时的位置 */
	int timer;		/**< 定时器 */
	int interval;		/**< 定时器的间隔时间 */
	float speed;		/**< 滚动速度 */
	float speed_delta;	/**< 速度差（加速度） */
	int64_t timestamp;	/**< 开始时间 */
	LCUI_BOOL is_running;	/**< 当前效果是否正在运行 */
} ui_scroll_effect_t;

/** 滚动条的相关数据 */
typedef struct ui_scrollbar_t_ {
	ui_widget_t *container;
	ui_widget_t *target;
	ui_widget_t *thumb;			/**< thumb of scrollbar */
	LCUI_BOOL is_dragging;			/**< whether the content is dragged */
	LCUI_BOOL is_draggable;			/**< whether the content can be dragged */
	ui_scrollbar_direction_t direction;	/**< 滚动条的方向（垂直或水平） */
	float thumb_x, thumb_y;			/**< 拖拽开始时的滑块位置 */
	float mouse_x, mouse_y;			/**< 拖拽开始时的鼠标坐标 */
	int touch_point_id;			/**< 触点的ID */
	float scroll_step;			/**< 每次滚动的距离，主要针对使用鼠标滚轮触发的滚动 */
	float pos;				/**< 当前的位置 */
	float old_pos;				/**< 拖拽开始时的位置 */
	float distance;				/**< 滚动距离 */
	int64_t timestamp;			/**< 数据更新时间，主要针对触控拖动时的位置变化 */
	ui_scroll_effect_t effect;		/**< 用于实现惯性滚动效果的相关数据 */
	ui_mutation_observer_t *observer;
} ui_scrollbar_t;

static ui_widget_prototype_t *ui_scrollbar_proto;

static const char *ui_scrollbar_css = css_string(

scrollbar {
	display: flex;
	position: absolute;
	background-color: #fafafa;
	box-sizing: border-box;
}

.scrollbar-track {
	flex: auto;
}

scrollbar.vertical {
	top: 0;
	right: 0;
	width: 14px;
	height: 100%;
	flex-direction: column;
}

scrollbar.horizontal {
	bottom: 0;
	left: 0;
	width: 100%;
	height: 14px;
}

.scrollbar-corner {
	flex: none;
	width: 14px;
	height: 14px;
	display: none;
}

.scrollbar-thumb {
	top: 0;
	left: 0;
	width: 14px;
	height: 14px;
	min-width: 14px;
	min-height: 14px;
	position: absolute;
	background-color: #aaa;
}

.scrollbar-thumb:hover {
	background-color: #bbb;
}

.scrollbar-thumb:active {
	background-color: #999;
}

.scrollbar-target {
	top: 0;
	left: 0;
	position: relative;
}

.has-horizontal-scrollbar {
	padding-bottom: 14px;
}

.has-vertical-scrollbar {
	padding-right: 14px;
}

.has-horizontal-scrollbar scrollbar.vertical .scrollbar-corner,
.has-vertical-scrollbar scrollbar.horizontal .scrollbar-corner {
	display: block;
}

);

/* clang-format on */

static void ui_scrollbar_on_scrolling(void *arg)
{
	float pos, distance, time;
	ui_scrollbar_t *scrollbar;
	ui_scroll_effect_t *effect;
	ui_widget_t *w = arg;

	scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	effect = &scrollbar->effect;
	time = (float)get_time_delta(effect->timestamp) / 1000.f;
	distance = (effect->speed + 0.5f * effect->speed_delta * time) * time;
	pos = effect->end_pos + y_iround(distance);
	DEBUG_MSG("distance: %g, pos: %g, speed_delta: %g, speed: %g\n",
		  distance, pos, effect->speed_delta,
		  effect->speed + effect->speed_delta * time);
	if (effect->is_running) {
		double speed = effect->speed + effect->speed_delta * time;
		if ((effect->speed > 0 && speed > 0) ||
		    (effect->speed < 0 && speed < 0)) {
			ui_scrollbar_set_position(w, pos);
			return;
		}
	}
	lcui_destroy_timer(effect->timer);
	effect->is_running = FALSE;
	effect->timer = -1;
}

static void ui_scroll_effect_init(ui_scroll_effect_t *effect)
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

static void ui_scroll_effect_update(ui_scroll_effect_t *effect, float pos)
{
	effect->speed = 0;
	effect->is_running = FALSE;
	effect->start_pos = pos;
	effect->timestamp = get_time_ms();
}

static void ui_scrollbar_start_scrolling(ui_widget_t *w)
{
	float distance;
	int64_t time_delta;

	ui_scrollbar_t *scrollbar;
	ui_scroll_effect_t *effect;

	scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	effect = &scrollbar->effect;
	effect->end_pos = scrollbar->pos;
	distance = effect->end_pos - effect->start_pos;
	time_delta = get_time_delta(effect->timestamp);
	/* 根据距离计算当前移动速度 */
	if (time_delta > 0) {
		effect->speed = 1000.f * distance / time_delta;
	} else {
		effect->speed = 0;
		return;
	}
	effect->speed_delta = -effect->speed;
	effect->timestamp = get_time_ms();
	if (effect->is_running) {
		return;
	}
	effect->is_running = TRUE;
	if (effect->timer > 0) {
		lcui_destroy_timer(effect->timer);
	}
	effect->timer =
	    lcui_set_interval(effect->interval, ui_scrollbar_on_scrolling, w);
	DEBUG_MSG("start_pos: %d, end_pos: %d\n", effect->start_pos,
		  effect->end_pos);
	DEBUG_MSG("effect->speed: %g, distance: %d, time: %d\n", effect->speed,
		  distance, (int)time_delta);
}

static void ui_scrollbar_thumb_on_mousemove(ui_widget_t *thumb, ui_event_t *e,
					    void *arg)
{
	ui_widget_t *target;
	ui_widget_t *container;
	ui_widget_t *w = e->data;
	ui_scrollbar_t *scrollbar;
	css_numeric_value_t size, layer_pos, x, y;

	scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	container = scrollbar->container ? scrollbar->container : w->parent;
	if (!scrollbar->is_dragging || !scrollbar->target) {
		return;
	}
	target = scrollbar->target;
	if (scrollbar->direction == UI_SCROLLBAR_HORIZONTAL) {
		size =
		    thumb->parent->content_box.width - thumb->border_box.width;
		x = scrollbar->thumb_x + e->mouse.x - scrollbar->mouse_x;
		x = y_max(0, y_min(x, size));
		y = 0;
		layer_pos = (float)((scrollbar->target->outer_box.width -
				     container->content_box.width) *
				    y_max(0, y_min(x / size, 1.0)));
		ui_widget_set_style_unit_value(target, css_key_left, -layer_pos,
					       CSS_UNIT_PX);
	} else {
		size = thumb->parent->content_box.height -
		       thumb->border_box.height;
		x = 0;
		y = scrollbar->thumb_y + e->mouse.y - scrollbar->mouse_y;
		y = y_max(0, y_min(y, size));
		layer_pos = (float)((scrollbar->target->outer_box.height -
				     container->content_box.height) *
				    y_max(0, y_min(y / size, 1.0)));
		ui_widget_set_style_unit_value(target, css_key_top, -layer_pos,
					       CSS_UNIT_PX);
	}
	if (scrollbar->pos != layer_pos) {
		ui_event_t e;
		ui_event_init(&e, "scroll");
		e.cancel_bubble = TRUE;
		ui_widget_emit_event(target, e, &layer_pos);
	}
	scrollbar->pos = layer_pos;
	ui_widget_update_style(target);
	ui_widget_move(thumb, x, y);
}

static void ui_scrollbar_thumb_on_mouseup(ui_widget_t *thumb, ui_event_t *e,
					  void *arg)
{
	ui_widget_t *w = e->data;
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	ui_widget_off(thumb, "mousemove", ui_scrollbar_thumb_on_mousemove, w);
	ui_widget_off(thumb, "mouseup", ui_scrollbar_thumb_on_mouseup, w);
	ui_widget_release_mouse_capture(thumb);
	scrollbar->is_dragging = FALSE;
}

static void Ui_scrollbar_thumb_on_mousedown(ui_widget_t *thumb, ui_event_t *e,
					    void *arg)
{
	ui_widget_t *w = e->data;
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	if (scrollbar->is_dragging) {
		return;
	}
	scrollbar->thumb_x = thumb->canvas_box.x;
	scrollbar->thumb_y = thumb->canvas_box.y;
	scrollbar->mouse_x = e->mouse.x;
	scrollbar->mouse_y = e->mouse.y;
	scrollbar->is_dragging = TRUE;
	ui_widget_set_mouse_capture(thumb);
	ui_widget_on(thumb, "mousemove", ui_scrollbar_thumb_on_mousemove, w,
		     NULL);
	ui_widget_on(thumb, "mouseup", ui_scrollbar_thumb_on_mouseup, w, NULL);
}

static void ui_scrollbar_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	if (!scrollbar->container) {
		ui_scrollbar_bind_container(w, w->parent);
	}
}

static void ui_scrollbar_on_container_destroy(ui_widget_t *container,
					      ui_event_t *e, void *arg)
{
	ui_widget_t *w = e->data;
	ui_scrollbar_bind_container(w, container == w->parent ? NULL: w->parent);
}

static void ui_scrollbar_on_target_destroy(ui_widget_t *target,
					   ui_event_t *e, void *arg)
{
	ui_scrollbar_bind_target(e->data, NULL);
}

static void ui_scrollbar_update_size(ui_widget_t *w)
{
	css_numeric_value_t n = 100.f, size, box_size;
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	ui_widget_t *thumb = scrollbar->thumb;

	if (!scrollbar->container) {
		return;
	}
	if (scrollbar->direction == UI_SCROLLBAR_HORIZONTAL) {
		if (scrollbar->target) {
			size = scrollbar->target->outer_box.width;
		} else {
			size = 0;
		}
		box_size = scrollbar->container->border_box.width;
		if (size > box_size && box_size > 0) {
			n = 100.f * box_size / size;
		}
		ui_widget_set_style_unit_value(thumb, css_key_width, n,
					       CSS_UNIT_PERCENT);
	} else {
		if (scrollbar->target) {
			size = scrollbar->target->outer_box.height;
		} else {
			size = 0;
		}
		box_size = scrollbar->container->border_box.height;
		if (size > box_size && box_size > 0) {
			n = 100.f * box_size / size;
		}
		ui_widget_set_style_unit_value(thumb, css_key_height, n,
					       CSS_UNIT_PERCENT);
	}
	ui_scrollbar_set_position(w, scrollbar->pos);
	ui_widget_update_style(thumb);
	if (n < 100.f) {
		ui_widget_show(w);
		if (scrollbar->direction == UI_SCROLLBAR_HORIZONTAL) {
			ui_widget_add_class(scrollbar->container,
					    "has-horizontal-scrollbar");
		} else {
			ui_widget_add_class(scrollbar->container,
					    "has-vertical-scrollbar");
		}
	} else {
		ui_widget_hide(w);
		if (scrollbar->direction == UI_SCROLLBAR_HORIZONTAL) {
			ui_widget_remove_class(scrollbar->container,
					       "has-horizontal-scrollbar");
		} else {
			ui_widget_remove_class(scrollbar->container,
					       "has-vertical-scrollbar");
		}
	}
}

static void ui_scrollbar_on_container_wheel(ui_widget_t *container,
					    ui_event_t *e, void *arg)
{
	float pos, new_pos;

	ui_widget_t *w = e->data;
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	// TODO: Add support to handle horizontal scrolling
	// https://docs.microsoft.com/en-us/windows/win32/controls/scroll-bars
	if (scrollbar->direction != UI_SCROLLBAR_VERTICAL) {
		return;
	}
	pos = ui_scrollbar_get_position(w);
	if (e->wheel.delta_y > 0) {
		new_pos = pos - scrollbar->scroll_step;
	} else {
		new_pos = pos + scrollbar->scroll_step;
	}
	/* If the position of the scroll bar is changed, then prevent
	 * the event bubbling, to avoid change the parent scroll bars */
	if (pos != ui_scrollbar_set_position(w, new_pos)) {
		e->cancel_bubble = TRUE;
	}
}

/** 容器的触屏事件响应 */
static void ui_scrollbar_on_container_touch(ui_widget_t *container,
					    ui_event_t *e, void *arg)
{
	unsigned time_delta;
	float pos, distance;
	unsigned i;

	ui_widget_t *w = e->data;
	ui_scrollbar_t *scrollbar;
	ui_touch_point_t *point;

	if (e->touch.n_points < 1) {
		return;
	}
	scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	if (scrollbar->touch_point_id == -1) {
		point = &e->touch.points[0];
		/* 如果这个触点的状态不是 TOUCHDOWN，则说明是上次触控拖拽操
		 * 作时的多余触点，直接忽略这次触控事件 */
		if (point->state != UI_EVENT_TOUCHDOWN) {
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
	case UI_EVENT_TOUCHDOWN:
		scrollbar->distance = 0;
		scrollbar->effect.speed = 0;
		scrollbar->effect.is_running = FALSE;
		scrollbar->old_pos = scrollbar->pos;
		if (scrollbar->is_dragging) {
			return;
		}
		scrollbar->mouse_x = point->x;
		scrollbar->mouse_y = point->y;
		scrollbar->is_draggable = TRUE;
		break;
	case UI_EVENT_TOUCHUP:
		ui_widget_release_touch_capture(container, -1);
		time_delta = (unsigned)get_time_delta(scrollbar->timestamp);
		if (scrollbar->is_dragging && time_delta < 50) {
			ui_scrollbar_start_scrolling(w);
		}
		scrollbar->touch_point_id = -1;
		scrollbar->is_dragging = FALSE;
		ui_widget_block_event(container, FALSE);
		break;
	case UI_EVENT_TOUCHMOVE:
		if (!scrollbar->is_draggable) {
			break;
		}
		e->cancel_bubble = TRUE;
		pos = scrollbar->old_pos;
		if (scrollbar->direction == UI_SCROLLBAR_HORIZONTAL) {
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
			ui_scroll_effect_update(&scrollbar->effect,
						scrollbar->pos);
		}
		scrollbar->distance = distance;
		scrollbar->timestamp = get_time_ms();
		ui_scrollbar_set_position(w, pos);
		if (scrollbar->is_dragging) {
			break;
		}
		/* If the position of the scroll bar is not changed, then
		 * mark current drag action should be ignore */
		if (scrollbar->is_draggable &&
		    scrollbar->old_pos == scrollbar->pos) {
			scrollbar->is_dragging = FALSE;
			scrollbar->is_draggable = FALSE;
			e->cancel_bubble = FALSE;
			break;
		}
		/* start drag action and block all events of container */
		scrollbar->is_dragging = TRUE;
		ui_clear_event_target(NULL);
		ui_widget_block_event(container, TRUE);
		ui_widget_set_touch_capture(container, point->id);
	default:
		break;
	}
}

static void ui_scrollbar_on_mutation(ui_mutation_list_t *mutation_list,
				     ui_mutation_observer_t *observer,
				     void *arg)
{
	list_node_t *node;
	ui_mutation_record_t *mutation;

	for (list_each(node, mutation_list)) {
		mutation = node->data;
		if (mutation->type == UI_MUTATION_RECORD_TYPE_PROPERTIES &&
		    (strcmp(mutation->property_name, "width") == 0 ||
		     strcmp(mutation->property_name, "height") == 0)) {
			ui_scrollbar_update_size(arg);
			break;
		}
	}
}

static void ui_scrollbar_on_container_set_scroll(ui_widget_t *container,
						 ui_event_t *e, void *arg)
{
	float *pos = arg;
	ui_scrollbar_set_position(e->data, *pos);
	e->cancel_bubble = TRUE;
}

static void ui_scrollbar_observe(ui_widget_t *w)
{
	ui_mutation_observer_init_t options = { .properties = TRUE };
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	if (scrollbar->container) {
		ui_mutation_observer_observe(scrollbar->observer,
					     scrollbar->container, options);
		ui_widget_on(scrollbar->container, "setscroll",
			     ui_scrollbar_on_container_set_scroll, w, NULL);
		ui_widget_on(scrollbar->container, "mousewheel",
			     ui_scrollbar_on_container_wheel, w, NULL);
		ui_widget_on(scrollbar->container, "touch",
			     ui_scrollbar_on_container_touch, w, NULL);
		ui_widget_on(scrollbar->container, "destroy",
			     ui_scrollbar_on_container_destroy, w, NULL);
	}
	if (scrollbar->target) {
		ui_mutation_observer_observe(scrollbar->observer,
					     scrollbar->target, options);
		ui_widget_add_class(scrollbar->target, "scrollbar-target");
		ui_widget_on(scrollbar->target, "destroy",
			     ui_scrollbar_on_target_destroy, w, NULL);
	}
	ui_scrollbar_update_size(w);
}

static void ui_scrollbar_disconnect(ui_widget_t *w)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	ui_mutation_observer_disconnect(scrollbar->observer);
	if (scrollbar->container) {
		ui_widget_off(scrollbar->container, "setscroll",
			      ui_scrollbar_on_container_set_scroll, w);
		ui_widget_off(scrollbar->container, "mousewheel",
			      ui_scrollbar_on_container_wheel, w);
		ui_widget_off(scrollbar->container, "touch",
			      ui_scrollbar_on_container_touch, w);
		ui_widget_off(scrollbar->container, "destroy",
			      ui_scrollbar_on_container_destroy, w);
	}
	if (scrollbar->target) {
		ui_widget_remove_class(scrollbar->target, "scrollbar-target");
		ui_widget_off(scrollbar->target, "destroy",
			      ui_scrollbar_on_target_destroy, w);
	}
}

void ui_scrollbar_bind_container(ui_widget_t *w, ui_widget_t *container)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	ui_scrollbar_disconnect(w);
	scrollbar->container = container;
	ui_scrollbar_observe(w);
}

void ui_scrollbar_bind_target(ui_widget_t *w, ui_widget_t *target)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	ui_scrollbar_disconnect(w);
	scrollbar->target = target;
	ui_scrollbar_observe(w);
}

float ui_scrollbar_get_position(ui_widget_t *w)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	return scrollbar->pos;
}

float ui_scrollbar_set_position(ui_widget_t *w, float pos)
{
	float new_pos, box_size, size, thumb_pos;
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);
	ui_widget_t *thumb = scrollbar->thumb;
	ui_widget_t *content = scrollbar->target;
	ui_event_t *e = { 0 };

	if (!content) {
		return 0;
	}
	new_pos = 1.0f * pos;
	memset(&e, 0, sizeof(e));
	if (scrollbar->direction == UI_SCROLLBAR_HORIZONTAL) {
		size = scrollbar->target->outer_box.width;
		if (scrollbar->container) {
			box_size = scrollbar->container->content_box.width;
		} else {
			box_size = w->parent->content_box.width;
		}
		if (new_pos + box_size > size) {
			new_pos = size - box_size;
		}
		if (new_pos < 0.0f) {
			new_pos = 0.0f;
		}
		thumb_pos = w->content_box.width - thumb->border_box.width;
		thumb_pos = thumb_pos * new_pos / (size - box_size);
		ui_widget_set_style_unit_value(thumb, css_key_left, thumb_pos,
					       CSS_UNIT_PX);
		ui_widget_set_style_unit_value(content, css_key_left, -new_pos,
					       CSS_UNIT_PX);
	} else {
		size = scrollbar->target->outer_box.height;
		if (scrollbar->container) {
			box_size = scrollbar->container->content_box.height;
		} else {
			box_size = w->parent->content_box.height;
		}
		if (new_pos + box_size > size) {
			new_pos = size - box_size;
		}
		if (new_pos < 0) {
			new_pos = 0;
		}
		thumb_pos = w->content_box.height - thumb->border_box.height;
		if (size == box_size) {
			thumb_pos = 0;
		} else {
			thumb_pos = thumb_pos * new_pos / (size - box_size);
		}
		ui_widget_set_style_unit_value(thumb, css_key_top, thumb_pos,
					       CSS_UNIT_PX);
		ui_widget_set_style_unit_value(content, css_key_top, -new_pos,
					       CSS_UNIT_PX);
	}
	pos = new_pos;
	if (scrollbar->pos != pos) {
		ui_event_t e;
		ui_event_init(&e, "scroll");
		e.cancel_bubble = TRUE;
		ui_widget_emit_event(content, e, &new_pos);
	}
	scrollbar->pos = pos;
	ui_widget_update_style(thumb);
	ui_widget_update_style(content);
	return pos;
}

void ui_scrollbar_set_direction(ui_widget_t *w,
				ui_scrollbar_direction_t direction)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	if (direction == UI_SCROLLBAR_HORIZONTAL) {
		ui_widget_add_class(w, "horizontal");
		ui_widget_remove_class(w, "vertical");
	} else {
		ui_widget_add_class(w, "vertical");
		ui_widget_remove_class(w, "horizontal");
	}
	scrollbar->direction = direction;
}

static void ui_scrollbar_on_set_attr(ui_widget_t *w, const char *name,
				     const char *value)
{
	ui_widget_t *content;

	if (strcmp(name, "parent") == 0) {
		content = ui_get_widget(value);
		if (content) {
			ui_scrollbar_bind_container(w, content);
		}
	} else if (strcmp(name, "target") == 0) {
		content = ui_get_widget(value);
		if (content) {
			ui_scrollbar_bind_target(w, content);
		}
	} else if (strcmp(name, "direction") == 0) {
		if (strcmp(value, "horizontal") == 0) {
			ui_scrollbar_set_direction(w, UI_SCROLLBAR_HORIZONTAL);
		} else {
			ui_scrollbar_set_direction(w, UI_SCROLLBAR_VERTICAL);
		}
	}
}

static void ui_scrollbar_on_init(ui_widget_t *w)
{
	ui_widget_t *track;
	ui_widget_t *corner;
	ui_scrollbar_t *self;

	self =
	    ui_widget_add_data(w, ui_scrollbar_proto, sizeof(ui_scrollbar_t));
	self->direction = UI_SCROLLBAR_VERTICAL;
	self->is_dragging = FALSE;
	self->is_draggable = FALSE;
	self->scroll_step = 64;
	self->target = NULL;
	self->container = NULL;
	self->old_pos = 0;
	self->pos = 0;
	self->touch_point_id = -1;
	self->thumb = ui_create_widget(NULL);
	track = ui_create_widget(NULL);
	corner = ui_create_widget(NULL);

	ui_widget_add_class(track, "scrollbar-track");
	ui_widget_add_class(corner, "scrollbar-corner");
	ui_widget_add_class(self->thumb, "scrollbar-thumb");
	ui_widget_on(self->thumb, "mousedown", Ui_scrollbar_thumb_on_mousedown,
		     w, NULL);
	ui_widget_on(w, "link", ui_scrollbar_on_link, NULL, NULL);
	ui_widget_append(track, self->thumb);
	ui_widget_append(w, track);
	ui_widget_append(w, corner);

	ui_scroll_effect_init(&self->effect);
	ui_scrollbar_set_direction(w, UI_SCROLLBAR_VERTICAL);
	self->observer =
	    ui_mutation_observer_create(ui_scrollbar_on_mutation, w);
}

static void ui_scrollbar_on_destroy(ui_widget_t *w)
{
	ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

	ui_scrollbar_disconnect(w);
	ui_mutation_observer_destroy(scrollbar->observer);
}

void ui_register_scrollbar(void)
{
	ui_scrollbar_proto = ui_create_widget_prototype("scrollbar", NULL);
	ui_scrollbar_proto->init = ui_scrollbar_on_init;
	ui_scrollbar_proto->setattr = ui_scrollbar_on_set_attr;
	ui_scrollbar_proto->destroy = ui_scrollbar_on_destroy;
	ui_load_css_string(ui_scrollbar_css, __FILE__);
}
