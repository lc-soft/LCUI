/* ***************************************************************************
 * windows_display.c -- surface support for windows platform.
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * windows_display.c -- windows 平台的图形显示功能支持。
 *
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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

//#define DEBUG
#include <stdio.h>
#include <LCUI_Build.h>
#define __IN_SURFACE_SOURCE_FILE__
#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_DISPLAY_H
#include LCUI_EVENTS_H

#define MIN_WIDTH		480
#define MIN_HEIGHT		500
#define WIN32_WINDOW_STYLE	(WS_OVERLAPPEDWINDOW | WS_MAXIMIZEBOX)

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

/** 适用于 windows 平台的 surface 数据结构 */
struct LCUI_SurfaceRec_ {
	HWND hwnd;
	int mode;
	int w, h;
	HDC fb_hdc;
	HBITMAP fb_bmp;
	LCUI_BOOL is_ready;
	LCUI_Graph fb;			/**< 帧缓存，保存当前窗口内呈现的图像内容 */
	LCUI_SurfaceTask task_buffer[TASK_TOTAL_NUM];
};

/** windows 下图形显示功能所需的数据 */
static struct WIN_Display {
	LCUI_BOOL is_inited;
	LinkedList surfaces;		/**< surface 记录 */
	LCUI_EventTrigger trigger;
} win = {0};

/** 根据 hwnd 获取 Surface */
static LCUI_Surface GetSurfaceByHWND( HWND hwnd )
{
	LinkedListNode *node;
	LinkedList_ForEach( node, &win.surfaces ) {
		if( ((LCUI_Surface)node->data)->hwnd == hwnd ) {
			return node->data;
		}
	}
	return NULL;
}

