/**
 * 
 */
#include <LCUI_Build.h>

#ifdef LCUI_BUILD_IN_WIN32
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_SURFACE_H
#include "resource.h"

#define WIN32_WINDOW_STYLE (WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX)

static struct {
	HINSTANCE main_instance;	/**< 主程序的资源句柄 */
	HINSTANCE dll_instance;		/**< 动态库中的资源句柄 */
} win32;

void Win32_LCUI_Init( HINSTANCE hInstance )
{
	win32.main_instance = hInstance;
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
	win32.dll_instance = hModule;
	return TRUE;
}

/** 根据 hwnd 获取 Surface */
static LCUI_Surface *getSurfaceByHWND( HWND hwnd )
{
	return NULL;
}

static LRESULT CALLBACK 
WndProc( HWND hwnd, UINT msg, WPARAM arg1, LPARAM arg2 )
{
	LCUI_Surface *surface;
	surface = getSurfaceByHWND( hwnd);
	if( !surface ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	switch(msg) {
	case WM_SETFOCUS:
		break;
	case WM_KILLFOCUS:
		break;
	case WM_CLOSE:
		// 解除映射，并销毁部件和Surface
		break;
	case WM_SHOWWINDOW:
		if( arg1 ) {
			Widget_Show( surface->target );
		} else {
			Widget_Hide( surface->target );
		}
		break;
	case WM_ACTIVATE:
		break;
	default:break;
	}
	return DefWindowProc( hwnd, msg, arg1, arg2 );
}

/** 新建一个 Surface */
LCUI_Surface *Surface_New(void)
{
	LCUI_Surface *surface;
	surface = (LCUI_Surface*)malloc(sizeof(LCUI_Surface));
	DirtyRectList_Init( &surface->rect );
	surface->target = NULL;
	/* 创建窗口 */
	surface->hwnd = CreateWindow(
			TEXT("LCUI"), TEXT ("LCUI Surface"),
			WIN32_WINDOW_STYLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0,
			NULL, NULL, win32.main_instance, NULL);
	return surface;
}

/** 删除 Surface */
void Surface_Delete( LCUI_Surface *surface )
{

}

void Surface_Move( LCUI_Surface *surface, int x, int y )
{
	x += GetSystemMetrics(SM_CXFIXEDFRAME);
	y += GetSystemMetrics(SM_CYFIXEDFRAME);
	SetWindowPos( surface->hwnd, HWND_NOTOPMOST, x, y, 0, 0,
			SWP_NOSIZE|SWP_NOZORDER );
}

void Surface_Resize( LCUI_Surface *surface, int w, int h )
{
	/* 加上窗口边框的尺寸 */
	w += GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	h += GetSystemMetrics(SM_CYFIXEDFRAME)*2;
	h += GetSystemMetrics(SM_CYCAPTION);
	SetWindowLong( surface->hwnd, GWL_STYLE, WIN32_WINDOW_STYLE );
	/* 调整窗口尺寸 */
	SetWindowPos( surface->hwnd, HWND_NOTOPMOST, 0, 0, w, h,
			SWP_NOMOVE|SWP_NOZORDER );
}

void Surface_Show( LCUI_Surface *surface )
{
	ShowWindow( surface->hwnd, SW_SHOWNORMAL );
}

void Surface_Hide( LCUI_Surface *surface )
{
	ShowWindow( surface->hwnd, SW_HIDE );
}

void Surface_SetCaptionW( LCUI_Surface *surface, const wchar_t *str )
{
	SetWindowText( surface->hwnd, str );
}

/** 将指定部件映射至 Surface 上 */
void Surface_MapWidget( LCUI_Surface *surface, LCUI_Widget *widget )
{
	/**
	 * 关联部件相关事件，以在部件变化时让 surface 做相应变化
	 */
}

void Surface_UnmapWidget( LCUI_Surface *surface )
{

}

int LCUISurface_Init(void)
{
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT ("LCUI");

	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = win32.main_instance;
	/* 载入动态库里的图标 */
	wndclass.hIcon         = LoadIcon( win32.dll_instance, MAKEINTRESOURCE(IDI_MAIN_ICON) );
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;
	
	if( !RegisterClass(&wndclass) ) {
		wchar_t str[256];
		wsprintf(str, L"LCUISurface_Init(): error code: %d\n", GetLastError());
		MessageBox( NULL, str, szAppName, MB_ICONERROR );
		return -1;
	}
	return 0;
}

void LCUISurface_Loop(void)
{
	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}
#endif