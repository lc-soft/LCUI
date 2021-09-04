/*
 * widget.c -- GUI widget APIs.
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
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/canvas.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/textcaret.h>
#include <LCUI/gui/widget/textedit.h>
#include <LCUI/gui/widget/anchor.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/gui/widget/sidebar.h>
#include <LCUI/gui/widget/scrollbar.h>
#include "widget_background.h"

void LCUI_InitWidget(void)
{
	ui_init_updater();
	ui_init_events();
	ui_init_widget_prototype();
	LCUIWidget_InitStyle();
	LCUIWidget_InitRenderer();
	ui_init_image_loader();
	LCUIWidget_InitBase();
	ui_init_widget_id();
}

void LCUI_FreeWidget(void)
{
	LCUIWidget_FreeTextView();
	ui_destroy_updater();
	LCUIWidget_FreeRoot();
	ui_destroy_events();
	LCUIWidget_FreeStyle();
	ui_destroy_widget_prototype();
	LCUIWidget_FreeRenderer();
	ui_destroy_image_loader();
	ui_destroy_widget_id();
	LCUIWidget_FreeBase();
}
