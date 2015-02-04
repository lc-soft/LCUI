#include <LCUI_Build.h>
#define __IN_SURFACE_SOURCE_FILE__
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/thread.h>
#include <LCUI/widget_build.h>
#include <LCUI/surface.h>
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
	LCUI_Widget target;
	LCUI_DirtyRectList rect;
	HDC fb_hdc;
	HBITMAP fb_bmp;
	LCUI_Graph fb;
	LCUI_SurfaceTask task_buffer[TASK_TOTAL_NUM];
};

static struct {
	HINSTANCE main_instance;	/**< 主程序的资源句柄 */
	HINSTANCE dll_instance;		/**< 动态库中的资源句柄 */
	LCUI_Thread loop_thread;	/**< 消息循环线程 */
	LCUI_Cond cond;			/**< 条件，当消息循环已创建时成立 */
	LCUI_BOOL is_ready;		/**< 消息循环线程是否已准备好 */
} win32 = { NULL, NULL };

static FrameCtrlCtx surface_framectrl_ctx;
static LinkedList surface_list;
static LCUI_BOOL surface_proc_active = FALSE;

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
	return NULL;
}

static LCUI_Surface GetSurfaceByWidget( LCUI_Widget widget )
{
	return NULL;
}

static LRESULT CALLBACK 
WndProc( HWND hwnd, UINT msg, WPARAM arg1, LPARAM arg2 )
{
	LCUI_Surface surface;
	_DEBUG_MSG("msg: %d\n", msg);
	surface = GetSurfaceByHWND( hwnd);
	if( !surface ) {
		return DefWindowProc( hwnd, msg, arg1, arg2 );
	}
	_DEBUG_MSG("tip");
	switch( msg ) {
	case WM_SETFOCUS:
		break;
	case WM_KILLFOCUS:
		break;
	case WM_CLOSE:
		Surface_Delete( surface );
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
	return DefWindowProc( hwnd, msg, arg1, arg2 );
}

/** 删除 Surface */
void Surface_Delete( LCUI_Surface surface )
{
	int i, n;

	surface->w = 0;
	surface->h = 0;
	Graph_Free( &surface->fb );
	DirtyRectList_Destroy( &surface->rect );
	n = LinkedList_GetTotal( &surface_list );
	for( i=0; i<n; ++i ) {
		if( surface == LinkedList_Get(&surface_list) ) {
			LinkedList_Delete( &surface_list );
			break;
		}
		LinkedList_ToNext( &surface_list );
	}
}

/** 新建一个 Surface */
LCUI_Surface Surface_New(void)
{
	int i;
	LCUI_Surface surface;

	surface = (LCUI_Surface)malloc(sizeof(struct LCUI_SurfaceRec_));
	surface->target = NULL;
	surface->mode = RENDER_MODE_BIT_BLT;
	surface->fb_hdc = NULL;
	surface->fb_bmp = NULL;
	Graph_Init( &surface->fb );
	surface->fb.color_type = COLOR_TYPE_ARGB;
	DirtyRectList_Init( &surface->rect );
	surface->hwnd = NULL;
	for( i=0; i<TASK_TOTAL_NUM; ++i ) {
		surface->task_buffer[i].is_valid = FALSE;
	}
	LinkedList_Insert( &surface_list, surface );
	_DEBUG_MSG("wait...\n");
	if( !win32.is_ready ) {
		/* 等待 Surface 线程创建完 windows 消息队列 */
		LCUICond_Wait( &win32.cond );
	}
	_DEBUG_MSG("ok\n");
	/* 让 Surface 线程去完成 windows 窗口的创建 */
	_DEBUG_MSG("post result: %d\n", PostThreadMessage( win32.loop_thread, MSG_SURFACE_CREATE, 0, (LPARAM)surface ));
	return surface;
}

/** 标记 Surface 中的无效区域 */
void Surface_InvalidateArea( LCUI_Surface surface, LCUI_Rect *p_rect )
{
	LCUI_Rect rect;
	if( !p_rect ) {
		rect.x = 0;
		rect.y = 0;
		rect.w = surface->w;
		rect.h = surface->h;
		p_rect = &rect;
	}
	DirtyRectList_Add( &surface->rect, p_rect );
}

void Surface_Move( LCUI_Surface surface, int x, int y )
{
	x += GetSystemMetrics(SM_CXFIXEDFRAME);
	y += GetSystemMetrics(SM_CYFIXEDFRAME);
	/* 如果已经得到窗口句柄，则直接改变窗口位置 */
	if( surface->hwnd ) {
		SetWindowPos( 
			surface->hwnd, HWND_NOTOPMOST,
			x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER 
		);
		return;
	}
	/* 缓存任务，等获得窗口句柄后处理 */
	surface->task_buffer[TASK_MOVE].x = x;
	surface->task_buffer[TASK_MOVE].y = y;
	surface->task_buffer[TASK_MOVE].is_valid = TRUE;
}

void Surface_Resize( LCUI_Surface surface, int w, int h )
{
	/* 调整位图缓存的尺寸 */
	Graph_Create( &surface->fb, w, h );
	surface->w = w;
	surface->h = h;
	// surface->fb_bmp
	Surface_InvalidateArea( surface, NULL );
	/* 加上窗口边框的尺寸 */
	w += GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	h += GetSystemMetrics(SM_CYFIXEDFRAME)*2;
	h += GetSystemMetrics(SM_CYCAPTION);
	if( surface->hwnd ) {
		SetWindowLong( 
			surface->hwnd, GWL_STYLE, WIN32_WINDOW_STYLE 
		);
		SetWindowPos( 
			surface->hwnd, HWND_NOTOPMOST, 
			0, 0, w, h, SWP_NOMOVE|SWP_NOZORDER
		);
		return;
	}
	surface->task_buffer[TASK_RESIZE].width = w;
	surface->task_buffer[TASK_RESIZE].height = h;
	surface->task_buffer[TASK_RESIZE].is_valid = TRUE;
}

void Surface_Show( LCUI_Surface surface )
{
	if( surface->hwnd ) {
		ShowWindow( surface->hwnd, SW_SHOWNORMAL );
		return;
	}
	surface->task_buffer[TASK_RESIZE].show = TRUE;
	surface->task_buffer[TASK_RESIZE].is_valid = TRUE;
}

void Surface_Hide( LCUI_Surface surface )
{
	if( !surface->hwnd ) {
		ShowWindow( surface->hwnd, SW_HIDE );
		return;
	}
	surface->task_buffer[TASK_RESIZE].show = FALSE;
	surface->task_buffer[TASK_RESIZE].is_valid = TRUE;
}

void Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *str )
{
	int len;
	wchar_t *caption;

	if( surface->hwnd ) {
		SetWindowText( surface->hwnd, str );
		return;
	}
	len = wcslen(str);
	caption = (wchar_t*)malloc(sizeof(wchar_t)*(len+1));
	wcsncpy( caption, str, len );
	if( surface->task_buffer[TASK_SET_CAPTION].is_valid 
	 && surface->task_buffer[TASK_SET_CAPTION].caption ) {
		free( surface->task_buffer[TASK_SET_CAPTION].caption );
	}
	surface->task_buffer[TASK_SET_CAPTION].caption = caption;
	surface->task_buffer[TASK_SET_CAPTION].is_valid = TRUE;
}

