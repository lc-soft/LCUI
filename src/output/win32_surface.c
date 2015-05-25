/* ***************************************************************************
 * win32_surface.c -- surface support for win32 platform.
 *
 * Copyright (C) 2014-2015 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * win32_surface.c -- win32平台的 surface 功能支持。
 *
 * 版权所有 (C) 2014-2015 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#define __IN_SURFACE_SOURCE_FILE__
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/thread.h>
#include <LCUI/widget_build.h>
#include <LCUI/display.h>
#include "resource.h"

#define WIN32_WINDOW_STYLE	(WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX)
#define MSG_SURFACE_CREATE	WM_USER+100

enum SurfaceTaskType {
	TASK_MOVE,
	TASK_RESIZE,
	TASK_SHOW,
	TASK_SET_CAPTION,
	TASK_TOTAL_NUM
};

typedef struct LCUI_SurfaceTask {
	LCUI_BOOL is_valid;
	union {
		struct {
			int x, y;
		};
		struct {
			int width, height;
		};
		LCUI_BOOL show;
		wchar_t *caption;
	};
} LCUI_SurfaceTask;

struct LCUI_SurfaceRec_ {
	HWND hwnd;
	int mode;
	int w, h;
	HDC fb_hdc;
	HBITMAP fb_bmp;
	LCUI_BOOL is_ready;
	LCUI_Graph fb;
	LCUI_SurfaceTask task_buffer[TASK_TOTAL_NUM];
};

static struct {
	HINSTANCE main_instance;	/**< 主程序的资源句柄 */
	HINSTANCE dll_instance;		/**< 动态库中的资源句柄 */
	LCUI_Thread loop_thread;	/**< 消息循环线程 */
	LCUI_Cond cond;			/**< 条件，当消息循环已创建时成立 */
	LCUI_BOOL is_ready;		/**< 消息循环线程是否已准备好 */
	LinkedList surfaces;		/**< surface 记录 */
	LCUI_SurfaceMethods methods;	/**< 方法集 */
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
static LCUI_Surface GetSurfaceByHWND( HWND hwnd )
{
	int i, n;
	LCUI_Surface surface;

	n = LinkedList_GetTotal( &win32.surfaces );
	for( i = 0; i < n; ++i ) {
		LinkedList_Goto( &win32.surfaces, i );
		surface = (LCUI_Surface)LinkedList_Get( &win32.surfaces );
		if( surface->hwnd == hwnd ) {
			return surface;
		}
	}
	return NULL;
}

static void Win32Surface_OnDestroy( void *args )
{
	LCUI_Surface surface = (LCUI_Surface)args;
	surface->w = 0;
	surface->h = 0;
	if( surface->hwnd ) {
		if( surface->fb_hdc ) {
			ReleaseDC( surface->hwnd, surface->fb_hdc );
		}
	}
	if( surface->fb_bmp ) {
		DeleteObject( surface->fb_bmp );
	}
	surface->fb_hdc = NULL;
	surface->fb_bmp = NULL;
	surface->hwnd = NULL;
	Graph_Free( &surface->fb );
}

static void Win32Surface_ExecDelete( LCUI_Surface surface )
{
	int i, n;
	LCUI_Surface s;

	n = LinkedList_GetTotal( &win32.surfaces );
	for( i = 0; i < n; ++i ) {
		LinkedList_Goto( &win32.surfaces, i );
		s = (LCUI_Surface)LinkedList_Get( &win32.surfaces );
		if( surface == s ) {
			LinkedList_Delete( &win32.surfaces );
			break;
		}
	}
}

/** 删除 Surface */
static void Win32Surface_Delete( LCUI_Surface surface )
{
	if( surface->hwnd ) {
		PostMessage( surface->hwnd, WM_CLOSE, 0, 0 );
	}
}

