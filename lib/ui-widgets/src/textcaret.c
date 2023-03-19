/*
 * textcaret.c -- textcaret widget, used in textedit.
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

#include <stdlib.h>
#include <ui.h>
#include <css.h>
#include <yutil.h>
#include <LCUI/timer.h>
#include <ui_widgets/textedit.h>
#include <platform/ime.h>

typedef struct ui_textcaret_task_t {
	LCUI_BOOL active;
	ui_widget_t *widget;
} ui_textcaret_task_t;

/** 文本插入符相关数据 */
typedef struct ui_textcaret_t {
	int timer_id;
	int blink_interval;
	LCUI_BOOL visible;
	ui_textcaret_task_t *task;
} ui_textcaret_t;

static ui_widget_prototype_t *ui_textcaret_proto = NULL;

static const char *ui_textcaret_css = css_string(

    textcaret {
	    pointer - events : none;
    width:
	    1px;
    height:
	    14px;
    position:
	    absolute;
	    background - color : #000;
    }

);

void ui_textcaret_refresh(ui_widget_t *widget)
{
	float x, y;

	ui_textcaret_t *caret;

	caret = ui_widget_get_data(widget, ui_textcaret_proto);
	if (!caret->visible) {
		return;
	}
	lcui_reset_timer(caret->timer_id, caret->blink_interval);
	ui_widget_get_offset(widget, ui_root(), &x, &y);
	ime_set_caret((int)x, (int)y);
	ui_widget_show(widget);
}

static void ui_textcaret_on_blink(void *arg)
{
	ui_textcaret_t *caret;
	ui_textcaret_task_t *task = arg;

	if (!task->active) {
		free(task);
		return;
	}
	caret = ui_widget_get_data(task->widget, ui_textcaret_proto);
	if (!caret->visible || ui_widget_is_visible(task->widget)) {
		ui_widget_hide(task->widget);
	} else {
		ui_widget_show(task->widget);
	}
}

void ui_textcaret_set_visible(ui_widget_t *widget, LCUI_BOOL visible)
{
	ui_textcaret_t *caret;

	caret = ui_widget_get_data(widget, ui_textcaret_proto);
	caret->visible = visible;
	if (visible) {
		ui_textcaret_refresh(widget);
	} else {
		lcui_reset_timer(caret->timer_id, caret->blink_interval);
		ui_widget_hide(widget);
	}
}

static void ui_textcaret_on_init(ui_widget_t *widget)
{
	ui_textcaret_t *caret;

	caret = ui_widget_add_data(widget, ui_textcaret_proto,
				   sizeof(ui_textcaret_t));
	caret->task = malloc(sizeof(ui_textcaret_task_t));
	caret->task->active = TRUE;
	caret->task->widget = widget;
	caret->blink_interval = 500;
	caret->visible = FALSE;
	caret->timer_id = lcui_set_interval(caret->blink_interval,
					    ui_textcaret_on_blink, caret->task);
}

void ui_textcaret_set_blink_time(ui_widget_t *widget, unsigned int n_ms)
{
	ui_textcaret_t *caret;

	caret = ui_widget_get_data(widget, ui_textcaret_proto);
	caret->blink_interval = n_ms;
	lcui_reset_timer(caret->timer_id, caret->blink_interval);
}

static void ui_textcaret_on_destroy(ui_widget_t *widget)
{
	ui_textcaret_t *caret;

	caret = ui_widget_get_data(widget, ui_textcaret_proto);
	caret->task->active = FALSE;
	if (lcui_destroy_timer(caret->timer_id) != -1) {
		free(caret->task);
		caret->task = NULL;
	}
	caret->timer_id = -1;
}

void ui_register_textcaret(void)
{
	ui_textcaret_proto = ui_create_widget_prototype("textcaret", NULL);
	ui_textcaret_proto->init = ui_textcaret_on_init;
	ui_textcaret_proto->destroy = ui_textcaret_on_destroy;
	ui_load_css_string(ui_textcaret_css, __FILE__);
}
