/* ***************************************************************************
 * linux_x11display.c -- surface support for linux xwindow.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
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

#define MIN_WIDTH 50
#define MIN_HEIGHT 50

enum SurfaceTaskType {
	TASK_CREATE,
	TASK_MOVE,
	TASK_RESIZE,
	TASK_SHOW,
	TASK_SET_CAPTION,
	TASK_DELETE,
	TASK_TOTAL_NUM
};

typedef struct LCUI_SurfaceTaskRec_ {
	int type;
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

typedef struct LCUI_SurfaceRec_ {
	int mode;
	int w, h;
	GC gc;
	Window window;
	XImage ximage;
	LCUI_BOOL is_ready;
	LCUI_Graph fb;
	LinkedListNode node;
} LCUI_SurfaceRec;

static struct X11_Display {
    	XVisualInfo vinfo;
	LCUI_BOOL is_inited;
	LinkedList surfaces;
	LCUI_X11AppDriver app;
	LCUI_EventTrigger trigger;
} x11;

LCUI_Surface main_surface;

static void ReleaseSurfaceTask( void *arg )
{
	LCUI_SurfaceTask task = arg;
	if( task->type == TASK_SET_CAPTION ) {
		free( task->caption );
		task->caption = NULL;
	}
}

static void X11Surface_OnCreate( LCUI_Surface s )
{
    	XGCValues gcv;
    	gcv.graphics_exposures = False;
    	unsigned long bdcolor = BlackPixel(x11.app->display, x11.app->screen);
	unsigned long bgcolor = WhitePixel(x11.app->display, x11.app->screen);
	s->window = XCreateSimpleWindow( x11.app->display, x11.app->win_root, 
					 0, 100, MIN_WIDTH, MIN_HEIGHT, 1, 
					 bdcolor, bgcolor );
	s->gc = XCreateGC( x11.app->display, s->window, 
			   GCGraphicsExposures, &gcv );
	if( !s->gc ) {
		printf("[x11display] create graphics context faild.\n");
		return;
	}
	s->is_ready = TRUE;
	main_surface = s;
	LCUI_SetLinuxX11MainWindow( s->window );
}

static void X11Surface_OnTask( LCUI_Surface surface, LCUI_SurfaceTask task )
{
	Window win = surface->window;
	Display *dpy = x11.app->display;

	switch( task->type ) {
	case TASK_CREATE: 
		X11Surface_OnCreate( surface );
		break;
	case TASK_RESIZE: {
		int w = task->width , h = task->height;
		w = MIN_WIDTH > w ? MIN_WIDTH: w;
		h = MIN_HEIGHT > h ? MIN_HEIGHT: h;
		XResizeWindow( dpy, win, w, h );
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
	default: break;
	}
}

static void X11Surface_SendTask( LCUI_Surface surface, LCUI_SurfaceTask task )
{
	LCUI_AppTaskRec apptask;
	LCUI_SurfaceTask newtask;
	newtask = NEW( LCUI_SurfaceTaskRec, 1 );
	*newtask = *task;
	apptask.arg[0] = surface;
	apptask.arg[1] = newtask;
	apptask.destroy_arg[0] = NULL;
	apptask.destroy_arg[1] = ReleaseSurfaceTask;
	apptask.func = (LCUI_AppTaskFunc)X11Surface_OnTask;
	LCUI_PostTask( &apptask );
}

static void X11Surface_Delete( LCUI_Surface surface )
{
	LCUI_SurfaceTaskRec task;
	task.type = TASK_DELETE;
	LinkedList_Unlink( &x11.surfaces, &surface->node );
	X11Surface_SendTask( surface, &task );
}

static LCUI_Surface X11Surface_New( void )
{
	LCUI_Surface surface;
	LCUI_SurfaceTaskRec task;
	task.type = TASK_CREATE;
	surface = NEW( LCUI_SurfaceRec, 1 );
	surface->is_ready = FALSE;
	surface->node.data = surface;
	Graph_Init( &surface->fb );
	surface->fb.color_type = COLOR_TYPE_ARGB;
	LinkedList_AppendNode( &x11.surfaces, &surface->node );
	X11Surface_SendTask( surface, &task );
	_DEBUG_MSG("create surface: %p\n", surface);
	return surface;
}

static LCUI_BOOL X11Surface_IsReady( LCUI_Surface surface )
{
	return surface->is_ready;
}

static void X11Surface_Move( LCUI_Surface surface, int x, int y )
{
	LCUI_SurfaceTaskRec task;
	task.type = TASK_MOVE;
	task.x = x;
	task.y = y;
	X11Surface_SendTask( surface, &task );
}

static void X11Surface_Resize( LCUI_Surface surface, int width, int height )
{
	LCUI_SurfaceTaskRec task;
	task.type = TASK_RESIZE;
	task.width = width;
	task.height = height;
	X11Surface_SendTask( surface, &task );
	_DEBUG_MSG("resize: %d, %d\n", width, height);
}

static void X11Surface_Show( LCUI_Surface surface )
{
	LCUI_SurfaceTaskRec task;
	task.type = TASK_SHOW;
	task.show = TRUE;
	X11Surface_SendTask( surface, &task );
}

static void X11Surface_Hide( LCUI_Surface surface )
{
	LCUI_SurfaceTaskRec task;
	task.type = TASK_SHOW;
	task.show = FALSE;
	X11Surface_SendTask( surface, &task );
}

static void X11Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *wstr )
{
	int len;
	LCUI_SurfaceTaskRec task;

	if( wstr ) {
		char *caption;
		len = LCUI_EncodeString( NULL, wstr, 0, ENCODING_UTF8 ) + 1;
		caption = malloc( sizeof(char) * len );
		if( !caption ) {
			return;
		}
		LCUI_EncodeString( caption, wstr, len, ENCODING_UTF8 );
		task.caption = caption;
		task.caption_len = len - 1;
	} else {
		task.caption = NULL;
		task.caption_len = 0;
	}
	task.type = TASK_SET_CAPTION;
	X11Surface_SendTask( surface, &task );
}

void X11Surface_SetOpacity( LCUI_Surface surface, float opacity )
{

}

/** 设置 Surface 的渲染模式 */
static void X11Surface_SetRenderMode( LCUI_Surface surface, int mode )
{
	surface->mode = mode;
}

