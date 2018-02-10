/*
 * windows_events.c -- App events support for windows platform.
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
#ifdef LCUI_BUILD_IN_WIN32
#include <windows.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include "resource.h"

#define WM_LCUI_TASK (WM_USER+20)

static struct WindowsDriver {
	LCUI_BOOL active;
	HWND main_hwnd;
	HINSTANCE main_instance;	/**< 主程序的资源句柄 */
	HINSTANCE dll_instance;		/**< 动态库中的资源句柄 */
	LCUI_EventTrigger trigger;
	const wchar_t *class_name;
} win;

static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg,
				 WPARAM arg1, LPARAM arg2 )
{
	MSG win_ev;
	LCUI_Surface surface;

	switch( msg ) {
	case WM_LCUI_TASK:
		LCUITask_Run( (LCUI_Task)arg2 );
		LCUITask_Destroy( (LCUI_Task)arg2 );
		return 0;
	case WM_DESTROY:
		return 0;
	case WM_CLOSE:
		surface = LCUIDisplay_GetSurfaceByHandle( hwnd );
		Surface_Destroy( surface );
		LCUI_Quit();
	default:break;
	}
	win_ev.hwnd = hwnd;
	win_ev.wParam = arg1;
	win_ev.lParam = arg2;
	win_ev.message = msg;
	if( !win.active ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	if( EventTrigger_Trigger( win.trigger, msg, &win_ev ) == 0 ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	return 0;
}

static void WIN_ProcessEvents( void )
{
	MSG msg;
	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

static int WIN_BindSysEvent( int event_id, LCUI_EventFunc func,
			     void *data, void(*destroy_data)(void*) )
{
	return EventTrigger_Bind( win.trigger, event_id, func,
				  data, destroy_data );
}

static int WIN_UnbindSysEvent( int event_id, LCUI_EventFunc func )
{
	return EventTrigger_Unbind( win.trigger, event_id, func );
}

static int WIN_UnbindSysEvent2( int handler_id )
{
	return EventTrigger_Unbind2( win.trigger, handler_id );
}

static void *WIN_GetData( void )
{
	return win.main_instance;
}

void LCUI_SetMainWindow( HWND hwnd )
{
	win.main_hwnd = hwnd;
}

/** win32的动态库的入口函数 */
BOOL APIENTRY DllMain( HMODULE hModule, DWORD reason, LPVOID unused )
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	win.dll_instance = hModule;
	return TRUE;
}

void LCUI_PreInitWinApp( void *data )
{
	win.main_instance = data;
}

LCUI_AppDriver LCUI_CreateWinAppDriver( void )
{
	WNDCLASSW wndclass;
	ASSIGN( app, LCUI_AppDriver );

	win.class_name = L"LCUI";
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = win.class_name;
	wndclass.hInstance = win.main_instance;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hIcon = LoadIcon( win.dll_instance,
				   MAKEINTRESOURCE( IDI_LCUI_ICON ) );
	if( !RegisterClassW( &wndclass ) ) {
		wchar_t str[256];
		swprintf( str, 255, __FUNCTIONW__
			  L": error code: %d\n", GetLastError() );
		MessageBoxW( NULL, str, win.class_name, MB_ICONERROR );
		return NULL;
	}
	app->GetData = WIN_GetData;
	app->ProcessEvents = WIN_ProcessEvents;
	app->BindSysEvent = WIN_BindSysEvent;
	app->UnbindSysEvent = WIN_UnbindSysEvent;
	app->UnbindSysEvent2 = WIN_UnbindSysEvent2;
	win.trigger = EventTrigger();
	win.active = TRUE;
	return app;
}

void LCUI_DestroyWinAppDriver( LCUI_AppDriver app )
{
	win.active = FALSE;
	UnregisterClassW( win.class_name, win.main_instance );
	EventTrigger_Destroy( win.trigger );
	free( app );
}

#endif
