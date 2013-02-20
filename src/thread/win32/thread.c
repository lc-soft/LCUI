#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_THREAD_H

#ifdef LCUI_THREAD_WIN32

typedef struct _LCUI_ThreadData {
	HANDLE handle;
	unsigned int tid;
	void *retval;
	void (*func)(void*);
	void *arg;
} LCUI_ThreadData;

static LCUI_BOOL db_init = FALSE;
static LCUI_Queue thread_database;

static unsigned __stdcall run_thread(void *arg)
{
	LCUI_ThreadData *thread;
	thread = (LCUI_ThreadData*)arg;
	thread->func( thread->arg );
	return 0;
}

int _LCUIThread_Create( LCUI_Thread *thread, void(*func)(void*), void *arg )
{
	LCUI_Thread *thread_ptr;
	if(!db_init) {
		db_init = TRUE;
		Queue_Init(&thread_database, sizeof(LCUI_ThreadData), NULL);
	}
	thread_ptr = malloc(sizeof(LCUI_ThreadData));
	thread_ptr->func = func;
	thread_ptr->arg = arg;
	thread_ptr->retval = NULL;
	thread_ptr->handle = (HANDLE)_beginthreadex( NULL, 0, run_thread, 
					thread_ptr, 0, &thread_ptr->tid );
	if( thread_ptr->handle == 0 ) {
		*thread = 0;
		return -1;
	}
	Queue_Add_Pointer( &thread_database, thread_ptr );
	*thread = thread_ptr->tid;
	return 0;
}

static LCUI_ThreadData *_LCUIThread_Self( void )
{
	int i, n;
	unsigned int tid;
	LCUI_ThreadData *thread_data;
	
	tid = GetCurrentThreadId();
	n = Queue_Get_Total( &thread_database );
	for(i=0; i<n; ++i) {
		thread_data = Queue_Get( &thread_database, i );
		if( !thread_data || thread_data->tid != tid ) {
			continue;
		}
		return thread_data;
	}
	return NULL;
}

static int _LCUIThread_Destroy( LCUI_Thread thread )
{
	int i, n;
	LCUI_ThreadData *thread_data;
	
	if(!thread){
		return -1;
	}
	n = Queue_Get_Total( &thread_database );
	for(i=0; i<n; ++i) {
		thread_data = Queue_Get( &thread_database, i );
		if( !thread_data || thread != thread_data->tid ) {
			continue;
		}
		Queue_Delete( &thread_database, i );
		return 0;
	}
	return -2;
}

LCUI_Thread LCUIThread_SelfID( void )
{
	return GetCurrentThreadId();
}

void _LCUIThread_Exit( void *retval )
{
	LCUI_ThreadData *thread;
	thread = _LCUIThread_Self();
	if( !thread ) {
		return;
	}
	thread->retval = retval;
}

void _LCUIThread_Cancel( LCUI_Thread thread )
{
	LCUI_ThreadData *data_ptr;
	data_ptr = _LCUIThread_Self();
	TerminateThread( data_ptr->handle, FALSE );
	_LCUIThread_Destroy( data_ptr );
}

int _LCUIThread_Join( LCUI_Thread thread, void **retval )
{
	LCUI_ThreadData *data_ptr;
	data_ptr = _LCUIThread_Self();
	WaitForSingleObject( data_ptr->handle, INFINITE );
	CloseHandle( data_ptr->handle );
	if( retval ) {
		*retval = data_ptr->retval;
	}
	return _LCUIThread_Destroy( data_ptr );
}
#endif
