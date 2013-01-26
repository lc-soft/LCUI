#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include <unistd.h>

static LCUI_Queue events;
static BOOL active = FALSE;
static thread_t eventloop_thread = -1;

void LCUI_EventsInit( void )
{
	Queue_Init( &events, sizeof(LCUI_Event), NULL );
}

void LCUI_DestroyEvents( void )
{
	Destroy_Queue( &events );
}

BOOL LCUI_PollEvent( LCUI_Event *event )
{
	LCUI_Event *tmp;
	
	if( !active ) {
		return FALSE;
	}
	
	Queue_Lock( &events );
	tmp = Queue_Get( &events, 0 );
	if( !tmp ) {
		Queue_UnLock( &events );
		return FALSE;
	}
	*event = *tmp;
	Queue_Delete( &events, 0 );
	Queue_UnLock( &events );
	return TRUE;
}

static void *LCUI_EventLoop( void *unused )
{
	LCUI_Event event;
	int delay_time = 1500;
	
	while( active ) {
		if( LCUI_PollEvent( &event ) ) {
			delay_time = 1500;
			switch( event.type ) {
			case LCUI_KEYDOWN:
				
				break;
			case LCUI_MOUSEMOTION:
				LCUI_HandleMouseMotion( &event );
				break;
			case LCUI_MOUSEBUTTONDOWN:
				LCUI_HandleMouseButtonDown( &event );
				break;
			case LCUI_MOUSEBUTTONUP:
				LCUI_HandleMouseButtonUp( &event );
				break;
			case LCUI_USEREVENT:
				
				break;
			}
		} else {
			if( delay_time <= 15000 ) {
				delay_time += 1500;
			}
			usleep( delay_time );
		}
	}
	thread_exit( NULL );
}

void LCUI_StopEventThread( void )
{
	if( !active ) {
		return;
	}
	active = FALSE;
	thread_join( eventloop_thread, NULL );
}

int LCUI_StartEventThread( void )
{
	LCUI_StopEventThread();
	active = TRUE;
	return thread_create(	&eventloop_thread, NULL, 
				LCUI_EventLoop, NULL );
}

BOOL LCUI_PushEvent( LCUI_Event *event )
{
	if( !active ) {
		return FALSE;
	}
	Queue_Lock( &events );
	if(Queue_Add( &events, event ) < 0) {
		Queue_UnLock( &events );
		return FALSE;
	}
	Queue_UnLock( &events );
	return TRUE;
}

