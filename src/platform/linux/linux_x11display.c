/* ***************************************************************************
 * linux_x11display.c -- surface support for linux xwindow.
 *
 * Copyright (C) 2016-2017 by Liu Chao <lc-soft@live.cn>
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
 * linux_x11display.c -- linux 平台的图形显示功能支持，基于 xwindow。
 *
 * 版权所有 (C) 2016-2017 归属于 刘超 <lc-soft@live.cn>
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
#include <stdlib.h>
#include <LCUI_Build.h>
#define LCUI_SURFACE_C
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include <LCUI/font/charset.h>
#include LCUI_DISPLAY_H
#include LCUI_EVENTS_H

#define MIN_WIDTH	320
#define MIN_HEIGHT	240

enum SurfaceTaskType {
	TASK_MOVE,
	TASK_RESIZE,
	TASK_SHOW,
	TASK_SET_CAPTION,
	TASK_DELETE,
	TASK_TOTAL_NUM
};

typedef struct LCUI_SurfaceTaskRec_ {
	LCUI_BOOL is_valid;
	union {
		struct {
			int x, y;
		};
		struct {
			int width, height;
		};
		LCUI_BOOL show;
		struct {
			char *caption;
			size_t caption_len;
		};
	};
} LCUI_SurfaceTaskRec, *LCUI_SurfaceTask;
typedef LCUI_SurfaceTaskRec LCUI_SurfaceTasks[TASK_TOTAL_NUM];

typedef struct LCUI_SurfaceRec_ {
	int mode;			/**< 渲染模式 */
	int width;			/**< 宽度 */
	int height;			/**< 高度 */
	GC gc;				/**< 图形操作上下文 */
	Window window;			/**< 对应的 X11 窗口 */
	XImage *ximage;			/**< 适用于 X11 的图像数据 */
	LCUI_BOOL is_ready;		/**< 标志，标识当前的表面是否已经准备好 */
	LCUI_Graph fb;			/**< 帧缓存，它里面的数据会映射到窗口中 */
	LCUI_Mutex mutex;		/**< 互斥锁 */
	LCUI_SurfaceTasks tasks;
	LinkedList rects;		/**< 列表，记录当前需要重绘的区域 */
	LinkedListNode node;		/**< 在表面列表中的结点 */
} LCUI_SurfaceRec;

static struct X11_Display {
	LCUI_BOOL is_inited;		/**< 标记，标识当前模块是否已经初始化 */
	LinkedList surfaces;		/**< 表面列表 */
	LCUI_X11AppDriver app;		/**< X11 应用驱动 */
	LCUI_EventTrigger trigger;	/**< 事件触发器 */
} x11 = {0};

static void X11Surface_ReleaseTask( LCUI_Surface surface, int type )
{
	LCUI_SurfaceTask task = &surface->tasks[type];
	if( !task->is_valid ) {
		return;
	}
	switch( type ) {
	case TASK_SET_CAPTION:
		free( task->caption );
		task->caption = NULL;
	default: break;
	}
	task->is_valid = FALSE;
}

static LCUI_Surface GetSurfaceByWindow( Window win )
{
	LinkedListNode *node;
	LinkedList_ForEach( node, &x11.surfaces ) {
		if( ((LCUI_Surface)node->data)->window == win ) {
			return node->data;
		}
	}
	return NULL;
}

