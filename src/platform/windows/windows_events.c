
#include <windows.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include "resource.h"

#define WM_LCUI_EVENT (WM_USER+1234)

static struct WindowsDriver {
	DWORD main_tid;
	HINSTANCE main_instance;	/**< 主程序的资源句柄 */
	HINSTANCE dll_instance;		/**< 动态库中的资源句柄 */
	LCUI_EventTrigger trigger;
} win;

static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, 
				 WPARAM arg1, LPARAM arg2 )
{
	WIN_SysEventRec win_event;
	win_event.msg = msg;
	win_event.hwnd = hwnd;
	win_event.wparam = arg1;
	win_event.lparam = arg2;
	win_event.data = NULL;
	switch( msg ) {
	case WM_LCUI_EVENT:
		return 0;
	case WM_CLOSE:
		LCUI_Quit();
	default:break;
	}
	if( EventTrigger_Trigger( win.trigger, msg, &win_event ) == 0 ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	return 0;
}

static void WIN_BreakEventWaiting( void )
{
	PostThreadMessage( win.main_tid, WM_LCUI_EVENT, 0, 0 );
}

static LCUI_BOOL WIN_WaitEvent( void )
{
	return WaitMessage();
}

static void WIN_PumpEvents( void )
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

int LCUI_InitWinApp( LCUI_AppDriver app )
{
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT ("LCUI");

	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = win.main_instance;
	/* 载入动态库里的图标 */
	wndclass.hIcon         = LoadIcon( win.dll_instance, MAKEINTRESOURCE(IDI_LCUI_ICON) );
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;

	if( !RegisterClass(&wndclass) ) {
		wchar_t str[256];
		wsprintf(str, L"LCUI_InitApp(): error code: %d\n", GetLastError());
		MessageBox( NULL, str, szAppName, MB_ICONERROR );
		return -1;
	}
	app->PumbEvents = WIN_PumpEvents;
	app->BreakEventWaiting = WIN_BreakEventWaiting;
	app->BindSysEvent = WIN_BindSysEvent;
	app->UnbindSysEvent = WIN_UnbindSysEvent;
	app->UnbindSysEvent2 = WIN_UnbindSysEvent2;
	app->GetData = WIN_GetData;
	win.main_tid = GetCurrentThreadId();
	win.trigger = EventTrigger();
	return 0;
}