/**
* 准备绘制 Surface 中的内容
* @param[in] surface	目标 surface
* @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
* @return		返回绘制上下文句柄
*/
static LCUI_PaintContext X11Surface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	return NULL;
	/***
	LCUI_PaintContext paint;
	paint = malloc(sizeof(LCUI_PaintContextRec));
	paint->rect = *rect;
	paint->with_alpha = FALSE;
	Graph_Init( &paint->canvas );
	LCUIRect_ValidateArea( &paint->rect, surface->w, surface->h );
	Graph_Quote( &paint->canvas, &surface->fb, &paint->rect );
	Graph_FillRect( &paint->canvas, RGB( 255, 255, 255 ), NULL, TRUE );
	return paint;***/
}

/**
* 结束对 Surface 的绘制操作
* @param[in] surface	目标 surface
* @param[in] paint_ctx	绘制上下文句柄
*/
static void X11Surface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx )
{
	free( paint_ctx );
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
static void X11Surface_Present( LCUI_Surface surface )
{

}

/** 更新 surface，应用缓存的变更 */
static void X11Surface_Update( LCUI_Surface surface )
{
	return;
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

int LCUI_InitLinuxX11Display( LCUI_DisplayDriver driver )
{
	strcpy( driver->name, "x11" );
	x11.app = LCUI_GetAppData();
	driver->getWidth = X11Display_GetWidth;
	driver->getHeight = X11Display_GetHeight;
	driver->new = X11Surface_New;
	driver->delete = X11Surface_Delete;
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
	x11.trigger = EventTrigger();
	x11.is_inited = TRUE;
	return 0;
}

int LCUI_ExitLinuxX11Display( void )
{
	// ...
	return 0;
}

#endif