static void X11Surface_OnResize( LCUI_Surface s, int width, int height )
{
	int depth;
    	XGCValues gcv;
	Visual *visual;
	if( width == s->width && height == s->height ) {
		return;
	}
	if( s->ximage ) {
		XDestroyImage( s->ximage );
		s->ximage = NULL;
	}
	if( s->gc ) {
		XFreeGC( x11.app->display, s->gc );
		s->gc = NULL;
	}
	Graph_Init( &s->fb );
	s->width = width;
	s->height = height;
	depth = DefaultDepth( x11.app->display, x11.app->screen );
	switch( depth ) {
	case 32:
	case 24:
		s->fb.color_type = COLOR_TYPE_ARGB;
		break;
	default: 
		LOG("[x11display] unsupport depth: %d.\n", depth);
		break;
	}
	Graph_Create( &s->fb, width, height );
	visual = DefaultVisual( x11.app->display, x11.app->screen );
    	s->ximage = XCreateImage( x11.app->display, visual, depth, ZPixmap, 
    				  0, (char *)(s->fb.bytes),
                      		  width, height, 32, 0 );
	if( !s->ximage ) {
		Graph_Free( &s->fb );
		LOG("[x11display] create XImage faild.\n");
		return;
	}
    	gcv.graphics_exposures = False;
	s->gc = XCreateGC( x11.app->display, s->window, 
			   GCGraphicsExposures, &gcv );
	if( !s->gc ) {
		LOG("[x11display] create graphics context faild.\n");
		return;
	}
}

static void X11Surface_RunTask( LCUI_Surface surface, int type )
{
	Window win = surface->window;
	Display *dpy = x11.app->display;
	LCUI_SurfaceTask task = &surface->tasks[type];

	switch( type ) {
	case TASK_RESIZE: {
		int w = task->width , h = task->height;
		w = MIN_WIDTH > w ? MIN_WIDTH: w;
		h = MIN_HEIGHT > h ? MIN_HEIGHT: h;
		LCUIMutex_Lock( &surface->mutex );
		X11Surface_OnResize( surface, w, h );
		XResizeWindow( dpy, win, w, h );
		LCUIMutex_Unlock( &surface->mutex );
		break;
	}
	case TASK_MOVE: 
		XMoveWindow( dpy, win, task->x, task->y );
		break;
	case TASK_SHOW:
		if( task->show ) {
			XMapWindow( dpy, win );
		} else {
			XUnmapWindow( dpy, win );
		}
		break;
	case TASK_SET_CAPTION: {
		XTextProperty name;
        	name.value    = (unsigned char*)task->caption;
        	name.encoding = XA_STRING;
        	name.format   = 8 * sizeof(char);
        	name.nitems   = task->caption_len;
        	XSetWMName( dpy, win, &name );
        	break;
        }
	case TASK_DELETE:
		LinkedList_Unlink( &x11.surfaces, &surface->node );
	default: break;
	}
}

static void X11Surface_Delete( LCUI_Surface surface )
{
	surface->tasks[TASK_DELETE].is_valid = TRUE;
}

static void X11Surface_Show( LCUI_Surface surface )
{
	surface->tasks[TASK_SHOW].show = TRUE;
	surface->tasks[TASK_SHOW].is_valid = TRUE;
}

static void X11Surface_Hide( LCUI_Surface surface )
{
	surface->tasks[TASK_SHOW].show = FALSE;
	surface->tasks[TASK_SHOW].is_valid = TRUE;
}

static void X11Surface_OnCreate( void *arg1, void *arg2 )
{
	LCUI_Surface s = arg1;
	unsigned long bdcolor = BlackPixel(x11.app->display, x11.app->screen);
	unsigned long bgcolor = WhitePixel(x11.app->display, x11.app->screen);
	s->window = XCreateSimpleWindow( x11.app->display, x11.app->win_root,
					 100, 100, MIN_WIDTH, MIN_HEIGHT, 1,
					 bdcolor, bgcolor );
	s->is_ready = TRUE;
	LCUI_SetLinuxX11MainWindow( s->window );
	X11Surface_Show( s );
}

static LCUI_Surface X11Surface_New( void )
{
	LCUI_Surface surface;
	surface = NEW( LCUI_SurfaceRec, 1 );
	surface->gc = NULL;
	surface->ximage = NULL;
	surface->is_ready = FALSE;
	surface->node.data = surface;
	surface->width = MIN_WIDTH;
	surface->height = MIN_HEIGHT;
	Graph_Init( &surface->fb );
	LCUIMutex_Init( &surface->mutex );
	LinkedList_Init( &surface->rects );
	surface->fb.color_type = COLOR_TYPE_ARGB;
	LinkedList_AppendNode( &x11.surfaces, &surface->node );
	LCUI_PostSimpleTask( X11Surface_OnCreate, surface, NULL );
	return surface;
}

