/*
 * linux_events.c -- The event loop support for linux.
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
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

void LCUI_PreInitLinuxApp(void *data)
{
	return;
}

static void LinuxApp_ProcessEvents(void)
{
}

static int LinuxApp_BindSysEvent(int event_id, LCUI_EventFunc func, void *data,
				 void (*destroy_data)(void *))
{
	return -1;
}

static int LinuxApp_UnbindSysEvent(int event_id, LCUI_EventFunc func)
{
	return -1;
}

static int LinuxApp_UnbindSysEvent2(int handler_id)
{
	return -1;
}

static void *LinuxApp_GetData(void)
{
	return NULL;
}

LCUI_AppDriver LCUI_CreateLinuxAppDriver(void)
{
	LCUI_AppDriver driver = NULL;
	static LCUI_AppDriverRec dummy_driver = {
		.id = LCUI_APP_LINUX,
		.ProcessEvents = LinuxApp_ProcessEvents,
		.BindSysEvent = LinuxApp_BindSysEvent,
		.UnbindSysEvent = LinuxApp_UnbindSysEvent,
		.UnbindSysEvent2 = LinuxApp_UnbindSysEvent2,
		.GetData = LinuxApp_GetData
	};

#ifdef LCUI_VIDEO_DRIVER_X11
	driver = LCUI_CreateLinuxX11AppDriver();
#endif
	if (!driver) {
		driver = &dummy_driver;
	}
	return driver;
}

void LCUI_DestroyLinuxAppDriver(LCUI_AppDriver driver)
{
#ifdef LCUI_VIDEO_DRIVER_X11
	if (driver->id == LCUI_APP_LINUX_X11) {
		LCUI_DestroyLinuxX11AppDriver(driver);
	}
#endif
}

#endif
