#include <LCUI_Build.h>
#if defined(LCUI_BUILD_IN_LINUX)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

static struct X11Driver {
	int screen;
	Display *display;
	Window rootwin;
	Colormap cmap;
	LCUI_EventTrigger trigger;
} x11;

static LCUI_BOOL X11_PostTask( LCUI_AppTask task )
{
	return FALSE;
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

static void X11_PumpEvents( void )
{

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
	return NULL;
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
	x11.rootwin = RootWindow( x11.display, x11.screen );
	x11.cmap = DefaultColormap( x11.display, x11.screen );
	app->WaitEvent = X11_WaitEvent;
	app->PumbEvents = X11_PumpEvents;
	app->PostTask = X11_PostTask;
	app->BindSysEvent = X11_BindSysEvent;
	app->UnbindSysEvent = X11_UnbindSysEvent;
	app->UnbindSysEvent2 = X11_UnbindSysEvent2;
	app->GetData = X11_GetData;
	x11.trigger = EventTrigger();
	return 0;
}
#endif
