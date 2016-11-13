
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_WIN32
#include <windows.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include "resource.h"

#define WM_LCUI_EVENT (WM_USER+20)

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
	case WM_LCUI_EVENT:
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
	return PostMessage( win.main_hwnd, WM_LCUI_EVENT, 0, (LPARAM)task );
}

static LCUI_BOOL WIN_WaitEvent( void )
{
	MSG msg;
	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
		return TRUE;
	}
	return WaitMessage();
}

static void WIN_DispatchEvent( void )
{
	MSG msg;
	if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
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

int LCUI_InitWinApp( LCUI_AppDriver app )
{
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT ("LCUI");

	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.hInstance     = win.main_instance;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hIcon         = LoadIcon( win.dll_instance, 
					   MAKEINTRESOURCE(IDI_LCUI_ICON) );

	if( !RegisterClass(&wndclass) ) {
		wchar_t str[256];
		wsprintf(str, L"LCUI_InitWinApp(): error code: %d\n", GetLastError());
		MessageBox( NULL, str, szAppName, MB_ICONERROR );
		return -1;
	}
	app->GetData = WIN_GetData;
	app->PostTask = WIN_PostTask;
	app->WaitEvent = WIN_WaitEvent;
	app->DispatchEvent = WIN_DispatchEvent;
	app->BindSysEvent = WIN_BindSysEvent;
	app->UnbindSysEvent = WIN_UnbindSysEvent;
	app->UnbindSysEvent2 = WIN_UnbindSysEvent2;
	win.trigger = EventTrigger();
	return 0;
}

#endif
