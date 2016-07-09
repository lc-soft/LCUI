
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

static LCUI_X11AppDriverRec x11;

enum AppTaskAgentState {
	STATE_NONE,
	STATE_RUNNING,
	STATE_PAUSE
};

static struct AppTaskAgent {
	int state;
	LinkedList tasks;
	LCUI_Mutex mutex;
	LCUI_Cond cond;
} agent;

void LCUI_SetLinuxX11MainWindow( Window win )
{
	_DEBUG_MSG("set start.\n");
	LCUIMutex_Lock( &agent.mutex );
	agent.state = STATE_PAUSE;
	x11.win_main = win;
	LCUICond_Signal( &agent.cond );
	LCUIMutex_Unlock( &agent.mutex );
	XSelectInput(x11.display, win, ExposureMask | KeyPressMask);
	_DEBUG_MSG("set done.\n");
}

static LCUI_BOOL X11_PostTask( LCUI_AppTask task )
{
	LCUIMutex_Lock( &agent.mutex );
	LinkedList_Append( &agent.tasks, task );
	LCUICond_Signal( &agent.cond );
	LCUIMutex_Unlock( &agent.mutex );
	if( agent.state == STATE_PAUSE && x11.win_main != 0 ) {
		int ret;
		XEvent ev;
		memset( &ev, 0, sizeof (ev) );
		ev.xclient.type = ClientMessage;
		ev.xclient.window = x11.win_main;
		ev.xclient.format = 32;
		ev.xclient.message_type = x11.wm_lcui;
		ret = XSendEvent(x11.display, x11.win_main, FALSE, NoEventMask, &ev);
		return TRUE;
	}
	_DEBUG_MSG("agent mode\n");
	return TRUE;
}

static LCUI_BOOL X11_WaitEvent( void )
{
	int fd;
	fd_set fdset;
	struct timeval zero_time;
	if( agent.tasks.length > 0 ) {
		return TRUE;
	}
	if( agent.state == STATE_RUNNING ) {
		_DEBUG_MSG("agent mode\n");
		LCUIMutex_Lock( &agent.mutex );
		while( agent.state == STATE_RUNNING ) {
			if( agent.tasks.length > 0 ) {
				LCUIMutex_Unlock( &agent.mutex );
				return TRUE;
			}
			LCUICond_Wait( &agent.cond, &agent.mutex );
		}
		LCUIMutex_Unlock( &agent.mutex );
		return FALSE;
	}
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
	LCUI_AppTask task;
	LinkedListNode *node;
	DEBUG_MSG("proc lcui task, length: %d\n", agent.tasks.length);
	LCUIMutex_Lock( &agent.mutex );
	node = LinkedList_GetNode( &agent.tasks, 0 );
	if( node ) {
		task = node->data;
		LinkedList_Unlink( &agent.tasks, node );
		LCUIMutex_Unlock( &agent.mutex );
		LCUI_RunTask( task );
		LCUI_DeleteTask( task );
		free( task );
		free( node );
		return;
	} else {
		LCUIMutex_Unlock( &agent.mutex );
	}
	if( agent.state == STATE_RUNNING ) {
		return;
	}
	DEBUG_MSG("waiting xevent\n");
	XNextEvent( x11.display, &xevent );
	DEBUG_MSG("waitting is done\n");
	DEBUG_MSG("%d\n", xevent.type);
	switch( xevent.type ) {
	case ClientMessage: 
		DEBUG_MSG("message_type: %ld, wm_lcui: %ld\n", xevent.xclient.message_type, x11.wm_lcui);
	default: break;
	}
	DEBUG_MSG("done\n");
}

static void X11_PumpEvents( void )
{
	while( X11_WaitEvent() ) {
		DEBUG_MSG("get event\n");
		X11_DispatchEvent();
	}
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
	x11.wm_lcui = XInternAtom(x11.display, "WM_DELETE_WINDOW", FALSE);
	XSetWMProtocols(x11.display, x11.win_root, &x11.wm_lcui, 1);
	app->WaitEvent = X11_WaitEvent;
	app->PumbEvents = X11_PumpEvents;
	app->PostTask = X11_PostTask;
	app->BindSysEvent = X11_BindSysEvent;
	app->UnbindSysEvent = X11_UnbindSysEvent;
	app->UnbindSysEvent2 = X11_UnbindSysEvent2;
	app->GetData = X11_GetData;
	agent.state = STATE_RUNNING;
	LinkedList_Init( &agent.tasks );
	LCUIMutex_Init( &agent.mutex );
	LCUICond_Init( &agent.cond );
	x11.trigger = EventTrigger();
	return 0;
}

#endif
