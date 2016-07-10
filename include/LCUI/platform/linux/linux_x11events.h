#ifndef LCUI_LINUX_X11_EVENTS_H
#define LCUI_LINUX_X11_EVENTS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

typedef struct LCUI_X11AppDriverRec_ {
	int screen;
	Display *display;
	Window win_root;
	Window win_main;
	Colormap cmap;
	Atom wm_lcui;
	LCUI_EventTrigger trigger;
} LCUI_X11AppDriverRec, *LCUI_X11AppDriver;

void LCUI_SetLinuxX11MainWindow( Window win );

void LCUI_PreInitLinuxX11App( void *data );

int LCUI_InitLinuxX11App( LCUI_AppDriver app );

#endif
