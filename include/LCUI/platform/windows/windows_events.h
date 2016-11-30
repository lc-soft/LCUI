#ifndef LCUI_WINDOWS_EVENTS_H
#define LCUI_WINDOWS_EVENTS_H

LCUI_API void LCUI_PreInitWinApp( void *data );

void LCUI_SetMainWindow( HWND hwnd );

LCUI_AppDriver LCUI_CreateWinAppDriver( void );

void LCUI_DestroyWinAppDriver( LCUI_AppDriver app );

#endif
