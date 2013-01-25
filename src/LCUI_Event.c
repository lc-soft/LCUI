#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_EVENT_H

void AppEventQueue_Init( LCUI_App *app )
{
	Queue_Init( &app->events, sizeof(LCUI_Event), NULL );
}

BOOL _LCUI_PollEvent( LCUI_App *app, LCUI_Event *event )
{
	LCUI_Event *tmp;
	
	if( !app ) {
		printf("%s(): %s", __FUNCTION__, APP_ERROR_UNRECORDED_APP );
	}
	Queue_Lock( &app->events );
	tmp = Queue_Get( &app->events, 0 );
	if( !tmp ) {
		Queue_UnLock( &app->events );
		return FALSE;
	}
	*event = *tmp;
	Queue_Delete( &app->events, 0 );
	Queue_UnLock( &app->events );
	return TRUE;
}

BOOL LCUI_PollEvent( LCUI_Event *event )
{
	return _LCUI_PollEvent( Get_Self_AppPointer(), event );
}

BOOL _LCUI_PushEvent( LCUI_App *app, LCUI_Event *event )
{
	if( !app ) {
		printf("%s(): %s", __FUNCTION__, APP_ERROR_UNRECORDED_APP );
	}
	Queue_Lock( &app->events );
	if(Queue_Add( &app->events, event ) < 0) {
		Queue_UnLock( &app->events );
		return FALSE;
	}
	Queue_UnLock( &app->events );
	return TRUE;
}

BOOL LCUI_PushEvent( LCUI_Event *event )
{
	return _LCUI_PushEvent( Get_Self_AppPointer(), event );
}