static LRESULT CALLBACK
WndProc( HWND hwnd, UINT msg, WPARAM arg1, LPARAM arg2 )
{
	LCUI_SystemEvent e;
	LCUI_Surface surface;

	e.type = LCUI_NONE;
	e.rel_x = e.rel_y = e.key_code = 0;
	e.type_name = NULL;

	surface = GetSurfaceByHWND(hwnd);
	DEBUG_MSG( "surface: %p, msg: %d\n", surface, msg );
	if( !surface ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	switch( msg ) {
	case WM_KEYDOWN:
		e.type = LCUI_KEYDOWN;
		e.key_code = arg1;
		break;
	case WM_KEYUP:
		e.type = LCUI_KEYUP;
		e.key_code = arg1;
		break;
	case WM_MOUSEMOVE: {
		POINT new_pos;
		GetCursorPos( &new_pos );
		ScreenToClient( hwnd, &new_pos );
		e.rel_x = new_pos.x;
		e.rel_y = new_pos.y;
		e.type = LCUI_MOUSEMOVE;
		break;
	}
	case WM_LBUTTONDOWN:
		e.type = LCUI_MOUSEDOWN;
		e.key_code = 1;
		break;
	case WM_LBUTTONUP:
		e.type = LCUI_MOUSEUP;
		e.key_code = 1;
		break;
	case WM_RBUTTONDOWN:
		e.type = LCUI_MOUSEDOWN;
		e.key_code = 2;
		break;
	case WM_RBUTTONUP:
		e.type = LCUI_MOUSEUP;
		e.key_code = 2;
		break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		LCUI_Rect area;

		BeginPaint( hwnd, &ps );
		/* 获取区域坐标及尺寸 */
		area.x = ps.rcPaint.left;
		area.y = ps.rcPaint.top;
		area.width = ps.rcPaint.right - area.x;
		area.height = ps.rcPaint.bottom - area.y;
		if( win32.methods.onInvalidRect ) {
			win32.methods.onInvalidRect( surface, &area );
		}
		EndPaint( hwnd, &ps );
		return 0;
	}
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		Win32Surface_ExecDelete( surface );
		break;
	case WM_SHOWWINDOW:
		if( arg1 ) {
			//Widget_Show( surface->target );
		} else {
			//Widget_Hide( surface->target );
		}
		break;
	case WM_ACTIVATE:
		break;
	default:break;
	}
	if( e.type != LCUI_NONE && win32.methods.onEvent ) {
		win32.methods.onEvent( surface, &e );
	}
	return DefWindowProc( hwnd, msg, arg1, arg2 );
}

/** 新建一个 Surface */
static LCUI_Surface Win32Surface_New(void)
{
	int i;
	LCUI_Surface surface;
	surface = (LCUI_Surface)LinkedList_Alloc( &win32.surfaces );
	surface->mode = RENDER_MODE_BIT_BLT;
	surface->hwnd = NULL;
	surface->fb_hdc = NULL;
	surface->fb_bmp = NULL;
	surface->is_ready = FALSE;
	Graph_Init( &surface->fb );
	surface->fb.color_type = COLOR_TYPE_ARGB;
	for( i=0; i<TASK_TOTAL_NUM; ++i ) {
		surface->task_buffer[i].is_valid = FALSE;
	}
	if( !win32.is_ready ) {
		/* 等待 Surface 线程创建完 windows 消息队列 */
		LCUICond_Wait( &win32.cond );
	}
	/* 让 Surface 线程去完成 windows 窗口的创建 */
	PostThreadMessage( win32.loop_thread, MSG_SURFACE_CREATE, 0, (LPARAM)surface );
	return surface;
}

static LCUI_BOOL Win32Surface_IsReady( LCUI_Surface surface )
{
	return surface->is_ready;
}

