/* ***************************************************************************
 * linux_x11events.c -- event loop support for linux xwindow.
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
 * linux_x11events.c -- linux 平台的事件驱动支持，基于 xwindow。
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

#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

static LCUI_X11AppDriverRec x11;

void LCUI_SetLinuxX11MainWindow( Window win )
{
	x11.win_main = win;
	XSetWMProtocols( x11.display, win, &x11.wm_delete, 1 );
	XSelectInput( x11.display, win, ExposureMask | KeyPressMask | 
		      ButtonPress | StructureNotifyMask | ButtonReleaseMask |
                      KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
                      PointerMotionMask | Button1MotionMask | 
                      VisibilityChangeMask );
	XFlush( x11.display );
}

static LCUI_BOOL X11_WaitEvent( void )
{
	int fd;
	fd_set fdset;
	struct timeval tv;
	XFlush( x11.display );
	fd = ConnectionNumber( x11.display );
	if( XEventsQueued( x11.display, QueuedAlready ) ) {
		return TRUE;
	}
	FD_ZERO( &fdset );
	FD_SET( fd, &fdset );
	tv.tv_sec = 0;
	tv.tv_usec = 10000;
	if( select(fd + 1, &fdset, NULL, NULL, &tv) == 1 ) {
	    return XPending( x11.display );
	}
	return FALSE;
}

static LCUI_BOOL X11_DispatchEvent( void )
{
	XEvent xevent;
	if( !XEventsQueued( x11.display, QueuedAlready ) ) {
		return FALSE;
	}
	XNextEvent( x11.display, &xevent );
	EventTrigger_Trigger( x11.trigger, xevent.type, &xevent );
	if( xevent.type == ClientMessage ) {
		if( xevent.xclient.data.l[0] == x11.wm_delete ) {
			LCUI_Quit();
		}
	}
	return TRUE;
}

static void X11_ProcessEvents( void )
{
	int i;
	if( !X11_WaitEvent() ) {
		return;
	}
	for( i = 0; X11_DispatchEvent() && i < 100; ++i );
}

static int X11_BindSysEvent( int event_id, LCUI_EventFunc func,
			      void *data, void(*destroy_data)(void*) )
{
	return EventTrigger_Bind( x11.trigger, event_id,
				  func, data, destroy_data );
}

static int X11_UnbindSysEvent( int event_id, LCUI_EventFunc func )
{
	return EventTrigger_Unbind( x11.trigger, event_id, func );
}

static int X11_UnbindSysEvent2( int handler_id )
{
	return EventTrigger_Unbind2( x11.trigger, handler_id );
}

static void *X11_GetData( void )
{
	return &x11;
}

void LCUI_PreInitLinuxX11App( void *data )
{
	return;
}

LCUI_AppDriver LCUI_CreateLinuxX11AppDriver( void )
{
	ASSIGN( app, LCUI_AppDriver );
	x11.display = XOpenDisplay( NULL );
	if( !x11.display ) {
		free( app );
		return NULL;
	}
	x11.screen = DefaultScreen( x11.display );
	x11.win_root = RootWindow( x11.display, x11.screen );
	x11.cmap = DefaultColormap( x11.display, x11.screen );
	x11.wm_delete = XInternAtom( x11.display, "WM_DELETE_WINDOW", FALSE );
	app->ProcessEvents = X11_ProcessEvents;
	app->BindSysEvent = X11_BindSysEvent;
	app->UnbindSysEvent = X11_UnbindSysEvent;
	app->UnbindSysEvent2 = X11_UnbindSysEvent2;
	app->GetData = X11_GetData;
	x11.trigger = EventTrigger();
	return app;
}

void LCUI_DestroyLinuxX11AppDriver( LCUI_AppDriver app )
{
	EventTrigger_Destroy( x11.trigger );
	XCloseDisplay( x11.display );
	x11.trigger = NULL;
	free( app );
}
#endif
