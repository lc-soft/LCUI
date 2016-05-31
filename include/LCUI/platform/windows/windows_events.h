#ifndef LCUI_WINDOWS_EVENTS_H
#define LCUI_WINDOWS_EVENTS_H

typedef struct WIN_SysEvent_ {
	HWND hwnd;
	WPARAM wparam;
	LPARAM lparam;
	UINT msg;
	void *data;
} WIN_SysEventRec, *WIN_SysEvent;

LCUI_API void LCUI_PreInitWinApp( void *data );

int LCUI_InitWinApp( LCUI_AppDriver app );

#endif