void Surface_SetOpacity( LCUI_Surface surface, float opacity )
{

}

void Surface_UnmapWidget( LCUI_Surface surface )
{
	if( !surface->target ) {
		return;
	}
	Surface_InvalidateArea( surface, NULL );
	surface->target = NULL;
}

static void
OnWidgetShow( LCUI_Widget widget, LCUI_WidgetEvent *unused )
{
	LCUI_Surface surface;
	surface = GetSurfaceByWidget( widget );
	if( !surface ) {
		return;
	}
	Surface_Show( surface );
}

static void
OnWidgetHide( LCUI_Widget widget, LCUI_WidgetEvent *unused )
{
	LCUI_Surface surface;
	surface = GetSurfaceByWidget( widget );
	if( !surface ) {
		return;
	}
	Surface_Hide( surface );
}

static void
OnWidgetDestroy( LCUI_Widget widget, LCUI_WidgetEvent *unused )
{
	LCUI_Surface surface;
	surface = GetSurfaceByWidget( widget );
	if( !surface ) {
		return;
	}
	Surface_Delete( surface );
}

static void
OnWidgetResize( LCUI_Widget widget, LCUI_WidgetEvent *e )
{
	LCUI_Surface surface;
	surface = GetSurfaceByWidget( widget );
	if( !surface ) {
		return;
	}
	//...
}

static void
OnWidgetOpacityChange( LCUI_Widget widget, LCUI_WidgetEvent *e )
{
	LCUI_Surface surface;
	surface = GetSurfaceByWidget( widget );
	if( !surface ) {
		return;
	}
	Surface_SetOpacity( surface, 1.0 );
}

/** 将指定部件映射至 Surface 上 */
void Surface_MapWidget( LCUI_Surface surface, LCUI_Widget widget )
{
	/**
	 * 关联部件相关事件，以在部件变化时让 surface 做相应变化
	 */
	/*解除与之前映射的部件的关系 */
	Surface_UnmapWidget( surface );
	surface->target = widget;
}

