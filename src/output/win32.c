/* ***************************************************************************
 * win32.c -- win32 platform support for graphical output
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * win32.c -- win32平台上的图形输出支持
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
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
#include LC_LCUI_H

#ifdef LCUI_VIDEO_DRIVER_WIN32

#include LC_GRAPH_H
#include LC_INPUT_H
#include LC_CURSOR_H
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include <Windows.h>
#include "resource.h"

#define WIN32_WINDOW_STYLE (WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX)

static HWND current_hwnd = NULL;
static int pixel_mem_len = 0;
static unsigned char *pixel_mem = NULL;
static HDC hdc_client, hdc_framebuffer;
static HBITMAP client_bitmap;
static HINSTANCE win32_hInstance = NULL, dll_hInstance = NULL;
static LCUI_Mutex screen_mutex;
static LCUI_Thread th_win32;
static LCUI_Sleeper win32_init_sleeper;
static LCUI_BOOL win32_init_error = TRUE;

LCUI_API void Win32_LCUI_Init( HINSTANCE hInstance )
{
	win32_hInstance = hInstance;
}

LCUI_API HWND Win32_GetSelfHWND( void )
{
	return current_hwnd;
}

LCUI_API void Win32_SetSelfHWND( HWND hwnd )
{
	current_hwnd = hwnd;
}

/** win32的动态库的入口函数 */
BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call,
                       LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	dll_hInstance = hModule;
	return TRUE;
}

static LRESULT CALLBACK Win32_LCUI_WndProc( HWND hwnd, UINT message, 
					   WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	LCUI_Rect area;

	switch (message) {
	case WM_KEYDOWN:
		LCUIKeyboard_HitKey( wParam );
		return 0;
	case WM_KEYUP:
		LCUIKeyboard_FreeKey( wParam );
		return 0;
	case WM_RBUTTONDOWN:
		LCUIMouse_ButtonDown( LCUIKEY_RIGHTBUTTON );
		return 0;
	case WM_RBUTTONUP:
		LCUIMouse_ButtonUp( LCUIKEY_RIGHTBUTTON );
		return 0;
	case WM_LBUTTONDOWN:
		LCUIMouse_ButtonDown( LCUIKEY_LEFTBUTTON );
		return 0;
	case WM_LBUTTONUP:
		LCUIMouse_ButtonUp( LCUIKEY_LEFTBUTTON );
		return 0;
	case WM_PAINT:
		BeginPaint( hwnd, &ps );
		/* 获取区域坐标及尺寸 */
		area.x = ps.rcPaint.left;
		area.y = ps.rcPaint.top;
		area.width = ps.rcPaint.right - area.x;
		area.height = ps.rcPaint.bottom - area.y;
		/* 记录该无效区域 */
		LCUIScreen_InvalidateArea( &area );
		EndPaint( hwnd, &ps );
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		LCUI_Quit();
		return 0;
	default:break;
	}
	return DefWindowProc( hwnd, message, wParam, lParam );
}

static int Win32_ScreenInit(void)
{
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT ("LCUI OutPut");

	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = Win32_LCUI_WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = win32_hInstance;
	/* 载入动态库里的图标 */
	wndclass.hIcon         = LoadIcon( dll_hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) );
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;
	
	if( !RegisterClass(&wndclass) ) {
		MessageBox( NULL, TEXT("This program requires Windows NT!" ),
		szAppName, MB_ICONERROR );
		return -1;
	}
	/* 创建窗口 */
	current_hwnd = CreateWindow(
			szAppName, TEXT ("LCUI Application"),
			WIN32_WINDOW_STYLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0,
			NULL, NULL, win32_hInstance, NULL);
	return 0;
}

