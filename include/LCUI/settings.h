/*
 * settings.h -- Functions for accessing and modifying global settings.
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

#ifndef LCUI_SETTINGS_H
#define LCUI_SETTINGS_H

LCUI_BEGIN_HEADER

typedef struct LCUI_SettingsRec_ {
	int frame_rate_cap;
	int parallel_rendering_threads;
	LCUI_BOOL record_profile;
	LCUI_BOOL fps_meter;
	LCUI_BOOL paint_flashing;
} LCUI_SettingsRec, *LCUI_Settings;

/* Initialize settings with the current global settings. */
LCUI_API void Settings_Init(LCUI_Settings settings);

/* Update global settings with the given input. */
LCUI_API void LCUI_ApplySettings(LCUI_Settings settings);

/* Reset global settings to their defaults. */
LCUI_API void LCUI_ResetSettings(void);

LCUI_END_HEADER

#endif