static LCUI_BOOL X11Surface_IsReady( LCUI_Surface surface )
{
	return surface->is_ready;
}

static void X11Surface_Move( LCUI_Surface surface, int x, int y )
{
	surface->tasks[TASK_MOVE].x = x;
	surface->tasks[TASK_MOVE].y = y;
	surface->tasks[TASK_MOVE].is_valid = TRUE;
}

static void X11Surface_Resize( LCUI_Surface surface, int width, int height )
{
	surface->tasks[TASK_RESIZE].width = width;
	surface->tasks[TASK_RESIZE].height = height;
	surface->tasks[TASK_RESIZE].is_valid = TRUE;
}

static void X11Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *wstr )
{
	int len;
	LCUI_SurfaceTask task;
	task = &surface->tasks[TASK_SET_CAPTION];
	if( wstr ) {
		char *caption;
		len = LCUI_EncodeString( NULL, wstr, 0, ENCODING_UTF8 ) + 1;
		caption = malloc( sizeof(char) * len );
		if( !caption ) {
			return;
		}
		LCUI_EncodeString( caption, wstr, len, ENCODING_UTF8 );
		task->caption = caption;
		task->caption_len = len - 1;
	} else {
		task->caption = NULL;
		task->caption_len = 0;
	}
	task->is_valid = TRUE;
}

void X11Surface_SetOpacity( LCUI_Surface surface, float opacity )
{

}

/** 设置 Surface 的渲染模式 */
static void X11Surface_SetRenderMode( LCUI_Surface surface, int mode )
{
	surface->mode = mode;
}

static LCUI_PaintContext X11Surface_BeginPaint( LCUI_Surface surface, 
						LCUI_Rect *rect )
{
	LCUI_PaintContext paint;
	paint = malloc(sizeof(LCUI_PaintContextRec));
	paint->rect = *rect;
	paint->with_alpha = FALSE;
	Graph_Init( &paint->canvas );
	LCUIMutex_Lock( &surface->mutex );
	LCUIRect_ValidateArea( &paint->rect, surface->width, surface->height );
	Graph_Quote( &paint->canvas, &surface->fb, &paint->rect );
	Graph_FillRect( &paint->canvas, RGB( 255, 255, 255 ), NULL, TRUE );
	return paint;
}

