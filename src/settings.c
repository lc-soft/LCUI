/* settings.c -- global settings.
 *
 * Copyright (c) 2020, James Duong <duong.james@gmail.com> All rights reserved.
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
#include <LCUI/types.h>
#include <LCUI/util.h>
#include <LCUI/main.h>
#include <LCUI/settings.h>

static LCUI_SettingsRec self;

static void TriggerSettingsChangedEvent(void)
{
	LCUI_SysEventRec event = { 0 };
	event.type = LCUI_SETTINGS_CHANGE;
	LCUI_TriggerEvent(&event, &self);
}

/* Initialize settings with the current global settings. */
void Settings_Init(LCUI_Settings settings)
{
	*settings = self;
}

/* Update global settings with the given input. */
void LCUI_ApplySettings(LCUI_Settings settings)
{
	self = *settings;
	self.frame_rate_cap = max(self.frame_rate_cap, 1);
	self.parallel_rendering_threads =
	    max(self.parallel_rendering_threads, 1);
	TriggerSettingsChangedEvent();
}

/* Reset global settings to their defaults. */
void LCUI_ResetSettings(void)
{
	self.frame_rate_cap = LCUI_MAX_FRAMES_PER_SEC;
	self.parallel_rendering_threads = 4;
	self.record_profile = FALSE;
	self.fps_meter = FALSE;
	self.paint_flashing = FALSE;
	TriggerSettingsChangedEvent();
}
