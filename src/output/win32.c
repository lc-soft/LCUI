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

static HWND current_hwnd = NULL;
static int pixel_mem_len = 0;
static unsigned char *pixel_mem = NULL;
static HDC hdc_client, hdc_framebuffer;
static HBITMAP client_bitmap;
static HINSTANCE win32_hInstance = NULL;
static LCUI_Mutex screen_mutex;

LCUI_API void
Win32_LCUI_Init( HINSTANCE hInstance )
{
	win32_hInstance = hInstance;
}

static LRESULT CALLBACK 
Win32_LCUI_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	LCUI_Rect area;

	switch (message) {
	case WM_KEYDOWN:
		printf("WM_KEYDOWN: %ld\n",wParam);
		LCUIKeyboard_HitKey( wParam );
		return 0;
	case WM_KEYUP:
		printf("WM_KEYUP: %ld\n",wParam);
		LCUIKeyboard_FreeKey( wParam );
		return 0;
	case WM_RBUTTONDOWN:
		Win32_LCUIMouse_ButtonDown( LCUIKEY_RIGHTBUTTON );
		return 0;
	case WM_RBUTTONUP:
		Win32_LCUIMouse_ButtonUp( LCUIKEY_RIGHTBUTTON );
		return 0;
	case WM_LBUTTONDOWN:
		//_DEBUG_MSG("left button down\n");
		Win32_LCUIMouse_ButtonDown( LCUIKEY_LEFTBUTTON );
		return 0;
	case WM_LBUTTONUP:
		//_DEBUG_MSG("left button up\n");
		Win32_LCUIMouse_ButtonUp( LCUIKEY_LEFTBUTTON );
		return 0;
	case WM_PAINT:
		DEBUG_MSG("WM_PAINT\n");
		BeginPaint( hwnd, &ps );
		/* 获取区域坐标及尺寸 */
		area.x = ps.rcPaint.left;
		area.y = ps.rcPaint.top;
		area.width = ps.rcPaint.right - area.x;
		area.height = ps.rcPaint.bottom - area.y;
		LCUIScreen_InvalidArea( area );
		EndPaint( hwnd, &ps );
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		LCUI_Quit();
		return 0;
	default:break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LCUI_API HWND
Win32_GetSelfHWND( void )
{
	return current_hwnd;
}

LCUI_API void
Win32_SetSelfHWND( HWND hwnd )
{
	current_hwnd = hwnd;
}

LCUI_API void
LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

LCUI_API int
LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}

LCUI_API int
LCUIScreen_Init( int w, int h, int mode )
{
	RECT client_rect;
	WNDCLASS wndclass;
	LCUI_Widget *root_widget;
	LCUI_Screen screen_info;
	LCUI_Size window_size;
	TCHAR szAppName[] = TEXT ("Typer");
	
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = Win32_LCUI_WndProc;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = win32_hInstance;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass)) {
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
		szAppName, MB_ICONERROR) ;
		return 0;
	}
	/* 初始化屏幕互斥锁 */
	LCUIMutex_Init( &screen_mutex );
	if( mode == LCUI_INIT_MODE_AUTO ) {
		if( w == 0 && h == 0 ) {
			mode = LCUI_INIT_MODE_FULLSCREEN;
		} else {
			mode = LCUI_INIT_MODE_WINDOW;
		}
	}
	if( mode == LCUI_INIT_MODE_FULLSCREEN ) {
		window_size.w = GetSystemMetrics(SM_CXSCREEN);
		window_size.h = GetSystemMetrics(SM_CYSCREEN);
		if( w == 0 ) {
			w = window_size.w;
		}
		if( h == 0 ) {
			h = window_size.h;
		}
		current_hwnd = CreateWindow (
				szAppName, TEXT ("LCUI"),
				WS_POPUP,
				CW_USEDEFAULT, CW_USEDEFAULT,
				window_size.w, window_size.h,
				NULL, NULL, win32_hInstance, NULL);
		screen_info.size.w = w;
		screen_info.size.h = h;
	} else {
		window_size.w = w;
		window_size.h = h;
		current_hwnd = CreateWindow (
				szAppName, TEXT ("LCUI"),
				WS_OVERLAPPEDWINDOW &~WS_THICKFRAME,
				CW_USEDEFAULT, CW_USEDEFAULT,
				window_size.w, window_size.h,
				NULL, NULL, win32_hInstance, NULL);
		GetClientRect( current_hwnd, &client_rect );
		screen_info.size.w = client_rect.right;
		screen_info.size.h = client_rect.bottom;
	}
	screen_info.bits = 32;
	screen_info.mode = mode;
	strcpy( screen_info.dev_name, "win32 GDI" );
	LCUIScreen_SetInfo( &screen_info );

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

	ShowWindow( current_hwnd, SW_SHOWNORMAL );
	UpdateWindow( current_hwnd );
	return 0;
}

/* 设置视频输出模式 */
LCUI_API int
LCUIScreen_SetMode( int w, int h, int mode )
{
	LCUI_Pos pos;
	RECT client_rect;
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
		SetWindowPos( current_hwnd, HWND_NOTOPMOST, 0, 0, real_size.w, real_size.h, SWP_SHOWWINDOW );
	} else {
		pos.x = (real_size.w - w)/2;
		pos.y = (real_size.h - h)/2;
		SetWindowLong( current_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW &~WS_THICKFRAME );
		SetWindowPos( current_hwnd, HWND_NOTOPMOST, pos.x, pos.y, w, h, SWP_SHOWWINDOW );
		GetClientRect( current_hwnd, &client_rect );
		w = client_rect.right;
		h = client_rect.bottom;
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

LCUI_API int
LCUIScreen_Destroy( void )
{
	LCUI_Sys.state = KILLED;
	DeleteDC( hdc_framebuffer );
	ReleaseDC( Win32_GetSelfHWND(), hdc_client );
	free( pixel_mem );
	LCUIMutex_Destroy( &screen_mutex );
	return 0;
}

LCUI_API void
LCUIScreen_SyncFrameBuffer( void )
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

LCUI_API int
LCUIScreen_PutGraph (LCUI_Graph *graph, LCUI_Pos des_pos )
{
	int total, y, n, des_row_x, src_row_x, des_x, src_x;
	LCUI_Graph *src;
	LCUI_Rect cut, src_rect;
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
	des_ptr = pixel_mem;
	Graph_Lock( src );
	/* 获取图像的截取区域 */ 
	if( LCUIRect_GetCutArea( screen_size,
		Rect( des_pos.x, des_pos.y, src_rect.width, src_rect.height ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	/* 根据二维坐标和图像尺寸，计算源图像的起始读取点的一维坐标 */
	src_row_x = (cut.y + src_rect.y) * src->width + cut.x + src_rect.x;
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
		src_row_x += src->width;
		/* DIB扫描行是上下颠倒的，因此是从行尾到行首递减 */
		des_row_x += screen_size.w;
	}
	Graph_Unlock( src );
	return 0;
}

LCUI_API void
LCUIScreen_CatchGraph( LCUI_Rect area, LCUI_Graph *out )
{
	return;
}

#endif
