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

static FrameCtrlCtx surface_framectrl_ctx;
static LinkedList surface_list;

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
	surface->target = NULL;
	surface->mode = RENDER_MODE_BIT_BLT;
	surface->fb_hdc = NULL;
	surface->fb_bmp = NULL;
	Graph_Init( &surface->fb );
	surface->fb.color_type = COLOR_TYPE_ARGB;
	DirtyRectList_Init( &surface->rect );
	/* 创建窗口 */
	surface->hwnd = CreateWindow(
			TEXT("LCUI"), TEXT ("LCUI Surface"),
			WIN32_WINDOW_STYLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0,
			NULL, NULL, win32.main_instance, NULL);

	LinkedList_Insert( &surface_list, surface );
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

/** 设置 Surface 的渲染模式 */
int Surface_SetRenderMode( LCUI_Surface *surface, int mode )
{
	switch(mode) {
	case RENDER_MODE_BIT_BLT: 
	case RENDER_MODE_STRETCH_BLT:
		surface->mode = mode;
		break;
	default: return -1;
	}
	return 0;
}

/** 重绘Surface中的一块区域内的图像 */
int Surface_Paint( LCUI_Surface *surface, LCUI_Rect rect )
{
	LCUI_Graph graph;
	LCUI_GraphLayer *glayer;
	if( !surface->target ) {
		return -1;
	}
	glayer = Widget_GetGraphLayer(surface->target);
	Graph_Init( &graph );
	GraphLayer_GetGraph( glayer, &graph, rect );
	Graph_Replace( &surface->fb, &graph, Pos(0,0) );
	Graph_Free( &graph );
	return 0;
}

/** 处理Surface的无效区域 */
void Surface_ProcInvalidArea( LCUI_Surface *surface )
{

}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
void Surface_Present( LCUI_Surface *surface )
{
	HDC hdc_client;
	RECT client_rect;

	hdc_client = GetDC( surface->hwnd );
	SetBitmapBits( surface->fb_bmp, surface->fb.mem_size, surface->fb.bytes );
	switch(surface->mode) {
	case RENDER_MODE_STRETCH_BLT:
		GetClientRect( surface->hwnd, &client_rect );
		StretchBlt( hdc_client, 0, 0,
			client_rect.right, client_rect.bottom,
			surface->fb_hdc, 0, 0,
			surface->w, surface->h, SRCCOPY );
		break;
       case RENDER_MODE_BIT_BLT:
		BitBlt( hdc_client, 0, 0, surface->w, surface->h,
			surface->fb_hdc, 0, 0, SRCCOPY );
		break;
	default:break;
	}
	ValidateRect( surface->hwnd, NULL );
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
	/** 初始化 Surface 列表 */
	LinkedList_Init( &surface_list, sizeof(LCUI_Surface) );
	LinkedList_SetDataNeedFree( &surface_list, TRUE );
	LinkedList_SetDestroyFunc( &surface_list, (void (*)(void*))Surface_Delete );
	return 0;
}

/** Surface 处理线程 */
static void LCUISurface_ProcThread(void *unused)
{
	/*
	 * 处理列表中每个 Surface 的更新，包括映射的被部件的消息处理、无效区
	 * 域更新、Surface 内容的更新。
	 */
	int i, n;
	LCUI_Surface *surface;

	FrameControl_Init( &surface_framectrl_ctx );
	FrameControl_SetMaxFPS( &surface_framectrl_ctx, 100 );
	
	while(1) {
		LCUIWidget_ProcInvalidArea();
		LinkedList_Goto( &surface_list, 0 );
		n = LinkedList_GetTotal( &surface_list );
		for( i=0; i<n; ++i ) {
			surface = (LCUI_Surface*)LinkedList_Get( &surface_list );
			Surface_ProcInvalidArea( surface );
			Surface_Present( surface );
			LinkedList_ToNext( &surface_list );
		}
		FrameControl_Remain( &surface_framectrl_ctx );
	}
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
