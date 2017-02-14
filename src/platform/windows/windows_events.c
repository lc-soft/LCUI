/* ***************************************************************************
 * windows_events.c -- app events support for windows platform.
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * windows_events.c -- windows 平台事件驱动支持。
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

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
	HWND main_hwnd;
	HINSTANCE main_instance;	/**< 主程序的资源句柄 */
	HINSTANCE dll_instance;		/**< 动态库中的资源句柄 */
	LCUI_EventTrigger trigger;
} win;

static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg,
				 WPARAM arg1, LPARAM arg2 )
{
	MSG win_ev;
	switch( msg ) {
	case WM_LCUI_TASK:
		LCUI_RunTask( (LCUI_AppTask)arg2 );
		LCUI_DeleteTask( (LCUI_AppTask)arg2 );
		return 0;
	case WM_CLOSE:
		LCUI_Quit();
	default:break;
	}
	win_ev.hwnd = hwnd;
	win_ev.wParam = arg1;
	win_ev.lParam = arg2;
	win_ev.message = msg;
	if( EventTrigger_Trigger( win.trigger, msg, &win_ev ) == 0 ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	return 0;
}

static LCUI_BOOL WIN_PostTask( LCUI_AppTask task )
{
	return PostMessage( win.main_hwnd, WM_LCUI_TASK, 0, (LPARAM)task );
}

static LCUI_BOOL WIN_WaitEvent( void )
{
	MSG msg;
	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
		return TRUE;
	}
	return WaitMessage();
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
	return EventTrigger_Bind( win.trigger, event_id, func, data, destroy_data );
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
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT( "LCUI" );
	ASSIGN( app, LCUI_AppDriver );

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.hInstance = win.main_instance;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hIcon = LoadIcon( win.dll_instance,
				   MAKEINTRESOURCE( IDI_LCUI_ICON ) );
	if( !RegisterClass( &wndclass ) ) {
		wchar_t str[256];
		wsprintf( str, L"LCUI_CreateWinAppDriver(): error code: %d\n", GetLastError() );
		MessageBox( NULL, str, szAppName, MB_ICONERROR );
		return NULL;
	}
	app->GetData = WIN_GetData;
	app->PostTask = WIN_PostTask;
	app->WaitEvent = WIN_WaitEvent;
	app->ProcessEvents = WIN_ProcessEvents;
	app->BindSysEvent = WIN_BindSysEvent;
	app->UnbindSysEvent = WIN_UnbindSysEvent;
	app->UnbindSysEvent2 = WIN_UnbindSysEvent2;
	win.trigger = EventTrigger();
	return app;
}

void LCUI_DestroyWinAppDriver( LCUI_AppDriver app )
{
	EventTrigger_Destroy( win.trigger );
	free( app );
}

#endif
