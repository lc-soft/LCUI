#ifndef __WINDOWS_EVENTS_H__
#define __WINDOWS_EVENTS_H__

typedef struct WIN_SysEvent_ {
	HWND hwnd;
	WPARAM wparam;
	LPARAM lparam;
	UINT msg;
	void *data;
} WIN_SysEventRec, *WIN_SysEvent;

void LCUI_PreInitWinApp( void *data );

int LCUI_InitWinApp( LCUI_AppDriver app );

#endif
