/*
 * textcaret.c -- textcaret widget, used in textedit.
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
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textcaret.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/ime.h>

typedef struct LCUI_TextCaretTaskRec_ {
	LCUI_BOOL active;
	LCUI_Widget widget;
} LCUI_TextCaretTaskRec, *LCUI_TextCaretTask;

/** 文本插入符相关数据 */
typedef struct LCUI_TextCaretRec_ {
	int timer_id;
	int blink_interval;
	LCUI_BOOL visible;
	LCUI_TextCaretTask task;
} LCUI_TextCaretRec, *LCUI_TextCaret;

static LCUI_WidgetPrototype prototype = NULL;

static const char *textcaret_css = CodeToString(

textcaret {
	pointer-events: none;
	focusable: false;
	width: 1px;
	height:14px;
	position: absolute;
	background-color: #000;
}

);

void TextCaret_Refresh(LCUI_Widget widget)
{
	float x, y;

	LCUI_TextCaret caret;

	caret = Widget_GetData(widget, prototype);
	if (!caret->visible) {
		return;
	}
	LCUITimer_Reset(caret->timer_id, caret->blink_interval);
	Widget_GetOffset(widget, LCUIWidget_GetRoot(), &x, &y);
	LCUIIME_SetCaret((int)x, (int)y);
	Widget_Show(widget);
}

static void TextCaret_OnBlink(void *arg)
{
	LCUI_TextCaret caret;
	LCUI_TextCaretTask task = arg;

	if (!task->active) {
		free(task);
		return;
	}
	caret = Widget_GetData(task->widget, prototype);
	if (!caret->visible || Widget_IsVisible(task->widget)) {
		Widget_Hide(task->widget);
	} else {
		Widget_Show(task->widget);
	}
}

void TextCaret_SetVisible(LCUI_Widget widget, LCUI_BOOL visible)
{
	LCUI_TextCaret caret;

	caret = Widget_GetData(widget, prototype);
	caret->visible = visible;
	if (visible) {
		TextCaret_Refresh(widget);
	} else {
		LCUITimer_Reset(caret->timer_id, caret->blink_interval);
		Widget_Hide(widget);
	}
}

static void TextCaret_OnInit(LCUI_Widget widget)
{
	LCUI_TextCaret caret;

	caret = Widget_AddData(widget, prototype, sizeof(LCUI_TextCaretRec));
	caret->task = malloc(sizeof(LCUI_TextCaretTaskRec));
	caret->task->active = TRUE;
	caret->task->widget = widget;
	caret->blink_interval = 500;
	caret->visible = FALSE;
	caret->timer_id = LCUI_SetInterval(caret->blink_interval,
		TextCaret_OnBlink, caret->task);
}

void TextCaret_SetBlinkTime(LCUI_Widget widget, unsigned int n_ms)
{
	LCUI_TextCaret caret;

	caret = Widget_GetData(widget, prototype);
	caret->blink_interval = n_ms;
	LCUITimer_Reset(caret->timer_id, caret->blink_interval);
}

static void TextCaret_OnDestroy(LCUI_Widget widget)
{
	LCUI_TextCaret caret;

	caret = Widget_GetData(widget, prototype);
	caret->task->active = FALSE;
	if (LCUITimer_Free(caret->timer_id) != -1) {
		free(caret->task);
		caret->task = NULL;
	}
	caret->timer_id = -1;
}

void LCUIWidget_AddTextCaret(void)
{
	prototype = LCUIWidget_NewPrototype("textcaret", NULL);
	prototype->init = TextCaret_OnInit;
	prototype->destroy = TextCaret_OnDestroy;
	LCUI_LoadCSSString(textcaret_css, __FILE__);
}
