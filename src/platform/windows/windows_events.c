
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
	LCUI_AppTask task;
	WIN_SysEventRec win_event;
	switch( msg ) {
	case WM_LCUI_EVENT:
		task = (LCUI_AppTask)arg2;
		LCUI_RunTask( task );
		LCUI_DeleteTask( task );
		return 0;
	case WM_CLOSE:
		LCUI_Quit();
	default:break;
	}
	win_event.msg = msg;
	win_event.data = NULL;
	win_event.hwnd = hwnd;
	win_event.wparam = arg1;
	win_event.lparam = arg2;
	if( EventTrigger_Trigger( win.trigger, msg, &win_event ) == 0 ) {
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

	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.hInstance     = win.main_instance;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hIcon         = LoadIcon( win.dll_instance, MAKEINTRESOURCE(IDI_LCUI_ICON) );

	if( !RegisterClass(&wndclass) ) {
		wchar_t str[256];
		wsprintf(str, L"LCUI_InitApp(): error code: %d\n", GetLastError());
		MessageBox( NULL, str, szAppName, MB_ICONERROR );
		return -1;
	}
	app->GetData = WIN_GetData;
	app->PostTask = WIN_PostTask;
	app->WaitEvent = WIN_WaitEvent;
	app->PumbEvents = WIN_PumpEvents;
	app->BindSysEvent = WIN_BindSysEvent;
	app->UnbindSysEvent = WIN_UnbindSysEvent;
	app->UnbindSysEvent2 = WIN_UnbindSysEvent2;
	win.trigger = EventTrigger();
	/**
	 * 创建一个隐藏的主窗体，用于接收 LCUI 的任务
	 * 之前用 PostThreadMessage() 无法发送自定义消息到主线程的消息循环，因此
	 * 改成创建隐藏窗体兵用 PostMessage() 发送任务。
	 */
	win.main_hwnd = CreateWindow(
		TEXT("LCUI"), TEXT("LCUI Task Receiver"), 0, 
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL,
		win.main_instance, NULL
	);
	return 0;
}

#endif