static void WinSurface_Destroy( LCUI_Surface surface )
{
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

static void WinSurface_ExecDelete( LCUI_Surface surface )
{
	LinkedListNode *node;
	LinkedList_ForEach( node, &win.surfaces ) {
		if( node->data == surface ) {
			WinSurface_Destroy( node->data );
			LinkedList_DeleteNode( &win.surfaces, node );
			break;
		}
	}
}

static void WinSurface_Delete( LCUI_Surface surface )
{
	if( surface->hwnd ) {
		PostMessage( surface->hwnd, WM_CLOSE, 0, 0 );
	}
}

static void OnCreateSurface( void *arg1, void *arg2 )
{
	HDC hdc_client;
	HINSTANCE instance;
	LCUI_Surface surface = arg1;
	instance = LCUI_GetAppData();
	/* 为 Surface 创建窗口 */
	surface->hwnd = CreateWindow(
		TEXT("LCUI"), TEXT("LCUI Surface"),
		WIN32_WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, NULL, NULL,
		instance, NULL
	);
	hdc_client = GetDC( surface->hwnd );
	RegisterTouchWindow( surface->hwnd, 0 );
	surface->fb_hdc = CreateCompatibleDC( hdc_client );
	surface->is_ready = TRUE;
	DEBUG_MSG("surface: %p, surface->hwnd: %p\n", surface, surface->hwnd);
}

/** 新建一个 Surface */
static LCUI_Surface WinSurface_New(void)
{
	int i;
	LCUI_AppTaskRec task;
	LCUI_Surface surface;
	surface = NEW(struct LCUI_SurfaceRec_, 1);
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
	LinkedList_Append( &win.surfaces, surface );
	task.arg[1] = NULL;
	task.arg[0] = surface;
	task.func = OnCreateSurface;
	task.destroy_arg[0] = NULL;
	task.destroy_arg[1] = NULL;
	LCUI_PostTask( &task );
	return surface;
}

static LCUI_BOOL WinSurface_IsReady( LCUI_Surface surface )
{
	return surface->is_ready;
}

static void WinSurface_ExecMove( LCUI_Surface surface, int x, int y )
{
	x += GetSystemMetrics( SM_CXFIXEDFRAME );
	y += GetSystemMetrics( SM_CYFIXEDFRAME );
	SetWindowPos( surface->hwnd, HWND_NOTOPMOST,
		      x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	return;
}

static void WinSurface_Move( LCUI_Surface surface, int x, int y )
{
	/* 缓存任务，等获得窗口句柄后处理 */
	surface->task_buffer[TASK_MOVE].x = x;
	surface->task_buffer[TASK_MOVE].y = y;
	surface->task_buffer[TASK_MOVE].is_valid = TRUE;
}

static void WinSurface_ExecResize( LCUI_Surface surface, int w, int h )
{
	HDC hdc_client;
	HBITMAP old_bmp;
	RECT rect_client, rect_window;
	surface->w = w;
	surface->h = h;
	DEBUG_MSG("w = %d, h = %d\n", w, h);
	Graph_Create( &surface->fb, w, h );
	hdc_client = GetDC( surface->hwnd );
	surface->fb_bmp = CreateCompatibleBitmap( hdc_client, w, h );
	old_bmp = (HBITMAP)SelectObject( surface->fb_hdc, surface->fb_bmp );
	if( old_bmp ) {
		DeleteObject( old_bmp );
	}
	GetClientRect( surface->hwnd, &rect_client );
	GetWindowRect( surface->hwnd, &rect_window );
	w += rect_window.right - rect_window.left;
	w -= rect_client.right - rect_client.left;
	h += rect_window.bottom - rect_window.top;
	h -= rect_client.bottom - rect_client.top;
	//SetWindowLong( surface->hwnd, GWL_STYLE, WIN32_WINDOW_STYLE );
	SetWindowPos( surface->hwnd, HWND_NOTOPMOST,
		      0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER );
}

static void WinSurface_Resize( LCUI_Surface surface, int w, int h )
{
	surface->task_buffer[TASK_RESIZE].width = w;
	surface->task_buffer[TASK_RESIZE].height = h;
	surface->task_buffer[TASK_RESIZE].is_valid = TRUE;
}

static void WinSurface_Show( LCUI_Surface surface )
{
	DEBUG_MSG("surface: %p, buffer show.\n", surface);
	surface->task_buffer[TASK_SHOW].show = TRUE;
	surface->task_buffer[TASK_SHOW].is_valid = TRUE;
}

static void WinSurface_Hide( LCUI_Surface surface )
{
	surface->task_buffer[TASK_SHOW].show = FALSE;
	surface->task_buffer[TASK_SHOW].is_valid = TRUE;
}

static void WinSurface_SetCaptionW( LCUI_Surface surface, const wchar_t *str )
{
	int len;
	wchar_t *caption = NULL;

	if( str ) {
		len = wcslen(str) + 1;
		caption = (wchar_t*)malloc(sizeof(wchar_t)*len);
		wcsncpy( caption, str, len );
	}
	if( surface->task_buffer[TASK_SET_CAPTION].is_valid
	    && surface->task_buffer[TASK_SET_CAPTION].caption ) {
		free( surface->task_buffer[TASK_SET_CAPTION].caption );
	}
	surface->task_buffer[TASK_SET_CAPTION].caption = caption;
	surface->task_buffer[TASK_SET_CAPTION].is_valid = TRUE;
}

void WinSurface_SetOpacity( LCUI_Surface surface, float opacity )
{

}

/** 设置 Surface 的渲染模式 */
static void WinSurface_SetRenderMode( LCUI_Surface surface, int mode )
{
	surface->mode = mode;
}

/**
* 准备绘制 Surface 中的内容
* @param[in] surface	目标 surface
* @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
* @return		返回绘制上下文句柄
*/
static LCUI_PaintContext WinSurface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	LCUI_PaintContext paint;
	paint = malloc(sizeof(LCUI_PaintContextRec));
	paint->rect = *rect;
	paint->with_alpha = FALSE;
	Graph_Init( &paint->canvas );
	LCUIRect_ValidateArea( &paint->rect, surface->w, surface->h );
	Graph_Quote( &paint->canvas, &surface->fb, &paint->rect );
	Graph_FillRect( &paint->canvas, RGB( 255, 255, 255 ), NULL, TRUE );
	return paint;
}

/**
* 结束对 Surface 的绘制操作
* @param[in] surface	目标 surface
* @param[in] paint_ctx	绘制上下文句柄
*/
static void WinSurface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx )
{
	free( paint_ctx );
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
static void WinSurface_Present( LCUI_Surface surface )
{
	HDC hdc_client;
	RECT client_rect;

	DEBUG_MSG("surface: %p, hwnd: %p\n", surface, surface->hwnd);
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
static void WinSurface_Update( LCUI_Surface surface )
{
	LCUI_SurfaceTask *t;
	if( !surface->hwnd ) {
		return;
	}
	DEBUG_MSG("surface: %p\n", surface);
	t = &surface->task_buffer[TASK_MOVE];
	if( t->is_valid ) {
		WinSurface_ExecMove( surface, t->x, t->y );
		t->is_valid = FALSE;
	}
	t = &surface->task_buffer[TASK_RESIZE];
	if( t->is_valid ) {
		WinSurface_ExecResize( surface, t->width, t->height );
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

static void OnWMPaint( LCUI_Event e, void *arg )
{
	PAINTSTRUCT ps;
	LCUI_Rect area;
	LCUI_Surface surface;
	LCUI_DisplayEventRec dpy_ev;
	WIN_SysEvent sys_event = arg;
	BeginPaint( sys_event->hwnd, &ps );
	/* 获取区域坐标及尺寸 */
	area.x = ps.rcPaint.left;
	area.y = ps.rcPaint.top;
	area.width = ps.rcPaint.right - area.x;
	area.height = ps.rcPaint.bottom - area.y;
	EndPaint( sys_event->hwnd, &ps );
	surface = GetSurfaceByHWND( sys_event->hwnd );
	if( !surface ) {
		return;
	}
	dpy_ev.type = DET_PAINT;
	dpy_ev.surface = surface;
	dpy_ev.paint.rect = area;
	EventTrigger_Trigger( win.trigger, DET_PAINT, &dpy_ev );
}

static void OnWMGetMinMaxInfo( LCUI_Event e, void *arg )
{
	MINMAXINFO *mminfo;
	WIN_SysEvent sys_event = arg;
	mminfo = (PMINMAXINFO)sys_event->lparam;
	mminfo->ptMinTrackSize.x = MIN_WIDTH;
	mminfo->ptMinTrackSize.y = MIN_HEIGHT;
}

static void OnWMSize( LCUI_Event e, void *arg )
{
	LCUI_Surface surface;
	LCUI_DisplayEventRec dpy_ev;
	WIN_SysEvent sys_event = arg;
	surface = GetSurfaceByHWND( sys_event->hwnd );
	if( !surface ) {
		return;
	}
	dpy_ev.surface = surface;
	dpy_ev.type = DET_RESIZE;
	dpy_ev.resize.width = LOWORD( sys_event->lparam );
	dpy_ev.resize.height = HIWORD( sys_event->lparam );
	EventTrigger_Trigger( win.trigger, DET_RESIZE, &dpy_ev );
}

static int WinDisplay_BindEvent( int event_id, LCUI_EventFunc func, 
				 void *data, void (*destroy_data)(void*) )
{
	return EventTrigger_Bind( win.trigger, event_id, func, 
				  data, destroy_data );
}

static void* WinSurface_GetHandle( LCUI_Surface s )
{
	return s->hwnd;
}

static int WinDisplay_GetWidth( void )
{
	return GetSystemMetrics( SM_CXSCREEN );
}

static int WinDisplay_GetHeight( void )
{
	return GetSystemMetrics( SM_CYSCREEN );
}

int LCUI_InitWinDisplay( LCUI_DisplayDriver driver )
{
	strcpy( driver->name, "windows" );
	driver->getWidth = WinDisplay_GetWidth;
	driver->getHeight = WinDisplay_GetHeight;
	driver->new = WinSurface_New;
	driver->delete = WinSurface_Delete;
	driver->isReady = WinSurface_IsReady;
	driver->show = WinSurface_Show;
	driver->hide = WinSurface_Hide;
	driver->move = WinSurface_Move;
	driver->resize = WinSurface_Resize;
	driver->update = WinSurface_Update;
	driver->present = WinSurface_Present;
	driver->setCaptionW = WinSurface_SetCaptionW;
	driver->setRenderMode = WinSurface_SetRenderMode;
	driver->setOpacity = WinSurface_SetOpacity;
	driver->getHandle = WinSurface_GetHandle;
	driver->beginPaint = WinSurface_BeginPaint;
	driver->endPaint = WinSurface_EndPaint;
	driver->bindEvent = WinDisplay_BindEvent;
	LCUI_BindSysEvent( WM_SIZE, OnWMSize, NULL, NULL );
	LCUI_BindSysEvent( WM_PAINT, OnWMPaint, NULL, NULL );
	LCUI_BindSysEvent( WM_GETMINMAXINFO, OnWMGetMinMaxInfo, NULL, NULL );
	LinkedList_Init( &win.surfaces );
	win.trigger = EventTrigger();
	win.is_inited = TRUE;
	return 0;
}

int LCUI_ExitWinDisplay( void )
{
	// ...
	return 0;
}

#endif