static void Win32Surface_ExecMove( LCUI_Surface surface, int x, int y )
{
	x += GetSystemMetrics( SM_CXFIXEDFRAME );
	y += GetSystemMetrics( SM_CYFIXEDFRAME );
	SetWindowPos( surface->hwnd, HWND_NOTOPMOST,
		x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	return;
}

static void Win32Surface_Move( LCUI_Surface surface, int x, int y )
{
	/* 缓存任务，等获得窗口句柄后处理 */
	surface->task_buffer[TASK_MOVE].x = x;
	surface->task_buffer[TASK_MOVE].y = y;
	surface->task_buffer[TASK_MOVE].is_valid = TRUE;
}

static void Win32Surface_ExecResize( LCUI_Surface surface, int w, int h )
{
	HDC hdc_client;
	HBITMAP old_bmp;

	surface->w = w;
	surface->h = h;
	_DEBUG_MSG("w = %d, h = %d\n", w, h);
	Graph_Create( &surface->fb, w, h );
	hdc_client = GetDC( surface->hwnd );
	surface->fb_bmp = CreateCompatibleBitmap( hdc_client, w, h );
	old_bmp = (HBITMAP)SelectObject( surface->fb_hdc, surface->fb_bmp );
	if( old_bmp ) {
		DeleteObject( old_bmp );
	}
	/* 加上窗口边框的尺寸 */
	w += GetSystemMetrics( SM_CXFIXEDFRAME ) * 2;
	h += GetSystemMetrics( SM_CYFIXEDFRAME ) * 2;
	h += GetSystemMetrics( SM_CYCAPTION );
	//SetWindowLong( surface->hwnd, GWL_STYLE, WIN32_WINDOW_STYLE );
	SetWindowPos( surface->hwnd, HWND_NOTOPMOST,
		      0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER );
}

static void Win32Surface_Resize( LCUI_Surface surface, int w, int h )
{
	surface->task_buffer[TASK_RESIZE].width = w;
	surface->task_buffer[TASK_RESIZE].height = h;
	surface->task_buffer[TASK_RESIZE].is_valid = TRUE;
}

static void Win32Surface_Show( LCUI_Surface surface )
{
	DEBUG_MSG("surface: %p, buffer show.\n", surface);
	surface->task_buffer[TASK_SHOW].show = TRUE;
	surface->task_buffer[TASK_SHOW].is_valid = TRUE;
}

static void Win32Surface_Hide( LCUI_Surface surface )
{
	surface->task_buffer[TASK_SHOW].show = FALSE;
	surface->task_buffer[TASK_SHOW].is_valid = TRUE;
}

static void Win32Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *str )
{
	int len;
	wchar_t *caption;

	len = wcslen(str) + 1;
	caption = (wchar_t*)malloc(sizeof(wchar_t)*len);
	wcsncpy( caption, str, len );

	if( surface->task_buffer[TASK_SET_CAPTION].is_valid
	 && surface->task_buffer[TASK_SET_CAPTION].caption ) {
		free( surface->task_buffer[TASK_SET_CAPTION].caption );
	}
	surface->task_buffer[TASK_SET_CAPTION].caption = caption;
	surface->task_buffer[TASK_SET_CAPTION].is_valid = TRUE;
}

void Win32Surface_SetOpacity( LCUI_Surface surface, float opacity )
{

}

/** 设置 Surface 的渲染模式 */
static void Win32Surface_SetRenderMode( LCUI_Surface surface, int mode )
{
	surface->mode = mode;
}

/**
 * 准备绘制 Surface 中的内容
 * @param[in] surface	目标 surface
 * @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
 * @return		返回绘制上下文句柄
 */
static LCUI_PaintContext Win32Surface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	LCUI_PaintContext paint;
	paint = (LCUI_PaintContext)malloc(sizeof(LCUI_PaintContextRec_));
	Graph_Init( &paint->canvas );
	Graph_Quote( &paint->canvas, &surface->fb, rect );
	paint->rect = *rect;
	return paint;
}

/**
 * 结束对 Surface 的绘制操作
 * @param[in] surface	目标 surface
 * @param[in] paint_ctx	绘制上下文句柄
 */
static void Win32Surface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx )
{
	free( paint_ctx );
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
static void Win32Surface_Present( LCUI_Surface surface )
{
	HDC hdc_client;
	RECT client_rect;

	_DEBUG_MSG("surface: %p, hwnd: %p\n", surface, surface->hwnd);
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
       default:
		BitBlt( hdc_client, 0, 0, surface->w, surface->h,
			surface->fb_hdc, 0, 0, SRCCOPY );
		break;
	}
	ValidateRect( surface->hwnd, NULL );
}