static void X11Surface_EndPaint( LCUI_Surface surface, 
				LCUI_PaintContext paint )
{
	LCUI_Rect *r;
	r = NEW( LCUI_Rect, 1 );
	*r = paint->rect;
	LinkedList_Append( &surface->rects, r );
	free( paint );
	LCUIMutex_Unlock( &surface->mutex );
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
static void X11Surface_Present( LCUI_Surface surface )
{
	LinkedListNode *node;
	LCUIMutex_Lock( &surface->mutex );
	LinkedList_ForEach( node, &surface->rects ) {
		LCUI_Rect *rect = node->data;
		XPutImage( x11.app->display, surface->window,
			   surface->gc, surface->ximage,
			   rect->x, rect->y, rect->x, rect->y,
			   rect->width, rect->height );
	}
	LinkedList_Clear( &surface->rects, free );
	LCUIMutex_Unlock( &surface->mutex );
}

/** 更新 surface，应用缓存的变更 */
static void X11Surface_Update( LCUI_Surface surface )
{
	int i;
	for( i = 0; i < TASK_TOTAL_NUM; ++i ) {
		if( surface->tasks[i].is_valid ) {
			X11Surface_RunTask( surface, i );
			X11Surface_ReleaseTask( surface, i );
		}
	}
}

static int WinDisplay_BindEvent( int event_id, LCUI_EventFunc func, 
				 void *data, void (*destroy_data)(void*) )
{
	return EventTrigger_Bind( x11.trigger, event_id, func, 
				  data, destroy_data );
}

static void* X11Surface_GetHandle( LCUI_Surface s )
{
	return NULL;
}

static int X11Display_GetWidth( void )
{
	Screen *s = DefaultScreenOfDisplay( x11.app->display );
	return XWidthOfScreen( s );
}

static int X11Display_GetHeight( void )
{
	Screen *s = DefaultScreenOfDisplay( x11.app->display );
	return XHeightOfScreen( s );
}

static void OnExpose( LCUI_Event e, void *arg )
{
	LCUI_Rect rect;
	XEvent *ev = arg;
	LCUI_Surface surface;
	LCUI_DisplayEventRec dpy_ev;
	rect.x = ev->xexpose.x;
	rect.y = ev->xexpose.y;
	rect.width = ev->xexpose.width;
	rect.height = ev->xexpose.height;
	surface = GetSurfaceByWindow( ev->xexpose.window );
	if( !surface ) {
		return;
	}
	dpy_ev.type = DET_PAINT;
	dpy_ev.surface = surface;
	dpy_ev.paint.rect = rect;
	EventTrigger_Trigger( x11.trigger, DET_PAINT, &dpy_ev );
}

/** 响应 X11 的 ConfigureNotify 事件，它通常在 x11 窗口位置、尺寸改变时触发 */
static void OnConfigureNotify( LCUI_Event e, void *arg )
{
	XEvent *ev = arg;
	LCUI_DisplayEventRec dpy_ev;
	XConfigureEvent xce = ev->xconfigure;
	LCUI_Surface s = GetSurfaceByWindow( xce.window );
	if( s->width == xce.width && s->height == xce.height ) {
		return;
	}
	dpy_ev.surface = s;
	dpy_ev.type = DET_RESIZE;
	dpy_ev.resize.width = xce.width;
	dpy_ev.resize.height = xce.height;
	X11Surface_OnResize( s, xce.width,xce.height );
	EventTrigger_Trigger( x11.trigger, DET_RESIZE, &dpy_ev );

}

LCUI_DisplayDriver LCUI_CreateLinuxX11DisplayDriver( void )
{
	ASSIGN( driver, LCUI_DisplayDriver );
	strcpy( driver->name, "x11" );
	x11.app = LCUI_GetAppData();
	if( !x11.app ) {
		return NULL;
	}
	driver->getWidth = X11Display_GetWidth;
	driver->getHeight = X11Display_GetHeight;
	driver->create = X11Surface_New;
	driver->destroy = X11Surface_Delete;
	driver->isReady = X11Surface_IsReady;
	driver->show = X11Surface_Show;
	driver->hide = X11Surface_Hide;
	driver->move = X11Surface_Move;
	driver->resize = X11Surface_Resize;
	driver->update = X11Surface_Update;
	driver->present = X11Surface_Present;
	driver->setCaptionW = X11Surface_SetCaptionW;
	driver->setRenderMode = X11Surface_SetRenderMode;
	driver->setOpacity = X11Surface_SetOpacity;
	driver->getHandle = X11Surface_GetHandle;
	driver->beginPaint = X11Surface_BeginPaint;
	driver->endPaint = X11Surface_EndPaint;
	driver->bindEvent = WinDisplay_BindEvent;
	LinkedList_Init( &x11.surfaces );
	LCUI_BindSysEvent( Expose, OnExpose, NULL, NULL );
	LCUI_BindSysEvent( ConfigureNotify, OnConfigureNotify, NULL, NULL );
	x11.trigger = EventTrigger();
	x11.is_inited = TRUE;
	return driver;
}

void LCUI_DestroyLinuxX11DisplayDriver( LCUI_DisplayDriver driver )
{
	
}

#endif
