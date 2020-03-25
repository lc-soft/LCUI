/*
 * widget_fpsmeter.h -- The widget fps meter operation set.
 *
 * Copyright (c) 2020, Vasilyy Balyasnyy <v.balyasnyy@gmail.com> All rights reserved.
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
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/font/fontlibrary.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_fontstyle.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/font/textlayer.h>
#include <LCUI/gui/widget_fpsmeter.h>

#define FPS_METER_TEXT_SIZE_MAX		(256)
#define FPS_METER_UPDATE_TIME_MS	(1000)

static LCUI_FpsMeterRec self = {FALSE, };


static const char *css = CodeToString(

textview {
	color: rgb(255,255,255);
	font-size: 16px;
	text-align: left;
	z-index: INT_MAX;
	position: absolute;
	padding: 20px, 20px 20px 20px;
	background-color: rgb(60, 60, 60);
	opacity: 0.5;
}

);

void LCUI_FpsMeter_Update()
{
	if (self.is_enabled == FALSE) {
		return;
	}

	char buf[FPS_METER_TEXT_SIZE_MAX];
	snprintf(buf, FPS_METER_TEXT_SIZE_MAX,
			"FPS: %ld\n"
			"Render thread count: %d\n"
			"Widget update count: %ld\n"
			"Render count: %ld",
			self.fps,
			self.render_thread_count,
			self.widget_update_count,
			self.render_count);
	TextView_SetText(self.widget, buf);
}

void LCUI_FpsMeter_FrameCount(void)
{
	self.frame_count++;
	if (LCUI_GetTimeDelta(self.last_time) >= FPS_METER_UPDATE_TIME_MS) {
		self.fps = (self.frame_count * 1000 ) / FPS_METER_UPDATE_TIME_MS;
		self.frame_count = 0;
		self.last_time = LCUI_GetTime();

		LCUI_FpsMeter_Update();
	}
}

void LCUI_FpsMeter_RenderThreadCount(int count)
{
	if (self.render_thread_count != count) {
		self.render_thread_count = count;
	}
}

void LCUI_FpsMeter_RenderCount(size_t count)
{
	if (self.render_count != count) {
		self.render_count = count;
	}
}

void LCUI_FpsMeter_WidgetUpdateCount(size_t count)
{
	if (self.widget_update_count != count) {
		self.widget_update_count = count;
	}
}

void LCUI_FpsMeter_Enable()
{
	self.last_time = LCUI_GetTime();
	self.frame_count = 0;
	self.fps = 0;
	self.render_thread_count = 1;

	self.widget = LCUIWidget_New("textview");
	LCUI_LoadCSSString(css, __FILE__);
	Widget_UpdateStyle(self.widget, TRUE);

	self.is_enabled = TRUE;
	LCUI_FpsMeter_Update();
	Widget_Append(LCUIWidget_GetRoot(), self.widget);
}

void LCUI_FpsMeter_Disable()
{
	self.is_enabled = FALSE;
	Widget_Destroy(self.widget);
}

