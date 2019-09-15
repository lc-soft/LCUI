/*
 * platform.c -- cross-platform driver interface
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

#ifndef LCUI_PLATFORM_H
#define LCUI_PLATFORM_H

#ifdef _WIN32
	#ifdef WINAPI_FAMILY_APP
		#define LCUI_CreateAppDriver() NULL
		#define LCUI_DestroyAppDriver(X) NULL
		#define LCUI_PreInitApp() NULL
		#define LCUI_CreateDisplayDriver() NULL
		#define LCUI_DestroyDisplayDriver(X) NULL
		#define LCUI_InitMouseDriver()
		#define LCUI_FreeMouseDriver()
		#define LCUI_InitKeyboardDriver()
		#define LCUI_FreeKeyboardDriver()
	#else
		#define LCUI_CreateAppDriver LCUI_CreateWinAppDriver
		#define LCUI_DestroyAppDriver LCUI_DestroyWinAppDriver
		#define LCUI_PreInitApp LCUI_PreInitWinApp
		#define LCUI_CreateDisplayDriver LCUI_CreateWinDisplay
		#define LCUI_DestroyDisplayDriver LCUI_DestroyWinDisplay
		#define LCUI_InitMouseDriver LCUI_InitWinMouse
		#define LCUI_FreeMouseDriver LCUI_FreeWinMouse
		#define LCUI_InitKeyboardDriver LCUI_InitWinKeyboard
		#define LCUI_FreeKeyboardDriver LCUI_FreeWinKeyboard
	#endif
	#if defined(WINAPI_PARTITION_APP)
		#define LCUI_APP_H <LCUI/platform/windows/uwp.h>
	#else
		#define LCUI_EVENTS_H <LCUI/platform/windows/windows_events.h>
		#define LCUI_MOUSE_H <LCUI/platform/windows/windows_mouse.h>
		#define LCUI_KEYBOARD_H <LCUI/platform/windows/windows_keyboard.h>
		#define LCUI_DISPLAY_H <LCUI/platform/windows/windows_display.h>
	#endif
#else
	#undef linux
	#define LCUI_CreateAppDriver LCUI_CreateLinuxAppDriver
	#define LCUI_DestroyAppDriver LCUI_DestroyLinuxAppDriver
	#define LCUI_PreInitApp LCUI_PreInitLinuxApp
	#define LCUI_CreateDisplayDriver LCUI_CreateLinuxDisplayDriver
	#define LCUI_DestroyDisplayDriver LCUI_DestroyLinuxDisplayDriver
	#define LCUI_InitMouseDriver LCUI_InitLinuxMouse
	#define LCUI_FreeMouseDriver LCUI_FreeLinuxMouse
	#define LCUI_InitKeyboardDriver LCUI_InitLinuxKeyboard
	#define LCUI_FreeKeyboardDriver LCUI_FreeLinuxKeyboard
	#define LCUI_EVENTS_H <LCUI/platform/linux/linux_events.h>
	#define LCUI_MOUSE_H <LCUI/platform/linux/linux_mouse.h>
	#define LCUI_KEYBOARD_H <LCUI/platform/linux/linux_keyboard.h>
	#define LCUI_DISPLAY_H <LCUI/platform/linux/linux_display.h>
#endif

#endif
