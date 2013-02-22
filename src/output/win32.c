#include <LCUI_Build.h>
#include LC_LCUI_H

#ifdef LCUI_VIDEO_DRIVER_WIN32
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include <Windows.h>

static HWND current_hwnd = NULL;
static unsigned char *pixel_mem = NULL;
static HDC hdc_client, hdc_framebuffer;
static HBITMAP client_bitmap;
static HINSTANCE win32_hInstance = NULL;

void Win32_LCUI_Init( HINSTANCE hInstance )
{
	win32_hInstance = hInstance;
}

static LRESULT CALLBACK 
Win32_LCUI_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static LCUI_BOOL need_write = TRUE;
	RECT rect;
	LCUI_Rect area;
	PAINTSTRUCT ps;
	HDC hdcClient,hdcArea;
	LCUI_Graph graph;
	
	switch (message) {
	case WM_KEYDOWN:
		printf("WM_KEYDOWN: %ld\n",lParam);
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_PAINT: 
		Graph_Init( &graph );
		BeginPaint( hwnd, &ps );
		/* 获取区域坐标及尺寸 */
		area.x = ps.rcPaint.left;
		area.y = ps.rcPaint.top;
		area.width = ps.rcPaint.right - area.x;
		area.height = ps.rcPaint.bottom - area.y;
		LCUIScreen_GetRealGraph( area, &graph );
		LCUIScreen_PutGraph( &graph, Pos(area.x, area.y) );
		/* 将帧缓冲内的位图数据更新至客户区内指定区域（area） */
		BitBlt( hdc_client, area.x, area.y, area.width, area.height, 
			hdc_framebuffer, area.x, area.y, SRCCOPY );
		
		EndPaint( hwnd, &ps );
		Graph_Free( &graph );
		return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

HWND Win32_GetSelfHWND( void )
{
	return current_hwnd;
}

void Win32_SetSelfHWND( HWND hwnd )
{
	current_hwnd = hwnd;
}

void LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

int LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}

int LCUIScreen_Init( void )
{
	MSG msg;
	RECT client_rect;
	LCUI_Graph *graph;
	WNDCLASS wndclass;
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
	
	current_hwnd = CreateWindow (
			szAppName, TEXT ("LCUI"),
			WS_OVERLAPPEDWINDOW &~WS_THICKFRAME,
			CW_USEDEFAULT, CW_USEDEFAULT,
			WIN32_WINDOW_WIDTH, WIN32_WINDOW_HEIGHT,
			NULL, NULL, win32_hInstance, NULL);
	
	GetClientRect( current_hwnd, &client_rect );

	LCUI_Sys.screen.fb_dev_fd = -1;
	LCUI_Sys.screen.fb_dev_name = "win32";
	LCUI_Sys.screen.bits = 32;
	LCUI_Sys.screen.size.w = client_rect.right;
	LCUI_Sys.screen.size.h = client_rect.bottom; 
	LCUI_Sys.screen.smem_len = LCUI_Sys.screen.size.w * LCUI_Sys.screen.size.h * 4;
	/* 分配内存，储存像素数据 */ 
	pixel_mem = malloc ( LCUI_Sys.screen.smem_len );
	LCUI_Sys.screen.fb_mem = pixel_mem;
	LCUI_Sys.root_glayer = GraphLayer_New();
	GraphLayer_Resize( LCUI_Sys.root_glayer, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h );
	graph = GraphLayer_GetSelfGraph( LCUI_Sys.root_glayer );
	Graph_Fill_Color( graph, RGB(255,255,255) );

	/* 获取客户区的DC */
	hdc_client = GetDC( current_hwnd );
	/* 为帧缓冲创建一个DC */
	hdc_framebuffer = CreateCompatibleDC( hdc_client );
	/* 为客户区创建一个Bitmap */ 
	client_bitmap = CreateCompatibleBitmap( hdc_client, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h );
	/* 为帧缓冲的DC选择client_bitmap作为对象 */
	SelectObject( hdc_framebuffer, client_bitmap );
	
	GraphLayer_Show( LCUI_Sys.root_glayer );
	ShowWindow( current_hwnd, SW_SHOWNORMAL );
	UpdateWindow( current_hwnd );
	return 0;
}

int LCUIScreen_Destroy( void )
{
	int err;
	LCUI_Graph *graph;
	
	LCUI_Sys.state = KILLED;
	graph = GraphLayer_GetSelfGraph( LCUI_Sys.root_glayer );
	GraphLayer_Free( LCUI_Sys.root_glayer );
	DeleteDC( hdc_framebuffer );
	ReleaseDC( Win32_GetSelfHWND(), hdc_client );
	free( pixel_mem );
	return 0;
}

int LCUIScreen_PutGraph (LCUI_Graph *src, LCUI_Pos pos )
{
	int bits;
	uchar_t *dest;
	unsigned int x, y, n, k, count;
	unsigned int temp1, temp2, temp3, i; 
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Graph_Valid (src)) {
		return -1;
	}
	/* 指向帧缓冲 */
	dest = LCUI_Sys.screen.fb_mem;		
	pic = src; 
	Graph_Init (&temp);
	
	if ( Get_Cut_Area ( LCUIScreen_GetSize(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) ) {/* 如果需要裁剪图形 */
		if(!Rect_Valid(cut_rect)) {
			return -2;
		}
		pos.x += cut_rect.x;
		pos.y += cut_rect.y;
		Graph_Cut (pic, cut_rect, &temp);
		pic = &temp;
	}
	
	Graph_Lock( pic );
	k = pos.y * LCUI_Sys.screen.size.w + pos.x;
	for (n=0,y = 0; y < pic->height; ++y) {
		for (x = 0; x < pic->width; ++x, ++n) {
			count = k + x;//count = 4 * (k + x);/* 计算需填充的像素点的坐标 */
			count = count << 2; 
			dest[count++] = pic->rgba[2][n]; 
			dest[count++] = pic->rgba[1][n]; 
			dest[count++] = pic->rgba[0][n];
			dest[count] = 255;
		}
		k += LCUI_Sys.screen.size.w;
	}
	Graph_Unlock( pic );
	Graph_Free( &temp );
	SetBitmapBits( client_bitmap, LCUI_Sys.screen.smem_len, LCUI_Sys.screen.fb_mem );
	return 0;
}

#endif