/** 设置 Surface 的渲染模式 */
int Surface_SetRenderMode( LCUI_Surface surface, int mode )
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
int Surface_Paint( LCUI_Surface surface, LCUI_Rect *p_rect )
{
	LCUI_Graph graph;
	LCUI_Rect rect;

	if( !surface->target ) {
		return -1;
	}
	if( !p_rect ) {
		rect.x = 0;
		rect.y = 0;
		rect.w = surface->w;
		rect.h = surface->h;
		p_rect = &rect;
	}

	//glayer = &surface->target->glayer;
	//Graph_Quote( &graph, &surface->fb, *p_rect );
	Graph_FillColor( &graph, ARGB(255,255,255,255) );
	//GraphLayer_GetGraph( glayer, &graph, *p_rect );
	return 0;
}

/** 处理Surface的无效区域 */
void Surface_ProcInvalidArea( LCUI_Surface surface )
{
	int i, n;
	LCUI_Rect *p_rect;

	n = LinkedList_GetTotal( &surface->rect );
	LinkedList_Goto( &surface->rect, 0 );
	for( i=0; i<n; ++i ) {
		p_rect = (LCUI_Rect*)LinkedList_Get( &surface->rect );
		Surface_Paint( surface, p_rect );
		LinkedList_Delete( &surface->rect );
	}
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
void Surface_Present( LCUI_Surface surface )
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

/** 处理当前缓存的任务 */
static void Surface_ProcTaskBuffer( LCUI_Surface surface )
{
	LCUI_SurfaceTask *t, *t2;
	t = &surface->task_buffer[TASK_MOVE];
	t2 = &surface->task_buffer[TASK_RESIZE];
	/* 此判断只是为了合并 移动 和 调整尺寸 操作 */
	if( t->is_valid ) {
		if( t2->is_valid ) {
			SetWindowPos( surface->hwnd, HWND_NOTOPMOST, 
				t->x, t->y, t2->width, t2->height, 
				SWP_NOZORDER
			);
		} else {
			SetWindowPos( surface->hwnd, HWND_NOTOPMOST, 
				t->x, t->y, 0, 0, 
				SWP_NOSIZE|SWP_NOZORDER
			);
		}
	} else if( t2->is_valid ) {
		SetWindowPos( surface->hwnd, HWND_NOTOPMOST, 
			t->x, t->y, t2->width, t2->height,
			SWP_NOMOVE|SWP_NOZORDER
		);
	}
	t->is_valid = FALSE;
	t2->is_valid = FALSE;

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
	if( t->is_valid ) {
		if( t->show ) {
			ShowWindow( surface->hwnd, SW_SHOWNORMAL );
		} else {
			ShowWindow( surface->hwnd, SW_HIDE );
		}
	}
	t->is_valid = FALSE;
}

static void LCUISurface_Loop( void *unused )
{
	MSG msg;
	LCUI_Surface surface;
	_DEBUG_MSG("start\n");
	/* 创建消息队列 */
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	win32.is_ready = TRUE;
	LCUICond_Broadcast( &win32.cond );
	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		if( msg.message == MSG_SURFACE_CREATE ) {
			surface = (LCUI_Surface)msg.lParam;
			/* 为 Surface 创建窗口 */
			surface->hwnd = CreateWindow(
				TEXT("LCUI"), TEXT("LCUI Surface"),
				WIN32_WINDOW_STYLE,
				CW_USEDEFAULT, CW_USEDEFAULT,
				0, 0,
				NULL, NULL, win32.main_instance, NULL
			);
			_DEBUG_MSG("surface->hwnd: %p\n", surface->hwnd);
			Surface_ProcTaskBuffer( surface );
			continue;
		}
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	_DEBUG_MSG("quit\n");
	LCUIThread_Exit(NULL);
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
	LCUICond_Init( &win32.cond );
	LCUIThread_Create( &win32.loop_thread, LCUISurface_Loop, NULL );
	return 0;
}

/** Surface 处理线程 */
static void LCUISurface_ProcThread(void *unused)
{
	/*
	 * 处理列表中每个 Surface 的更新，包括映射的被部件的消息处理、无效区
	 * 域更新、Surface 内容的更新。
	 */
	//int i, n;
	//LCUI_Surface surface;

	FrameControl_Init( &surface_framectrl_ctx );
	FrameControl_SetMaxFPS( &surface_framectrl_ctx, 100 );
	surface_proc_active = TRUE;

	while( surface_proc_active ) {
		/*LCUIWidget_ProcInvalidArea();
		LinkedList_Goto( &surface_list, 0 );
		n = LinkedList_GetTotal( &surface_list );
		for( i=0; i<n; ++i ) {
			surface = (LCUI_Surface)LinkedList_Get( &surface_list );
			Surface_ProcInvalidArea( surface );
			Surface_Present( surface );
			LinkedList_ToNext( &surface_list );
		}*/
		FrameControl_Remain( &surface_framectrl_ctx );
	}

	LCUIThread_Exit(NULL);
}

#endif