/** Win32的消息循环线程 */
static void LCUI_Win32_Thread( void *arg )
{
	MSG msg;
	if( Win32_ScreenInit() == -1 ) {
		/* 设置标志为TRUE，表示初始化失败 */
		win32_init_error = TRUE;
		/* 打断处于睡眠状态的线程的睡眠 */
		LCUISleeper_BreakSleep( &win32_init_sleeper );
		LCUIThread_Exit(NULL);
		return;
	}
	/* 设置标志为FALSE，表示初始化成功 */
	win32_init_error = FALSE;
	/* 隐藏windows的鼠标游标 */
	ShowCursor( FALSE );
	LCUISleeper_BreakSleep( &win32_init_sleeper );
	while( LCUI_Sys.state == ACTIVE ) {
		/* 获取消息 */
		if( GetMessage( &msg, Win32_GetSelfHWND(), 0, 0 ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	LCUIThread_Exit(NULL);
}

/** 初始化屏幕 */
LCUI_API int LCUIScreen_Init( int w, int h, int mode )
{
	LCUI_Widget *root_widget;
	LCUI_Screen screen_info;

	/* 初始化屏幕互斥锁 */
	LCUIMutex_Init( &screen_mutex );
	LCUISleeper_Create( &win32_init_sleeper );
	/* 创建线程 */
	LCUIThread_Create( &th_win32, LCUI_Win32_Thread, NULL );
	/* 进行睡眠，最长时间为5秒 */
	LCUISleeper_StartSleep( &win32_init_sleeper, 5000 );
	/* 若初始化出现错误 */
	if( win32_init_error ) {
		return -1;
	}
	/* 准备屏幕相关信息 */
	screen_info.bits = 32;
	screen_info.mode = mode;
	strcpy( screen_info.dev_name, "win32 GDI" );
	/* 设置屏幕信息 */
	LCUIScreen_SetInfo( &screen_info );
	/* 设置图形输出模式 */
	LCUIScreen_SetMode( w, h, mode );
	/* 获取屏幕信息 */
	LCUIScreen_GetInfo( &screen_info );

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);
	pixel_mem_len = w*h*4;

	/* 分配内存，储存像素数据 */ 
	pixel_mem = (uchar_t*)malloc( pixel_mem_len );
	/* 获取客户区的DC */
	hdc_client = GetDC( current_hwnd );
	/* 为帧缓冲创建一个DC */
	hdc_framebuffer = CreateCompatibleDC( hdc_client );
	/* 为客户区创建一个Bitmap */ 
	client_bitmap = CreateCompatibleBitmap( hdc_client, w, h );
	/* 为帧缓冲的DC选择client_bitmap作为对象 */
	SelectObject( hdc_framebuffer, client_bitmap );
	
	root_widget = RootWidget_GetSelf();
	Widget_Resize( root_widget, screen_info.size );
	Widget_SetBackgroundColor( root_widget, RGB(255,255,255) );
	Widget_SetBackgroundTransparent( root_widget, FALSE );
	Widget_Show( root_widget );

	UpdateWindow( current_hwnd );
	/* 显示窗口 */
	ShowWindow( current_hwnd, SW_SHOWNORMAL );
	/* 前置并激活窗口 */
	SetForegroundWindow( current_hwnd );
	return 0;
}

LCUI_API void LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

LCUI_API int LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}

/* 设置视频输出模式 */
LCUI_API int LCUIScreen_SetMode( int w, int h, int mode )
{
	LCUI_Pos pos;
	LCUI_Size real_size;
	LCUI_Screen screen_info;
	LCUI_Widget *root_widget;
	
	LCUIMutex_Lock( &screen_mutex );
	LCUIScreen_GetInfo( &screen_info );
	real_size.w = GetSystemMetrics(SM_CXSCREEN);
	real_size.h = GetSystemMetrics(SM_CYSCREEN);
	if( mode == LCUI_INIT_MODE_AUTO ) {
		if( w == 0 && h == 0 ) {
			mode = LCUI_INIT_MODE_FULLSCREEN;
		} else {
			mode = LCUI_INIT_MODE_WINDOW;
		}
	}
	if( mode == LCUI_INIT_MODE_FULLSCREEN ) {
		if( w == 0 ) {
			w = real_size.w;
		}
		if( h == 0 ) {
			h = real_size.h;
		}
		SetWindowLong( current_hwnd, GWL_STYLE, WS_POPUP );
		SetWindowPos(	current_hwnd, HWND_NOTOPMOST, 0, 0, 
				real_size.w, real_size.h, SWP_SHOWWINDOW );
	} else {
		RECT rect;
		int boader_w, boader_h;
		/* 计算边框的尺寸 */
		boader_w = GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
		boader_h = GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
		boader_h += GetSystemMetrics(SM_CYCAPTION);
		/* 计算窗口尺寸 */
		w = w + boader_w;
		h = h + boader_h;
		pos.x = (real_size.w - w)/2;
		pos.y = (real_size.h - h)/2;
		
		SetWindowLong(	current_hwnd, GWL_STYLE, WIN32_WINDOW_STYLE );
		/* 调整窗口尺寸 */
		SetWindowPos(	current_hwnd, HWND_NOTOPMOST, 
				pos.x, pos.y, w, h, SWP_SHOWWINDOW );
		/* 获取客户区的尺寸 */
		GetClientRect( current_hwnd, &rect );
		w = rect.right;
		h = rect.bottom;
	}
	screen_info.size.w = w;
	screen_info.size.h = h;
	screen_info.mode = mode;
	LCUIScreen_SetInfo( &screen_info );
	root_widget = RootWidget_GetSelf();
	Widget_Resize( root_widget, screen_info.size );

	LCUIMutex_Unlock( &screen_mutex );
	return 0;
}

LCUI_API int LCUIScreen_Destroy( void )
{
	LCUI_Sys.state = KILLED;
	DeleteDC( hdc_framebuffer );
	ReleaseDC( Win32_GetSelfHWND(), hdc_client );
	free( pixel_mem );
	LCUIMutex_Destroy( &screen_mutex );
	return 0;
}

LCUI_API void LCUIScreen_SyncFrameBuffer( void )
{
	RECT client_rect;
	LCUIMutex_Lock( &screen_mutex );
	SetBitmapBits( client_bitmap, pixel_mem_len, pixel_mem );
	switch(LCUIScreen_GetMode()) {
	case LCUI_INIT_MODE_FULLSCREEN:
		/* 全屏模式下，则使用拉伸模式，将帧拉伸至全屏 */
		GetClientRect( current_hwnd, &client_rect );
		StretchBlt( hdc_client, 0, 0, 
			client_rect.right, client_rect.bottom,
			hdc_framebuffer, 0, 0, 
			LCUIScreen_GetWidth(), LCUIScreen_GetHeight(),
			SRCCOPY );
		break;
	case LCUI_INIT_MODE_WINDOW:
		/* 将帧缓冲内的位图数据更新至客户区内指定区域（area） */
		BitBlt( hdc_client, 0, 0, 
			LCUIScreen_GetWidth(), LCUIScreen_GetHeight(),
			hdc_framebuffer, 0, 0,
			SRCCOPY );
		break;
	}
	ValidateRect( current_hwnd, NULL );
	LCUIMutex_Unlock( &screen_mutex );
}

LCUI_API int LCUIScreen_PutGraph (LCUI_Graph *graph, LCUI_Pos des_pos )
{
	int total, y, n, des_row_x, src_row_x, des_x, src_x;
	LCUI_Graph *src;
	LCUI_Rect cut, src_rect, area;
	LCUI_Size screen_size;
	uchar_t *des_ptr;
	
	src = Graph_GetQuote( graph );
	src_rect = Graph_GetValidRect( graph );
	screen_size.w = GetSystemMetrics(SM_CXSCREEN);
	screen_size.h = GetSystemMetrics(SM_CYSCREEN);
	if(!Graph_IsValid(src)) {
		return -1;
	}
	if(des_pos.x >= screen_size.w || des_pos.y >= screen_size.h) {
		return -1;
	}
	Graph_Lock( src );
	des_ptr = pixel_mem;
	area.x = des_pos.x;
	area.y = des_pos.y;
	area.w = src_rect.w;
	area.h = src_rect.h;
	/* 获取图像的截取区域 */ 
	LCUIRect_GetCutArea( screen_size, area, &cut );
	des_pos.x += cut.x;
	des_pos.y += cut.y;
	/* 根据二维坐标和图像尺寸，计算源图像的起始读取点的一维坐标 */
	src_row_x = (cut.y + src_rect.y) * src->w + cut.x + src_rect.x;
	/* 根据二维坐标和屏幕尺寸，计算帧缓冲的起始写入点的一维坐标 */
	des_row_x = des_pos.y * screen_size.w + des_pos.x;
	for(y=0; y<cut.height; ++y) {
		src_x = src_row_x;
		des_x = des_row_x;
		total = src_x + cut.width;
		for (; src_x < total; ++des_x,++src_x) {
			n = des_x << 2;
			des_ptr[n++] = src->rgba[2][src_x];
			des_ptr[n++] = src->rgba[1][src_x];
			des_ptr[n++] = src->rgba[0][src_x];
		}
		src_row_x += src->w;
		des_row_x += screen_size.w;
	}
	Graph_Unlock( src );
	return 0;
}

LCUI_API void LCUIScreen_CatchGraph( LCUI_Rect area, LCUI_Graph *out )
{
	LCUI_Rect cut_rect;
	LCUI_Size fb_size;
	unsigned char *src_pixel;
	int x, y, n, k, count;

	src_pixel = pixel_mem;

	if( !LCUI_Active() ) {
		return;
	}
	/* 获取帧缓冲的尺寸 */
	fb_size.w = GetSystemMetrics(SM_CXSCREEN);
	fb_size.h = GetSystemMetrics(SM_CYSCREEN);
	/* 如果需要裁剪图形 */
	LCUIRect_GetCutArea( fb_size, area, &cut_rect );
	if( cut_rect.w <= 0 || cut_rect.h <= 0 ) {
		return;
	}
	area.x += cut_rect.x;
	area.y += cut_rect.y;
	area.width = cut_rect.width;
	area.height = cut_rect.height;
	Graph_Create (out, area.width, area.height);
	Graph_Lock (out);
	/* 只能正常捕获32位显示器中显示的图形，有待完善 */
	for (n=0,y=0; y < area.height; ++y) {
		k = (area.y + y) * fb_size.w + area.x;
		for (x = 0; x < area.width; ++x) {
			count = k + x;
			count = count << 2;
			out->rgba[2][n] = src_pixel[count];
			out->rgba[1][n] = src_pixel[count + 1];
			out->rgba[0][n] = src_pixel[count + 2];
			++n;
		}
	}
	Graph_Unlock (out);
}

#endif
