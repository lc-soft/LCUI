
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
	XSelectInput( x11.display, win, ExposureMask | KeyPressMask );
	LCUI_SetTaskAgent( FALSE );
}

static LCUI_BOOL X11_PostTask( LCUI_AppTask task )
{
	XEvent ev;
	memset( &ev, 0, sizeof (ev) );
	ev.xclient.type = ClientMessage;
	ev.xclient.window = x11.win_main;
	ev.xclient.format = 32;
	ev.xclient.message_type = x11.wm_lcui;
	XSendEvent(x11.display, x11.win_main, FALSE, NoEventMask, &ev);
	return TRUE;
}

static LCUI_BOOL X11_WaitEvent( void )
{
	int fd;
	fd_set fdset;
	struct timeval zero_time;
	XFlush( x11.display );
	fd = ConnectionNumber( x11.display );
	if( XEventsQueued(x11.display, QueuedAlready) ) {
		return TRUE;
	}
	FD_ZERO( &fdset );
	FD_SET( fd, &fdset );
	if( select(fd + 1, &fdset, NULL, NULL, &zero_time) == 1 ) {
	    return XPending( x11.display );
	}
	return FALSE;
}

static void X11_DispatchEvent( void )
{
	XEvent xevent;
	XNextEvent( x11.display, &xevent );
	DEBUG_MSG("%d\n", xevent.type);
	switch( xevent.type ) {
	case ClientMessage: 
	default: break;
	}
	EventTrigger_Trigger( x11.trigger, xevent.type, &xevent );
}

static int X11_BindSysEvent( int event_id, LCUI_EventFunc func,
			      void *data, void(*destroy_data)(void*) )
{
	return EventTrigger_Bind( x11.trigger, event_id, func, data, destroy_data );
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

int LCUI_InitLinuxX11App( LCUI_AppDriver app )
{
	x11.display = XOpenDisplay(NULL);
	if( !x11.display ) {
		return -1;
	}
	x11.screen = DefaultScreen( x11.display );
	x11.win_root = RootWindow( x11.display, x11.screen );
	x11.cmap = DefaultColormap( x11.display, x11.screen );
	x11.wm_lcui = XInternAtom( x11.display, "WM_LCUI", FALSE );
	XSetWMProtocols( x11.display, x11.win_root, &x11.wm_lcui, 1 );
	app->WaitEvent = X11_WaitEvent;
	app->DispatchEvent = X11_DispatchEvent;
	app->PostTask = X11_PostTask;
	app->BindSysEvent = X11_BindSysEvent;
	app->UnbindSysEvent = X11_UnbindSysEvent;
	app->UnbindSysEvent2 = X11_UnbindSysEvent2;
	app->GetData = X11_GetData;
	x11.trigger = EventTrigger();
	return 0;
}

#endif