/** 更新 surface，应用缓存的变更 */
static void Win32Surface_Update( LCUI_Surface surface )
{
	LCUI_SurfaceTask *t;

	if( !surface->hwnd ) {
		return;
	}
	DEBUG_MSG("surface: %p\n", surface);
	t = &surface->task_buffer[TASK_MOVE];
	if( t->is_valid ) {
		Win32Surface_ExecMove( surface, t->x, t->y );
		t->is_valid = FALSE;
	}

	t = &surface->task_buffer[TASK_RESIZE];
	if( t->is_valid ) {
		Win32Surface_ExecResize( surface, t->width, t->height );
		t->is_valid = FALSE;
	}

	t = &surface->task_buffer[TASK_SET_CAPTION];
	if( t->is_valid ) {
		SetWindowText( surface->hwnd, t->caption );
		if( t->caption ) {
			free( t->caption );
			t->caption = NULL;
		}
	}
	t->is_valid = FALSE;

	t = &surface->task_buffer[TASK_SHOW];
	DEBUG_MSG("surface: %p, hwnd: %p, is_valid: %d, show: %d\n",
	 surface, surface->hwnd, t->is_valid, t->show);
	if( t->is_valid ) {
		if( t->show ) {
			ShowWindow( surface->hwnd, SW_SHOWNORMAL );
		} else {
			ShowWindow( surface->hwnd, SW_HIDE );
		}
	}
	t->is_valid = FALSE;
}

static void LCUISurface_Loop( void *args )
{
	MSG msg;
	LCUI_Surface surface;
	DEBUG_MSG("start\n");
	/* 创建消息队列 */
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	win32.is_ready = TRUE;
	LCUICond_Broadcast( &win32.cond );
	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		if( msg.message == MSG_SURFACE_CREATE ) {
			HDC hdc_client;
			surface = (LCUI_Surface)msg.lParam;
			/* 为 Surface 创建窗口 */
			surface->hwnd = CreateWindow(
				TEXT("LCUI"), TEXT("LCUI Surface"),
				WIN32_WINDOW_STYLE,
				CW_USEDEFAULT, CW_USEDEFAULT,
				0, 0, NULL, NULL,
				win32.main_instance, NULL
			);
			hdc_client = GetDC( surface->hwnd );
			surface->fb_hdc = CreateCompatibleDC( hdc_client );
			surface->is_ready = TRUE;
			_DEBUG_MSG("surface: %p, surface->hwnd: %p\n", surface, surface->hwnd);
			continue;
		}
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	DEBUG_MSG("quit\n");
	LCUIThread_Exit(NULL);
}

/** 初始化适用于 Win32 平台的 surface 支持 */
LCUI_SurfaceMethods *LCUIDisplay_InitWin32( LCUI_DisplayInfo *info )
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
	wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;

	if( !RegisterClass(&wndclass) ) {
		wchar_t str[256];
		wsprintf(str, L"LCUISurface_Init(): error code: %d\n", GetLastError());
		MessageBox( NULL, str, szAppName, MB_ICONERROR );
		return NULL;
	}
	info->width = GetSystemMetrics(SM_CXSCREEN);
	info->height = GetSystemMetrics(SM_CYSCREEN);
	win32.is_ready = FALSE;
	strncpy( win32.methods.name, "win32", 32 );
	win32.methods.new = Win32Surface_New;
	win32.methods.delete = Win32Surface_Delete;
	win32.methods.isReady = Win32Surface_IsReady;
	win32.methods.show = Win32Surface_Show;
	win32.methods.hide = Win32Surface_Hide;
	win32.methods.move = Win32Surface_Move;
	win32.methods.resize = Win32Surface_Resize;
	win32.methods.update = Win32Surface_Update;
	win32.methods.present = Win32Surface_Present;
	win32.methods.setCaptionW = Win32Surface_SetCaptionW;
	win32.methods.setRenderMode = Win32Surface_SetRenderMode;
	win32.methods.setOpacity = Win32Surface_SetOpacity;
	win32.methods.beginPaint = Win32Surface_BeginPaint;
	win32.methods.endPaint = Win32Surface_EndPaint;
	win32.methods.onInvalidRect = NULL;
	win32.methods.onEvent = NULL;
	LCUICond_Init( &win32.cond );
	LinkedList_Init( &win32.surfaces, sizeof( struct LCUI_SurfaceRec_ ) );
	LinkedList_SetDataMemReuse( &win32.surfaces, TRUE );
	LinkedList_SetDataNeedFree( &win32.surfaces, TRUE );
	LinkedList_SetDestroyFunc( &win32.surfaces, Win32Surface_OnDestroy );
	LCUIThread_Create( &win32.loop_thread, LCUISurface_Loop, NULL );
	return &win32.methods;
}

void LCUIDisplay_ExitWin32(void)
{

}

#endif
