/*
 * lib/ui-widgets/src/textcaret.c: -- textcaret widget, used in textinput.
 *
 * Copyright (c) 2018-2022-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <ui.h>
#include <css.h>
#include <yutil.h>
#include <timer.h>
#include <ui_widgets/textcaret.h>
#include <ui_widgets/textinput.h>
#include <platform/ime.h>

typedef struct ui_textcaret_task_t {
        bool active;
        ui_widget_t *widget;
} ui_textcaret_task_t;

/** 文本插入符相关数据 */
typedef struct ui_textcaret_t {
        int timer_id;
        int blink_interval;
        bool visible;
        ui_textcaret_task_t *task;
} ui_textcaret_t;

static ui_widget_prototype_t *ui_textcaret_proto = NULL;

static const char *ui_textcaret_css = "\
textcaret {\
	pointer-events: none;\
	width: 1px;\
	height: 14px;\
	position: absolute;\
	background-color: #000;\
}";

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

void ui_textcaret_set_visible(ui_widget_t *widget, bool visible)
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
